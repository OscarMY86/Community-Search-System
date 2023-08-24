#include "Utility.h"
#include "Graph.h"

Graph::Graph(const char *_dir) {
	dir = string(_dir);

	K = -1;

	nodes = NULL;
	edges = NULL;
	pnodes = NULL;
	pedges = NULL;

	inL = NULL;
	computed = NULL;
	height = NULL;

	elements = NULL;

	q = NULL;

	degrees = NULL;

	heap = NULL;

	cids = NULL;
	levels = NULL;

	parent = NULL;
	weights = NULL;

	pos_id = NULL;
	st = NULL;
	t_id = NULL;
	table = NULL;

	log_table = NULL;
}

Graph::~Graph() {
	if(nodes != NULL) delete[] nodes;
	if(edges != NULL) delete[] edges;
	if(pnodes != NULL) delete[] pnodes;
	if(pedges != NULL) delete[] pedges;

	if(inL != NULL) delete[] inL;
	if(computed != NULL) delete[] computed;
	if(height != NULL) delete[] height;

	if(elements != NULL) delete[] elements;

	if(q != NULL) delete[] q;

	if(degrees != NULL) delete[] degrees;

	if(heap != NULL) delete heap;

	if(cids != NULL) delete[] cids;
	if(levels != NULL) delete[] levels;

	if(parent != NULL) delete[] parent;
	if(weights != NULL) delete[] weights;

	if(pos_id != NULL) delete[] pos_id;
	if(st != NULL) {
		for(int i = 0;i < logns;i ++) {
			delete[] st[i];
			st[i] = NULL;
		}
		delete[] st;
	}
	if(t_id != NULL) delete[] t_id;
	if(table != NULL) {
		for(int i = 0;i < nt;i ++) {
			delete[] table[i];
			table[i] = NULL;
		}
		delete[] table;
	}

	if(log_table != NULL) delete[] log_table;
}

void Graph::read_graph(int binary) {
	FILE *f;
	if(binary) f = open_file((dir + string("/edges.bin")).c_str(), "rb");
	else f = open_file((dir + string("/edges.txt")).c_str(), "r");

	if(binary) {
		fread(&n, sizeof(int), 1, f);
		fread(&m, sizeof(int), 1, f);
	}
	else fscanf(f, "%d%d", &n, &m);

	nodes = new Node[n];
	edges = new Edge[2*m];

	q = new int[n];

	int edge_c = 0;

	for(int i = 0;i < n;i ++) nodes[i].first = NULL;

	int *buf = NULL;

	if(binary) {
		buf = new int[2*n+m];
		fread(buf, sizeof(int), 2*n+m, f);

		int j = 0;
		for(int i = 0;i < n;i ++) {
			if(buf[j] != i) {
				printf("WA in read binary input graph!\n");
				return ;
			}

			int d = buf[j+1];
			j += 2;
			while(d --) {
				int a = i, b = buf[j];
				++ j;

				edges[edge_c].node_id = b;
				edges[edge_c].sc = 1;
				edges[edge_c].deleted = 0;
				edges[edge_c].duplicate = &edges[edge_c+1];
				add_edge(nodes[a], &edges[edge_c]);
				++ edge_c;

				edges[edge_c].node_id = a;
				edges[edge_c].sc = 1;
				edges[edge_c].deleted = 0;
				edges[edge_c].duplicate = &edges[edge_c-1];
				add_edge(nodes[b], &edges[edge_c]);
				++ edge_c;
			}
		}
	}

	else {

		for(int i = 0;i < m;i ++) {
			int a, b;
			fscanf(f, "%d%d", &a, &b);

			edges[edge_c].node_id = b;
			edges[edge_c].sc = 1;
			edges[edge_c].deleted = 0;
			edges[edge_c].duplicate = &edges[edge_c+1];
			add_edge(nodes[a], &edges[edge_c]);
			++ edge_c;

			edges[edge_c].node_id = a;
			edges[edge_c].sc = 1;
			edges[edge_c].deleted = 0;
			edges[edge_c].duplicate = &edges[edge_c-1];
			add_edge(nodes[b], &edges[edge_c]);
			++ edge_c;
		}
	}

	fclose(f);

	if(buf != NULL) delete[] buf;

	pnodes = new Node[n];
	pedges = new Edge[2*m];

	for(int i = 0;i < n;i ++) pnodes[i].first = NULL;

	elements = new Element[n];
	for(int i = 0;i < n;i ++) elements[i].value = i;
}

void Graph::gen_p2p_query(const char *thresh) {
	srand(time(NULL));

	string output_name = dir;
	if(thresh[0] == '-') output_name += "/p2p-query.txt";
	else output_name += "/p2p-query-" + string(thresh) + ".txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	int threshold = atoi(thresh);

	if(threshold == -1) {
		int t = 10000;

		fprintf(fout, "%d\n", t);
		for(int i = 0;i < t;i ++) {
			int a, b;
			do {
				a = rand()%n;
				b = rand()%n;
			}while(a == b||cids[a] != cids[b]);

			fprintf(fout, "%d %d\n", a, b);
		}

		fclose(fout);
		return ;
	}

	vector<int> id(n);
	for(int i = 0;i < n;i ++) id[i] = i;
	for(int i = 0;i < 100;i ++) random_shuffle(id.begin(), id.end());

	vector<pair<int,int> > vp;
	vp.reserve(1000000);

	char *vis = new char[n];
	int *q = new int[n];
	int *d = new int[n];
	memset(vis, 0, sizeof(char)*n);

	for(int i = 0;i < n;i ++) {
		vis[id[i]] = 1;
		q[0] = id[i];
		d[id[i]] = 0;
		int q_c = 1;
		for(int j = 0;j < q_c&&vp.size() < 1000000-100;j ++) {
			if(d[q[j]] >= threshold+5) break;
			for(Edge *e = pnodes[q[j]].first;e != NULL;e = e->next) if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				d[e->node_id] = d[q[j]]+1;
				q[q_c++] = e->node_id;

				if(d[e->node_id] >= threshold&&d[e->node_id] < threshold+5) vp.push_back(make_pair(id[i], e->node_id));
			}
		}

		for(int j = 0;j < q_c;j ++) vis[q[j]] = 0;
	}

	int t = 10000;

	if(vp.empty()) {
		fprintf(fout, "0\n");
		fclose(fout);
		printf("WA in gen_p2p_query!\n");
		return ;
	}

	fprintf(fout, "%d\n", t);
	for(int i = 0;i < t;i ++) {
		int x = rand()%vp.size();

		fprintf(fout, "%d %d\n", vp[x].first, vp[x].second);
	}

	//printf("Maximum number of edges in mSPT-based approach: %d\n", mcnt);
	//for(int i = 0;i < t;i ++) fprintf(fout, "%d %d\n", ma, mb);

	fclose(fout);

	delete[] vis;
	delete[] q;
	delete[] d;
}

void Graph::gen_tcse_query(const char *thresh) {
	srand(time(NULL));

	string output_name = dir;
	if(thresh[0] == '-') output_name += "/tcse-query.txt";
	else output_name += "/tcse-query-" + string(thresh) + ".txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	int threshold = atoi(thresh);

	vector<pair<int,int> > vp;

	for(int i = 0;i < n;i ++) {
		if(pnodes[i].first != NULL&&pnodes[i].first->sc >= 2) vp.push_back(make_pair(pnodes[i].first->sc, i));
	}

	sort(vp.begin(), vp.end());

	int begin = 0, end = (int)vp.size();

	if(threshold > vp[end-1].first) {
		printf("WA threshold in gen_tcse_query!\n");
		return ;
	}

	if(threshold != -1) {
		for(int i = 0;i < (int)vp.size();i ++) if(vp[i].first >= threshold) {
			begin = i;
			break;
		}
	}

	int t = 10000;

	fprintf(fout, "%d\n", t);

	for(int i = 0;i < t;i ++) {
		int a = rand()%(end-begin) + begin, b;
		int maxk = vp[a].first+1;
		if(threshold != -1&&maxk > threshold+10) maxk = threshold+10;

		if(threshold == -1||threshold == 0) {
			b = 2;
			if(maxk > 2) b = rand()%(maxk-2) + 2;
		}
		else {
			b = threshold;
			if(maxk > threshold) b = rand()%(maxk - threshold) + threshold;
		}

		fprintf(fout, "%d %d\n", vp[a].second, b);
	}

	fclose(fout);
}

void Graph::gen_mcse_query() {
	srand(time(NULL));

	string output_name = dir + "/mcse-query.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	vector<pair<int,int> > vp;

	for(int i = 0;i < n;i ++) {
		if(pnodes[i].first != NULL&&pnodes[i].first->sc >= 3) vp.push_back(make_pair(pnodes[i].first->sc, i));
	}

	int t = 10000;

	fprintf(fout, "%d\n", t);

	for(int i = 0;i < t;i ++) {
		int a = rand()%vp.size();

		fprintf(fout, "%d\n", vp[a].second);
	}

	fclose(fout);
}

