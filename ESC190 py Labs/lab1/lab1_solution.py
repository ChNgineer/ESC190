from lab1_utilities import *
from collections import deque
import math


def get_installations_from_file(file_name):
    installations = []
    # Split the lines and discard header & garbage
    with open(file_name) as f:
        data = f.readlines()
        for line in data[1:]:
            dat = line.strip().split('\t')
            name, ward, position, indoor = dat[0], int(dat[2]), (float(dat[7]), float(dat[8])), (dat[-1] == 'INDOORS')
            installations.append(Installation(name, ward, position, indoor))
    return installations


def euclidean_distance(position1, position2):
    return ((position1[0]-position2[0])**2 + (position1[1]-position2[1])**2)**0.5


def get_adjacency_mtx(installations):
    # Make the matrix
    weighted_connect = [[0]*len(installations) for _ in range(len(installations))]

    for i in range(len(installations)):
        a = installations[i]
        for j in range(i+1, len(installations)):
            b = installations[j]
            if math.fabs(a.ward - b.ward) <= 1:
                scale = 1.5 if a.indoor or b.indoor else 1
                distance = scale * euclidean_distance(a.position, b.position)
                weighted_connect[i][j] = distance
                weighted_connect[j][i] = distance
    return weighted_connect


def make_graph(installations):
    # My solution uses sorting, possible without it
    installs = sorted(installations, key = lambda x: x.ward)

    # Get the name and edges for the graph
    values = [install.name for install in installs]
    weighted_connect = get_adjacency_mtx(installs)

    return Graph(values, weighted_connect)


def find_shortest_path(installation_A, installation_B, graph):
    queue = deque([installation_A])

    visited = {}
    unvisit = {node: float('inf') for node in graph.installations}

    path_to_visited = {}
    path_to_unvisited = {installation_A: [installation_A]}

    prev = [None]*len(graph.installations)
    node = None
    dist = 0

    while (len(queue) > 0) and (node != installation_B):
        node = queue.popleft()

        # Don't revisit nodes
        if node in visited:
            continue
        else:
            visited[node] = dist
            path_to_visited[node] = path_to_unvisited[node]
            del unvisit[node]

        # Get index of current node
        index = graph.artwork_to_index[node]

        # Update all neighour edges
        for n, weight in enumerate(graph.adjacency_mtx[index]):
            # Skip if no connecting edge
            if weight == 0:
                continue
            # Skip if visited node
            elif graph.installations[n] not in unvisit:
                continue
            # Update cost of neighbours
            elif unvisit[graph.installations[n]] > (dist + weight):
                unvisit[graph.installations[n]] = dist + weight
                path_to_unvisited[graph.installations[n]] = path_to_visited[node] + [graph.installations[n]]
                prev[n] = node

        # Pick the closest unvisited
        if node == installation_B:
            break
        new = min(unvisit, key=unvisit.get)

        # If there is no path possible
        if unvisit[new] == float('inf'):
            return None, []

        # Update distance and new node
        dist = unvisit[new]
        queue.append(new)
    return visited[installation_B], path_to_visited[installation_B]
