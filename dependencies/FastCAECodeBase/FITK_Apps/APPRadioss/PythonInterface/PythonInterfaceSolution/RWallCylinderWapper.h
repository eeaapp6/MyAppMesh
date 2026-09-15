/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file RWallCylinderWapper.h
 * @brief 圆柱刚性墙装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-14
 */
#ifndef _RWALL_CYLINDER_WAPPER_H___
#define _RWALL_CYLINDER_WAPPER_H___

#include <QObject>

#include "PyRWallBase.h"
#include "PythonInterfaceSolutionAPI.h"

class RWallCylinder : public PyInterface::PyRWallBase
{
public:
    explicit RWallCylinder(const QString& name);
    explicit RWallCylinder();
    virtual ~RWallCylinder() = default;
};

/**
 * @brief 圆柱刚性墙装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-14
 */
class PythonInterfaceSolutionAPI RWallCylinderWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建圆柱刚性墙对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    RWallCylinder* new_RWallCylinder();

    /**
     * @brief 删除圆柱刚性墙对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    void delete_RWallCylinder(RWallCylinder* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    RWallCylinder* static_RWallCylinder_GetRWallCylinder(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString getName(RWallCylinder* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setName(RWallCylinder* obj, const QString& name);

    /**
     * @brief 创建圆柱刚性墙
     * @param[i] name 刚性墙名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString createRWall(RWallCylinder* obj, const QString& name = QString());

    /**
    * @brief 设置参考节点ID
    * @param[i] value 参考节点ID
    * @author wangning (2185896382@qq.com)
    * @date 2026-05-15
    */
    //void setNodeId(RWallCylinder* obj, int value);

        /**
     * @brief 设置滑移类型
     * @param[i] type 滑移类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setSlidingType(RWallCylinder* obj, int type);

    /**
     * @brief 设置搜索距离
     * @param[i] value 搜索距离
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setDSearch(RWallCylinder* obj, double value);

    /**
    * @brief 设置摩擦系数
    * @param[i] value 摩擦系数
    * @author wangning (2185896382@qq.com)
    * @date 2026-05-15
    */
    QString setFrictionCoef(RWallCylinder* obj, double value);

    /**
     * @brief 设置特征直径
     * @param[i] value 特征直径
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setDiameter(RWallCylinder* obj, double value);

    /**
     * @brief 设置滤波因子
     * @param[i] value 滤波因子
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setFilteringFactor(RWallCylinder* obj, double value);

    /**
     * @brief 设置滤波标志
     * @param[i] value 滤波标志
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setFilteringFlag(RWallCylinder* obj, int value);

    /**
     * @brief 设置添加到刚性墙的节点组
     * @param[i] name 节点组名称 设置"None"表示无节点组
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setNodeGroup1(RWallCylinder* obj, const QString& name);

    /**
     * @brief 设置从刚性墙移除的节点组
     * @param[i] name 节点组名称 设置"None"表示无节点组
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setNodeGroup2(RWallCylinder* obj, const QString& name);

    /**
     * @brief 设置圆柱基点坐标
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setBasicPoint(RWallCylinder* obj, double x, double y, double z);

    /**
     * @brief 设置圆柱轴向量
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-14
     */
    QString setAxisVector(RWallCylinder* obj, double nx, double ny, double nz);
};

#endif