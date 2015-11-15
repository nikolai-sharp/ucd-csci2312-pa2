//
//  Exceptions.hpp
//  pa2
//
//  Created by Nikolai Sharp on 11/14/15.
//  Copyright © 2015 Nikolai Sharp. All rights reserved.
//

#ifndef Exceptions_hpp
#define Exceptions_hpp

#include <stdio.h>
#include <string>
#include <ostream>

namespace Clustering
{
    
    class DimensionalityMismatchEx
    {
    private:
        DimensionalityMismatchEx();
        std::string name;
        std::string m_dimError = "DimMismatchEx";
        int m_pointID1;
        int m_pointID2;
        
    public:
        DimensionalityMismatchEx(const char *name, int pID1, int pID2);
        std::string getName(){return name;};
        int getPID1(){return m_pointID1;};
        int getPID2(){return m_pointID2;};
        std::string getErr(){return m_dimError;};
        friend std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx dimEx);
    };
    
    class OutOfBoundsEx
    {
    private:
        OutOfBoundsEx();
        std::string name;
        std::string m_error = "OutOfBoundsEx";
        int m_ID;
        std::string m_class;
    public:
        OutOfBoundsEx(const char* funcName, const char* funcClass, int id);
        std::string getName() {return name;}
        std::string getClass() {return m_class;}
        std::string getError() {return m_error;}
        int getID() {return m_ID;}
        friend std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx outEx);
        
    };
    
    class RemoveFromEmptyEx
    {
    private:
        RemoveFromEmptyEx();
        std::string name;
        std::string m_error = "RemoveFromEmptyEx";
        int m_ID;
    public:
        RemoveFromEmptyEx(const char* funcName, int ID);
        std::string getName(){return name;}
        std::string getError(){return m_error;}
        int getID(){return m_ID;}
        friend std::ostream &operator<<(std::ostream &os, const RemoveFromEmptyEx emptyEx);
    };
}

#endif /* Exceptions_hpp */

