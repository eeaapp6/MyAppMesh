/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file GravityWapper.h
 * @brief 重力场装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-14
 */
#ifndef _GRAVITY_WAPPER_H___
#define _GRAVITY_WAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceSolutionAPI.h"

namespace Radioss
{
    class FITKGravity;
    class FITKGravityManager;
}

class Gravity
{
public:
    explicit Gravity(const QString& name);
    explicit Gravity();
    virtual ~Gravity() = default;

    QString getName();

    bool setName(const QString& name);

    bool createGravity(const QString& name);

    bool setTimeFunction(const QString& name);

    bool setGravityDirection(const QString& direction);

    // bool setSystemID(int id);

    // bool setSensorID(int id);

    bool setNodeGroup(const QString& name);

    bool setAScaleX(double scale);

    bool setFScaleY(double scale);

    Radioss::FITKGravity* getGravity();

    void updateRender();

public:
    Radioss::FITKGravity* _gravity{};

    Radioss::FITKGravityManager* _gravityManager{};
};

/**
 * @brief 重力场装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-14
 */
class PythonInterfaceSolutionAPI GravityWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建重力场对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    Gravity* new_Gravity();

    /**
     * @brief 删除重力场对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    void delete_Gravity(Gravity* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    Gravity* static_Gravity_GetGravity(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString getName(Gravity* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setName(Gravity* obj, const QString& name);

    /**
     * @brief 创建重力场对象
     * @param[i] name 重力场名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString createGravity(Gravity* obj, const QString& name = QString());

    /**
    * @brief 设置时间函数
    * @param[i] name 时间函数名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setTimeFunction(Gravity* obj, const QString& name);

    /**
     * @brief 设置重力方向
    * @param[i] direction 重力方向，使用 x/y/z 表示
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setGravityDirection(Gravity* obj, const QString& direction);

    // /**
    //  * @brief 设置坐标系ID
    //  * @param[i] id 坐标系ID
    //  * @author wangning (2185896382@qq.com)
    //  * @date 2026-05-14
    //  */
    // void setSystemID(Gravity* obj, int id);

    // /**
    //  * @brief 设置传感器ID
    //  * @param[i] id 传感器ID
    //  * @author wangning (2185896382@qq.com)
    //  * @date 2026-05-14
    //  */
    // void setSensorID(Gravity* obj, int id);

    /**
    * @brief 设置节点组
    * @param[i] name 节点组名称 "Global"表示全局
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setNodeGroup(Gravity* obj, const QString& name);

    /**
     * @brief 设置X方向缩放因子
     * @param[i] scale X方向缩放因子
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setAScaleX(Gravity* obj, double scale);

    /**
     * @brief 设置Y方向缩放因子
     * @param[i] scale Y方向缩放因子
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setFScaleY(Gravity* obj, double scale);
};

#endif