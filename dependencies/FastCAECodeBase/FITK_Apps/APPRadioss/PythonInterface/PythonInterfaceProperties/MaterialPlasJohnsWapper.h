/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file MaterialPlasJohnsWapper.h
 * @brief PlasJohns 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _MATERIAL_PLAS_JOHNS_WAPPER_H___
#define _MATERIAL_PLAS_JOHNS_WAPPER_H___

#include <QObject>

#include "PyMaterialsBase.h"
#include "PythonInterfacePropertiesAPI.h"

class PlasJohns : public PyInterface::PyMaterialsBase
{
public:
    explicit PlasJohns(const QString& name);
    explicit PlasJohns();
    virtual ~PlasJohns() = default;
};

/**
 * @brief PlasJohns 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI MaterialPlasJohnsWapper : public QObject
{
    Q_OBJECT

public slots:
    PlasJohns* new_PlasJohns();

    void delete_PlasJohns(PlasJohns* obj);

    /**
     * @brief 在 Python 解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    PlasJohns* static_PlasJohns_GetPlasJohns(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString getName(PlasJohns* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(PlasJohns* obj, const QString& name);

    /**
     * @brief 创建材料
     * @param[i] name 材料名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString createMaterial(PlasJohns* obj, const QString& name);

    /**
     * @brief 设置材料类型
     * @param[i] type 材料类型，0 表示 Classic，1 表示 Simplified
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setType(PlasJohns* obj, int type);

    /**
     * @brief 设置材料密度
     * @param[i] density 密度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setDensity(PlasJohns* obj, double density);

    /**
     * @brief 设置杨氏模量
     * @param[i] modulus 杨氏模量值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setYoungsModulus(PlasJohns* obj, double modulus);

    /**
     * @brief 设置泊松比
     * @param[i] ratio 泊松比值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setPoissonsRatio(PlasJohns* obj, double ratio);

    /**
     * @brief 设置屈服应力
     * @param[i] stress 屈服应力值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setYieldStress(PlasJohns* obj, double stress);

    /**
     * @brief 设置塑性硬化参数
     * @param[i] hardening 塑性硬化参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setPlasticHardening(PlasJohns* obj, double hardening);

    /**
     * @brief 设置塑性硬化指数
     * @param[i] exponent 塑性硬化指数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setPlasticHardeningExponent(PlasJohns* obj, double exponent);

    /**
     * @brief 设置断裂应变
     * @param[i] strain 断裂应变值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setFailureStrain(PlasJohns* obj, double strain);

    /**
     * @brief 设置最大应力
     * @param[i] stress 最大应力值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setMaxStress(PlasJohns* obj, double stress);

    /**
     * @brief 设置极限拉伸应力
     * @param[i] uts 极限拉伸应力值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setUTS(PlasJohns* obj, double uts);

    /**
     * @brief 设置极限拉伸应力对应应变
     * @param[i] strain 工程应变值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setEpsilonUTS(PlasJohns* obj, double strain);

    /**
     * @brief 设置应变率系数
     * @param[i] coefficient 应变率系数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setStrainRateCoefficient(PlasJohns* obj, double coefficient);

    /**
     * @brief 设置参考应变率
     * @param[i] epsilon0 参考应变率值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setReferenceStrainRate(PlasJohns* obj, double epsilon0);

    /**
     * @brief 设置应变率计算标记
     * @param[i] icc 应变率计算标记值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setStrainRateComputation(PlasJohns* obj, int icc);

    /**
     * @brief 设置应变率平滑标记
     * @param[i] smoothing 应变率平滑标记值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setStrainRateSmoothing(PlasJohns* obj, int smoothing);

    /**
     * @brief 设置应变率平滑截止频率
     * @param[i] frequency 截止频率值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setCutoffFreq(PlasJohns* obj, double frequency);

    /**
     * @brief 设置硬化系数
     * @param[i] hardening 硬化系数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setChard(PlasJohns* obj, double hardening);

    /**
     * @brief 设置温度指数
     * @param[i] exponent 温度指数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setTempExponent(PlasJohns* obj, double exponent);

    /**
     * @brief 设置熔化温度
     * @param[i] temperature 熔化温度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setMeltingTemperature(PlasJohns* obj, double temperature);

    /**
     * @brief 设置比热容
     * @param[i] rhoCp 比热容值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setRhoCp(PlasJohns* obj, double rhoCp);

    /**
     * @brief 设置参考温度
     * @param[i] temperature 参考温度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setTref(PlasJohns* obj, double temperature);
};

#endif