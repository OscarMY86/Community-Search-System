#include "Utility.h"
#include "Graph.h"
#include "ConnGraph.h"

void parse_graph(char *input_name, char *output_name) ;
void parse_graph_gr(char *input_name, char *output_name) ;
void transform_edge_to_binary(char *dir) ;
void display_usage() ;


void gen_sc(char *dir) {
	Graph *graph = new Graph(dir);
	graph->read_graph(1);
	graph->all_SC_BU();
	string output_name = string(dir) + "/cg.bin";
	FILE *fout = open_file(output_name.c_str(), "wb");
	graph->output_all_sc(fout);
	fclose(fout);
	graph->max_spanning_tree();
	delete graph;
}

void smcc_naive(char *dir) {
	Graph *cg = new Graph(dir);
	cg->read_graph(1);
	cg->smcc_naive("query_ecc");
	delete cg;
}

void smcc_mspt(char *dir) {
	ConnGraph *cg = new ConnGraph(dir);
	cg->read_cg(1);
	cg->read_mSPT();
	cg->smcc_query_mSPT("query_ecc");
	delete cg;
}



int main(int argc, char *argv[]) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	//parse_graph("data/soc-LiveJournal1.txt", "data/edges.txt");
	//return 0;
	srand(time(NULL));

	//printf("RAND_MAX: %d\n", RAND_MAX);

	//char *input[100] = {"", "process-update-query", "data/CA-GrQc", "update-0", "output"};
	//char *input[100] = {"", "", "data/Amazon0601", "bottom-up", "output"};
	//char *input[100] = {"", "smccl-mSPT", "data/CA-GrQc", "smccl-query-10", "2"};
	//char *input[100] = {"", "gen-smccl-query", "data/CA-GrQc", "10", "output"};
	//char *input[100] = {"", "check-conn", "data/CA-GrQc", "log.txt"};
	//char *input[100] = {"", "transform-edge-to-binary", "data/CA-GrQc"};
	//argc = 5; argv = input;

#ifdef _LINUX_
	struct timeval start, end;
	gettimeofday(&start, NULL);
#else
	int start, end;
	start = clock();
