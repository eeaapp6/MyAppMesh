/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRenderWindowVTKAPI.h
 * @brief  导入导出接口声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-08
 * 
 */
#ifndef _FITKRenderWindowVTKAPI_H_
#define _FITKRenderWindowVTKAPI_H_

#include <QtCore/QtGlobal>

#if defined(FITKRenderWindowVTK_API)
#define FITKRenderWindowVTKAPI Q_DECL_EXPORT
#else
#define FITKRenderWindowVTKAPI Q_DECL_IMPORT
#endif

#ifndef FITKRENDERWINDOWVTKVERSION
#define FITKRENDERWINDOWVTKVERSION "1.0.0"  
#endif

#endif // _FITKRenderWindowVTKAPI_H_
