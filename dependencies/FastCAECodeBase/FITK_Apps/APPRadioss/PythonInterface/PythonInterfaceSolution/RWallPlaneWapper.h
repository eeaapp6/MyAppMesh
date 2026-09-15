/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file RWallPlaneWapper.h
 * @brief 平面刚性墙装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
#ifndef _RWALL_PLANE_WAPPER_H___
#define _RWALL_PLANE_WAPPER_H___

#include <QObject>

#include "PyRWallBase.h"
#include "PythonInterfaceSolutionAPI.h"

class RWallPlane : public PyInterface::PyRWallBase
{
public:
    explicit RWallPlane(const QString& name);
    explicit RWallPlane();
    virtual ~RWallPlane() = default;
};

/**
 * @brief 平面刚性墙装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
class PythonInterfaceSolutionAPI RWallPlaneWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建平面刚性墙对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    RWallPlane* new_RWallPlane();

    /**
     * @brief 删除平面刚性墙对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    void delete_RWallPlane(RWallPlane* obj);

    /**
     * @brief 在Python解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    RWallPlane* static_RWallPlane_GetRWallPlane(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString getName(RWallPlane* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setName(RWallPlane* obj, const QString& name);

    /**
     * @brief 创建平面刚性墙
     * @param[i] name 刚性墙名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString createRWall(RWallPlane* obj, const QString& name);

    /**
     * @brief 设置滑移类型
     * @param[i] type 滑移类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setSlidingType(RWallPlane* obj, int type);

    /**
     * @brief 设置搜索距离
     * @param[i] value 搜索距离
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setDSearch(RWallPlane* obj, double value);

    /**
     * @brief 设置摩擦系数
     * @param[i] value 摩擦系数
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setFrictionCoef(RWallPlane* obj, double value);

    /**
     * @brief 设置滤波因子
     * @param[i] value 滤波因子
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setFilteringFactor(RWallPlane* obj, double value);

    /**
     * @brief 设置滤波标志
     * @param[i] value 滤波标志
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setFilteringFlag(RWallPlane* obj, int value);

    /**
     * @brief 设置添加到刚性墙的节点组
     * @param[i] name 节点组名称，设置为None表示清空
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setNodeGroup1(RWallPlane* obj, const QString& name);

    /**
     * @brief 设置从刚性墙移除的节点组
     * @param[i] name 节点组名称，设置为None表示清空
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setNodeGroup2(RWallPlane* obj, const QString& name);

    /**
     * @brief 设置平面基点坐标
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setBasicPoint(RWallPlane* obj, double x, double y, double z);

    /**
     * @brief 设置平面法向量
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setNormalVector(RWallPlane* obj, double nx, double ny, double nz);
};

#endif