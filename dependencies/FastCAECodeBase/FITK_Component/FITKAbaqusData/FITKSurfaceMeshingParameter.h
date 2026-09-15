/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKSurfaceMeshingParameter.h
 * @brief 面网格划分参数
 * @author fulipeng (fulipengqd@yeah.net)
 * @date 2024-12-17
 */
#ifndef _FITKSURFACE_MESHING_PARAMETR__H___
#define _FITKSURFACE_MESHING_PARAMETR__H___

#include "FITKAbaqusDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceStructural/FITKAbaMeshPartVTK.h"

#include <QObject>
#include <QMetaEnum>

namespace AbaqusData
{
    /**
     * @brief 面网格划分参数抽象类
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-12-17
     */
    class FITKAbaqusDataAPI FITKSurfaceMeshingParameter : public Core::FITKAbstractDataObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief   Construct a new FITKSurfaceMeshingParameter object
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        explicit FITKSurfaceMeshingParameter() = default;
        /**
         * @brief   Destroy the FITKSurfaceMeshingParameter object
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-12-17
         */
        virtual ~FITKSurfaceMeshingParameter() = default;
        /**
         * @brief    设置模型ID
         * @param[i] modelID                    模型ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-02
         */
        void setModelID(int modelID);
        /**
         * @brief    获取模型ID
         * @return   int                        模型ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-07-02
         */
        int getModelID();
        /**
         * @brief       设置高阶标记。
         * @param[in]   flag：是否划分高阶单元
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        void setQuadraticFlag(bool flag);

        /**
         * @brief       获取高阶标记。
         * @return      是否划分高阶单元
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        bool getQuadraticFlag();
        /**
         * @brief       设置四边形网格划分单元类型。
         * @param[in]   type：四边形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void setQuadElementType(Interface::FITKAbaMeshPartVTK::AbaEleType type);

        /**
         * @brief       获取四边形网格划分单元类型。
         * @return      四边形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType getQuadElementType();

        /**
         * @brief       设置三角形网格划分单元类型。
         * @param[in]   type：三角形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void setTriElementType(Interface::FITKAbaMeshPartVTK::AbaEleType type);

        /**
         * @brief       获取三角形网格划分单元类型。
         * @return      三角形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType getTriElementType();

    protected:
        /**
         * @brief  模型ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-07-02
         */
        int _modelID = -1;
        /**
         * @brief       高阶标记。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        bool _isQuadratic = false;
        /**
         * @brief       指定的四边形单元类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType _quadEType = Interface::FITKAbaMeshPartVTK::S4R;

        /**
         * @brief       指定的三角形单元类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType _triEType = Interface::FITKAbaMeshPartVTK::S3;

    };
}
#endif
