//
// Created by Nikolai Sharp on 9/20/15.
//

#include <math.h>
//#include <cassert>
#include "Point.h"

namespace Clustering {


//Fist constructor takes in a number of dimensions and allocates space on the heap for
//a point with that umber of dimensions
    Point::Point(int num) {
        dim = num;
        values = new double[dim];
    }

//Second constructor takes in a number of dimensions as well as a pointer to an existing point   TODO ?
//Point::Point(int num, double * oPoint)
//{
//
//}


//Copy constructor. Uses regular constructor to initilize values.
    Point::Point(const Point &oPoint): Point(oPoint.dim)
    {
        //For loop copies values of oPoint into the values of this point
        for (int i = 0; i < oPoint.dim; i++) {
            *(values + i) = oPoint.values[i];
        }
    }


    Point &Point::operator=(const Point &rhs)
    {
        //only copies if the number of dimensions are the same and it is not itself
        if ((dim == rhs.dim))  //TODO make sure not self
        {
            //For loop copies values of oPoint into the values of this point
            for (int i = 0; i < rhs.dim; i++) {
                *(values + i) = rhs.values[i];
            }
            return *this;
        }

        else
        {
            std::cout << "\nthat is not possible";
            return *this;
        }



    }


//Destructor to delete any memory allocated on the heap
    Point::~Point() {
        delete[] values;
    }

//Set function sets value of specific dimension of point
//I like starting with the 0th dimension. It makes more sense to me.
    void Point::setValue(int sDim, double val) {
        //check to make sure is a valid dimension
        if (sDim >= 0 && sDim < this->getDims())
            *(values + sDim) = val;
        else
            std::cout << "\nthat is not possible";
    }

//Returns value at specific dimension of point
    double Point::getValue(int sDim) const {
        //check if is valid dimension
        if (sDim >= 0 && sDim < this->getDims())
            return *(values + sDim);
        else
            std::cout << "\nthat is not possible";
    }

//Calculates distance between two like points
    double Point::distanceTo(const Point &oPoint) const {
        if (oPoint.dim == dim) {
            //dynamic memory to hold distances between specific dimensions
            double *distance = new double[oPoint.dim];
            //initialize sum to zero to avoid other possible numbers sneaking their way in
            double sum = 0;

            //for loop to calculate distances between points and square that value, and increase sum
            for (int i = 0; i < dim; i++) {
                distance[i] = oPoint.values[i] - values[i];//
                distance[i] = pow(distance[i], 2); //I could have had those two on the same ling, but it would be long
                sum = sum + distance[i];
            }

            //Gotta deallocate that memory;
            delete[] distance;
            return sqrt(sum);
        }

        else        //TODO ?
        {
            std::cout << "\nthat is not possible";
        }
    }


//Multiplies each dimension by rhs
    Point &Point::operator*=(double rhs) {

        //Point *p = new Point(dim);
        for (int i = 0; i < dim; i++) {
            values[i] = values[i] * rhs;
        }
        //Return address of this to satisfy function requirements
        return *this;
    }


//Divides each dimension by rhs
    Point &Point::operator/=(double rhs) {

        for (int i = 0; i < dim; i++) {
            values[i] = values[i] / rhs;
        }
        //Return address of this to satisfy function requirements
        return *this;
    }

// returns a temporary box with values multiplied by rhs
    const Point Point::operator*(double rhs) const {
        Point p(this->dim);
        //I can't think of an easy way to initialize p to the values of this.
        //So I just re wrote the code
        for (int i = 0; i < p.dim; i++) {
            p.values[i] = this->values[i];
        }

        //reuses *= code
        p *= rhs;

        return p;
    }

// returns a box with the values divided by rhs
    const Point Point::operator/(double rhs) const
    {
        Point p(this->dim);
        //I can't think of an easy way to initialize p to the values of this.
        //So I just re wrote the code
        for (int i = 0; i < p.dim; i++) {
            p.values[i] = this->values[i];
        }

        //reuses /= code
        p /= rhs;

        return p;
    }

    //Overloads the += operator to add one point to another.
    Point &Clustering::operator+=(Point &point, const Point &point1)
    {
        if (point.dim == point1.dim)
            for (int i = 0; i < point.dim; i++)
            {
                point.values[i] += point1.values[i];
            }
        else
            std::cout << "\nthat is not possible";
        return point;
    }

    //overloads -= operator to subtract one point from another
    Point &Clustering::operator-=(Point &point, const Point &point1)
    {
        if (point.dim == point1.dim)
            for (int i = 0; i < point.dim; i++)
            {
                point.values[i] -= point1.values[i];
            }
        else
            std::cout << "\nthat is not possible";
        return point;
    }

    //
    const Point Clustering::operator+(const Point &point, const Point &point1)
    {
        //create a temporary point to return
        if (point.getDims() == point1.getDims())
        {
            Point p(point.getDims());

            for (int i = 0; i < point.getDims(); i++)
            {
                p.va
            }

            return p;
        }


    }



    std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        //opens parentheses for viewing
        os << "\n(";
        //for loop adds each dimension followed by a comma, except for last
        for (int i = 0; i < point.getDims() - 1; i++) {
            os << point.getValue(i) << ",";
        }
        //adds the final value followed by a close parentheses
        os << point.getValue(point.getDims() - 1) << ")";
    }
}



