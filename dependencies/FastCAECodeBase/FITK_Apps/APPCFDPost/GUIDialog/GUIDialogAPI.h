/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file GUIDialogAPI.h
 * @brief 求解参数对话框接口声明
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-04-19
 *
 */
#ifndef __GUIDialogAPI_H
#define __GUIDialogAPI_H

#include <QtCore/QtGlobal>

#if defined( GUIDialog_API )
#    define GUIDialogAPI Q_DECL_EXPORT
#else
#    define GUIDialogAPI Q_DECL_IMPORT
#endif

#endif // __GUIWIDGETAPI_H__
