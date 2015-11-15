//
// Created by Nikolai Sharp on 9/20/15.
//

#include <math.h>
#include <MacTypes.h>
//#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "Point.h"
#include "Exceptions.h"


namespace Clustering
{


//Fist constructor takes in a number of dimensions and allocates space on the heap for
//a point with that umber of dimensions
    Point::Point(unsigned int num)
    {
        dim = num;
        values = new double[dim];
        //for loop to set all values to 0
        for (int i = 0; i < dim; i++)
            values[i] = 0;
        __id = generateID();
    }

//Second constructor takes in a number of dimensions as well as a pointer to an existing point
//Point::Point(int num, double * oPoint)
//{
//
//}


//Copy constructor. Uses regular constructor to initilize values.
    Point::Point(const Point &oPoint)
    {
        dim = oPoint.dim;
        values = new double[dim];
        //For loop copies values of oPoint into the values of this point
        for (int i = 0; i < dim; i++) {
            this->values[i] = oPoint.values[i];
        }
    }


    Point &Point::operator=(const Point &rhs)
    {
        //only copies if the number of dimensions are the same and it is not itself
        try
        {
            if (dim != rhs.dim)
            {
                throw DimensionalityMismatchEx("operator=", __id, rhs.__id);//TODO update PID
            }
            //For loop copies values of oPoint into the values of this point
            for (int i = 0; i < rhs.dim; i++) {
                values[i] = rhs.values[i];
            }
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        return *this;
    }


//Destructor to delete any memory allocated on the heap
    Point::~Point() {
        //delete[] values;
    }

//Set function sets value of specific dimension of point
//I like starting with the 0th dimension. It makes more sense to me.
    void Point::setValue(int sDim, double val) {

        try
        {
            if (sDim < 0 && sDim >= dim)
                throw OutOfBoundsEx("setValue","Point",__id);
            values[sDim] = val;
        }
        catch (OutOfBoundsEx outEx)
        {
            throw outEx;
        }



        //check to make sure is a valid dimension
//        if (sDim >= 0 && sDim < dim)
//            values[sDim] = val;
//        else
//            std::cout << "\nthat is not possible2";
    }

//Returns value at specific dimension of point
    double Point::getValue(int sDim) const {
        try
        {
            if (sDim < 0 || sDim >= dim)
                throw OutOfBoundsEx("getValue()","Point",__id);
            return values[sDim];
        }
        catch(OutOfBoundsEx outEx)
        {
            throw outEx;
        }


        //check if is valid dimension
        if (sDim >= 0 && sDim < dim)
            return *(values + sDim);
        else
            std::cout << "\nthat is not possible3";
        return 0;
    }

//Calculates distance between two like points
    double Point::distanceTo(const Point &oPoint) const {
        try
        {
            if (dim != oPoint.dim)
                throw DimensionalityMismatchEx("distanceTo", __id, oPoint.__id);
            //dynamic memory to hold distances between specific dimensions
            double *distance = new double[oPoint.dim];
            //initialize sum to zero to avoid other possible numbers sneaking their way in
            double sum = 0;

            //for loop to calculate distances between points and square that value, and increase sum
            for (int i = 0; i < dim; i++) {
                distance[i] = oPoint.values[i] - values[i];//
                distance[i] = distance[i] * distance[i]; //I could have had those two on the same ling, but it would be long
                sum = sum + distance[i];
            }

            //Gotta deallocate that memory;
            delete distance;
            return sqrt(sum);
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        
        return 0;
    }


//Multiplies each dimension by rhs
    Point &Point::operator*=(double rhs) {

        
        for (int i = 0; i < dim; i++) {
            values[i] = values[i] * rhs;
        }
        
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
        Point p(dim);
        //I can't think of an easy way to initialize p to the values of this.
        //So I just re wrote the code
        for (int i = 0; i < p.dim; i++) {
            p.values[i] = values[i];
        }

        //reuses *= code
        p *= rhs;

        return p;
    }

// returns a box with the values divided by rhs
    const Point Point::operator/(double rhs) const
    {
        Point p(dim);
        //I can't think of an easy way to initialize p to the values of this.
        //So I just re wrote the code
        for (int i = 0; i < p.dim; i++) {
            p.values[i] = values[i];
        }

        //reuses /= code
        p /= rhs;

        return p;
    }

    //Overloads the += operator to add one point to another.
    Point &operator+=(Point &point, const Point &point1)
    {
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator+=", point.__id, point1.__id);
            
            for (int i = 0; i < point.dim; i++)
            {
                point.values[i] += point1.values[i];
            }
            
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
            return point;
    }

    //overloads -= operator to subtract one point from another
    Point &operator-=(Point &point, const Point &point1)
    {
        try
        {
            
            if (point.dim != point1.dim)
            {
                throw DimensionalityMismatchEx("operator-=",point.__id, point1.__id);
            }
            for (int i = 0; i < point.dim; i++)
            {
                point.values[i] -= point1.values[i];
            }
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        return point;
    }

    //return temp point that represents one point added to another
    const Point operator+(const Point &point, const Point &point1)
    {
        //create a temporary point to return
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator+", point.__id, point1.__id);
            Point p(point.dim);

            //set values of p adding the other two together
            for (int i = 0; i < point.dim; i++)
            {
                p.values[i] = point.values[i] + point1.values[i];
            }
            return p;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        
        return 0;

    }

    //return temp point that represents one point subtracted from another
    const Point operator-(const Point &point, const Point &point1)
    {
        //create a temporary point to return
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator-", point.__id, point1.__id);
            Point p(point.dim);

            //set values of p adding the other two together
            for (int i = 0; i < point.dim; i++)
            {
                p.values[i] = point.values[i] - point1.values[i];
            }
            return p;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        
            return 0;
    }

    //overload == operator
    bool operator==(const Point &point, const Point &point1)
    {
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator==", point.__id, point1.__id);
            //for loop returns false if any elements are different
            for (int i = 0; i < point.dim; i++)
            {
                if (point.values[i] != point1.values[i])
                    return false;
            }
            //once all elements have been tested, return true
            return true;
        }

            //return false if different number of dims
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        return false;
    }

    //overload != operator. reuse ==..my favorite.
    bool operator!=(const Point &point, const Point &point1)
    {
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator!=", point.__id, point.__id);
            return !(point == point1);
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
    }

    //overload < operator
    bool operator<(const Point &point, const Point &point1)
    {
        //makes sure points are comparable, and that they are not already equal. reuses !=
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator<", point.__id, point1.__id);
            //tests dimensions in lexicographic order.
            for (int i = 0; i < point.dim; i++)
            {
                //returns true at first sign of true
                if (point.values[i] < point1.values[i])
                    return true;

                    //returns false at first sign of false
                else if (point.values[i] > point1.values[i])
                    return false;
                //continues on to next dimension otherwise
            }
            //should not get past here logically
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }

            //if points do not have the same number of dimensions or are equal, return false
        
        return false;
    }

    //overlaod > operator. if not less than and not equal to, should be greater
    bool operator>(const Point &point, const Point &point1)
    {
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator>", point.__id, point1.__id);
            return (point != point1 && !(point < point1));
        } catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        
    }

