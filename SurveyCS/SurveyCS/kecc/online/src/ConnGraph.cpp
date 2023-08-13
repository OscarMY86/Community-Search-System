#include "ConnGraph.h"

ConnGraph::ConnGraph(const char *_dir) {
	dir = string(_dir);

	nodes = NULL;
	edges = NULL;
	snodes = NULL;
	sedges = NULL;
	elements = NULL;

	Q = NULL;
	computed = NULL;
	vis = NULL;

	tnodes = NULL;
	tedges = NULL;
	parent = NULL;
	weight = NULL;
	pedges = NULL;
	ppedges = NULL;
	buckets = NULL;
	bins = NULL;

	heads = NULL;
	ll = NULL;

	heap = NULL;

	pp = NULL;

	tmp_nodes = NULL;
	tmp_edges = NULL;
}

ConnGraph::~ConnGraph() {
	if(nodes != NULL) delete[] nodes;
	if(edges != NULL) delete[] edges;
	if(snodes != NULL) delete[] snodes;
	if(sedges != NULL) delete[] sedges;
	if(elements != NULL) delete[] elements;

	if(Q != NULL) delete[] Q;
	if(computed != NULL) delete[] computed;
	if(vis != NULL) delete[] vis;

	if(tnodes != NULL) delete[] tnodes;
	if(tedges != NULL) delete[] tedges;
	if(parent != NULL) delete[] parent;
	if(weight != NULL) delete[] weight;
	if(pedges != NULL) delete[] pedges;
	if(ppedges != NULL) delete[] ppedges;
	if(buckets != NULL) delete[] buckets;
	if(bins != NULL) delete[] bins;

	if(heads != NULL) delete[] heads;
	if(ll != NULL) delete[] ll;

	if(heap == NULL) delete heap;

	if(pp != NULL) delete[] pp;

	if(tmp_nodes != NULL) delete[] tmp_nodes;
	if(tmp_edges != NULL) delete[] tmp_edges;
}

//checked
void ConnGraph::read_cg(int binary) {
	/*
	if(!binary) {
		printf("reading nonbinary file is not implemented yet!\n");
		return ;
	}*/

	FILE *f = open_file((dir + string("/cg.bin")).c_str(), "rb");

	fread(&n, sizeof(int), 1, f);
	fread(&m, sizeof(int), 1, f);

	nodes = new Node[n];
	edges = new Edge[2*m+2*Max_Update];

	Q = new int[2*n];
	computed = new int[n];
	vis = new char[n];
	memset(vis, 0, sizeof(char)*n);

	int edge_c = 0;

	for(int i = 0;i < n;i ++) nodes[i].first = NULL;

	int *buf = NULL;

	buf = new int[3*m];
	fread(buf, sizeof(int), 3*m, f);

	for(int i = 0;i < 3*m;i += 3) {
		int a = buf[i], b = buf[i+1], c = buf[i+2];

		if(i > 0&&c > buf[i-1]) printf("WA in read_cg!\n");

		edges[edge_c].node_id = b;
		edges[edge_c].sc = c;
		edges[edge_c].deleted = 0;
		edges[edge_c].duplicate = &edges[edge_c+1];
		add_edge(nodes[a], &edges[edge_c]);
		++ edge_c;

		edges[edge_c].node_id = a;
		edges[edge_c].sc = c;
		edges[edge_c].deleted = 0;
		edges[edge_c].duplicate = &edges[edge_c-1];
		add_edge(nodes[b], &edges[edge_c]);
		++ edge_c;
	}

	edge_n = edge_c;

	fclose(f);

	if(buf != NULL) delete[] buf;

	snodes = new Node[n];
	sedges = new Edge[2*m + 2*Max_Update];

	tmp_nodes = new Node[n];
	tmp_edges = new Edge[2*m + 2*Max_Update];

	for(int i = 0;i < n;i ++) snodes[i].first = tmp_nodes[i].first = NULL;

	elements = new Element[n];
	for(int i = 0;i < n;i ++) elements[i].value = i;

	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) {
		if(e->next != NULL&&e->sc < e->next->sc) {
			printf("Edges of the connectivity graph are not sorted!\n");
			return ;
		}
	}

	/*********************************/
	vector<pair<int,int> > vp;
	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) if(e->node_id > i) {
		vp.push_back(make_pair(i,e->node_id));
	}
	sort(vp.begin(), vp.end());

	FILE *ff = open_file((dir+string("/edges_t.txt")).c_str(), "w");

	fprintf(ff, "%d %d\n", n, (int)vp.size());
	for(int i = 0;i < (int)vp.size();i ++) fprintf(ff, "%d %d\n", vp[i].first, vp[i].second);

	fclose(ff);
}
//checked
void ConnGraph::read_mSPT() {
	string input_name = dir + "/mSPT.txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	int tm;
	fscanf(fin, "%d%d", &n, &tm);

	if(tm != n-1) {
		//printf("Not connected!\n");
		//return ;
	}

	int cid;
	int *levels = new int[n];

	for(int i = 0;i < n;i ++) {
		fscanf(fin, "%d%d", &cid, levels+i);
		if(cid != 0) {
			//printf("Note connected!\n");
			//return ;
		}
	}

	if(tnodes == NULL) tnodes = new Node[n];
	if(tedges == NULL) tedges = new Edge[2*tm];

	int edge_c = 0;
	for(int i = 0;i < n;i ++) tnodes[i].first = tnodes[i].last = NULL;

	if(parent == NULL) parent = new int[n];
	if(weight == NULL) weight = new int[n];

	for(int i = 0;i < n;i ++) parent[i] = -1;

	if(pedges == NULL) pedges = new Edge*[n];
	for(int i = 0;i < n;i ++) pedges[i] = NULL;

	for(int i = 0;i < tm;i ++) {
		int a, b, c;
		fscanf(fin, "%d%d%d", &a, &b, &c);

		if(levels[a] != levels[b]+1&&levels[b] != levels[a]+1) printf("WA in read_mSPT\n");

		if(levels[a] < levels[b]) {
			parent[b] = a;
			weight[b] = c;
			pedges[b] = &tedges[edge_c];
		}
		else {
			parent[a] = b;
			weight[a] = c;
			pedges[a] = &tedges[edge_c];
		}

		tedges[edge_c].node_id = b;
		tedges[edge_c].sc = c;
		tedges[edge_c].duplicate = &tedges[edge_c+1];
		add_edge(tnodes[a], &tedges[edge_c]);
		++ edge_c;

		tedges[edge_c].node_id = a;
		tedges[edge_c].sc = c;
		tedges[edge_c].duplicate = &tedges[edge_c-1];
		add_edge(tnodes[b], &tedges[edge_c]);
		++ edge_c;
	}

	for(int i = 0;i < n;i ++) if((levels[i] != 0&&parent[i] == -1)||(levels[i] == 0&&parent[i] != -1)) {
		printf("WA1 in read_mSPT!\n");
	}

	for(int i = 0;i < n;i ++) for(Edge *e = tnodes[i].first;e != NULL;e = e->next) {
		if(e->next != NULL&&e->sc < e->next->sc) printf("WA2 in read_mSPT!\n");
	}

	fclose(fin);

	delete[] levels;
}
//ckecked
void ConnGraph::mark_nontree() {
	if(ppedges == NULL) ppedges = new Edge*[n];
	if(buckets == NULL) buckets = new Bin*[n];
	if(bins == NULL) bins = new Bin[m+Max_Update];

	int bin_c = 0;
	for(int i = 0;i < n;i ++) {
		buckets[i] = NULL;
		ppedges[i] = NULL;
	}

	max_key = 0;

	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL; e = e->next) if(e->node_id > i) {
		if(parent[i] == e->node_id) {
			ppedges[i] = e;

			e->nontree = e->duplicate->nontree = NULL;
		}
		else if(parent[e->node_id] == i) {
			ppedges[e->node_id] = e;

			e->nontree = e->duplicate->nontree = NULL;
		}
		else {
			e->nontree = e->duplicate->nontree = &bins[bin_c];

			if(e->sc > max_key) max_key = e->sc;

			bins[bin_c].edge = e;
			insert_into_bucket(e->sc, &bins[bin_c]);
			++ bin_c;
		}
	}
}

