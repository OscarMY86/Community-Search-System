from flask import Flask, request, jsonify
from flask_cors import CORS
# import subprocess

app = Flask(__name__)
CORS(app)

@app.route('/')
def index():
    return "Backend server is running"

@app.route('/search', methods=['GET'])
def search():
    search_name = request.args.get('name')
    k = request.args.get('k')
    method = request.args.get('option')

    def name_id(name):
        with open('../DBLP-Geneartion/dataset/name2id.txt', 'r', encoding='utf-8') as file:
            for line in file:
                parts = line.split()
                if ' '.join(parts[:-1]) == name:
                    return int(parts[-1])
        return -1

    id = name_id(search_name)
    if id == -1:
        return jsonify({'error': 'Unfound name'})

    with open("search.txt", "w") as f:
        f.write("Id: " + str(id) + "\n" + "Name: " + search_name + "\n" + "k-value: " + str(k) + "\n" + "Method: " + method )


    ###### Write the input id and k-value to 4 files ######
    # with open("../SurveyCS/SurveyCS/data/Astroph/core_querynodes.txt", "w") as f:
    #     f.write(str(id) + " " + str(k) + "\n")

    with open("../SurveyCS/SurveyCS/data/Astroph/truss_querynodes.txt", "w", encoding='utf-8') as f:
        f.write(str(id) + " " + str(k) + "\n")

    # with open("../SurveyCS/SurveyCS/data/Astroph/query1.txt", "w") as f:
    #     f.write(str(id) + " " + str(k) + "\n")

    # with open("../SurveyCS/SurveyCS/data/Astroph/query_ecc.txt", "w") as f:
    #     f.write(str(id) + " " + str(k) + "\n")


    ###### Run the Algorithm ######
    # command = ['../SurveyCS/SurveyCS/ktruss/online/ktcom', '../SurveyCS/SurveyCS/data/Astroph/']
    # result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # if result.returncode == 0:
    #     output = result.stdout.decode('utf-8')
    #     return jsonify({'output': output})
    # else:
    #     error = result.stderr.decode('utf-8')
    #     return jsonify({'error': error}), 500

    return jsonify({'status': 'success'})

if __name__ == '__main__':
    app.run(debug=True, port=5000)