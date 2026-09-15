/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKElementHex.h"

namespace Interface
{

    FITKElementHex8::FITKElementHex8()
    {
        //填充八个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementHex8::getEleType()
    {
        return FITKModelEnum::FITKEleType::Hex8;
    }

    bool FITKElementHex8::getQuadraticFlag()
    {
        return false;
    }

    int FITKElementHex8::getNodeCount()
    {
        //8节点
        return 8;
    }

    int FITKElementHex8::getFaceCount()
    {
        return 6;
    }

    int FITKElementHex8::getEdgeCount()
    {
        return 12;
    }

    QList<int> FITKElementHex8::getFace(const int index, const bool & lowerOrder)
    {
        Q_UNUSED(lowerOrder);
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://s1
        {
            idList << _elemenTopo[3] << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
            break;
        }
        case 1://s2
        {
            idList << _elemenTopo[4] << _elemenTopo[5] << _elemenTopo[6] << _elemenTopo[7];
            break;
        }
        case 2://s3
        {
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[5] << _elemenTopo[4];
            break;
        }
        case 3://s4
        {
            idList << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[6] << _elemenTopo[5];
            break;
        }
        case 4://s5
        {
            idList << _elemenTopo[2] << _elemenTopo[3] << _elemenTopo[7] << _elemenTopo[6];
            break;
        }
        case 5://s6
        {
            idList << _elemenTopo[0] << _elemenTopo[4] << _elemenTopo[7] << _elemenTopo[3];
            break;
        }
        }
        return idList;
    }


    QList<int> FITKElementHex8::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[3]};
        case 3: return QList<int>{_elemenTopo[3], _elemenTopo[0]};

        case 4: return QList<int>{_elemenTopo[4], _elemenTopo[5]};
        case 5: return QList<int>{_elemenTopo[5], _elemenTopo[6]};
        case 6: return QList<int>{_elemenTopo[6], _elemenTopo[7]};
        case 7: return QList<int>{_elemenTopo[7], _elemenTopo[4]};

        case 8: return QList<int>{_elemenTopo[0], _elemenTopo[4]};
        case 9: return QList<int>{_elemenTopo[1], _elemenTopo[5]};
        case 10: return QList<int>{_elemenTopo[2], _elemenTopo[6]};
        case 11: return QList<int>{_elemenTopo[3], _elemenTopo[7]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementHex8::getElementDim()
    {
        return 3;
    }

    FITKElementHex20::FITKElementHex20()
    {
        // 填充20个节点
        this->setNodeCount(this->getNodeCount());
    }

    FITKModelEnum::FITKEleType FITKElementHex20::getEleType()
    {
        return FITKModelEnum::FITKEleType::Hex20;
    }

    bool FITKElementHex20::getQuadraticFlag()
    {
        return true;
    }

    int FITKElementHex20::getNodeCount()
    {
        return 20;
    }

    int FITKElementHex20::getFaceCount()
    {
        return 6;
    }

    int FITKElementHex20::getEdgeCount()
    {
        return 12;
    }

    QList<int> FITKElementHex20::getFace(const int index, const bool & lowerOrder)
    {
        QList<int> idList = {};
        if (index >= getFaceCount() || _elemenTopo.size() < getNodeCount())return idList;

        switch (index)
        {
        case 0://s1
        {
            idList << _elemenTopo[3] << _elemenTopo[2] << _elemenTopo[1] << _elemenTopo[0];
            if (!lowerOrder)
                idList << _elemenTopo[10] << _elemenTopo[9] << _elemenTopo[8] << _elemenTopo[11];
            break;
        }
        case 1://s2
        {
            idList << _elemenTopo[4] << _elemenTopo[5] << _elemenTopo[6] << _elemenTopo[7];
            if (!lowerOrder)
                idList << _elemenTopo[12] << _elemenTopo[13] << _elemenTopo[14] << _elemenTopo[15];
            break;
        }
        case 2://s3
        {
            idList << _elemenTopo[0] << _elemenTopo[1] << _elemenTopo[5] << _elemenTopo[4];
            if (!lowerOrder)
                idList << _elemenTopo[8] << _elemenTopo[17] << _elemenTopo[12] << _elemenTopo[16];
            break;
        }
        case 3://s4
        {
            idList << _elemenTopo[1] << _elemenTopo[2] << _elemenTopo[6] << _elemenTopo[5];
            if (!lowerOrder)
                idList << _elemenTopo[9] << _elemenTopo[18] << _elemenTopo[13] << _elemenTopo[17];
            break;
        }
        case 4://s5
        {
            idList << _elemenTopo[2] << _elemenTopo[3] << _elemenTopo[7] << _elemenTopo[6];
            if (!lowerOrder)
                idList << _elemenTopo[10] << _elemenTopo[19] << _elemenTopo[14] << _elemenTopo[18];
            break;
        }
        case 5://s6
        {
            idList << _elemenTopo[0] << _elemenTopo[4] << _elemenTopo[7] << _elemenTopo[3];
            if (!lowerOrder)
                idList << _elemenTopo[16] << _elemenTopo[15] << _elemenTopo[19] << _elemenTopo[11];
            break;
        }
        }

        return idList;
    }

    QList<int> FITKElementHex20::getEdge(const int index)
    {
        switch (index)
        {
        case 0: return QList<int>{_elemenTopo[0], _elemenTopo[8], _elemenTopo[1]};
        case 1: return QList<int>{_elemenTopo[1], _elemenTopo[9], _elemenTopo[2]};
        case 2: return QList<int>{_elemenTopo[2], _elemenTopo[10], _elemenTopo[3]};
        case 3: return QList<int>{_elemenTopo[3], _elemenTopo[11], _elemenTopo[0]};

        case 4: return QList<int>{_elemenTopo[4], _elemenTopo[12], _elemenTopo[5]};
        case 5: return QList<int>{_elemenTopo[5], _elemenTopo[13], _elemenTopo[6]};
        case 6: return QList<int>{_elemenTopo[6], _elemenTopo[14], _elemenTopo[7]};
        case 7: return QList<int>{_elemenTopo[7], _elemenTopo[15], _elemenTopo[4]};

        case 8: return QList<int>{_elemenTopo[0], _elemenTopo[16], _elemenTopo[4]};
        case 9: return QList<int>{_elemenTopo[1], _elemenTopo[17], _elemenTopo[5]};
        case 10: return QList<int>{_elemenTopo[2], _elemenTopo[18], _elemenTopo[6]};
        case 11: return QList<int>{_elemenTopo[3], _elemenTopo[19], _elemenTopo[7]};
        default: return QList<int>();
        }
        return QList<int>();
    }

    int FITKElementHex20::getElementDim()
    {
        return 3;
    }


}