void ConnGraph::check_parallel_edge() {
	vector<pair<int,int> > vp;
	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) {
		if(e->node_id > i) vp.push_back(make_pair(i, e->node_id));
	}

	sort(vp.begin(),  vp.end());
	for(int i = 1;i < (int)vp.size();i ++) if(vp[i].first == vp[i-1].first&&vp[i].second == vp[i-1].second) printf("Parrallel!\n");
}

void ConnGraph::gen_update_query(int gen_query) {
	if(!gen_query) {
		string output_name = string(dir) + "/update-0.txt";

		FILE *fout = open_file(output_name.c_str(), "w");

		int total_cnt = 20;
		pair<int,int> deleted[500];

		int max_sc = 0;
		for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) if(e->sc > max_sc) max_sc = e->sc;

		vector<pair<int,int> > vp;
		for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) if(e->sc > max_sc*4/5) {
			if(e->node_id > i) vp.push_back(make_pair(i, e->node_id));
		}


		fprintf(fout, "%d\n", total_cnt);

		int deleted_cnt = 0, deleted_c = 0;
		for(int i = 0;i < total_cnt;i ++) {
			if(deleted_cnt < total_cnt/2&&(rand()%5 <= 2||deleted_c == 0)) {
				int a, b;
				while(1) {
					int id = rand()%vp.size();
					a = vp[id].first; b = vp[id].second;
					vp[id] = vp[vp.size()-1];
					vp.pop_back();

					for(Edge *e = nodes[a].first;e != NULL;e = e->next) if(e->node_id == b) {
						e->deleted = e->duplicate->deleted = 1;
						break;
					}

					break;
					if(count_cc() == 1) break;

					for(Edge *e = nodes[a].first;e != NULL;e = e->next) if(e->node_id == b) {
						e->deleted = e->duplicate->deleted = 0;
					}
				}
				deleted[deleted_c].first = a; deleted[deleted_c].second = b;
				++ deleted_c;

				++ deleted_cnt;
				fprintf(fout, "D %d %d\n", a, b);
			}
			else {
				if(deleted_c == 0) printf("WA in gen_update_query!\n");
				int id = rand()%deleted_c;
				fprintf(fout, "I %d %d\n", deleted[id].first, deleted[id].second);

				for(Edge *e = nodes[deleted[id].first].first;e != NULL;e = e->next) if(e->node_id == deleted[id].second) {
					e->deleted = e->duplicate->deleted = 0;
				}

				-- deleted_c;
				deleted[id] = deleted[deleted_c];
			}
		}

		fclose(fout);

		return ;
	}

	if(heads == NULL) heads = new Bin*[n];
	if(ll == NULL) ll = new Bin[n];
	for(int i = 0;i < n;i ++) heads[i] = NULL;

	if(heap == NULL) heap = new LinearHeap(n);

	memset(computed, 0, sizeof(int)*n);
	if(pp == NULL) {
		pp = new int[n];
		for(int i = 0;i < n;i ++) pp[i] = -1;
	}

	string output_name = string(dir) + "/update-0.txt";
	FILE *fin = open_file(output_name.c_str(), "r");

	output_name = string(dir) + "/update-1.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	int t;
	fscanf(fin, "%d", &t);
	fprintf(fout, "%d\n", 2*t);
	for(int i = 0;i < t;i ++) {
		char buf[128];
		int a, b;
		fscanf(fin, "%s%d%d", buf, &a, &b);
		fprintf(fout, "%s %d %d\n", buf, a, b);

		if(buf[0] == 'I') insert_edge(a, b);
		else if(buf[0] == 'D') delete_edge(a, b);
		else {
			printf("WA in update-0.txt!\n");
			return ;
		}

		vector<int> vp;
		for(int j = 0;j < n;j ++) if(tnodes[j].first->sc > 3) vp.push_back(j);
		random_shuffle(vp.begin(), vp.end());

		a = vp[rand()%vp.size()];
		int K = tnodes[a].first->sc;

		vis[a] = 1; Q[0] = a;
		int q_c = 1;
		for(int j = 0;j < q_c;j ++) for(Edge *e = tnodes[Q[j]].first;e != NULL&&e->sc >= K;e = e->next) {
			if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				Q[q_c ++] = e->node_id;
			}
		}

		for(int j = 0;j < q_c;j ++) vis[Q[i]] = 0;

		int q_n = 10;
		fprintf(fout, "Q %d", q_n);
		if(q_c < q_n) {
			printf("WA in gen_update_query!\n");
			return ;
		}
		for(int j = 0;j < q_n;j ++) {
			int id = rand()%q_c;
			fprintf(fout, " %d", Q[id]);
			-- q_c;
			Q[id] = Q[q_c];
		}
		fprintf(fout, "\n");
	}

	fclose(fin);
	fclose(fout);
}

void ConnGraph::gen_random_smccl_query(const char *sizes) {
	string output_name = string(dir) + string("/smccl-query-") + string(sizes) + string(".txt");
	FILE *fout = open_file(output_name.c_str(), "w");

	vector<int> vp;
	for(int j = 0;j < n;j ++) if(tnodes[j].first->sc > 6) vp.push_back(j);
	random_shuffle(vp.begin(), vp.end());

	int t = 1000, q_n;

	int size = atoi(sizes);

	if(vis == NULL) vis = new char[n];
	if(Q == NULL) Q = new int[n];
	memset(vis, 0, sizeof(char)*n);

	fprintf(fout, "%d\n", t);

	for(int i = 0;i < t;i ++) {
		int a = vp[rand()%vp.size()];
		int K = tnodes[a].first->sc;

		if(size == 0) q_n = rand()%29+2;
		else q_n = size;

#ifdef _DEBUG_
		for(int j = 0;j < n;j ++) if(vis[j] != 0) {
			printf("WA\n");
			break;
		}
#endif

		vis[a] = 1; Q[0] = a;
		int q_c = 1;
		for(int j = 0;j < q_c;j ++) for(Edge *e = tnodes[Q[j]].first;e != NULL&&e->sc >= K;e = e->next) {
			if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				Q[q_c ++] = e->node_id;
			}
		}

		for(int j = 0;j < q_c;j ++) vis[Q[j]] = 0;

		int q_n = 10;
		if(q_c < q_n) {
			-- i;
			continue;
		}
		fprintf(fout, "%d %d", q_n, rand()%20 + 10);
		for(int j = 0;j < q_n;j ++) {
			int id = rand()%q_c;
			fprintf(fout, " %d", Q[id]);
			-- q_c;
			Q[id] = Q[q_c];
		}
		fprintf(fout, "\n");
	}
	fclose(fout);
}

void ConnGraph::gen_random_smcc_query(const char *sizes) {
	string output_name = string(dir) + string("/smcc-query-") + string(sizes) + string(".txt");
	FILE *fout = open_file(output_name.c_str(), "w");

	vector<int> vp;
	for(int j = 0;j < n;j ++) if(tnodes[j].first->sc > 5) vp.push_back(j);
	random_shuffle(vp.begin(), vp.end());

	int t = 1000, q_n;

	int size = atoi(sizes);

	if(vis == NULL) vis = new char[n];
	if(Q == NULL) Q = new int[n];
	memset(vis, 0, sizeof(char)*n);

	fprintf(fout, "%d\n", t);

	for(int i = 0;i < t;i ++) {
		int a = vp[rand()%vp.size()];
		int K = tnodes[a].first->sc;

		if(size == 0) q_n = rand()%29+2;
		else q_n = size;

#ifdef _DEBUG_
		for(int j = 0;j < n;j ++) if(vis[j] != 0) {
			printf("WA\n");
			break;
		}
#endif

		vis[a] = 1; Q[0] = a;
		int q_c = 1;
		for(int j = 0;j < q_c;j ++) for(Edge *e = tnodes[Q[j]].first;e != NULL&&e->sc >= K;e = e->next) {
			if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				Q[q_c ++] = e->node_id;
			}
		}

		for(int j = 0;j < q_c;j ++) vis[Q[j]] = 0;

		//int q_n = 10;
		if(q_c < q_n) {
			-- i;
			continue;
		}
		fprintf(fout, "%d", q_n);
		for(int j = 0;j < q_n;j ++) {
			int id = rand()%q_c;
			fprintf(fout, " %d", Q[id]);
			-- q_c;
			Q[id] = Q[q_c];
		}
		fprintf(fout, "\n");
	}
	fclose(fout);
}

