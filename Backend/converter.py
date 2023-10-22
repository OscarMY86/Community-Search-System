import json
import os

def convert_to_json(txt_file_path):

    # with open(txt_file_path, 'r') as file:
    #     txt_data = file.read()

    # lines = txt_data.split('\n')
    # data = []

    # # Process each line and convert it to JSON format
    # for line in lines:
    #     line = line.strip()
    #     if line:
    #         values = line.split()
    #         if len(values) == 2:
    #             source, target = values
    #             entry = {'source': source.strip(), 'target': target.strip()}
    #             data.append(entry)
            

    # # Convert the data to JSON
    # if os.path.exists('result.json'):
    #     os.remove('result.json')
    #     with open('result.json', 'w') as file:
    #         json_data = json.dumps(data)
    #         file.write(json_data)
    # else:
    #     with open('result.json', 'w') as file:
    #         json_data = json.dumps(data)
    #         file.write(json_data)
    


    # Ths version is for grouping
    community = 1
    nodes = {}
    links = []

    # Open the edges.txt file and read line by line
    with open(txt_file_path, 'r') as f:
        for line in f:

            parts = line.split()

            # If the line is not '-1', process the nodes and the edge
            if parts[0] != '-1':
                # Get the nodes ids
                node1, node2 = parts[0], parts[1]

                # Add the nodes to the nodes dictionary
                if node1 not in nodes:
                    nodes[node1] = community
                if node2 not in nodes:
                    nodes[node2] = community

                # Add the edge to the links list
                links.append({'source': node1, 'target': node2})
            else:
                # If the line is '-1', increment the community number
                community += 1

    nodes_list = [{'id': id, 'group': group} for id, group in nodes.items()]
    final_dict = {'nodes': nodes_list, 'links': links}

    # with open('edges.json', 'w') as f:
    #     json.dump(final_dict, f, indent=4)

    if os.path.exists('result.json'):
        os.remove('result.json')
        with open('result.json', 'w') as f:
            json.dump(final_dict, f, indent=4)
    else:
        with open('result.json', 'w') as f:
            json.dump(final_dict, f, indent=4)