#endif

	if(argc < 2) {
		//display_usage();
		char path[] = "/Users/lqin/Documents/workspace/data/Twitter/";
		//char path[] = "/Users/lqin/Documents/workspace/data/test_ecc/";
		//transform_edge_to_binary(path);
		//gen_sc(path);
		//smcc_naive(path);
		smcc_mspt(path);
		return 0;
	}

	int binary_input = 0;

	if(strcmp(argv[1], "process-update-query") == 0) { // only process steiner component query
		if(argc < 4) {
			display_usage();
			return 0;
		}
		ConnGraph *cg = new ConnGraph(argv[2]);
		cg->read_cg(binary_input);
		cg->read_mSPT();
		cg->mark_nontree();
		cg->check_parallel_edge();
		cg->process_update_query(argv[3]);
		delete cg;
	}
	else if(strcmp(argv[1], "gen-smcc-query") == 0) { // gen smcc query
		if(argc < 4) {
			display_usage();
			return 0;
		}
		ConnGraph *cg = new ConnGraph(argv[2]);
		cg->read_mSPT();
		cg->gen_random_smcc_query(argv[3]);
		delete cg;
	}
	else if(strcmp(argv[1], "gen-smccl-query") == 0) { // gen smcc query
		if(argc < 4) {
			display_usage();
			return 0;
		}
		ConnGraph *cg = new ConnGraph(argv[2]);
		cg->read_mSPT();
		cg->gen_random_smccl_query(argv[3]);
		delete cg;
	}
	else if(strcmp(argv[1], "gen-update-query") == 0) { // gen edge deletion/insertion, and steiner component query
		if(argc < 4) {
			display_usage();
			return 0;
		}

		ConnGraph *cg = new ConnGraph(argv[2]);
		cg->read_cg(binary_input);
		cg->read_mSPT();
		cg->mark_nontree();

		if(strcmp(argv[3], "gen-query") == 0) cg->gen_update_query(1);
		else cg->gen_update_query(0);

		delete cg;
	}
	else if(strcmp(argv[1], "cal-mspt-size") == 0) { //transform the mSPT into binary
		Graph *graph = new Graph(argv[2]);
		graph->read_mSPT();
		graph->cal_mSPT_size();
		delete graph;
	}
	else if(strcmp(argv[1], "extract-mcc") == 0) { // extract the maximum connected component
		Graph *graph = new Graph(argv[2]);
		graph->read_graph(0);
		graph->extract_mcc();
		delete graph;
	}
	else if(strcmp(argv[1], "count-cc") == 0) { // count the number of connected components
		Graph *graph = new Graph(argv[2]);
		graph->read_graph(binary_input);
		graph->count_cc();
		delete graph;
	}
	else if(strcmp(argv[1], "check-conn") == 0) {
		Graph *graph = new Graph(argv[2]);
		graph->read_graph(binary_input);
		graph->set_k(4);
		graph->check_connectivity(argv[3]);
		delete graph;
	}
	else if(strcmp(argv[1], "min-max-conn") == 0) { // compute the minimum and maximum connectivity
		Graph *graph = new Graph(argv[2]);
		graph->read_mSPT();
		graph->min_max_conn();
		delete graph;
	}
	else if(strcmp(argv[1], "gen-gc-query") == 0) {
		if(argc < 4) {
			display_usage();
			return 0;
		}
		Graph *graph = new Graph(argv[2]);
		graph->read_graph(binary_input);
		//graph->gen_gc_query(argv[3]);
		delete graph;
	}
	else if(strcmp(argv[1], "transform-edge-to-binary") == 0) {
		transform_edge_to_binary(argv[2]);
	}
	else if(strcmp(argv[1], "smcc-mSPT") == 0) {
		ConnGraph *cg = new ConnGraph(argv[2]);
		cg->read_cg(binary_input);
		cg->read_mSPT();
		cg->smcc_query_mSPT(argv[3]);
		delete cg;
	}
	else if(strcmp(argv[1], "smcc-naive") == 0) {
		Graph *cg = new Graph(argv[2]);
		cg->read_graph(binary_input);
		cg->smcc_naive(argv[3]);
		delete cg;
	}
	else if(strcmp(argv[1], "smccl-mSPT") == 0) {
		ConnGraph *cg = new ConnGraph(argv[2]);
		cg->read_cg(binary_input);
		cg->read_mSPT();
		cg->smccl_query_mSPT(argv[3]);
		delete cg;
	}
	else if(strcmp(argv[1], "smccl-naive") == 0) {
		Graph *cg = new Graph(argv[2]);
		cg->read_graph(binary_input);
		cg->smccl_naive(argv[3]);
		delete cg;
	}
	else if(strcmp(argv[1], "sc") == 0) {
		if(argc < 5) {
			display_usage();
			return 0;
		}
		Graph *cg = new Graph(argv[2]);
		if(argv[4][0] == '0') cg->read_graph(binary_input);
		else if(argv[4][0] == '1') cg->read_mSPT();
		else if(argv[4][0] == '2') cg->read_mSPTO();
		cg->sc(argv[3], atoi(argv[4]));
		delete cg;
	}
	else if(strcmp(argv[1], "mSPT-p2p") == 0) {
		Graph *graph = new Graph(argv[2]);
		graph->read_mSPT();
		graph->mSPT_P2P(argv[3]);
		delete graph;
	}
	else if(strcmp(argv[1], "spt-opt") == 0) {
		if(argc < 4) {
			display_usage();
			return 0;
		}
		Graph *graph = new Graph(argv[2]);
		if(strcmp(argv[3], "output") == 0) graph->optimization_tree(1);
		else graph->optimization_tree(0);
		delete graph;
	}
	else if(strcmp(argv[1], "kSC") == 0) {
		if(argc < 4) {
			display_usage();
			return 0;
		}
		Graph *graph = new Graph(argv[2]);
		graph->read_graph(binary_input);

		graph->kSC(atoi(argv[3]));

		if(strcmp(argv[4], "output") == 0) graph->output_k_edge_connected_subgraphs();

		delete graph;
	}
	else if(strcmp(argv[1], "all-sc") == 0) {
		if(argc < 5) {
			display_usage();
			return 0;
		}

		Graph *graph = new Graph(argv[2]);
		graph->read_graph(binary_input);

		if(strcmp(argv[3], "naive") == 0) {
			graph->all_SC_naive();
		}
		else if(strcmp(argv[3], "bottom-up") == 0) {
			graph->all_SC_BU();
		}

		if(strcmp(argv[4], "output") == 0) {
			string output_name = string(argv[2]) + "/cg.bin";
			FILE *fout = open_file(output_name.c_str(), "wb");
			graph->output_all_sc(fout);
			fclose(fout);
		}

		graph->max_spanning_tree();

		delete graph;
	}
	else if(strcmp(argv[1], "mSPT-tcse") == 0) {
		Graph *graph = new Graph(argv[2]);
		graph->read_mSPT();
		if(argc >= 5&&strcmp(argv[4], "output") == 0) graph->mSPT_tcse(argv[3], 1);
		else graph->mSPT_tcse(argv[3]);
		delete graph;
	}
	else if(strcmp(argv[1], "mSPT-mcse") == 0) {
		Graph *graph = new Graph(argv[2]);
		graph->read_mSPT();
		if(argc >= 5&&strcmp(argv[4], "output") == 0) graph->mSPT_mcse(argv[3], 1);
		else graph->mSPT_mcse(argv[3]);
		delete graph;
	}
	else if(strcmp(argv[1], "mSPT-scse") == 0) {
		Graph *graph = new Graph(argv[2]);
		graph->read_mSPT();
		if(argc >= 5&&strcmp(argv[4], "output") == 0) graph->mSPT_scse(argv[3], 1);
		else graph->mSPT_scse(argv[3]);
		delete graph;
	}
	else display_usage();

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	//seconds = end2.tv_sec - end1.tv_sec;
	//useconds = end2.tv_usec - end1.tv_usec;
	//mtime2 = seconds*1000000 + useconds;

	//printf("Total ltime: %lld\n", mtime);

