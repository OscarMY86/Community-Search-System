import json

with open('./grp/BFS_Only_topk.txt', 'r') as file:
    bfs_topk_data = file.read()
node_list = json.loads(bfs_topk_data)
num_nodes_bfs_topk = len(node_list)
print("Number of nodes in BFS_Only_topk.txt:", num_nodes_bfs_topk)

with open('./grp/rmapper.txt', 'r') as file:
    rmapper_data = file.read()
mapping_dict = json.loads(rmapper_data)
remapped_node_list = [str(mapping_dict[str(node)]) for node in node_list]

with open('./grp/subgraph.txt', 'r') as file:
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

with open('./grp/final_result.txt', 'w') as file:
    file.writelines(filtered_edges)

num_nodes_final_result = len(set([line.split()[0] for line in filtered_edges] + [line.split()[1] for line in filtered_edges]))
print("Number of nodes in final_result.txt:", num_nodes_final_result)