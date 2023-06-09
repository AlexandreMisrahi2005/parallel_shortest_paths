def add_weight_to_lines(file_name):
    """
    file_name should be a file of edge pairs
    this adds a weight to each line: 1 if the line is unique, else its multiplicity (edge weight)
    """
    edges_seen = dict()
    try:
        with open(file_name, 'r') as file:
            lines = file.readlines()
        for line in lines:
            u,v = line.split()
            if (u,v) in edges_seen.keys():
                edges_seen[(u,v)] += 1
            else:
                edges_seen[(u,v)] = 1
        with open(file_name, 'w') as file:
            for edge,weight in edges_seen.items():
                modified_line = str(edge[0])+' '+str(edge[1])+' '+str(weight)+'\n'
                file.write(modified_line)
        print(f'Successfully modified {file_name}')
    except FileNotFoundError:
        print(f'Error: {file_name} not found')
    except Exception as e:
        print(f'An error occurred: {e}')

file_name = "graphs/rmat_medium.txt"
add_weight_to_lines(file_name)