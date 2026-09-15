/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiationParaManager.h"
#include "FITKOFRadiationModel.h"
#include "FITKOFRadiationAbsorptionModel.h"
#include "FITKOFRadiationScatterModel.h"
#include "FITKOFRadiationSunDirection.h"
#include "FITKOFRadiationSunLoad.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

namespace Interface
{
    FITKOFRadiationParaManager::FITKOFRadiationParaManager()
    {
        this->initRadiationModels();
        this->initRadiationAbsorptionModels();
        this->initRadiationScatterModels();
        this->initRadiationSunDirections();
        this->initRadiationSunLoads();
    }

    FITKOFRadiationParaManager::~FITKOFRadiationParaManager()
    {

    }

    QStringList FITKOFRadiationParaManager::filterRadiationModels(FITKOFSolverTypeEnum::FITKOFSolverType solverType)
    {
        QStringList listType;
        //求解器遍历
        for (QList<FITKOFRadiationModelInfo>::iterator iter = m_radiationModels.begin(); iter != m_radiationModels.end(); ++iter)
        {
            //进行过滤
            if (iter->_enableType.contains(solverType))
                listType.append(iter->_radiationModelName);
        }
        return listType;
    }

    FITKOFRadiationModelInfo FITKOFRadiationParaManager::getRadiationModelInfo(QString radiationModelName)
    {
        for (QList<FITKOFRadiationModelInfo>::iterator iter = m_radiationModels.begin(); iter != m_radiationModels.end(); ++iter)
        {
            if (iter->_radiationModelName == radiationModelName) return *iter;
        }
        return FITKOFRadiationModelInfo();
    }

    QStringList FITKOFRadiationParaManager::filterRadiationAbsorptionModels(FITKOFSolverTypeEnum::FITKOFSolverType solverType)
    {
        QStringList listType;
        //求解器遍历
        for (QList<FITKOFRadiationAbsorptionModelInfo>::iterator iter = m_radiationAbsorptionModels.begin(); iter != m_radiationAbsorptionModels.end(); ++iter)
        {
            //进行过滤
            if (iter->_enableType.contains(solverType))
                listType.append(iter->_radiationAbsorptionModelName);
        }
        return listType;
    }

    FITKOFRadiationAbsorptionModelInfo FITKOFRadiationParaManager::getRadiationAbsorptionModelInfo(QString radiationModelName)
    {
        for (QList<FITKOFRadiationAbsorptionModelInfo>::iterator iter = m_radiationAbsorptionModels.begin(); iter != m_radiationAbsorptionModels.end(); ++iter)
        {
            if (iter->_radiationAbsorptionModelName == radiationModelName) return *iter;
        }
        return FITKOFRadiationAbsorptionModelInfo();
    }

    QStringList FITKOFRadiationParaManager::filterRadiationScatterModels(FITKOFSolverTypeEnum::FITKOFSolverType solverType)
    {
        QStringList listType;
        //求解器遍历
        for (QList<FITKOFRadiationScatterModelInfo>::iterator iter = m_radiationScatterModels.begin(); iter != m_radiationScatterModels.end(); ++iter)
        {
            //进行过滤
            if (iter->_enableType.contains(solverType))
                listType.append(iter->_radiationScatterModelName);
        }
        return listType;
    }

    FITKOFRadiationScatterModelInfo FITKOFRadiationParaManager::getRadiationScatterModelInfo(QString radiationModelName)
    {
        for (QList<FITKOFRadiationScatterModelInfo>::iterator iter = m_radiationScatterModels.begin(); iter != m_radiationScatterModels.end(); ++iter)
        {
            if (iter->_radiationScatterModelName == radiationModelName) return *iter;
        }
        return FITKOFRadiationScatterModelInfo();
    }

    QStringList FITKOFRadiationParaManager::filterRadiationSunDirections(FITKOFSolverTypeEnum::FITKOFSolverType solverType)
    {
        QStringList listType;
        //求解器遍历
        for (QList<FITKOFRadiationSunDirectionInfo>::iterator iter = m_radiationSunDirections.begin(); iter != m_radiationSunDirections.end(); ++iter)
        {
            //进行过滤
            if (iter->_enableType.contains(solverType))
                listType.append(iter->_radiationSunDirectionName);
        }
        return listType;
    }

    FITKOFRadiationSunDirectionInfo FITKOFRadiationParaManager::getRadiationSunDirectionInfo(QString radiationModelName)
    {
        for (QList<FITKOFRadiationSunDirectionInfo>::iterator iter = m_radiationSunDirections.begin(); iter != m_radiationSunDirections.end(); ++iter)
        {
            if (iter->_radiationSunDirectionName == radiationModelName) return *iter;
        }
        return FITKOFRadiationSunDirectionInfo();
    }

