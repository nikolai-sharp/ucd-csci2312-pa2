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
        unsigned int dim;
        // release points, tells class whether or not it can deallocate points
        bool __release_points;
        //centroid
        Point __centroid;
        bool __centroidIsValid;
        //cluster id
        int __id;


    public:
        //declaration of Move class:
        friend class Move; //TODO check this
        //Cluster();

        Cluster(unsigned int d);

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
//                std::string line2 = line;
                std::stringstream lineStream(line);
//                std::string value;
//                int i = 0;
//                while (getline(lineStream, value, ','))
//                {
//                    i++;
//                }
                Point *tPoint = new Point(cluster.dim);
//                std::stringstream lineStream2(line2);

                lineStream >> *tPoint;
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

        //KMeans and centroid functions
        void setCentroid(Point &point);
        const Point getCenroid() {return __centroid;}
        void computeCentroid();
        bool centroidIsValid() {bool truth = __centroidIsValid; return truth;}
        void pickPoints(int k, PointPtr pointArray);
        int getSize() const {return size;}
        double intraClusterDistance() const;
        friend double interClusterDistance(const Cluster &c1, const Cluster &c2);
        double getClusterEdges() {return size * (size - 1) / 2;}
        friend double interClusterEdges(const Cluster &c1, const Cluster &c2) {return c1.getSize()*c2.getSize();}


        //id functions
        int getID() {return __id;}
        int generateID() {static int numberOfClusters = 0; return  ++numberOfClusters;}

        PointPtr &operator[](int i); //use to access points in Kmeans





        class Move
        {
        public:
            Move(const PointPtr &ptr, Cluster &from, Cluster &to) {perform(ptr, from, to);}
        private:
            void perform(const PointPtr &ptr, Cluster &from, Cluster &to) {to.add(from.remove(ptr));}
        };

    };

//     inlclude "Cluster.cpp"   //keep for templates in Cluster // don't forget to remove Cluster.cpp from CMakeLists.

}
#endif //CLUSTERING_CLUSTER_H
