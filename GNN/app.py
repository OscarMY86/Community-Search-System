from flask import Flask, request, jsonify
import json
import subprocess
from src.converter import convert_to_json
from flask_cors import CORS

app = Flask(__name__)
CORS(app)
def generate_result(option, graph_id):
    queue_input_file = './grp/queue_input.txt'
    rmapper_file = './grp/rmapper.txt'
    subgraph_file = './grp/subgraph.txt'
    result_file = './grp/final_result.txt'

    with open(queue_input_file, 'w') as file:
        file.write(str(graph_id))

    cmd = [
        "python3",
        "main.py",
        "--data-set",
        "dblp",
        "--community-size",
        "30"
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        return "Cannot generate result"

    if option == "BFSO":
        graph_data_file = './grp/BFS_Only_topk.txt'
    elif option == "BFSS":
        graph_data_file = './grp/BFS_Swap_topk.txt' 
    elif option == "GreedyT":
        graph_data_file = './grp/GreedyT_topk.txt'
    elif option == "GreedyG":
        graph_data_file =  './grp/GreedyG_topk.txt'
    else:
        return "Invalid option"

    with open(graph_data_file, 'r') as file:
        graph_data = file.read()
    node_list = json.loads(graph_data)
    num_nodes_graph_data = len(node_list)
    print("Number of nodes in the graph data:", num_nodes_graph_data)

    with open(rmapper_file, 'r') as file:
        rmapper_data = file.read()
    mapping_dict = json.loads(rmapper_data)
    remapped_node_list = [str(mapping_dict[str(node)]) for node in node_list]

    with open(subgraph_file, 'r') as file:
        subgraph_data = file.readlines()
    with open('./grp/m.txt', 'r') as file:
        m_data = file.readlines()

    m_mapping_dict = {}
    for line in m_data:
        real_node, mapped_node = line.split()
        m_mapping_dict[mapped_node] = real_node

    filtered_edges = []
    for line in subgraph_data:
        src, dest = line.split()
        if src in remapped_node_list and dest in remapped_node_list:
            filtered_edges.append(line.replace(src, m_mapping_dict[src]).replace(dest, m_mapping_dict[dest]))

    with open(result_file, 'w') as file:
        file.writelines(filtered_edges)
    num_nodes_final_result = len(set([line.split()[0] for line in filtered_edges] + [line.split()[1] for line in filtered_edges]))
    print("Number of nodes in final_result.txt:", num_nodes_final_result)

    return "Result generated successfully"


@app.route('/', methods=['GET'])
def index():
    return "Backend for GNN is running"

@app.route('/search', methods=['GET'])
def handle_generate_result():
    search_name = request.args.get('name')
    option = request.args.get('option')
    edges_file = './grp/final_result.txt'

    if option is None or search_name is None:
        return jsonify({'error': 'Invalid request. Missing option or graph_id.'}), 400

    def name_id(name):
        with open('./raw/name2id.txt', 'r', encoding='utf-8') as file:
            for line in file:
                parts = line.split()
                if ' '.join(parts[:-1]) == name:
                    return int(parts[-1])
        return -1

    id = name_id(search_name)
    if id == -1:
        return jsonify({'error': 'Invalid search name.'}), 400

    result = generate_result(option, id)
    convert_to_json(edges_file)
    with open('./grp/gnn_result.json') as json_file:
        json_data = json.load(json_file)
    return jsonify(json_data)

if __name__ == '__main__':
    app.run(debug=True, port=8081, host="0.0.0.0")