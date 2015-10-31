# ucd-csci2312-pa2

pa3_1.0

Compiler: I used CLion 1.1.1 for mac.


# Instructions:

Open ifstream:
std::ifstream csv("infile.txt");
    
Initiate a KMeans instance with the number of dimensions of the data:
KMeans instance(dimensions);

Read points into instance:
csv >> instance;

Run the instance with the number(K) of clusters you wish
instance.run(k);

The default output filename is Sharp_pa3_out.txt. This can be changed at either: KMeans.cpp line 132 or KMeans.h line 78