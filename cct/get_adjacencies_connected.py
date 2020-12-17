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

Reads a shapefile and population data, find the neighbouring nodes in the
giant component and save results to local files.
Inputs:
<shape>:    Shapefile of the census tracts at country extent;
<code_name>:The loop uk code (GISJOIN) string to match the tracts;
<pop_data>: CSV file containing the tracts within the CSA;
<fileout>:  Optional filename to save the nodes properties of the resulting
connected component.

Returns: a undirected edge list in the format (i, j) and
the equivalent prop_file format for the random walk. Considers pop_data IDs as
input for polygon filtering.

-----------------------------------------------------------------------------
"""


####------
# Reads a shapefile and population data, find the neighbouring nodes in the
# giant component and save results to local files.
# Returns: a undirected edge list in the format (i, j) and
# the equivalent prop_file format for RW input. Considers pop_data IDs as
# input for polygon filtering, key:ward_code or code
####------

import pandas as pd
from shapely.geometry import shape
import fiona
import sys
import itertools
import networkx as nx


if len(sys.argv) < 4:
    print("Usage: %s <shape> <code_name> <pop_data> [fileout]\n" % sys.argv[0])
    exit(1)

arg_shape = sys.argv[1]
arg_code = str(sys.argv[2])
pop_data = pd.read_csv(sys.argv[3], sep=',', dtype=str, header=None)

# create edge_list using shape file features ID
with fiona.open(arg_shape) as shp:
    feat_dict = {feat['properties'][arg_code]:feat for feat in shp}
shp.close()

edges_temp = []

# get all adjacencies
for i, j in itertools.combinations(pop_data.iloc[:, 0], 2):
        geom1 = shape(feat_dict[i]['geometry'])
        geom2 = shape(feat_dict[j]['geometry'])
        if geom1.touches(geom2):
            idx_i = pop_data[pop_data.iloc[:,0] == i].index[0]
            idx_j = pop_data[pop_data.iloc[:,0] == j].index[0]
            edges_temp.append([idx_i, idx_j, i, j])

# get largest component, build graph with geocodes
G = nx.Graph()
G.add_edges_from([[x[2],x[3]] for x in edges_temp])
largest_cc = max(nx.connected_components(G), key=len)
G_new = G.subgraph(largest_cc).copy()

# update pop_data with only values in giant component
pop_data = pop_data[pop_data.iloc[:,0].isin(largest_cc)]
pop_data = pop_data.reset_index(drop=True)

# output file for edges
edge_ids = "edges_ids_"+sys.argv[3][-23:-4]+".txt"
edge_geocodes = "edges_geocode_"+sys.argv[3][-23:-4]+".txt"

# save edge list with geocode and with id
with open(edge_ids, "w") as ids, open(edge_geocodes, "w") as geos:
    for i,j in G_new.edges():
        idx_i = pop_data[pop_data.iloc[:,0] == i].index[0]
        idx_j = pop_data[pop_data.iloc[:,0] == j].index[0]
        ids.write("%s %s\n" %(idx_i, idx_j))
        geos.write("%s %s\n" %(i, j))

# save node property file if the argument is passed
# ignore area code and sum column
if len(sys.argv) > 4:
    pop_data.iloc[:, 2:].to_csv(
    sys.argv[4],
    sep=' ',
    header=False,
    index=True)
