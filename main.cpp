#include <iostream>
#include <fstream>
#include "Point.h"
#include "Cluster.h"
#include "KMeans.h"
#include "Exceptions.h"

using namespace Clustering;

int main() {
    
    
        std::ifstream csv("infile3000k5d5.txt");
//        std::ifstream csv("infile10kd5.txt");
//        std::ifstream csv("infile2kd3.txt");
    
        KMeans instance(5);
        
        if (csv.is_open())
        {
            csv >> instance;
            instance.run(5);
        }
    

    
    
    

//    std::cout << instance;
    
    
    
//    try
//    {
//        Point p1(3);
//        Point p2(4);
//        Point p3(5);
//        p1.setValue(0, 1);
//        p1.setValue(1, 2);
//        p1.setValue(2, 3);
//        p2.setValue(0, 4);
//        p2.setValue(1, 5);
//        p2.setValue(2, 6);
//        p2.setValue(3, 7);
//       
//        p1 != p2;
//    }
//    catch (DimensionalityMismatchEx dimMM)
//    {
//        std::cout << dimMM;
//    }

    
    
    return 0;
}

