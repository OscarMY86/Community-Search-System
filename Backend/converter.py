# Limit the community size at most n, every community should contain only at most community_size = n
import json
import os
from collections import deque, defaultdict

def load_names(file_path):
    id_to_name = {}
    with open(file_path, 'r', encoding='utf-8' ) as f:
        lines = f.readlines()
        for line in lines:
            name, id = line.strip().rsplit(' ', 1)
            id_to_name[id] = name
    return id_to_name

def bfs(source, edges):
   neighbor_distance = {node: float('inf') for node in edges}
   neighbor_distance[source] = 0
   queue = deque([source])
   while queue:
       node = queue.popleft()
       for neighbor in edges[node]:
           if neighbor_distance[node] + 1 < neighbor_distance[neighbor]:
               neighbor_distance[neighbor] = neighbor_distance[node] + 1
               queue.append(neighbor)
   return neighbor_distance


def convert_to_json(txt_file_path, name_file_path,limit):
    id_to_name = load_names(name_file_path)
    community = 1
    nodes = {}
    links = []
    community_size = 0
    temp_links = []
    edges = defaultdict(list)
    main_node = None
    if limit is not None:
        limit = int(limit)
    with open(txt_file_path, 'r') as f:
        for line in f:
            parts = line.split()
            if parts[0] != '-1':
                node1, node2 = parts[0], parts[1]
                if main_node is None:
                    main_node = node1
                temp_links.append({'source': node1, 'target': node2})
                edges[node1].append(node2)
                edges[node2].append(node1)
                
                if community_size < limit:
                    if node1 not in nodes:
                        name = id_to_name.get(node1, 'Unknown')
                        neighbor_distance = bfs(main_node, edges).get(node1, 0)
                        neighbor_number = len(edges[node1])
                        nodes[node1] = {'name': name, 'group': 0 if node1 == main_node else community, 'neighbor_number': neighbor_number, 'neighbor_distance': neighbor_distance}
                    if node2 not in nodes and community_size < limit:
                        name = id_to_name.get(node2, 'Unknown')
                        neighbor_distance = bfs(main_node, edges).get(node2, 0)
                        neighbor_number = len(edges[node2])
                        nodes[node2] = {'name': name, 'group': 0 if node2 == main_node else community, 'neighbor_number': neighbor_number, 'neighbor_distance': neighbor_distance}
                        community_size += 1
            else: 
                community += 1
                community_size = 0

    for node in nodes:
        nodes[node]['neighbor_number'] = len(edges[node])
        nodes[node]['neighbor_distance'] = bfs(main_node, edges)[node] # added

    # Filter out the links that are not between the nodes in the final node list
    for link in temp_links:
        if link['source'] in nodes and link['target'] in nodes:
            links.append(link)

    nodes_list = [{'id': id, 'name': data['name'], 'group': data['group'], 'neighbor_number': data['neighbor_number'], 'neighbor_distance': data['neighbor_distance']} for id, data in nodes.items()]
    final_dict = {'nodes': nodes_list, 'links': links}

    if os.path.exists('./testset/result.json'):
        os.remove('./testset/result.json')
    with open('./testset/result.json', 'w') as f:
        json.dump(final_dict, f, indent=4)
    
    if os.path.exists('./data/result.json'):
        os.remove('./data/result.json')
    with open('./data/result.json', 'w') as f:
        json.dump(final_dict, f, indent=4)