int ConnGraph::count_cc() {
	int cnt = 0;

	if(computed == NULL) computed = new int[n];
	memset(computed, 0, sizeof(int)*n);

	for(int i = 0;i < n;i ++) {
		if(computed[i]) continue;

		++ cnt;

		Q[0] = i;
		int q_c = 1;
		computed[i] = 1;

		for(int j = 0;j < q_c;j ++) {
			for(Edge *e = nodes[Q[j]].first;e != NULL;e = e->next) if(!e->deleted&&!computed[e->node_id]) {
				computed[e->node_id] = 1;
				Q[q_c++] = e->node_id;
			}
		}
	}

	memset(computed, 0, sizeof(int)*n);

	return cnt;
}

void ConnGraph::process_update_query(const char *file_name) {
	string input_name = dir + "/" + file_name + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	if(heads == NULL) heads = new Bin*[n];
	if(ll == NULL) ll = new Bin[2*n];
	for(int i = 0;i < n;i ++) heads[i] = NULL;

	if(heap == NULL) heap = new LinearHeap(n);

	memset(computed, 0, sizeof(int)*n);
	if(pp == NULL) {
		pp = new int[n];
		for(int i = 0;i < n;i ++) pp[i] = -1;
	}

	char op[10000];
	vector<vector<int> > vp;

	char ops[10];
	int op_n;
	fscanf(fin, "%d", &op_n);

	for(int i = 0;i < op_n;i ++) {
		fscanf(fin, "%s", ops);
		vector<int> tt;
		if(ops[0] == 'I') {
			int a, b;
			fscanf(fin, "%d%d", &a, &b);
			op[i] = 'I';
			tt.push_back(a); tt.push_back(b);
		}
		else if(ops[0] == 'D') {
			int a, b;
			fscanf(fin, "%d%d", &a, &b);
			op[i] = 'D';
			tt.push_back(a); tt.push_back(b);
		}
		else if(ops[0] == 'Q') {
			int q_n;
			int tmp;
			fscanf(fin, "%d", &q_n);
			for(int i = 0;i < q_n;i ++) {
				fscanf(fin, "%d", &tmp);
				tt.push_back(tmp);
			}
		}
		vp.push_back(tt);
	}

	int q[100];

#ifdef _LINUX_
	struct timeval start, end;
	gettimeofday(&start, NULL);
#else
	int start, end;
	start = clock();
#endif

	for(int i = 0;i < op_n;i ++) {
		//printf("%d\n", i);
		vector<int> &tt = vp[i];
		if(op[i] == 'I') insert_edge(tt[0], tt[1]);
		else if(op[i] == 'D') delete_edge(tt[0], tt[1]);
		else if(op[i] == 'Q') {
			int q_n = (int)tt.size();
			for(int j = 0;j < q_n;j ++) q[j] = tt[j];
			smcc_query_mSPT(q_n, q);
		}
		else {
			printf("WA update-query file!\n");
			return ;
		}

		//printf("%d %s\n", i, op);
	}

	fclose(fin);

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	printf("update ltime: %lld\n", mtime);

#else
	end = clock();

	printf("update time: %d\n", end-start);
#endif
}

void ConnGraph::check_mSPT() {
	vector<pair<pair<int,int>, int> > vp;
	vp.reserve(m);
	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) {
		if(e->node_id > i) vp.push_back(make_pair(make_pair(i, e->node_id), e->sc));
		if(e->sc != e->duplicate->sc) printf("WA in check_mSPT\n");

		if(parent[i] == e->node_id) {
			if(e->sc != weight[i]) {
				printf("WA1 in check_mSPT %d %d %d %d\n", i, e->node_id, e->sc, weight[i]);
				return ;
			}
		}
		else if(parent[e->node_id] == i) {
			if(e->sc != weight[e->node_id]) {
				printf("WA1 in check_mSPT %d %d %d %d\n", i, e->node_id, e->sc, weight[e->node_id]);
				return ;
			}
		}
	}

	random_shuffle(vp.begin(), vp.end());

	for(int i = 0;i < 100000&&i < (int)vp.size();i ++) {
		int a = vp[i].first.first, b = vp[i].first.second, c = vp[i].second;
		int lca;
		int conn = pair_connectivity(a, b, lca, 0);

		if(conn != c) printf("WA in check_mSPT %d %d %d %d\n", a, b, c, conn);
	}

	int cnt = 0;
	for(int i = 0;i < n;i ++) for(Edge *e = tnodes[i].first;e != NULL;e = e->next) {
		if(e->next != NULL&&e->sc < e->next->sc) printf("Edges in mSPT are not sorted!\n");
		++ cnt;
		if(parent[i] == e->node_id) {
			if(weight[i] != e->sc) printf("WA2 in check_mSPT\n");
		}
		else if(parent[e->node_id] == i) {
			if(weight[e->node_id] != e->sc) printf("WA2 in check_mSPT\n");
		}
		else printf("WA3 in check_mSPT (%d %d) (%d %d) (%d %d)\n", i, parent[i], e->node_id, parent[e->node_id], e->node_id, e->duplicate->node_id);
	}

	if(cnt != 2*(n-1)) printf("WA4 in check_mSPT %d %d\n", n, cnt);

	for(int i = 0;i < n;i ++) if(parent[i] == i) printf("WA5 in check_mSPT %d\n", i);
}

void ConnGraph::check_cg() {
	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) {
		if(e->next != NULL&&e->sc < e->next->sc) {
			printf("Edges of the connectivity graph are not sorted!\n");
			return ;
		}
	}
}

void ConnGraph::output_cg() {
	m = 0;
	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) if(e->node_id > i) ++ m;

	vector<pair<int,pair<int,int> > > vp;
	vp.reserve(m);

	string output_name = string(dir) + "/cg_update.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	fprintf(fout, "%d %d\n", n, m);

	int edge_c = 0;

	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) if(e->node_id > i) {
		vp.push_back(make_pair(e->sc, make_pair(i,e->node_id)));
	}

	sort(vp.begin(), vp.end());

	for(int i = (int)vp.size()-1;i >= 0;i --) {
		fprintf(fout, "%d %d %d\n", vp[i].second.first, vp[i].second.second, vp[i].first);
	}

	fclose(fout);
}

void ConnGraph::smcc_query_mSPT(const char *file_name) {
	string input_name = dir + "/" + file_name + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	vector<vector<int> > query;

	if(computed == NULL) computed = new int[n];
	if(Q == NULL) Q = new int[n];
	memset(computed, 0, sizeof(int)*n);

	if(heads == NULL) heads = new Bin*[n];
	if(ll == NULL) ll = new Bin[2*n];
	for(int i = 0;i < n;i ++) heads[i] = NULL;

	int t;
	fscanf(fin, "%d", &t);
	for(int i = 0;i < t;i ++) {
		int d;
		fscanf(fin, "%d", &d);
		vector<int> tmp;
		while(d --) {
			int tt;
			fscanf(fin, "%d", &tt);
			tmp.push_back(tt);
		}
		query.push_back(tmp);
	}

	fclose(fin);

#ifdef _LINUX_
	struct timeval start, end;
	gettimeofday(&start, NULL);
#else
	int start, end;
	start = clock();
#endif

	int q[40], q_n;

	for(int i = 0;i < t;i ++) {
		printf("procesing query %d\n", i);
		vector<int> &tmp = query[i];
		q_n = tmp.size();
		for(int j = 0;j < q_n;j ++) q[j] = tmp[j];

		smcc_query_mSPT(q_n, q);
	}

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	printf("smcc_mSPT ltime: %lld\n", mtime);

#else
	end = clock();

	printf("smcc_mSPT time: %d\n", end-start);
#endif
}

