//
// Created by Nikolai Sharp on 9/20/15.
//

#include <cassert>
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
		os << "\ncluster size:" << cluster.size << "dims:" << cluster.dim;

		//while loop stops at end
		while (ptr != nullptr)
		{
			//print associated point
			os << *ptr->p;

			//increment point
			ptr = ptr->next;
		}
		//os << std::endl;
		return os;
	}

	//I realize now after all of this that I could have used the add function.. oh well.
	Cluster::Cluster(const Cluster &cluster)
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
		while(nextPtr != nullptr)
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

		//call destructor to wipe out cluster
		if (size != 0)
		{
			this->~Cluster();
		}

		//set nextPtr to head of right hand cluster
		nextPtr = cluster.points;


		while (nextPtr != nullptr)
		{
			add(nextPtr->p);

			nextPtr = nextPtr->next;
		}


		return *this;
	}

	Cluster::~Cluster()
	{
		//needs to delete all dynamically allocated things
		//set ptPtr to beginning
		ptPtr = points;
		//set nextPtr to ptPtr.next
		nextPtr = ptPtr->next;
		while (ptPtr != nullptr)
		{
			//delete LNode
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

	void Cluster::add(PointPtr const &ptr)
	{
		//assert that point is of correct dimension, or cluster is empty
		assert (ptr->getDims() == dim || size == 0);

		//create new LNode for use in any case
		newPtr = new LNode;

		//assign new LNode point pointer to point
		newPtr->p = ptr;

		//set newPtr to point to nullpointer (used later in while loop)
		newPtr->next = nullptr;

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
				if (*ptr > *ptPtr->p && *ptr <= *nextPtr->p)
				{
					//now we want to order similar points by address, and exclude multiple equal addresses
					//case 1 in which value is equal and address is less:
					if (*ptr <= *nextPtr->p && ptr < nextPtr->p)
					{
						//put it there
						newPtr->next = nextPtr;
						ptPtr->next = newPtr;
					}
					//case 2 in which value is equal and address is equal, break out of while loop
					else if (*ptr == *nextPtr->p && ptr == nextPtr->p)
					{
						break;
					}
					//case 3 in which value is value is equal and address is greater
					else if (*ptr == *nextPtr->p && ptr > nextPtr->p)
					{
						//bump values out
						ptPtr = ptPtr->next;
						nextPtr = nextPtr->next;
					}
					//cases 4 in which case 3 is invoked and next point is larger
					else// if (*ptr == *ptPtr->p && *ptr < *nextPtr->p)
					{
						//put it there
						newPtr->next = nextPtr;
						ptPtr->next = newPtr;
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
			if (newPtr->next == nullptr)
			{
				//decrease size, let user know, and delete the LNode
				size--;
				delete newPtr;
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

				//break out of while loop
				break;
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

	Cluster &Cluster::operator+=(const Cluster &rhs)
	{
		//make sure clusters hold same number of dimensions
		assert (rhs.dim == dim);
		//I'll use nextPtr to navigate other cluster
		nextPtr = rhs.points;

		while (nextPtr != nullptr)
		{
			//add the point. should not add duplicate specific points
			add(nextPtr->p);

			//increment nextPtr
			nextPtr = nextPtr->next;
		}
		return *this;
	}

	Cluster &Cluster::operator-=(const Cluster &rhs)
	{
		//make sure clusters have same number of dimensions
		assert (rhs.dim == dim);

		//use similar logic as +=
		//I'll use nextPtr to navigate other cluster
		nextPtr = rhs.points;

		while (nextPtr != nullptr)
		{
			//remove the point. won't remove points not in array
			remove(nextPtr->p);

			//increment nextPtr
			nextPtr = nextPtr->next;
		}

		return *this;
	}

	Cluster &Cluster::operator+=(const Point &rhs)
	{
		assert (rhs.getDims() == dim);
		//add point and return cluster
		//use newPtr->p to allow use of add function
		*newPtr->p = rhs;
		add(newPtr->p);
		return *this;
	}

	Cluster &Cluster::operator-=(const Point &rhs)
	{
		assert (rhs.getDims() == dim);
		//can just remove the point and return cluster
		//use newPtr->p to allow use of remove function
		*newPtr->p = rhs;
		remove(newPtr->p);
		return *this;
	}

	const Cluster Clustering::operator+(const Cluster &lhs, const Cluster &rhs)
	{
		assert (lhs.dim == rhs.dim);
		//create new cluster using lhs. I don't think I want to do this dynamically.
		Cluster sum(lhs);

		//add sum to rhs
		sum += rhs;

		//return sum
		return sum;
	}

	//extremely similar to + not doing dynamically..
	const Cluster Clustering::operator-(const Cluster &lhs, const Cluster &rhs)
	{
		assert (lhs.dim == rhs.dim);
		//create cluster with lhs, subtract rhs from new cluster, return new cluster.
		Cluster diff(lhs);
		diff -= rhs;
		return diff;
	}


	const Cluster Clustering::operator+(const Cluster &lhs, const PointPtr &rhs)
	{
		assert (lhs.dim == rhs->getDims());
		//create cluster to add two together, return new cluster.
		Cluster sum(lhs);
		sum += *rhs;
		return sum;
	}

	const Cluster Clustering::operator-(const Cluster &lhs, const PointPtr &rhs)
	{
		assert (lhs.dim == rhs->getDims());
		//create cluster with lhs, -= the point, and return new cluster.
		Cluster diff(lhs);
		diff -= *rhs;
		return diff;
	}
}