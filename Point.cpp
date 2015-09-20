//
// Created by Nikolai Sharp on 9/20/15.
//

#include <math.h>
#include "Point.h"

using namespace Clustering;

//Fist constructor takes in a number of dimensions and allocates space on the heap for
//a point with that umber of dimensions
Point::Point(int num)
{
    dim = num;
    values = new double[num];
}

//Second constructor takes in a number of dimensions as well as a pointer to an existing point   TODO ?
//Point::Point(int num, double * oPoint)
//{
//
//}


//Copy constructor
Point::Point(const Point & oPoint)
{
    //Reuses constructor to allocate memory
    Point(oPoint.dim);
    //For loop copies values of oPoint into the values of this point
    for (int i = 0; i < oPoint.dim; i++)
    {
        *(values + i) = oPoint.values[i];
    }
}

//= operator to copy values and not pointer
Point::Point &operator=(const Point & oPoint)  // TODO Fix
{
    //only copies if the number of dimensions are the same
    if (dim == oPoint.dim)
    {
        //For loop copies values of oPoint into the values of this point
        for (int i = 0; i < oPoint.dim; i++)
        {
            *(values + i) = oPoint.values[i];
        }
    }

    else //TODO ?
    {

    }
}

//Destructor to delete any memory allocated on the heap
Point::~Point()
{
    delete[] values;
}

//Set function sets value of specific dimension of point
Point::setValue(int sDim, double val)
{
    *(values + sDim) = val;
}

//Returns value at specific dimension of point
Point::getValue(int sDim) const
{
    return *(values + sDim);
}

//Calculates distance between two like points
Point::distanceTo(const Point & oPoint) const
{
    if (oPoint.dim == dim)
    {
        //dynamic memory to hold distances between specific dimensions
        double * distance = new double[oPoint.dim];
        //initialize sum to zero to avoid other possible numbers sneaking their way in
        double sum = 0;

        //for loop to calculate distances between points and square that value, and increase sum
        for (int i = 0; i < dim; i++)
        {
            distance[i] = oPoint.values[i] - values[i];//
            distance[i] = pow(distance[i],2); //I could have had those two on the same ling, but it would be long
            sum = sum + distance[i];
        }

        //Gotta deallocate that memory;
        delete[] distance;
        return sqrt(sum);       //TODO I have no idea why it thinks its returning a long
    }

    else        //TODO ?
    {

    }
}

Point::Point &operator*=(double)