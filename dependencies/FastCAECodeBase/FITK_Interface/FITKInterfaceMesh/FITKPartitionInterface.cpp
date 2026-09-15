/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPartitionInterface.h"

namespace FITKPartition
{
    // Implementation of member functions can be added here
    void FITKPartitionInterface::setPartition1(int partitionID, int cellIndex, int surfaceIndex)
    {
        m_partitionID1 = partitionID;
        m_cellIndex1 = cellIndex;
        m_surfaceIndex1 = surfaceIndex;
    }

    void FITKPartitionInterface::setPartition2(int partitionID, int cellIndex, int surfaceIndex)
    {
        m_partitionID2 = partitionID;
        m_cellIndex2 = cellIndex;
        m_surfaceIndex2 = surfaceIndex;
    }

    void FITKPartitionInterface::setInterfaceIndex(int interfaceIndex)
    {
        m_interFaceIndex = interfaceIndex;
    }

    int FITKPartitionInterface::getInterfaceIndex()
    {
        return m_interFaceIndex;
    }
    int FITKPartitionInterface::getPartitionID1()
    {
        return m_partitionID1;
    }
    int FITKPartitionInterface::getPartitionID2()
    {
        return m_partitionID2;
    }
    int FITKPartitionInterface::getCellIndex1()
    {
        return m_cellIndex1;
    }
    int FITKPartitionInterface::getCellIndex2()
    {
        return m_cellIndex2;
    }
    int FITKPartitionInterface::getSurfaceIndex1()
    {
        return m_surfaceIndex1;
    }
    int FITKPartitionInterface::getSurfaceIndex2()
    {
        return m_surfaceIndex2;
    }

 /*************************************************************/

    void FITKPartitionInterfaceManager::setPartitionInfo(const QList<QSet<int>>& faceNeighborRegion)
    {
        _faceNeighborRegion = faceNeighborRegion;
    }

    void FITKPartitionInterfaceManager::setCellsPartitionInfo(const std::vector<int>& cellPartitionInfo)
    {
        _cellPartitionInfo = cellPartitionInfo;
        //更新分区数量
        _partitionNum = -1;
        for (auto partitionID : cellPartitionInfo)
            if (partitionID > _partitionNum)
                _partitionNum = partitionID;
        _partitionNum++;//分区ID从0开始，数量=最大ID+1
    }

    int FITKPartitionInterfaceManager::getPartitionNum() const
    {
        return _partitionNum;
    }

    const std::vector<int>& FITKPartitionInterfaceManager::getCellsPartitionInfo()
    {
        return _cellPartitionInfo;
    }

    QList<int> FITKPartitionInterfaceManager::getCellsByPartitionIndex(int partitionIndex)
    {
        if(partitionIndex<0|| partitionIndex>_partitionNum-1)
            return QList<int>();
        QList<int> cellIndexs;
        for (int i=0;i< _cellPartitionInfo.size();++i)
        {
            if (_cellPartitionInfo[i] == partitionIndex)
                cellIndexs += i;
        }
        return cellIndexs;
    }

    int FITKPartitionInterfaceManager::getCellPartitionIndex(int cellIndex)
    {
        if (cellIndex < 0 || cellIndex >= _cellPartitionInfo.size())
            return -1;
        return _cellPartitionInfo[cellIndex];
    }

    void FITKPartitionInterfaceManager::setNodesPartitionInfo(const std::vector<int>& nodePartitionInfo)
    {
        _nodePartitionInfo = nodePartitionInfo;
    }

    const std::vector<int>& FITKPartitionInterfaceManager::getNodesPartitionInfo()
    {
        return _nodePartitionInfo;
    }

    int FITKPartitionInterfaceManager::getNodePartitionIndex(int nodeIndex)
    {
        if (nodeIndex < 0 || nodeIndex >= _nodePartitionInfo.size())
            return -1;
        return _nodePartitionInfo[nodeIndex];
    }

    QList<int> FITKPartitionInterfaceManager::getNeighborPartition(int partitionIndex) const
    {
        if (_faceNeighborRegion.size() < 1)
            return QList<int>();
        QList<int> neighbor;
        for (QSet<int> regionIDs : _faceNeighborRegion)
        {
            regionIDs.subtract(QSet<int>{partitionIndex});
            if (regionIDs.size() == 1)
                neighbor.append(regionIDs.toList());
        }
        return neighbor;
    }

    int FITKPartitionInterfaceManager::getPartitionFaceCount() const
    {
        return _faceNeighborRegion.size();
    }

    QList<int> FITKPartitionInterfaceManager::getFaceNeighborByFaceIndex(int faceIndex) const
    {
        if (_faceNeighborRegion.isEmpty() || faceIndex<0 
            || faceIndex >_faceNeighborRegion.size() - 1)

            return QList<int>();

        return _faceNeighborRegion[faceIndex].toList();
    }

    int FITKPartitionInterfaceManager::getFaceIndexByFaceNeighbor(int partitionIndex1, int partitionIndex2) const
    {
        return  _faceNeighborRegion.indexOf(QSet<int>{partitionIndex1, partitionIndex2});
    }

    QList<FITKPartitionInterface*> FITKPartitionInterfaceManager::getEleFaceListByFaceIndex(int faceIndex)
    {
        QList<FITKPartitionInterface*> partitionInterfaces;
        for (int i = 0; i < this->getDataCount(); ++i)
        {
            FITKPartitionInterface* data = this->getDataByIndex(i);
            if (data->getInterfaceIndex() == faceIndex)
                partitionInterfaces.append(data);
        }
        return partitionInterfaces;
    }

    QList<FITKPartitionInterface*> FITKPartitionInterfaceManager::getEleFaceListByFaceIndex(int partitionIndex1, int partitionIndex2)
    {
        int faceIndex = getFaceIndexByFaceNeighbor(partitionIndex1, partitionIndex2);
        return 
            this->getEleFaceListByFaceIndex(faceIndex);
    }

}