void ConnGraph::smcc_query_mSPT(int q_n, int *q) {
#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(computed[i]) printf("WA in smcc_query_mSPT\n");
#endif
	int max_key = tnodes[q[0]].first->sc;
	int edge_c = 1;
	ll[0].edge = tnodes[q[0]].first;
	ll[0].next = heads[max_key];
	heads[max_key] = &ll[0];

	//printf( "max_key=%d\n", max_key );

	for(int i = 0;i < q_n;i ++) computed[q[i]] = 1;
	Q[0] = q[0]; vis[q[0]] = 1;

	int conn = 0, cnt = 1, q_c = 1;
	while(max_key >= conn) {
		while(max_key >= conn&&heads[max_key] == NULL) -- max_key;
		if(max_key < conn) break;
		Edge *e = heads[max_key]->edge; heads[max_key] = heads[max_key]->next;

		while(e != NULL&&e->sc >= max_key) {
			if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				Q[q_c ++] = e->node_id;

				ll[edge_c].edge = tnodes[e->node_id].first;
				int key = ll[edge_c].edge->sc;
				if(key > max_key) key = max_key;
				ll[edge_c].next = heads[key];
				heads[key] = &ll[edge_c];
				++ edge_c;

				if(computed[e->node_id]) {
					++ cnt;
					if(cnt == q_n) conn = max_key;
				}
			}

			e = e->next;
		}
		if(e != NULL) {
			ll[edge_c].edge = e;
			int key = ll[edge_c].edge->sc;
			ll[edge_c].next = heads[key];
			heads[key] = &ll[edge_c];
			++ edge_c;
		}
	}

	while(max_key >= 0) {
		if(heads[max_key] != NULL) heads[max_key] = NULL;
		-- max_key;
	}

	for(int i = 0;i < q_c;i ++) vis[Q[i]] = 0;
	for(int i = 0;i < q_n;i ++) computed[q[i]] = 0;
}

void ConnGraph::smccl_query_mSPT(const char *file_name) {
	string input_name = dir + "/" + file_name + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	vector<vector<int> > query;
	vector<int> L;

	if(computed == NULL) computed = new int[n];
	if(Q == NULL) Q = new int[n];
	memset(computed, 0, sizeof(int)*n);

	if(heads == NULL) heads = new Bin*[n];
	if(ll == NULL) ll = new Bin[2*n];
	for(int i = 0;i < n;i ++) heads[i] = NULL;

	int t;
	fscanf(fin, "%d", &t);
	for(int i = 0;i < t;i ++) {
		int d, tt;
		fscanf(fin, "%d%d", &d,&tt);
		L.push_back(tt);
		vector<int> tmp;
		while(d --) {
			fscanf(fin, "%d", &tt);
			tmp.push_back(tt);
		}
		query.push_back(tmp);
	}

	fclose(fin);

#ifdef _LINUX_
	struct timeval start, end;
	gettimeofday(&start, NULL);
#else
	int start, end;
	start = clock();
#endif

	int q[40], q_n;

	for(int i = 0;i < t;i ++) {
		//printf("%d\n", i);
		vector<int> &tmp = query[i];
		q_n = tmp.size();
		for(int j = 0;j < q_n;j ++) q[j] = tmp[j];

		smccl_query_mSPT(q_n, q, L[i]);
	}

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	printf("smccl_mSPT ltime: %lld\n", mtime);

#else
	end = clock();

	printf("smccl_mSPT time: %d\n", end-start);
#endif
}

void ConnGraph::smccl_query_mSPT(int q_n, int *q, int L) {
#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(computed[i]) printf("WA in smcc_query_mSPT\n");
#endif
	int max_key = tnodes[q[0]].first->sc;
	int edge_c = 1;
	ll[0].edge = tnodes[q[0]].first;
	ll[0].next = heads[max_key];
	heads[max_key] = &ll[0];

	for(int i = 0;i < q_n;i ++) computed[q[i]] = 1;
	Q[0] = q[0]; vis[q[0]] = 1;

	int conn = 0, cnt = 1, q_c = 1;
	while(max_key >= conn) {
		while(max_key >= conn&&heads[max_key] == NULL) -- max_key;
		if(max_key < conn) break;
		Edge *e = heads[max_key]->edge; heads[max_key] = heads[max_key]->next;

		while(e != NULL&&e->sc >= max_key) {
			if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				Q[q_c ++] = e->node_id;

				ll[edge_c].edge = tnodes[e->node_id].first;
				int key = ll[edge_c].edge->sc;
				if(key > max_key) key = max_key;
				ll[edge_c].next = heads[key];
				heads[key] = &ll[edge_c];
				++ edge_c;

				if(computed[e->node_id]) ++ cnt;
			}

			if(cnt == q_n&&q_c >= L&&conn == 0) conn = max_key;
			e = e->next;
		}
		if(e != NULL) {
			ll[edge_c].edge = e;
			int key = ll[edge_c].edge->sc;
			ll[edge_c].next = heads[key];
			heads[key] = &ll[edge_c];
			++ edge_c;
		}
	}

	while(max_key >= 0) {
		if(heads[max_key] != NULL) heads[max_key] = NULL;
		-- max_key;
	}

	for(int i = 0;i < q_c;i ++) vis[Q[i]] = 0;
	for(int i = 0;i < q_n;i ++) computed[q[i]] = 0;
}

void ConnGraph::insert_edge(int a, int b) {
	int sedge_c = 0;
	vector<int> vv;
	int conn = extract_scp(a, b, sedge_c, 0, vv);

#ifdef _DEBUG_
	printf("Connectivity: %d\n", conn);
	for(Edge *e = nodes[a].first;e != NULL;e = e->next) if(e->node_id == b) printf("WA in insert_edge\n");
#endif

	tmp_edges[sedge_c].node_id = b;
	tmp_edges[sedge_c].sc = 1;
	tmp_edges[sedge_c].deleted = 0;
	tmp_edges[sedge_c].duplicate = &tmp_edges[sedge_c+1];
	add_edge(tmp_nodes[a], &tmp_edges[sedge_c]);
	++ sedge_c;

	tmp_edges[sedge_c].node_id = a;
	tmp_edges[sedge_c].sc = 1;
	tmp_edges[sedge_c].deleted = 0;
	tmp_edges[sedge_c].duplicate = &tmp_edges[sedge_c-1];
	add_edge(tmp_nodes[b], &tmp_edges[sedge_c]);
	++ sedge_c;

	vector<Element *> kcc;

	for(int i = 0;i < (int)vv.size();i ++) {
		if(vis[vv[i]]) continue;

		extract_subgraph(vv[i]);

		vector<Element *> cc;
		decompose(vv[i], cc, conn+1);

		if(cc.size() == 1) {
			for(Element *e = cc[0];e != NULL;e = e->next) vis[e->value] = 1;
			kcc.push_back(cc[0]);
		}
		else remove_inter_edges(cc);

		-- i;
	}

	for(int i = 0;i < (int)vv.size();i ++) {
		vis[vv[i]] = 0;
		tmp_nodes[vv[i]].first = NULL;
	}

	vector<Bin *> incremented;
	reassign_edge_weights_insert(a, b, kcc, conn, incremented);

	update_mSPT_increment(incremented);
}

void ConnGraph::delete_edge(int a, int b) {
#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(snodes[i].first != NULL) {
		printf("WA1 in delete_edge\n");
		break;
	}
#endif

	int sedge_c = 0;
	vector<int> vv;
	int conn = extract_scp(a, b, sedge_c, 0, vv);

#ifdef _DEBUG_
	printf("Connectivity: %d\n", conn);
#endif

	for(Edge *e = tmp_nodes[a].first;e != NULL;e = e->next) if(e->node_id == b) {
		remove_edge(tmp_nodes[a], e);
		remove_edge(tmp_nodes[b], e->duplicate);
#ifdef _DEBUG_
		if(e->duplicate->node_id != a) printf("WA2 in delete_edge\n");
#endif
		break;
	}

	vector<Element *> kcc;

	for(int i = 0;i < (int)vv.size();i ++) {
		if(vis[vv[i]]) continue;

		extract_subgraph(vv[i]);

		vector<Element *> cc;
		decompose(vv[i], cc, conn);

		if(cc.size() == 1) {
			for(Element *e = cc[0];e != NULL;e = e->next) vis[e->value] = 1;
			kcc.push_back(cc[0]);
		}
		else remove_inter_edges(cc);

		-- i;
	}

	for(int i = 0;i < (int)vv.size();i ++) {
		vis[vv[i]] = 0;
		tmp_nodes[vv[i]].first = NULL;
	}

#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(snodes[i].first != NULL) {
		printf("WA2 in delete_edge\n");
		break;
	}
#endif

	vector<int> decremented;
	int deleted_vertex;
	reassign_edge_weights_delete(a, b, kcc, conn, decremented, deleted_vertex);

#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(snodes[i].first != NULL) {
		printf("WA3 in delete_edge\n");
		break;
	}
