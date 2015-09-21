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


	delete pt1;
    return 0;
}
