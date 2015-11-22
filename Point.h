//
// Created by Nikolai Sharp on 9/20/15.
//

#ifndef CLUSTERING_POINT_H
#define CLUSTERING_POINT_H

#include <math.h>
#include <MacTypes.h>
//#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include "Exceptions.h"

namespace Clustering
{

    class Point {
        unsigned int dim;        // number of dimensions of the point
        std::vector<double> values; // values of the point's dimensions
        int __id;
        //static int numberOfPoints;

    public:
        //added default constructor to set dim to 0
        Point() = delete;//{values = nullptr; dim = 0;}
        Point(unsigned int);
        //Point(int, double *);//TODO

        // Big three: cpy ctor, overloaded operator=, dtor
        Point(const Point &);
        Point &operator=(const Point &);
        ~Point();

        // Accessors & mutators
        unsigned int getDims() const { return dim; }
        void setValue(int, double);
        void setDims(unsigned int d) {dim = d;}
        double getValue(int) const;
        const unsigned int getId() const {return __id;}
        int generateID() {static int numberOfPoints = 0;return  ++numberOfPoints;}

        // Functions
        double distanceTo(const Point &) const;

        // Overloaded operators

        // Members
        Point &operator*=(double);
        Point &operator/=(double);
        const Point operator*(double) const; // prevent (p1*2) = p2;<-- Do you mean later?
        const Point operator/(double) const;

        //double &operator[](int index) { return values[index - 1]; } // TODO out-of-bds? what?

        // Friends
        friend Point &operator+=(Point &, const Point &);
        friend Point &operator-=(Point &, const Point &);
        friend const Point operator+(const Point &, const Point &);
        friend const Point operator-(const Point &, const Point &);

        friend bool operator==(const Point &, const Point &);
        friend bool operator!=(const Point &, const Point &);

        friend bool operator<(const Point &, const Point &);
        friend bool operator>(const Point &, const Point &);
        friend bool operator<=(const Point &, const Point &);
        friend bool operator>=(const Point &, const Point &);

        friend std::ostream &operator<<(std::ostream &, const Point &);
        friend std::istream &operator>>(std::istream &, Point &);

        //nabbed this during class. not sure what it's useful for yet.
        const double &operator[](const unsigned int index) const;
        
    };

//    #inlclude "Point.cpp"   //keep for templates in Cluster // don't forget to remove Point.cpp from CMakeLists.

}
#endif //CLUSTERING_POINT_H