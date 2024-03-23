from src.parser import parameter_parser
from src.subgraph import SubGraph
from src import utils
import torch
from torch_geometric.datasets import Reddit
from src.pre_community import *



def load_data(args):
    '''
    Load data
    '''
    seed_list = None
    train_nodes = None
    labels= None
    if args.data_set in ['cora','citeseer','pubmed']:
        graph = utils.graph_reader1(args.data_set)
        features = utils.feature_reader1(args.data_set)
        target = utils.target_reader1(args.data_set)
    if args.data_set in ["reddit"]:
        path = osp.join(osp.dirname(osp.realpath(__file__)), 'data', args.data_set)
        data = Reddit(path)[0]
        edge_index, features, target = data.edge_index.numpy(), data.x.numpy(), data.y.numpy()
        graph = utils.construct_graph(edge_index)
        target = target[:, np.newaxis]
    if args.data_set in ["dblp", 'amazon']:
        if args.iteration:
            print("It is in load_data with iteration")
            edge, labels = load_snap(data_set='com_' + args.data_set, com_size=args.community_size)
            graph = nx.from_edgelist(edge)
            n = graph.number_of_nodes()
            features = np.array([np.random.normal(0, 1, 100) for _ in range(n)])
            target = None
        else:
            print("It is in load_data without iteration")
            edge,seed_list,train_nodes,labels=pre_com(data_set='com_'+args.data_set,subgraph_list=[args.subgraph_size],train_ratio=args.train_ratio,cmty_size=args.community_size,seed_cnt=args.seed_cnt)
            graph = nx.from_edgelist(edge)
            n = graph.number_of_nodes()
            features=np.array([np.random.normal(0, 1, 100) for _ in range(n)])
            target=None

    return graph,features,target,seed_list,train_nodes,labels

def main():
    '''
    Parsing command line parameters
    '''
    args = parameter_parser()
    torch.manual_seed(args.seed)
    random.seed(args.seed)
    if(args.iteration==True):
        run_iteration(args)
        return
    if args.data_set == 'facebook':
        run_facebook(args)
    else:
        run(args)

def run_facebook(args):
    '''
    Run the search at random community on each of Facebook's Ego Graphs
    In order to keep consistent with the paper experiment,
    the subgraph-size and train-ratio on Facebook are fixed as ego-graphs' size
    and 8/ ego-graphs' size. And there is no Rking loss in the paper experiment.
    '''
    seedlist = [0, 107, 348, 414, 686, 698, 1684, 1912, 3437, 3980]
    for seed in seedlist:
        args.ego = 'facebook_' + str(seed)
        edges, features, circle, seed = load_facebook(seed=seed)
        features = np.array(features)
        graph = nx.from_edgelist(edges)
        target = None
        args.subgraph_size=len(graph.nodes)
        args.train_ratio=8/args.subgraph_size
        subg = SubGraph(args, graph, features, target)
        utils.tab_printer(args)
        itr = 0
        while itr< args.seed_cnt:
            print("%d "% (itr)+20*'*')
            print("\nCurrent Seed Node is %d" % seed)
            label=circle[random.randint(0, len(circle) - 1)]
            random.shuffle(label)
            negnode=list(set(graph.nodes).difference(set(label+[seed])))
            random.shuffle(negnode)
            numlabel=int(args.subgraph_size*args.train_ratio/2)
            trian_node=label[:numlabel-1]+[seed]+negnode[:numlabel]
            print(trian_node,label,seed)
            isOK= subg.community_search(seed,trian_node,label+[seed])
            itr+=isOK
        subg.methods_result()

def run(args):
    '''
    Randomly selected seeds run the community search
    The SNAP dataset is pre-processed,Randomly select a community in which the node label 1 and the other node label 0
    '''
    print("It is in run(args)")
    graph, features, target, seed_list, train_nodes, labels = load_data(args)
    subg = SubGraph(args, graph, features, target)
    utils.tab_printer(args)
    trian_node = None
    label = None
    itr = 0
    print("Now assigning seed node\n")
    with open('./grp/queue_input.txt', 'r') as f:
        seed = int(f.readline().strip())
    with open('./grp/m.txt', 'r') as f:
        for line in f:
            line = line.strip().split()
            if int(line[0]) == seed:
                seed = int(line[1])
                break
    

    while itr < args.seed_cnt:
        # seed = random.randint(0, len(subg.graph.nodes) - 1)
        seed_list = None
        trian_node = [seed]
        label = [seed]
        if (seed_list is not None):
            seed = seed_list[itr]
            trian_node = train_nodes[itr]
            label = labels[itr]
        print("%d " % (itr) + 20 * '*')
        print("\nCurrent Seed Node is %d" % seed)
        isOK = subg.community_search(seed, trian_node, label)
        itr+=isOK
    subg.methods_result()


def run_iteration(args):
    '''
    Run community search with iteration
    '''
    print("It is in run_iteration(args)")
    graph, features, target, _ ,_ ,com_list= load_data(args)
    utils.tab_printer(args)
    subg = SubGraph(args, graph, features, target)
    itr = 0
    with open('./data/queue_input.txt', 'r') as f:
        seed = int(f.readline().strip())

    while itr < args.seed_cnt:
        if args.data_set in ['dblp','amazon']:
                # seed_list = None
                # trian_node = [seed]
                # label = [seed]            
                random.shuffle(com_list)
                com_max = com_list[0]
                target =[ 1 if i in com_max else 0 for i in range(len(graph.nodes))]
                subg.target = np.array(target)[:, np.newaxis]
                with open('./data/queue_input.txt', 'r') as f:
                    seed = int(f.readline().strip())
                # seed = com_max[random.randint(0, len(com_max) - 1)]
        else:
                seed = random.randint(0, len(graph.nodes) - 1)
        print("%d " % (itr) + 20 * '*')
        print("\nCurrent Seed Node is %d" % seed)
        isOK = subg.community_search_iteration(seed)
        itr += isOK
    subg.methods_result()

if __name__ == "__main__":	
    main()