void Graph::gen_scse_query() {
	srand(time(NULL));

	string output_name = dir + "/scse-query.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	vector<pair<int,int> > vp;

	char *vis = new char[n];
	memset(vis, 0, sizeof(char)*n);
	int *q = new int[n];

	int *cnt = new int[n];
	for(int i = 0;i < n;i ++) cnt[i] = -1;

	for(int i = 0;i < n;i ++) {
		if(cnt[i] != -1) continue;

		if(pnodes[i].first != NULL&&pnodes[i].first->sc >= 3) {
			int q_c = 1;
			q[0] = i;
			vis[i] = 1;
			for(int j = 0;j < q_c;j ++) {
				int a = q[j];
				for(Edge *e = pnodes[a].first;e != NULL&&e->sc >= 3;e = e->next) if(!vis[e->node_id]) {
					vis[e->node_id] = 1;
					q[q_c ++] = e->node_id;
				}
			}

			for(int j = 0;j < q_c;j ++) {
				vp.push_back(make_pair(q_c, q[j]));
				cnt[q[j]] = q_c;
			}
		}
	}

	delete[] vis;
	delete[] q;
	delete[] cnt;

	int t = 10000;

	fprintf(fout, "%d\n", t);

	for(int i = 0;i < t;i ++) {
		int a = rand()%vp.size();
		int b = 3;
		if(vp[a].first > 3) b = rand()%(vp[a].first-2) + 3;

		fprintf(fout, "%d %d\n", vp[a].second, b);
	}

	fclose(fout);
}

void Graph::merge(int s, int t, Heap *heap) {
	pnodes[s].tail->next = pnodes[t].head;
	pnodes[s].tail = pnodes[t].tail;

	Edge *e = pnodes[t].first;
	Edge *tmp;

	while(e != NULL) {
		tmp = e->next;

		if(e->node_id == s) {
			if(heap != NULL) heap->set_key(s, heap->get_key(s) - e->sc);
			delete_edge(pnodes[e->node_id], e->duplicate);
		}
		else {
			e->duplicate->node_id = s;
			add_edge(pnodes[s], e);
		}

		e = tmp;
	}

	pnodes[t].first = NULL;
}

void Graph::count_cc() {
	int cnt = 0;

	if(computed == NULL) computed = new int[n];
	memset(computed, 0, sizeof(int)*n);

	int q_c = 0;

	for(int i = 0;i < n;i ++) {
		if(computed[i]) continue;

		++ cnt;

		q[0] = i;
		q_c = 1;
		computed[i] = 1;

		for(int j = 0;j < q_c;j ++) {
			for(Edge *e = nodes[q[j]].first;e != NULL;e = e->next) if(!computed[e->node_id]) {
				computed[e->node_id] = 1;
				q[q_c++] = e->node_id;
			}
		}
	}

	memset(computed, 0, sizeof(int)*n);

	printf("Number of connected components: %d\n", cnt);
}

void Graph::extract_mcc() {
	int cnt = 0;

	if(computed == NULL) computed = new int[n];
	memset(computed, 0, sizeof(int)*n);

	int q_c = 0;

	int sa, ss = -1;

	for(int i = 0;i < n;i ++) {
		if(computed[i]) continue;

		++ cnt;

		q[0] = i;
		q_c = 1;
		computed[i] = 1;

		for(int j = 0;j < q_c;j ++) {
			for(Edge *e = nodes[q[j]].first;e != NULL;e = e->next) if(!computed[e->node_id]) {
				computed[e->node_id] = 1;
				q[q_c++] = e->node_id;
			}
		}

		if(q_c > ss) {
			ss = q_c;
			sa = i;
		}
	}

	memset(computed, 0, sizeof(int)*n);

	q[0] = sa; computed[sa] = 1;
	q_c = 1;

	cnt = 0;
	for(int j = 0;j < q_c;j ++) {
		for(Edge *e = nodes[q[j]].first;e != NULL;e = e->next) {
			++ cnt;
			if(!computed[e->node_id]) {
				computed[e->node_id] = 1;
				q[q_c++] = e->node_id;
			}
		}
	}

	sort(q, q+q_c);

	int *id = new int[n];
	for(int i = 0;i < n;i ++) id[i] = -1;
	for(int i = 0;i < q_c;i ++) id[q[i]] = i;

	cnt /= 2;

	printf("n = %d, m = %d; nn = %d, nm = %d\n", n, m, q_c, cnt);

	FILE *fout = open_file((dir + string("/edges_mcc.txt")).c_str(), "w");

	fprintf(fout, "%d %d\n", q_c, cnt);
	for(int i = 0;i < q_c;i ++) for(Edge *e = nodes[q[i]].first;e != NULL;e = e->next) if(e->node_id > q[i]) {
		if(id[q[i]] == id[e->node_id]) printf("WA in extract_mcc!\n");

		fprintf(fout, "%d %d\n",  id[q[i]], id[e->node_id]);
	}

	fclose(fout);

	delete[] id;
}

void Graph::min_max_conn() {
	int mi = 100000, ma = 0;
	for(int i = 0;i < n;i ++) for(Edge *e = pnodes[i].first;e != NULL;e = e->next) {
		if(e->sc > ma) ma = e->sc;
		if(e->sc < mi) mi = e->sc;
	}

	printf("The minimum connectivity is: %d\n", mi);
	printf("The maximum connectivity is: %d\n", ma);
}

void Graph::check_connectivity(char *file_name) {
	char buf[128];
	FILE *fin = open_file((dir+string("/") + string(file_name)).c_str(), "r");

	vector<int> cc;
	char *in_cc = new char[n];
	memset(in_cc, 0, sizeof(char)*n);

	while(fscanf(fin, "%s", buf) == 1) {
		cc.clear();

		while(buf[strlen(buf)-1] == ',') {
			buf[strlen(buf)-1] = '\0';
			cc.push_back(atoi(buf));
			fscanf(fin, "%s", buf);
		}
		cc.push_back(atoi(buf));

		for(int i = 0;i < (int)cc.size();i ++) in_cc[cc[i]] = 1;
		for(int i = 0;i < (int)cc.size();i ++) pnodes[cc[i]].first = NULL;

		for(int i = 0;i < (int)cc.size();i ++) {
			if(i) printf(",");
			printf("%d", cc[i]);
		}
		printf("\n");

		int pedge_c = 0;

		for(int i = 0;i < (int)cc.size();i ++) for(Edge *e = nodes[cc[i]].first;e != NULL;e = e->next) {
			if(in_cc[e->node_id]&&e->node_id > cc[i]) {
				int a = cc[i], b = e->node_id;

				pedges[pedge_c].node_id = b;
				pedges[pedge_c].sc = 1;
				pedges[pedge_c].duplicate = &pedges[pedge_c+1];
				add_edge(pnodes[a], &pedges[pedge_c]);
				++ pedge_c;

				pedges[pedge_c].node_id = a;
				pedges[pedge_c].sc = 1;
				pedges[pedge_c].duplicate = &pedges[pedge_c-1];
				add_edge(pnodes[b], &pedges[pedge_c]);
				++ pedge_c;
			}
		}

		int low = 10000;

		for(int i = 0;i < (int)cc.size();i ++) for(int j = i+1;j < (int)cc.size();j ++) {
			int q_c = 0;
			int tmp = max_flow(cc[i], cc[j], pnodes, 0, q_c);
			if(tmp < low) low = tmp;
		}

		printf("%d\n", low);

		for(int i = 0;i < (int)cc.size();i ++) pnodes[cc[i]].first = NULL;
		for(int i = 0;i < (int)cc.size();i ++) in_cc[cc[i]] = 0;
	}

	delete[] in_cc;

	fclose(fin);
}

void Graph::decomposition(int ss, vector<Element *> &cc, int &max_l) {
	if(heap == NULL) heap = new LinearHeap(n);

	if(inL == NULL) {
		inL = new char[n];
		memset(inL, 0, sizeof(char)*n);
	}

	cc.clear();

	int cnt = 0;

	while(pnodes[ss].first != NULL) {
		int s = ss;

		++ cnt;

		heap->insert(s, 0);

		int q_c = 0;
		int key;

		while(1) {

			if(!heap->extract_max(s, key)) break;

			inL[s] = 1;

			q[q_c] = s;
			++ q_c;

			int new_qc = q_c;
			for(int i = q_c - 1;i < new_qc;i ++) {
				int u = q[i];
				for(Edge *e = pnodes[u].first;e != NULL;e = e->next) if(!inL[e->node_id]) {
					int new_key = heap->get_key(e->node_id);
					if(new_key < K) {
						if(new_key > 0) heap->remove(e->node_id);

						new_key += e->sc;
						if(new_key >= K) {
							heap->set_key(e->node_id, new_key);
							q[new_qc ++] = e->node_id;
						}
						else heap->insert(e->node_id, new_key);
					}
					else heap->set_key(e->node_id, new_key + e->sc);
				}

				if(u == s) continue;

				heap->set_key(s, heap->get_key(s) + heap->get_key(u));
				heap->set_key(u, 0);
				inL[u] = 0;
				merge(s, u, heap);
			}
		}

		-- q_c;
		while(q_c > 0&&heap->get_key(q[q_c]) < K) {
			int t = q[q_c]; -- q_c;
			cc.push_back(pnodes[t].head);

			heap->set_key(t, 0);
			inL[t] = 0;

			for(Edge *e = pnodes[t].first;e != NULL;e = e->next) delete_edge(pnodes[e->node_id], e->duplicate);

			pnodes[t].first = NULL;
		}

		for(int i = 0;i <= q_c;i ++) {
			heap->set_key(q[i], 0);
			inL[q[i]] = 0;
		}
	}

	if(cnt > max_l) max_l = cnt;

	cc.push_back(pnodes[ss].head);
}

