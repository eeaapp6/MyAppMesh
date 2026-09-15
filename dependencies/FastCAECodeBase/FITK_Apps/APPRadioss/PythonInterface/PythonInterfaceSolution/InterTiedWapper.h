/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file InterTiedWapper.h
 * @brief 动力学绑定装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
#ifndef _INTER_TIED_WAPPER_H___
#define _INTER_TIED_WAPPER_H___

#include <QObject>

#include "PyInteractionBase.h"
#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    struct TiedOptionalData;
    class FITKInteractionKinematicTied;
    class FITKInteractionKinematicTiedDefault;
}

class InterTied : public PyInterface::PyInteractionBase
{
public:
    explicit InterTied(const QString& name);
    explicit InterTied();
    virtual ~InterTied() = default;
};

/**
 * @brief 动力学绑定装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
class PythonInterfaceSolutionAPI InterTiedWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建动力学绑定对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    InterTied* new_InterTied();

    /**
     * @brief 删除动力学绑定对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    void delete_InterTied(InterTied* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    InterTied* static_InterTied_GetInterTied(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString getName(InterTied* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setName(InterTied* obj, const QString& name);

    /**
     * @brief 创建动力学绑定对象
     * @param[in] name 动力学绑定名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString createInteraction(InterTied* obj, const QString& name);

    /**
     * @brief 设置主面
     * @param[in] name 主面名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setMasterSurface(InterTied* obj, const QString& name);

    /**
     * @brief 设置从节点组
     * @param[in] name 从节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setNodeGroup(InterTied* obj, const QString& name);

    /**
     * @brief 设置层级
     * @param[in] level 层级
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setLevel(InterTied* obj, int level);

    /**
     * @brief 设置搜索距离
     * @param[in] value 搜索距离
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-25
     */
    QString setSearchDistance(InterTied* obj, double value);

    //设置忽略标志。flag 标志值
    QString setIgnoreFlag(InterTied* obj, int flag);

    //设置点焊公式标志。flag 标志值
    QString setSpotWeldFormulationFlag(InterTied* obj, int flag);

    //设置搜索公式标志。flag 标志值
    QString setSearchFormulationFlag(InterTied* obj, int flag);

    //设置节点删除标志。flag 标志值
    QString setNodeDeletionFlag(InterTied* obj, int flag);

    //QString setIstfFlagDefault(InterTied* obj, int flag);//该参数无效

    //设置破坏标志。value 标志值
    QString setRupt(InterTied* obj, int value);

    //设置滤波标志。value 标志值
    QString setIfilter(InterTied* obj, int value);

    //设置应力率相关函数。curveName 曲线名称
    QString setFctIDsr(InterTied* obj, const QString& curveName);

    //设置法向应力-相对位移函数。curveName 曲线名称
    QString setFctIDsn(InterTied* obj, const QString& curveName);

    //设置切向应力-相对位移函数。curveName 曲线名称
    QString setFctIDst(InterTied* obj, const QString& curveName);

    //设置对称标志。value 标志值
    QString setIsym(InterTied* obj, int value);

    //设置最大法向距离。value 距离值
    QString setMaxNDist(InterTied* obj, double value);

    //设置最大切向距离。value 距离值
    QString setMaxTDist(InterTied* obj, double value);

    //设置应力缩放系数。value 系数值
    QString setFscaleStress(InterTied* obj, double value);

    //设置应变率缩放系数。value 系数值
    QString setFscaleStrRate(InterTied* obj, double value);

    //设置距离缩放系数。value 系数值
    QString setFscaleDist(InterTied* obj, double value);

    //设置 alpha 系数。value 系数值
    QString setAlpha(InterTied* obj, double value);

    //设置面积。value 面积值
    QString setArea(InterTied* obj, double value);

    //设置应力比例系数。value 系数值
    QString setStfac(InterTied* obj, double value);

    //设置粘性系数。value 系数值
    QString setVisc(InterTied* obj, double value);

    //设置 Istf 标志。value 标志值
    QString setIstf(InterTied* obj, int value);

private:
    Radioss::FITKInteractionKinematicTied* getInterTied(InterTied* obj);

    Radioss::FITKInteractionKinematicTiedDefault* getInterTiedDefault(InterTied* obj);

    Radioss::TiedOptionalData* getTiedOptionalData(InterTied* obj);
};

#endif