from flask import Flask, request, jsonify, make_response,send_file
from flask_cors import CORS
import json
import subprocess
import converter
import os
import shutil
app = Flask(__name__)
CORS(app)

@app.route('/')
def index():
    return "Backend server is running"

@app.route('/save', methods=['GET'])
def save():
    search_name = request.args.get('name')
    k = request.args.get('k')
    method = request.args.get('option')
    limit = request.args.get('limit')
    file_name = f"{search_name}_{k}_{method}_{limit}.json"
    source_file_path = os.path.join("testset", "result.json")
    target_file_path = os.path.join("testset", file_name)

    try:
        shutil.copyfile(source_file_path, target_file_path)
        return send_file(target_file_path, as_attachment=True)
    except Exception as e:
        return f"Error: {e}"
    
@app.route('/search', methods=['GET'])
def search():
    search_name = request.args.get('name')
    k = request.args.get('k')
    method = request.args.get('option')
    limit = request.args.get('limit')
    txt_file_path = ''
    name_file_path = './name2id.txt'
    file_name = f"{search_name}_{k}_{method}_{limit}.json"
    file_path = os.path.join("testset", file_name)
    if os.path.exists(file_path):
        with open(file_path, 'r') as file:
            json_data = json.load(file)
        return jsonify(json_data)
    def name_id(name):
        with open('name2id.txt', 'r', encoding='utf-8') as file:
            for line in file:
                parts = line.split()
                if ' '.join(parts[:-1]) == name:
                    return int(parts[-1])
        return -1

    id = name_id(search_name)
    if id == -1:
        return make_response("Invalid name", 400)

    with open("search.txt", "w") as f:
        f.write("Id: " + str(id) + "\n" + "Name: " + search_name + "\n" + "k-value: " + str(k) + "\n" + "Method: " + method )

    ###### Write the input id and k-value ######

    if (method == "clique" or method == "ecc"):
        return make_response("Invalid method", 400)

    if (method == "core"):
        with open("./SurveyCS/SurveyCS/data/Astroph/core_querynodes.txt", "w") as f:
            f.write(str(id) + " " + str(k) + "\n")
        command = ['./SurveyCS/SurveyCS/kcore/online/kccom', './SurveyCS/SurveyCS/data/Astroph/']
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        txt_file_path = f'./SurveyCS/SurveyCS/data/Astroph/corecom/{id}.txt'
        converter.convert_to_json(txt_file_path, name_file_path,limit)
        os.remove(txt_file_path)

    if (method == "truss"):
        with open("./SurveyCS/SurveyCS/data/Astroph/truss_querynodes.txt", "w", encoding='utf-8') as f:
            f.write(str(id) + " " + str(k) + "\n")
        command = ['./SurveyCS/SurveyCS/ktruss/online/ktcom', './SurveyCS/SurveyCS/data/Astroph/']
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        txt_file_path = f'./SurveyCS/SurveyCS/data/Astroph/trusscom/{id}.txt'
        converter.convert_to_json(txt_file_path, name_file_path,limit)
        os.remove(txt_file_path)

    with open('./testset/result.json') as json_file:
        json_data = json.load(json_file)
    return jsonify(json_data)

if __name__ == '__main__':
    app.run(debug=True, port=5000, host="0.0.0.0")
    
