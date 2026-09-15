/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file   FITKVTKAlgorithmAPI.h
 * @brief  For export macro.
 *  
 * @author ChengHaotian (yeguangbaozi@foxmail.com)
 * @date   2024-03-21
 *********************************************************************/

#ifndef _FITKVTKALGORITHM_API_H_
#define _FITKVTKALGORITHM_API_H_

#include <QtCore/QtGlobal>

#include <QDebug>

#if defined(FITKVTKALGORITHM_API)
#define FITKVTKALGORITHMAPI Q_DECL_EXPORT
#else
#define FITKVTKALGORITHMAPI Q_DECL_IMPORT
#endif

#ifndef FITKVTKAlgorithmVersion
#define FITKVTKAlgorithmVersion "0.8.0"
#endif 

#endif
