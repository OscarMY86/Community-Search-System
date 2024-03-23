# Read the edges from the file
with open('edges.txt', 'r') as file:
    edges = file.readlines()

# Sort the edges based on column 0 and column 1
sorted_edges = sorted(edges, key=lambda x: (int(x.split()[0]), int(x.split()[1])))

# Write the sorted edges back to the file
with open('dblp-graph.txt', 'w') as file:
    file.writelines(sorted_edges)
