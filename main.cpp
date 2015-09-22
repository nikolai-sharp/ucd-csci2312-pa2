#include <iostream>
#include "Point.h"
#include "Cluster.h"
using namespace Clustering;

int main() {
//tests galore!
	//create first point
    Point p1(3);
    p1.setValue(0, 1);
    p1.setValue(1, 2);
    p1.setValue(2, 3);

	//test << overload
    std::cout << p1;

	//test *= operator overload
    p1 *= 4;
    std::cout << p1;

	//test /= operator
    p1 /= 2;
    std::cout << p1;

	//test initial assignment overload
    Point p2(p1);
    std::cout << p2;

	//test = operator overload in initialization
    Point p3 = p2 * 3;
    std::cout << p3;

	//test = operator after initialization
    Point p4(3);
	p4 = p3 / 2;
    std::cout << p4;

	//test getDims and distanceTo functions
    std::cout << std::endl <<  p2.getDims() << std::endl << p3.distanceTo(p4);

	//test point initialization with 5 dimensions
    Point p5(5);
    p5.setValue(0,1);
    p5.setValue(1, 2);
    p5.setValue(2, 3);
    p5.setValue(3, 4);
    p5.setValue(4, 5);

	//test << with another number of dimensions
    std::cout << p5;

	//more init tests
    p5 = p4;
    std::cout << p5;

	//more init tests and distanceTo function with more dimensions
    Point p6 = p5 * 4;
    std::cout << p6;
    std::cout << std::endl << p6.distanceTo(p5);

	//test += overload
    p6 += p5;
    std::cout << p6;

	//test += overload with points of different numbers of dimensions
    p6 += p4;

	//test -= operator
    p5 -= p6;
    std::cout << p5;

	//test distanceTo func again
    std::cout << std::endl << p5.distanceTo(p6);

	//creation of more points for more tests
    Point p7 = p5 + p6;
    std::cout << p7;

    std::cout << (p6 - p7);

	//more tests of = overloaded and comparison(==) operator
	Point p8 = p5;
	std::cout << std::endl << (p8 == p5) << std::endl << (p5 == p6);

	//test compared to p1 and p2.. lexicographic. last dimension different
	Point p9(3);
	p9.setValue(0,2);
	p9.setValue(1,4);
	p9.setValue(2,5);

	//prints values for easy mental testing
	std::cout << p1;
	std::cout << p2;
	std::cout << p3;
	std::cout << p4;
	std::cout << p9;

	std::cout << std::endl << (p9 < p1);
	std::cout << std::endl << (p1 < p3);
	std::cout << std::endl << (p3 < p4);
	std::cout << std::endl << (p1 < p2);
	std::cout << std::endl << std:: endl << (p1 == p2) << std::endl << (p1 != p3);
	std::cout << std::endl << std:: endl << (p9 == p2) << std::endl << (p1 != p2);

	//test less than and greater than instances
	std::cout << std::endl << std:: endl << (p3 > p2) << std::endl << (p2 > p3);
	std::cout << std::endl << std:: endl << (p1 < p9) << std::endl << (p1 > p9);
	std::cout << std::endl << std:: endl << (p1 < p2) << std::endl << (p2 < p3);

	//reprint values for easy mental testing
	std::cout << p1;
	std::cout << p2;
	std::cout << p3;
	std::cout << p4;
	std::cout << p9;

	//test less than or equal to as well as greater than or equal to
	std::cout << std::endl << std:: endl << (p1 <= p2) << std::endl << (p2 >= p3);
	std::cout << std::endl << std:: endl << (p1 <= p4) << std::endl << (p3 >= p9);
	std::cout << std::endl << std:: endl << (p1 >= p2) << std::endl << (p2 <= p3);

	std::cout << "\n\nNow begins cluster class testing\n";

	//create LNode for test
	LNode test;
	test.p = &p1;

	//test LNode printing
	std::cout << *test.p;

	//test LNode for printing
	LNodePtr testptr;
	testptr = &test;

	std::cout << *testptr->p;

	//test manipulation of pointers via LNode
	*testptr->p *= 4;
	//test multiple cout problem:
	std::cout << *testptr->p << p9; //it worked!

	//finally test cluster add and print
	std::cout << std::endl << std::endl;

	Cluster c1;

	c1.add(&p1);

	std::cout << c1;

	c1.add(&p2);
	c1.add(&p3);

	std::cout << c1;

	//try to add point of more dims, should not work
	//c1.add(&p5); //will not let me. good.
	//std::cout << c1;

	c1.add(&p9);

	std::cout << c1;


	//create more points for cluster testing
	Point p10 = p9 * 10;
	Point p11 = p9 * 2;

	//add them to cluster
	c1.add(&p10);
	c1.add(&p11);

	//print result
	std::cout << c1;

	//test dynamic creation of empty point and its addition to cluster
	Point *pt1 = new Point();
	//test setting empty point equal to another.
	*pt1 = p11;

	c1.add(pt1);

	std::cout << c1;

	//test copy constructor
	Cluster c2(c1);

	std::cout << "\nc2:" << c2;

	Cluster * ct1 = new Cluster(c1);

	//test of dynamically allocated cluster and cluster destructor (I think)
	std::cout << "\nct1:" << *ct1;
	delete ct1;
	//std::cout << "\nct1:" << *ct1;

	Cluster *ct2 = new Cluster;
	ct2->add(&p1);
	ct2->add(&p2);
	ct2->add(&p3);
	ct2->add(&p4);
	ct2->add(&p9);
	ct2->add(&p10);
	ct2->add(&p11);

	//test organization of points in cluster (had addresses show too) with multiple similar points
	Point p12(3);
	p12.setValue(0,2);
	p12.setValue(1,4);
	p12.setValue(2,6);

	Point p13(3);
	p13.setValue(0,2);
	p13.setValue(1,4);
	p13.setValue(2,6);

	ct2->add(&p12);
	ct2->add(&p11);
	ct2->add(&p11);
	ct2->add(&p13);
	ct2->add(&p13);

	std::cout << "ct2" << *ct2;

	Cluster * ct3 = new Cluster(*ct2);

	std::cout << "\nthis should be 1:" << (*ct2 == *ct3);
	std::cout << "\nthis should be 0:" << (c1 == *ct2);

	std::cout << p1 << p2 << p3 << p4 << p9 << p10 << p11 << p12 << p13;

	*ct2 -= c1;

	std::cout << *ct2;

	Cluster c4;
	c4 = (c1-c2);
	c4 += &p2;
	c4 -= &p9;

	std::cout << p9 << "\nc4           	  boop: " << c4;

	Cluster c5;
	c5 = (c1 + c2);
	c5 -= &p1;
	std::cout << c1 << c2 << c5;


	Point po1(3);
	po1.setValue(0,1);
	po1.setValue(1,1);
	po1.setValue(2,1);

	Point po2(3);
	po2.setValue(0,2);
	po2.setValue(1,2);
	po2.setValue(2,2);

	Point po3(3);
	po3.setValue(0,3);
	po3.setValue(1,3);
	po3.setValue(2,3);

	Point po4(3);
	po4.setValue(0,4);
	po4.setValue(1,4);
	po4.setValue(2,4);

	Point po5(3);
	po5.setValue(0,2);
	po5.setValue(1,2);
	po5.setValue(2,2);

	Cluster cl1;
	Cluster cl2;

	cl1.add(&po1);
	cl1.add(&po2);
	cl1.add(&po3);
	cl2.add(&po4);
	cl2.add(&po5);
	cl2.add(&po3);

	std::cout << "\n              cl1 + cl2:" << (cl1 + cl2);
	std::cout << "\n              cl1 - cl2:" << (cl1 - cl2);
	std::cout << "\n              cl2 + cl1:" << (cl2 + cl1);
	std::cout << "\n              cl2 - cl1:" << (cl2 - cl1);

	std::cout << "\n            (cl1 +cl2) - (cl1-cl2) - (cl2 - cl1)" << (cl1 +cl2) - (cl1-cl2) - (cl2 - cl1);


	delete ct3;
	delete ct2;
	delete pt1;
    return 0;
}
