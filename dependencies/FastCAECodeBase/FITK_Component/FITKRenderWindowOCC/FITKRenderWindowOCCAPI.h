/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKRenderWindowOCCAPI.h
 * @brief   For export macro.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-11
 *********************************************************************/

#ifndef _FITKRENDERWINDOWOCC_API_H_
#define _FITKRENDERWINDOWOCC_API_H_

#include <QtCore/QtGlobal>

#include <QDebug>

#if defined(FITKRENDERWINDOWOCC_API)
#define FITKRENDERWINDOWOCCAPI Q_DECL_EXPORT
#else
#define FITKRENDERWINDOWOCCAPI Q_DECL_IMPORT
#endif

// Library version information
#ifndef FITKRENDERWINDOWOCC_VERSION
#define FITKRENDERWINDOWOCC_VERSION "1.0.0"
#endif

#endif
