/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFIOHDF5API.h
 * @brief  声明外部接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-07
 *********************************************************************/

#ifndef _FITKFLOWOFIOHDF5_API_H_
#define _FITKFLOWOFIOHDF5_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKFLOWOFIOHDF5_API)
#define FITKFLOWOFIOHDF5API Q_DECL_EXPORT
#else
#define FITKFLOWOFIOHDF5API Q_DECL_IMPORT
#endif

#endif
