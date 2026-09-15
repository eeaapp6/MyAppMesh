/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFSolution.h
 * @brief  solution 数据接口声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-22
 *********************************************************************/
#ifndef _FITK_OF_SOLUTION_H___
#define _FITK_OF_SOLUTION_H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOFEnum.hpp"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    class FITKOFAbsSolutionSolver;
    /**
     * @brief  代数方程控制参数数据对象
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-03
     */
    class FITKInterfaceFlowOFAPI FITKOFAlgebraicEquationsPara
    {
    public:
        explicit FITKOFAlgebraicEquationsPara();
        virtual ~FITKOFAlgebraicEquationsPara();
        /**
         * @brief    获取变量名
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        QString getVariableName();
        /**
         * @brief    获取solver
         * @return   FITKOFAbsSolutionSolver *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        FITKOFAbsSolutionSolver* getSolutionSolver();
        /**
         * @brief    获取增加的参数数据
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        Core::FITKParameter* getSolverAdditionalPara();
        /**
         * @brief    设置变量名称
         * @param[i] vname 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        void setVariableName(QString vname);
        /**
         * @brief    设置solver
         * @param[i] solver 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        void setSolutionSolver(FITKOFAbsSolutionSolver* solver);
        /**
         * @brief    设置增加的参数数据
         * @param[i] para 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-03
         */
        void setSolverAdditionalPara(Core::FITKParameter* para);

    private:
        /**
         * @brief  变量名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-03
         */
        QString _variableName{};
        /**
         * @brief  方程求解数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-03
         */
        FITKOFAbsSolutionSolver* _solver{};
        /**
         * @brief  增加的参数数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-03
         */
        Core::FITKParameter* _solverAdditionalPara{};
    };

    /**
     * @brief  Solution数据对象
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFSolution : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
            FITKCLASS(Interface, FITKOFSolution);
    public:
        explicit FITKOFSolution();
        virtual ~FITKOFSolution();

        void onlyKeepSolvers(QStringList nameList);

        void onlyKeepResiduals(QStringList nameList);
        /**
         * @brief    获取solver变量个数
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        int getSolversCount();
        /**
         * @brief    获取solver变量数据
         * @param[i] index 
         * @return   FITKOFAlgebraicEquationsPara *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        FITKOFAlgebraicEquationsPara* getSolverVariablePara(int index);
        /**
         * @brief    获取求解器有关的迭代过程名称
         * @return   QString 迭代过程类型名称 一般有（SIMPLE PIMPLE PISO)
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QString getSolverSpeciallyDataName();
        /**
         * @brief    获取求解器有关的数据参数
         * @return   Core::FITKParameter*
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getSolverSpeciallyDataPara();
        /**
         * @brief    获取Residuals变量的数量
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        int getResidualsCount();
        /**
         * @brief    获取Residuals变量名称
         * @param[i] index 
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QString getResidualVariableName(int index);
        /**
         * @brief    获取Residuals变量参数
         * @param[i] index 
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getResidualVariablePara(int index);
        /**
         * @brief    获取Relaxation数据
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getRelaxation();
        /**
         * @brief    获取Limits数据
         * @return   Core::FITKParameter *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        Core::FITKParameter* getLimits();
        /**
         * @brief    追加参数Solver
         * @param[i] vname 变量名称
         * @param[i] vpara 参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        void appendSolutionSolver(FITKOFAlgebraicEquationsPara* vpara);
        /**
         * @brief    设置与求解器相关的迭代过程参数数据
         * @param[i] name 迭代过程类型名称 一般有（SIMPLE PIMPLE PISO)
         * @param[i] para 参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        void setSpeciallyData(QString name, Core::FITKParameter* para);
        /**
         * @brief    设置Residual数据
         * @param[i] name 变量名称
         * @param[i] para 参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        void setResidualData(QString name, Core::FITKParameter* para);
        /**
         * @brief    添加Residuals数据
         * @param[i] name 变量名称
         * @param[i] calcName 算法名称
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        void initAppendResidual(QString name, QString calcName = QString());
    private:

    private:
        /**
         * @brief  代数方程变量配置数据参数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        QList<FITKOFAlgebraicEquationsPara*> _solvers{};
        /**
         * @brief  特殊的数据：不同求解器的数据不同<名称， 参数>
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        QPair<QString, Core::FITKParameter*> _speciallyData{};
        /**
         * @brief  residuals 数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        QList<QPair<QString, Core::FITKParameter*>> _residuals{};
        /**
         * @brief  Relaxation数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _relaxation{};
        /**
         * @brief  limits 数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-22
         */
        Core::FITKParameter* _limits{};
    };
}


#endif
