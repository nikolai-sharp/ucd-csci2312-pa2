//
// Created by Nikolai Sharp on 9/20/15.
//

#ifndef CLUSTERING_CLUSTER_H
#define CLUSTERING_CLUSTER_H

#include "Point.h"

namespace Clustering {

    typedef Point *PointPtr;
    typedef struct LNode *LNodePtr;

//    struct LNode;
//    typedef LNode *LNodePtr;


    struct LNode {
        PointPtr p;
        LNodePtr next;
    };


    class Cluster {
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
        friend std::ostream &operator<<(std::ostream &, const Cluster &);
        //friend std::istream &operator>>(std::istream &, Cluster &); //not sure what to do with this. not in inst.

        // Set-preserving operators (do not duplicate points in the space)
        // - Friends
        friend bool operator==(const Cluster &lhs, const Cluster &rhs); //TODO

        // - Members
        Cluster &operator+=(const Cluster &rhs); // union
        Cluster &operator-=(const Cluster &rhs); // (asymmetric) difference

        Cluster &operator+=(const Point &rhs); // add point
        Cluster &operator-=(const Point &rhs); // remove point

        // Set-destructive operators (duplicate points in the space)
        // - Friends
        friend const Cluster operator+(const Cluster &lhs, const Cluster &rhs);
        friend const Cluster operator-(const Cluster &lhs, const Cluster &rhs);

        friend const Cluster operator+(const Cluster &lhs, const PointPtr &rhs);
        friend const Cluster operator-(const Cluster &lhs, const PointPtr &rhs);

    };

}
#endif //CLUSTERING_CLUSTER_H
