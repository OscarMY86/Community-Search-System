running jar file

java -jar EqiTruss.jar o GraphFileName PathForIndex

-o is the option 
	1- index creation: create the index from the given graph file and write the index files to the given path
	2- community search: read index files from the given path and ask for a query node and a truss value to search its community
	
-GraphFileName: file name which includes the graph edge list, e.g. toyg.txt

-PathForIndex: path for directory to read or write index files 

e.g. java -jar EqiTruss.jar 1 ../../data/AstroPh/graph.txt astro
	//this will create index files for toy graph
	
	java -jar EqiTruss.jar 2 ../../data/AstoPh/graph.txt astro
	//this will read index files for toy graph and ask for a query node 