    // overload <= operator. use previous functions
    bool operator<=(const Point &point, const Point &point1)
    {
        //return  less than    or     equal to
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator>=", point.__id, point1.__id);
            return (point < point1 || point == point1);
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
    }

    //overlaod >= operator. use previous functions
    bool operator>=(const Point &point, const Point &point1)
    {
        try
        {
            if (point.dim != point1.dim)
                throw DimensionalityMismatchEx("operator>=", point.__id, point1.__id);
            //return greater than  or    equal to
            return (point > point1 || point == point1);
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
    }

    std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        os << std::endl;
        //for loop adds each dimension followed by a comma, except for last
        for (int i = 0; i < point.dim - 1; i++) {
            os << point.getValue(i) << ", ";
        }
        //adds the final value
        os << point.getValue(point.dim - 1);

        // used for organize testing purposes
        //os << &point;

        //discovered that I needed to return to os variable.
        //can not output multiple things from same cout otherwise.
        return os;
    }

    std::istream &operator>>(std::istream &is, Point &point)
    {
        
        try
        {
            std::string line;
            getline(is, line);
            std::string line2 = line;
            std::stringstream lineStream(line);
            std::stringstream lineStream2(line2);
            std::string value;
            double d;
            int dimCheck = 0;
            while (getline(lineStream2, value, ','))
            {
                dimCheck++;
            }
            if (point.dim != dimCheck)
                throw DimensionalityMismatchEx("operator>>", point.__id, 0);
            
            int i = 0;
            while (getline(lineStream, value, ','))
            {
                d = stod(value);
                
                //std::cout << "Value: " << d << std::endl;
                
                point.setValue(i++,d);
            }
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            throw dimMM;
        }
        return is;
    }
    
    double Point::operator[](int index)
    {
        try
        {
            if (index < 0 || index > dim - 1)
                throw OutOfBoundsEx("operator[]", "Point", __id);
            return values[index];
        }
        catch (OutOfBoundsEx outEx)
        {
            //std::cerr << outEx;
            throw outEx;//TODO throw to what??
        }
    }
    
}

//