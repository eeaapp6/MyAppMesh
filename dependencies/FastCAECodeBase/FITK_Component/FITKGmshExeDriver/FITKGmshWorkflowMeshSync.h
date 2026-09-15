/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshWorkflowMeshSync.h
 * @brief  工作流网格参数 ↔ GmshExec 同步（无 UI）
 *********************************************************************/
#ifndef FITKGMSHWORKFLOWMESHSYNC_H
#define FITKGMSHWORKFLOWMESHSYNC_H

#include "FITKGmshExeDriverAPI.h"

#include "FITK_Component/FITKAbaqusData/FITKGlobalMeshingParameter.h"

namespace GmshExe
{
    class FITKGmshExeDriverAPI FITKGmshWorkflowMeshSync
    {
    public:
        static void deriveAlgorithmsFromElementType(AbaqusData::FITKGlobalMeshingParameter *gp, int meshDim);

        /** 将 Part 网格参数写入 GmshExec（含 Region Field） */
        static bool applyPartParamsToGmshExec(AbaqusData::FITKGlobalMeshingParameter *gp, int meshDim,
            QString *errOut = nullptr);

        /** 解析 Region 字符串；method: 0=Box,1=Cylinder,2=Sphere */
        static bool parseRegionSpec(int method, const QString &spec, QString *errOut = nullptr);
    };
}

#endif
