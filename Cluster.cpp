//
// Created by Nikolai Sharp on 9/20/15.
//


#include "Cluster.h"


namespace Clustering
{
    void Cluster::add(Point const &ptr)
    {
        try
        {
            //invalidate centroid
            __centroidIsValid = false;
            pItr = points.begin();
            //First we check if cluster is empty. if so, we just make the new point the first LNode
            if (size == 0)
            {
                points.push_front(ptr);
                endItr = points.begin();
                //std::cout << "end:" << *endItr << "begin:" << (*points.begin()) << std::endl;
            }
            
            //Next we check if point will be added at the very beginning and put it there
            else if (ptr < (*pItr))
            {
                points.push_front(ptr);
                //std::cout << "end:" << *endItr << "begin:" << (*points.begin()) << std::endl;
            }
            //other beginning case:
            else if (ptr == (*pItr) && ptr.getId() < (*pItr).getId())
            {
                points.push_front(ptr);
            }
            //Now check if point should be added at end
            else if (ptr > *endItr)
            {
                points.insert_after(endItr, ptr);
                endItr++;
            }
            //other end case:
            else if (ptr == (*endItr) && ptr.getId() > (*endItr).getId())
            {
               points.insert_after(endItr, ptr);
                endItr++;
            }
            //last case, find where it should go anywhere in between the previous two
            else
            {
                nItr = pItr;
                nItr++;
                //find where it needs to go. make sure not infinite loop
                while (nItr != points.end())
                {
                    //if it fits squarely: add
                    if (ptr > (*pItr) && ptr < (*nItr))
                    {
                        points.insert_after(pItr, ptr);
                        break;
                    }
                    //if == on left and < on right, ensure id is greater on left, add
                    else if (ptr == *pItr && ptr < *nItr && ptr.getId() > (*pItr).getId())
                    {
                        points.insert_after(pItr, ptr);
                        break;
                    }
                    //if > left and <= right, check addresses, add
                    else if (ptr > *pItr && ptr == *nItr && ptr.getId() < (*nItr).getId())
                    {
                        points.insert_after(pItr, ptr);
                        break;
                    }
                    //finally, if >= on left and <= on right, check both addresses, add
                    else if (ptr == *pItr && ptr == *nItr && ptr.getId() > (*pItr).getId() && ptr.getId() < (*nItr).getId())
                    {
                        points.insert_after(pItr, ptr);
                        break;
                    }
                    //if none of these cases apply, point does not fit, shift over
                    else
                    {
                        pItr++;
                        nItr++;
                    }
                }
                //if this got to the end, then the point was not added
                //new note: considering the new kmeans logic, we should never get here.
                if (pItr == endItr)
                {
                    //decrease size, let user know, and delete the LNode
                    size--;
                    std::cerr << "\n\nERROR: point " << ptr.getId() << " was not added. Check logic and try again.\n\n";
                    //this should never happen, but if the point was not added, centroid is still valid
                    //__centroidIsValid = true;
                    
                }
            }
            
            //increment size, point was added
            size++;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::add();" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::add();" << dimMM;
            //throw dimMM;
        }
    }
    
    const Point &Cluster::remove(Point const &ptr)
    {
        try
        {
            if (size == 0)
                throw RemoveFromEmptyEx("Cluster::remove()", __id);
            bool wasRemoved =  false;
            //set pItr to beginning
            pItr = points.begin();
            //std::cout << *pItr << ptr;
            nItr = pItr;
            //set nItr to point after pItr to check against ptr
            nItr++;
            //		__centroidIsValid = false;
            
            // check if pItr == ptr address
            //std::cout << ptr;
            //std::cout << "\npoint:" << ptr;
            if (ptr.getId() == (*pItr).getId() && ptr == *pItr)
            {
                points.pop_front();
                size--;
                wasRemoved =  true;
                __centroidIsValid = false;
                
                //delete pItr;
            }
            else
            {
                //loop through and check values
                while (nItr != points.end())
                {
                    if (*nItr == ptr && (*nItr).getId() == ptr.getId())
                    {
                        if (nItr == endItr)
                            endItr = pItr;
                        points.erase_after(pItr);
                        
                        wasRemoved =  true;
                        
                        //decrement size
                        size--;
                        
                        //invalidate centroid
                        __centroidIsValid = false;
                        //break out of while loop
                        break;
                    }
                    else
                    {
                        //increment both
                        pItr++;
                        
                        nItr++;
                    }
                }
            }
            if (!wasRemoved)
                throw (ptr);
            //returns ptr so you can use in add statement
            return ptr;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::remove():" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::remove():" << dimMM;
            //throw dimMM;
        }
        catch (RemoveFromEmptyEx emptyEx)
        {
            throw emptyEx;
        }
        catch (Point ptr1)
        {
            std::cerr << "Error: Point " << ptr1.getId() << " was not removed from Cluster " << __id;
        }
        return ptr;
    }
    