void Graph::remove_inter_edges(const vector<Element *> &cc, int assign_sc) {
	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = j+1;

	int q_c = 0;

	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) {
		int s = e->value;
		Edge *list = nodes[s].first;
		Edge *delete_first = nodes[s].last->next;
		nodes[s].first = nodes[s].last = NULL;
		int cnt = 0;

		while(list != NULL&&!list->deleted) {
			Edge *tmp = list->next;
			if(computed[list->node_id] == computed[s]) {

				if(nodes[s].first == NULL) {
					nodes[s].first = nodes[s].last = list;
					list->pre = NULL;
				}
				else {
					nodes[s].last->next = list;
					list->pre = nodes[s].last;
					nodes[s].last = list;
				}

				++ cnt;
			}
			else {
				list->deleted = 1;

				if(assign_sc) list->sc = K-1;

				list->next = delete_first;
				if(delete_first != NULL) delete_first->pre = list;
				delete_first = list;
			}

			list = tmp;
		}

		degrees[s] = cnt;
		if(cnt < K) {
			q[q_c] = s;
			++ q_c;
		}

		if(nodes[s].first == NULL) {
			nodes[s].first = nodes[s].last = delete_first;
			if(delete_first != NULL) delete_first->pre = NULL;
		}
		else {
			nodes[s].last->next = delete_first;
			if(delete_first != NULL) delete_first->pre = nodes[s].last;
		}
	}

	for(int j = 0;j < (int)cc.size();j ++) for(Element *e = cc[j];e != NULL;e = e->next) computed[e->value] = 0;

	kcore_optimization(q_c, assign_sc);
}

int Graph::kSC(int _K) {
	K = _K;

	if(computed == NULL) computed = new int[n];
	if(height == NULL) height = new int[n];
	if(degrees == NULL) degrees = new int[n];

	memset(computed, 0, sizeof(int)*n);
	memset(height, 0, sizeof(int)*n);

	int q_c = 0;
	for(int i = 0;i < n;i ++) {
		int cnt = 0;
		for(Edge *edge = nodes[i].first;edge != NULL&&!edge->deleted;edge = edge->next) ++ cnt;

		if(cnt < K) {
			q[q_c] = i;
			++ q_c;
		}

		degrees[i] = cnt;
	}

	kcore_optimization(q_c);

	int max_l = 0, non_trivial = 0;

	for(int i = 0;i < n;i ++) {
		// if(i%10000 == 0) printf(".");

		if(computed[i]) continue;

		if(construct_pgraph(i, height) > 1) non_trivial = 1;

		vector<Element *> cc;
		decomposition(i, cc, max_l);

		if(cc.size() == 1) for(Element *e = cc[0];e != NULL;e = e->next) computed[e->value] = 1;
		else remove_inter_edges(cc);

		-- i;
	}

	int max_height = height[0];
	for(int i = 0;i < n;i ++) if(height[i] > max_height) max_height = height[i];

	// printf("K: %d, Height: %d, Max L: %d\n", K, max_height, max_l);

	return non_trivial;
}

int Graph::kSC_BU(int _K, int *id1, int n_id1, int *id2, int &n_id2) {
	K = _K;

	if(computed == NULL) computed = new int[n];
	if(height == NULL) height = new int[n];
	if(degrees == NULL) degrees = new int[n];

	//n_id1 = n;
	//for(int i = 0;i < n;i ++) id1[i] = i;
	//for(int i = 0;i < 100;i ++) random_shuffle(id1, id1+n);

	//sort(id1, id1+n_id1);

	for(int i = 0;i < n_id1;i ++) computed[id1[i]] = height[id1[i]] = 0;

	int q_c = 0;
	for(int i = 0;i < n_id1;i ++) {
		int cnt = 0;
		for(Edge *edge = nodes[id1[i]].first;edge != NULL&&!edge->deleted;edge = edge->next) ++ cnt;

		if(cnt < K) {
			q[q_c] = id1[i];
			++ q_c;
		}

		degrees[id1[i]] = cnt;
	}

	kcore_optimization(q_c, 1);

	int max_l = 0, non_trivial = 0;

	n_id2 = 0;

	for(int i = 0;i < n_id1;i ++) {
		// if(i%10000 == 0) printf(".");

		if(computed[id1[i]]) continue;

		if(construct_pgraph(id1[i], height) > 1) non_trivial = 1;

		vector<Element *> cc;
		decomposition(id1[i], cc, max_l);

		if(cc.size() == 1) {
			for(Element *e = cc[0];e != NULL;e = e->next) {
				computed[e->value] = 1;
				id2[n_id2 ++] = e->value;
			}
		}
		else remove_inter_edges(cc, 1);

		-- i;
	}

	return non_trivial;
}

int Graph::kSC_P2P(int _K, int u, int v) {
	K = _K;

	if(computed == NULL) computed = new int[n];
	if(height == NULL) height = new int[n];
	if(degrees == NULL) degrees = new int[n];

	for(int i = 0;i < n;i ++) computed[i] = height[i] = 0;

	int q_c = 0;
	for(int i = 0;i < n;i ++) {
		int cnt = 0;
		for(Edge *edge = nodes[i].first;edge != NULL&&!edge->deleted;edge = edge->next) ++ cnt;

		if(cnt < K) {
			q[q_c] = i;
			++ q_c;
		}

		degrees[i] = cnt;
	}

	kcore_optimization(q_c, 1);

	int max_l = 0;

	while(1) {
		if(computed[u]||computed[v]) return 0;

		construct_pgraph(u, height);

		vector<Element *> cc;
		decomposition(u, cc, max_l);

		if(cc.size() == 1) {
			for(Element *e = cc[0];e != NULL;e = e->next) if(e->value == v) return 1;
			return 0;
		}
		else remove_inter_edges(cc, 1);
	}

	return 0;
}

Element *Graph::kSC_tcse(int _K, int u) {
	K = _K;

	if(computed == NULL) computed = new int[n];
	if(height == NULL) height = new int[n];
	if(degrees == NULL) degrees = new int[n];

	for(int i = 0;i < n;i ++) computed[i] = height[i] = 0;

	int q_c = 0;
	for(int i = 0;i < n;i ++) {
		int cnt = 0;
		for(Edge *edge = nodes[i].first;edge != NULL&&!edge->deleted;edge = edge->next) ++ cnt;

		if(cnt < K) {
			q[q_c] = i;
			++ q_c;
		}

		degrees[i] = cnt;
	}

	kcore_optimization(q_c, 1);

	int max_l = 0;

	if(computed[u]) printf("WA in kSC_tcse\n");

	while(1) {
		construct_pgraph(u, height);

		vector<Element *> cc;
		decomposition(u, cc, max_l);

		if(cc.size() == 1) return cc[0];
		else remove_inter_edges(cc, 1);
	}

	return NULL;
}

bool Graph::find(int value, const vector<int> &values) {
	int low = 0, high = (int)values.size()-1;

	while(low < high) {
		int mid = (low+high)/2;
		if(values[mid] < value) low = mid+1;
		else high = mid;
	}
	return values[low] == value;
}

void Graph::output_k_edge_connected_subgraphs() {
	ostringstream os;
	//os<<dir<<"/decomposition_"<<K<<".txt";
	os<<dir<<"/my.txt";

	FILE *fout = open_file(os.str().c_str(), "w");

	int cnt = 0, max_size = 0;

	memset(computed, 0, sizeof(int)*n);
	for(int i = 0;i < n;i ++) if(!computed[i]) {
		int q_c = 1;
		q[0] = i;
		computed[i] = 1;

		for(int j = 0;j < q_c;j ++) {
			int s = q[j];
			for(Edge *e = nodes[s].first;e != NULL&&!e->deleted;e = e->next) if(!computed[e->node_id]) {
				computed[e->node_id] = 1;
				q[q_c] = e->node_id;
				++ q_c;
			}
		}

		if(q_c == 1) continue;

		sort(q, q+q_c);
		fprintf(fout, "%d", q[0]);
		for(int j = 1;j < q_c;j ++) fprintf(fout, ", %d", q[j]);
		fprintf(fout, "\n");

		++ cnt;
		if(q_c > max_size) max_size = q_c;
	}

	//printf("%d %d\n", cnt, max_size);

	fclose(fout);
}

