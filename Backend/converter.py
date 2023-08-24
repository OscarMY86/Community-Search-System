import json

txt_file_path = './SurveyCS/SurveyCS/data\Astroph/trusscom/1077429.txt'
json_file_path = './testset/test2.json'

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

# Save the JSON data to a file
with open(json_file_path, 'w') as file:
    file.write(json_data)