    QStringList FITKOFRadiationParaManager::filterRadiationSunLoads(FITKOFSolverTypeEnum::FITKOFSolverType solverType)
    {
        QStringList listType;
        //求解器遍历
        for (QList<FITKOFRadiationSunLoadInfo>::iterator iter = m_radiationSunLoads.begin(); iter != m_radiationSunLoads.end(); ++iter)
        {
            //进行过滤
            if (iter->_enableType.contains(solverType))
                listType.append(iter->_radiationSunLoadName);
        }
        return listType;
    }

    FITKOFRadiationSunLoadInfo FITKOFRadiationParaManager::getRadiationSunLoadInfo(QString radiationModelName)
    {
        for (QList<FITKOFRadiationSunLoadInfo>::iterator iter = m_radiationSunLoads.begin(); iter != m_radiationSunLoads.end(); ++iter)
        {
            if (iter->_radiationSunLoadName == radiationModelName) return *iter;
        }
        return FITKOFRadiationSunLoadInfo();
    }

    void FITKOFRadiationParaManager::initRadiationModels()
    {
        //none
        FITKOFRadiationModelInfo infoNone;
        infoNone._radiationModelName = "none";
        infoNone._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoNone._createFuns = [&] { return new FITKOFRadiationModelNone; };
        m_radiationModels.append(infoNone);

        //P1
        FITKOFRadiationModelInfo infoP1;
        infoP1._radiationModelName = "P1";
        infoP1._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoP1._createFuns = [&] { return new FITKOFRadiationModelP1; };
        m_radiationModels.append(infoP1);

        //Discrete Ordinates
        FITKOFRadiationModelInfo infoDiscreteOrdinates;
        infoDiscreteOrdinates._radiationModelName = "Discrete Ordinates";
        infoDiscreteOrdinates._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoDiscreteOrdinates._createFuns = [&] { return new FITKOFRadiationModelDiscreteOrdinates; };
        m_radiationModels.append(infoDiscreteOrdinates);

        //Surface to Surface
        FITKOFRadiationModelInfo infoSurfToSurf;
        infoSurfToSurf._radiationModelName = "Surface to Surface";
        infoSurfToSurf._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoSurfToSurf._createFuns = [&] { return new FITKOFRadiationModelSurfToSurf; };
        m_radiationModels.append(infoSurfToSurf);
    }

    void FITKOFRadiationParaManager::initRadiationAbsorptionModels()
    {
        //none
        FITKOFRadiationAbsorptionModelInfo infoNone;
        infoNone._radiationAbsorptionModelName = "none";
        infoNone._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoNone._createFuns = [&] { return new FITKOFRadiationAbsorptionModelNone; };
        m_radiationAbsorptionModels.append(infoNone);

        //Constant
        FITKOFRadiationAbsorptionModelInfo infoConstant;
        infoConstant._radiationAbsorptionModelName = "Constant";
        infoConstant._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstant._createFuns = [&] { return new FITKOFRadiationAbsorptionModelConstant; };
        m_radiationAbsorptionModels.append(infoConstant);
    }

    void FITKOFRadiationParaManager::initRadiationScatterModels()
    {
        //none
        FITKOFRadiationScatterModelInfo infoNone;
        infoNone._radiationScatterModelName = "none";
        infoNone._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoNone._createFuns = [&] { return new FITKOFRadiationScatterModelNone; };
        m_radiationScatterModels.append(infoNone);

        //Constant
        FITKOFRadiationScatterModelInfo infoConstant;
        infoConstant._radiationScatterModelName = "Constant";
        infoConstant._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstant._createFuns = [&] { return new FITKOFRadiationScatterModelConstant; };
        m_radiationScatterModels.append(infoConstant);
    }

    void FITKOFRadiationParaManager::initRadiationSunDirections()
    {
        //Constant
        FITKOFRadiationSunDirectionInfo infoConstant;
        infoConstant._radiationSunDirectionName = "Constant";
        infoConstant._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstant._createFuns = [&] { return new FITKOFRadiationSunDirectionConstant; };
        m_radiationSunDirections.append(infoConstant);

        //Sun Tracking
        FITKOFRadiationSunDirectionInfo infoSunTracking;
        infoSunTracking._radiationSunDirectionName = "Sun Tracking";
        infoSunTracking._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoSunTracking._createFuns = [&] { return new FITKOFRadiationSunDirectionSunTracking; };
        m_radiationSunDirections.append(infoSunTracking);
    }

    void FITKOFRadiationParaManager::initRadiationSunLoads()
    {
        //Constant
        FITKOFRadiationSunLoadInfo infoConstant;
        infoConstant._radiationSunLoadName = "Constant";
        infoConstant._enableType.append(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION);
        infoConstant._createFuns = [&] { return new FITKOFRadiationSunLoadConstant; };
        m_radiationSunLoads.append(infoConstant);
    }

}