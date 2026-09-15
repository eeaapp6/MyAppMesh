/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file PhysicsFactoryWapper.h
 * @brief 物理工厂 python 接口装饰器
 * @author wangning (2185896382@qq.com)
 * @date 2026-06-03
 *
 */
#ifndef __PYTHONINTERFACE_PHYSICSFACTORYWAPPER_H___
#define __PYTHONINTERFACE_PHYSICSFACTORYWAPPER_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceAPI.h"
#include "PyPhysicsFactory.h"

namespace PyInterface
{
    class PyPhysicsFactory;
}

/**
 * @brief 物理工厂接口封装，供装饰器装饰
 * @author wangning (2185896382@qq.com)
 * @date 2026-06-03
 */
class PhysicsFactory : public PyInterface::PyPhysicsFactory
{
public:
    PhysicsFactory();
    ~PhysicsFactory();
};

/**
 * @brief 物理工厂 PYTHON 接口装饰器
 * @author wangning (2185896382@qq.com)
 * @date 2026-06-03
 */
class PythonInterfaceAPI PhysicsFactoryWapper : public QObject
{
    Q_OBJECT

public slots:
    /**
     * @brief 新建物理工厂对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    PhysicsFactory* new_PhysicsFactory();

    /**
     * @brief 删除物理工厂对象
     * @param[i] factory 物理工厂对象
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    void delete_PhysicsFactory(PhysicsFactory* factory);

    /**
     * @brief 设置求解器类型
     * @param[i] factory 物理工厂对象
     * @param[i] solverType 求解器类型，1 表示 SIMPLE，2 表示 Inter，3 表示 CHT Multi Region
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setSolver(PhysicsFactory* factory, int solverType);

    /**
     * @brief 设置湍流模型类型
     * @param[i] factory 物理工厂对象
     * @param[i] turbulenceType 湍流类型，0 表示关闭湍流，1 表示 RANS，2 表示 LES
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setTurbence(PhysicsFactory* factory, int turbulenceType);

    /**
     * @brief 设置边界
     * @param[i] factory 物理工厂对象
     * @param[i] regionName 网格区域名称
     * @param[i] boundaryName 网格边界名称
     * @param[i] boundaryType 边界类型
     * @param[i] regionType 区域类型
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setBoundary(PhysicsFactory* factory, const QString& regionName, const QString& boundaryName, int boundaryType, int regionType);

    /**
     * @brief 设置 Newtonian 模型参数,目前只适用SIMPLE
     * @param[i] factory 物理工厂对象
     * @param[i] v 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setNewtonianModelParam(PhysicsFactory* factory, double v);

    /**
     * @brief 设置 surfaceNormalFixedValue 边界参数
     * @param[i] factory 物理工厂对象
     * @param[i] name 边界名称
     * @param[i] v 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setSurfaceNormalFixedValueBCParam(PhysicsFactory* factory, const QString& name, double v);

    /**
     * @brief 设置 Total Pressure 边界参数
     * @param[i] factory 物理工厂对象
     * @param[i] name 边界名称
     * @param[i] p0 参数值
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setTotalPressureBCParam(PhysicsFactory* factory, const QString& name, double p0);

    /**
     * @brief 设置运行时间参数
     * @param[i] factory 物理工厂对象
     * @param[i] time 时间参数
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setRunTimeParam(PhysicsFactory* factory, double time);

    /**
     * @brief 设置运行输出参数
     * @param[i] factory 物理工厂对象
     * @param[i] param 输出参数
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString setRunOutputParam(PhysicsFactory* factory, double param);

    /**
     * @brief 打开运行界面并直接执行求解器启动槽函数
     * @param[i] factory 物理工厂对象
     * @return 执行结果信息，失败时返回错误原因
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-03
     */
    QString startRunSolver(PhysicsFactory* factory);

    //导出后处理vtk文件
    QString exportVTK(PhysicsFactory* factory, const QString& outputPath);
};

#endif