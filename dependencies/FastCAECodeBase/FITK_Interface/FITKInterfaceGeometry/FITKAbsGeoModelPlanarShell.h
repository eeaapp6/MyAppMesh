/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelPlanarShell.h
 * @brief       从草图创建平面壳模型几何命令抽象类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-06-25
 *********************************************************************/

#ifndef __FITKABSGEOMODELPLANARSHELL_H__
#define __FITKABSGEOMODELPLANARSHELL_H__

#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"

namespace Interface 
{
    /**
     * @brief       从草图创建平面壳模型几何命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-06-25
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPlanarShell :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelPlanarShell);
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-25
         */
        FITKAbsGeoModelPlanarShell() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-25
         */
        virtual ~FITKAbsGeoModelPlanarShell() = default;

        /**
         * @brief       获取几何命令类型。
         * @return      几何命令类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-25
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置草图数据ID。
         * @param[in]   sketchDataID：草图数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-25
         */
        void setSketchDataID(int sketchDataID);

        /**
         * @brief       获取草图数据ID。
         * @return      草图数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-25
         */
        int getSketchDataID() const;

    protected:
        /**
         * @brief       草图数据ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-25
         */
        int m_sketchDataID = -1;

    };
}

#endif // __FITKABSGEOMODELPLANARSHELL_H__
