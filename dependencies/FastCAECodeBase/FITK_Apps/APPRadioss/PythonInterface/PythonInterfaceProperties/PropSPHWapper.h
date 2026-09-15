/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PropSPHWapper.h
 * @brief SPH 属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
#ifndef _PROP_SPH_WAPPER_H___
#define _PROP_SPH_WAPPER_H___

#include <QObject>

#include "PyPropertyBase.h"
#include "PythonInterfacePropertiesAPI.h"

class SPH : public PyInterface::PyPropertyBase
{
public:
    explicit SPH(const QString& name);
    explicit SPH();
    virtual ~SPH() = default;
};

/**
 * @brief SPH 属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
class PythonInterfacePropertiesAPI PropSPHWapper : public QObject
{
    Q_OBJECT

public slots:
    SPH* new_SPH();

    void delete_SPH(SPH* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    SPH* static_SPH_GetSPH(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString getName(SPH* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setName(SPH* obj, const QString& name);

    /**
     * @brief 创建属性
     * @param[i] name 属性名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString createProperty(SPH* obj, const QString& name);

    /**
     * @brief 设置粒子质量
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setMP(SPH* obj, double value);

    /**
     * @brief 设置二次体积粘性
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setQA(SPH* obj, double value);

    /**
     * @brief 设置线性体积粘性
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setQB(SPH* obj, double value);

    /**
     * @brief 设置保守平滑系数
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setACS(SPH* obj, double value);

    /**
     * @brief 设置 skew ID
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    //void setSkewID(SPH* obj, int value);

    /**
     * @brief 设置平滑长度变化选项
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setHID(SPH* obj, int value);

    /**
     * @brief 设置 SPH 修正阶次
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setOrderID(SPH* obj, int value);

    /**
     * @brief 设置平滑长度
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setH(SPH* obj, double value);

    /**
     * @brief 设置抗拉不稳定系数
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-13
     */
    QString setStab(SPH* obj, double value);
};

#endif