#else
	end = clock();

	printf("Total time: %d\n", end-start);
#endif

	return 0;
}

void display_usage() {
	printf("[0]exe\t [1]gen-gc-query [2]graph_dir [3]number_of_vertices\n");
	printf("\t\t number_of_vertices can be -1 or [2,30]\n");
	printf("\t [1]process-update-query [2]graph_dir [3]update_file\n");
	printf("\t [1]count-cc [2]graph_dir\n");
	printf("\t [1]extract-mcc [2]graph_dir\n");
	printf("\t [1]min-max-conn [2]graph_dir\n");
	printf("\t [1]transform-edge-to-binary [2]graph_dir\n");
	printf("\t [1]kSC [2]graph_dir [3]k [4]output or not\n");
	printf("\t [1]all-sc [2]graph_dir [3]naive or bottom-up [4]output or not\n");
	printf("\t [1]spt-opt [2]graph_dir [3]output or not\n");
	printf("\t [1]gen-update-query [2]graph_dir [3]gen-query or not\n");
	printf("\t [1]gen-smcc-query [2]graph_dir [3]query_size\n");
	printf("\t [1]smcc-mSPT [2]graph_dir [3]query\n");
	printf("\t [1]sc [2]graph_dir [3]query [4]method 0,1,2\n");
}

void transform_edge_to_binary(char *dir) {
	string input_name = string(dir) + "/edges.txt";

	FILE *fin = open_file(input_name.c_str(), "r");
	int n, m;
	fscanf(fin, "%d%d", &n, &m);

	vector<pair<int,int> > vp;
	vp.reserve(m);

	for(int i = 0;i < m;i ++) {
		int a, b;
		fscanf(fin, "%d%d", &a, &b);
		if(a < b) vp.push_back(make_pair(a,b));
		else if(a > b) vp.push_back(make_pair(b,a));
	}

	fclose(fin);

	vector<int> ids;
	ids.reserve(2*m);

	for(int i = 0;i < (int)vp.size();i ++) {
		ids.push_back(vp[i].first);
		ids.push_back(vp[i].second);
	}

	sort(ids.begin(), ids.end());
	ids.erase(unique(ids.begin(), ids.end()), ids.end());

	sort(vp.begin(), vp.end());
	vp.erase(unique(vp.begin(), vp.end()), vp.end());

	//n = (int)ids.size();
	n = ids[ids.size() - 1] + 1;
	m = (int)vp.size();

	/*
	for(int i = 0;i < n;i ++) if(ids[i] != i) {
		printf("IDs are inconsistent: %d %d!\n", i, ids[i]);
		return ;
	}
	*/

	int *buf = new int[n];

	string output_name1 = string(dir) + "/edges_plain.txt";
	string output_name2 = string(dir) + "/edges.bin";
	FILE *fout1 = open_file(output_name1.c_str(), "w");
	FILE *fout2 = open_file(output_name2.c_str(), "wb");

	fprintf(fout1, "%d %d\n", n, m);
	fwrite(&n, sizeof(int), 1, fout2);
	fwrite(&m, sizeof(int), 1, fout2);

	int j = 0;
	for(int i = 0;i < n;i ++) {
		int cnt = 0;
		while(j < (int)vp.size()&&vp[j].first == i) {
			buf[cnt++] = vp[j].second;
			++ j;
		}

		//fprintf(fout1, "%d %d", ids[i], cnt);
		fprintf(fout1, "%d %d", i, cnt);
		for(int k = 0;k < cnt;k ++) fprintf(fout1, " %d", buf[k]);
		fprintf(fout1, "\n");

		//fwrite(&ids[i], sizeof(int), 1, fout2);
		fwrite(&i, sizeof(int), 1, fout2);
		fwrite(&cnt, sizeof(int), 1, fout2);
		fwrite(buf, sizeof(int), cnt, fout2);
	}

	fclose(fout1);
	fclose(fout2);

	delete[] buf;

	printf("Sizeof(int): %d\n", (int)sizeof(int));
}

