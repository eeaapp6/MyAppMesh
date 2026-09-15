/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementQuad.h"

namespace Interface
{
    FITKElementQuad4::FITKElementQuad4()
    {
        //填充四节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementQuad4::getEleType()
    {
        return FITKModelEnum::FITKEleType::Quad4;
    }

    bool FITKElementQuad4::getQuadraticFlag()
    {
        return false;
    }

    int FITKElementQuad4::getNodeCount()
    {
        //一共四个节点
        return 4;
    }

    int FITKElementQuad4::getFaceCount()
    {
        return 6;
    }

    int FITKElementQuad4::getEdgeCount()
    {
        return 4;
    }

    QList<int> FITKElementQuad4::getFace(const int index, const bool & lowerOrder)
    {
        Q_UNUSED(lowerOrder);
        //返回单元的所有节点
    
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://spos
        {
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3];
            break;
        }
        case 1://sneg
        {
            idList << _elemenTopo[3] << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
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
            idList << _elemenTopo[2] << _elemenTopo[3];
            break;
        }
        case 5://e4
        {
            idList << _elemenTopo[3] << _elemenTopo[0];
            break;
        }
        }
        return idList;
    }

    QList<int> FITKElementQuad4::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[3]};
        case 3: return QList<int>{_elemenTopo[3], _elemenTopo[0]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementQuad4::getElementDim()
    {
        return 2;
    }

    FITKElementQuad8::FITKElementQuad8()
    {
        //填充四节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementQuad8::getEleType()
    {
        return FITKModelEnum::FITKEleType::Quad8;
    }

    bool FITKElementQuad8::getQuadraticFlag()
    {
        return true;
    }

    int FITKElementQuad8::getNodeCount()
    {
        //一共8个节点
        return 8;
    }


    int FITKElementQuad8::getFaceCount()
    {
        return 6;
    }

    int FITKElementQuad8::getEdgeCount()
    {
        return 4;
    }

    QList<int> FITKElementQuad8::getFace(const int index, const bool & lowerOrder)
    {
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://spos
        {
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3];
            if (!lowerOrder)   
                 idList << _elemenTopo[4] << _elemenTopo[5] << _elemenTopo[6] << _elemenTopo[7];
            break;
        }
        case 1://sneg
        {
            idList << _elemenTopo[3] << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
            if (!lowerOrder)
                idList << _elemenTopo[6] << _elemenTopo[5] << _elemenTopo[4] << _elemenTopo[7];
            break;
        }
        case 2://e1
        {
            idList << _elemenTopo[0] << _elemenTopo[1] ;
            if (!lowerOrder)
                idList << _elemenTopo[4];
            break;
        }
        case 3://e2
        {
            idList << _elemenTopo[1] << _elemenTopo[2] ;
            if (!lowerOrder)
                idList << _elemenTopo[5];
            break;
        }
        case 4://e3
        {
            idList << _elemenTopo[2] << _elemenTopo[3] ;
            if (!lowerOrder)
                idList << _elemenTopo[6];
            break;
        }
        case 5://e4
        {
            idList << _elemenTopo[3] << _elemenTopo[0] ;
            if (!lowerOrder)
                idList << _elemenTopo[7];
            break;
        }
        }
        return idList;
    }

    QList<int> FITKElementQuad8::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[4], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[5], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[6], _elemenTopo[3]};
        case 3: return QList<int>{_elemenTopo[3], _elemenTopo[7], _elemenTopo[4]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementQuad8::getElementDim()
    {
        return 2;
    }

}

