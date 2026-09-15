/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKLineMeshingParameter.h
 * @brief       线单元划分参数。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-11-04
 *********************************************************************/

#ifndef __FITKLINEMESHINGPARAMETER_H__
#define __FITKLINEMESHINGPARAMETER_H__

#include "FITKAbaqusDataAPI.h"
#include "FITK_Interface/FITKInterfaceStructural/FITKAbaMeshPartVTK.h"

namespace AbaqusData
{
    /**
     * @brief       线单元划分参数。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-11-04
     */
    class FITKAbaqusDataAPI FITKLineMeshingParameter : public Core::FITKAbstractDataObject
    {
        Q_OBJECT;

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        explicit FITKLineMeshingParameter() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        virtual ~FITKLineMeshingParameter() = default;

        /**
         * @brief       设置边ID。
         * @param[in]   modelID：边ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void setModelID(int modelID);

        /**
         * @brief       获取边ID。
         * @return      边ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
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
         * @brief       设置三角形网格划分单元类型。
         * @param[in]   type：三角形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void setLineElementType(Interface::FITKAbaMeshPartVTK::AbaEleType type);

        /**
         * @brief       获取三角形网格划分单元类型。
         * @return      三角形单元类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType getLineElementType();

    protected:
        /**
         * @brief       几何ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        int _modelID = -1;
        /**
         * @brief       高阶标记。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-05
         */
        bool _isQuadratic = false;
        /**
         * @brief       指定的线单元类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        Interface::FITKAbaMeshPartVTK::AbaEleType _lineEType = Interface::FITKAbaMeshPartVTK::B31;

    };
}

#endif // !__FITKLINEMESHINGPARAMETER_H__
