/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file InterMultiUsageImpactWapper.h
 * @brief 多用途冲击装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
#ifndef _INTER_MULTI_USAGE_IMPACT_WAPPER_H___
#define _INTER_MULTI_USAGE_IMPACT_WAPPER_H___

#include <QObject>

#include "PyInteractionBase.h"
#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    struct MultiUsageImpactOptionalData;
    class FITKInteractionMultiUsageImpact;
    class FITKInteractionMultiUsageImpactDefaultValue;
}

class InterMultiUsageImpact : public PyInterface::PyInteractionBase
{
public:
    explicit InterMultiUsageImpact(const QString& name);
    explicit InterMultiUsageImpact();
    virtual ~InterMultiUsageImpact() = default;
};

/**
 * @brief 多用途冲击装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
class PythonInterfaceSolutionAPI InterMultiUsageImpactWapper : public QObject
{
    Q_OBJECT

public slots:
    InterMultiUsageImpact* new_InterMultiUsageImpact();

    void delete_InterMultiUsageImpact(InterMultiUsageImpact* obj);

    InterMultiUsageImpact* static_InterMultiUsageImpact_GetInterMultiUsageImpact(const QString& name);

    QString getName(InterMultiUsageImpact* obj);

    QString setName(InterMultiUsageImpact* obj, const QString& name);

    QString createInteraction(InterMultiUsageImpact* obj, const QString& name);

    QString setMasterSurface(InterMultiUsageImpact* obj, const QString& name);

    QString setNodeGroup(InterMultiUsageImpact* obj, const QString& name);

    //设置是否启用热传导或热摩擦。value 标志值
    QString setIthe(InterMultiUsageImpact* obj, int value);

    //设置带有曲率的间隙包络线标志。value 标志值
    QString setIcurv(InterMultiUsageImpact* obj, int value);

    //设置局部曲率标志。value 标志值
    QString setIadm(InterMultiUsageImpact* obj, int value);

    //设置间隙比例因子。value 系数值
    QString setFscaleGap(InterMultiUsageImpact* obj, double value);

    //设置最大间隙。value 间隙值
    QString setGAPmax(InterMultiUsageImpact* obj, double value);

    //设置初始渗透最大分数。value 分数值
    QString setFpenmax(InterMultiUsageImpact* obj, double value);

    //设置绑定关联点标志。value 标志值
    QString setItied(InterMultiUsageImpact* obj, int value);

    //设置最小刚度。value 刚度值
    QString setSTmin(InterMultiUsageImpact* obj, double value);

    //设置最大刚度。value 刚度值
    QString setSTmax(InterMultiUsageImpact* obj, double value);

    //设置网格大小百分比。value 百分比值
    QString setMeshSize(InterMultiUsageImpact* obj, double value);

    //设置最小接口时间步长。value 时间步长值
    QString setDtmin(InterMultiUsageImpact* obj, double value);

    //设置主侧刚度比例因子。value 系数值
    QString setStfac(InterMultiUsageImpact* obj, double value);

    //设置摩擦系数。value 系数值
    QString setFric(InterMultiUsageImpact* obj, double value);

    //设置最小间隙。value 间隙值
    QString setGapmin(InterMultiUsageImpact* obj, double value);

    //设置开始时间。value 时间值
    QString setTstart(InterMultiUsageImpact* obj, double value);

    //设置停用时间。value 时间值
    QString setTstop(InterMultiUsageImpact* obj, double value);

    //设置边界条件停用标志。value 标志值
    QString setIbc(InterMultiUsageImpact* obj, int value);

    //设置界面刚度临界阻尼系数。value 系数值
    QString setVisS(InterMultiUsageImpact* obj, double value);

    //设置界面摩擦临界阻尼系数。value 系数值
    QString setVisF(InterMultiUsageImpact* obj, double value);

    //设置排序因子。value 因子值
    QString setBumult(InterMultiUsageImpact* obj, double value);

    //设置摩擦配方标志。value 标志值
    QString setIfric(InterMultiUsageImpact* obj, int value);

    //设置摩擦过滤标志。value 标志值
    QString setIfiltr(InterMultiUsageImpact* obj, int value);

    //设置滤波系数。value 系数值
    QString setXfreq(InterMultiUsageImpact* obj, double value);

    //设置传感器标识符。value 标识符值
    QString setSensID(InterMultiUsageImpact* obj, int value);

    //设置摩擦系数曲线。curveName 曲线名称
    QString setFctIDf(InterMultiUsageImpact* obj, const QString& curveName);

    //设置摩擦系数横坐标比例因子。value 系数值
    QString setAscaleF(InterMultiUsageImpact* obj, double value);

    //设置摩擦标识符。value 标识符值
    QString setFricID(InterMultiUsageImpact* obj, int value);

    //设置刚度标志。value 标志值
    QString setIstf(InterMultiUsageImpact* obj, int value);

    //设置间隙单元选项标志。value 标志值
    QString setIgap(InterMultiUsageImpact* obj, int value);

    //设置气囊通风孔关闭标志。value 标志值
    QString setIbag(InterMultiUsageImpact* obj, int value);

    //设置节点退化表面标志。value 标志值
    QString setIdel(InterMultiUsageImpact* obj, int value);

    //设置次节点停用标志。value 标志值
    QString setIremGap(InterMultiUsageImpact* obj, int value);

    //设置辅助节点停用标志。value 标志值
    QString setIremI2(InterMultiUsageImpact* obj, int value);

    //设置初始穿透刚度停用标志。value 标志值
    QString setInacti(InterMultiUsageImpact* obj, int value);

    //设置摩擦惩罚公式类型。value 标志值
    QString setIform(InterMultiUsageImpact* obj, int value);

    //设置第一节点标识。value 标识值，暂时无效
    //QString setNodeID1(InterMultiUsageImpact* obj, int value);

    //设置第二节点标识。value 标识值，暂时无效
    //QString setNodeID2(InterMultiUsageImpact* obj, int value);

    //设置摩擦定律系数 C1。value 系数值
    QString setC1(InterMultiUsageImpact* obj, double value);

    //设置摩擦定律系数 C2。value 系数值
    QString setC2(InterMultiUsageImpact* obj, double value);

    //设置摩擦定律系数 C3。value 系数值
    QString setC3(InterMultiUsageImpact* obj, double value);

    //设置摩擦定律系数 C4。value 系数值
    QString setC4(InterMultiUsageImpact* obj, double value);

    //设置摩擦定律系数 C5。value 系数值
    QString setC5(InterMultiUsageImpact* obj, double value);

    //设置摩擦定律系数 C6。value 系数值
    QString setC6(InterMultiUsageImpact* obj, double value);

    //设置 90 度半径元素数。value 数值
    QString setNRadm(InterMultiUsageImpact* obj, int value);

    //设置渗透百分比标准。value 百分比值
    QString setPadm(InterMultiUsageImpact* obj, double value);

    //设置角度标准。value 角度值
    QString setAngladm(InterMultiUsageImpact* obj, double value);

    //设置热交换系数。value 系数值
    QString setKthe(InterMultiUsageImpact* obj, double value);

    //设置热交换曲线。curveName 曲线名称
    QString setFctIDK(InterMultiUsageImpact* obj, const QString& curveName);

    //设置接口温度。value 温度值
    QString setTint(InterMultiUsageImpact* obj, double value);

    //设置热接触配方标志。value 标志值
    QString setItheForm(InterMultiUsageImpact* obj, int value);

    //设置热交换横坐标比例因子。value 系数值
    QString setAscaleK(InterMultiUsageImpact* obj, double value);

    //设置辐射因子。value 因子值
    QString setFrad(InterMultiUsageImpact* obj, double value);

    //设置辐射最大距离。value 距离值
    QString setDrad(InterMultiUsageImpact* obj, double value);

    //设置二次摩擦发热系数。value 系数值
    QString setFheats(InterMultiUsageImpact* obj, double value);

    //设置主摩擦发热系数。value 系数值
    QString setFheatm(InterMultiUsageImpact* obj, double value);

private:
    Radioss::FITKInteractionMultiUsageImpact* getInterMultiUsageImpact(InterMultiUsageImpact* obj);

    Radioss::FITKInteractionMultiUsageImpactDefaultValue* getInterMultiUsageImpactDefault(InterMultiUsageImpact* obj);

    Radioss::MultiUsageImpactOptionalData* getMultiUsageImpactOptionalData(InterMultiUsageImpact* obj);
};

#endif