#endif

	update_mSPT_decrement(deleted_vertex, decremented, conn);

#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(snodes[i].first != NULL) {
		printf("WA4 in delete_edge\n");
		break;
	}
#endif
}
//checked
int ConnGraph::find_root(int a, int &q_c) {
	int new_qc = q_c+1;
	Q[q_c] = a;
	while(1) {
		if(pp[a] != -1) {
			a = pp[a];
			Q[new_qc ++] = a;
		}
		else if(parent[a] != -1) {
			a = parent[a];
			Q[new_qc ++] = a;
		}
		else break;
	}

	int root = a;

	for(int i = q_c;i < new_qc;i ++) {
		a = Q[i];
		if(pp[a] == -1&&a != root) {
			Q[q_c ++] = a;
			pp[a] = root;
		}
	}
	//pp[root] = -1;

	return root;
}
//checked
int ConnGraph::find_root_K(int a, int &q_c, int K) {
	int new_qc = q_c+1;
	Q[q_c] = a;
	while(1) {
		if(pp[a] != -1) {
			a = pp[a];
			Q[new_qc ++] = a;
		}
		else if(parent[a] != -1&&weight[a] >= K) {
			a = parent[a];
			Q[new_qc ++] = a;
		}
		else break;
	}

	int root = a;

	for(int i = q_c;i < new_qc;i ++) {
		a = Q[i];
		if(pp[a] == -1&&a != root) {
			Q[q_c ++] = a;
			pp[a] = root;
		}
	}

	return root;
}

void ConnGraph::remove_inter_edges(const vector<Element *> &cc) {
	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = j+1;

	int q_c = 0;

	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) {
		int s = e->value;
		Edge *list = tmp_nodes[s].first;
		Edge *delete_first = tmp_nodes[s].last->next;
		tmp_nodes[s].first = tmp_nodes[s].last = NULL;
		int cnt = 0;

		while(list != NULL&&!list->deleted) {
			Edge *tmp = list->next;
			if(computed[list->node_id] == computed[s]) {

				if(tmp_nodes[s].first == NULL) {
					tmp_nodes[s].first = tmp_nodes[s].last = list;
					list->pre = NULL;
				}
				else {
					tmp_nodes[s].last->next = list;
					list->pre = tmp_nodes[s].last;
					tmp_nodes[s].last = list;
				}
			}
			else {
				list->deleted = 1;

				list->next = delete_first;
				if(delete_first != NULL) delete_first->pre = list;
				delete_first = list;
			}

			list = tmp;
		}

		if(tmp_nodes[s].first == NULL) {
			tmp_nodes[s].first = tmp_nodes[s].last = delete_first;
			if(delete_first != NULL) delete_first->pre = NULL;
		}
		else {
			tmp_nodes[s].last->next = delete_first;
			if(delete_first != NULL) delete_first->pre = tmp_nodes[s].last;
		}
	}

	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = 0;
}

void ConnGraph::swap_tree_edge(int a, int b, Bin *bin, int removed_vertex, int deleted) {
	int ta = a;
	vector<int> path;
	path.push_back(ta);
	while(ta != removed_vertex) {
		ta = parent[ta];
		path.push_back(ta);
#ifdef _DEBUG_
		if(ta == -1) printf("WA1 in swap_tree_edge\n");
#endif
	}

	Edge *tpedge = pedges[removed_vertex];
	Edge *tppedge = ppedges[removed_vertex];

	for(int i = (int)path.size()-1;i > 0;i --) {
		parent[path[i]] = path[i-1];
		weight[path[i]] = weight[path[i-1]];
		pedges[path[i]] = pedges[path[i-1]];
		ppedges[path[i]] = ppedges[path[i-1]];

#ifdef _DEBUG_
		if(parent[path[i]] == path[i]) printf("WA2 in swap_tree_edge\n");
#endif
	}

	remove_from_mSPT_bidirectional(tpedge);
	pedges[a] = tpedge;
	parent[a] = b;
	weight[a] = bin->edge->sc;
	add_to_mSPT_bidirectional(a, b, weight[a], tpedge);

#ifdef _DEBUG_
	if(parent[a] == a) printf("WA3 in swap_tree_edge\n");
#endif

	ppedges[a] = bin->edge;
	bin->edge->nontree = bin->edge->duplicate->nontree = NULL;
	delete_from_bucket(bin->edge->sc, bin);

	if(!deleted) {
		bin->edge = tppedge;
		tppedge->nontree = tppedge->duplicate->nontree = bin;
		insert_into_bucket(tppedge->sc, bin);
	}
}
//check
void ConnGraph::update_mSPT_decrement(int deleted_vertex, const vector<int> &decremented, int K) {
	int q_c = 0;

	//int lca;
	//printf("1 3601 2890 %d\n", pair_connectivity(3601, 2890, lca, 0));

	if(deleted_vertex != -1) {
		parent[deleted_vertex] = -1;

		int ok = 0;

		while(max_key > 0&&buckets[max_key] == NULL) -- max_key;

		for(int i = max_key;i > 0&&!ok;i --) for(Bin *bin = buckets[i];bin != NULL;bin = bin->next) {
			int a = bin->edge->node_id, b = bin->edge->duplicate->node_id;

			int pa = find_root(a, q_c);
			int pb = find_root(b, q_c);

			if(pa != pb) {
				ok = 1;

#ifdef _DEBUG_
				if(pa != deleted_vertex&&pb != deleted_vertex||i != bin->edge->sc) {
					printf("WA1 in update_mSPT_decrement!\n");
					return ;
				}
#endif

				if(pb == deleted_vertex) swap(a, b);

				swap_tree_edge(a, b, bin, deleted_vertex, 1);
				if(pp[deleted_vertex] == -1) Q[q_c ++] = deleted_vertex;
				pp[deleted_vertex] = b;

				break;
			}
		}

		for(int i = 0;i < q_c;i ++) pp[Q[i]] = -1;
		q_c = 0;
	}

#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(pp[i] != -1) printf("WA2 in update_mSPT_decrement\n");
#endif

	//vector<int> tp;
	//tp.reserve(decremented.size());

	//for(int i = 0;i < (int)decremented.size();i ++) {
	//	tp.push_back(parent[decremented[i]]);
	//	parent[decremented[i]] = -1;
	//}

	int cnt = (int)decremented.size();

	for(Bin *bin = buckets[K];bin != NULL&&cnt;) {
		Bin *tmp = bin->next;

		int a = bin->edge->node_id, b = bin->edge->duplicate->node_id;

		int pa = find_root_K(a, q_c, K);
		int pb = find_root_K(b, q_c, K);

#ifdef _DEBUG_
		if(bin->edge->sc != K) printf("WA k in update_mSPT_decrement!\n");
		//int conn, lca;
		//conn = pair_connectivity(a, b, lca, q_c);
		//if(pa == pb&&bin->edge->sc != conn||pa != pb) printf("%d %d %d %d %d=%d\n", a, b, bin->edge->sc, conn, pa, pb);
#endif

		if(pa != pb) {

#ifdef _DEBUG_
			if(parent[pa] != -1&&weight[pa] > K&&parent[pb] != -1&&weight[pb] > K) {
				printf("WA3 in update_mSPT_decrement!\n");
				return ;
			}
			if(K != bin->edge->sc) {
				printf("WA7 in update_mSPT_decrement!\n");
				return ;
			}
#endif
			int conn, lca;
			conn = pair_connectivity(a, b, lca, q_c);
			if(conn != K-1) {
				printf("WA4 in update_mSPT_decrement %d %d!\n", conn, K-1);
				return ;
			}

			int ta = a, tt = -1;
			while(ta != lca) {
				if(weight[ta] == conn) {
					tt = ta;
					break;
				}
				ta = parent[ta];
			}
			if(tt == -1) {
				swap(a, b);
				ta = a;
				while(ta != lca) {
					if(weight[ta] == conn) {
						tt = ta;
						break;
					}
					ta = parent[ta];
				}
			}

			if(tt == -1) {
				printf("WA5 in update_mSPT_decrement!\n");
				return ;
			}

			swap_tree_edge(a, b, bin, tt, 0);
			if(pp[tt] == -1) Q[q_c ++] = tt;
			pp[tt] = b;

			//printf("3601 2890 %d\n", pair_connectivity(3601, 2890, lca, q_c));

			-- cnt;
		}

		bin = tmp;
	}

	for(int i = 0;i < q_c;i ++) pp[Q[i]] = -1;
}

