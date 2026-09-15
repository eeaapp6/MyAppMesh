/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_PARTITION_INTERFACE_H__
#define __FITK_PARTITION_INTERFACE_H__

#include "FITKInterfaceMeshAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include <QSet>

namespace FITKPartition
{
    /**
     * @brief 记录分区面单元对儿的信息。该类中，Interface是交界面的意思，PartitionInterface是分区交界面的意思
     */
    class FITKInterfaceMeshAPI FITKPartitionInterface : public Core::FITKAbstractDataObject
    {
    public:
        explicit FITKPartitionInterface() = default;
        virtual ~FITKPartitionInterface() = default;
        /**
        * @brief 设置分区面(边)索引
        * @author wangning (2185896382@qq.com)
        * @date  2025-12-31
        */
        void setInterfaceIndex(int interfaceIndex);
        /**
        * @brief 获取分区面(边)索引
        * @author wangning (2185896382@qq.com)
        * @date  2025-12-31
        */
        int getInterfaceIndex();

        void setPartition1(int partitionID, int cellIndex, int surfaceIndex);
        void setPartition2(int partitionID, int cellIndex, int surfaceIndex);

        int getPartitionID1();
        int getPartitionID2();

        int getCellIndex1();
        int getCellIndex2();

        int getSurfaceIndex1();
        int getSurfaceIndex2();

    protected:
        int m_interFaceIndex = -1;//分区面(边)索引

        int  m_partitionID1 {0}; //单元所属分区号（=分区索引）
        int  m_cellIndex1 {0};   //单元索引
        int  m_surfaceIndex1 {0};//单元面(边)索引

        int  m_partitionID2 {0}; //单元所属分区号（=分区索引）
        int  m_cellIndex2 {0};   //单元索引
        int  m_surfaceIndex2 {0};//单元面(边)索引
    };

    /**
     * @brief 分区交接面管理器，记录了分区面(边)信息和单元分区信息、节点分区信息。该类中，Interface是交界面的意思，PartitionInterface是分区交界面的意思
     */
    class FITKInterfaceMeshAPI FITKPartitionInterfaceManager : public Core::FITKAbstractDataObject, 
        public Core::FITKAbstractDataManager<FITKPartitionInterface>
    {
    public:
        FITKPartitionInterfaceManager() = default;
        ~FITKPartitionInterfaceManager() = default;

        /**
         * @brief 设置分区面(边)信息
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        void setPartitionInfo(const QList<QSet<int>>& faceNeighborRegion);

        /**
         * @brief 设置/获取 单元分区信息(按索引排序)
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        void setCellsPartitionInfo(const std::vector<int>& cellPartitionInfo);
        const std::vector<int>& getCellsPartitionInfo();

        /**
        * @brief 通过分区索引获取某个分区的所有单元
        * @author wangning (2185896382@qq.com)
        * @date  2025-12-31
        */
        QList<int> getCellsByPartitionIndex(int partitionIndex);

        /**
         * @brief 通过单元索引获取 单元分区号
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        int getCellPartitionIndex(int cellIndex);

        /**
        * @brief 设置/获取 节点分区信息(按索引排序)
        * @author wangning (2185896382@qq.com)
        * @date  2025-12-31
        */
        void setNodesPartitionInfo(const std::vector<int>& nodePartitionInfo);
        const std::vector<int>& getNodesPartitionInfo();
        /**
         * @brief 通过节点索引获取 节点分区号
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        int getNodePartitionIndex(int nodeIndex);

        /**
         * @brief 根据给定分区号，获取邻居分区号
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        QList<int> getNeighborPartition(int partitionIndex) const;
        /**
         * @brief 获取分区个数
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        int getPartitionNum() const;
        /**
         * @brief 获取分区面(边)个数
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        int getPartitionFaceCount() const;
        /**
         * @brief 根据分区面(边)号获取两侧分区号
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        QList<int> getFaceNeighborByFaceIndex(int faceIndex) const;
         /**
         * @brief 根据两侧分区号获取分区面(边)号，返回-1代表两分区不相邻
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        int getFaceIndexByFaceNeighbor(int partitionIndex1, int partitionIndex2) const;
        /**
         * @brief 根据分区面(边)号获取交界处单元对链表
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        QList<FITKPartitionInterface*> getEleFaceListByFaceIndex(int faceIndex);
        /**
        * @brief 根据两侧分区号获取交界处单元对链表
        * @author wangning (2185896382@qq.com)
        * @date  2025-12-31
        */
        QList<FITKPartitionInterface*> getEleFaceListByFaceIndex(int partitionIndex1, int partitionIndex2);

    private:
        /**
         * @brief 分区面（按索引排序）两侧的分区号
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        QList<QSet<int>> _faceNeighborRegion;
        /**
         * @brief  单元所属分区号（按索引排序）
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        std::vector<int> _cellPartitionInfo;
        /**
         * @brief  节点所属分区号（按索引排序）
         * @author wangning (2185896382@qq.com)
         * @date  2025-12-31
         */
        std::vector<int> _nodePartitionInfo;

        int  _partitionNum = -1; //分区数量
    };

}

#endif // __FITK_PARTITION_INTERFACE_H__
