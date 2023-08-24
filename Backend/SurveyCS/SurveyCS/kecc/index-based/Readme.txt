#This program is the index-based approach for k-ecc community search
#The input files should be contain in a folder including:
#1.graph file: graph.txt
#2.querynode file: same as online method

#To complie the project:
g++ -std=c++14 -O3 src/*.cpp -o kecc

#The program input includes one paremeter: The data folder described above.
#The format of the graph.txt should be processed using the following command:
./run graph-ecc ../../data/Astroph/

#The program should first build the index using the following command:
./kecc all-sc ../../data/Astroph/ bottom-up output

#The query step can then be run with three parameters: 1.search method, 2.data folder, 3. output file prefix
./kecc smcc-mSPT ../../data/Astroph/ qurey_ecc
