/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementTri.h"

namespace Interface
{

    FITKElementTri3::FITKElementTri3()
    {
        //填充3个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementTri3::getEleType()
    {
        return FITKModelEnum::FITKEleType::Tri3;
    }

    bool FITKElementTri3::getQuadraticFlag()
    {
        return false;
    }

    int FITKElementTri3::getNodeCount()
    {
        //3个节点
        return 3;
    }

    int FITKElementTri3::getFaceCount()
    {
        return 5;
    }

    int FITKElementTri3::getEdgeCount()
    {
        return 3;
    }

    QList<int> FITKElementTri3::getFace(const int index, const bool & lowerOrder)
    {
        Q_UNUSED(lowerOrder);
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://spos
        {
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2];
            break;
        }
        case 1://sneg
        {
            idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
            break;
        }
        case 2://e1
        {
            idList << _elemenTopo[0] << _elemenTopo[1];
            break;
        }
        case 3://e2
        {
            idList << _elemenTopo[1] << _elemenTopo[2];
            break;
        }
        case 4://e3
        {
            idList << _elemenTopo[2] << _elemenTopo[0];
            break;
        }
        }
        return idList;
    }

    QList<int> FITKElementTri3::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[0]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementTri3::getElementDim()
    {
        return 2;
    }

    FITKElementTri6::FITKElementTri6()
    {
        //填充6个节点
        this->setNodeCount(this->getNodeCount());
    }

    Interface::FITKModelEnum::FITKEleType FITKElementTri6::getEleType()
    {
        return FITKModelEnum::FITKEleType::Tri6;
    }

    bool FITKElementTri6::getQuadraticFlag()
    {
        return true;
    }

    int FITKElementTri6::getNodeCount()
    {
        //6节点
        return 6;
    }

    int FITKElementTri6::getFaceCount()
    {
        return 5;
    }

    int FITKElementTri6::getEdgeCount()
    {
        return 3;
    }

    QList<int> FITKElementTri6::getFace(const int index, const bool & lowerOrder)
    {
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://spos
        {
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2] ;
            if (!lowerOrder)
                idList << _elemenTopo[3] << _elemenTopo[4] << _elemenTopo[5];
            break;
        }
        case 1://sneg
        {
            idList  << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0] ;
            if (!lowerOrder)
                idList << _elemenTopo[4] << _elemenTopo[3] << _elemenTopo[5];
            break;
        }
        case 2://e1
        {
            idList << _elemenTopo[0] << _elemenTopo[1] ; 
            if (!lowerOrder) idList << _elemenTopo[3];
            break;
        }
        case 3://e2
        {
            idList << _elemenTopo[1] << _elemenTopo[2] ;
            if (!lowerOrder) idList << _elemenTopo[4];
            break;
        }
        case 4://e3
        {
            idList << _elemenTopo[2] << _elemenTopo[0] ;
            if (!lowerOrder) idList << _elemenTopo[5];
            break;
        }
        }
        return idList;
    }

    QList<int> FITKElementTri6::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[3], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[4], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[5], _elemenTopo[0]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementTri6::getElementDim()
    {
        return 2;
    }

}


