//
// Created by Nikolai Sharp on 9/20/15.
//


#include "Cluster.h"


namespace Clustering
{
	//keeping at beginning for now. want for testing
	std::ostream &operator<<(std::ostream &os, const Cluster &cluster)
	{
		//create temp LNode pointer to navigate
		LNodePtr ptr;

		//start out at first element
		ptr = cluster.points;

		//announce beginning:
		//os << "\ncluster size:" << cluster.size << "dims:" << cluster.dim;

		//while loop stops at end
		while (ptr != nullptr)
		{
			//print associated point
			os << *ptr->p << " : " << cluster.__id;

			//increment point
			ptr = ptr->next;
		}
		//os << std::endl;
		return os;
	}

	//I want to keep this.
	Cluster::Cluster(unsigned int d): __centroid(d)
	{
		size = 0;
		points = nullptr;
		endPtr = nullptr;
		dim = d;
		__id = generateID();
		__centroidIsValid = false;


	}

	//I realize now after all of this that I could have used the add function.. oh well.
	Cluster::Cluster(const Cluster &cluster): Cluster(cluster.dim)
	{
		//set id and increment number of clusters
		//__id = generateID();

		if (cluster.size > 0)
		{
			try
            {
                //point points to first element and ptptr to next element in cluster
                points = new LNode;
                points->p = cluster.points->p;
                //nextPtr will navigate other cluster. if only one point in list, nextPtr will = nullptr
                nextPtr = cluster.points->next;
                //endPtr will take up the rear behind newPtr
                endPtr = points;
                //set dims
                dim = cluster.dim;
                
                //increment size
                this->size++;
                
                //now create new linked list of LNodes that point to same points as other cluster
                while (nextPtr != nullptr)
                {
                    //create new LNode and point it to next point
                    newPtr = new LNode;
                    newPtr->p = nextPtr->p;
                    //point previous LNode to new
                    endPtr->next = newPtr;
                    //set newPtr->next to point to nullPtr in case nextPtr->next is null as well
                    newPtr->next = nullptr;
                    
                    //increment ptPtr and nextPtr
                    endPtr = endPtr->next;
                    nextPtr = nextPtr->next;
                    
                    //increment size
                    this->size++;
                }
                
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
		else
		{
			size = 0;
			points = nullptr;
			endPtr = nullptr;
			dim = cluster.dim;
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
            LNodePtr nextPtr2;
            
            //call destructor to wipe out cluster
            if (size != 0)
            {
                this->~Cluster();
            }
            
            //set nextPtr to head of right hand cluster
            nextPtr2 = cluster.points;
            
            
            while (nextPtr2 != nullptr)
            {
                add(nextPtr2->p);
                
                nextPtr2 = nextPtr2->next;
            }
            
            
            
            return *this;
        }
        catch (std::string error)
        {
            throw error;
        }
	}

	Cluster::~Cluster()
	{
		//needs to delete all dynamically allocated things
		//set ptPtr to beginning
		if (size > 1)
		{
			ptPtr = points;
			//set nextPtr to ptPtr.next
			nextPtr = ptPtr->next;
			while (ptPtr != nullptr)
			{
				//delete LNode and associated points
				//ptPtr->p->~Point();
				delete ptPtr->p;
				//delete ptPtr;


				//set ptPtr to nextPtr
				ptPtr = nextPtr;

				//Then nextPtr to nextPtr.next, but only if it is not already nullptr
				if (nextPtr != nullptr)
					nextPtr = nextPtr->next;
			}
			//added in case I want to call destructor and re-use cluster.. or for = operator
			size = 0;
			dim = 0;
		}
		else if (size == 1)
		{
			points->p->~Point();
			delete points;
		}
	}
    void Cluster::add(PointPtr const &ptr)
    {
        try
        {
            newPtr = new LNode;
            
            //assign new LNode point pointer to point
            newPtr->p = ptr;
            
            //set newPtr to point to nullpointer (used later in while loop)
            newPtr->next = nullptr;
            
            //invalidate centroid
            __centroidIsValid = false;
            
            //First we check if cluster is empty. if so, we just make the new point the first LNode
            if (size == 0)
            {
                //make new LNode head
                points = newPtr;
                
                //Make new LNode the end as well
                endPtr = newPtr;
                
                //Make new LNode point to nullptr, to 0
                endPtr->next = nullptr;
                
                //set number of dimensions for cluster
                //dim = endPtr->p->getDims();
            }
            
            //Next we check if point will be added at the very beginning and put it there
            else if (*ptr < *points->p)
            {
                //point new LNode to current head
                newPtr->next = points;
                
                //make new LNode new head
                points = newPtr;
            }
            //other beginning case:
            else if (*ptr == *points->p && ptr < points->p)
            {
                //point new LNode to current head
                newPtr->next = points;
                
                //make new LNode new head
                points = newPtr;
            }
            //Now check if point should be added at end
            else if (*ptr > *endPtr->p)
            {
                //point current end to new end
                endPtr->next = newPtr;
                
                //make new LNode the end
                endPtr = newPtr;
                endPtr->next = nullptr;
            }
            //other end case:
            else if (*ptr == *endPtr->p && ptr > endPtr->p)
            {
                //point current end to new end
                endPtr->next = newPtr;
                
                //make new LNode the end
                endPtr = newPtr;
                endPtr->next = nullptr;
            }
            //last case, find where it should go anywhere in between the previous two
            else
            {
                //set ptPtr to head. we will check between each two points
                ptPtr = points;
                
                //set nextPtr to point after that for comparison
                nextPtr = ptPtr->next;
                
                //find where it needs to go. make sure not infinite loop
                while (newPtr->next == nullptr && nextPtr != nullptr)
                {
                    //if it fits squarely: add
                    if (*ptr > *ptPtr->p && *ptr < *nextPtr->p)
                    {
                        //put it there
                        newPtr->next = nextPtr;
                        ptPtr->next = newPtr;
                    }
                    //if == on left and < on right, ensure address is greater on left, add
                    else if (*ptr == *ptPtr->p && *ptr < *nextPtr->p && ptr > ptPtr->p)
                    {
                        //put it there
                        newPtr->next = nextPtr;
                        ptPtr->next = newPtr;
                    }
                    //if > left and <= right, check addresses, add
                    else if (*ptr > *ptPtr->p && *ptr == *nextPtr->p && ptr < nextPtr->p)
                    {
                        //put it there
                        newPtr->next = nextPtr;
                        ptPtr->next = newPtr;
                    }
                    //finally, if >= on left and <= on right, check both addresses, add
                    else if (*ptr == *ptPtr->p && *ptr == *nextPtr->p && ptr > ptPtr->p && ptr < nextPtr->p)
                    {
                        //put it there
                        newPtr->next = nextPtr;
                        ptPtr->next = newPtr;
                    }
                    //if none of these cases apply, point does not fit, shift over
                    else
                    {
                        ptPtr = ptPtr->next;
                        nextPtr = nextPtr->next;
                    }
                }
                //if this got to the end, then the point was not added
                //new note: considering the new kmeans logic, we should never get here.
                if (newPtr->next == nullptr)
                {
                    //decrease size, let user know, and delete the LNode
                    size--;
                    delete newPtr;
                    std::cout << "\n\nERROR: point " << *newPtr->p << " was not added. Check logic and try again.\n\n";
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
//	void Cluster::add(PointPtr const &ptr)
//	{
//        //create new LNode for use in any case
//        newPtr = new LNode;
//        
//        //assign new LNode point pointer to point
//        newPtr->p = ptr;
//        
//        //set newPtr to point to nullpointer (used later in while loop)
//        newPtr->next = nullptr;
//        
//        if (size == 0)
//        {
//            //make new LNode head
//            points = newPtr;
//            
//            //Make new LNode the end as well
//            endPtr = newPtr;
//            
//            //Make new LNode point to nullptr, to 0
//            endPtr->next = nullptr;
//            
//            //set number of dimensions for cluster
//            //dim = endPtr->p->getDims();
//            size++;
//            __centroidIsValid = false;
//        }
//        //Next we check if point will be added at the very beginning and put it there
//        
//        else
//        {
//            //point current end to new end
//            endPtr->next = newPtr;
//            
//            //make new LNode the end
//            endPtr = newPtr;
//            endPtr->next = nullptr;
//            size++;
//            __centroidIsValid = false;
//        }
//    
//        
//    }

	const PointPtr &Cluster::remove(PointPtr const &ptr)
	{
        try
        {
            if (size == 0)
                throw RemoveFromEmptyEx("Cluster::remove()", __id);
            //set ptPtr to beginning
            ptPtr = points;
            
            //set nextPtr to point after ptPtr to check against ptr
            nextPtr = ptPtr->next;
            
            //		__centroidIsValid = false;
            
            // check if ptPtr == ptr address
            //std::cout << ptr;
            if (points->p == ptr)
            {
                //make points = nextPtr and delete ptPtr
                points = nextPtr;
                
                //decrement size here too
                size--;
                
                __centroidIsValid = false;
                
                delete ptPtr;
            }
            
            else
            {
                //loop through and check values
                while (ptPtr->next != nullptr)
                {
                    if (nextPtr->p == ptr)
                    {
                        if (nextPtr == endPtr)
                        {
                            endPtr = ptPtr;
                        }
                        //point ptPtr to nextPtr.next to cut out nextPtr
                        ptPtr->next = nextPtr->next;
                        
                        //decrement size
                        size--;
                        
                        //delete LNode link in nextPtr
                        delete nextPtr;
                        
                        //invalidate centroid
                        __centroidIsValid = false;
                        //break out of while loop
                        break;
                    }
                    else
                    {
                        //increment both
                        ptPtr = ptPtr->next;
                        nextPtr = nextPtr->next;
                    }
                }
            }
            
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
        return ptr;
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
            LNodePtr lhpt;
            LNodePtr rhpt;
            
            lhpt = lhs.points;
            rhpt = rhs.points;
            
            //ends at end of list. can just compare addresses of each LNode.p
            while (lhpt != nullptr)
            {
                if (lhpt->p != rhpt->p)
                    return false;
                
                //increment both
                lhpt = rhpt->next;
                rhpt = rhpt->next;
            }
            
            
            //return true if we get through both tests without failing
            return true;

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


	const Cluster operator+(const Cluster &lhs, const PointPtr &rhs)
	{
		try
        {
            //create cluster to add two together, return new cluster.
            Cluster sum(lhs.dim);
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

	const Cluster operator-(const Cluster &lhs, const PointPtr &rhs)
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
            //I'll use nextPtr2 to navigate other cluster
            LNodePtr nextPtr2;
            nextPtr2 = rhs.points;
            
            while (nextPtr2 != nullptr)
            {
                //add the point. should not add duplicate specific points
                add(nextPtr2->p);
                
                //increment nextPtr
                nextPtr2 = nextPtr2->next;
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
            //I'll use nextPtr to navigate other cluster
            LNodePtr nextPtr2;
            nextPtr2 = rhs.points;
            
            while (nextPtr2 != nullptr)
            {
                //add the point. should not add duplicate specific points
                remove(nextPtr2->p);
                
                //increment nextPtr
                nextPtr2 = nextPtr2->next;
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

	Cluster &Cluster::operator+=(const PointPtr &rhs)
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

	Cluster &Cluster::operator-=(const PointPtr &rhs)
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
                PointPtr tPoint = new Point(dim);
                
                //set ptPtr to beginning
                ptPtr = points;
                while (ptPtr != nullptr)
                {
                    *tPoint = *tPoint + *ptPtr->p;
                    ptPtr = ptPtr->next;
                }
                *tPoint /= size;
                //			std::cout << *tPoint;
                
                //setCentroid(*tPoint);
                __centroid = *tPoint;
                
                __centroidIsValid = true;

        }
			//delete tPoint;
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
            ptPtr = points;
            PointPtr maxPtr = ptPtr->p, minPtr = ptPtr->p;
            double max, min;
            //add up dimensions of point
            for (int i = 0; i < dim; i++)
            {
                max += (*ptPtr->p)[i];
            }
            min = max;
            
            ptPtr = ptPtr->next;
            
            //find the points with the smallest and largest sum of values
            while (ptPtr != nullptr)
            {
                double vSum = 0;
                for (int i = 0; i < dim; i++)
                {
                    vSum += (*ptPtr->p)[i];
                }
                //			std::cout << vSum << std::endl;
                if (vSum > max)
                {
                    max = vSum;
                    maxPtr = ptPtr->p;
                }
                else if (vSum < min)
                {
                    min = vSum;
                    minPtr = ptPtr->p;
                }
                
                ptPtr = ptPtr->next;
            }
            
            //subtract minPtr from maxPtr and divide by k to
            //adding these together will allow a linear set of points to begin testing. Not perfectly evenly disbursed
            //but better than nothing.
            PointPtr tPoint = new Point((*maxPtr - *minPtr)/k);
            for (int i = 0; i < k; i++)
            {
                //offset ptptr by 1/2
                //std::cout << (minPtr - *tPoint/2) + *tPoint*(i+1);
                pointArray.push_back((*minPtr - *tPoint/2) + *tPoint*(i+1));
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

	PointPtr &Cluster::operator[](int i)
	{
		try
        {
            if (i > size - 1 || i < 0)
                throw OutOfBoundsEx("operator[]", "Cluster", __id);
            int index = 0;
            ptPtr = points;
            while (ptPtr != nullptr)
            {
                if (index == i)
                    return ptPtr->p;
                else
                {
                    index++;
                    ptPtr = ptPtr->next;
                }
            }
            
            return points->p;
        }
        catch (OutOfBoundsEx outEx)
        {
            throw outEx;
        }
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
                LNodePtr it = points;
                
                //this iterator keeps track of the starting point of the next while loop
                //starts at points.next because we do not need a points distance to itself(0)
                LNodePtr itT = points->next;
                
                //this iterator keeps track of the second while loop,
                LNodePtr it2;
                
                while (it != nullptr)
                {
                    it2 = itT;
                    while (it2 != nullptr)
                    {
                        d = d + it->p->distanceTo(*it2->p);
                        it2 = it2->next;
                    }
                    //all of the distances associated with 'it' have been added. increment to next
                    it = it->next;
                    //increment itT to one after it, if itT is not already nullptr
                    if (itT != nullptr)
                        itT = itT->next;
                    
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
            LNodePtr it = c1.points;
            //for inner loop(navigates between points in c2
            LNodePtr it2;
            
            //sum of distances
            double d = 0;
            
            while (it != nullptr)
            {
                //set it2 to beginning
                it2 = c2.points;
                while (it2 != nullptr)
                {
                    d += it2->p->distanceTo(*it->p);
                    it2 = it2->next;
                }
                it = it->next;
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
    
    Cluster::Move::Move(const PointPtr &ptr, Cluster &from, Cluster &to)
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
    
    void Cluster::Move::perform(const PointPtr &ptr, Cluster &from, Cluster &to)
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
}