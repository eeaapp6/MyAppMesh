/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file InitialValueTraWapper.h
 * @brief 平动初始值装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-14
 */
#ifndef _INITIAL_VALUE_TRA_WAPPER_H___
#define _INITIAL_VALUE_TRA_WAPPER_H___

#include <QObject>

#include "PyInitialValueBase.h"
#include "PythonInterfaceSolutionAPI.h"

class InitialValueTra : public PyInterface::PyInitialValueBase
{
public:
    explicit InitialValueTra(const QString& name);
    explicit InitialValueTra();
    virtual ~InitialValueTra() = default;
};

/**
 * @brief 平动初始值装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-14
 */
class PythonInterfaceSolutionAPI InitialValueTraWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建平动初始值对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    InitialValueTra* new_InitialValueTra();

    /**
     * @brief 删除平动初始值对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    void delete_InitialValueTra(InitialValueTra* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    InitialValueTra* static_InitialValueTra_GetInitialValueTra(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString getName(InitialValueTra* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setName(InitialValueTra* obj, const QString& name);

    /**
     * @brief 创建平动初始值对象
     * @param[i] name 初始值名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString createInitialValue(InitialValueTra* obj, const QString& name = QString());

    /**
     * @brief 设置初始值
     * @param[i] x X方向值
     * @param[i] y Y方向值
     * @param[i] z Z方向值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setValue(InitialValueTra* obj, double x, double y, double z);

    /**
     * @brief 设置节点组
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setNodeGroup(InitialValueTra* obj, const QString& name);
};

#endif