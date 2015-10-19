//
// Created by Nikolai Sharp on 9/20/15.
//


#include "Cluster.h"



namespace Clustering
{
	//keeping at beginning for now. want for testing
	std::ostream &Clustering::operator<<(std::ostream &os, const Cluster &cluster)
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

	Cluster::Cluster()
	{
		size = 0;
		points = nullptr;
		endPtr = nullptr;
		dim = 0;
		__id = generateID();
	}

	//I realize now after all of this that I could have used the add function.. oh well.
	Cluster::Cluster(const Cluster &cluster)
	{
		//set id and increment number of clusters
		__id = generateID();
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

	Cluster &Cluster::operator=(const Cluster &cluster) //todo
	{
		//make sure not self and that this is empty
		assert (&cluster != this);
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
				delete ptPtr->p;
				delete ptPtr;


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
			delete points;
		}
	}

	void Cluster::add(PointPtr const &ptr)
	{
		//assert that point is of correct dimension, or cluster is empty
		//assert (ptr->getDims() == dim || size == 0);

		//create new LNode for use in any case
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
			dim = endPtr->p->getDims();
		}

			//Next we check if point will be added at the very beginning and put it there
		else if (*ptr < *points->p)
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
				//Check if it fits between the two. if it does, put it there
				if (*ptr >= *ptPtr->p && *ptr <= *nextPtr->p)
				{
					//now we want to order similar points by address, and exclude multiple equal addresses
					// case 1 breaks if either address is equal to ptr
					if (ptr == nextPtr->p || ptr == ptPtr->p)
					{
						break;
					}
						//case 2 if
					else if ((*ptr == *nextPtr->p && ptr < nextPtr->p) || *ptr < *nextPtr->p)
					{
						//put it there
						newPtr->next = nextPtr;
						ptPtr->next = newPtr;
					}
					else
					{
						ptPtr = ptPtr->next;
						nextPtr = nextPtr->next;
					}
				}

					//if not, shift two over
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
				std::cout << "\n\nERROR: point was not added. Check logic and try again.\n\n";
				//this should never happen, but if the point was not added, centroid is still valid
				__centroidIsValid = true;

			}
		}

		//increment size, point was added
		size++;
	}

	const PointPtr &Cluster::remove(PointPtr const &ptr)
	{
		//set ptPtr to beginning
		ptPtr = points;

		//set nextPtr to point after ptPtr to check against ptr
		nextPtr = ptPtr->next;

		// check if ptPtr == ptr address
		if (ptPtr->p == ptr)
		{
			//make points = nextPtr and delete ptPtr
			points = nextPtr;

			//decrement size here too
			size--;

			//invalidate centroid
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

	bool Clustering::operator==(const Cluster &lhs, const Cluster &rhs)
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

	const Cluster Clustering::operator+(const Cluster &lhs, const Cluster &rhs)
	{
		//assert (lhs.dim == rhs.dim);
		//create cluster.
		Cluster sum;

		//add lhs and rhs to sum
		sum += rhs;
		sum += lhs;

		//return sum
		return sum;
	}

	//extremely similar to + not doing dynamically..
	const Cluster Clustering::operator-(const Cluster &lhs, const Cluster &rhs)
	{
		//assert (lhs.dim == rhs.dim);
		//create cluster with lhs, subtract rhs from new cluster, return new cluster.
		Cluster diff(lhs);
		diff -= rhs;
		return diff;
	}


	const Cluster Clustering::operator+(const Cluster &lhs, const PointPtr &rhs)
	{
		//assert (lhs.dim == rhs->getDims());
		//create cluster to add two together, return new cluster.
		Cluster sum;
		sum += lhs;
		sum += rhs;
		return sum;
	}

	const Cluster Clustering::operator-(const Cluster &lhs, const PointPtr &rhs)
	{
		//assert (lhs.dim == rhs->getDims());
		//create cluster with lhs, -= the point, and return new cluster.
		Cluster diff(lhs);
		diff -= rhs;
		return diff;
	}

	//had to redo these.. had strange issues
	Cluster &Cluster::operator+=(const Cluster &rhs)
	{
		//assert (rhs.dim == dim);
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

	Cluster &Cluster::operator-=(const Cluster &rhs)
	{
		//assert (rhs.dim == dim);
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

	Cluster &Cluster::operator+=(const PointPtr &rhs)
	{
		add(rhs);
		return *this;
	}

	Cluster &Cluster::operator-=(const PointPtr &rhs)
	{
		remove(rhs);
		return *this;
	}


	void Cluster::setCentroid(const Point &point)
	{
		__centroid = point;
		__centroidIsValid = true;
	}


	void Cluster::computeCentroid()
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

		setCentroid(*tPoint);


		delete tPoint;

	}

	void Cluster::pickPoints(int k, PointPtr pointArray)
	{
		ptPtr = points;
		//take the last point minus the first point and divide it by k to get the average difference between points
		//adding these together will allow a linear set of points to begin testing. Not perfectly evenly disbursed
		//but better than nothing.
		PointPtr tPoint = new Point((*endPtr->p - *ptPtr->p)/k);
		for (int i = 0; i < k; i++)
		{
			//offset ptptr by 1/2
			//std::cout << (*ptPtr->p - *tPoint/2) + *tPoint*(i+1);
			pointArray[i] = (*ptPtr->p - *tPoint/2) + *tPoint*(i+1);
		}
	}

	PointPtr &Cluster::operator[](int i)
	{
		if (i < size && i >= 0)
		{
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
		}

	}

	//rather than use the sum of all of the distanes between points and dividing that by 2, which would
	//iterate n^2 times, this function increments the point we are working with and doesn't add distances twice
	//coming to a function runs (n^2+n)/2 times. Should be useful in larger clusters, and run almost half the
	//number of times
	double Cluster::intraClusterDistance() const
	{
		double d = 0;

		//iterator keeps track of the first while loop.
		LNodePtr it = points;

		//this iterator keeps track of the starting point of the next while loop
		//starts at points.next because we do not need a points distance to itself(0)
		LNodePtr  itT = points->next;

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
		//return the average, which is the sum of all of the distances divided by the size of the cluster;
		return d/size;
	}

	double Clustering::interClusterDistance(const Cluster &c1, const Cluster &c2)
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
		return d/(c1.size*c2.size);
	}




}