import json
import subprocess
import os
from graphviz import Digraph
from concurrent.futures import ProcessPoolExecutor
import sys


def read_response_file(response_file_path, build_dir):
    """Reads the contents of a response file (.rsp) and returns it as a string."""
    # Command also removes '@' at the beginning
    full_path = os.path.join(build_dir, response_file_path[1:])
    with open(full_path, "r") as file:
        # Response files can contain flags in a newline-separated list; join them into a single string
        contents = file.read().replace("\n", " ")
    return contents


def modify_command(command, build_dir):
    """Modifies the given command to include the -H flag and handles response files."""
    parts = command.split()
    modified_parts = []
    for part in parts:
        if part.startswith("@"):
            # Read and substitute response file contents
            rsp_contents = read_response_file(part, build_dir)
            modified_parts.append(rsp_contents)
        else:
            modified_parts.append(part)

    # Replace build output with -H to generate header inclusion tree stream
    if "-o" in modified_parts:
        index = modified_parts.index("-o") - 1
        modified_parts.insert(index, "-H")  # Insert -H flag before the output flag
    return " ".join(modified_parts)


def process_compile_command(build_dir, output_file_path, modified_command):
    """Process a single compile command entry in parallel."""
    # Execute the command, redirecting output to the .dep file
    print(f"Prepared to process: {output_file_path}")
    with open(output_file_path, "w") as outfile:
        subprocess.run(
            modified_command, shell=True, stdout=outfile, stderr=outfile, cwd=build_dir
        )
    return f"Processed: {output_file_path}"


def generate_dependency_trees(project_root, build_dir, output_dir):
    """Generates dependency trees for each source file"""
    # Grab compiler output json file
    compile_commands_path = os.path.join(build_dir, "compile_commands.json")
    if not os.path.exists(compile_commands_path):
        print(
            f"Error: {compile_commands_path} does not exist. Make sure to run CMake with -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
        )
        return False

    # output_dir = os.path.join(project_root, 'dependency_tree')
    os.makedirs(output_dir, exist_ok=True)  # Ensure the output directory exists

    with open(compile_commands_path, "r") as file:
        compile_commands = json.load(file)

    # Build out folder structure first
    pool_exec_entry = []
    for entry in compile_commands:
        # Calculate the file's path relative to the project root
        relative_path = os.path.relpath(entry["file"], start=project_root)
        # Construct the .dep file's path, ensuring it's within the dependencies_trees directory
        output_file_path = os.path.join(output_dir, relative_path + ".dep")
        # Ensure the directory structure for this output file exists
        output_file_dir = os.path.dirname(output_file_path)
        os.makedirs(output_file_dir, exist_ok=True)

        modified_command = modify_command(entry["command"], build_dir)
        pool_exec_entry.append((build_dir, output_file_path, modified_command))

    # Use ProcessPoolExecutor to parallelize the processing
    with ProcessPoolExecutor() as executor:
        futures = [
            executor.submit(process_compile_command, *pool_entry)
            for pool_entry in pool_exec_entry
        ]
        for future in futures:
            print(future.result())

    print("Dependency trees generated.")
    return True


def parse_dep_file(dep_file_path):
    """Parses a .dep file and returns a list of includes, ignoring non-hierarchical sections."""
    with open(dep_file_path, "r") as file:
        lines = file.readlines()

    # Process lines to extract include hierarchy
    includes = []
    include_guards_section = False
    for line in lines:
        # Check if we've reached the include guards section
        if "Multiple include guards may be useful for:" in line:
            include_guards_section = True
            continue  # Skip processing this line and the rest in the include guards section

        if not include_guards_section and line.strip():
            depth = len(line) - len(line.lstrip("."))
            # DOT standards don't allow some characters such as colon
            file_path = line.strip().lstrip(".").strip().replace(":","-")
            includes.append((depth, file_path))


    return includes


def generate_graph_for_dep_file(dep_file_path):
    """Create heiarchical graph to display include tree"""
    # Directory of the .dep file
    dep_dir = os.path.dirname(dep_file_path)
    # Isolate filename
    dep_filename = os.path.basename(dep_file_path)
    # Remove .dep from file name
    base_filename, _ = os.path.splitext(dep_filename)
    # Save location to place graph alongside .dep file
    graph_filename = os.path.join(dep_dir, base_filename)

    # read inclusion tree text file
    includes = parse_dep_file(dep_file_path)
    if not includes:
        print(f"No includes found in {dep_filename}, skipping.")
        return

    graph = Digraph(comment="Include Dependency Graph", format="svg")
    graph.attr(rankdir="LR")

    # Create subgraph for top-level includes to ensure they are at the top
    with graph.subgraph() as s:
        s.attr(rank='source')
        s.attr("node", shape="box", style="filled", fillcolor="lightgrey")

        # First, add all top-level nodes to the subgraph
        for depth, include_file in includes:
            if depth == 1:  # Identify top-level includes
                node_name = include_file
                s.node(node_name, label=include_file)
    node_stack = []
    # Process and add all other includes, creating edges based on their depth
    for depth, include_file in includes:
        node_name = include_file
        # Account for zeros based indexing and parent node
        stack_depth = depth - 2
        child_entry_depth = depth - 1 
        # Ignore top level nodes
        if depth != 1:
            graph.node(node_name, label=include_file)
        else:
            node_stack = []
        
        if depth == 1:
            # Setup parent node
            node_stack.append(node_name)
        else:
            # Keep updating child nodes
            if depth > len(node_stack):
                node_stack.append(node_name)
            else:
                node_stack[child_entry_depth] = node_name
            graph.edge(node_stack[stack_depth], node_stack[child_entry_depth])

    # Specify output file for the graph (SVG format)
    output_path = graph.render(filename=graph_filename, view=False, cleanup=True)
    print(f"Graph generated: {output_path}")


def generate_graphs(proj_root_dir, build_dir, output_dir):
    """Wrapped call to generate all outputs"""

    # if not generate_dependency_trees(proj_root_dir, build_dir, output_dir):
    #     return
    # Create visual output
    for root, dirs, files in os.walk(output_dir):
        for file in files:
            if file.endswith(".dep"):
                dep_file_path = os.path.join(root, file)
                generate_graph_for_dep_file(dep_file_path)
    print("Dependency tree graphs generated.")

"""
@NOTE This script does not handle symbolic links as declared in the .dep file outputs.
Headers may appear to include them selves although that CAN be a symbolic linkage to that header file.

@NOTE It is always best to read the .dep file for the concrete result of what the compiler actually operates with during compile time.
"""
if __name__ == "__main__":
    # Ensure that compiler is executed with CMAKE_EXPORT_COMPILE_COMMANDS=ON
    if len(sys.argv) < 3:
        print(
            "Usage:"
            "python gen_dep_tree.py ../../my_code_project ../../build"
            "or"
            "python gen_dep_tree.py ../../my_code_project ../../build ./include_trees"
        )
    else:
        # Define your project's build directory where compile_commands.json is generated
        PROJECT_ROOT_DIR = sys.argv[1]
        # Define where the project build output directory exists at
        BUILD_DIR = sys.argv[2]
        # Define the output directory for the dependency trees
        OUTPUT_DIR = sys.argv[3] if len(sys.argv) > 3 else "./dependency_tree"

    generate_graphs(PROJECT_ROOT_DIR, BUILD_DIR, OUTPUT_DIR)
