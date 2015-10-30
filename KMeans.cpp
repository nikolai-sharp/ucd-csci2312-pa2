//
// Created by Nikolai Sharp on 10/21/15.
//

#include "KMeans.h"
#include <fstream>
#include <vector>
#include <cmath>

namespace Clustering
{


	KMeans::KMeans(unsigned int d) : kCluster(d)
	{
		point_space = new Cluster(d);
		dim = d;
		steps = 0;
		score = 0;
		pScore = 0;
		scoreDiff = SCORE_DIFF_THRESHOLD + 1;
		centroidArray = nullptr;
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
		dim = dims;
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
		std::ofstream outfile("out.txt");
		numOfClusters = k;
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
		for (int i = 0; i < k-1; i++)
		{
			kCluster[i]->setCentroid(centroidArray[i]);
		}
		point_space->setCentroid(centroidArray[k - 1]);

//		for (int i = 0; i < kCluster.getSize(); i++)
//		{
//			std::cout << "\nbefore centroid " << i << ":" << kCluster[i]->getCenroid();
//		}
//		std::cout << "\nbefore pointspace centroid" << ":" << point_space->getCenroid() << "\n\n";

		//KMeans algorithm
		clustify();


		outfile << *this;

		outfile.close();
	}

	double KMeans::dIn()
	{
		double sum = 0;
		for (int a = 0; a < numOfClusters; a++)
		{

			if (a != numOfClusters - 1)
			{
				sum = sum + kCluster[a]->intraClusterDistance();
			}
			else
			{
				sum = sum + point_space->intraClusterDistance();
			}
		}

		return sum;
	}

	double KMeans::dOut()
	{
		double sum = 0;
		for (int a = 0; a < numOfClusters - 1; a++)
		{
			//test all a to other clusters, m
			for (int c = a + 1; c < numOfClusters; c++)
			{
				//these two cases should account for every set of intercluster distances
				//since pointspace is the last cluster this would get to, it shouldn't even run
				//std::cout << "\n test: " << kCluster[a]->interClusterDistance(*kCluster[a], *kCluster[c]);
				if (c != numOfClusters-1)
					sum = sum + interClusterDistance(*kCluster[a], *kCluster[c]);
					//sum += Clustering::interClusterDistance(*kCluster[a], *kCluster[c]);
				else if (c == numOfClusters - 1)
					sum = sum + interClusterDistance(*kCluster[a],*point_space);
				//sum += Clustering::interClusterDistance(*kCluster[a],*point_space);
			}
		}
		return sum;
	}

	double KMeans::pIn()
	{
		double sum = 0;
		for (int a = 0; a < numOfClusters; a++)
		{

			if (a != numOfClusters - 1)
			{
				sum = sum + kCluster[a]->getClusterEdges();
			}
			else
			{
				sum = sum + point_space->getClusterEdges();
			}
		}
		return sum;
	}

	double KMeans::pOut()
	{
		double sum = 0;

		for (int a = 0; a < numOfClusters - 1; a++)
		{
			//test all a to other clusters, m
			for (int c = a + 1; c < numOfClusters; c++)
			{
				//these two cases should account for every set of intercluster distances
				//since pointspace is the last cluster this would get to, it shouldn't even run
				if (c != numOfClusters-1)
					sum = sum + interClusterEdges(*kCluster[a], *kCluster[c]);
				else if (c == numOfClusters - 1)
					sum = sum + interClusterEdges(*kCluster[a],*point_space);
			}
		}
		return sum;
	}


	void KMeans::clustify()
	{
		scoreDiff = SCORE_DIFF_THRESHOLD + 1;

		while (scoreDiff > SCORE_DIFF_THRESHOLD )
		{
			//loop through all clusters
			for (int i = 0; i < numOfClusters; i++)
			{
				//loop through all points
				//go through point_space on last loop, otherwise loop through kCluster[i]
				//find closest centroid, and move point to that cluster
				if (i != numOfClusters - 1)
				{
					//loop through kClusters
					for (int x = 0; x < numOfClusters - 1; x++)
					{


						//loop through points of each cluster
						for (int y = 0; y < kCluster[x]->getSize(); y++)
						{
							//create min index and min distance
							int minI = x;
							double minD = (*(kCluster[x]))[y]->distanceTo(kCluster[x]->getCenroid());

							//std::cout << "\nscoop:" << (*(kCluster[x]))[y]->distanceTo(kCluster[x]->getCenroid());
							//loop through clusters centroids, include point_space(represented by index k-1)
							for (int z = 0; z < numOfClusters; z++)
							{
								//'if' for kCluster points not including current kCluster 'x'
								if (z != numOfClusters - 1 && z != x &&
									minD > (*(kCluster[x]))[y]->distanceTo(kCluster[z]->getCenroid()))
								{
									minI = z;
									minD = (*(kCluster[x]))[y]->distanceTo(kCluster[z]->getCenroid());
								}
									//else if for point_space centroid
								else if (z == numOfClusters - 1 && minD > (*(kCluster[x]))[y]->distanceTo(point_space->getCenroid()))
								{
									minI = z;
									minD = (*(kCluster[x]))[y]->distanceTo(point_space->getCenroid());
								}
							}
							//move point if closer centroid is in another cluster. first if for kCluster
							if (minI != x && minI != numOfClusters - 1)
							{
								Cluster::Move((*(kCluster[x]))[y], *kCluster[x], *kCluster[minI]);
							}
								//move point to point_space if point_space centroid is coser
							else if (minI == numOfClusters - 1)
							{
								Cluster::Move((*(kCluster[x]))[y], *kCluster[x], *point_space);
							}

						}
					}
				}
				else
				{
					//loop through point_space points
					int count = point_space->getSize();
					for (int j = count - 1; j >= 0; j--)
					{
						//std::cout << "\nj: " << j;
						//set min index to k-1 to signify point_space
						int minI = numOfClusters - 1;
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
						if (minI != numOfClusters - 1)
						{
							Cluster::Move((*point_space)[j], *point_space, *kCluster[minI]);
						}
					}
				}
			}
			//loop through clusters again, check for centroid validity and recalc if not
			for (int x = 0; x < numOfClusters; x++) //TODO
			{
				// go through point_space
				if (x == numOfClusters - 1 && point_space->getSize() > 0 && !point_space->centroidIsValid())
				{
					point_space->computeCentroid();
				}
				else if (x != numOfClusters - 1 && kCluster[x]->getSize() > 0 && !kCluster[x]->centroidIsValid())
				{
					kCluster[x]->computeCentroid();
				}
			}

				//calculate score
				//std::cout << "\ndin: " << dIn() << " dout: " << dOut() << " pin: " << pIn() << " pout: " << pOut();

			score = computeClusteringScore();
			scoreDiff = std::abs(pScore - score);
			pScore = score;
//			scoreDiff--;
			//std::cout << "\nstep: " << steps << ":" << *this;
			steps++;

			std::cout << "\nsteps: " << steps << " score: " << score << " scoreDiff: " << scoreDiff;

//			for (int i = 0; i < kCluster.getSize(); i++)
//		{
//			std::cout << "\ncentroid " << i << ":" << kCluster[i]->getCenroid();
//		}
//		std::cout << "\npointspace centroid" << ":" << point_space->getCenroid();

		}

	}

}

