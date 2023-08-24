#This program is the online approach for k-ecc community search
#The input files should be contain in a folder including:

#1.graph file: graph.txt

#2.querynode file: query_ecc.txt 
#format:
#first line: number of querynodes | example: 5
#each line: k querynode |example: 1 201


#The program will output the query results of each querynode to a file with the same order as the querynode id (i.e. 1.txt), and the ouput is the node list of the community
#The query time of each query will also output in the console.

#To complie the project:
g++ -std=c++14 -O3 src/*.cpp -o kecc

#The program input includes one paremeter: The data folder described above.
#The format of the graph.txt should be processed using the following command:
./run graph-ecc Astroph/

#The program can then be run with three parameters: 1.search method, 2.data folder, 3. output file prefix
./kecc smcc-naive ../../data/Astroph/ query_ecc

#To further process the community nodelist to edgelist with 4 parameters: 1.gen-graph method name, 2. data folder, 3.community nodelist file prefix, 4.number of querynodes:
./run gen-graph ../../data/Astroph/ output_ecc_ 5 
