/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file MaterialPlasTabWapper.h
 * @brief PlasTab 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
#ifndef _MATERIAL_PLAS_TAB_WAPPER_H___
#define _MATERIAL_PLAS_TAB_WAPPER_H___

#include <QObject>

#include "PyMaterialsBase.h"
#include "PythonInterfacePropertiesAPI.h"

class PlasTab : public PyInterface::PyMaterialsBase
{
public:
    explicit PlasTab(const QString& name);
    explicit PlasTab();
    virtual ~PlasTab() = default;
};

/**
 * @brief PlasTab 材料装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-12
 */
class PythonInterfacePropertiesAPI MaterialPlasTabWapper : public QObject
{
    Q_OBJECT

public slots:
    PlasTab* new_PlasTab();

    void delete_PlasTab(PlasTab* obj);

    /**
     * @brief 在 Python 解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    PlasTab* static_PlasTab_GetPlasTab(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString getName(PlasTab* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setName(PlasTab* obj, const QString& name);

    /**
     * @brief 创建材料
     * @param[i] name 材料名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString createMaterial(PlasTab* obj, const QString& name);

    /**
     * @brief 设置材料密度
     * @param[i] density 密度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setDensity(PlasTab* obj, double density);

    /**
     * @brief 设置杨氏模量
     * @param[i] modulus 杨氏模量值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setYoungsModulus(PlasTab* obj, double modulus);

    /**
     * @brief 设置泊松比
     * @param[i] ratio 泊松比值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setPoissonsRatio(PlasTab* obj, double ratio);

    /**
     * @brief 设置失效应变
     * @param[i] strain 失效应变值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setFailureStrain(PlasTab* obj, double strain);

    /**
     * @brief 设置拉伸失效应变
     * @param[i] strain 拉伸失效应变值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setTensileFailureStrain(PlasTab* obj, double strain);

    /**
     * @brief 设置最大拉伸失效应变
     * @param[i] strain 最大拉伸失效应变值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setMaxTensileFailureStrain(PlasTab* obj, double strain);

    /**
     * @brief 设置函数数量
     * @param[i] count 函数数量值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setNumberOfFunctions(PlasTab* obj, int count);

    /**
     * @brief 设置屈服应力函数标识
     * @param[i] index 列表索引，从1开始
     * @param[i] param 新的函数标识值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setFunction(PlasTab* obj, int index, const QString& curveName);

    /**
     * @brief 设置函数缩放因子
        * @param[i] index 列表索引，从1开始
     * @param[i] param 新的缩放因子值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
        QString setFctScale(PlasTab* obj, int index, double param);

    /**
     * @brief 设置应变率参数
        * @param[i] index 列表索引，从1开始
     * @param[i] param 新的应变率值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
        QString setEpsilon(PlasTab* obj, int index, double param);

    /**
     * @brief 设置平滑应变率选项标志
     * @param[i] flag 选项标志值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setSmoothStrainRateOptFlag(PlasTab* obj, int flag);

    /**
     * @brief 设置硬化系数
     * @param[i] hardening 硬化系数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setChard(PlasTab* obj, double hardening);

    /**
     * @brief 设置应变率平滑截止频率
     * @param[i] frequency 截止频率值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setFcut(PlasTab* obj, double frequency);

    /**
     * @brief 设置应变率选择标志
     * @param[i] flag 应变率选择标志值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setVpFlag(PlasTab* obj, int flag);

    /**
     * @brief 设置元素删除拉伸应变
     * @param[i] strain 拉伸应变值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setTensileStrainForElementDeletion(PlasTab* obj, double strain);

    /**
     * @brief 设置压力屈服因子函数 ID
     * @param[i] id 函数 ID 值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setFctIDp(PlasTab* obj, int id);

    /**
     * @brief 设置缩放因子
     * @param[i] factor 缩放因子值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setFscaleFactor(PlasTab* obj, double factor);

    /**
     * @brief 设置杨氏模量函数标识
     * @param[i] id 函数标识值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setFctIODE(PlasTab* obj, int id);

    /**
     * @brief 设置饱和杨氏模量
     * @param[i] modulus 饱和杨氏模量值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setEint(PlasTab* obj, double modulus);

    /**
     * @brief 设置杨氏模量演化参数
     * @param[i] ce 演化参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-12
     */
    QString setCe(PlasTab* obj, double ce);
};

#endif