void Graph::add_edge(Node &node, Edge *edge) {
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

void Graph::delete_edge(Node &node, Edge *edge) {
	if(edge->pre == NULL) {
		node.first = edge->next;
		if(edge->next != NULL) edge->next->pre = NULL;
	}
	else {
		if(edge == node.last) node.last = edge->pre;

		Edge *tmp = edge->pre;
		tmp->next = edge->next;

		if(edge->next != NULL) edge->next->pre = tmp;
	}
}

void Graph::delete_edge_to_last(Node &node, Edge *edge) {
	edge->deleted = 1;
	if(node.first == node.last) {

#ifdef _DEBUG_
		if(node.first != edge) printf("WA in delete_edge_to_last!\n");
#endif

		return ;
	}

	if(edge->pre == NULL) {
		node.first = edge->next;
		if(edge->next != NULL) edge->next->pre = NULL;
	}
	else {
		if(edge == node.last) node.last = edge->pre;

		Edge *tmp = edge->pre;
		tmp->next = edge->next;

		if(edge->next != NULL) edge->next->pre = tmp;
	}

	edge->next = node.last->next;
	if(edge->next != NULL) edge->next->pre = edge;
	node.last->next = edge;
	edge->pre = node.last;
}

int Graph::construct_pgraph(int s, int *height) {
	int pedge_c = 0;
	int q_c = 1;

	computed[s] = 1;
	q[0] = s;

	for(int i = 0;i < q_c;i ++) {
		s = q[i];
		++ height[q[i]];

		pnodes[s].head = pnodes[s].tail = &elements[s];
		elements[s].next = NULL;

		for(Edge *edge = nodes[s].first;edge != NULL&&!edge->deleted;edge = edge->next) {
			if(!computed[edge->node_id]) {
				computed[edge->node_id] = 1;
				q[q_c] = edge->node_id;
				++ q_c;
			}

			if(edge->node_id > s) {
				int a = s, b = edge->node_id;

				pedges[pedge_c].node_id = b;
				pedges[pedge_c].sc = 1;
				pedges[pedge_c].duplicate = &pedges[pedge_c+1];
				add_edge(pnodes[a], &pedges[pedge_c]);
				++ pedge_c;

				pedges[pedge_c].node_id = a;
				pedges[pedge_c].sc = 1;
				pedges[pedge_c].duplicate = &pedges[pedge_c-1];
				add_edge(pnodes[b], &pedges[pedge_c]);
				++ pedge_c;
			}
		}
	}

	for(int i = 0;i < q_c;i ++) computed[q[i]] = 0;

	return q_c;
}

void Graph::kcore_optimization(int q_c, int assign_sc) {
	for(int i = 0;i < q_c;i ++) {
		int s = q[i];
		computed[s] = 1;

		for(Edge *edge = nodes[s].first;edge != NULL&&!edge->deleted;edge = edge->next) {
			int t = edge->node_id;

			delete_edge_to_last(nodes[t], edge->duplicate);
			-- degrees[t];

			if(degrees[t] == K-1) {
				q[q_c] = t;
				++ q_c;
			}

			edge->deleted = 1;

			if(assign_sc) edge->sc = edge->duplicate->sc = K-1;
		}

		nodes[s].last = nodes[s].first;
	}
}

int Graph::max_flow(int s, int t, Node *nn, int res, int &q_c) {
	if(inL == NULL) {
		inL = new char[n];
		memset(inL, 0, sizeof(char)*n);
	}

	Edge **pre = new Edge*[n];

	vector<Edge *> flow;

	while(K == -1||res < K) {
		q_c = 1;

		q[0] = s;
		pre[s] = NULL;
		inL[s] = 1;

		int find = 0;

		pre[t] = NULL;

		for(int i = 0;i < q_c&&!find;i ++) {
			int u = q[i];
			for(Edge *e = nn[u].first;e != NULL;e = e->next) if(e->sc > 0&&!inL[e->node_id]) {
				pre[e->node_id] = e;
				inL[e->node_id] = 1;
				q[q_c] = e->node_id;
				++ q_c;

				if(e->node_id == t) {
					find = 1;
					break;
				}
			}
		}

		for(int i = 0;i < q_c;i ++) inL[q[i]] = 0;

		if(!find) break;

		++ res;

		for(Edge *e = pre[t];e != NULL;) {
			flow.push_back(e);

			-- e->sc;
			++ e->duplicate->sc;

			e = pre[e->duplicate->node_id];
		}
	}

	for(int j = 0;j < (int)flow.size();j ++) flow[j]->sc = flow[j]->duplicate->sc = 1;

	delete[] pre;

	return res;
}

void Graph::all_SC_naive() {
	//printf("Compute all SCs using naive approach!\n");
#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	for(int i = 1;i <= n;i ++) {
		if(i > 1) if(!kSC(i)) break;

		//if(i > 1) output_k_edge_connected_subgraphs();

		for(int j = 0;j < n;j ++) {
			for(Edge* e = nodes[j].first;e != NULL;e = e->next) {
				if(!e->deleted) e->sc = i;
				else e->deleted = 0;
				nodes[j].last = e;
			}
		}
	}

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("all_SC_naive ltime: %lld\n", mtime);

#else
        end = clock();

        printf("all_SC_naive time: %d\n", end-start);
#endif

	//printf("Here\n");

	//string output_name = string(dir) + "/all-SC.bin";
	//FILE *fout = open_file(output_name.c_str(), "wb");

	//output_all_sc(fout);

	//fclose(fout);
}

void Graph::all_SC_BU() {
	//printf("Compute all SCs using bottom-up approach!\n");

#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	int *id1 = new int[n], *id2 = new int[n];
	int n_id1, n_id2;

	for(int i = 0;i < n;i ++) id1[i] = i;
	n_id1 = n;

	for(int i = 2;i <= n;i ++) {
		//printf("%d\n", i);
		if(!kSC_BU(i, id1, n_id1, id2, n_id2)) {
			printf("Max K: %d\n", i);
			break;
		}

		//output_k_edge_connected_subgraphs();

		n_id1 = n_id2;
		int *tmp = id1; id1 = id2; id2 = tmp;
	}

	delete[] id1;
	delete[] id2;

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("all_SC_BU ltime: %lld\n", mtime);

#else
        end = clock();

        printf("all_SC_naive time: %d\n", end-start);
#endif

	//printf("Here\n");

	//string output_name = string(dir) + "/all-SC-BU.bin";
	//FILE *fout = open_file(output_name.c_str(), "wb");

	//output_all_sc(fout);

	//fclose(fout);
}

void Graph::output_all_sc(FILE *fout) {
	int *buf = new int[3*m];
	vector<pair<int,pair<int,int> > > vp;
	vp.reserve(m);

	FILE *ff = open_file((dir+string("/cg.txt")).c_str(), "w");

	fprintf(ff, "%d %d\n", n, m);

	fwrite(&n, sizeof(int), 1, fout);
	fwrite(&m, sizeof(int), 1, fout);

	int edge_c = 0;

	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) if(e->node_id > i) {
		vp.push_back(make_pair(e->sc, make_pair(i,e->node_id)));
	}

	sort(vp.begin(), vp.end());

	for(int i = (int)vp.size()-1;i >= 0;i --) {
		buf[edge_c] = vp[i].second.first; buf[edge_c+1] = vp[i].second.second; buf[edge_c+2] = vp[i].first;
		edge_c += 3;

		fprintf(ff, "%d %d %d\n", vp[i].second.first, vp[i].second.second, vp[i].first);
	}

	fclose(ff);

	if(edge_c/3 != m) printf("WA edge count in output_all_sc!\n");

	fwrite(buf, sizeof(int), edge_c, fout);

	/*for(int i = 0;i < n;i ++) {
		int cnt = 2;
		vector<pair<int, int> > vp;
		for(Edge *e = nodes[i].first;e != NULL;e = e->next) if(e->node_id > i) {
			vp.push_back(make_pair(e->node_id, e->sc));
			//buf[cnt] = e->node_id;
			//buf[cnt+1] = e->sc;
			//cnt += 2;
		}
		sort(vp.begin(), vp.end());
		for(int j = 0;j < (int)vp.size();j ++) {
			buf[cnt] = vp[j].first;
			buf[cnt+1] = vp[j].second;
			cnt += 2;
		}

		buf[0] = i; buf[1] = cnt/2-1;
		//fwrite(buf, sizeof(int), cnt, fout);

		for(int j = 0;j < (int)vp.size();j ++) fprintf(fout, "%d %d %d\n", i, vp[j].first, vp[j].second);
	}*/

	delete[] buf;
}

int Graph::find_root(int x, int *parent) {
	int root = x;
	while(parent[root] != root) root = parent[root];

	while(parent[x] != root) {
		int tmp = parent[x];
		parent[x] = root;
		x = tmp;
	}

	return root;
}

void Graph::max_spanning_tree() {
#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	for(int i = 0;i < n;i ++) pnodes[i].first = pnodes[i].last = NULL;
	int edge_c = 0;

	int max_sc = 0;

	for(int i = 0;i < n;i ++) for(Edge *e = nodes[i].first;e != NULL;e = e->next) {
		if(e->node_id > i) {
			if(e->sc > max_sc) max_sc = e->sc;

			pedges[edge_c].node_id = i;
			pedges[edge_c].sc = e->node_id;
			pedges[edge_c].next = NULL;

			if(pnodes[e->sc].first == NULL) pnodes[e->sc].first = pnodes[e->sc].last = &pedges[edge_c];
			else {
				pnodes[e->sc].last->next = &pedges[edge_c];
				pnodes[e->sc].last = &pedges[edge_c];
			}

			++ edge_c;
		}
	}

	int *parent = new int[n];
	int *rank = new int[n];

	vector<pair<pair<int,int>, int> > vp;
	vp.reserve(n);

	for(int i = 0;i < n;i ++) {
		parent[i] = i;
		rank[i] = 0;
	}

	for(int i = max_sc;i > 0;i --) {
		for(Edge *e = pnodes[i].first;e != NULL;e = e->next) {
			int pa = find_root(e->node_id, parent);
			int pb = find_root(e->sc, parent);

			if(pa == pb) continue;

			vp.push_back(make_pair(make_pair(e->node_id, e->sc), i));

			if(rank[pa] < rank[pb]) parent[pa] = pb;
			else if(rank[pa] > rank[pb]) parent[pb] = pa;
			else {
				parent[pa] = pb;
				++ rank[pb];
			}
		}
	}

	for(int i = 0;i < n;i ++) pnodes[i].first = pnodes[i].last = NULL;

	edge_c = 0;
	for(int i = 0;i < (int)vp.size();i ++) {
		int a = vp[i].first.first, b = vp[i].first.second;

		pedges[edge_c].node_id = b;
		pedges[edge_c].next = pnodes[a].first;
		pnodes[a].first = &pedges[edge_c];
		++ edge_c;

		pedges[edge_c].node_id = a;
		pedges[edge_c].next = pnodes[b].first;
		pnodes[b].first = &pedges[edge_c];
		++ edge_c;
	}

	int c_id = 0;
	int *c_ids = new int[n];
	int *levels = new int[n];

	if(computed == NULL) computed = new int[n];

	memset(computed, 0, sizeof(int)*n);

	//printf("27:");
	//for(Edge *e = pnodes[27].first;e != NULL;e = e->next) printf(" %d", e->node_id);
	//printf("\n");

	for(int i = 0;i < n;i ++) {
		if(computed[i]) continue;

		int q_c = 1;
		q[0] = i;
		levels[i] = 0;
		c_ids[i] = c_id;

		computed[i] = 1;

		for(int j = 0;j < q_c;j ++) {
			for(Edge *e = pnodes[q[j]].first;e != NULL;e = e->next) if(!computed[e->node_id]) {
				//if(q[j] == 27||e->node_id == 27) printf("Edge %d %d %d\n", q[j], e->node_id, levels[q[j]]);
				computed[e->node_id] = 1;
				q[q_c ++] = e->node_id;
				levels[e->node_id] = levels[q[j]]+1;
				c_ids[e->node_id] = c_id;
			}
		}

		++ c_id;
	}

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("mSPT ltime: %lld\n", mtime);

#else
        end = clock();

        printf("mSPT time: %d\n", end-start);
#endif

	string output_name = string(dir) + "/mSPT.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	fprintf(fout, "%d %d\n", n, (int) vp.size());
	//if(vp.size() != n-1) printf("WA in tree size!\n");

	for(int i = 0;i < n;i ++) fprintf(fout, "%d %d\n", c_ids[i], levels[i]);

	for(int i = 0;i < (int)vp.size();i ++) fprintf(fout, "%d %d %d\n", vp[i].first.first, vp[i].first.second, vp[i].second);

	fclose(fout);

	delete[] c_ids;
	delete[] levels;
	delete[] parent;
	delete[] rank;
}

