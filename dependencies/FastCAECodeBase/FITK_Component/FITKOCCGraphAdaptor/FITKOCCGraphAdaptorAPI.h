/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphAdaptorAPI.h
 * @brief       For export macro.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-05
 *********************************************************************/

#ifndef _FITKOCCGRAPHADAPTOR_API_H_
#define _FITKOCCGRAPHADAPTOR_API_H_

#include <QtCore/QtGlobal>

#include <QDebug>

#if defined(FITKOCCGRAPHADAPTOR_API)
#define FITKOCCGRAPHADAPTORAPI Q_DECL_EXPORT
#else
#define FITKOCCGRAPHADAPTORAPI Q_DECL_IMPORT
#endif

#ifndef FITKOCCGraphAdaptorVersion
#define FITKOCCGraphAdaptorVersion "1.0.0"
#endif

#endif // _FITKOCCGRAPHADAPTOR_API_H_