/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FailureModelBIQUADWapper.h
 * @brief BIQUAD 失效模型装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _FAILURE_MODEL_BIQUAD_WAPPER_H___
#define _FAILURE_MODEL_BIQUAD_WAPPER_H___

#include <QObject>

#include "PyFailureModelBase.h"
#include "PythonInterfacePropertiesAPI.h"

namespace Radioss
{
    class FITKFailureModelBIQUAD;
}

class PythonInterfacePropertiesAPI BIQUAD : public PyInterface::PyFailureModelBase
{
public:
    explicit BIQUAD(const QString& name);
    explicit BIQUAD();
    virtual ~BIQUAD() = default;

    Radioss::FITKFailureModelBIQUAD* getFailureModelBIQUAD();
};

class PythonInterfacePropertiesAPI FailureModelBIQUADWapper : public QObject
{
    Q_OBJECT

public slots:
    // 新建 BIQUAD 失效模型对象
    BIQUAD* new_BIQUAD();

    // 删除 BIQUAD 失效模型对象
    void delete_BIQUAD(BIQUAD* obj);

    // 在 Python 解释器中通过名称获取对象
    BIQUAD* static_BIQUAD_GetBIQUAD(const QString& name);

    // 获取名称
    QString getName(BIQUAD* obj);

    // 设置名称
    QString setName(BIQUAD* obj, const QString& name);

    // 创建 BIQUAD 失效模型对象
    QString createFailureModel(BIQUAD* obj, const QString& name = QString());

    // 添加材料
    QString addMaterial(BIQUAD* obj, const QString& materialName);

    // 移除材料
    QString removeMaterial(BIQUAD* obj, const QString& materialName);

    // 设置单轴压缩失效塑性应变
    QString setC1(BIQUAD* obj, double value);

    // 设置剪切失效塑性应变
    QString setC2(BIQUAD* obj, double value);

    // 设置单轴拉伸失效塑性应变
    QString setC3(BIQUAD* obj, double value);

    // 设置平面应变拉伸失效塑性应变
    QString setC4(BIQUAD* obj, double value);

    // 设置双轴拉伸失效应变
    QString setC5(BIQUAD* obj, double value);

    // 设置壳单元厚度方向失效积分点比例
    QString setPthickfail(BIQUAD* obj, double value);

    // 设置材料选择标志
    QString setMFlag(BIQUAD* obj, int flag);

    // 设置特定行为标志
    QString setSFlag(BIQUAD* obj, int flag);

    // 设置局部缩颈失稳起始值
    QString setInststart(BIQUAD* obj, double value);

    // 设置单元尺寸因子函数曲线，curveName 为曲线名称
    QString setFctIDel(BIQUAD* obj, const QString& curveName);

    // 设置参考单元尺寸
    QString setElref(BIQUAD* obj, double value);

    // 设置 M-Flag=99 时的失效塑性应变比 R1
    QString setR1(BIQUAD* obj, double value);

    // 设置 M-Flag=99 时的失效塑性应变比 R2
    QString setR2(BIQUAD* obj, double value);

    // 设置 M-Flag=99 时的失效塑性应变比 R4
    QString setR4(BIQUAD* obj, double value);

    // 设置 M-Flag=99 时的失效塑性应变比 R5
    QString setR5(BIQUAD* obj, double value);
};

#endif