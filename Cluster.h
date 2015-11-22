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
#include <vector>
#include <forward_list>
#include "Exceptions.h"
#include "Point.h"


namespace Clustering
{

//    typedef Point *PointPtr;
    //typedef std::shared_ptr<Point*> PointPtr;
//    typedef struct LNode *LNodePtr;

//    struct LNode;
//    typedef LNode *LNodePtr;


//    struct LNode {
//        PointPtr p;
//        LNodePtr next;
//    };


    class Cluster {
    private:
        std::forward_list<Point> points;
        int size;
        //point to current LNode in linked list
        std::forward_list<Point>::iterator pItr;
        //point to next LNode for comparison to ptPtr
        std::forward_list<Point>::iterator nItr;
        
        
        
        ////////////////todo
        //used to create new LNode
        std::forward_list<Point>::iterator endItr;
        //add member variable to hold number of dimensions this cluster holds
        unsigned int dim;
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
        void add(const Point &);
        const Point &remove(const Point &);

        // Overloaded operators

        // IO
        friend std::ostream &operator<<(std::ostream &, const Cluster & cluster);
        friend std::istream &operator>>(std::istream &is, Cluster &cluster)
        {
            try
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
                    Point tPoint(cluster.dim);
                    //                std::stringstream lineStream2(line2);
                    
                    lineStream >> tPoint;
                    //std::cout << tPoint;
                    cluster.add(tPoint);
                }
            }
            catch (DimensionalityMismatchEx dimMM)
            {
                std::cerr << "Cluster::operator>>;" << dimMM;
            }
            return is;
        }

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

        //KMeans and centroid functions
        void setCentroid(Point &point);
        const Point getCenroid() {return __centroid;}
        void computeCentroid();
        bool centroidIsValid() {bool truth = __centroidIsValid; return truth;}
        void pickPoints(int k, std::vector<Point> &pointArray);
        int getSize() const {return size;}
        double intraClusterDistance() const;
        friend double interClusterDistance(const Cluster &c1, const Cluster &c2);
        double getClusterEdges() {return size * (size - 1) / 2;}
        friend double interClusterEdges(const Cluster &c1, const Cluster &c2) {return c1.getSize()*c2.getSize();}


        //id functions
        int getID() {return __id;}
        int generateID() {static int numberOfClusters = 0; return  ++numberOfClusters;}

        Point &operator[](int i); //use to access points in Kmeans
        
        int getDimms() const {return dim;}
        void clear();





        class Move
        {
        public:
            Move(const Point &ptr, Cluster &from, Cluster &to);
        private:
            void perform(const Point &ptr, Cluster &from, Cluster &to);
        };

    };

//     inlclude "Cluster.cpp"   //keep for templates in Cluster // don't forget to remove Cluster.cpp from CMakeLists.

}
#endif //CLUSTERING_CLUSTER_H
//