/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file RigidBodyWapper.h
 * @brief 刚体连接装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
#ifndef _RIGID_BODY_WAPPER_H___
#define _RIGID_BODY_WAPPER_H___

#include <QObject>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKAbstractConnection;
    class FITKConnectionManager;
    class FITKConnectionRBODY;
}

class RigidBody
{
public:
    //按名称获取刚体连接对象。
    explicit RigidBody(const QString& name);

    //创建刚体连接对象包装器。
    explicit RigidBody();

    virtual ~RigidBody() = default;

    //获取刚体连接名称。
    QString getName() const;

    //设置刚体连接名称。
    bool setName(const QString& name);

    //创建刚体连接对象。
    bool createRigidBody(const QString& name);

    //设置次节点组。
    bool setSecondaryNodesGroup(const QString& name);

    //设置包络面。
    bool setEnvelopeSurface(const QString& name);

    //获取刚体连接数据对象。
    Radioss::FITKConnectionRBODY* getRigidBody();

    //刷新树和图形显示。
    void updateRender();

private:
    Radioss::FITKConnectionManager* _connectionManager{ nullptr };
    Radioss::FITKConnectionRBODY* _rbody{ nullptr };
};

/**
 * @brief 刚体连接装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-25
 */
class PythonInterfaceSolutionAPI RigidBodyWapper : public QObject
{
    Q_OBJECT

public slots:
    //新建刚体连接对象。
    RigidBody* new_RigidBody();

    //删除刚体连接对象。
    void delete_RigidBody(RigidBody* obj);

    //按名称获取刚体连接对象。
    RigidBody* static_RigidBody_GetRigidBody(const QString& name);

    //获取刚体连接名称。
    QString getName(RigidBody* obj);

    //设置刚体连接名称。
    QString setName(RigidBody* obj, const QString& name);

    //创建刚体连接对象。
    QString createRigidBody(RigidBody* obj, const QString& name);

    //设置主节点 ID。nodeID 主节点 ID 值
    QString setMainNodeID(RigidBody* obj, int nodeID);

    // //设置传感器 ID。sensorID 传感器 ID 值
    // QString setSensorID(RigidBody* obj, int sensorID);

    // //设置局部坐标系 ID。systemID 局部坐标系 ID 值
    // QString setSystemID(RigidBody* obj, int systemID);

    //设置惯性类型。type 惯性类型值
    QString setInertiaType(RigidBody* obj, int type);

    //设置质量。mass 质量值
    QString setMass(RigidBody* obj, double mass);

    //设置次节点组。name 次节点组名称
    QString setSecondaryNodesGroup(RigidBody* obj, const QString& name);

    //设置刚性墙失效标志。isDeact 标志值
    QString setDeactivationRigidWall(RigidBody* obj, bool isDeact);

    //设置重心计算方式。type 计算方式值
    QString setGravityCenterComputation(RigidBody* obj, int type);

    //设置包络面。name 包络面名称
    QString setEnvelopeSurface(RigidBody* obj, const QString& name);

    //设置惯量值。index 只能输入 0-5；value 惯量值
    QString setInertiaValue(RigidBody* obj, int index, double value);

    //设置领域分解标志。isDom 标志值
    QString setDomainDecomposition(RigidBody* obj, bool isDom);

    //设置失效标志。isFail 标志值
    QString setIfail(RigidBody* obj, bool isFail);

    //设置法向力缩放值。value 数值
    QString setFN(RigidBody* obj, double value);

    //设置切向力缩放值。value 数值
    QString setFT(RigidBody* obj, double value);

    //设置法向指数。value 指数值
    QString setExpN(RigidBody* obj, int value);

    //设置切向指数。value 指数值
    QString setExpT(RigidBody* obj, int value);

private:
    //获取刚体连接数据对象。
    Radioss::FITKConnectionRBODY* getRigidBody(RigidBody* obj);
};

#endif