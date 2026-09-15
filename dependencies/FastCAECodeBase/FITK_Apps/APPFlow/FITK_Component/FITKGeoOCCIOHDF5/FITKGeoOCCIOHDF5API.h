/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKGeoOCCIOHDF5API.h
 * @brief   For export macro.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-09
 *********************************************************************/

#ifndef _FITKGEOOCCIOHDF5_API_H_
#define _FITKGEOOCCIOHDF5_API_H_

#include <QtCore/QtGlobal>

#include <QDebug>

#if defined(FITKGEOOCCIOHDF5_API)
#define FITKGEOOCCIOHDF5API Q_DECL_EXPORT
#else
#define FITKGEOOCCIOHDF5API Q_DECL_IMPORT
#endif

#ifndef FITKGEOOCCIOHDF5VERSION
#define FITKGEOOCCIOHDF5VERSION "1.1.0"
#endif
#endif // _FITKGEOOCCIOHDF5_API_H_