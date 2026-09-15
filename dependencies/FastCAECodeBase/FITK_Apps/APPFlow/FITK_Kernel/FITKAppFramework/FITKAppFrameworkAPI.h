/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAppFrameworkAPI.h
 * @brief 声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef _FITKAPPFRAMEWORK_API_H_
#define _FITKAPPFRAMEWORK_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKAppFramework_API)
#define FITKAppFrameworkAPI Q_DECL_EXPORT
#else
#define FITKAppFrameworkAPI Q_DECL_IMPORT
#endif

#ifndef FITKAppFrameworkVersion
#define FITKAppFrameworkVersion "1.0.7"
#endif

#endif
