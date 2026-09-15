/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAdaptorAPI.h
 * @brief 声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-07
 *
 */
#ifndef _FITKADAPTOR_API_H_
#define _FITKADAPTOR_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKAdaptor_API)
#define FITKAdaptorAPI Q_DECL_EXPORT
#else
#define FITKAdaptorAPI Q_DECL_IMPORT
#endif


#ifndef FITKAdaptorVersion
#define FITKAdaptorVersion "0.5.0"
#endif

#endif
