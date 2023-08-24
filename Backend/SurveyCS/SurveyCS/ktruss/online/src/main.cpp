#include "graph_IO.h"
#include <iostream>
#include <sys/stat.h>

using namespace std;



set<int> S;



int main(int argc, char* argv[]){

	//Tsize=5;
	struct timeval tpstart, tpend;
	double timeuse = 0;
	//ofstream sout(argv[1],ios::out|ios::app|ios::binary);



	char* maindir=argv[1];
	char graphdata[100];
	strcpy(graphdata,maindir);
	strcat(graphdata,"graph.txt");
	char comoutput[100];
	strcpy(comoutput,maindir);
	strcat(comoutput,"trusscom/");

	if(mkdir(comoutput,0777)!=0)
	{
		cout<<"Output folder created!"<<endl;
	} else {
		cout<<"Failed to create output folder."<<endl;
	}

	string genfile=std::string(maindir)+"truss_query.csv";
	ofstream gout(genfile,ios::out|ios::app|ios::binary);
	gout<<"node id"<<","<<"query k"<<","<<"community number"<<","<<"running time(ms)"<<endl;
	gout.close();


	cout<<"Graph loading..."<<endl;
	Readin_Graph(graphdata);

	char infile[100];
	strcpy(infile,maindir);
	strcat(infile,"truss_querynodes.txt");
	FILE *fin = fopen(infile, "r");
	int x,y;
	while(fscanf(fin, "%d %d", &x, &y)!=EOF)
	{
		cout<<"community search: id= "<<x<<", k="<<y<<endl;
		int v=x;
		int k=y;
		double total_t;
		clock_t start_t, end_t;
		start_t = clock();
		memset(truss,k,(m/2)* sizeof(int));
		memset(sup,0,(m/2)* sizeof(int));
		truss_decompostion_K(k);
		ktrussCommunitySearch(v,k);
		end_t = clock();
		total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC*1000;
		Print_ktrussCommunty(maindir,comoutput,v,k,total_t);

		delete [] del;
	}


	return 0;
}