void parse_graph(char *input_name, char *output_name) {
	FILE *fin = open_file(input_name, "r");
	char buf[1024];

	fgets(buf, 1024, fin);
	fgets(buf, 1024, fin);
	fgets(buf, 1024, fin);
	fgets(buf, 1024, fin);
	//fgets(buf, 1024, fin);

	int n = 0, m;
	vector<pair<int,int> > vp;
	vector<int> nodes;

	//nodes.reserve(1000000*140);
	//vp.reserve(1000000*70);

	int a, b;
	int cnt = 0;
	while(fscanf(fin, "%d%d", &a, &b) == 2) {
		++ cnt;

		if(cnt%1000000 == 0) printf(".");
		//printf("%d %d\n", a, b);

		if(a > n) n = a;
		if(b > n) n = b;

		if(a > b) vp.push_back(make_pair(b, a));
		else if(a < b) vp.push_back(make_pair(a,b));
	}

	for(int i = 0;i < (int)vp.size();i ++) {
		nodes.push_back(vp[i].first);
		nodes.push_back(vp[i].second);
	}

	printf("Max node_id: %d\n", n);

	//printf("here\n");

	sort(vp.begin(), vp.end());
	vp.erase(unique(vp.begin(), vp.end()), vp.end());

	m = (int)vp.size();

	sort(nodes.begin(), nodes.end());
	nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

	int *id = new int[n+1];

	for(int i = 0;i < (int)nodes.size();i ++) id[nodes[i]] = i;

	n = (int)nodes.size();

	fclose(fin);

	FILE *fout = open_file(output_name, "w");

	fprintf(fout, "%d %d\n", n, m);
	for(int i = 0;i < m;i ++) fprintf(fout, "%d %d\n", id[vp[i].first], id[vp[i].second]);

	delete[] id;

	fclose(fout);
}

void parse_graph_gr(char *input_name, char *output_name) {
	FILE *fin = open_file(input_name, "r");
	char buf[1024];


	int n = 0, m;
	vector<pair<int,int> > vp;
	vector<int> nodes;

	nodes.reserve(1000000*180);
	vp.reserve(1000000*90);

	int a, b;
	int cnt = 0;
	while(fscanf(fin, "%s", buf) == 1) {
		if(buf[0] != 'a') {
			fgets(buf, 1023, fin);
			continue;
		}

		++ cnt;

		if(cnt%1000000 == 0) printf(".");
		//printf("%d %d\n", a, b);
		int c;
		fscanf(fin, "%d%d%d", &a, &b, &c);

		if(a > n) n = a;
		if(b > n) n = b;

		nodes.push_back(a);
		nodes.push_back(b);

		if(a > b) vp.push_back(make_pair(b, a));
		else if(a < b) vp.push_back(make_pair(a,b));
	}

	printf("Max node_id: %d\n", n);

	//printf("here\n");

	sort(vp.begin(), vp.end());
	vp.erase(unique(vp.begin(), vp.end()), vp.end());

	m = (int)vp.size();

	sort(nodes.begin(), nodes.end());
	nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

	int *id = new int[n+1];

	for(int i = 0;i < (int)nodes.size();i ++) id[nodes[i]] = i;

	n = (int)nodes.size();

	fclose(fin);

	FILE *fout = open_file(output_name, "w");

	fprintf(fout, "%d %d\n", n, m);
	for(int i = 0;i < m;i ++) fprintf(fout, "%d %d\n", id[vp[i].first], id[vp[i].second]);

	delete[] id;

	fclose(fout);
}

