import os
from graphviz import Digraph
import hashlib
import sys

GRAPH_STYLE = {"rankdir": "LR", "nodesep": "1", "ranksep": "20"}


def parse_d_file(d_file_path):
    """
    Parses a single .d file to extract dependencies and the source directory.
    Returns dependencies and source file.
    """
    dependencies = set()
    source_file = ""
    try:
        with open(d_file_path, "r") as file:
            # Manage .d pathing outputs
            content = file.read().replace("\\\n", "").replace("\\", "/")
            # Read all dependencies listed in file
            parts = content.split(":", 1)
            if len(parts) > 1:
                source_file = os.path.normpath(parts[0].strip())
                deps = parts[1].split()
                for dep in deps:
                    dep_cleaned = os.path.normpath(dep.strip())
                    if dep_cleaned.endswith((".h", ".hpp")):
                        dependencies.add(dep_cleaned)
    except Exception as e:
        print(f"Error reading {d_file_path}: {e}")
    return (
        dependencies,
        source_file,
    )


def generate_cluster_key(relative_dir):
    """
    Generate unique names for cluster outputs
    """
    # This example uses the path itself to generate a hash
    hash_object = hashlib.sha256(relative_dir.encode())
    hex_dig = hash_object.hexdigest()[:8]  # Shorten the hash for brevity
    base_name = os.path.basename(relative_dir)
    cluster_key = f"cluster_{base_name}_{hex_dig}"
    return cluster_key


def generate_graphs_with_structure(dep_files_dir, output_path, ignore_dir=None):
    """
    Produces project wide dependency graphs at all levels
    """
    # Setup root output graph
    overall_graph = Digraph(
        comment="Overall Dependency Graph", engine="dot", format="svg"
    )
    overall_graph.attr(**GRAPH_STYLE)
    overall_graph.attr("node", shape="record", style="filled", fillcolor="lightgrey")
    overall_graph.attr("edge", color="black", arrowhead="vee")

    # Prepare to map all clusters in project
    clusters = {}

    # Parse all .d files to fill the clusters with dependencies
    for root, dirs, files in os.walk(dep_files_dir):
        for file in files:
            if file.endswith(".d"):
                d_file_path = os.path.join(root, file)
                if ignore_dir and ignore_dir in d_file_path:
                    continue  # Skip files in the ignored directory
                # Read available .d files
                dependencies, source_file = parse_d_file(d_file_path)
                relative_dir = os.path.relpath(root, dep_files_dir)
                cluster_key = generate_cluster_key(relative_dir)
                # Setup cluster entry
                if cluster_key not in clusters:
                    clusters[cluster_key] = {
                        "graph": Digraph(name=cluster_key, engine="dot", format="svg"),
                        "path": relative_dir,
                        "files": [],
                    }
                    clusters[cluster_key]["graph"].attr(**GRAPH_STYLE)
                    clusters[cluster_key]["graph"].attr(
                        "node", shape="record", style="filled", fillcolor="lightgrey"
                    )
                    clusters[cluster_key]["graph"].attr(
                        "edge", color="black", arrowhead="vee"
                    )

                # Store source file and its dependencies for processing later
                clusters[cluster_key]["files"].append((source_file, dependencies))

    render_dir_cluster_file_graph(clusters, output_path, overall_graph)

    # Render the overall graph at the root of the output directory
    overall_graph_file = os.path.join(output_path, "overall_dependency_graph")
    overall_graph.render(filename=overall_graph_file, cleanup=True)
    print(f"Overall dependency graph generated at {overall_graph_file}.svg")


def render_dir_cluster_file_graph(clusters, output_path, overall_graph):
    """
    Generates directory specific dependency graph for all files in directory
    """
    # Process each source file and cluster
    for cluster_key, cluster_info in clusters.items():
        output_dir = os.path.join(output_path, cluster_info["path"])
        # Create output dirs
        os.makedirs(output_dir, exist_ok=True)

        # Process and render individual source file graphs
        for source_file, dependencies in cluster_info["files"]:
            render_source_file_graph(
                source_file, dependencies, output_path, overall_graph
            )

        # Now, generate and render the cluster graph
        # Reset attributes for the cluster graph to ensure fresh start
        cluster_graph = cluster_info["graph"]
        cluster_graph.clear()  # Clear any previous content, if necessary
        cluster_graph.attr(**GRAPH_STYLE)
        cluster_graph.attr(
            "node", shape="record", style="filled", fillcolor="lightgrey"
        )
        cluster_graph.attr("edge", color="black", arrowhead="vee")

        # Add nodes and edges for all files within the cluster
        for source_file, dependencies in cluster_info["files"]:
            source_label = os.path.basename(source_file)
            cluster_graph.node(source_label)
            for dep in dependencies:
                dep_label = os.path.basename(dep)
                cluster_graph.node(dep_label)
                cluster_graph.edge(source_label, dep_label)

        # Render and save the cluster graph
        cluster_output_file = os.path.join(output_dir, f"{cluster_key}")
        cluster_graph.render(filename=cluster_output_file, cleanup=True)
        print(f"Cluster graph generated at {cluster_output_file}")


def render_source_file_graph(source_file, dependencies, output_dir, overall_graph):
    """
    Generates individual source graph files and adds ontop project wide dependency graph
    """
    file_graph = Digraph(name=os.path.basename(source_file), engine="dot", format="svg")
    file_graph.attr(**GRAPH_STYLE)
    file_graph.attr("node", shape="record", style="filled", fillcolor="lightgrey")
    file_graph.attr("edge", color="black", arrowhead="vee")

    source_label = os.path.basename(source_file)
    file_graph.node(source_label)
    # Add to overall graph
    overall_graph.node(source_label)
    for dep in dependencies:
        dep_label = os.path.basename(dep)
        file_graph.node(dep_label)
        file_graph.edge(source_label, dep_label)
        # Duplicate nodes in overall graph are fine
        overall_graph.node(dep_label)
        # Add edge to overall graph
        overall_graph.edge(source_label, dep_label)

    # Render and save the individual source file graph
    file_name = os.path.splitext(source_file)[0]
    file_graph.render(filename=os.path.join(output_dir, file_name), cleanup=True)
    print(
        f"Graph generated for {source_file} at {os.path.join(output_dir, file_name)}.svg"
    )


if __name__ == "__main__":
    # If you encounter a file save error, verify your systems file length name max characters allowance
    # Ensure that compiler is executed with a -MMD or -MD options to generate .d (dependency files)
    if len(sys.argv) < 3:
        print(
            "Usage: python dep_flat_tree.py DEP_FILES_DIR OUTPUT_PATH [IGNORE_DIR]"
            "python dep_flat_tree.py ../../../build ./right_here"
            "or"
            "python dep_flat_tree.py ../../../build ./right_here ../../../build/thirdparty"
        )
    else:
        BUILD_ROOT_DIR = sys.argv[1]
        OUTPUT_DIR = sys.argv[2]
        IGNORE_DIR = sys.argv[3] if len(sys.argv) > 3 else None
        generate_graphs_with_structure(BUILD_ROOT_DIR, OUTPUT_DIR, IGNORE_DIR)
