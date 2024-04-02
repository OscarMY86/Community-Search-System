import json

def load_names(file_path):
    id_to_name = {}
    with open(file_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        for line in lines:
            name, id = line.strip().rsplit(' ', 1)
            id_to_name[id] = name
    return id_to_name

def convert_to_json(edges_file):
    id_to_name = load_names('./raw/name2id.txt')
    nodes = {}
    links = []
    with open('./grp/queue_input.txt', 'r') as f:
        main_node = f.readline().strip()
    with open(edges_file, 'r') as f:
        for line in f:
            source, target = line.strip().split(' ')
            if source not in nodes:
                nodes[source] = {'id': source, 'name': id_to_name.get(source, ''), 'group': 0 if source == main_node else 1}
            if target not in nodes:
                nodes[target] = {'id': target, 'name': id_to_name.get(target, ''), 'group': 0 if target == main_node else 1}
            links.append({'source': source, 'target': target})
    json_data = {'nodes': list(nodes.values()), 'links': links}

    with open('./grp/gnn_result.json', 'w') as f:
        json.dump(json_data, f, indent=4)