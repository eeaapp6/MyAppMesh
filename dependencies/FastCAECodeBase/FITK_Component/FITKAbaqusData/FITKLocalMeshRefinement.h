/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKLocalMeshRefinement.h
 * @brief       工作流局部网格加密条目（持久化于 Part 网格参数）
 *********************************************************************/
#ifndef __FITKLOCALMESHREFINEMENT__H__
#define __FITKLOCALMESHREFINEMENT__H__

#include "FITKAbaqusDataAPI.h"

#include <QString>
#include <QVector>

namespace AbaqusData
{
    /** @brief 局部加密 Field 类型：0=Box, 1=Cylinder, 2=Sphere */
    struct FITKLocalMeshRefinement
    {
        QString name;
        int method = 0;
        QString regionSpec;
        double sizeIn = 1.0;
        double thickness = 0.2;
        double sizeOut = 8.0;
    };

    using FITKLocalMeshRefinementList = QVector<FITKLocalMeshRefinement>;
}

#endif
