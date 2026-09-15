/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file PyPhysicsFactory.h
 * @brief python物理工厂基类
 * @author wangning (2185896382@qq.com)
 * @date 2026-06-02
 *
 */
#ifndef __PYTHONINTERFACE_PYPHYSICSFACTORY_H___
#define __PYTHONINTERFACE_PYPHYSICSFACTORY_H___

#include <QObject>
#include <QString>

#include "PythonInterfaceAPI.h"

namespace Interface
{
    class FITKFlowPhysicsHandlerFactory;
    class FITKAbstractOFSolver;
}

namespace  PyInterface
{
    /**
     * @brief python物理工厂类
     * @author wangning (2185896382@qq.com)
     * @date 2026-06-02
     */
    class PythonInterfaceAPI PyPhysicsFactory : public QObject
    {
        Q_OBJECT

    public:
        /**
         * @brief 构造对象
         * @param[i] parent QObject父对象
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        explicit PyPhysicsFactory(QObject* parent = nullptr);
        /**
         * @brief 析构对象
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        virtual ~PyPhysicsFactory() = default;

        /**
         * @brief 设置求解器类型
         * @param[i] solverType 求解器类型，1 表示 SIMPLE，2 表示 Inter，3 表示 CHT Multi Region
         * @return 设置成功返回 true，类型无效或工厂对象为空时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setSolver(int solverType);

        /**
         * @brief 设置湍流模型类型
         * @param[i] turbulenceType 湍流类型，0 表示关闭湍流，1 表示 RANS，2 表示 LES
         * @return 设置成功返回 true，类型无效、工厂对象为空或当前类型无可用模型时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setTurbence(int turbulenceType);

        /**
         * @brief 设置边界
         * @param[i] regionName 网格区域名称
         * @param[i] boundaryName 网格边界名称
         * @param[i] boundaryType 边界类型
         * @param[i] regionType 区域类型
         * @return 设置成功返回 true，参数无效、对象为空或边界已存在时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setBoundary(const QString& regionName, const QString& boundaryName, int boundaryType, int regionType);

        /**
         * @brief 设置求解方案下的Newtonian型transport Model的double参数'v'
         * @param[i] v 参数值
         * @return 设置成功返回 true，参数无效或对象为空时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setNewtonianModelParam(double v);

        /**
         * @brief 设置surfaceNormalFixedValue边界条件的double参数'Reference Value'
         * @param[i] name 边界名称
         * @param[i] v 参数值
         * @return 设置成功返回 true，参数无效或对象为空时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setSurfaceNormalFixedValueBCParam(const QString& name, double v);

        /**
         * @brief 设置Total Pressure边界条件的double参数'p0'
         * @param[i] name 边界名称
         * @param[i] p0 参数值
         * @return 设置成功返回 true，参数无效或对象为空时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setTotalPressureBCParam(const QString& name, double p0);

        /**
         * @brief 设置运行时间参数
         * @param[i] time 时间参数
         * @return 设置成功返回 true，参数无效或对象为空时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setRunTimeParam(double time);

        /**
         * @brief 设置运行输出参数
         * @param[i] param 输出参数
         * @return 设置成功返回 true，参数无效或对象为空时返回 false
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        bool setRunOutputParam(double param);

    protected:
        /**
         * @brief 求解器对象
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        Interface::FITKAbstractOFSolver* getSolver();

    private:
        /**
         * @brief 物理场处理工厂对象
         * @author wangning (2185896382@qq.com)
         * @date 2026-06-02
         */
        Interface::FITKFlowPhysicsHandlerFactory* _factoryData{};

    };

}
#endif