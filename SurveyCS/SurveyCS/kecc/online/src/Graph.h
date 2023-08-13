#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "Utility.h"
#include "Heap.h"
#include "LinearHeap.h"

using namespace std;

struct Bin;

struct Element{
	Element *next;
	int value;
};

struct Edge {
	Edge *pre, *next;
	Edge *duplicate;
	Bin *nontree;
	int node_id, sc;
	char deleted;
};

struct Node {
	Element *head, *tail;
	Edge *first, *last;
};

struct Bin {
	Edge *edge;
	Bin *pre, *next;
};

class Graph {
private:
	string output_file;
	string dir;
	int K;
	int n, m;

	Element *elements; // buffer to allocate all elements
	Edge *edges; // buffer to allocate all edges
	Node *nodes;

	Node *pnodes; // nodes of super graph
	Edge *pedges; // edges of super graph

	char *inL; // used in kCD_opt(), kCCkCD(), max_flow()
	int *computed; // used in kCCkCD(), kCSkCD(), construct_pgraph()
	int *height; // record the maximum number of recursive calls in kSC

	int *q; // queque

	int *degrees; // used for k-core optimization

	Heap *heap;

	int *cids; // component id, used for disconnected graphs
	int *levels;

	int *parent;
	int *weights;

	int *pos_id;
	int weights_n;
	int **st;
	int ns, logns;
	int *t_id;
	int nt, logn;
	int **table;

	int *log_table;

public:
	Graph(const char *_dir) ;
	~Graph() ;

	void set_k(int _K) { K = _K; }

	void smcc_naive(const char *query_name) ;
	void sc(const char *query_name, int method) ; // method = 0 -- naive, 1 -- mSPT, 2 -- mSPT2
	void smccl_naive(const char *query_name) ;


	//void naive_tcse(const char *query_name, int output = 0) ;
	void mSPT_tcse(const char *query_name, int output = 0) ;

	void mSPT_mcse(const char *query_name, int output = 0) ;
	void mSPT_scse(const char *query_name, int output = 0) ;

	void naive_P2P(const char *query_name) ;
	void read_mSPT();
	void mSPT_P2P(const char *query_name) ;
	void read_mSPTO();
	void cal_mSPT_size() { printf("%s %d\n", dir.c_str(), (9*n+2)*4/1024); }

	void gen_p2p_query(const char *thresh) ;
	void gen_tcse_query(const char *thresh) ;
	void gen_mcse_query() ;
	void gen_scse_query();
	void count_cc() ;
	void extract_mcc() ;
	void min_max_conn() ;

	void check_connectivity(char *file_name) ;
	void read_graph(int binary);
	int kSC(int _K); //decomposition based paradigm for computing k-strong components (or k-edge-connected subgraphs)
					 //returns 1 iff there is at least one non-trivial k-strong component
	void output_k_edge_connected_subgraphs() ;
	void output_all_sc(FILE *fout) ;

	void all_SC_naive() ;
	void all_SC_BU() ;

	void max_spanning_tree() ;
	void optimization_tree(int output) ;

private:
	void smcc_query_kSC(int q_n, int *qq, int L) ;
	void sc_query_mSPT(int q_n, int *qq) ;
	void sc_query_mSPTO(int q_n, int *qq) ;
	int mSPTO_P2P(int a, int b) ;

	void construct_LCA_index(FILE *fout, FILE *fout_bin, int n_levels, int *levels, int *weights, int output) ;
	void DFS(int pid, int id, int lid, const Node *pnodes, const Edge *pedges, int *weight, int &n_levels, int *levels, int *id_pos, int *weights) ;
	int find_root(int x, int *parent) ;

	void remove_inter_edges(const vector<Element *> &cc, int assign_sc = 0) ;
	int kSC_BU(int _K, int *id1, int n_id1, int *id2, int &n_id2) ;
	int kSC_P2P(int _K, int u, int v) ;
	Element *kSC_tcse(int _K, int u) ;

	void decomposition(int s, vector<Element *> &cc, int &max_l) ;
	void merge(int s, int t, Heap *heap) ;
	void add_edge(Node &node, Edge *edge) ;
	void delete_edge(Node &node, Edge *edge) ;
	void delete_edge_to_last(Node &node, Edge *edge) ;
	int construct_pgraph(int s, int *height) ; //compute partition graph for the connected components containing node s, return the number of nodes
	void kcore_optimization(int q_c, int assign_sc = 0) ;

	int max_flow(int s, int t, Node *nn, int initial_value, int &q_c) ;
	bool find(int value, const vector<int> &values) ;
};

#endif
