#This program is the online approach for k-core community search
#The input files should be contain in a folder including:
#1.graph file: graph.txt
#2.querynode file: core_querynode.txt |format each line: querynode k |example: 201 5
#The program will output the query results of each querynode to a file with the same name as the querynode id (i.e. 201.txt), and the output is the edge list of each community and will end with a -1 symbol.

#To compile the project:
g++ -std=c++11 src/main.cpp -o kccom

#The program input includes one paremeter: The data folder described above.
#To run the program:
./kccom ../../data/Astroph/
