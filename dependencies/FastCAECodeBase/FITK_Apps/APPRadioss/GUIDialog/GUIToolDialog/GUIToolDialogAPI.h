/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIToolDialogAPI.h
 * @brief  外部宏接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-22
 *********************************************************************/
#ifndef _GUITOOLDIALOG_API_H__
#define _GUITOOLDIALOG_API_H__

#include <QtCore/QtGlobal>

#ifdef  GUIToolDialog_API
#define GUIToolDialogAPI Q_DECL_EXPORT
#else
#define GUIToolDialogAPI Q_DECL_IMPORT
#endif

#endif
