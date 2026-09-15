/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PythonInterfaceCommons.h
 * @brief  PythonInterface公有类
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-06-17
 *********************************************************************/
#ifndef __PythonInterfaceCommons_H___
#define __PythonInterfaceCommons_H___

#include <QString>

#include "PythonInterfaceAPI.h"

/**
 * @brief  PythonInterface公有类
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-06-17
 */
class PythonInterfaceAPI PythonInterfaceCommons
{
public:
    /**
     * @brief    发送信息至HttpServer
     * @param[i] message                    信息
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-06-17
     */
    static void sendMessageToHttpServer(const QString& message);
};

#endif