void ConnGraph::update_mSPT_increment(const vector<Bin *> &incremented) {
	for(int i = 0;i < (int)incremented.size();i ++) {
		Bin *bin = incremented[i];

		int a = bin->edge->duplicate->node_id, b = bin->edge->node_id, c = bin->edge->sc;

		int conn, lca;
		conn = pair_connectivity(bin->edge->duplicate->node_id, bin->edge->node_id, lca, 0);

		if(conn >= c) continue;

		int ta = a, tt = -1;
		while(ta != lca) {
			if(weight[ta] == conn) {
				tt = ta;
				break;
			}
			ta = parent[ta];
		}
		if(tt == -1) {
			swap(a, b);
			ta = a;
			while(ta != lca) {
				if(weight[ta] == conn) {
					tt = ta;
					break;
				}
				ta = parent[ta];
			}
		}

		if(tt == -1) {
			printf("WA in update_mSPT_increment!\n");
			return ;
		}

#ifdef _DEBUG_
		if(a == b) printf("WA1 in update_mSPT_increment\n");
#endif

		swap_tree_edge(a, b, bin, tt, 0);
	}
}

void ConnGraph::remove_from_mSPT_bidirectional(Edge *tree_edge) {
	int a = tree_edge->duplicate->node_id, b = tree_edge->node_id;
	remove_edge(tnodes[a], tree_edge);
	remove_edge(tnodes[b], tree_edge->duplicate);
}

void ConnGraph::add_to_mSPT_bidirectional(int a, int b, int c, Edge *tree_edge) {
	tree_edge->node_id = b;
	tree_edge->sc = c;
	add_edge_sorted(tnodes[a], tree_edge);

	tree_edge = tree_edge->duplicate;
	tree_edge->node_id = a;
	tree_edge->sc = c;
	add_edge_sorted(tnodes[b], tree_edge);
}
//checked
void ConnGraph::reassign_edge_weights_delete(int a, int b, const vector<Element *> &cc, int K, vector<int> &decremented, int &deleted_vertex) {
	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = j+1;

	//printf("2868 3881 %d %d\n", computed[2868], computed[3881]);

	if(computed[a] != computed[b]) {
		int q_c = 1;
		Q[0] = a;
		vis[a] = 1;

		for(int i = 0;i < q_c;i ++) {
			Edge *e = nodes[Q[i]].first;
			for(;e != NULL&&e->sc > K;e = e->next) {
				if(!vis[e->node_id]) {
					vis[e->node_id] = 1;
					Q[q_c ++] = e->node_id;
				}
			}

			if(e == NULL||e->sc < K) continue;

			Edge *tail = e->pre;
			Edge *th = NULL, *tt = NULL;

			while(e != NULL&&e->sc == K) {
				Edge *tmp = e->next;

				if(!vis[e->node_id]) {
					vis[e->node_id] = 1;
					Q[q_c ++] = e->node_id;
				}

				if(computed[Q[i]] == computed[e->node_id]) {
					if(tail == NULL) {
						nodes[Q[i]].first = e;
						e->pre = NULL;
					}
					else {
						tail->next = e;
						e->pre = tail;
					}
					tail = e;
				}
				else {
					-- e->sc;
					if(tt == NULL) {
						th = e;
						e->pre = NULL;
					}
					else {
						tt->next = e;
						e->pre = tt;
					}
					tt = e;

					if(e->node_id > Q[i]) {

						if(parent[Q[i]] == e->node_id) {
							-- weight[Q[i]];
#ifdef _DEBUG_
							if(weight[Q[i]] != e->sc||e->sc != K-1) printf("WA10 in reassign_edge_weights_delete! %d %d\n", weight[Q[i]], e->sc);
#endif
							decrease_weight(pedges[Q[i]]);

							decremented.push_back(Q[i]);
						}
						else if(parent[e->node_id] == Q[i]) {
							-- weight[e->node_id];
#ifdef _DEBUG_
							if(weight[e->node_id] != e->sc||e->sc != K-1) printf("WA10 in reassign_edge_weights_delete! %d %d\n", weight[e->node_id], e->sc);
#endif
							decrease_weight(pedges[e->node_id]);

							decremented.push_back(e->node_id);
						}
						else {
#ifdef _DEBUG_
							if(e->nontree == NULL) printf("WA1 in reassign_edge_weights_delete!\n");
#endif
							delete_from_bucket(e->sc+1, e->nontree);
							insert_into_bucket(e->sc, e->nontree);
						}
					}
				}

				e = tmp;
			}

			if(th != NULL) {
				if(tail == NULL) {
					nodes[Q[i]].first = th;
					th->pre = NULL;
				}
				else {
					tail->next = th;
					th->pre = tail;
				}
				tail = tt;
			}
			if(e != NULL) {
				if(tail == NULL) {
					nodes[Q[i]].first = e;
					e->pre = NULL;
				}
				else {
					tail->next = e;
					e->pre = tail;
				}
			}
			else {
				nodes[Q[i]].last = tail;
				tail->next = NULL;
			}
		}

		for(int i = 0;i < q_c;i ++) vis[Q[i]] = 0;
	}

	deleted_vertex = -1;

	for(Edge *e = nodes[a].first;e != NULL;e = e->next) if(e->node_id == b) {
		if(parent[a] == b) deleted_vertex = a;
		else if(parent[b] == a) deleted_vertex = b;
		else {
#ifdef _DEBUG_
			if(e->nontree == NULL) printf("WA in reassign_edge_weights_delete!\n");
#endif
			delete_from_bucket(e->sc, e->nontree);
		}

		remove_edge(nodes[a], e);
		remove_edge(nodes[b], e->duplicate);

		break;
	}

	if(deleted_vertex != -1) {
		for(int i = 0;i < (int)decremented.size();i ++) if(decremented[i] == deleted_vertex) {
			decremented[i] = decremented.back();
			decremented.pop_back();
			break;
		}
	}

	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = 0;
}

void ConnGraph::decrease_weight(Node &node, Edge *edge) {
	-- edge->sc;

	if(edge->next == NULL||edge->next->sc <= edge->sc) return ;

	if(edge->pre == NULL) {
		node.first = edge->next;
		edge->next->pre = NULL;
	}
	else {
		edge->pre->next = edge->next;
		edge->next->pre = edge->pre;
	}

	Edge *e = edge->next, *pe;
	while(e != NULL&&e->sc > edge->sc) {
		pe = e;
		e = e->next;
	}
	edge->pre = pe;
	edge->next = e;
	if(e != NULL) e->pre = edge;
	else node.last = edge;
	pe->next = edge;
}

void ConnGraph::decrease_weight(Edge *edge) {
	decrease_weight(tnodes[edge->duplicate->node_id], edge);
	decrease_weight(tnodes[edge->node_id], edge->duplicate);
}

void ConnGraph::increase_weight(Node &node, Edge *edge) {
	++ edge->sc;

	if(edge->pre == NULL||edge->pre->sc >= edge->sc) return ;

	edge->pre->next = edge->next;
	if(edge->next != NULL) edge->next->pre = edge->pre;
	else node.last = edge->pre;

	Edge *e = edge->pre, *pe;
	while(e != NULL&&e->sc < edge->sc) {
		pe = e;
		e = e->pre;
	}

	edge->pre = e;
	edge->next = pe;
	pe->pre = edge;
	if(e != NULL) e->next = edge;
	else node.first = edge;
}


void ConnGraph::increase_weight(Edge *edge) {
	increase_weight(tnodes[edge->duplicate->node_id], edge);
	increase_weight(tnodes[edge->node_id], edge->duplicate);
}

