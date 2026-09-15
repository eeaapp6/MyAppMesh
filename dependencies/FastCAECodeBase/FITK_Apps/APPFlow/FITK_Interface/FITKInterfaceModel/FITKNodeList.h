/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKNodeList.h
 * @brief 节点列表声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-28
 *
 */
#ifndef __FITKNODELIST_H__
#define __FITKNODELIST_H__

#include "FITKInterfaceModelAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include <QVector>
#include <QMap>
#include <QMutex>

namespace Core
{
    class FITKNode;
}

namespace Interface
{
    /**
     * @brief 节点列表
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-28
     */
    class FITKInterfaceModelAPI FITKNodeList //: public Core::FITKAbstractObject
    {
    public:
       /**
        * @brief Construct a new FITKNodeList object
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-28
        */
        explicit FITKNodeList() = default;
        /**
         * @brief Destroy the FITKNodeList object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual ~FITKNodeList();

        /**
         * @brief       追加节点。
         * @param[in]   节点
         * @return      节点ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-20
         */
        virtual int addNode(Core::FITKNode* node);

        /**
         * @brief 追加节点
         * @param[i]  x              x坐标
         * @param[i]  y              y坐标
         * @param[i]  z              z坐标
         * @param[i]  isNative       是否是与几何关联的网格划分节点
         * @param[i]  tag            几何标记
         * @return int     节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual int addNode(const double x, const double y, const double z, bool isNative = false, unsigned int tag = 0);
        /**
         * @brief 追加节点
         * @param[i]  id             节点ID
         * @param[i]  x              x坐标
         * @param[i]  y              y坐标
         * @param[i]  z              z坐标
         * @param[i]  isNative       是否是与几何关联的网格划分节点
         * @param[i]  tag            几何标记
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual void addNode(const int id, const double x, const double y, const double z, bool isNative = false, unsigned int tag = 0);
        /**
         * @brief 移除第index个节点，index从0开始
         * @param[i]  index          节点索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual void removeNodeAt(const int index);

        /**
         * @brief       根据节点索引列表移除节点。[虚函数]
         * @param[in]   indice：节点索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-06
         */
        virtual void removeReplceNodesAt(QList<int> indice);

        /**
         * @brief       根据节点索引列表移除节点。[虚函数]
         * @param[in]   indice：节点索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-06
         */
        virtual void removeNodesAt(QList<int> indice);

        /**
         * @brief 根据ID移除节点
         * @param[i]  id            节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        virtual void removeNodeByID(const int id);
        /**
         * @brief    根据Id移除多个节点
         * @param[i] ids                        节点id列表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-28
         */
        virtual void removeNodeByIDs(const QList<int>& ids);
        /**
         * @brief 获取节点数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        int getNodeCount();
        /**
         * @brief 获取第index个节点
         * @param[i]  index         节点索引
         * @return Core::FITKNode*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        Core::FITKNode* getNodeAt(const int index);

        /**
         * @brief       根据索引获取网格节点
         * @param[in]   index：节点索引
         * @return      网格节点对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-20
         */
        template <class T>
        T* getTNodeAt(const int index)
        {
            return dynamic_cast<T*>(getNodeAt(index));
        }

        /**
         * @brief 根据ID获取节点
         * @param[i]  id          节点ID
         * @return Core::FITKNode*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        Core::FITKNode* getNodeByID(const int id);

        /**
         * @brief       根据ID获取网格节点
         * @param[in]   id：节点ID
         * @return      网格节点对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-06-20
         */
        template <class T>
        T* getTNodeByID(const int id)
        {
            return dynamic_cast<T*>(getNodeByID(id));
        }

        /**
         * @brief 根据索引获取节点ID
         * @param[i]  index         索引
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
          int getNodeIDByIndex(const int index);
        /**
         * @brief 根据ID获取节点索引
         * @param[i]  id            节点ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
         int getNodeIndexByID(const int id);
        /**
         * @brief 获取节点最大id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-28
         */
        int getNodeMaxID();

        /**
         * @brief 移除全部节点
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-28
         */
        void removeAllNode();
        /**
         * @brief 获取节点边界框
         * @param[out]  bndBox      边界框数组，大小为6，依次为xmin, xmax, ymin, ymax, zmin, zmax
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-08-26
         */
        void getNodeBndBox(double *bndBox) const;

    protected:
        /**
         * @brief 更新ID与索引的映射，移除节点等过程调用
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-30
         */
        void updateNodeIDIndexMap();

        /**
         * @brief       根据索引移除节点。此方法不会更新节点最大ID，仅供内部使用
         * @param[in]   index：节点索引
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-07
         */
        void fastRemoveNodeAt(const int index);

        /**
         * @brief       根据ID移除节点。此方法不会更新节点最大ID，仅供内部使用
         * @param[in]   id：节点ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-07
         */
        void fastRemoveNodeByID(const int id);

    protected:
        /**
         * @brief 线程互斥锁
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        QMutex _mutex;
        /**
         * @brief 节点列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        QVector<Core::FITKNode*> _nodeList{};

        /**
         * @brief 辅助存储， ID与index映射关系
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-28
         */
        QMap<int, int> _idIndexMap{};
        /**
         * @brief 辅助存储， 最大节点ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        int _nodeMaxID = 0;
    };
}



#endif // !__FITKNODELIST_H__
