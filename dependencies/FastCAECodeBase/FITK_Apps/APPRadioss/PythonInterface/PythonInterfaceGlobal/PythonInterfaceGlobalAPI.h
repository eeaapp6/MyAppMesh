/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PythonInterfaceGlobalAPI.h
 * @brief PythonInterfaceGlobal模块API导出宏
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2026-05-07
 */
#ifndef _PYTHONINTERFACEGLOBAL_API_H_
#define _PYTHONINTERFACEGLOBAL_API_H_

#include <QtCore/QtGlobal>

#if defined(PythonInterfaceGlobal_EXPORTS)
#define PythonInterfaceGlobalAPI Q_DECL_EXPORT
#else
#define PythonInterfaceGlobalAPI Q_DECL_IMPORT
#endif

#endif
