/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file CurveWapper.h
 * @brief 曲线装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
#ifndef _CURVE_WAPPER_H___
#define _CURVE_WAPPER_H___

#include <QObject>

#include "PythonInterfacePropertiesAPI.h"

namespace Radioss
{
    class FITKNonLinerCurve;
    class FITKNonLinerCurveManager;
}

/**
 * @brief 曲线封装类，为 Python 装饰器接口准备的封装类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
class PythonInterfacePropertiesAPI Curve
{
public:
    explicit Curve(const QString& name);
    explicit Curve();
    virtual ~Curve() = default;

    QString getName();

    bool setName(const QString& name);

    bool createCurve(const QString& name);

    bool addPoint(double x, double y);

    bool removePoint(int index);

    bool updatePoint(int index, double x, double y);

    bool removeAllPoints();

    int getPointCount();

    Radioss::FITKNonLinerCurve* getCurve();

    /**
     * @brief 刷新曲线相关界面
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    void updateRender();

protected:
    Radioss::FITKNonLinerCurve* _curve{};

    Radioss::FITKNonLinerCurveManager* _curveManager{};
};

/**
 * @brief 曲线装饰器类
 * @author wangning (2185896382@qq.com)
 * @date 2026-05-22
 */
class PythonInterfacePropertiesAPI CurveWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建曲线对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    Curve* new_Curve();

    /**
     * @brief 删除曲线对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    void delete_Curve(Curve* obj);

    /**
     * @brief 在 Python 解释器中通过名称获取对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    Curve* static_Curve_GetCurve(const QString& name);

    /**
     * @brief 获取名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString getName(Curve* obj);

    /**
     * @brief 设置名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString setName(Curve* obj, const QString& name);

    /**
     * @brief 创建曲线对象
     * @param[i] name 曲线名称
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString createCurve(Curve* obj, const QString& name = QString());

    /**
     * @brief 添加曲线点
     * @param[i] x 点横坐标
     * @param[i] y 点纵坐标
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString addPoint(Curve* obj, double x, double y);

    /**
     * @brief 删除指定索引的曲线点
     * @param[i] index 点索引，按 0 开始
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString removePoint(Curve* obj, int index);

    /**
     * @brief 修改指定索引的曲线点
     * @param[i] index 点索引，按 0 开始
     * @param[i] x 点横坐标
     * @param[i] y 点纵坐标
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString updatePoint(Curve* obj, int index, double x, double y);

    /**
     * @brief 移除全部曲线点
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    QString removeAllPoints(Curve* obj);

    /**
     * @brief 获取曲线点数量
     * @author wangning (2185896382@qq.com)
     * @date 2026-05-22
     */
    int getPointCount(Curve* obj);
};

#endif