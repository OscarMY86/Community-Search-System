#ifndef _CONN_GRAPH_H_
#define _CONN_GRAPH_H_

#include "Utility.h"
#include "Heap.h"
#include "LinearHeap.h"
#include "Graph.h"

using namespace std;

class ConnGraph {
private:
	string dir;
	int n, m;

	Element *elements; // buffer to allocate all elements
	Edge *edges; // edges for graph
	Node *nodes; // nodes for graph
	int edge_n; // for inserting new edges

	Node *tmp_nodes; //
	Edge *tmp_edges; //

	Node *snodes; // nodes of super graph
	Edge *sedges; // edges of super graph

	Node *tnodes; // nodes for mSPT
	Edge *tedges; // edges for mSPT
	int *parent; // parent for mSPT
	int *weight; // weight for mSPT
	Edge **pedges; // point to edges in the bidirectional representation
	Edge **ppedges; // point to edges in the connectivity graph
	Bin **buckets; // store all nontree edges with the same weight in the same bucket
	Bin *bins; // all nontree edges;
	int max_key;

	int *pp; // used for finding root when deleting edge

	Bin **heads; // used for processing steiner component queries on cg when updating graph
	Bin *ll; // store all vertices with their weights

	int *Q; // queue
	int *computed; // indicate whether a vertex is visited or not
	char *vis;

	Heap *heap; // used for decompose a graph

public:
	ConnGraph(const char *_dir) ;
	~ConnGraph() ;

	void read_cg(int binary) ;
	void read_mSPT() ;
	void mark_nontree() ;

	void check_parallel_edge() ;

	void test_kSC(int K) ;

	void process_update_query(const char *file_name) ;
	void insert_edge(int a, int b) ;
	void delete_edge(int a, int b) ;
	void smcc_query_mSPT(const char *file_name) ;
	void smcc_query_mSPT(int q_n, int *q) ;
	void smccl_query_mSPT(const char *file_name) ;
	void smccl_query_mSPT(int q_n, int *q, int L) ;
	void gen_update_query(int gen_query) ;

	void gen_random_smcc_query(const char* sizes) ;
	void gen_random_smccl_query(const char* sizes) ;

private:
	void add_edge(Node &node, Edge *edge) ; // add to the end
	void add_edge_sorted(Node &node, Edge *edge) ; // add edge, sorted by key
	void remove_edge(Node &node, Edge *edge) ; //delete an edge
	void merge_sgraph(int s, int t, Heap *heap) ; // merge s and t int the graph stored in snodes and sedges
	void update_bucket_structure(int pre_key, Bin *bin) ;
	void delete_from_bucket(int pre_key, Bin *bin) ;
	void insert_into_bucket(int key, Bin *bin) ;
	void update_mSPT_increment(const vector<Bin *> &incremented) ;
	void update_mSPT_decrement(int deleted_vertex, const vector<int> &decremented, int K) ;
	void remove_from_mSPT_bidirectional(Edge *tree_edge) ;
	void add_to_mSPT_bidirectional(int a, int b, int c, Edge *tree_edge) ;
	void output_cg() ;

	int count_cc() ; // count the number of connected components

	void check_mSPT() ;
	void check_cg() ;

	void decrease_weight(Edge *edge) ;
	void decrease_weight(Node &node, Edge *edge) ;
	void increase_weight(Edge *edge) ;
	void increase_weight(Node &node, Edge *edge) ;

	int find_root(int a, int &q_c) ;
	int find_root_K(int a, int &q_c, int K) ;
	void swap_tree_edge(int a, int b, Bin *bin, int removed_vertex, int deleted) ;
	void remove_inter_edges(const vector<Element *> &cc) ;

	void reassign_edge_weights_insert(int a, int b, const vector<Element *> &cc, int K, vector<Bin *> &incremented) ; // each component in cc is (k+1)-edge connected
	void reassign_edge_weights_delete(int a, int b, const vector<Element *> &cc, int K, vector<int> &decremented, int &deleted_vertex) ; // each component in cc is k-edge connected
	void extract_subgraph(int s) ;
	int extract_scp(int a, int b, int &sedge_c, int q_c, vector<int> &vv) ; // extract the steiner component containing (a,b), and return their connectivity
	int pair_connectivity(int a, int b, int &lca, int q_c) ; // compute the pair connectivity between a and b
	void decompose(int ss, vector<Element *> &cc, int K) ; //decompose the graph stored in snodes and sedges, with start vertex ss
};

#endif
