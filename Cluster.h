//
// Created by Nikolai Sharp on 9/20/15.
//

#ifndef CLUSTERING_CLUSTER_H
#define CLUSTERING_CLUSTER_H

#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Point.h"


namespace Clustering {

    typedef Point *PointPtr;
    //typedef std::shared_ptr<Point*> PointPtr;
    typedef struct LNode *LNodePtr;

//    struct LNode;
//    typedef LNode *LNodePtr;


    struct LNode {
        PointPtr p;
        LNodePtr next;
    };


    class Cluster {
    private:
        int size;
        //Using this as the head
        LNodePtr points;
        //point to current LNode in linked list
        LNodePtr ptPtr;
        //point to next LNode for comparison to ptPtr
        LNodePtr nextPtr;
        //point to last LNode
        LNodePtr endPtr;
        //used to create new LNode
        LNodePtr newPtr;
        //add member variable to hold number of dimensions this cluster holds
        int dim;
        // release points, tells class whether or not it can deallocate points
        bool __release_points;
        //centroid
        Point __centroid;


    public:
        Cluster() : size(0), points(nullptr), endPtr(nullptr), dim(0){};

        // The big three: cpy ctor, overloaded operator=, dtor
        Cluster(const Cluster &);
        Cluster &operator=(const Cluster &);
        ~Cluster();

        // Set functions: They allow calling c1.add(c2.remove(p));
        void add(const PointPtr &);
        const PointPtr &remove(const PointPtr &);

        // Overloaded operators

        // IO
        friend std::ostream &operator<<(std::ostream &, const Cluster & cluster);
        friend std::istream &operator>>(std::istream &is, Cluster &cluster)
        {
            std::string line;
            while (std::getline(is, line))
            {
                std::string line2 = line;
                std::stringstream lineStream(line);
                std::string value;
                int i = 0;
                while (getline(lineStream, value, ','))
                {
                    i++;
                }
                Point *tPoint = new Point(i);
                std::stringstream lineStream2(line2);

                lineStream2 >> *tPoint;
                cluster += tPoint;
            }
            return is;
        }

        // Set-preserving operators (do not duplicate points in the space)
        // - Friends
        friend bool operator==(const Cluster &lhs, const Cluster &rhs); //TODO

        // - Members
		Cluster &operator+=(const Cluster &rhs); // union
		Cluster &operator-=(const Cluster &rhs); // (asymmetric) difference

        Cluster &operator+=(const PointPtr &rhs); // add point
        Cluster &operator-=(const PointPtr &rhs); // remove point

        // Set-destructive operators (duplicate points in the space)
        // - Friends
        friend const Cluster operator+(const Cluster &lhs, const Cluster &rhs);
        friend const Cluster operator-(const Cluster &lhs, const Cluster &rhs);

        friend const Cluster operator+(const Cluster &lhs, const PointPtr &rhs);
        friend const Cluster operator-(const Cluster &lhs, const PointPtr &rhs);

        //centroid functions
        void setCentroid(const Point&);
        const Point getCenroid() {return __centroid;}


    };

//     inlclude "Cluster.cpp"   //keep for templates in Cluster // don't forget to remove Cluster.cpp from CMakeLists.

}
#endif //CLUSTERING_CLUSTER_H
