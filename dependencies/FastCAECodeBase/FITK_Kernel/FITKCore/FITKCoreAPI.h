/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKCoreAPI.h
 * @brief 导入导出库声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef _FITKCORE_API_H_
#define _FITKCORE_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKCore_API)
#define FITKCoreAPI Q_DECL_EXPORT
#else
#define FITKCoreAPI Q_DECL_IMPORT
#endif

#ifndef FITKCoreVersion
#define FITKCoreVersion "1.0.4"
#endif

#endif


