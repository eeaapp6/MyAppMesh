/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementVertex.h"

namespace Interface
{
    FITKElementVertex1::FITKElementVertex1()
    {
        //填充3个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementVertex1::getEleType()
    {
        return FITKModelEnum::Vertex1;
    }

    bool FITKElementVertex1::getQuadraticFlag()
    {
        return false;
    }

    int FITKElementVertex1::getNodeCount()
    {
        //1个节点
        return 1;
    }
    int FITKElementVertex1::getFaceCount()
    {
        return 0;
    }

    int FITKElementVertex1::getEdgeCount()
    {
        return 0;
    }
    
    QList<int> FITKElementVertex1::getFace(const int index, const bool & lowerOrder)
    {
        Q_UNUSED(index);
        Q_UNUSED(lowerOrder);
        QList<int> idList = {};
        return idList;
    }
    QList<int> FITKElementVertex1::getEdge(const int index)
    {
        Q_UNUSED(index);
        QList<int> idList = {};
        return idList;
    }
    int FITKElementVertex1::getElementDim()
    {
        return 0;
    }

}

