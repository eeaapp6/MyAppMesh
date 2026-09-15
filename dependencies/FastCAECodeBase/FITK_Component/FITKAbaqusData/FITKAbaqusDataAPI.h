/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAdaptorAPI.h
 * @brief 声明接口
 * @author fulipeng (flipengqd@yeah.net)
 * @date 2024-03-07
 *
 */

#ifndef _FITKABAQUSDATA_API_H_
#define _FITKABAQUSDATA_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKAbaqusData_API)
#define FITKAbaqusDataAPI Q_DECL_EXPORT
#else
#define FITKAbaqusDataAPI Q_DECL_IMPORT
#endif


#ifndef FITKAbaqusDataVersion
#define FITKAbaqusDataVersion "1.1.1"
#endif

#endif
