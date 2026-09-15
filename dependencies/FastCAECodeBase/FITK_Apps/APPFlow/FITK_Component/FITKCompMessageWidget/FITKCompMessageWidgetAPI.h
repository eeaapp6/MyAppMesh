/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKCompMessageWidgetAPI.h
 * @brief 导入导出库声明 
 * @author BaGuijun (baguijun@163.com)
 * @date 2024-03-05
 * 
 */
#ifndef _FITKCompMessageWidgetAPI_H_
#define _FITKCompMessageWidgetAPI_H_

#include <QtCore/QtGlobal>

#if defined(FITKCompMessageWidget_API)
#define CompMesWidgetAPI Q_DECL_EXPORT
#else
#define CompMesWidgetAPI Q_DECL_IMPORT
#endif

#ifndef FITKCOMPMESSAGEWIDGETVERSION
#define FITKCOMPMESSAGEWIDGETVERSION "1.0.0"
#endif

#endif
