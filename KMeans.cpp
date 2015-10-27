//
// Created by Nikolai Sharp on 10/21/15.
//

#include "KMeans.h"
#include <fstream>
#include <vector>

namespace Clustering
{


	KMeans::KMeans(unsigned int d):kCluster(dim)
	{
		point_space = new Cluster(d);
		dim = d;
	}

	KMeans::~KMeans()
	{
		delete point_space;
		delete[] centroidArray;

		//delete kCluster;//not a pointer


	}

	std::ostream &Clustering::operator<<(std::ostream &os, const KMeans &km)
	{
		os << *km.point_space;
		//TODO loop through created clusters

		os << km.kCluster;


		return os;
	}


	void KMeans::Clist::addCluster(unsigned int dims)
	{
		if (size == 0)
		{
			firstC = new CNode;
			firstC->c = new Cluster(dims);
			lastC = firstC;
			size++;
			lastC->next = nullptr;
		}
		else
		{
			newC = new CNode;
			newC->c = new Cluster(dims);
			lastC->next = newC;
			lastC = newC;
			lastC->next = nullptr;
			size++;
		}
	}

	CPointer &KMeans::Clist::operator[](int i)
	{
		if (i < size && i >= 0)
		{
			int index = 0;
			ptrC = firstC;
			while (ptrC != nullptr)
			{
				if (index == i)
					return ptrC->c;
				else
				{
					index++;
					ptrC = ptrC->next;
				}
			}
		}
	}

	KMeans::Clist::Clist(unsigned int dims)
	{
		firstC = nullptr;
		lastC = nullptr;
		dim = dim;
	}


	KMeans::Clist::~Clist()
	{
		ptrC = firstC;
		while (ptrC != nullptr)
		{
			newC = ptrC->next;
			delete ptrC->c;
			delete ptrC;
			ptrC = newC;
		}
	}


	std::ostream &Clustering::operator<<(std::ostream &os, const KMeans::Clist &clist)
	{
		CNodePtr ptr = clist.firstC;
		while (ptr != nullptr)
		{
			os << *ptr->c;
			ptr = ptr->next;
		}
		return os;
	}
	void KMeans::run(unsigned int k)
	{
		numOfClusters = k;
		double score, scoreDiff;
		//create k-1 clusters


		for (int i = 0; i < k - 1; i++)
		{
			kCluster.addCluster(dim);
		}


		//create initial k centroids
		centroidArray = new Point[k];
		//pick centroids and assign then to the array.
		point_space->pickPoints(k, centroidArray);

		//set the centroids of each cluster, starting with kClusters
		for (int i = 0; i < kCluster.getSize(); i++)
			kCluster[i]->setCentroid(centroidArray[i]);
		point_space->setCentroid(centroidArray[k-1]);

//		scoreDiff = SCORE_DIFF_THRESHOLD + 1;

		scoreDiff = SCORE_DIFF_THRESHOLD + 8;
		//std::cout << *(*point_space)[1];

//		Cluster::Move((*point_space)[1],*point_space,*kCluster[0]);
//		Cluster::Move((*point_space)[1],*point_space,*kCluster[1]);
//
////	//	//for testing
//		for (int i = 0; i < kCluster.getSize(); i++)
//		{
//			std::cout << kCluster[i]->getCenroid();
//		}
//		std::cout << point_space->getCenroid();




		//KMeans algorithm
		while (scoreDiff > SCORE_DIFF_THRESHOLD)
		{
			//loop through all clusters
			for (int i = 0; i < numOfClusters; i++)
			{
				//loop through all points

				//go through point_space on last loop, otherwise loop through kCluster[i]
				//find closest centroid, and move point to that cluster
				if (i == numOfClusters - 1)
				{
					//loop through point_space points
					int count = point_space->getSize();
					for (int j = count - 1; j > 0; j--)
					{
						std::cout << "\nj: " << j;
						//set min index to k-1 to signify point_space
						int minI = k-1;
						//find min distance
						double minD = (*point_space)[j]->distanceTo(point_space->getCenroid());
						//loop through centroids, don't need to go through point_space
						for (int l = 0; l < numOfClusters - 1; l++)
						{
//							std::cout << "\ni: " << i << "j: " << j;
//							std::cout << "l: " << l << ":" << (*point_space)[j]->distanceTo(kCluster[l]->getCenroid());
							//set new minP and minD if closer
							if ((*point_space)[j]->distanceTo(kCluster[l]->getCenroid()) < minD)
							{
								minI = l;
								minD = (*point_space)[j]->distanceTo(kCluster[l]->getCenroid());
							}
						}

						//move the point if i != k-1
						if (minI != numOfClusters-1)
						{
							Cluster::Move((*point_space)[j], *point_space, *kCluster[minI]);
						}
					}
				}


				else //TODO finish this going through all poitns and moving
				{
					//loop through kClusters
					for (int x = 0 ;x < k-2; x++)
					{

						//loop through points of each cluster
						for (int y = 0; y < kCluster[x]->getSize(); y++)
						{
							//create min index and min distance
							int minI = x;
							//double minD = (*(kCluster[x]))[y]->distanceTo(kCluster[x]->getCenroid());
							std::cout << "scoop:" << (*(kCluster[x]))[y]->distanceTo(kCluster[x]->getCenroid());
							//double minD = (kCluster[x]
							//loop through clusters, include point_space(represented by index k-1)
						}
					}
				}
			}
			//loop through clusters again, check for centroid validity and recalc if not
			for (int x = 0; x < k; x++)
			{
				// go through point_space
				if (x == k - 1 && point_space->getSize() > 0 && !point_space->centroidIsValid())
				{
					point_space->computeCentroid();
				}
				else if (x != k-1 && kCluster[x]->getSize() > 0 && !kCluster[x]->centroidIsValid())
				{
					kCluster[x]->computeCentroid();
				}
			}

			//calculate scoreDiff
			scoreDiff--;
		}
	}
}