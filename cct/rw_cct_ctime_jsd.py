"""
This program is free software. You can use, copy, distribute, modify and/or redistribute
it under the terms of the MIT/Expat License. See the file LICENSE for more details.

(c) 2020 Sandro Sousa

If you use this software please consider citing the original paper:

A. Bassolas, S. Sousa, V. Nicosia
"Diffusion segregation and the disproportionate incidence of
COVID-19 in African American communities", Journal of The Royal
Society Interface


-----------------------------------------------------------------------------

Compute the class coverage time CCT as a function of time where the
value <N(t)> corresponds to the number of unique classes seen
at time t averaged over num iterations. The time series Y(t) shows
the time evolution of the classes seen for each node.
<edges> reported as UNDIRECTED, self-loops allowed, no multiple edges.
<prop> count of each group by area, no total at MxC matrix
<num> number of walk repetions for average calculation
<epsilon> threshold value for the JSD divergence (diff P|Q < th)
<idx> range of nodes or single id to compute, e.g.: 1, 0-10

> output:
    class
    time to reach epsilon

-----------------------------------------------------------------------------
"""

import numpy as np
import sys
import os


if len(sys.argv) < 5:
    print("Usage: %s <edges> <prop> <num> <epsilon> <idx>\n" % sys.argv[0])
    exit(1)


def compute_neighbours(file_in):
    # dictionary with node neighbours
    E = dict()
    with open(file_in, "r") as lines:
        edges = lines.readlines()
        n_cols = len(edges[0].split(" "))
        if n_cols == 2:
            wg = False
            for line in edges:
                i,j = [int(x) for x in line.split()]
                # process self-loop
                if i == j:
                    if i in E:
                        E[i].append(j)
                    else:
                        E[i] = [j]
                    continue
                # add j to neighbours of node i
                if i in E:
                    E[i].append(j)
                if i not in E:
                    E[i] = [j]
                # add i to neighbours of node j
                if j in E:
                    E[j].append(i)
                if j not in E:
                    E[j] = [i]
        elif n_cols == 3:
            wg = True
            for line in edges:
                i,j,w = line.split()
                i,j,w = (int(i), int(j), float(w))
                # process self-loop
                if i == j:
                    if i in E:
                        E[i][0].append(j) # list of neighbours
                        E[i][1].append(w) # list of weights
                    if i not in E:
                        E[i] = [[j],[w]]
                    continue
                # # add j to neighbours of node i
                if i in E:
                    E[i][0].append(j) # list of neighbours
                    E[i][1].append(w) # list of weights
                if i not in E:
                    E[i] = [[j],[w]]
                # # add i to neighbours of node j
                if j in E:
                    E[j][0].append(i)
                    E[j][1].append(w)
                if j not in E:
                    E[j] = [[i],[w]]
        else:
            print("too many columns in edges")
            sys.exit(1)
    return E, wg


def walk(i, Q_dist, idx_c):
    P = np.copy(P_dist)  # city level distribution
    Q = np.copy(Q_dist)  # walk cummulative distribution
    P_dist_new = np.delete(P, idx_c)
    Q_dist_new = np.delete(Q, idx_c)
    t = 0  # time to reach epsilon
    JSD = 1 # divergence
    while JSD > epsilon:
        # selects one of i neighbours
        if weighted:
            # select by weights (normalised)
            weights = neigh_dict[i][1]
            weights = [w/sum(weights) for w in weights]
            j = np.random.choice(neigh_dict[i][0], p=weights)
        else:
            # select uniformly
            j = np.random.choice(neigh_dict[i])
        Q_dist_j = np.delete(classes[j], idx_c)
        Q_dist_new += Q_dist_j
        # compute only at each t steps
        # if t < 500 or t%20 == 0:
        # calc Kullback–Leibler divergence of each dist
        M = 0.5 * (P_dist_new + Q_dist_new)
        dkl_p = calc_DKL(P_dist_new, M)
        dkl_q = calc_DKL(Q_dist_new, M)
        # calc Jensen–Shannon divergence
        JSD = 0.5 * (dkl_p + dkl_q)
        # update time and next neighbour
        t += 1
        i = j
    return t


def calc_DKL(p, q):  # compute Kullback–Leibler divergence
    p = p / sum(p)
    q = q / sum(q)
    dkl = 0.0
    for i in range(len(p)):
        if p[i] > 0:
            dkl += p[i] * np.log(p[i] / q[i])
    return dkl


# %%
# initialize dict with neighbours
neigh_dict, weighted = compute_neighbours(sys.argv[1])

# load population data and set variables
prop = np.loadtxt(sys.argv[2], dtype='float')
classes = prop[:, 1:]
P_dist = classes.sum(axis=0) # city level distribution

# get classes missing data at city level and add 1
idxs = np.where(P_dist == 0)[0]
if len(idxs) > 0:
    classes[:, idxs] = 1

# process range or single node from param
if "-" in sys.argv[5]:
    start, end = [int(x) for x in sys.argv[5].split('-')]
else:
    start, end = [int(sys.argv[5]), int(sys.argv[5])+1]
num = int(sys.argv[3])
epsilon = float(sys.argv[4])


# %%
# loop over all nodes given on input
for node in range(start, end):
    times = []
    Q_dist = classes[node]
    # repeat "num" iterations for each node
    for n in range(num):
        # class sampled from distribution at node
        sampled = np.random.choice(Q_dist, p=[x/sum(Q_dist) for x in Q_dist])
        idx_c = np.where(Q_dist == sampled)[0][0]
        # launch class dependent walk
        t = walk(node, Q_dist, idx_c)
        times.append([idx_c, t])

    # write output per node
    fname = "rwcct_jsd_%s_%s_%s"%(num, node, os.path.basename(sys.argv[2]))
    with open(fname, "w") as res:
        for line in times:
            ln = " ".join([str(x) for x in line])
            res.write(ln+"\n")
