/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file    FITKOFDriverAPI.h
 * @brief   声明openFOAM网格划分驱动
 * @author  fulipeng (fulipengqd@yeah.net)
 * @date    2024-06-20
 *
 */
#ifndef FITKOFDRIVER_API_H
#define FITKOFDRIVER_API_H

#include <QtCore/QtGlobal>

#if defined(FITKOFDRIVER_API)
    #define FITKOFDRIVERAPI Q_DECL_EXPORT
#else
    #define FITKOFDRIVERAPI Q_DECL_IMPORT
#endif

#endif
