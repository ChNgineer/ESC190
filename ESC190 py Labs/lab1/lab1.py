from lab1_utilities import *


def get_installations_from_file(file_name):
    f = open(file_name, 'r')
    next(f)
    result = []
    for i in f:
        temp_list = i.strip().split('\t')
        bool = False
        if str(temp_list[15]).upper() == 'INDOORS':
            bool = True
        curr_inst = Installation(str(temp_list[0]), int(temp_list[2]), (float(temp_list[7]), float(temp_list[8])), bool)
        result.append(curr_inst)
    return result


def euclidean_distance(position1, position2):
    dx = position2[0] - position1[0]
    dy = position2[1] - position1[1]
    return (dx**2 + dy**2)**(1/2)


def get_adjacency_mtx(installations):
    result = []
    for i in installations:
        temp_list = []
        for j in installations:
            if ((i.ward == j.ward) or (i.ward == j.ward + 1) or (i.ward == j.ward -1)) and i != j:
                if (i.indoor == False) and (j.indoor == False):
                    temp_list.append(abs(euclidean_distance(i.position, j.position)))
                else:
                    temp_list.append(1.5*abs(euclidean_distance(i.position, j.position)))
            else:
                temp_list.append(0)
        result.append(temp_list)
    return result


def make_graph(installations):
    installations = get_installations_from_file('test_data.txt')
    name_list = []
    for i in installations:
        name_list.append(i.name)
    graph = Graph(name_list, get_adjacency_mtx(installations))
    return graph


def find_shortest_path(installation_A, installation_B, graph):
    visited = [installation_A]
    dict_of_distances = {}
    total_dist = 0
    unvisited = []
    new_graph = graph.adjacency_mtx.copy()
    for i in range(len(graph.adjacency_mtx)):
        dict_of_distances.update({graph.installations[i]: float('inf')})
        for j in range(len(graph.adjacency_mtx[i])):
            if graph.adjacency_mtx[i][j] > 0:
                unvisited.append(graph.installations[i])
    for i in range(len(graph.adjacency_mtx)):
        for j in range(len(graph.adjacency_mtx[i])):
            if graph.adjacency_mtx[i][j] == 0:
                new_graph[i][j] = float('inf')
    unvisited = list(dict.fromkeys(unvisited))
    unvisited.pop(0)
    dict_of_distances.update({installation_A:0})

    for i in range(len(new_graph)):
        for j in range(len(new_graph[i])):
            if new_graph[i][j] < dict_of_distances.get(graph.installations[i]) and new_graph[i][j] == min(new_graph[i]):
                total_dist += new_graph[i][j]
                dict_of_distances.update({graph.installations[i]: total_dist})
                visited.append(graph.installations[i])
    print(dict_of_distances)
    return (dict_of_distances.get(installation_B), visited)
