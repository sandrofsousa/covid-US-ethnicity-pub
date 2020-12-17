# Obtaining the edge-list and node properties for CCT data  

At the repository mentioned at the beginning, the reader will find the script
**get_adjacencies_connected.py** which returns the edge-list and the nodes
properties (ethnicity distribution) of the largest connected component. The
script takes the following inputs:
```
<shape>     Shapefile of the census tracts at country extent
<code_name> The loop uk code (GISJOIN) string to match the tracts
<pop_data>  CSV file containing the tracts within the CSA
<fileout>   Optional filename to save the nodes properties of the resulting
```
Obtaining the individual CSV file for each CSA is trivial and can be obtained
either by filtering the data by the **CSAA** field using the preferred editor or
using a library such as pandas in python to group rows by the column. The
CSV files from this process are available in the folder **census_ethnicity_csa**.

The resulting file with the node properties is named by convention
**nodes_ethnics_agg_censustract_csa_XXX** where XXX refers to the CSA numeric
code. The column containing the total population was removed so that the output
file contains:
```
'Node ID';
'White alone;
'Black or African American';
'American Indian and Alaska Native';
'Asian';
'Native Hawaiian and Other Pacific Islander';
'Some Other Race';
'Two or More Races'
```
Note that the **GISJOIN** field is not the same as GEOID10, for merging data from NHGIS GISJON field must be used.

Edge-list details:
* Node IDs are defined by the row index of the population table
* Edges are reported once
* Resulting graph considering the giant component only

---
# Running the CCT random walk  

The random walk on the adjacency graph can be simulated by running the python
script **rw_cct_ctime_jsd.py** and the following input must be provided:
```
<edge>    Edge-list file;
<prop>    Node properties file with the frequency of each ethnicity;
<num>     Number of walk repetitions from the same node;
<epsilon> Threshold value for the JSD divergence;
<idx>     range of nodes or single node ID to run the walk from, e.g.: 1, 0-10.
```
All the files provided here in this repository are already in input format
needed by the script. An output file for each node will be saved locally at
the same directory where the code is being executed and follows the format:
* Ethnicity
* Time to reach epsilon

The commute network uses the **rw_cct_ctime_jsd_directed.py** script and the
corresponding files in the "commute" folder should be used.


---

## List of files in the CCT repository

Adjacency:

- edges_ids_censustract_csa_XXX: Edge-list with the assigned numeric ID
- edges_geocode_censustract_csa_XXX: Edge-list with the GISJOIN codes
- nodes_ethnics_agg_censustract_csa_XXX: Node properties associated with the census tract
- nodes_id_geocode_XXX: Look up codes for node IDs to GISJOIN
- ethnics_agg_censustract_csa_XXX: Original CSV file with ethnicity data

Commute:
Note: Please refer to the **MFPT** section for details about data collection.

- colorethmix_XXX: Node properties
- network_XXX: Edge-list with the corresponding edge weight
