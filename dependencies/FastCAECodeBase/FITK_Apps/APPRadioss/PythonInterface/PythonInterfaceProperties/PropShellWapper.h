/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PropShellWapper.h
 * @brief 壳属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _PROP_SHELL_WAPPER_H___
#define _PROP_SHELL_WAPPER_H___

#include <QObject>

#include "PyPropertyBase.h"
#include "PythonInterfacePropertiesAPI.h"

class Shell : public PyInterface::PyPropertyBase
{
public:
    explicit Shell(const QString& name);
    explicit Shell();
    virtual ~Shell() = default;
};

/**
 * @brief 壳属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI PropShellWapper : public QObject
{
    Q_OBJECT

public slots:
    Shell* new_Shell();

    void delete_Shell(Shell* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    Shell* static_Shell_GetShell(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString getName(Shell* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(Shell* obj, const QString& name);

    /**
     * @brief 创建属性
     * @param[i] name 属性名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString createProperty(Shell* obj, const QString& name);

    /**
     * @brief 设置夹挤自由度标志
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIpinch(Shell* obj, int value);

    /**
     * @brief 设置厚度失效比例
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setPthickFail(Shell* obj, double value);

    /**
     * @brief 设置膜向沙漏系数
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setHm(Shell* obj, double value);

    /**
     * @brief 设置离面沙漏系数
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setHf(Shell* obj, double value);

    /**
     * @brief 设置转动沙漏系数
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setHr(Shell* obj, double value);

    /**
     * @brief 设置膜向阻尼
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setDm(Shell* obj, double value);

    /**
     * @brief 设置数值阻尼
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setDn(Shell* obj, double value);

    /**
     * @brief 设置厚度积分点数
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setN(Shell* obj, int value);

    /**
     * @brief 设置壳厚度
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setThick(Shell* obj, double value);

    /**
     * @brief 设置剪切因子
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setAshear(Shell* obj, double value);

    /**
     * @brief 设置壳单元公式标志
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIshell(Shell* obj, int value);

    /**
     * @brief 设置壳小应变公式标志
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIsmstr(Shell* obj, int value);

    /**
     * @brief 设置厚度应力计算标志
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIthick(Shell* obj, int value);

    /**
     * @brief 设置平面应力塑性标志
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIplas(Shell* obj, int value);

    /**
     * @brief 设置三节点壳单元公式标志
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIsh3n(Shell* obj, int value);

    /**
     * @brief 设置钻转自由度刚度标志
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIdril(Shell* obj, int value);
};

#endif