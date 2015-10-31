# ucd-csci2312-pa2

pa3_1.0

Compiler: I used CLion 1.1.1 for mac.


# Instructions:
in main.cpp:


1. Open ifstream:
  
std::ifstream csv("infile.txt");
    

2. Initiate a KMeans instance with the number of dimensions of the data:
  
KMeans instance(dimensions);


3. if file is open, read it into the KMeans instance, and run it
  
if (csv.is_open())
{
  csv >> instance;
  instance.run(K);
}


The default output filename is Sharp_pa3_out.txt. This can be changed at either: KMeans.cpp line 132 or KMeans.h line 78
