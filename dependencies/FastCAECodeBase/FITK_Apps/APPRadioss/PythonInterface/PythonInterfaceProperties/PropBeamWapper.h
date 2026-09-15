/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PropBeamWapper.h
 * @brief 梁属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _PROP_BEAM_WAPPER_H___
#define _PROP_BEAM_WAPPER_H___

#include <QObject>

#include "PyPropertyBase.h"
#include "PythonInterfacePropertiesAPI.h"

class Beam : public PyInterface::PyPropertyBase
{
public:
    explicit Beam(const QString& name);
    explicit Beam();
    virtual ~Beam() = default;
};

/**
 * @brief 梁属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI PropBeamWapper : public QObject
{
    Q_OBJECT

public slots:
    Beam* new_Beam();

    void delete_Beam(Beam* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    Beam* static_Beam_GetBeam(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString getName(Beam* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(Beam* obj, const QString& name);

    /**
     * @brief 创建属性
     * @param[i] name 属性名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString createProperty(Beam* obj, const QString& name);

    /**
     * @brief 设置小应变选项
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setSmallStrainOption(Beam* obj, int value);

    /**
     * @brief 设置梁膜阻尼
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setBeamMembraneDamping(Beam* obj, double value);

    /**
     * @brief 设置梁弯曲阻尼
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setBeamFlexuralDamping(Beam* obj, double value);

    /**
     * @brief 设置横截面面积
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setCrossSection(Beam* obj, double value);

    /**
     * @brief 设置截面惯性矩 Iyy
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setAreaMomentInertiaIyy(Beam* obj, double value);

    /**
     * @brief 设置截面惯性矩 Izz
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setAreaMomentInertiaIzz(Beam* obj, double value);

    /**
     * @brief 设置截面惯性矩 Ixx
     * @param[i] value 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setAreaMomentInertiaIxx(Beam* obj, double value);

    /**
     * @brief 设置是否忽略剪切
     * @param[i] isNeglects 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setNeglectsShear(Beam* obj, bool isNeglects);

    /**
     * @brief 设置节点1旋转自由度
     * @param[i] direction 方向字符串，x、y、z，不区分大小写
     * @param[i] isEnable 是否启用
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setRotationDOF1(Beam* obj, const QString& direction, bool isEnable);

    /**
     * @brief 设置节点2旋转自由度
     * @param[i] direction 方向字符串，x、y、z，不区分大小写
     * @param[i] isEnable 是否启用
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setRotationDOF2(Beam* obj, const QString& direction, bool isEnable);
};

#endif