/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIPostDialogAPIAPI.h
 * @brief  后处理相关窗口外部接口声明
 * @author XingXuanCi (xingxuanci@diso.cn)
 * @date   2024-11-22
 *********************************************************************/
#ifndef __GUIPostDialogAPIAPI_H_
#define __GUIPostDialogAPIAPI_H_

#include <QtCore/QtGlobal>

#if defined( GUIPostDialog_API )
#    define GUIPostDialogAPI Q_DECL_EXPORT
#else
#    define GUIPostDialogAPI Q_DECL_IMPORT
#endif

#endif // __GUIPostDialogAPIAPI_H_
