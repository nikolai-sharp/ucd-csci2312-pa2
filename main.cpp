#include <iostream>
#include "Point.h"
using namespace Clustering;

int main() {

    Point p1(3);
    p1.setValue(0, 1);
    p1.setValue(1, 2);
    p1.setValue(2, 3);

    std::cout << p1;

    p1 *= 4;
    std::cout << p1;

    p1 /= 2;
    std::cout << p1;

    Point p2(p1);
    std::cout << p2;

    Point p3 = p2 * 3;
    std::cout << p3;

    Point p4 = p3 / 2;
    std::cout << p4;

    std::cout << std::endl <<  p2.getDims() << std::endl << p3.distanceTo(p4);

    Point p5(5);
    p5.setValue(0,1);
    p5.setValue(1, 2);
    p5.setValue(2, 3);
    p5.setValue(3, 4);
    p5.setValue(4, 5);

    std::cout << p5;

    p5 = p4;
    std::cout << p5;

    Point p6 = p5 * 4;
    std::cout << p6;
    std::cout << std::endl << p6.distanceTo(p5);

    p6 += p5;
    std::cout << p6;

    p6 += p4;

    p5 -= p6;
    std::cout << p5;

    std::cout << std::endl << p5.distanceTo(p6);


    return 0;
}
