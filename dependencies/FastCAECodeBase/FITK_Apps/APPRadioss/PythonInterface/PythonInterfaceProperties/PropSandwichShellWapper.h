/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PropSandwichShellWapper.h
 * @brief 夹层壳属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
#ifndef _PROP_SANDWICH_SHELL_WAPPER_H___
#define _PROP_SANDWICH_SHELL_WAPPER_H___

#include <QObject>

#include "PyPropertyBase.h"
#include "PythonInterfacePropertiesAPI.h"

class SandwichShell : public PyInterface::PyPropertyBase
{
public:
    explicit SandwichShell(const QString& name);
    explicit SandwichShell();
    virtual ~SandwichShell() = default;
};

/**
 * @brief 夹层壳属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
class PythonInterfacePropertiesAPI PropSandwichShellWapper : public QObject
{
    Q_OBJECT

public slots:
    //新建 SandwichShell 属性对象。
    SandwichShell* new_SandwichShell();

    //删除 SandwichShell 属性对象。
    void delete_SandwichShell(SandwichShell* obj);

    //按名称获取 SandwichShell 属性对象。
    SandwichShell* static_SandwichShell_GetSandwichShell(const QString& name);

    //获取属性名称。
    QString getName(SandwichShell* obj);

    //设置属性名称。
    QString setName(SandwichShell* obj, const QString& name);

    //创建 SandwichShell 属性。
    QString createProperty(SandwichShell* obj, const QString& name);

    //设置壳单元类型标志。value 标志值
    QString setIshell(SandwichShell* obj, int value);

    //设置主从控制标志。value 标志值
    QString setIsmstr(SandwichShell* obj, int value);

    //设置三节点壳单元选项。value 选项值
    QString setIsh3n(SandwichShell* obj, int value);

    //设置钻转自由度标志。value 标志值
    QString setIdril(SandwichShell* obj, int value);

    //设置厚度失效比例。value 比例值
    QString setPthickFail(SandwichShell* obj, double value);

    //设置膜应变率阻尼系数。value 系数值
    QString setHm(SandwichShell* obj, double value);

    //设置弯曲应变率阻尼系数。value 系数值
    QString setHf(SandwichShell* obj, double value);

    //设置转动应变率阻尼系数。value 系数值
    QString setHr(SandwichShell* obj, double value);

    //设置膜阻尼系数。value 系数值
    QString setDm(SandwichShell* obj, double value);

    //设置法向阻尼系数。value 系数值
    QString setDn(SandwichShell* obj, double value);

    //设置夹层层数。value 层数值
    QString setNumOfLayers(SandwichShell* obj, int value);

    //设置应变计算选项。value 选项值
    QString setIstrain(SandwichShell* obj, int value);

    //设置总厚度。value 厚度值
    QString setThick(SandwichShell* obj, double value);

    //设置横向剪切比例系数。value 系数值
    QString setAshear(SandwichShell* obj, double value);

    //设置厚度更新选项。value 选项值
    QString setIthick(SandwichShell* obj, int value);

    //设置塑性选项。value 选项值
    QString setIplas(SandwichShell* obj, int value);

    //设置局部 x 方向向量分量。value 分量值
    QString setVx(SandwichShell* obj, double value);

    //设置局部 y 方向向量分量。value 分量值
    QString setVy(SandwichShell* obj, double value);

    //设置局部 z 方向向量分量。value 分量值
    QString setVz(SandwichShell* obj, double value);

    //设置坐标系 ID。value 坐标系 ID 值
    //QString setSkewID(SandwichShell* obj, int value);

    //设置正交材料方向选项。value 选项值
    QString setIorth(SandwichShell* obj, int value);

    //设置层位置是否为正向。value 布尔标志值
    QString setIpos(SandwichShell* obj, bool value);

    //设置层角度。index 为 1-based 层索引；value 角度值
    QString setLayerAngle(SandwichShell* obj, int index, double value);

    //设置层厚度。index 为 1-based 层索引；value 厚度值
    QString setLayerThickness(SandwichShell* obj, int index, double value);

    //设置层 z 向位置。index 为 1-based 层索引；value 位置值
    QString setLayerPositionZ(SandwichShell* obj, int index, double value);

    //设置层材料。index 为 1-based 层索引；materialName 材料名称
    QString setLayerMaterial(SandwichShell* obj, int index, const QString& materialName);

    //设置层失效权重。index 为 1-based 层索引；value 权重值
    QString setLayerFailureWeight(SandwichShell* obj, int index, double value);
};

#endif