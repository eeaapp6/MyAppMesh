/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file AI_Solution.h
 * @brief AI工具封装类，组合求解设置基础能力
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-20
 */
#ifndef _AI_SOLUTION_H___
#define _AI_SOLUTION_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceSolutionAPI.h"

/**
 * @brief AI工具封装类，组合求解设置基础能力
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-20
 */
class PythonInterfaceSolutionAPI AI_Solution
{
public:
    explicit AI_Solution() = default;
    virtual ~AI_Solution() = default;

    /**
     * @brief 获取刚性墙类型
     * @param[i] name 刚性墙名称
     * @return 刚性墙类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-20
     */
    QString getRWallType(const QString& name);

    /**
     * @brief 获取初速度场类型
     * @param[i] name 初速度场名称
     * @return 初速度场类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString getInitialValueType(const QString& name);

    /**
     * @brief 获取相互作用类型
     * @param[i] name 相互作用名称
     * @return 相互作用类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getInteractionType(const QString& name);

    /**
     * @brief 获取探针类型
     * @param[i] name 探针名称
     * @return 探针类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getProbeType(const QString& name);

    /**
     * @brief 获取边界条件类型
     * @param[i] name 边界条件名称
     * @return 边界条件类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getBCSType(const QString& name);
};

/**
 * @brief AI_Solution装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-20
 */
class PythonInterfaceSolutionAPI AI_SolutionWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 创建AI_Solution对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-20
     */
    AI_Solution* new_AI_Solution();

    /**
     * @brief 删除AI_Solution对象
     * @param[in] obj AI_Solution对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-20
     */
    void delete_AI_Solution(AI_Solution* obj);

    /**
     * @brief 获取刚性墙类型
     * @param[in] obj AI_Solution对象
     * @param[in] name 刚性墙名称
     * @return 刚性墙类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-20
     */
    QString getRWallType(AI_Solution* obj, const QString& name);

    /**
     * @brief 获取初速度场类型
     * @param[in] obj AI_Solution对象
     * @param[in] name 初速度场名称
     * @return 初速度场类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString getInitialValueType(AI_Solution* obj, const QString& name);

    /**
     * @brief 获取相互作用类型
     * @param[in] obj AI_Solution对象
     * @param[in] name 相互作用名称
     * @return 相互作用类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getInteractionType(AI_Solution* obj, const QString& name);

    /**
     * @brief 获取探针类型
     * @param[in] obj AI_Solution对象
     * @param[in] name 探针名称
     * @return 探针类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getProbeType(AI_Solution* obj, const QString& name);

    /**
     * @brief 获取边界条件类型
     * @param[in] obj AI_Solution对象
     * @param[in] name 边界条件名称
     * @return 边界条件类型字符串
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-27
     */
    QString getBCSType(AI_Solution* obj, const QString& name);
};

#endif