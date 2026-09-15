/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 /**
 *
 * @file FITKInterfacePhysicsAPI.h
 * @brief  声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-29
 *
 */
#ifndef _FITKINTERFACEPHYSICS_API_H_
#define _FITKINTERFACEPHYSICS_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfacePhysics_API)
#define FITKInterfacePhysicsAPI Q_DECL_EXPORT
#else
#define FITKInterfacePhysicsAPI Q_DECL_IMPORT
#endif

#ifndef FITKInterfacePhysicsVersion
#define FITKInterfacePhysicsVersion "0.2.0"
#endif

#endif
