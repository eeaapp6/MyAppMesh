/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file ProbeSectionWapper.h
 * @brief 截面探针装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
#ifndef _PROBE_SECTION_WAPPER_H___
#define _PROBE_SECTION_WAPPER_H___

#include <QObject>

#include "PyProbeBase.h"
#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKProbeSection;
}

class PythonInterfaceSolutionAPI ProbeSection : public PyInterface::PyProbeBase
{
public:
    explicit ProbeSection(const QString& name);
    explicit ProbeSection();
    virtual ~ProbeSection() = default;

    /**
     * @brief 获取截面探针对象指针
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    Radioss::FITKProbeSection* getProbeSection();

    /**
     * @brief 设置单点节点组1
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    bool setSingleNodeGroup1(const QString& name);

    /**
     * @brief 设置单点节点组2
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    bool setSingleNodeGroup2(const QString& name);

    /**
     * @brief 设置单点节点组3
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    bool setSingleNodeGroup3(const QString& name);

    /**
     * @brief 设置节点组
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    bool setNodeGroup(const QString& name);

    /**
     * @brief 设置壳单元组
     * @param[i] name 单元组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-26
     */
    bool setShellElementGroup(const QString& name);
};

/**
 * @brief 截面探针装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-26
 */
class PythonInterfaceSolutionAPI ProbeSectionWapper : public QObject
{
    Q_OBJECT

public slots:
    // 新建截面探针对象
    ProbeSection* new_ProbeSection();

    // 删除截面探针对象
    void delete_ProbeSection(ProbeSection* obj);

    // 在 Python 解释器中通过名称获取对象
    ProbeSection* static_ProbeSection_GetProbeSection(const QString& name);

    // 获取名称
    QString getName(ProbeSection* obj);

    // 设置名称
    QString setName(ProbeSection* obj, const QString& name);

    // 创建截面探针对象
    QString createProbe(ProbeSection* obj, const QString& name = QString());

    // 设置单点节点组1
    QString setSingleNodeGroup1(ProbeSection* obj, const QString& name);

    // 设置单点节点组2
    QString setSingleNodeGroup2(ProbeSection* obj, const QString& name);

    // 设置单点节点组3
    QString setSingleNodeGroup3(ProbeSection* obj, const QString& name);

    // 设置节点组
    QString setNodeGroup(ProbeSection* obj, const QString& name);

    // 设置 I_SAVE 标志
    QString setIsave(ProbeSection* obj, int flag);

    // 设置移动坐标系 ID
    //QString setFrameID(ProbeSection* obj, int frameID);//暂时无效

    // 设置文件名
    QString setFileName(ProbeSection* obj, const QString& fileName);

    // 设置时间步长
    QString setDt(ProbeSection* obj, double value);

    // 设置指数平均滤波系数
    QString setAlpha(ProbeSection* obj, double value);

    // 设置壳单元组
    QString setShellElementGroup(ProbeSection* obj, const QString& name);

    // 设置局部坐标系中心选项
    QString setIframe(ProbeSection* obj, int flag);
};

#endif