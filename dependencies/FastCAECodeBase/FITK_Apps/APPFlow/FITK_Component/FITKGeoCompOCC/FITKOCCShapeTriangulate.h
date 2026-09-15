/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKOCCShapeTriangulate.h
 * @brief  OCC模型三角化声明
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-08-10
 *
 */
#ifndef _FITK_OCCSHAPE_TRIANGLE_H___
#define _FITK_OCCSHAPE_TRIANGLE_H___

class TopoDS_Shape;

namespace Interface
{
    class FITKVirtualTopoManager;
    class FITKGeometryMeshVS;
}

namespace OCC
{
    class FITKAbstractOCCModel;
    /**
     * @brief occ模型三角化
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-08-10
     */
    class FITKOCCShapeTriangulate
    {
    public:
        /**
         * @brief Construct a new FITKOCCShapeTriangulate object
         * @param[i]  occModel       occ模型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-10
         */
        explicit FITKOCCShapeTriangulate(FITKAbstractOCCModel* occModel);
        /**
         * @brief Destroy the FITKOCCShapeTriangulate object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-10
         */
        virtual ~FITKOCCShapeTriangulate() = default;
        /**
         * @brief 三角化
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-10
         */
        void triangulate();

    private:
        /**
         * @brief 离散点模型
         * @param[i]  id             虚拟拓扑对象id
         * @param[i]  shape          点模型
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-10
         */
        void discretePoint(const int id, const TopoDS_Shape& shape);
        /**
        * @brief 离散边
        * @param[i]  id             虚拟拓扑对象id
        * @param[i]  shape          边模型
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-10
        */
        void discreteEdge(const int id, const TopoDS_Shape& shape);
        /**
        * @brief 离散面模型
        * @param[i]  id             虚拟拓扑对象id
        * @param[i]  shape          面模型
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-08-10
        */
        void discreteFace(const int id, const TopoDS_Shape& shape);
        /**
         * @brief 三角化边和面
         * @param[i]  shape          尺寸形状
         * @param[i]  factor         容差
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-10
         */
        void triangulate(const TopoDS_Shape & shape, double factor = 0.001);

    private:
        /**
         * @brief       OCC模型数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-13
         */
        FITKAbstractOCCModel* _occModel{ nullptr };

        /**
         * @brief 拓扑管理器
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-10
         */
        Interface::FITKVirtualTopoManager* _vtmanager{};
        /**
         * @brief 可视化数据对象，存储三角形
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-08-10
         */
        Interface::FITKGeometryMeshVS* _meshVS{};
    };
}
#endif
