#This program is the online approach for k-clique community search
#The input files should be contain in a folder including:
#1.graph file: graph.txt
#2.querynode file: query1.txt |format each line: querynode |example: 201
#The program will output the query results of each querynode to a file with the same order as the querynode id (i.e. 1.txt), and the output is the node list of the community
#The query time of each query will also output in the console.

#To compile the project:
g++ -std=c++14 -O3 src/*.cpp -o kclique

#The program input includes one parameter: The data folder described above.
#The format of the graph.txt should be processed using the following command:
./run graph-cp ../../data/Astroph/

#The program can then be run with three parameters: 1.search method, 2.data folder, 3. output file prefix # change is, run this 
./kclique ../../data/Astroph/ b

#To further process the community nodelist to edgelist with 4 parameters: 1.gen-graph method name, 2. data folder, 3.community nodelist file prefix, 4.number of querynodes: # change is, run this 
./run gen-graph ../../dataAstroph/ output_clique_ 5 
