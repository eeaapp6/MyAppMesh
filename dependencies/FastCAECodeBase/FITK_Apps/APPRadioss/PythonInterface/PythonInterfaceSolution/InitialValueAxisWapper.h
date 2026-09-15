/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file InitialValueAxisWapper.h
 * @brief 角速度初始值装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
#ifndef _INITIAL_VALUE_AXIS_WAPPER_H___
#define _INITIAL_VALUE_AXIS_WAPPER_H___

#include <QObject>

#include "PyInitialValueBase.h"
#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKInitialFieldAxis;
}

class InitialValueAxis : public PyInterface::PyInitialValueBase
{
public:
    explicit InitialValueAxis(const QString& name);
    explicit InitialValueAxis();
    virtual ~InitialValueAxis() = default;

    Radioss::FITKInitialFieldAxis* getInitialValueAxis();
};

/**
 * @brief 角速度初始值装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
class PythonInterfaceSolutionAPI InitialValueAxisWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建角速度初始值对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    InitialValueAxis* new_InitialValueAxis();

    /**
     * @brief 删除角速度初始值对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    void delete_InitialValueAxis(InitialValueAxis* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    InitialValueAxis* static_InitialValueAxis_GetInitialValueAxis(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString getName(InitialValueAxis* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setName(InitialValueAxis* obj, const QString& name);

    /**
     * @brief 创建角速度初始值对象
     * @param[i] name 初始值名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString createInitialValue(InitialValueAxis* obj, const QString& name = QString());

    /**
     * @brief 设置初始平动速度
     * @param[i] x X方向值
     * @param[i] y Y方向值
     * @param[i] z Z方向值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setValue(InitialValueAxis* obj, double x, double y, double z);

    /**
     * @brief 设置节点组
     * @param[i] name 节点组名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setNodeGroup(InitialValueAxis* obj, const QString& name);

    /**
     * @brief 设置旋转轴方向
     * @param[i] dir 方向，支持 X/Y/Z
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setDir(InitialValueAxis* obj, const QString& dir);

    // /**
    //  * @brief 设置参考坐标系ID
    //  * @param[i] id 坐标系ID
    //  * @author wangning (2185896382@qq.com)
    //  * @date 2026-05-22
    //  */
    // QString setFrameID(InitialValueAxis* obj, int id);

    /**
     * @brief 设置绕轴角速度
     * @param[i] value 角速度值
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setVr(InitialValueAxis* obj, double value);
};

#endif