	//keeping at beginning for now. want for testing
	std::ostream &operator<<(std::ostream &os, const Cluster &cluster)
	{
		//create temp LNode pointer to navigate
		std::forward_list<Point>::const_iterator ptr = cluster.points.begin();

		//announce beginning:
		//os << "\ncluster size:" << cluster.size << "dims:" << cluster.dim;

		//while loop stops at end
        for (int i = 0; i < cluster.getSize(); i++)
		{
			//print associated point
			os << *ptr << " : " << cluster.__id;

			//increment point
            ptr++;
		}
		//os << std::endl;
		return os;
	}

	//I want to keep this.
	Cluster::Cluster(unsigned int d): __centroid(d)
	{
		size = 0;
		dim = d;
		__id = generateID();
		__centroidIsValid = false;


	}

	//I realize now after all of this that I could have used the add function.. oh well.
	Cluster::Cluster(const Cluster &cluster): Cluster(cluster.dim)
	{
		//set id and increment number of clusters
		//__id = generateID();

        try
        {
            points = cluster.points;
            dim = cluster.dim;
            size = cluster.size;
            __centroid = cluster.__centroid;
            __centroidIsValid = cluster.__centroidIsValid;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster(Cluster):" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster(Cluster):" << dimMM;
            //throw dimMM;
        }
	}

	Cluster &Cluster::operator=(const Cluster &cluster) //todo
	{
		try
        {
            if (&cluster != this)
            {
                std::string error = "Cluster operator= SelfAssignmentEx ID1:" + std::to_string(__id);
                error = error + " ID2:" + std::to_string(cluster.__id);
                throw error;
            }
            points = cluster.points;
            dim = cluster.dim;
            size = cluster.size;
            __centroid = cluster.__centroid;
            __centroidIsValid = cluster.__centroidIsValid;
            
            
            return *this;
        }
        catch (std::string error)
        {
            throw error;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::&operator=:" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::&operator=::" << dimMM;
            //throw dimMM;
        }
        return *this;
	}

	Cluster::~Cluster()
	{
		//shouldn't have any more dunamically allocated memory to free.
        clear();
		
	}
    
	bool operator==(const Cluster &lhs, const Cluster &rhs)
	{
		try
        {
            // first, check sizes.. pretty good indicator of not equal. Plus I need them to be equal for other test
            if (lhs.size != rhs.size)
                return false;
            //should only need to test element by element. should be organized by address and value
            //need two point pointers
            
            std::forward_list<Point>::const_iterator rhpt = rhs.points.begin();
            std::forward_list<Point>::const_iterator lhpt = lhs.points.begin();
            
            
            //ends at end of list. can just compare addresses of each LNode.p
            while (lhpt != lhs.points.end() && rhpt != rhs.points.end())
            {
                if ((*lhpt).getId() != (*rhpt).getId())
                    return false;
                
                //increment both
                lhpt++;
                rhpt++;
            }
            if (lhpt == lhs.points.end() && rhpt == rhs.points.end())
                return true;
            else
                return false;

        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::operator==:" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::operator==:" << dimMM;
            throw dimMM;
        }
        //if we get here, an exception was thrown and function is false
        return false;
    }

