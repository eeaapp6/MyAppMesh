/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file AI_Properties.h
 * @brief AI工具封装类，组合材料与属性基础能力
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-18
 */
#ifndef _AI_PROPERTIES_H___
#define _AI_PROPERTIES_H___

#include <QObject>
#include <QString>
#include "PythonInterfacePropertiesAPI.h"

/**
 * @brief AI工具封装类，组合材料与属性基础能力
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-18
 */
class PythonInterfacePropertiesAPI AI_Properties
{
public:
    explicit AI_Properties() = default;
    virtual ~AI_Properties() = default;

    /**
     * @brief 获取材料类型
     * @param[i] name 材料名称
     * @return 材料类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-18
     */
    QString getMaterialType(const QString& name);

    /**
     * @brief 获取属性类型
     * @param[i] name 属性名称
     * @return 属性类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-18
     */
    QString getPropertyType(const QString& name);

    /**
     * @brief 获取失效模型类型
     * @param[i] name 失效模型名称
     * @return 失效模型类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getFailureModelType(const QString& name);

    /**
     * @brief 获取状态方程类型
     * @param[i] name 状态方程名称
     * @return 状态方程类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getEquationOfStateType(const QString& name);
};

/**
 * @brief AI_Properties装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-18
 */
class PythonInterfacePropertiesAPI AI_PropertiesWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 创建AI_Properties对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-18
     */
    AI_Properties* new_AI_Properties();

    /**
     * @brief 删除AI_Properties对象
     * @param[in] obj AI_Properties对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-18
     */
    void delete_AI_Properties(AI_Properties* obj);

    /**
     * @brief 获取材料类型
     * @param[in] obj AI_Properties对象
     * @param[in] name 材料名称
     * @return 材料类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-18
     */
    QString getMaterialType(AI_Properties* obj, const QString& name);

    /**
     * @brief 获取属性类型
     * @param[in] obj AI_Properties对象
     * @param[in] name 属性名称
     * @return 属性类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-18
     */
    QString getPropertyType(AI_Properties* obj, const QString& name);

    /**
     * @brief 获取失效模型类型
     * @param[in] obj AI_Properties对象
     * @param[in] name 失效模型名称
     * @return 失效模型类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getFailureModelType(AI_Properties* obj, const QString& name);

    /**
     * @brief 获取状态方程类型
     * @param[in] obj AI_Properties对象
     * @param[in] name 状态方程名称
     * @return 状态方程类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getEquationOfStateType(AI_Properties* obj, const QString& name);
};

#endif
