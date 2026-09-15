/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceMeshGenAPI.h
 * @brief  定义接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-17
 *
 */
#ifndef _FITKInterfaceMeshGen_API_API_H_
#define _FITKInterfaceMeshGen_API_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceMeshGen_API)
#define FITKInterfaceMeshGenAPI Q_DECL_EXPORT
#else
#define FITKInterfaceMeshGenAPI Q_DECL_IMPORT
#endif


#ifndef FITKInterfaceMeshGenVersion
#define FITKInterfaceMeshGenVersion "1.0.0"
#endif

#endif
