# ICS-GNN: A Lightweight Interactive Community Search via Graph Neural Network
Codes and data for PVLDB Vol 14 submission.

## Dependencies
Make sure you have installed
- python >= 3.8
- pytorch >= 1.5.0
- [torch-geometric](https://pytorch-geometric.readthedocs.io/en/latest/notes/installation.html) >= 1.6.1 
- texttable >=1.6.3
- networkx >=2.4
## Usage

#### Run 
- Clone this repo.
- Enter the directory where you clone it, and run the following code
    ```bash
    python main.py --data-set cora --community-size 30 --train-ratio 0.02 --subgraph-size 400 --seed-cnt 20
    ```
- Or you can try to start an iteration
    ```bash
    python main.py --iteration Ture
    ```
- Or you can run facebook 
    ```bash
    python main.py --iteration Ture
    #In order to keep consistent with the paper experiment,
    #the subgraph-size and train-ratio on Facebook are fixed as ego-graphs' size
    #and 8/ ego-graphs' size. And there is no rking loss in the paper experiment.
    ```
 
#### General Options
You can check out the other options available using:
```bash
python main.py --help
```
- `--epochs`, Number of training epochs. Default is 200.
- `--seed`, Random seed. Default is 42.
- `--dropout`, Dropout parameter. Default is 0.5.
- `--learning-rate`, Learning rate. Default is 0.01.
- `--data-set`, The name of data set. Default is cora.
- `--community-size`, The size of final community. Default is 30.
- `--train-ratio`, Test data ratio. Default is 0.02.
- `--subgraph-size`, The size of subgraphs. Default is 400. 
- `--layers`, The size of hidden layers. Default is [16].
- `--seed-cnt`, The number of random seeds. Default is 20.
- `--iteration`, Whether to start iteration. Default is False.
- `--upsize`, Maximum number of node can be found per iteration.Default is 20.
- `--possize`, Incremental train node pairs per iteration. Default is 1.
- `--round`, The number of iteration rounds. Default is 10.

 ## Datasets

The Cora/Reddit/Pubmed/Citeseer datasets are from [torch-geometric](https://pytorch-geometric.readthedocs.io/en/latest/notes/installation.html), the rest of the datasets come from [SNAP](http://snap.stanford.edu/data/index.html) 

Since Reddit is too big to upload, you can [download it](https://data.dgl.ai/dataset/reddit.zip) separately and untar it in `./data/reddit`  
