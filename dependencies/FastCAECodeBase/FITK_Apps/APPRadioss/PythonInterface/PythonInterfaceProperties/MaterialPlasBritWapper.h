/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file MaterialPlasBritWapper.h
 * @brief PlasBrit 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _MATERIAL_PLAS_BRIT_WAPPER_H___
#define _MATERIAL_PLAS_BRIT_WAPPER_H___

#include <QObject>

#include "PyMaterialsBase.h"
#include "PythonInterfacePropertiesAPI.h"

class PlasBrit : public PyInterface::PyMaterialsBase
{
public:
    explicit PlasBrit(const QString& name);
    explicit PlasBrit();
    virtual ~PlasBrit() = default;
};

/**
 * @brief PlasBrit 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI MaterialPlasBritWapper : public QObject
{
    Q_OBJECT

public slots:
    PlasBrit* new_PlasBrit();

    void delete_PlasBrit(PlasBrit* obj);

    /**
     * @brief 在 Python 解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    PlasBrit* static_PlasBrit_GetPlasBrit(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString getName(PlasBrit* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(PlasBrit* obj, const QString& name);

    /**
     * @brief 创建材料
     * @param[i] name 材料名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString createMaterial(PlasBrit* obj, const QString& name);

    // 设置材料密度
    QString setDensity(PlasBrit* obj, double density);

    // 设置杨氏模量
    QString setYoungsModulus(PlasBrit* obj, double modulus);

    // 设置泊松比
    QString setPoissonsRatio(PlasBrit* obj, double ratio);

    // 设置塑性屈服应力
    QString setPlasticityYieldStress(PlasBrit* obj, double stress);

    // 设置塑性硬化参数
    QString setPlasticityHardeningParameter(PlasBrit* obj, double parameter);

    // 设置塑性硬化指数
    QString setPlasticityHardeningExponent(PlasBrit* obj, double exponent);

    // 设置塑性最大应力
    QString setPlasticityMaximumStress(PlasBrit* obj, double stress);

    // 设置应变率系数
    QString setStrainRateCoefficient(PlasBrit* obj, double coefficient);

    // 设置参考应变速率
    QString setReferenceStrainRate(PlasBrit* obj, double rate);

    // 设置应变率计算标志
    QString setStrainRateComputation(PlasBrit* obj, int computation);

    // 设置应变率平滑标志
    QString setStrainRateSmoothing(PlasBrit* obj, int smoothing);

    // 设置截止频率
    QString setCutoffFreq(PlasBrit* obj, double frequency);

    // 设置方向1拉伸破坏应变
    QString setTensileFilureStrain1(PlasBrit* obj, double strain);

    // 设置方向1最大拉伸破坏应变
    QString setMaximumTensileFilureStrain1(PlasBrit* obj, double strain);

    // 设置方向1最大损伤因子
    QString setMaximumDamageFactor1(PlasBrit* obj, double factor);

    // 设置方向1最大拉伸应变
    QString setMaximumTensileStrain1(PlasBrit* obj, double strain);
    
    // 设置方向2拉伸破坏应变
    QString setTensileFilureStrain2(PlasBrit* obj, double strain);

    // 设置方向2最大拉伸破坏应变
    QString setMaximumTensileFilureStrain2(PlasBrit* obj, double strain);

    // 设置方向2最大损伤因子
    QString setMaximumDamageFactor2(PlasBrit* obj, double factor);
    
    // 设置方向2最大拉伸应变
    QString setMaximumTensileStrain2(PlasBrit* obj, double strain);
};

#endif