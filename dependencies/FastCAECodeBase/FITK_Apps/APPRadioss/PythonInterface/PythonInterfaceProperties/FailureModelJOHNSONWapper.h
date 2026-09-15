/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file FailureModelJOHNSONWapper.h
 * @brief JOHNSON 失效模型装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _FAILURE_MODEL_JOHNSON_WAPPER_H___
#define _FAILURE_MODEL_JOHNSON_WAPPER_H___

#include <QObject>

#include "PyFailureModelBase.h"
#include "PythonInterfacePropertiesAPI.h"

namespace Radioss
{
    class FITKFailureModelJOHNSON;
}

class PythonInterfacePropertiesAPI JOHNSON : public PyInterface::PyFailureModelBase
{
public:
    explicit JOHNSON(const QString& name);
    explicit JOHNSON();
    virtual ~JOHNSON() = default;

    Radioss::FITKFailureModelJOHNSON* getFailureModelJOHNSON();
};

class PythonInterfacePropertiesAPI FailureModelJOHNSONWapper : public QObject
{
    Q_OBJECT

public slots:
    // 新建 JOHNSON 失效模型对象
    JOHNSON* new_JOHNSON();

    // 删除 JOHNSON 失效模型对象
    void delete_JOHNSON(JOHNSON* obj);

    // 在 Python 解释器中通过名称获取对象
    JOHNSON* static_JOHNSON_GetJOHNSON(const QString& name);

    // 获取名称
    QString getName(JOHNSON* obj);

    // 设置名称
    QString setName(JOHNSON* obj, const QString& name);

    // 创建 JOHNSON 失效模型对象
    QString createFailureModel(JOHNSON* obj, const QString& name = QString());

    // 添加材料
    QString addMaterial(JOHNSON* obj, const QString& materialName);

    // 移除材料
    QString removeMaterial(JOHNSON* obj, const QString& materialName);

    // 设置参数 D1
    QString setD1(JOHNSON* obj, double value);

    // 设置参数 D2
    QString setD2(JOHNSON* obj, double value);

    // 设置参数 D3
    QString setD3(JOHNSON* obj, double value);

    // 设置参数 D4
    QString setD4(JOHNSON* obj, double value);

    // 设置参数 D5
    QString setD5(JOHNSON* obj, double value);

    // 设置参考应变率 Eps0
    QString setEps0(JOHNSON* obj, double value);

    // 设置壳失效标志 Ifailsh
    QString setIfailsh(JOHNSON* obj, int flag);

    // 设置实体失效标志 Ifailso
    QString setIfailso(JOHNSON* obj, int flag);

    // 设置裂纹扩展判据 Dadv
    QString setDadv(JOHNSON* obj, double value);

    // 设置 XFEM 标志 Ixfem
    QString setIxfem(JOHNSON* obj, int flag);
};

#endif