	const Cluster operator+(const Cluster &lhs, const Cluster &rhs)
	{
		try
        {
            //create cluster.
            Cluster sum(lhs.dim);
            
            //add lhs and rhs to sum
            sum += rhs;
            sum += lhs;
            
            //return sum
            return sum;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator+(cluster)\n";
            //throw dimMM;
        }
		catch (OutOfBoundsEx outEx)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator+\n(cluster)";
            //throw outEx;
        }
        return lhs;
	}

	//extremely similar to + not doing dynamically..
	const Cluster operator-(const Cluster &lhs, const Cluster &rhs)
	{
		//assert (lhs.dim == rhs.dim);
		//create cluster with lhs, subtract rhs from new cluster, return new cluster.
        try
        {
            Cluster diff(lhs);
            diff -= rhs;
            return diff;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator-(cluster)\n";
            //throw dimMM;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator-(cluster)\n";
            //throw outEx;
        }
        return lhs; //TODO, not sure if right
	}


	const Cluster operator+(const Cluster &lhs, const Point &rhs)
	{
		try
        {
            //create cluster to add two together, return new cluster.
            Cluster sum(lhs.getDimms());
            sum += lhs;
            sum += rhs;
            return sum;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator+(point)\n";
            //throw dimMM;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator+(point)\n";
            //throw outEx;
        }
        return lhs; //not sure if right TODO
	}

	const Cluster operator-(const Cluster &lhs, const Point &rhs)
	{
		try
        {
            //create cluster with lhs, -= the point, and return new cluster.
            Cluster diff(lhs);
            diff -= rhs;
            return diff;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator-(point)\n";
            //throw dimMM;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator-(point)\n";
            //throw outEx;
        }
        return lhs;
	}

	//had to redo these.. had strange issues
	Cluster &Cluster::operator+=(const Cluster &rhs)
	{
		try
        {
            if (rhs.getSize() != 0)
            {
                std::forward_list<Point>::const_iterator rhpt = rhs.points.begin();
                while (rhpt != rhs.points.end())
                {
                    //add the point. should not add duplicate specific points
                    add(*rhpt);
                    
                    //increment nItr
                    rhpt++;
                }
            }
            return *this;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::operator+=(cluster);" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::operator+=(cluster);" << dimMM;
            //throw dimMM;
        }
        return *this;
	}

	Cluster &Cluster::operator-=(const Cluster &rhs)
	{
		try
        {
            if(rhs.getSize() != 0)
            {
                std::forward_list<Point>::const_iterator rhpt = rhs.points.begin();
                
                while (rhpt != rhs.points.end())
                {
                    //add the point. should not add duplicate specific points
                    remove(*rhpt);
                    
                    rhpt++;
                }
            }
            return *this;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::operator-=(cluster);" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::operator-=(cluster);" << dimMM;
            //throw dimMM;
        }
        return *this;//todo not sure
	}

	Cluster &Cluster::operator+=(const Point &rhs)
	{
		try
        {
            add(rhs);
            return *this;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator+=(point)\n";
           // throw dimMM;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator+=(point)\n";
            //throw outEx;
        }
        return *this;
	}

	Cluster &Cluster::operator-=(const Point &rhs)
	{
		try
        {
            remove(rhs);
            return *this;
        }
        
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator-=(point)\n";
            //throw dimMM;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "correctionFromPrevious:Cluster::operator-=(point)\n";
            //throw outEx;
        }
        return *this;
	}


	void Cluster::setCentroid(Point &point)
	{
        try
        {
            __centroid = point;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::setCentroid();" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::setCentroid();" << dimMM;
            //throw dimMM;
        }
		//__centroidIsValid = true;
	}


	void Cluster::computeCentroid()
	{
		try
        {
            if (size == 0)
                throw RemoveFromEmptyEx("Cluster::computeCentroid", __id);
            if (size > 0)
            {
                Point tPoint(dim);
                nItr = points.begin();
                //set pItr to beginning
                pItr = points.begin();
                while (pItr != points.end())
                {
                    tPoint = tPoint + *pItr;
                    pItr++;
                }
                
                tPoint /= size;
                //			std::cout << *tPoint;
                
                //setCentroid(*tPoint);
                __centroid = tPoint;
                
                __centroidIsValid = true;

            }
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::computeCentroid();" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::computeCentroid();" << dimMM;
            //throw dimMM;
        }
        catch (RemoveFromEmptyEx emptyEx)
        {
            throw emptyEx;
        }

	}

    void Cluster::pickPoints(int k, std::vector<Point> &pointArray)
	{
		try
        {
            pItr = points.begin();
            Point maxPtr = *pItr, minPtr = *pItr;
            double max, min;
            //add up dimensions of point
            for (int i = 0; i < dim; i++)
            {
                max += (*pItr)[i];
            }
            min = max;
            
            pItr++;
            
            //find the points with the smallest and largest sum of values
            double vSum = 0;
            while (pItr != points.end())
            {
                for (int i = 0; i < dim; i++)
                {
                    vSum += (*pItr)[i];
                }
                //			std::cout << vSum << std::endl;
                if (vSum > max)
                {
                    max = vSum;
                    maxPtr = *pItr;
                }
                else if (vSum < min)
                {
                    min = vSum;
                    minPtr = *pItr;
                }
                
                pItr++;
            }
            
            //subtract minPtr from maxPtr and divide by k to
            //adding these together will allow a linear set of points to begin testing. Not perfectly evenly disbursed
            //but better than nothing.
            Point tPoint((maxPtr - minPtr)/k);
            for (int i = 0; i < k; i++)
            {
                //offset pItr by 1/2
                //std::cout << (minPtr - *tPoint/2) + *tPoint*(i+1);
                pointArray.push_back((minPtr - tPoint/2) + tPoint*(i+1));
                pointArray.at(i).setDims(dim);
                //std::cout << pointArray.at(i);
            }
            
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::pickPoints();" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::pickPoints();" << dimMM;
            //throw dimMM;
        }
        //std::cout << "!!SIZE!!::" << pointArray.size() << std::endl;
	}

	Point &Cluster::operator[](int i)
	{
		try
        {
            if (i >= size || i < 0)
                throw OutOfBoundsEx("operator[]", "Cluster", __id);
            int index = 0;
            //std::cout << (*points.begin());
            pItr = points.begin();
            while (pItr != points.end())
            {
                if (index == i)
                {
//                    if (i == 0)
                        //std::cout << *pItr;
                    return *pItr;
                }
                else
                {
                    index++;
                    pItr++;
                }
            }
        }
        catch (OutOfBoundsEx outEx)
        {
            throw outEx;
        }
        return (*points.begin());
	}

	//rather than use the sum of all of the distanes between points and dividing that by 2, which would
	//iterate n^2 times, this function increments the point we are working with and doesn't add distances twice
	//coming to a function runs (n^2+n)/2 times. Should be useful in larger clusters, and run almost half the
	//number of times
	double Cluster::intraClusterDistance() const
	{
		try
        {
            if (size > 1)
            {
                double d = 0;
                
                //iterator keeps track of the first while loop.
                std::forward_list<Point>::const_iterator it = points.begin();
                
                //this iterator keeps track of the starting point of the next while loop
                //starts at points.next because we do not need a points distance to itself(0)
                std::forward_list<Point>::const_iterator itT = points.begin();
                itT++;
                
                //this iterator keeps track of the second while loop,
                std::forward_list<Point>::const_iterator it2;
                
                while (it != points.end())
                {
                    it2 = itT;
                    while (it2 != points.end())
                    {
                        d = d + (*it).distanceTo(*it2);
                        it2++;
                    }
                    //all of the distances associated with 'it' have been added. increment to next
                    it++;
                    //increment itT to one after it, if itT is not already nullptr
                    if (itT != points.end())
                        itT++;
                    
                }
                //return the average, which is the sum of all of the distances divided by the intra cluster edges
                //double edges = size * (size-1)/2;
                return d;
            }
            else
                return 0;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::intraClusterDistance();" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::intraClusterDistance();" << dimMM;
            //throw dimMM;
        }
        return 0;//todo
	}

	double interClusterDistance(const Cluster &c1, const Cluster &c2)
	{
		try
        {
            //for first loop (navigates between points in c1)
            std::forward_list<Point>::const_iterator it = c1.points.begin();
            //for inner loop(navigates between points in c2
            std::forward_list<Point>::const_iterator it2;
            
            //sum of distances
            double d = 0;
            
            while (it != c1.points.end())
            {
                //set it2 to beginning
                it2 = c2.points.begin();
                while (it2 != c2.points.end())
                {
                    d += (*it2).distanceTo(*it);
                    it2++;
                }
                it++;
            }
            //divide by the number of connections made and return
            return d;
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "Cluster::interClusterDistance();" << outEx;
            //throw outEx;
        }
        catch (DimensionalityMismatchEx dimMM)
        {
            std::cerr << "Cluster::interClusterDistance();" << dimMM;
            //throw dimMM;
        }
        return 0;//todo
	}
    
    Cluster::Move::Move(const Point &ptr, Cluster &from, Cluster &to)
    {
        try
        {
            perform(ptr, from, to);
        }
        catch (RemoveFromEmptyEx emptyEx)
        {
            throw emptyEx;
        }
    }
    
    void Cluster::Move::perform(const Point &ptr, Cluster &from, Cluster &to)
    {
        try
        {
            to.add(from.remove(ptr));
        }
        catch (RemoveFromEmptyEx emptyEx)
        {
            throw emptyEx;
        }
    }
    void Cluster::clear()
    {
        points.clear();
        size = 0;
        __centroidIsValid = false;
    }
}