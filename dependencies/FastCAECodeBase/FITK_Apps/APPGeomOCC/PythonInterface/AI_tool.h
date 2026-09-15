/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file AI_tool.h
 * @brief AI工具辅助类
 * @author wangning (2185896382@qq.com)
 * @date 2026-06-08
 */
#ifndef __AI_tool_H___
#define __AI_tool_H___

#include "PythonInterfaceAPI.h"
#include <QObject>
#include <QString>

/**
 * @brief AI工具类
 * @author GitHub Copilot
 * @date 2026-06-08
 */
class PythonInterfaceAPI AI_tool
{
public:
    explicit AI_tool() = default;
    virtual ~AI_tool() = default;

    //获取几何是否存在
    QString isGeometryExist(const QString& name);

     /**
     * @brief 获取几何类型
     * @param[i] name 几何名称
     * @return 几何类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-20
     */
    QString getGeometryType(const QString& name);

};

/**
 * @brief AI_tool 的 PythonQt 装饰器
 * @author GitHub Copilot
 * @date 2026-06-08
 */
class PythonInterfaceAPI AI_toolWrapper : public QObject
{
    Q_OBJECT
public slots:
    /**
     * @brief 创建 AI_tool 对象
     * @return AI_tool*
     */
    AI_tool* new_AI_tool();

    /**
     * @brief 删除 AI_tool 对象
     * @param[i] obj AI_tool 对象指针
     */
    void delete_AI_tool(AI_tool* obj);

    /**
     * @brief 判断几何是否存在
     * @param[i] obj AI_tool 对象指针
     * @param[i] name 几何名称
     * @return 判断结果
     */
    QString isGeometryExist(AI_tool* obj, const QString& name);

    /**
     * @brief 获取几何类型
     * @param[i] obj AI_tool 对象指针
     * @param[i] name 几何名称
     * @return 几何类型描述
     */
    QString getGeometryType(AI_tool* obj, const QString& name);
};

#endif