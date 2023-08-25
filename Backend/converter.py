import json

def convert_to_json(txt_file_path):
    with open(txt_file_path, 'r') as file:
        txt_data = file.read()

    lines = txt_data.split('\n')
    data = []

    # Process each line and convert it to JSON format
    for line in lines:
        pair = line.split()
        if len(pair) == 2:
            data.append({
                'source': pair[0],
                'target': pair[1]
            })

    # Convert the data to JSON
    json_data = json.dumps(data)

    return json_data

# with open('./SurveyCS/SurveyCS/data/Astroph/trusscom/495623.txt', 'r') as file:
#     lines = file.readlines()

# data = []

# # Process each line and create a list of dictionaries
# for line in lines:
#     values = line.split()
#     if len(values) == 2:
#         data.append({
#             'source': int(values[0]),
#             'target': int(values[1])
#         })

# # Write the data to a .json file
# with open('./testset/result.json', 'w') as file:
#     json.dump(data, file)