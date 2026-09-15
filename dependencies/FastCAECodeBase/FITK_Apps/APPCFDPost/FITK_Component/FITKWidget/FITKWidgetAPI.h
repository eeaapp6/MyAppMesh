/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
* @file   DrawerWidgetAPI.h
* @brief  自定义抽屉组件接口文件
* @author XuXinwei
* @version 1.0.0
*/
#ifndef _DRAWER_WIDGET_API_H_
#define _DRAWER_WIDGET_API_H_

#include <QtCore/qglobal.h>

#if defined(FITKWidget_API)
#  define FITKWidgetAPI Q_DECL_EXPORT
#else
#  define FITKWidgetAPI Q_DECL_IMPORT
#endif

#ifndef FITKWIDGETVERSION
#define FITKWIDGETVERSION "1.0.1"
#endif

#endif // _DRAWER_WIDGET_API_H_