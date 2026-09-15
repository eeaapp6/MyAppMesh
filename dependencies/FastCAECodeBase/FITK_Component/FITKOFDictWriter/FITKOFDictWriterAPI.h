/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKOFDictWriterAPI.h
 * @brief 声明写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date 2024-07-16
 */
#ifndef _FITKOFDICTWRITER_API_H_
#define _FITKOFDICTWRITER_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKOFDictWriter_API)
#define FITKOFDictWriterAPI Q_DECL_EXPORT
#else
#define FITKOFDictWriterAPI Q_DECL_IMPORT
#endif

#endif
