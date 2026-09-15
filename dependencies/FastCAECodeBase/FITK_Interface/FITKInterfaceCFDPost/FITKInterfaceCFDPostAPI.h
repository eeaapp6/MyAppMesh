/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKInterfaceCFDPostAPI.h
 * @brief  后处理数据接口
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKInterfaceCFDPostAPI_H_
#define _FITKInterfaceCFDPostAPI_H_

#include <QtCore/QtGlobal>


#if defined(FITKInterfaceCFDPost_API)
#define FITKInterfaceCFDPostAPI Q_DECL_EXPORT
#else
#define FITKInterfaceCFDPostAPI Q_DECL_IMPORT
#endif

#endif
