from flask import Flask, request, jsonify
from flask_cors import CORS
import json
import subprocess
import converter

app = Flask(__name__)
CORS(app)

@app.route('/')
def index():
    return "Backend server is running"

@app.route('/test', methods = {'GET'})
def test():

    ### Same from search.py ###
    search_name = request.args.get('name')
    k = request.args.get('k')
    method = request.args.get('option')

    def name_id(name):
        with open('name2id.txt', 'r', encoding='utf-8') as file:
            for line in file:
                parts = line.split()
                if ' '.join(parts[:-1]) == name:
                    return int(parts[-1])
        return -1

    id = name_id(search_name)
    if id == -1:
        return None

    with open("search.txt", "w") as f:
        f.write("Id: " + str(id) + "\n" + "Name: " + search_name + "\n" + "k-value: " + str(k) + "\n" + "Method: " + method )

    txt_file_path = f'./SurveyCS/SurveyCS/data/Astroph/trusscom/{id}.txt'
    json_data = converter.convert_to_json(txt_file_path)

    # with open('./testset/test3.json') as json_file:
    #     data = json.load(json_file)
    return jsonify(json_data)

@app.route('/search', methods=['GET'])
def search():
    search_name = request.args.get('name')
    k = request.args.get('k')
    method = request.args.get('option')
    txt_file_path = ''
    # json_file_path = './testset/result.json'

    def name_id(name):
        with open('name2id.txt', 'r', encoding='utf-8') as file:
            for line in file:
                parts = line.split()
                if ' '.join(parts[:-1]) == name:
                    return int(parts[-1])
        return -1

    id = name_id(search_name)
    if id == -1:
        return None

    with open("search.txt", "w") as f:
        f.write("Id: " + str(id) + "\n" + "Name: " + search_name + "\n" + "k-value: " + str(k) + "\n" + "Method: " + method )

    ###### Write the input id and k-value to 4 files ######

    if (method == "clique" or method == "ecc"):
        return None

    if (method == "core"):
        with open("./SurveyCS/SurveyCS/data/Astroph/core_querynodes.txt", "w") as f:
            f.write(str(id) + " " + str(k) + "\n")
        command = ['./SurveyCS/SurveyCS/kcore/online/kccom', './SurveyCS/SurveyCS/data/Astroph/']
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        txt_file_path = f'./SurveyCS/SurveyCS/data/Astroph/corecom/{id}.txt'
        converter.convert_to_json(txt_file_path)

    if (method == "truss"):
        with open("./SurveyCS/SurveyCS/data/Astroph/truss_querynodes.txt", "w", encoding='utf-8') as f:
            f.write(str(id) + " " + str(k) + "\n")
        command = ['./SurveyCS/SurveyCS/ktruss/online/ktcom', './SurveyCS/SurveyCS/data/Astroph/']
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        txt_file_path = f'./SurveyCS/SurveyCS/data/Astroph/trusscom/{id}.txt'
        converter.convert_to_json(txt_file_path)

    with open('./testset/result.json') as json_file:
        json_data = json.load(json_file)
    return jsonify(json_data)

if __name__ == '__main__':
    app.run(debug=True, port=5000, host="0.0.0.0")