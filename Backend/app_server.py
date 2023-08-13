from flask import Flask, request, jsonify
import subprocess

app = Flask(__name__)

@app.route('/search', methods=['POST'])
def search():
    data = request.get_json()
    search_name = data['name']
    k = data['k']

    def name_id(name):
        with open('../DBLP-Geneartion/dataset/name2id.txt', 'r') as file:
            for line in file:
                parts = line.split()
                if ' '.join(parts[:-1]) == name:
                    return int(parts[-1])
        return -1

    id = name_id(search_name)
    if id == -1:
        return jsonify({'error': 'Unfound name'})

    with open("search.txt", "w") as f:
        f.write(str(id) + " " + search_name + " " + str(k) + "\n")

    with open("../SurveyCS/SurveyCS/data/Astroph/core_querynodes.txt", "w") as f:
        f.write(str(id) + " " + str(k) + "\n")

    with open("../SurveyCS/SurveyCS/data/Astroph/truss_querynodes.txt", "w") as f:
        f.write(str(id) + " " + str(k) + "\n")

    with open("../SurveyCS/SurveyCS/data/Astroph/query1.txt", "w") as f:
        f.write(str(id) + " " + str(k) + "\n")

    with open("../SurveyCS/SurveyCS/data/Astroph/query_ecc.txt", "w") as f:
        f.write(str(id) + " " + str(k) + "\n")

    command = ['../SurveyCS/SurveyCS/ktruss/online/ktcom', '../SurveyCS/SurveyCS/data/Astroph/']
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if result.returncode == 0:
        output = result.stdout.decode('utf-8')
        return jsonify({'output': output})
    else:
        error = result.stderr.decode('utf-8')
        return jsonify({'error': error}), 500

if __name__ == '__main__':
    app.run()