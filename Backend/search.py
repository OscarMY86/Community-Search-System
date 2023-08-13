import sys
import subprocess

def name_id(name):
    with open('../DBLP-Geneartion/dataset/name2id.txt', 'r') as file:
        for line in file:
            parts = line.split()
            if ' '.join(parts[:-1]) == name:
                return int(parts[-1])
    return -1


search_name = input("Enter a search name: ")
k = int(input("Enter number K: "))

id = name_id(search_name)
if (id == -1):
    print("Unfound name")
    sys.exit()

# Open the file in write mode
with open("search.txt", "w") as f:
    f.write(str(id) + " " + search_name + " " + str(k) + "\n")

# with open("../SurveyCS/SurveyCS/data/Astroph/core_querynodes.txt", "w") as f:
#     f.write(str(id) + " " + str(k) + "\n")

with open("../SurveyCS/SurveyCS/data/Astroph/truss_querynodes.txt", "w") as f:
    f.write(str(id) + " " + str(k) + "\n")

# with open("../SurveyCS/SurveyCS/data/Astroph/query1.txt", "w") as f:
#     f.write(str(id) + " " + str(k) + "\n")

# with open("../SurveyCS/SurveyCS/data/Astroph/query_ecc.txt", "w") as f:
#     f.write(str(id) + " " + str(k) + "\n")

# Run the k-truss algorithm
# Path: "/SurveyCS/SurveyCS/ktruss/online" /home/oscar/Desktop/fyp/SurveyCS/SurveyCS/ktruss/online/ktcom
command = ['../SurveyCS/SurveyCS/ktruss/online/ktcom', '../SurveyCS/SurveyCS/data/Astroph/']
result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

if result.returncode == 0:
    output = result.stdout.decode('utf-8')
    print("Command executed successfully.")
    print("Output:")
    print(output)
else:
    error = result.stderr.decode('utf-8')
    print("Command execution failed.")
    print("Error:")
    print(error)