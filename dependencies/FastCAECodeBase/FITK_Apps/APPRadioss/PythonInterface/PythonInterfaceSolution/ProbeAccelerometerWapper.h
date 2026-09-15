/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file ProbeAccelerometerWapper.h
 * @brief 加速度探针装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _PROBE_ACCELEROMETER_WAPPER_H___
#define _PROBE_ACCELEROMETER_WAPPER_H___

#include <QObject>

#include "PyProbeBase.h"
#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKProbeAcceleration;
}

class PythonInterfaceSolutionAPI ProbeAccelerometer : public PyInterface::PyProbeBase
{
public:
    explicit ProbeAccelerometer(const QString& name);
    explicit ProbeAccelerometer();
    virtual ~ProbeAccelerometer() = default;

    /**
     * @brief 获取加速度探针对象指针
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    Radioss::FITKProbeAcceleration* getProbeAccelerometer();

    /**
     * @brief 设置单点节点组
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    bool setSingleNodeGroup(const QString& name);
};

/**
 * @brief 加速度探针装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
class PythonInterfaceSolutionAPI ProbeAccelerometerWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建加速度探针对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    ProbeAccelerometer* new_ProbeAccelerometer();

    /**
     * @brief 删除加速度探针对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    void delete_ProbeAccelerometer(ProbeAccelerometer* obj);

    /**
     * @brief 在 Python 解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    ProbeAccelerometer* static_ProbeAccelerometer_GetProbeAccelerometer(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    QString getName(ProbeAccelerometer* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    QString setName(ProbeAccelerometer* obj, const QString& name);

    /**
     * @brief 创建加速度探针对象
     * @param[i] name 探针名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    QString createProbe(ProbeAccelerometer* obj, const QString& name = QString());

    /**
     * @brief 设置坐标系 ID
     * @param[i] systemID 坐标系 ID
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    QString setSystemID(ProbeAccelerometer* obj, int systemID);

    /**
     * @brief 设置单点节点组
     * @param[i] name 节点组名称，设置为 None 表示清空
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    QString setSingleNodeGroup(ProbeAccelerometer* obj, const QString& name);

    /**
     * @brief 获取首节点 ID
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    int getNodeID(ProbeAccelerometer* obj);

    /**
     * @brief 设置截止频率
     * @param[i] freq 截止频率
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    QString setCutOffFreq(ProbeAccelerometer* obj, double freq);

};

#endif