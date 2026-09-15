/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKInterfaceGeoIOHDF5API.h
 * @brief   HDF5 API.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-09
 *********************************************************************/

#ifndef _FITKINTERFACEGEOIOHDF5_API_H_
#define _FITKINTERFACEGEOIOHDF5_API_H_

#include <QtCore/QtGlobal>

#include <QDebug>

#if defined(FITKINTERFACEGEOIOHDF5_API)
#define FITKINTERFACEGEOIOHDF5API Q_DECL_EXPORT
#else
#define FITKINTERFACEGEOIOHDF5API Q_DECL_IMPORT
#endif

#ifndef FITKINTERFACEGEOIOHDF5VERSION
#define FITKINTERFACEGEOIOHDF5VERSION "1.0.0"
#endif

#endif // _FITKINTERFACEGEOIOHDF5_API_H_