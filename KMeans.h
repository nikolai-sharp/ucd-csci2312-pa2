//
// Created by Nikolai Sharp on 10/21/15.
//

#ifndef CSCI2312PA2_KMEANS_H
#define CSCI2312PA2_KMEANS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <forward_list>
#include <cmath>
#include "Exceptions.h"
#include "Cluster.h"

namespace Clustering
{
//	typedef struct Cluster *CPointer;
//	typedef struct CNode *CNodePtr;
//
//	struct CNode {
//		CPointer c;
//		CNodePtr next;
//	};

	class KMeans
	{
	public:


		KMeans(unsigned int dim);
        ~KMeans() {delete point_space; centroidArray.clear();}
		//TODO Copy constructor

		//Primary run function. takes k as number of clusters.
		void run(unsigned int k);


		//friend ostream operator
		friend std::ostream &operator<<(std::ostream &os, const KMeans &km);

		//kmeans algorythm
		void clustify();

		//calculations for betaCV
		double dIn();
		double dOut();
		double pIn();
		double pOut();
		double computeClusteringScore() {return (dIn()/pIn()) / (dOut()/pOut());}
        
        //map function
        const std::pair<unsigned int,unsigned int> p(unsigned int p1, unsigned int p2) const;

		//cluster list class for cluster linked list
//		class Clist
//		{
//		public:
//			Clist(unsigned int dims);
//			~Clist();
//			void addCluster(unsigned int dims);
//			CPointer &operator[](int i);
//			unsigned  int getSize() {return size;}
//			friend std::ostream &operator<<(std::ostream &os, const Clist & clist);
//		private:
//			//number of clusters for os<<
//			unsigned int size, dim;
//			//cnode pointers
//			CNodePtr firstC;
//			CNodePtr lastC;
//			CNodePtr newC;
//			CNodePtr ptrC;
//		};

	private:
		//made this a pointer
		Cluster *point_space;
		//std::vector<Point> *centroidArray;
        std::vector<Point> centroidArray;
		unsigned int numOfClusters;
        std::vector<Cluster> kCluster;
		int steps;
        int numberOfPoints;

		unsigned int dim;
		//scorediffthreshhold //TODO find best
		double const SCORE_DIFF_THRESHOLD = 0;
		double score, pScore, scoreDiff;
        
//        static std::unordered_map<std::pair<unsigned int,unsigned int>,double> dMapK;



	public:
        friend std::istream &operator>>(std::istream &is, KMeans &km)
        {
            is >> *km.point_space;
            km.numberOfPoints = km.point_space->getSize();
            km.point_space->setTotalPoints(km.numberOfPoints);
            return is;
        }

	};
}
#include "KMeans.cpp"

#endif //CSCI2312PA2_KMEANS_H

//