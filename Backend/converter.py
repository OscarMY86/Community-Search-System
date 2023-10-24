# import json
# import os

# def convert_to_json(txt_file_path):
#     community = 1
#     nodes = {}
#     links = []

#     # Open the edges.txt file and read line by line
#     with open(txt_file_path, 'r') as f:
#         for line in f:
#             parts = line.split()
#             if parts[0] != '-1':
#                 node1, node2 = parts[0], parts[1]
#                 if node1 not in nodes:
#                     nodes[node1] = community
#                 if node2 not in nodes:
#                     nodes[node2] = community

#                 links.append({'source': node1, 'target': node2})
#             else: 
#                 community += 1

#     nodes_list = [{'id': id, 'group': group} for id, group in nodes.items()]
#     final_dict = {'nodes': nodes_list, 'links': links}

#     if os.path.exists('result.json'):
#         os.remove('result.json')
#         with open('result.json', 'w') as f:
#             json.dump(final_dict, f, indent=4)
#     else:
#         with open('result.json', 'w') as f:
#             json.dump(final_dict, f, indent=4)


# Limit the community size at most n, every community should contain only at most community_size = n
import json
import os

def convert_to_json(txt_file_path):
    community = 1
    nodes = {}
    links = []
    community_size = 0
    temp_links = []

    with open(txt_file_path, 'r') as f:
        for line in f:
            parts = line.split()
            if parts[0] != '-1':
                node1, node2 = parts[0], parts[1]
                temp_links.append({'source': node1, 'target': node2})
                
                if community_size < 100:
                    if node1 not in nodes:
                        nodes[node1] = community
                        community_size += 1
                    if node2 not in nodes and community_size < 100:
                        nodes[node2] = community
                        community_size += 1
            else: 
                community += 1
                community_size = 0

    # Filter out the links that are not between the nodes in the final node list
    for link in temp_links:
        if link['source'] in nodes and link['target'] in nodes:
            links.append(link)

    nodes_list = [{'id': id, 'group': group} for id, group in nodes.items()]
    final_dict = {'nodes': nodes_list, 'links': links}

    if os.path.exists('./testset/result.json'):
        os.remove('./testset/result.json')
        with open('./testset/result.json', 'w') as f:
            json.dump(final_dict, f, indent=4)
    else:
        with open('./testset/result.json', 'w') as f:
            json.dump(final_dict, f, indent=4)
