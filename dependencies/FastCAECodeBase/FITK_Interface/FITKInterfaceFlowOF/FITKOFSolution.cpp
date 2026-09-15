/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSolution.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITKOFSolutionSolver.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"

namespace Interface
{
    FITKOFAlgebraicEquationsPara::FITKOFAlgebraicEquationsPara()
    {

    }

    FITKOFAlgebraicEquationsPara::~FITKOFAlgebraicEquationsPara()
    {
        if (_solver) delete _solver;
        if (_solverAdditionalPara) delete _solverAdditionalPara;
    }

    QString FITKOFAlgebraicEquationsPara::getVariableName()
    {
        return _variableName;
    }

    FITKOFAbsSolutionSolver* FITKOFAlgebraicEquationsPara::getSolutionSolver()
    {
        return _solver;
    }

    Core::FITKParameter* FITKOFAlgebraicEquationsPara::getSolverAdditionalPara()
    {
        return _solverAdditionalPara;
    }

    void FITKOFAlgebraicEquationsPara::setVariableName(QString vname)
    {
        _variableName = vname;
    }

    void FITKOFAlgebraicEquationsPara::setSolutionSolver(FITKOFAbsSolutionSolver* solver)
    {
        if (_solver != solver && _solver) delete _solver;
        _solver = solver;
    }

    void FITKOFAlgebraicEquationsPara::setSolverAdditionalPara(Core::FITKParameter* para)
    {
        if (_solverAdditionalPara != para && _solverAdditionalPara) delete _solverAdditionalPara;
        _solverAdditionalPara = para;
    }

    FITKOFSolution::FITKOFSolution()
    {
        //初始化
        _speciallyData.second = nullptr;
        _relaxation = new Core::FITKParameter;
        _limits = new Core::FITKParameter;
    }

    FITKOFSolution::~FITKOFSolution()
    {
        //销毁数据
        if (_speciallyData.second) delete _speciallyData.second;
        if (_relaxation) delete _relaxation;
        if (_limits) delete _limits;

        for (QList<FITKOFAlgebraicEquationsPara*>::iterator iter = _solvers.begin(); iter != _solvers.end(); ++iter)
            if (*iter) delete *iter;
        _solvers.clear();

        for (QList<QPair<QString, Core::FITKParameter*>>::iterator iter = _residuals.begin(); iter != _residuals.end(); ++iter)
            if (iter->second) delete iter->second;
        _residuals.clear();
    }

    void FITKOFSolution::onlyKeepSolvers(QStringList nameList)
    {
        for (int i = this->_solvers.size() - 1; i >= 0; --i)
        {
            FITKOFAlgebraicEquationsPara* para = _solvers.at(i);
            if (para && nameList.contains(para->getVariableName()))continue;
            if (para) delete para;
            this->_solvers.removeAt(i);
        }
    }

    void FITKOFSolution::onlyKeepResiduals(QStringList nameList)
    {
        for (int i = this->_residuals.size() - 1; i >= 0; --i)
        {
            QString name = this->_residuals.at(i).first;
            if (nameList.contains(name))continue;
            if (this->_residuals.at(i).second) delete this->_residuals.at(i).second;
            this->_residuals.removeAt(i);
        }
    }

    int FITKOFSolution::getSolversCount()
    {
        return _solvers.size();
    }

    FITKOFAlgebraicEquationsPara* FITKOFSolution::getSolverVariablePara(int index)
    {
        if (index < 0 || index >= _solvers.size())return nullptr;
        return _solvers.at(index);
    }

    QString FITKOFSolution::getSolverSpeciallyDataName()
    {
        return _speciallyData.first;
    }

    Core::FITKParameter* FITKOFSolution::getSolverSpeciallyDataPara()
    {
        return _speciallyData.second;
    }

    int FITKOFSolution::getResidualsCount()
    {
        return _residuals.size();
    }

    QString FITKOFSolution::getResidualVariableName(int index)
    {
        if (index < 0 || index >= _residuals.size())return nullptr;
        return _residuals.at(index).first;
    }

    Core::FITKParameter* FITKOFSolution::getResidualVariablePara(int index)
    {
        if (index < 0 || index >= _residuals.size())return nullptr;
        return _residuals.at(index).second;
    }

    Core::FITKParameter* FITKOFSolution::getRelaxation()
    {
        return _relaxation;
    }

    Core::FITKParameter* FITKOFSolution::getLimits()
    {
        return _limits;
    }

    void FITKOFSolution::appendSolutionSolver(FITKOFAlgebraicEquationsPara* vpara)
    {
        if (!vpara)return;
        _solvers.append(vpara);
    }

    void FITKOFSolution::setSpeciallyData(QString name, Core::FITKParameter* para)
    {
        _speciallyData.first = name;
        if (_speciallyData.second) delete _speciallyData.second;
        _speciallyData.second = para;
    }

    void FITKOFSolution::setResidualData(QString name, Core::FITKParameter* para)
    {
        if (!para)return;
        for (QList<QPair<QString, Core::FITKParameter*>>::iterator iter = _residuals.begin(); iter != _residuals.end(); ++iter)
        {
            if (iter->first != name)continue;
            if (iter->second)
                delete iter->second;
            iter->second = para;
            return;
        }
        _residuals.append(QPair<QString, Core::FITKParameter*>(name, para));
    }

    void FITKOFSolution::initAppendResidual(QString name, QString calcName)
    {
        Core::FITKParameter* data = new Core::FITKParameter;
        if (calcName.isEmpty())
        {
        }
        else if (calcName == "SIMPLE")
        {
            data->createDoubleParameter(name, 1e-04);
        }
        else if (calcName == "PIMPLE")
        {
            data->createDoubleParameter(name, 1e-04);
            //data->createDoubleParameter("Relative", 0);
            //data->createDoubleParameter("Absolute", 1e-04);
        }

        _residuals.append(QPair<QString, Core::FITKParameter*>(name, data));
    }
}
