/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 /**
 *
 * @file FITKInterfaceModelAPI.h
 * @brief API接口声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-27
 *
 */
#ifndef _FITKINTERFACEMODEL_API_H_
#define _FITKINTERFACEMODEL_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceModel_API)
#define FITKInterfaceModelAPI Q_DECL_EXPORT
#else
#define FITKInterfaceModelAPI Q_DECL_IMPORT
#endif

#ifndef FITKInterfaceModelVersion
#define FITKInterfaceModelVersion "0.9.1"
#endif

#endif
