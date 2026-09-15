/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

 /**
 * 
 * @file FITKInterfaceFlowPHengLEIAPI.h
 * @brief 声明导出库的宏
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-11
 * 
 */
#ifndef _FITKFlowPHengLEI_API_H_
#define _FITKFlowPHengLEI_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceFlowPHengLEI_API)
#define FITKFlowPHengLEIAPI Q_DECL_EXPORT
#else
#define FITKFlowPHengLEIAPI Q_DECL_IMPORT
#endif

#endif
