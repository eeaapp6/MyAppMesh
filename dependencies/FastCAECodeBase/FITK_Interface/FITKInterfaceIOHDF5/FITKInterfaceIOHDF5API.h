/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKInterfaceIOHDF5API.h
 * @brief  声明接口
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-08
 *
 */
#ifndef _FITKINTERFACEIO_HDF5_API_H_
#define _FITKINTERFACEIO_HDF5_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceIOHDF5_API)
#define FITKINTERFACEHDF5IOAPI Q_DECL_EXPORT
#else
#define FITKINTERFACEHDF5IOAPI Q_DECL_IMPORT
#endif


#ifndef FITKInterfaceIOHDF5Version
#define FITKInterfaceIOHDF5Version "1.0.0"
#endif

#endif
