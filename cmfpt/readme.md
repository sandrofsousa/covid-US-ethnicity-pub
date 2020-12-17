Files and codes contained:

Network files for each city
Ethnicity files for each city
Cell assignment files for each city
Codes to obtain the mfpt between ethnicities for the adjacency and commuting grapjs

Network files:

The network files for the adjacency graph can be found are adjcsa/network_*.csv  where * corresponds to the city CSA code
The network files for the commuting graph can be found are comcsa/network_*.csv  where * corresponds to the city CSA code

It is a directed graph whose heading corresponds to:

source;target;weight


Ethnicity files:

The ethnicity files for the adjacency graph can be found are adjcsa/coloreth_*.csv  where * corresponds to the city CSA code
The ethnicity files for the commuting graph can be found are comcsa/colorethmix_*.csv  where * corresponds to the city CSA code

The heading for them is 

node;ethnicity;probability

The correspondence between the class code and each ethnicity is:


0-->White alone
1-->Black or African American alone
2-->American Indian and Alaska Native alone
3--> Asian alone
4--> Native Hawaiian and Other Pacific Islander alone
5-->Some Other Race alone
6-->Two or More Races

Cell equivalence files:

The equivalence files for the adjacency graph can be found are adjcsa/classequi_*.csv  where * corresponds to the city CSA code
The equivalence files for the commuting graph can be found are comcsa/classequi_*.csv  where * corresponds to the city CSA code

The heading for them is 

census_tract_id;node_id

Codes:

There a total of 4 codes, two for the results on the real city and two for the null-model.

The codes for the real cities are name as adj.c for the adjacency network and com.c for the commuting graph and both can be used by compiling and introducing the csa code of the desired city

gcc adj.c -lm
./a.out csa_code

gcc com.c -lm
./a.out csa_code


The codes for the null model are named as adj_null.c for the adjacency network and com_null.c for the commuting graph and both can be used by compiling and introducing the csa code of the desired city


gcc adj_null.c -lm
./a.out csa_code

gcc com_null.c -lm
./a.out csa_code


