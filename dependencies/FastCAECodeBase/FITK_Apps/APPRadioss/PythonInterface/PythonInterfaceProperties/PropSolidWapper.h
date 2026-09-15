/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PropSolidWapper.h
 * @brief 实体属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _PROP_SOLID_WAPPER_H___
#define _PROP_SOLID_WAPPER_H___

#include <QObject>

#include "PyPropertyBase.h"
#include "PythonInterfacePropertiesAPI.h"

class Solid : public PyInterface::PyPropertyBase
{
public:
    explicit Solid(const QString& name);
    explicit Solid();
    virtual ~Solid() = default;
};

/**
 * @brief 实体属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI PropSolidWapper : public QObject
{
    Q_OBJECT

public slots:
    Solid* new_Solid();

    void delete_Solid(Solid* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    Solid* static_Solid_GetSolid(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString getName(Solid* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(Solid* obj, const QString& name);

    /**
     * @brief 创建属性
     * @param[i] name 属性名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString createProperty(Solid* obj, const QString& name);

    /**
     * @brief 设置稳定化数值阻尼
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setDn(Solid* obj, double val);

    /**
     * @brief 设置二次体积粘性
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setQa(Solid* obj, double val);

    /**
     * @brief 设置线性体积粘性
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setQb(Solid* obj, double val);

    /**
     * @brief 设置沙漏粘性系数
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setH(Solid* obj, double val);

    /**
     * @brief 设置数值粘性 LambdaV
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setLambdaV(Solid* obj, double val);

    /**
     * @brief 设置数值粘性 MuV
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setMuV(Solid* obj, double val);

    /**
     * @brief 设置最小时间步长
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setMinTimeStep(Solid* obj, double val);

    /**
     * @brief 设置每个实体单元的方向数
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setNdir(Solid* obj, int val);

    /**
     * @brief 设置SPH粒子型PartID
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    //void setSPHPartID(Solid* obj, int val);

    /**
     * @brief 设置实体单元公式标志
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIsolid(Solid* obj, int val);

    /**
     * @brief 设置小应变公式标志
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIsmstr(Solid* obj, int val);

    /**
     * @brief 设置接触压力公式标志
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIcpre(Solid* obj, int val);

    /**
     * @brief 设置四节点四面体公式标志
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setItetra4(Solid* obj, int val);

    /**
     * @brief 设置十节点四面体公式标志
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setItetra10(Solid* obj, int val);

    /**
     * @brief 设置节点质量分配标志
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setImas(Solid* obj, int val);

    /**
     * @brief 设置单元坐标系公式标志
     * @param[i] val 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setIFrame(Solid* obj, int val);
};

#endif