void ConnGraph::reassign_edge_weights_insert(int a, int b, const vector<Element *> &cc, int K, vector<Bin *> &incremented) {
	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = j+1;

	//printf("2868 3881 %d %d\n", computed[2868], computed[3881]);

	//for(Edge *e = nodes[837].first;e != NULL;e = e->next) printf("837 %d %d\n", e->node_id, e->sc);

	//printf("vis[837] %d\n", vis[837]);

	if(computed[a] == computed[b]) {
		int q_c = 1;
		Q[0] = a;
		vis[a] = 1;

		for(int i = 0;i < q_c;i ++) {
			Edge *e = nodes[Q[i]].first;
			for(;e != NULL&&e->sc > K;e = e->next) {
				if(!vis[e->node_id]) {
					vis[e->node_id] = 1;
					Q[q_c ++] = e->node_id;
				}
			}

			if(e == NULL||e->sc < K) continue;

			Edge *tail = e->pre;
			Edge *th = NULL, *tt = NULL;

			while(e != NULL&&e->sc == K) {
				Edge *tmp = e->next;

				if(!vis[e->node_id]) {
					vis[e->node_id] = 1;
					Q[q_c ++] = e->node_id;
				}

				//if(Q[i] == 837) printf("837 %d %d\n", e->node_id, e->sc);


				//if(Q[i] == 837&&e->node_id == 171) printf("1 837 171 %d\n", e->sc);
				//if(Q[i] == 171&&e->node_id == 837) printf("2 837 171 %d\n", e->sc);

				if(computed[Q[i]] == computed[e->node_id]) {
					++ e->sc;
					//if(Q[i] == 837&&e->node_id == 171) printf("837 171 %d\n", e->sc);
					if(tail == NULL) {
						nodes[Q[i]].first = e;
						e->pre = NULL;
					}
					else {
						tail->next = e;
						e->pre = tail;
					}
					tail = e;

					if(e->node_id > Q[i]) {
						if(parent[Q[i]] == e->node_id) {
#ifdef _DEBUG_
							if(weight[Q[i]] != K) printf("WA10 in reassign_edge_weights_insert!\n");
#endif
							++ weight[Q[i]];
							increase_weight(pedges[Q[i]]);
						}
						else if(parent[e->node_id] == Q[i]) {
#ifdef _DEBUG_
							if(weight[e->node_id] != K) printf("WA10 in reassign_edge_weights_insert!\n");
							//if(e->node_id == 837) printf("%d %d\n", Q[i], e->node_id);
#endif
							++ weight[e->node_id];
							increase_weight(pedges[e->node_id]);
						}
						else {
#ifdef _DEBUG_
							if(e->nontree == NULL) printf("WA1 in reassign_edge_weights_insert!\n");
#endif
							delete_from_bucket(e->sc-1, e->nontree);
							insert_into_bucket(e->sc, e->nontree);

							incremented.push_back(e->nontree);
						}
					}
				}
				else {
					if(tt == NULL) {
						th = e;
						e->pre = NULL;
					}
					else {
						tt->next = e;
						e->pre = tt;
					}
					tt = e;
				}

				e = tmp;
			}

			if(th != NULL) {
				if(tail == NULL) {
					nodes[Q[i]].first = th;
					th->pre = NULL;
				}
				else {
					tail->next = th;
					th->pre = tail;
				}
				tail = tt;
			}
			if(e != NULL) {
				if(tail == NULL) {
					nodes[Q[i]].first = e;
					e->pre = NULL;
				}
				else {
					tail->next = e;
					e->pre = tail;
				}
			}
			else {
				nodes[Q[i]].last = tail;
				tail->next = NULL;
			}
		}

		for(int i = 0;i < q_c;i ++) vis[Q[i]] = 0;

		++ K;
	}

	Bin *bin = &bins[edge_n/2];
	bin->edge = &edges[edge_n];
	bin->pre = NULL;
	bin->next = buckets[K];
	if(buckets[K] != NULL) buckets[K]->pre = bin;
	buckets[K] = bin;
	incremented.push_back(bin);


	edges[edge_n].node_id = b;
	edges[edge_n].sc = K;
	edges[edge_n].deleted = 0;
	edges[edge_n].nontree = bin;
	edges[edge_n].duplicate = &edges[edge_n+1];
	add_edge_sorted(nodes[a], &edges[edge_n]);
	++ edge_n;

	edges[edge_n].node_id = a;
	edges[edge_n].sc = K;
	edges[edge_n].deleted = 0;
	edges[edge_n].nontree = bin;
	edges[edge_n].duplicate = &edges[edge_n-1];
	add_edge_sorted(nodes[b], &edges[edge_n]);
	++ edge_n;

	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = 0;
}
//checked
void ConnGraph::delete_from_bucket(int pre_key, Bin *bin) {
#ifdef _DEBUG_
	int ok = 0;
	for(Bin *b = buckets[pre_key];b != NULL;b = b->next) if(b == bin) {
		ok = 1;
		break;
	}
	if(!ok) printf("WA1 in delete_from_bucket");
#endif

	if(bin->pre == NULL) {
#ifdef _DEBUG_
		if(bin != buckets[pre_key]) {
			printf("WA in delete_from_bucket! %d %d\n", bin, buckets[pre_key]);
			for(Bin *b = buckets[pre_key];b != NULL;b = b->next) {
				printf("%d %d %d\n", b->pre, b, b->next);
				if(b == bin) break;
			}
		}
#endif
		buckets[pre_key] = bin->next;
		if(buckets[pre_key] != NULL) buckets[pre_key]->pre = NULL;
	}
	else {
		bin->pre->next = bin->next;
		if(bin->next != NULL) bin->next->pre = bin->pre;
	}
}
//checked
void ConnGraph::insert_into_bucket(int key, Bin *bin) {
#ifdef _DEBUG_
	if(key != bin->edge->sc&&key != bin->edge->duplicate->sc) printf("WA in insert_into_bucket\n");
#endif

	bin->next = buckets[key];
	bin->pre = NULL;
	if(buckets[key] != NULL) buckets[key]->pre = bin;
	buckets[key] = bin;

	if(key > max_key) max_key = key;
}

void ConnGraph::update_bucket_structure(int pre_key, Bin *bin) {
	delete_from_bucket(pre_key, bin);
	insert_into_bucket(bin->edge->sc, bin);
}

void ConnGraph::decompose(int ss, vector<Element *> &cc, int K) {
	cc.clear();

	while(snodes[ss].first != NULL) {
		int s = ss;
		heap->insert(s, 0);

		int q_c = 0;
		int key;

		while(heap->extract_max(s, key)) {
			computed[s] = 1;
			Q[q_c ++] = s;

			int temp_qc = q_c;
			for(int i = q_c - 1;i < temp_qc;i ++) {
				int u = Q[i];
				for(Edge *e = snodes[u].first;e != NULL;e = e->next) if(!computed[e->node_id]) {
					int new_key = heap->get_key(e->node_id);
					if(new_key < K) {
						if(new_key > 0) heap->remove(e->node_id);

						new_key += e->sc;
						if(new_key >= K) {
							heap->set_key(e->node_id, new_key);
							Q[temp_qc ++] = e->node_id;
						}
						else heap->insert(e->node_id, new_key);
					}
					else heap->set_key(e->node_id, new_key + e->sc);
				}

				if(u == s) continue;

				heap->set_key(s, heap->get_key(s) + heap->get_key(u));
				heap->set_key(u, 0);
				computed[u] = 0;
				merge_sgraph(s, u, heap);
			}
		}

		-- q_c;
		while(q_c > 0&&heap->get_key(Q[q_c]) < K) {
			int t = Q[q_c]; -- q_c;
			cc.push_back(snodes[t].head);

			heap->set_key(t, 0);
			computed[t] = 0;

			for(Edge *e = snodes[t].first;e != NULL;e = e->next) remove_edge(snodes[e->node_id], e->duplicate);

			snodes[t].first = NULL;
		}

		for(int i = 0;i <= q_c;i ++) {
			heap->set_key(Q[i], 0);
			computed[Q[i]] = 0;
		}
	}

	cc.push_back(snodes[ss].head);
}

void ConnGraph::merge_sgraph(int s, int t, Heap *heap) {
	snodes[s].tail->next = snodes[t].head;
	snodes[s].tail = snodes[t].tail;

	Edge *e = snodes[t].first;
	Edge *tmp;

	while(e != NULL) {
		tmp = e->next;

		if(e->node_id == s) {
			if(heap != NULL) heap->set_key(s, heap->get_key(s) - e->sc);
			remove_edge(snodes[e->node_id], e->duplicate);
		}
		else {
			e->duplicate->node_id = s;
			add_edge(snodes[s], e);
		}

		e = tmp;
	}

	snodes[t].first = NULL;
}

