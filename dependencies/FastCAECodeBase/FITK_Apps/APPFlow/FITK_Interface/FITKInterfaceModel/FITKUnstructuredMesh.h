/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKUnstructuredMesh.h
 * @brief 非结构网格声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef __FITKABSTRUCTUNSTRUCTED_MESH_H__
#define __FITKABSTRUCTUNSTRUCTED_MESH_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKNodeList.h"
#include "FITKElementList.h"
#include "FITKAbstractMesh.h"

namespace Interface
{
    class FITKUnstructuredMeshTopo;
    class FITKGeoToMeshMapper;

    /**
     * @brief 非结构化网格
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-07
     */
    class FITKInterfaceModelAPI FITKUnstructuredMesh
        : public FITKAbstractMesh, public FITKNodeList, public FITKElementList
    {
    public:
        /**
         * @brief Construct a new FITKUnstructuredMesh object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        explicit FITKUnstructuredMesh();
        /**
         * @brief Destroy the FITKUnstructuredMesh object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual ~FITKUnstructuredMesh();
        /**
         * @brief 获取模型类型
         * @return AbsModelType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual FITKModelEnum::AbsModelType getAbsModelType() override;

        /**
         * @brief       判断模型组件是否有效。[虚函数][重写]
         * @param[in]   compType：组件类型
         * @param[in]   members：组件成员ID
         * @return      是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-10-11
         */
        virtual bool isComponentValid(FITKModelEnum::FITKModelSetType compType, const QList<int> & members) override;

        /**
         * @brief 更新数据
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual void update() override;

        /**
         * @brief       清除网格数据。[虚函数][重写]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-07
         */
        virtual void clearMesh() override;

        /**
         * @brief 获取网格维度
         * @return FITKModelEnum::FITKMeshDim
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-03
         */
        virtual FITKModelEnum::FITKMeshDim getMeshDim() override;
         /**
         * @brief 通过位运算获取网格维度
         * @return unsigned int 返回网格维度的位表示，参考 FITKModelEnum::FITKMeshDim 
         * @author LiBaojun
         * @date 2025-04-11
         */
        unsigned int getMeshDimBit() override;

        
        /**
         * @brief 评判网格质量
         * @param[i] eleIndex 单元索引
         * @return FITKElemntQuality
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-23
         */
        FITKElemntQuality checkElementQuality(const int eleIndex);

        /**
         * @brief 获取单元方向，支持线单元与壳单元
         * @param[o] dir 单元方向
         * @param[i] id  单元ID
         * @return bool 是否获取成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-09
         */
        bool getElementDirection(double* dir , const int id);

        /**
         * @brief       获取网格内是否包含独立网格节点或单元。
         * @return      是否包含
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-23
         */
        bool hasOrphanMesh();

        /**
         * @brief       获取网格内是否包含几何关联网格节点或单元。
         * @return      是否包含
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-23
         */
        bool hasNativeMesh();

        /**
         * @brief       清空关联网格。（存在几何映射关系）[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        virtual void clearNativeMesh();

        /**
         * @brief       清空独立网格。（不存在几何映射关系）[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-27
         */
        virtual void clearOrphanMesh();
        /**
         * @brief  构建非结构化网格拓扑关系
         * @return 是否构建成功
         * @author libaojun
         * @date   2025-09-05
         */
        virtual bool buildUnstructuredMeshTopo();
        /**
         * @brief 获取非结构化网格拓扑
         * @return FITKUnstructuredMeshTopo*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-09-02
         */
        FITKUnstructuredMeshTopo* getUnstructuredMeshTopo();

        /**
         * @brief       获取网格点坐标。[虚函数][重写]
         * @param[in]   pointID：节点拓扑ID
         * @param[out]  coor：坐标
         * @param[in]   modelIndex：模型索引（未使用）[缺省]
         * @return      该节点是否有效
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-09-30
         */
        virtual bool getPointCoor(int pointID, double* coor, int modelIndex = 0) override;
        /**
         * @brief    获取几何到网格映射
         * @return   FITKGeoToMeshMapper*       映射
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-28
         */
        FITKGeoToMeshMapper* getGeoToMeshMapper();
    private:
        /**
         * @brief 计算边的长度
         * @param[i] edge 边的节点ID
         * @return double
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-23
         */
        double calMeshEdgeLength(const QList<int> & edge);
        /**
         * @brief 获取线单元方向
         * @param[o] dir 单元方向
         * @param[i] ele  单元对象
         * @return bool 是否获取成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-09
         */
        bool getLineEleDirection(double* dir, FITKAbstractElement* ele);
        /**
         * @brief 获取壳单元方向
         * @param[o] dir 单元方向
         * @param[i] ele  单元对象指针
         * @return bool 是否获取成功
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-09
         */
        bool getShellEleDirection(double* dir, FITKAbstractElement* ele);
    protected:
        /**
         * @brief  几何体到网格映射
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-08-28
         */
        FITKGeoToMeshMapper* _geoToMeshMapper = nullptr;
    private:
        /**
         * @brief Unstructured mesh topology
         */
        FITKUnstructuredMeshTopo* _meshTopo{nullptr};

    };
}



#endif