void Graph::optimization_tree(int output) {
	string input_name = string(dir) + "/mSPT.txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	fscanf(fin, "%d%d", &n, &m);

	int *cid = new int[n];
	for(int i = 0;i < n;i ++) {
		int a, b;
		fscanf(fin, "%d%d", &a, &b);
		cid[i] = a;
	}

	int *a = new int[m];
	int *b = new int[m];
	int *c = new int[m];
	for(int i = 0;i < m;i ++) fscanf(fin, "%d%d%d", a+i, b+i, c+i);

	fclose(fin);

#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	int *weight = new int[n];
	vector<pair<int,int> > vp;
	vp.reserve(2*n);

	int *parent = new int[2*n];
	for(int i = 0;i < 2*n;i ++) parent[i] = i;

	if(degrees == NULL) degrees = new int[n];

	for(int i = 0;i < n;i ++) degrees[i] = 0;

	int *i_parent = new int[n];
	for(int i = 0;i < n;i ++) i_parent[i] = -1;

	if(pnodes == NULL) pnodes = new Node[n];
	if(pedges == NULL) pedges = new Edge[2*n];

	for(int i = 0;i < m;i ++) pnodes[i].first = pnodes[i].last = NULL;
	int edge_c = 0;

	for(int i = 0;i < m;i ++) {
		weight[i] = c[i];

		int pa = find_root(a[i], parent);
		int pb = find_root(b[i], parent);

		parent[n+i] = n+i;
		parent[pa] = n+i;
		parent[pb] = n+i;
		vp.push_back(make_pair(pa, n+i));
		vp.push_back(make_pair(pb, n+i));

		if(pa >= n) {
			++ degrees[pa-n];

			pedges[edge_c].node_id = i;
			pedges[edge_c].next = pnodes[pa-n].first;
			pnodes[pa-n].first = &pedges[edge_c];
			++ edge_c;

			pedges[edge_c].node_id = pa-n;
			pedges[edge_c].next = pnodes[i].first;
			pnodes[i].first = &pedges[edge_c];
			++ edge_c;
		}
		else i_parent[pa] = i;
		if(pb >= n) {
			++ degrees[pb-n];

			pedges[edge_c].node_id = i;
			pedges[edge_c].next = pnodes[pb-n].first;
			pnodes[pb-n].first = &pedges[edge_c];
			++ edge_c;

			pedges[edge_c].node_id = pb-n;
			pedges[edge_c].next = pnodes[i].first;
			pnodes[i].first = &pedges[edge_c];
			++ edge_c;
		}
		else i_parent[pb] = i;
	}

	delete[] a;
	delete[] b;
	delete[] c;

	int *levels = new int[2*m];
	int *weights = new int[2*m];
	int *id_pos = new int[m];

	for(int i = 0;i < m;i ++) id_pos[i] = -1;

	int n_levels = 0;

	for(int i = 0;i < m;i ++) if(degrees[i] == 0) {
		id_pos[i] = n_levels;
		weights[n_levels] = weight[i];
		levels[n_levels ++] = 0;

		DFS(-1, i, 0, pnodes, pedges, weight, n_levels, levels, id_pos, weights);
	}

	string output_name = string(dir) + "/mSPT-Opt.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	string output_bin = string(dir) + "/mSPT-Opt.bin";
	FILE *fout_bin = open_file(output_bin.c_str(), "wb");

	if(output) {
		fprintf(fout, "%d", n);
		for(int i = 0;i < n;i ++) fprintf(fout, " %d %d", cid[i], i_parent[i]);
		fprintf(fout, "\n");

		fprintf(fout, "%d", m);
		for(int i = 0;i < m;i ++) fprintf(fout, " %d", id_pos[i]);
		fprintf(fout, "\n");

		fwrite(&n, sizeof(int), 1, fout_bin);
		for(int i = 0;i < n;i ++) {
			fwrite(cid+i, sizeof(int), 1, fout_bin);
			fwrite(i_parent+i, sizeof(int), 1, fout_bin);
		}
		fwrite(&m, sizeof(int), 1, fout_bin);
		fwrite(id_pos, sizeof(int), m, fout_bin);
	}

	delete[] cid;
	delete[] weight;
	delete[] id_pos;
	delete[] i_parent;

	construct_LCA_index(fout, fout_bin, n_levels, levels, weights, output);

	//fprintf(fout, "%d %d %d\n", n, m, (int)vp.size());
	//for(int i = 0;i < m;i ++) fprintf(fout, "%d\n", weight[n+i]);
	//for(int i = 0;i < (int)vp.size();i ++) fprintf(fout, "%d %d\n", vp[i].first, vp[i].second);

	fclose(fout_bin);
	fclose(fout);

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("mSPTO ltime, %lld\n", mtime);

#else
        end = clock();

        printf("mSPTO time, %d\n", end-start);
#endif

	delete[] weights;
	delete[] levels;
}

