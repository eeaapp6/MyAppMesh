/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementTet.h"

namespace Interface
{

    FITKElementTet4::FITKElementTet4()
    {
        //填充四个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementTet4::getEleType()
    {
        return FITKModelEnum::FITKEleType::Tet4;
    }

    bool FITKElementTet4::getQuadraticFlag()
    {
        return false;
    }

    int FITKElementTet4::getNodeCount()
    {
        //四节点
        return 4;
    }

    int FITKElementTet4::getFaceCount()
    {
        return 4;
    }

    int FITKElementTet4::getEdgeCount()
    {
        return 6;
    }

    QList<int> FITKElementTet4::getFace(const int index, const bool & lowerOrder)
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
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[3];
            break;
        }
        case 2://s3
        {
            idList << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3];
            break;
        }
        case 3://s4
        {
            idList << _elemenTopo[0] << _elemenTopo[3] << _elemenTopo[2];
            break;
        }
        }
        return idList;
    }

    QList<int> FITKElementTet4::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[0]};
        case 3: return QList<int>{_elemenTopo[0], _elemenTopo[3]};
        case 4: return QList<int>{_elemenTopo[1], _elemenTopo[3]};
        case 5: return QList<int>{_elemenTopo[2], _elemenTopo[3]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementTet4::getElementDim()
    {
        return 3;
    }

    FITKElementTet10::FITKElementTet10()
    {
        //填充10个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementTet10::getEleType()
    {
        return FITKModelEnum::FITKEleType::Tet10;
    }

    bool FITKElementTet10::getQuadraticFlag()
    {
        return true;
    }

    int FITKElementTet10::getNodeCount()
    {
        //10节点
        return 10;
    }

    int FITKElementTet10::getFaceCount()
    {
        return 4;
    }

    int FITKElementTet10::getEdgeCount()
    {
        return 6;
    }

    QList<int> FITKElementTet10::getFace(const int index, const bool & lowerOrder)
    {
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://s1
        {
            idList << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
            if (!lowerOrder)
                idList << _elemenTopo[5] << _elemenTopo[4] << _elemenTopo[6];
            break;
        }
        case 1://s2
        {
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[3];
            if (!lowerOrder)
                idList << _elemenTopo[4] << _elemenTopo[8] << _elemenTopo[7];
            break;
        }
        case 2://s3
        {
            idList << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[3] ;
            if (!lowerOrder)
                idList << _elemenTopo[5] << _elemenTopo[9] << _elemenTopo[8];
            break;
        }
        case 3://s4
        {
            idList << _elemenTopo[0] << _elemenTopo[3] << _elemenTopo[2] ;
            if (!lowerOrder)
                idList << _elemenTopo[7] << _elemenTopo[9] << _elemenTopo[6];
            break;
        }
        }
        return idList;
    }

    QList<int> FITKElementTet10::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[4], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[5], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[6], _elemenTopo[0]};
        case 3: return QList<int>{_elemenTopo[0], _elemenTopo[7], _elemenTopo[3]};
        case 4: return QList<int>{_elemenTopo[1], _elemenTopo[8], _elemenTopo[3]};
        case 5: return QList<int>{_elemenTopo[2], _elemenTopo[9], _elemenTopo[3]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementTet10::getElementDim()
    {
        return 3;
    }

}

