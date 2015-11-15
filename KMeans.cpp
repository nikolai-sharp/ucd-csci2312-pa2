//
// Created by Nikolai Sharp on 10/21/15.
//
#include "KMeans.h"
#include <fstream>
#include <vector>
#include <cmath>
#include "Exceptions.h"

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
		//centroidArray = nullptr;
	}

	KMeans::~KMeans()
	{
		//point_space->~Cluster();
		delete point_space;
		//delete kCluster;//not a pointer

//		for (int i = 0; i < numOfClusters; i++)
//		{
//			centroidArray[i].~Point();
//		}

        centroidArray.clear();

		//kCluster.~Clist();



	}

	std::ostream &operator<<(std::ostream &os, const KMeans &km)
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
        //try catch
        return firstC->c;
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
			ptrC->c->~Cluster();
			delete ptrC;
			ptrC = newC;
		}
	}


	std::ostream &operator<<(std::ostream &os, const KMeans::Clist &clist)
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
//        for (int i = 0; i < k; i++)
        
//        for (int i = 0; i < k; i++)
//            centroidArray.push_back(Point(dim));
        
		//pick centroids and assign then to the array.
		point_space->pickPoints(k, centroidArray);
        
//        for (int i = 0; i < k; i++)
//            std::cout << centroidArray.at(i);
        

		//set the centroids of each cluster, starting with kClusters
        try
        {
            for (int i = 0; i < k-1; i++)
            {
                kCluster[i]->setCentroid(centroidArray.at(i));
            }
            point_space->setCentroid(centroidArray.at(k-1));
        }
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "KMeans::run()" << outEx;
        }

//		for (int i = 0; i < kCluster.getSize(); i++)
//		{
//			std::cout << "\nbefore centroid " << i << ":" << kCluster[i]->getDimms() << kCluster[i]->getCenroid();
//		}
//		std::cout << "\nbefore pointspace centroid" << ":" << point_space->getDimms() << point_space->getCenroid() << "\n\n";

        //std::cout << "\n\n wtf?::" << (*point_space)[2]->distanceTo(kCluster[1]->getCenroid());
		//KMeans algorithm
		clustify();
        
        //test for removefromemptyex
//        try
//        {
//            Cluster c1(3);
//            Cluster c2(3);
//            Point p1(3);
//            c1.add(&p1);
//            Cluster::Move::Move(&p1,c1,c2);
//            Cluster::Move::Move(&p1,c1,c2);
//        }
//        catch (RemoveFromEmptyEx remex)
//        {
//            std::cerr << "KMeans::run()test;" << remex;
//        }

		std::cout << "\nprinting:\n";
		outfile << *this;

		outfile.close();
	}

	double KMeans::dIn()
	{
		try
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
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "KMeans::dIn();" << outEx;
            throw outEx;
        }
    }

	double KMeans::dOut()
	{
        
		try
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
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "KMeans::dOut();" << outEx;
            throw outEx;
        }
	}

	double KMeans::pIn()
	{
		try
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
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "KMeans::pIn();" << outEx;
            throw outEx;
        }
	}

	double KMeans::pOut()
	{
		try
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
        catch (OutOfBoundsEx outEx)
        {
            std::cerr << "KMeans::pOut();" << outEx;
            throw outEx;
        }
	}


	void KMeans::clustify()
	{
		scoreDiff = SCORE_DIFF_THRESHOLD + 1;

		while (scoreDiff > SCORE_DIFF_THRESHOLD && steps < 4)
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
								else if (z == numOfClusters - 1 && z != x &&
										minD > (*(kCluster[x]))[y]->distanceTo(point_space->getCenroid()))
								{
									minI = z;
									minD = (*(kCluster[x]))[y]->distanceTo(point_space->getCenroid());
								}
							}
							//move point if closer centroid is in another cluster. first if for kCluster
							try
                            {
                                if (minI != x && minI != numOfClusters - 1)
                                {
                                    Cluster::Move((*(kCluster[x]))[y], *kCluster[x], *kCluster[minI]);
                                    y--;
                                }
                                //move point to point_space if point_space centroid is coser
                                else if (minI != x && minI == numOfClusters - 1)
                                {
                                    Cluster::Move((*(kCluster[x]))[y], *kCluster[x], *point_space);
                                    y--;
                                }
                            }
                            catch (OutOfBoundsEx outEx)
                            {
                                throw outEx;
                            }
                            catch (RemoveFromEmptyEx emptyEx)
                            {
                                std::cerr << "KMeans::clustify()" << emptyEx;
                            }
                            

						}
					}
				}
				else
				{
					//loop through point_space points
					for (int j = 0; j < point_space->getSize(); j++)
					{
						//std::cout << "\nj: " << j;
						//set min index to k-1 to signify point_space
						int minI = numOfClusters - 1;
						//find min distance
                        //std::cout << "wtf2: " << (*point_space)[j]->distanceTo(point_space->getCenroid()) << std::endl;
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
						try
                        {
                            if (minI != numOfClusters - 1)
                            {
                                Cluster::Move((*point_space)[j], *point_space, *kCluster[minI]);
                                j--;
                            }
                        }
                        catch (OutOfBoundsEx outEx)
                        {
                            throw outEx;
                        }
                        catch (RemoveFromEmptyEx emptyEx)
                        {
                            std::cerr << "KMeans::clustify()" << emptyEx;
                        }
					}
				}
			}
			//loop through clusters again, check for centroid validity and recalc if not
            try
            {
                for (int x = 0; x < numOfClusters; x++) //TODO
                {
                    // go through point_space
                    if (x != numOfClusters - 1 && kCluster[x]->getSize() > 0 && !kCluster[x]->centroidIsValid())
                    {
                        kCluster[x]->computeCentroid();
                    }
                    else if (x == numOfClusters - 1 && point_space->getSize() > 0 && !point_space->centroidIsValid())
                    {
                        point_space->computeCentroid();
                    }
                }
            }
            catch (OutOfBoundsEx outEx)
            {
                std::cerr << "KMeans::clustify()" << outEx;
            }

				//calculate score
				//std::cout << "\ndin: " << dIn() << " dout: " << dOut() << " pin: " << pIn() << " pout: " << pOut();

			score = computeClusteringScore();
			scoreDiff = std::abs(pScore - score);
			pScore = score;
//			scoreDiff--;
			//std::cout << "\nstep: " << steps << ":" << *this;
			steps++;

            std::cout << "steps: " << steps << " score: " << score << " scoreDiff: " << scoreDiff << std::endl;

//			for (int i = 0; i < kCluster.getSize(); i++)
//		{
//			std::cout << "\ncentroid " << i << ":" << kCluster[i]->getCenroid();
//		}
//		std::cout << "\npointspace centroid" << ":" << point_space->getCenroid();

		}
		std::cout << "\n!!done!!\n";

	}

}

//
