/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PythonInterfacePropertiesAPI.h
 * @brief PythonInterfaceProperties模块API导出宏
 * @date 2026-05-11
 */
#ifndef _PYTHONINTERFACEPROPERTIES_API_H_
#define _PYTHONINTERFACEPROPERTIES_API_H_

#include <QtCore/QtGlobal>

#if defined(PythonInterfaceProperties_EXPORTS)
#define PythonInterfacePropertiesAPI Q_DECL_EXPORT
#else
#define PythonInterfacePropertiesAPI Q_DECL_IMPORT
#endif

#endif
