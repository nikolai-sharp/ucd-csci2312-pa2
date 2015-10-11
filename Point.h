//
// Created by Nikolai Sharp on 9/20/15.
//

#ifndef CLUSTERING_POINT_H
#define CLUSTERING_POINT_H

#include <iostream>

namespace Clustering {

    class Point {
        int dim;        // number of dimensions of the point
        double *values; // values of the point's dimensions

    public:
        //added default constructor to set dim to 0
        Point(): dim(0){};
        Point(int);
        //Point(int, double *);//TODO

        // Big three: cpy ctor, overloaded operator=, dtor
        Point(const Point &);
        Point &operator=(const Point &);
        ~Point();

        // Accessors & mutators
        int getDims() const { return dim; }
        void setValue(int, double);
        double getValue(int) const;

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

        //I don't really see how I might use this. It is not specifically in the instructions.
        //friend std::istream &operator>>(std::istream &, Point &);
    };

//    #inlclude "Point.cpp"   //keep for templates in Cluster // don't forget to remove Point.cpp from CMakeLists.

}
#endif //CLUSTERING_POINT_H