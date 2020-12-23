# Data

The reader interested in using the formatted data used on the simulations
can obtain a copy at the [DRYAD][1] repository.
References to files and folders consider the data available at the
repository.


# Obtaining the edge-list and node properties for CCT data  

The script `get_adjacencies_connected.py` returns the edge-list and the nodes
properties (ethnicity distribution) of the largest connected component. It
takes the following inputs:

> shape:     Shapefile of the census tracts at country extent  
> code_name: The loop uk code (GISJOIN) string to match the tracts  
> pop_data:  CSV file containing the tracts within the CSA  
> fileout:   Optional filename to save the nodes properties of the resulting  

Obtaining the individual CSV file for each CSA is trivial and can be obtained
either by filtering the data by the `CSAA` field using the preferred editor or
using a library such as pandas in python to group rows by the column. The
CSV files from this process are available in the folder `census_ethnicity_csa`
at [DRYAD][1].

The resulting file with the node properties is named by convention
`nodes_ethnics_agg_censustract_csa_XXX` where `XXX` refers to the CSA numeric
code. The column containing the total population was removed so that the output
file contains:

> Node ID;  
> White alone;  
> Black or African American;  
> American Indian and Alaska Native;  
> Asian;  
> Native Hawaiian and Other Pacific Islander;  
> Some Other Race;  
> Two or More Race'  

Note that the `GISJOIN` field is not the same as `GEOID10`, for merging data
from NHGIS the `GISJOIN` field must be used. The definition of each CSA for
the adjacency data is available in the file `csacodes_adjacency.csv` and it
contains the CSA numeric code, description and state. For CSA definitions of
the commute data please consult the file `csacodes.txt` at the MFPT folder
in [DRYAD][1].

Edge-list details:
* Node IDs are defined by the row index of the population table
* Edges are reported once
* Resulting graph considering the giant component only

---
# Running the CCT random walk  

The random walk on the adjacency graph can be simulated by running the python
script `rw_cct_ctime_jsd.py` and the following input must be provided:

> edge:    Edge-list file;  
> prop:    Node properties file with the frequency of each ethnicity;  
> num:     Number of walk repetitions from the same node;  
> epsilon: Threshold value for the JSD divergence;  
> idx:     range of nodes or single node ID to run the walk from, e.g.: 1, 0-10.  

All the files provided in [DRYAD][1] are already in the input format
needed by the script. An output file for each node will be saved locally at
the same directory where the code is being executed and follows the format:
```
"Ethnicity" "Time to reach epsilon"
```
where each line corresponds to one repetition of the random walk from node i.

The commute network uses the `rw_cct_ctime_jsd_directed.py` script and the
corresponding files in the "commute" folder should be used.

---

## List of files in the DRYAD CCT repository

**Adjacency:**

Adjacency:
- edges_ids_censustract_csa_XXX: Edge-list with the assigned numeric ID
- nodes_ethnics_agg_censustract_csa_XXX: Node properties associated with the census tract

Suplementary files:
- edges_geocode_censustract_csa_XXX: Edge-list with the GISJOIN codes
- nodes_id_geocode_XXX: Look up codes for node IDs to GISJOIN
- ethnics_agg_censustract_csa_XXX: Original CSV file with ethnicity data


**Commute:**   
*Note that the commute data in the CCT folder corresponds to the same used in
**MFPT** where reformatting was used for convenience only to run the `CCT`
random walk.*

- colorethmix_XXX: Node properties
- network_XXX: Edge-list with the corresponding edge weight


[1]: https://doi.org/10.5061/dryad.hqbzkh1f9 "DRYAD"
