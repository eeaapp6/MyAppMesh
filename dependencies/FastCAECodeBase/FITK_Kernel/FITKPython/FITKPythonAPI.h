/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKPythonAPI.h
 * @brief 导入导出库声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-02-26
 * 
 */
#ifndef _FITKPYTHON_API_H_
#define _FITKPYTHON_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKPython_API)
#define FITKPythonAPI Q_DECL_EXPORT
#else
#define FITKPythonAPI Q_DECL_IMPORT
#endif

#ifndef FITKPythonVersion
#define FITKPythonVersion "1.0.0"   
#endif

#endif
