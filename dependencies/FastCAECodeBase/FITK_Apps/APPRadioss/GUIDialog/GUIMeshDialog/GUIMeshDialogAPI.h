/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIMeshDialogAPI.h
 * @brief  宏接口
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-07-22
 *********************************************************************/
#ifndef _GUIMeshDialog_H__
#define _GUIMeshDialog_H__

#include <QtCore/QtGlobal>

#ifdef GUIMeshDialog_API
#define GUIMeshDialogAPI Q_DECL_EXPORT
#else
#define GUIMeshDialogAPI Q_DECL_IMPORT
#endif

#endif