void Graph::construct_LCA_index(FILE *fout, FILE *fout_bin, int n_levels, int *levels, int *weights, int output) {
	int logn = 1;
	while((1<<logn) < n_levels) ++ logn;
	logn /= 2;

	int newn = (n_levels+logn-1)/logn;
	int *min_levels = new int[newn];
	int *min_pos = new int[newn];

	for(int i = 0;i < newn;i ++) {
		min_levels[i] = n_levels*10;
		min_pos[i] = -1;
		for(int j = i*logn;j < (i+1)*logn&&j < n_levels;j ++) {
			if(levels[j] < min_levels[i]) {
				min_levels[i] = levels[j];
				min_pos[i] = j;
			}
		}
	}

	if(output) {
		fprintf(fout, "%d", n_levels);
		for(int i = 0;i < n_levels;i ++) fprintf(fout, " %d", weights[i]);
		fprintf(fout, "\n");

		fwrite(&n_levels, sizeof(int), 1, fout_bin);
		fwrite(weights, sizeof(int), n_levels, fout_bin);
	}

	int new_logn = 1;
	while((1<<new_logn) < newn) ++ new_logn;

	int **dp = new int*[new_logn+1];
	for(int i = 0;i <= new_logn;i ++) dp[i] = new int[newn];

	for(int i = 0;i < newn;i ++) dp[0][i] = min_pos[i];
	for(int i = 1;i <= new_logn;i ++) for(int j = 0;j < newn;j ++) {
		dp[i][j] = dp[i-1][j];
		if(j + (1<<(i-1)) < newn&&levels[dp[i-1][j + (1<<(i-1))]] < levels[dp[i][j]]) dp[i][j] = dp[i-1][j + (1<<(i-1))];
	}

	if(output) {
		fprintf(fout, "%d %d\n", new_logn+1, newn);
		for(int i = 0;i <= new_logn;i ++) {
			for(int j = 0;j < newn;j ++) fprintf(fout, "%d ", dp[i][j]);
			fprintf(fout, "\n");
		}

		int tmp = new_logn+1;
		fwrite(&tmp, sizeof(int), 1, fout_bin);
		fwrite(&newn, sizeof(int), 1, fout_bin);
		for(int i = 0;i <= new_logn;i ++) fwrite(dp[i], sizeof(int), newn, fout_bin);

		printf("LCA size: %d\n", (new_logn+1)*newn + (1<<logn)*(logn*logn));
	}

	int *hash = new int[(1<<logn)];
	for(int i = 0;i < (1<<logn);i ++) hash[i] = -1;

	int **table = new int*[(1<<logn)];
	for(int i = 0;i < (1<<logn);i ++) table[i] = new int[logn*logn];

	int n_table = 0;
	int *id_pos = new int[newn];

	//if(hash[3] != -1) printf("WAAA!\n");

	for(int i = 0;i < newn;i ++) {
		int bitset = 0;
		for(int j = i*logn+1;j < (i+1)*logn;j ++) {
			//bitset = (bitset<<1);
			if(j >= n_levels||levels[j] >= levels[j-1]) bitset = (bitset | (1<<(j-i*logn-1)));
		}

		if(hash[bitset] != -1) {
			id_pos[i] = hash[bitset];

			/*
			int *val = table[id_pos[i]];
			for(int j = i*logn;j < (i+1)*logn;j ++) for(int k = j;k < (i+1)*logn&&k < n_levels;k ++) {
				for(int ii = j;ii <= k;ii ++) if(levels[ii] < levels[val[(j-i*logn)*logn + k-i*logn]+i*logn]) {
					for(int jj = 0;jj < logn-1;jj ++) {
						if(bitset&(1<<jj)) printf("1 ");
						else printf("-1 ");
					}
					printf("\n");

					for(int jj = i*logn;jj < (i+1)*logn;jj ++) printf("%d ", levels[jj]);
					printf("\n");
					for(int jj = 0;jj < logn;jj ++) for(int kk = jj;kk < logn;kk ++) printf("[%d %d: %d], ", jj, kk, val[jj*logn+kk]);
					printf("\n");

					printf("WA1 %d!\n", bitset);

					return ;
				}
			}
			*/

			continue;
		}

		hash[bitset] = n_table;
		id_pos[i] = n_table;
		int *val = table[n_table];
		++ n_table;

		for(int j = i*logn;j < (i+1)*logn&&j < n_levels;j ++) {
			val[(j - i*logn)*logn + j - i*logn] = j - i*logn;
			for(int k = j+1;k < (i+1)*logn&&k < n_levels;k ++) {
				val[(j-i*logn)*logn + k-i*logn] = val[(j-i*logn)*logn + k-1-i*logn];
				if(levels[k] < levels[val[(j-i*logn)*logn + k-i*logn] + i*logn]) val[(j-i*logn)*logn + k-i*logn] = k - i*logn;
			}
		}

		/*
		if(bitset == 3) {
			int *val = table[id_pos[i]];
			{
					for(int jj = 0;jj < logn-1;jj ++) {
						if(bitset&(1<<jj)) printf("1 ");
						else printf("-1 ");
					}
					printf("\n");

					for(int jj = i*logn;jj < (i+1)*logn;jj ++) printf("%d ", levels[jj]);
					printf("\n");
					for(int jj = 0;jj < logn;jj ++) for(int kk = jj;kk < logn;kk ++) printf("[%d %d: %d], ", jj, kk, val[jj*logn+kk]);
					printf("\n");

					printf("bitset = %d!\n", bitset);
			}
		}
		*/

		//val = table[id_pos[i]];
		/*
		for(int j = i*logn;j < (i+1)*logn;j ++) for(int k = j;k < (i+1)*logn&&k < n_levels;k ++) {
			for(int ii = j;ii <= k;ii ++) if(levels[ii] < levels[val[(j-i*logn)*logn + k-i*logn]+i*logn]) {
				printf("WA2!\n");
			}
		}
		*/
	}

	if(output) {
		for(int i = 0;i < newn;i ++) fprintf(fout, "%d ", id_pos[i]);
		fprintf(fout, "\n");

		fprintf(fout, "%d %d\n", n_table, logn);
		for(int i = 0;i < n_table;i ++) {
			int *val = table[i];
			for(int j = 0;j < logn*logn;j ++) fprintf(fout, "%d ", val[j]);
			fprintf(fout, "\n");
		}

		fwrite(id_pos, sizeof(int), newn, fout_bin);
		fwrite(&n_table, sizeof(int), 1, fout_bin);
		fwrite(&logn, sizeof(int), 1, fout_bin);
		for(int i = 0;i < n_table;i ++) fwrite(table[i], sizeof(int), logn*logn, fout_bin);
	}

	delete[] id_pos;

	for(int i = 0;i < (1<<logn);i ++) {
		delete[] table[i];
		table[i] = NULL;
	}
	delete[] table;

	for(int i = 0;i <= new_logn;i ++) {
		delete[] dp[i];
		dp[i] = NULL;
	}
	delete[] dp;

	delete[] hash;

	delete[] min_levels;
	delete[] min_pos;
}

void Graph::DFS(int pid, int id, int lid, const Node *pnodes, const Edge *pedges, int *weight, int &n_levels, int *levels, int *id_pos, int *weights) {
	Edge **s_e = new Edge*[n];
	int *pids = new int[n];
	int *ids = new int[n];
	int *lids = new int[n];

	s_e[0] = pnodes[id].first;
	pids[0] = pid;
	ids[0] = id;
	lids[0] = lid;

	int s_n = 0;

	while(s_n >= 0) {
		if(s_e[s_n] != NULL) {
			if(s_e[s_n]->node_id == pids[s_n]) {
				s_e[s_n] = s_e[s_n]->next;
				continue;
			}

			id_pos[s_e[s_n]->node_id] = n_levels;
			weights[n_levels] = weight[s_e[s_n]->node_id];
			levels[n_levels ++] = lids[s_n]+1;

			++ s_n;
			s_e[s_n] = pnodes[s_e[s_n-1]->node_id].first;
			pids[s_n] = ids[s_n-1];
			ids[s_n] = s_e[s_n-1]->node_id;
			lids[s_n] = lids[s_n-1]+1;
		}
		else {
			-- s_n;
			if(s_n >= 0) {
				weights[n_levels] = weight[ids[s_n]];
				levels[n_levels ++] = lids[s_n];

				s_e[s_n] = s_e[s_n]->next;
			}
		}
	}

	delete[] s_e;
	delete[] pids;
	delete[] ids;
	delete[] lids;

	/*
	for(Edge *e = pnodes[id].first;e != NULL;e = e->next) {
		if(e->node_id == pid) continue;

		id_pos[e->node_id] = n_levels;
		weights[n_levels] = weight[e->node_id];
		levels[n_levels ++] = lid+1;

		if(weight[e->node_id] < weight[id]) printf("WA in DFS\n");

		DFS(id, e->node_id, lid+1, pnodes, pedges, weight, n_levels, levels, id_pos, weights);

		weights[n_levels] = weight[id];
		levels[n_levels ++] = lid;
	}
	*/
}

void Graph::naive_P2P(const char *query_name) {
	string input_name = dir + "/" + string(query_name) + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	int t;
	fscanf(fin, "%d", &t);

	int *a = new int[t];
	int *b = new int[t];
	int *c = new int[t];

	for(int i = 0;i < t;i ++) fscanf(fin, "%d%d", a+i, b+i);

	fclose(fin);

#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	for(int i = 0;i < t;i ++) {

		for(int k = 1;;k ++) {
			for(int j = 0;j < n;j ++) for(Edge *e = nodes[j].first;e != NULL;e = e->next) {
				e->deleted = false;
				nodes[j].last = e;
			}
			// printf("computing %d %d %d\n", a[i], b[i], k);
			if(!kSC_P2P(k, a[i], b[i])) {
				c[i] = k-1;
				break;
			}
		}

		//printf("%d %d %d: %d\n", i, a[i], b[i], c[i]);
	}

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("naive-P2P ltime, %lld\n", mtime);

#else
        end = clock();

        printf(" time: %d\n", end-start);
#endif

	string output_name = dir + "/" + string(query_name) + "_P2P_naive.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	for(int i = 0;i < t;i ++) fprintf(fout, "%d %d %d\n", a[i], b[i], c[i]);

	fclose(fout);

	delete[] a;
	delete[] b;
	delete[] c;
}

void Graph::read_mSPT() {
	string input_name = dir + "/mSPT.txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	fscanf(fin, "%d%d", &n, &m);

	if(cids == NULL) cids = new int[n];
	if(levels == NULL) levels = new int[n];
	//printf("Hre\n");

	for(int i = 0;i < n;i ++) fscanf(fin, "%d%d", cids+i, levels+i);

	if(pnodes == NULL) pnodes = new Node[n];
	if(pedges == NULL) pedges = new Edge[2*m];

	int edge_c = 0;
	for(int i = 0;i < n;i ++) pnodes[i].first = pnodes[i].last = NULL;

	if(parent == NULL) parent = new int[n];
	if(weights == NULL) weights = new int[n];

	for(int i = 0;i < n;i ++) parent[i] = -1;

	for(int i = 0;i < m;i ++) {
		int a, b, c;
		fscanf(fin, "%d%d%d", &a, &b, &c);

		if(levels[a] != levels[b]+1&&levels[b] != levels[a]+1) printf("WA in read_mSPT\n");

		if(levels[a] < levels[b]) {
			parent[b] = a;
			weights[b] = c;
		}
		else {
			parent[a] = b;
			weights[a] = c;
		}

		pedges[edge_c].node_id = b;
		pedges[edge_c].sc = c;
		pedges[edge_c].next = NULL;
		if(pnodes[a].last == NULL) pnodes[a].first = pnodes[a].last = &pedges[edge_c];
		else {
			pnodes[a].last->next = &pedges[edge_c];
			pnodes[a].last = &pedges[edge_c];
		}
		++ edge_c;

		pedges[edge_c].node_id = a;
		pedges[edge_c].sc = c;
		pedges[edge_c].next = NULL;
		if(pnodes[b].last == NULL) pnodes[b].first = pnodes[b].last = &pedges[edge_c];
		else {
			pnodes[b].last->next = &pedges[edge_c];
			pnodes[b].last = &pedges[edge_c];
		}
		++ edge_c;
	}

	for(int i = 0;i < n;i ++) if((levels[i] != 0&&parent[i] == -1)||(levels[i] == 0&&parent[i] != -1)) {
		printf("WA1 in read_mSPT!\n");
	}

	for(int i = 0;i < n;i ++) for(Edge *e = pnodes[i].first;e != NULL;e = e->next) {
		if(e->next != NULL&&e->sc < e->next->sc) printf("WA2 in read_mSPT!\n");
	}

	fclose(fin);
}

