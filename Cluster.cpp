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
		os << "\nc1:";

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

	Cluster::Cluster(const Cluster &cluster)
	{
//		//starts us off at the head //todo
//		LNode * oldPtr = cluster.points;
//		ptPtr = points;
//
//
////		for (int i = 0; i < cluster.size; i++)
////		{
////			ptPtr->p = oldPtr->p;
////
////			oldPtr = oldPtr->next;
////		}
	}

//	Cluster &Cluster::operator=(const Cluster &cluster) //todo
//	{
//		return <#initializer#>;
//	}

	Cluster::~Cluster() //todo
	{

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
		else if (*ptr <= *points->p)
		{
			//point new LNode to current head
			newPtr->next = points;

			//make new LNode new head
			points = newPtr;
		}

		//Now check if point should be added at end
		else if (*ptr >= *endPtr->p)
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
					//set new pointer to point to next
					newPtr->next = nextPtr;

					//set ptPtr to point to newPtr
					ptPtr->next = newPtr;
				}

				//if not, shift two over
				else
				{
					ptPtr = ptPtr->next;
					nextPtr = nextPtr->next;
				}
			}
		}

		//increment size, point was added
		size++;
	}

}