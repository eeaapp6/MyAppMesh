/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementWedge.h"

namespace Interface
{
    FITKElementWedge6::FITKElementWedge6()
    {
        //填充6个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementWedge6::getEleType()
    {
        return FITKModelEnum::FITKEleType::Wedge6;
    }

    bool FITKElementWedge6::getQuadraticFlag()
    {
        return false;
    }

    int FITKElementWedge6::getNodeCount()
    {
        //6个节点
        return 6;
    }
    int FITKElementWedge6::getFaceCount()
    {
        return 5;
    }

    int FITKElementWedge6::getEdgeCount()
    {
        return 9;
    }

    QList<int> FITKElementWedge6::getFace(const int index, const bool & lowerOrder)
    {
        Q_UNUSED(lowerOrder);
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://s1
        {
            idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
            break;
        }
        case 1://s2
        {
            idList << _elemenTopo[3] << _elemenTopo[4] << _elemenTopo[5];
            break;
        }
        case 2://s3
        {
            idList << _elemenTopo[4] << _elemenTopo[3] << _elemenTopo[0] << _elemenTopo[1];
            break;
        }
        case 3://s4
        {
            idList << _elemenTopo[5] << _elemenTopo[4] << _elemenTopo[1] << _elemenTopo[2];
            break;
        }
        case 4://s5
        {
            idList << _elemenTopo[0] << _elemenTopo[3] << _elemenTopo[5] << _elemenTopo[2];
            break;
        }
        }
        return idList;
    }

    QList<int> FITKElementWedge6::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[0]};

        case 3: return QList<int>{_elemenTopo[3], _elemenTopo[4]};
        case 4: return QList<int>{_elemenTopo[4], _elemenTopo[5]};
        case 5: return QList<int>{_elemenTopo[5], _elemenTopo[3]};

        case 6: return QList<int>{_elemenTopo[0], _elemenTopo[3]};
        case 7: return QList<int>{_elemenTopo[1], _elemenTopo[4]};
        case 8: return QList<int>{_elemenTopo[2], _elemenTopo[5]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementWedge6::getElementDim()
    {
        return 3;
    }

}


