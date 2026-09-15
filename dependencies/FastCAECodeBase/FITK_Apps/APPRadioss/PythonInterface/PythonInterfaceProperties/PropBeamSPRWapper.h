/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file PropBeamSPRWapper.h
 * @brief 梁弹簧属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
#ifndef _PROP_BEAM_SPR_WAPPER_H___
#define _PROP_BEAM_SPR_WAPPER_H___

#include <QObject>

#include "PyPropertyBase.h"
#include "PythonInterfacePropertiesAPI.h"

class BeamSPR : public PyInterface::PyPropertyBase
{
public:
    explicit BeamSPR(const QString& name);
    explicit BeamSPR();
    virtual ~BeamSPR() = default;
};

/**
 * @brief 梁弹簧属性装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-13
 */
class PythonInterfacePropertiesAPI PropBeamSPRWapper : public QObject
{
    Q_OBJECT

public slots:
    //新建 BeamSPR 属性对象。
    BeamSPR* new_BeamSPR();

    //删除 BeamSPR 属性对象。
    void delete_BeamSPR(BeamSPR* obj);

    //按名称获取 BeamSPR 属性对象。
    BeamSPR* static_BeamSPR_GetBeamSPR(const QString& name);

    //获取属性名称。
    QString getName(BeamSPR* obj);

    //设置属性名称。
    QString setName(BeamSPR* obj, const QString& name);

    //创建 BeamSPR 属性。
    QString createProperty(BeamSPR* obj, const QString& name);

    //设置质量。value 质量值
    QString setMass(BeamSPR* obj, double value);

    //设置弹簧惯量。value 弹簧惯量值
    QString setInertia(BeamSPR* obj, double value);

    //设置坐标 ID。value 坐标 ID 值
    //void setSkewID(BeamSPR* obj, int value);

    //设置传感器 ID。value 传感器 ID 值
    //QString setSensID(BeamSPR* obj, int value);

    //设置传感器标志。value 传感器标志值
    QString setIsflag(BeamSPR* obj, int value);

    //设置失效标准。value 失效标准值
    QString setIfail(BeamSPR* obj, int value);

    //设置每单位长度输入标志。value 标志值
    QString setIleng(BeamSPR* obj, int value);

    //设置故障模型标志。value 标志值
    QString setIfail2(BeamSPR* obj, int value);

    //设置参考速度。value 参考速度值
    QString setV0(BeamSPR* obj, double value);

    //设置参考角速度。value 参考角速度值
    QString setW0(BeamSPR* obj, double value);

    //设置应变率切割频率。value 频率值
    QString setFcut(BeamSPR* obj, double value);

    //设置平滑应变率标志。value 标志值
    QString setFsmooth(BeamSPR* obj, int value);

    //设置刚度。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 刚度值
    QString setK(BeamSPR* obj, int dof, double value);

    //设置阻尼。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 阻尼值
    QString setC(BeamSPR* obj, int dof, double value);

    //设置非线性刚度函数比例系数。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 比例系数值
    QString setA(BeamSPR* obj, int dof, double value);

    //设置对数速率效应比例系数 B。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setB(BeamSPR* obj, int dof, double value);

    //设置对数速率效应比例系数 D。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setD(BeamSPR* obj, int dof, double value);

    //设置非线性刚度函数 ID1。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；curveName 曲线名称
    QString setFunction1(BeamSPR* obj, int dof, const QString& curveName);

    //设置弹簧硬化标志 H。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 标志值
    QString setH(BeamSPR* obj, int dof, int value);

    //设置函数 ID2。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；curveName 曲线名称
    QString setFunction2(BeamSPR* obj, int dof, const QString& curveName);

    //设置函数 ID3。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；curveName 曲线名称
    QString setFunction3(BeamSPR* obj, int dof, const QString& curveName);

    //设置函数 ID4。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；curveName 曲线名称
    QString setFunction4(BeamSPR* obj, int dof, const QString& curveName);

    //设置最小失效阈值。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 最小值
    QString setMin(BeamSPR* obj, int dof, double value);

    //设置最大失效阈值。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 最大值
    QString setMax(BeamSPR* obj, int dof, double value);

    //设置阻尼函数横坐标比例系数 F。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setF(BeamSPR* obj, int dof, double value);

    //设置阻尼函数纵坐标比例系数 E。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setE(BeamSPR* obj, int dof, double value);

    //设置刚度函数横坐标比例系数 Ascale。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setAscale(BeamSPR* obj, int dof, double value);

    //设置阻尼函数 h() 纵坐标比例系数 Hscale。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setHscale(BeamSPR* obj, int dof, double value);

    //设置相对速度系数 vc。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setVc(BeamSPR* obj, int dof, double value);

    //设置相对速度指数 n。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 指数值
    QString setN(BeamSPR* obj, int dof, double value);

    //设置故障比例系数 alpha。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 系数值
    QString setAlpha(BeamSPR* obj, int dof, double value);

    //设置指数 beta。dof 只能输入 1-6，对应DOF_SPR枚举中的六个值；value 指数值
    QString setExponent(BeamSPR* obj, int dof, double value);
};

#endif