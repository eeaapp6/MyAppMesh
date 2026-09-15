/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceMeshAPI.h
 * @brief 声明接口宏
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 *
 */
#ifndef _FITKINTERFACEMESH_API_H_
#define _FITKINTERFACEMESH_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceMesh_API)
#define FITKInterfaceMeshAPI Q_DECL_EXPORT
#else
#define FITKInterfaceMeshAPI Q_DECL_IMPORT
#endif

#ifndef FITKInterfaceMeshVersion
#define FITKInterfaceMeshVersion "0.5.0"
#endif

#endif
