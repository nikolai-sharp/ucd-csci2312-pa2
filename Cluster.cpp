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
			os << std::endl << *ptr->p << " : ";

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

		// check if ptPtr == ptr address
		if (ptPtr->p == ptr)
		{
			//make points = nextPtr and delete ptPtr
			points = nextPtr;

			//decrement size here too
			size--;
			delete ptPtr;
		}


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
			else
			{
				//increment both
				ptPtr = ptPtr->next;
				nextPtr = nextPtr->next;
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


}