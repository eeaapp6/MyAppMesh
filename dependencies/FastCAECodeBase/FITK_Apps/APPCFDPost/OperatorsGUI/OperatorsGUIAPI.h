/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief  接口声明
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 */
#ifndef _OperatorsGUIAPI_H_
#define _OperatorsGUIAPI_H_

#include <QtCore/QtGlobal>

#ifdef OperatorsGUI_API
#define OperatorsGUIAPI Q_DECL_EXPORT
#else
#define OperatorsGUIAPI Q_DECL_IMPORT
#endif
#endif