void Graph::mSPT_P2P(const char *query_name) {
	string input_name = dir + "/" + string(query_name) + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	int t;
	fscanf(fin, "%d", &t);

	int *a = new int[t];
	int *b = new int[t];
	int *c = new int[t];

	for(int i = 0;i < t;i ++) fscanf(fin, "%d%d", a+i, b+i);

	fclose(fin);

#ifdef _LINUX_
	struct timeval start, end;
	gettimeofday(&start, NULL);
#else
	int start, end;
	start = clock();
#endif

	int cnt;

	for(int i = 0;i < t;i ++) {
		if(cids[a[i]] != cids[b[i]]) {
			c[i] = 0;
			continue;
		}

		c[i] = n*2;

		cnt = 0;

		int ta = a[i], tb = b[i];
		while(levels[ta] > levels[tb]) {
			if(weights[ta] < c[i]) c[i] = weights[ta];
			++ cnt;
			ta = parent[ta];
		}
		while(levels[tb] > levels[ta]) {
			if(weights[tb] < c[i]) c[i] = weights[tb];
			++ cnt;
			tb = parent[tb];
		}

		while(ta != tb) {
			if(parent[ta] == -1||parent[tb] == -1) {
				printf("WA1: %d %d, %d %d, %d %d\n", ta, parent[ta], tb, parent[tb], levels[ta], levels[tb]);
				break;
			}
			if(levels[ta] != levels[tb]) printf("WA2\n");
			if(weights[ta] < c[i]) c[i] = weights[ta];
			ta = parent[ta];

			if(weights[tb] < c[i]) c[i] = weights[tb];
			tb = parent[tb];

			cnt += 2;
		}
	}

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	printf("mSPT-P2P ltime, %lld\n", mtime);

#else
	end = clock();

	printf(" time: %d\n", end-start);
#endif

	//printf("%d\n", cnt);

	string output_name = dir + "/" + string(query_name) + "_P2P_mSPT.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	for(int i = 0;i < t;i ++) fprintf(fout, "%d %d %d\n", a[i], b[i], c[i]);

	fclose(fout);

	delete[] a;
	delete[] b;
	delete[] c;
}

void Graph::read_mSPTO() {
	string input_name = dir + "/mSPT-Opt.txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	fscanf(fin, "%d", &n);

	if(cids == NULL) cids = new int[n];
	if(pos_id == NULL) pos_id = new int[n];
	if(parent == NULL) parent = new int[n];

	for(int i = 0;i < n;i ++) fscanf(fin, "%d%d", cids+i, pos_id+i);

	fscanf(fin, "%d", &m);
	for(int i = 0;i < m;i ++) fscanf(fin, "%d", parent+i);

	for(int i = 0;i < n;i ++) pos_id[i] = parent[pos_id[i]];

	fscanf(fin, "%d", &weights_n);
	if(weights == NULL) weights = new int[weights_n];
	for(int i = 0;i < weights_n;i ++) fscanf(fin, "%d", &weights[i]);

	fscanf(fin, "%d%d", &logns, &ns);
	if(st == NULL) {
		st = new int*[logns];
		for(int i = 0;i < logns;i ++) st[i] = new int[ns];
	}

	for(int i = 0;i < logns;i ++) for(int j = 0;j < ns;j ++) {
		fscanf(fin, "%d", &st[i][j]);
		st[i][j] = weights[st[i][j]];
	}

	if(t_id == NULL) t_id = new int[ns];
	for(int i = 0;i < ns;i ++) fscanf(fin, "%d", &t_id[i]);

	fscanf(fin, "%d%d", &nt, &logn);
	if(table == NULL) {
		table = new int*[nt];
		for(int i = 0;i < nt;i ++) table[i] = new int[logn*logn];
	}
	for(int i = 0;i < nt;i ++) for(int j = 0;j < logn*logn;j ++) fscanf(fin, "%d", &table[i][j]);

	fclose(fin);

	if(log_table == NULL) log_table = new int[ns+1];

	log_table[0] = 0;
	for(int i = 0;i <= logns;i ++) for(int j = (1<<i);j < (1<<(i+1))&&j <= ns;j ++) log_table[j] = i;
}

int Graph::mSPTO_P2P(int a, int b) {
	int ti = pos_id[a], tj = pos_id[b];
	if(ti > tj) swap(ti, tj);

	int res = n*2;

	int li = ti/logn, lj = tj/logn, tmp;

	if(li == lj) {
		res = weights[table[t_id[li]][(ti%logn)*logn + tj%logn] + li*logn];
		return res;
	}

	if(lj > li+1) {
		int x = log_table[lj-li-1];
		int *val = st[x];
		if((tmp = val[li+1]) < res) res = tmp;
		if((tmp = val[lj-(1<<x)]) < res) res = tmp;
	}

	ti %= logn; tj %= logn;
	if((tmp = weights[table[t_id[li]][ti*logn + logn-1] + li*logn]) < res) res = tmp;
	if((tmp = weights[table[t_id[lj]][tj] + lj*logn]) < res) res = tmp;

	return res;
}


void Graph::smcc_naive(const char *file_name) {
	string input_name = dir + "/" + file_name + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	vector<vector<int> > query;

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

	int qq[40], q_n;

	for(int i = 0;i < t;i ++) {
		//printf("%d\n", i);
		vector<int> &tmp = query[i];
		q_n = tmp.size();
		for(int j = 0;j < q_n;j ++) qq[j] = tmp[j];

		char st[16];
		sprintf(st, "%d", i+1);
		output_file = string(dir) + "/output_ecc_" + string(st) + ".txt";
		smcc_query_kSC(q_n, qq, 0);
	}

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	printf("smcc_naive ltime: %lld\n", mtime);

#else
	end = clock();

	printf("smcc_naive time: %d\n", end-start);
#endif
}

void Graph::sc(const char *file_name, int method) {
	string input_name = dir + "/" + file_name + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	vector<vector<int> > query;

	if(computed == NULL) computed = new int[n];
	if(q == NULL) q = new int[n];
	for(int i = 0;i < n;i ++) computed[i] = 0;

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

	int qq[40], q_n;

	for(int i = 0;i < t;i ++) {
		//printf("%d\n", i);
		vector<int> &tmp = query[i];
		q_n = tmp.size();
		for(int j = 0;j < q_n;j ++) qq[j] = tmp[j];

		if(method == 0) smcc_query_kSC(q_n, qq, 0);
		else if(method == 1) sc_query_mSPT(q_n, qq);
		else if(method == 2) sc_query_mSPTO(q_n, qq);
		else printf("WA in sc\n");
	}

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	printf("smcc_%d ltime: %lld\n", method, mtime);

#else
	end = clock();

	printf("smcc_%d time: %d\n", method, end-start);
#endif
}

void Graph::sc_query_mSPT(int q_n, int *qq) {
	int q_c = 1;
	q[0] = qq[0];
	computed[qq[0]] = 1;

	int ch = qq[0], res = 1000000;

	for(int i = 1;i < q_n;i ++) {
		int ta = ch, tb = qq[i];
		//printf("%d\n", i);

		//if(levels == NULL) printf("WWW\n");

		while(levels[ta] > levels[tb]) {
			if(weights[ta] < res) res = weights[ta];
			//printf("%d %d\n", levels[ta], levels[parent[ta]]);
			ta = parent[ta];
			computed[ta] = 1;
			q[q_c ++] = ta;
		}
		if(computed[tb]) continue;
		else {
			computed[tb] = 1;
			q[q_c ++] = tb;
		}

		int find = 0;

		while(levels[tb] > levels[ta]) {
			if(weights[tb] < res) res = weights[tb];
			tb = parent[tb];
			if(computed[tb]) {
				find = 1;
				break;
			}
			else {
				computed[tb] = 1;
				q[q_c ++] = tb;
			}
		}

		if(find) continue;

		//if(levels[ta] != levels[tb]) printf("WA %d %d\n", levels[ta], levels[tb]);

		while(ta != tb) {
			if(weights[ta] < res) res = weights[ta];
			ta = parent[ta];
			computed[ta] = 1;
			q[q_c ++] = ta;

			if(weights[tb] < res) res = weights[tb];
			tb = parent[tb];
			computed[tb] = 1;
			q[q_c ++] = tb;

			//if(q_c >= n) printf("WA\n");
		}

		ch = ta;
	}

	for(int i = 0;i < q_c;i ++) computed[q[i]] = 0;
}

void Graph::sc_query_mSPTO(int q_n, int *qq) {
	int res = 1000000;
	for(int i = 1;i < q_n;i ++) {
		int tmp = mSPTO_P2P(qq[0], qq[i]);
		if(tmp < res) res = tmp;
	}
}

