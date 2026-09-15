/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshExeDriverAPI.h
 * @brief  外部接口声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-27
 *********************************************************************/
#ifndef _FITKGmshExeDriver_API_H_
#define _FITKGmshExeDriver_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKGmshExeDriver_API)
#define FITKGmshExeDriverAPI Q_DECL_EXPORT
#else
#define FITKGmshExeDriverAPI Q_DECL_IMPORT
#endif

#ifndef FITKGMSHEXEDRIVERVERSION
#define FITKGMSHEXEDRIVERVERSION "2.0.0"
#endif

#endif // _FITKGmshExeDriver_API_H_
