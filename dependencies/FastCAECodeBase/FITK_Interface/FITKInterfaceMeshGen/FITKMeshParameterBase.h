/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshParameterBase.h
 * @brief  网格划分参数基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-01-25
 *********************************************************************/
#ifndef _FITKMeshParameterBase_H_
#define _FITKMeshParameterBase_H_

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface
{
    /**
     * @brief  网格参数类型
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-25
     */
    enum class FITKMeshParamType
    {
        FITK_Mesh_None,               //空
        FITK_Mesh_Edge,               //边
        FITK_Mesh_Surface,            //面
        FITK_Mesh_Volume              //体
    };
    /**
     * @brief  网格划分参数基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-25
     */
    class FITKInterfaceMeshGenAPI FITKMeshParameterBase : public Core::FITKAbstractDataObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-25
         */
        explicit FITKMeshParameterBase() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-25
         */
        virtual ~FITKMeshParameterBase() = default;
        /**
         * @brief    获取网格参数类型
         * @return   FITKMeshParamType          网格参数类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-25
         */
        virtual FITKMeshParamType getMeshParamType() = 0;
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
    protected:
        /**
         * @brief       几何ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        int _modelID = -1;
    };
}

#endif