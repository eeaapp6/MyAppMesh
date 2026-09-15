/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceIOAPI.h
 * @brief  声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-08
 *
 */
#ifndef _FITKINTERFACE_IO_API_H_
#define _FITKINTERFACE_IO_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceIO_API)
#define FITKInterfaceIOAPI Q_DECL_EXPORT
#else
#define FITKInterfaceIOAPI Q_DECL_IMPORT
#endif

#ifndef FITKInterfaceIOVersion
#define FITKInterfaceIOVersion "1.0.1"
#endif

#endif
