//
//  Exceptions.cpp
//  pa2
//
//  Created by Nikolai Sharp on 11/14/15.
//  Copyright © 2015 Nikolai Sharp. All rights reserved.
//

#include "Exceptions.h"

namespace Clustering
{
    DimensionalityMismatchEx::DimensionalityMismatchEx(const char *funcName, int pID1, int pID2)
    {
        name = funcName;
        m_pointID1 = pID1;
        m_pointID2 = pID2;
    }
    
    std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx dimEx)
    {
        os << dimEx.m_dimError << "; Point:: " << dimEx.name << "; pID1: " << dimEx.m_pointID1;
        os<< "; pID2: " << dimEx.m_pointID2 << std::endl;
        
        return os;
    }
    
    OutOfBoundsEx::OutOfBoundsEx(const char* funcName, const char* funcClass, int id)
    {
        name = funcName;
        m_class = funcClass;
        m_ID = id;
    }
    
    std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx outEx)
    {
        os << outEx.m_error << "; class:" << outEx.m_class << ";ID:" << outEx.m_ID;
        os  << ";Point::" << outEx.name << std::endl;
        return os;
    }
    
    RemoveFromEmptyEx::RemoveFromEmptyEx(const char* funcName, int ID)
    {
        name = funcName;
        m_ID = ID;
    }
    
    std::ostream &operator<<(std::ostream &os, const RemoveFromEmptyEx emptyEx)
    {
        os << emptyEx.m_error << "; Cluster::" << emptyEx.name << ";ID:" << emptyEx.m_ID << std::endl;
        return os;
    }
}