//checked
void ConnGraph::remove_edge(Node &node, Edge *edge) {
	if(edge->pre == NULL) {
#ifdef _DEBUG_
		if(node.first != edge) printf("WA1 in remove_edge!\n");
#endif
		node.first = edge->next;
		if(edge->next != NULL) edge->next->pre = NULL;
	}
	else {
		Edge *tmp = edge->pre;
		tmp->next = edge->next;

		if(edge->next != NULL) edge->next->pre = tmp;
	}

#ifdef _DEBUG_
	if(edge->next == NULL&&edge != node.last) printf("WA2 in remove_edge!\n");
#endif

	if(edge == node.last) node.last = edge->pre;
}

//checked
int ConnGraph::pair_connectivity(int a, int b, int &lca, int q_c) {
#ifdef _DEBUG_
	if(a == b) {
		printf("a == b \n");
		return 100000;
	}

	for(int i = 0;i < n;i ++) if(computed[i] != 0) {
		printf("WA in pair_connectivity!\n");
		break;
	}
#endif

	int new_qc = q_c+2;
	Q[q_c] = a; Q[q_c+1] = b;
	computed[a] = n; computed[b] = n;

	int w_a = n, w_b = n, res = -1;

	while(1) {
		if(parent[a] == -1||(parent[b] != -1&&w_a < w_b)) {
#ifdef _DEBUG_
			if(weight[b] <= 0) printf("WA3 in pair_connectivity!\n");
#endif
			if(weight[b] < w_b) w_b = weight[b];
			b = parent[b];

			if(computed[b] == 0) {
				computed[b] = w_b;
				Q[new_qc ++] = b;
			}
			else {
				lca = b;
				res = computed[b];
				if(w_b < res) res = w_b;
				break;
			}
		}
		else {
#ifdef _DEBUG_
			if(parent[a] == -1) {
				printf("WA in compute pair connectivity!\n");
				return -1;
			}
			if(weight[a] <= 0) printf("WA4 in pair_connectivity! %d %d %d\n", a, parent[a], weight[a]);
#endif
			if(weight[a] < w_a) w_a = weight[a];
			a = parent[a];

			if(computed[a] == 0) {
				computed[a] = w_a;
				Q[new_qc ++] = a;
			}
			else {
				lca = a;
				res = computed[a];
				if(w_a < res) res = w_a;
				break;
			}
		}
	}

	for(int i = q_c;i < new_qc;i ++) computed[Q[i]] = 0;

#ifdef _DEBUG_
	for(int i = 0;i < n;i ++) if(computed[i] != 0) {
		printf("WA in pair_connectivity!\n");
		break;
	}
	if(res <= 0) {
		printf("WA2 in pair_connectivity %d!\n", res);
	}
#endif

	return res;
}

void ConnGraph::test_kSC(int K) { //wrong, need to run decompose multiple times
	int sedge_c = 0;
	for(int i = 0;i < n;i ++) {
		snodes[i].head = snodes[i].tail = &elements[i];
		elements[i].next = NULL;

		for(Edge *e = nodes[i].first;e != NULL;e = e->next) {
			//if(e->sc < K-1) break;

			if(e->node_id > i) {
				int ta = i, tb = e->node_id, c = e->sc;

#ifdef _DEBUG_
				c = 1;
#endif

				sedges[sedge_c].node_id = tb;
				sedges[sedge_c].sc = c;
				sedges[sedge_c].duplicate = &sedges[sedge_c+1];
				add_edge(snodes[ta], &sedges[sedge_c]);
				++ sedge_c;

				sedges[sedge_c].node_id = ta;
				sedges[sedge_c].sc = c;
				sedges[sedge_c].duplicate = &sedges[sedge_c-1];
				add_edge(snodes[tb], &sedges[sedge_c]);
				++ sedge_c;
			}
		}
	}

	vector<Element *> cc;
	decompose(0, cc, K);

	FILE *fout = open_file((dir+"/log.txt").c_str(), "w");

	for(int i = 0;i < (int)cc.size();i ++) {
		vector<int> vp;
		for(Element *e = cc[i];e != NULL;e = e->next) vp.push_back(e->value);

		if(vp.size() == 1) continue;

		sort(vp.begin(), vp.end());
		fprintf(fout, "%d", vp[0]);
		for(int j = 1;j < (int)vp.size();j ++) fprintf(fout, ", %d", vp[j]);
		fprintf(fout, "\n");
	}

	fclose(fout);
}

void ConnGraph::extract_subgraph(int s) {
	int pedge_c = 0;
	int q_c = 1;

	computed[s] = 1;
	Q[0] = s;

	for(int i = 0;i < q_c;i ++) {
		s = Q[i];

		snodes[s].head = snodes[s].tail = &elements[s];
		elements[s].next = NULL;

		for(Edge *edge = tmp_nodes[s].first;edge != NULL&&!edge->deleted;edge = edge->next) {
			if(!computed[edge->node_id]) {
				computed[edge->node_id] = 1;
				Q[q_c] = edge->node_id;
				++ q_c;
			}

			if(edge->node_id > s) {
				int a = s, b = edge->node_id;

				sedges[pedge_c].node_id = b;
				sedges[pedge_c].sc = edge->sc;
				sedges[pedge_c].duplicate = &sedges[pedge_c+1];
				add_edge(snodes[a], &sedges[pedge_c]);
				++ pedge_c;

				sedges[pedge_c].node_id = a;
				sedges[pedge_c].sc = edge->sc;
				sedges[pedge_c].duplicate = &sedges[pedge_c-1];
				add_edge(snodes[b], &sedges[pedge_c]);
				++ pedge_c;
			}
		}
	}

	for(int i = 0;i < q_c;i ++) computed[Q[i]] = 0;
}

//checked
int ConnGraph::extract_scp(int a, int b, int &sedge_c, int q_c, vector<int> &vv) {
	int lca;
	int res = pair_connectivity(a, b, lca, 0);

	Q[q_c] = a; computed[a] = 1;
	int new_qc = q_c + 1;

	for(int i = q_c;i < new_qc;i ++) {
		int s = Q[i];

		//snodes[s].head = snodes[s].tail = &elements[s];
		//elements[s].next = NULL;

		for(Edge *e = nodes[s].first;e != NULL;e = e->next) {
			if(e->sc < res) break;

			if(e->node_id > s) {
				int ta = s, tb = e->node_id, c = e->sc;

				if(c == res) c = 1;
#ifdef _DEBUG_
				c = 1;
#endif

				tmp_edges[sedge_c].node_id = tb;
				tmp_edges[sedge_c].sc = c;
				tmp_edges[sedge_c].deleted = 0;
				tmp_edges[sedge_c].duplicate = &tmp_edges[sedge_c+1];
				add_edge(tmp_nodes[ta], &tmp_edges[sedge_c]);
				++ sedge_c;

				tmp_edges[sedge_c].node_id = ta;
				tmp_edges[sedge_c].sc = c;
				tmp_edges[sedge_c].deleted = 0;
				tmp_edges[sedge_c].duplicate = &tmp_edges[sedge_c-1];
				add_edge(tmp_nodes[tb], &tmp_edges[sedge_c]);
				++ sedge_c;
			}

			if(!computed[e->node_id]) {
				computed[e->node_id] = 1;
				Q[new_qc ++] = e->node_id;
			}
		}
	}

	vv.reserve(new_qc - q_c);
	for(int i = q_c;i < new_qc;i ++) {
		computed[Q[i]] = 0;
		vv.push_back(Q[i]);
	}

	return res;
}

//checked
void ConnGraph::add_edge(Node &node, Edge *edge) {
	edge->next = NULL;

	if(node.first == NULL) {
		node.first = node.last = edge;
		edge->pre = NULL;
	}
	else {
		node.last->next = edge;
		edge->pre = node.last;
		node.last = edge;
	}
}

//checked
void ConnGraph::add_edge_sorted(Node &node, Edge *edge) {
	if(node.first == NULL||node.first->sc <= edge->sc) {
		edge->pre = NULL;
		edge->next = node.first;
		if(node.first != NULL) node.first->pre = edge;
		else node.last = edge;
		node.first = edge;

		return ;
	}

	Edge *e = node.first, *pe;
	while(e != NULL&&e->sc > edge->sc) {
		pe = e;
		e = e->next;
	}

	edge->pre = pe;
	edge->next = e;
	if(e != NULL) e->pre = edge;
	else node.last = edge;
	pe->next = edge;
}
