/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKDynamicCase.h
 * @brief  锻造项目专用算例数据类
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2026-05-07
 **/
#ifndef FITKDYNAMICCASE_H
#define FITKDYNAMICCASE_H

#include "FITKDataCase.h"
#include "FITKAbaqusDataAPI.h"

namespace Forging 
{
    /**
     * @brief  锻造数据类（对原有算例类进行扩展）
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2026-05-07
     **/
    class FITKAbaqusDataAPI FITKDynamicCase : public AbaqusData::FITKDataCase 
    {
        Q_OBJECT
        FITKCLASS(Forging, FITKDynamicCase);

    public:
        explicit FITKDynamicCase();
        virtual ~FITKDynamicCase()   = default;




    };

} // namespace Forging



#endif // !FITKDYNAMICCASE_H