import json

def convert_to_json(txt_file_path):
    with open(txt_file_path, 'r') as file:
        txt_data = file.read()

    lines = txt_data.split('\n')
    data = []

    # Process each line and convert it to JSON format
    for line in lines:
        line = line.strip()
        if line:
            values = line.split()
            if len(values) == 2:
                source, target = values
                entry = {'source': source.strip(), 'target': target.strip()}
                data.append(entry)
            

    # Convert the data to JSON
    with open('result.json', 'w') as file:
        json_data = json.dumps(data)
        file.write(json_data)


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