void Graph::smccl_naive(const char *file_name) {
	string input_name = dir + "/" + file_name + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	vector<vector<int> > query;
	vector<int> L;

	int t;
	fscanf(fin, "%d", &t);
	for(int i = 0;i < t;i ++) {
		int d, tt;
		fscanf(fin, "%d%d", &d, &tt);
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

	int qq[40], q_n;

	for(int i = 0;i < t;i ++) {
		vector<int> &tmp = query[i];
		q_n = tmp.size();
		for(int j = 0;j < q_n;j ++) qq[j] = tmp[j];

		smcc_query_kSC(q_n, qq, L[i]);
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


void Graph::smcc_query_kSC(int q_n, int *qq, int L) {
	if(computed == NULL) computed = new int[n];
	if(height == NULL) height = new int[n];
	if(degrees == NULL) degrees = new int[n];

	printf( "computing smcc...\n" );
	int *result = new int[n];
	memset( result, 0, sizeof(int) * n );

	for(int ii = 2;ii <= n;ii ++) {
		K = ii;
		//printf( "[%d]", ii );
		for(int i = 0;i < n;i ++) computed[i] = height[i] = 0;

		int q_c = 0;
		for(int i = 0;i < n;i ++) {
			int cnt = 0;
			for(Edge *edge = nodes[i].first;edge != NULL;edge = edge->next) {
				edge->deleted = 0;
				nodes[i].last = edge;
				++ cnt;
			}

			if(cnt < K) {
				q[q_c] = i;
				++ q_c;
			}

			degrees[i] = cnt;
		}

		kcore_optimization(q_c, 1);

		int max_l = 0;
		Element *res = NULL;

		while(1) {
			construct_pgraph(qq[0], height);

			vector<Element *> cc;
			decomposition(qq[0], cc, max_l);

			if(cc.size() == 1) {
				res = cc[0];
				break;
			}
			else remove_inter_edges(cc, 1);
		}

		for(int i = 0;i < n;i ++) computed[i] = 0;
		int cnt = 0;
		for(Element *e = res;e != NULL;e = e->next) {
			computed[e->value] = 1;
			++ cnt;
		}
		int find = 1;
		for(int i = 0;i < q_n;i ++) if(!computed[qq[i]]) find = 0;

		printf( "K=%d,cnt=%d,qq[0]=%d,q_n=%d,find=%d\n", K, cnt, qq[0], q_n, find );

		if( find && cnt >= L && cnt > 1 )
			memcpy(result, computed, sizeof(int)*n);
		if(!find||cnt < L || cnt <= 1) {
			printf( "Result Found!\n" );
			FILE *fout = fopen(output_file.c_str(), "w");
			int cnt = 0;
			for( int i = 0; i < n; ++i ) if( result[i] ) ++cnt;
			fprintf( fout, "%d\n", cnt );
			for( int i = 0; i < n; ++i ) if( result[i] ) fprintf(fout, "%d ", i);
			fclose(fout);
			break;
		}
	}
	delete[] result;
}

void Graph::mSPT_tcse(const char *query_name, int output) {
	string input_name = dir + "/" + string(query_name) + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	int t;
	fscanf(fin, "%d", &t);

	int *a = new int[t];
	int *b = new int[t];

	for(int i = 0;i < t;i ++) fscanf(fin, "%d%d", a+i, b+i);

	fclose(fin);

	char *vis = new char[n];
	int *q = new int[n];
	memset(vis, 0, sizeof(char)*n);

#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	string output_name = dir + "/" + string(query_name) + "_tcse_mSPT.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	long long sum = 0;

	for(int i = 0;i < t;i ++) {
		q[0] = a[i];
		vis[a[i]] = 1;

		int q_c = 1, k = b[i];
		for(int j = 0;j < q_c;j ++) {
			for(Edge *e = pnodes[q[j]].first;e != NULL&&e->sc >= k;e = e->next) if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				q[q_c ++] = e->node_id;
			}
		}

		if(output) {
			vector<int> res(q, q+q_c);
			sort(res.begin(), res.end());
			for(int j = 0;j < (int)res.size();j ++) fprintf(fout, "%d ", res[j]);
			fprintf(fout, "\n");
		}

		sum += q_c;

		for(int j = 0;j < q_c;j ++) vis[q[j]] = 0;

		//printf("%d %d %d: %d\n", i, a[i], b[i], q_c);
	}

	printf("Average output size: %lld\n", sum/t);

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("mSPT-tcse ltime, %lld\n", mtime);

#else
        end = clock();

        printf(" time: %d\n", end-start);
#endif

	fclose(fout);

	delete[] a;
	delete[] b;

	delete[] vis;
	delete[] q;
}

void Graph::mSPT_mcse(const char *query_name, int output) {
	string input_name = dir + "/" + string(query_name) + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	int t;
	fscanf(fin, "%d", &t);

	int *a = new int[t];

	for(int i = 0;i < t;i ++) fscanf(fin, "%d", a+i);

	fclose(fin);

	char *vis = new char[n];
	int *q = new int[n];
	memset(vis, 0, sizeof(char)*n);

#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	string output_name = dir + "/" + string(query_name) + "_mcse_mSPT.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	long long sum = 0;

	for(int i = 0;i < t;i ++) {
		q[0] = a[i];
		vis[a[i]] = 1;

		int q_c = 1, k = pnodes[a[i]].first->sc;
		for(int j = 0;j < q_c;j ++) {
			for(Edge *e = pnodes[q[j]].first;e != NULL&&e->sc >= k;e = e->next) if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				q[q_c ++] = e->node_id;
			}
		}

		if(output) {
			vector<int> res(q, q+q_c);
			sort(res.begin(), res.end());
			for(int j = 0;j < (int)res.size();j ++) fprintf(fout, "%d ", res[j]);
			fprintf(fout, "\n");
		}

		sum += q_c;
		for(int j = 0;j < q_c;j ++) vis[q[j]] = 0;

		//printf("%d %d %d: %d\n", i, a[i], b[i], q_c);
	}

	printf("Average output size: %lld\n", sum/t);

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("mSPT-mcse ltime, %lld\n", mtime);

#else
        end = clock();

        printf(" time: %d\n", end-start);
#endif

	fclose(fout);

	delete[] a;

	delete[] vis;
	delete[] q;
}

void Graph::mSPT_scse(const char *query_name, int output) {
	string input_name = dir + "/" + string(query_name) + ".txt";
	FILE *fin = open_file(input_name.c_str(), "r");

	int t;
	fscanf(fin, "%d", &t);

	int *a = new int[t];
	int *b = new int[t];

	for(int i = 0;i < t;i ++) fscanf(fin, "%d%d", a+i, b+i);

	fclose(fin);

	char *vis = new char[n];
	int *q = new int[n];
	memset(vis, 0, sizeof(char)*n);

	int *head = new int[n];
	int *next = new int[2*n];
	Edge **t_edges = new Edge*[2*n];

	for(int i = 0;i < n;i ++) head[i] = -1;

#ifdef _LINUX_
        struct timeval start, end;
        gettimeofday(&start, NULL);
#else
        int start, end;
        start = clock();
#endif

	string output_name = dir + "/" + string(query_name) + "_scse_mSPT.txt";
	FILE *fout = open_file(output_name.c_str(), "w");

	long long sum = 0;

	for(int i = 0;i < t;i ++) {
		q[0] = a[i];
		vis[a[i]] = 1;
		int q_c = 1, k = 0;

		int edge_c = 0;

		t_edges[edge_c] = pnodes[a[i]].first;
		next[edge_c] = head[pnodes[a[i]].first->sc];
		head[pnodes[a[i]].first->sc] = edge_c;
		++ edge_c;

		int maxk = pnodes[a[i]].first->sc;

		while(maxk >= k) {
			Edge *e = t_edges[head[maxk]];
			head[maxk] = next[head[maxk]];

			if(!vis[e->node_id]) {
				vis[e->node_id] = 1;
				q[q_c ++] = e->node_id;

				//if(q_c == n) {
				//	int xx = 0;
				//	++ xx;
				//}

				if(q_c == b[i]) k = maxk;

				Edge *e1 = pnodes[e->node_id].first;
				if(e1 != NULL&&e1->sc >= k) {
					int tk = e1->sc;
					if(tk > maxk) tk = maxk;
					t_edges[edge_c] = e1;
					next[edge_c] = head[tk];
					head[tk] = edge_c;
					++ edge_c;
				}
			}

			e = e->next;
			if(e != NULL&&e->sc >= k) {
				int tk = e->sc;
				if(tk > maxk) tk = maxk;
				t_edges[edge_c] = e;
				next[edge_c] = head[tk];
				head[tk] = edge_c;
				++ edge_c;
			}

			while(maxk >= k&&head[maxk] == -1) -- maxk;
		}

		if(output) {
			vector<int> res(q, q+q_c);
			sort(res.begin(), res.end());
			for(int j = 0;j < (int)res.size();j ++) fprintf(fout, "%d ", res[j]);
			fprintf(fout, "\n");
		}

		sum += q_c;

		for(int j = 0;j < q_c;j ++) vis[q[j]] = 0;
		while(maxk >= 0) {
			head[maxk] = -1;
			-- maxk;
		}

		//printf("%d %d %d: %d\n", i, a[i], b[i], q_c);
	}

	printf("Average output size: %lld\n", sum/t);

#ifdef _LINUX_
        gettimeofday(&end, NULL);

        long long mtime, seconds, useconds;
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds*1000000 + useconds;

        printf("mSPT-scse ltime, %lld\n", mtime);

#else
        end = clock();

        printf(" time: %d\n", end-start);
#endif

	fclose(fout);

	delete[] a;
	delete[] b;

	delete[] vis;
	delete[] q;

	delete[] head;
	delete[] t_edges;
	delete[] next;
}
