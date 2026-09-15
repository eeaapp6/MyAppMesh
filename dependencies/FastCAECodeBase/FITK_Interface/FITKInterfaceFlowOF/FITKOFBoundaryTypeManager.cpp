/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFBoundaryTypeManager.h"
#include "FITKOFBoundaryType.h"

namespace Interface
{

    FITKOFBoundaryTypeManager::FITKOFBoundaryTypeManager()
    {
        this->initSIMPLESolverBTy();
        this->initInterSolverBTy();
        this->initCHTMultiRegionSolverBTy();
    }

    FITKOFBoundaryTypeManager::~FITKOFBoundaryTypeManager()
    {
        QHash< FITKOFSolverTypeEnum::FITKOFSolverType, QList<FITKOFBoundaryTypeInfo*>>::iterator iter = m_boundaryTypes.begin();
        for (; iter != m_boundaryTypes.end(); ++iter)
        {
            QList<FITKOFBoundaryTypeInfo*> bTyData = iter.value();
            for (QList<FITKOFBoundaryTypeInfo*>::iterator iterData = bTyData.begin(); iterData != bTyData.end(); ++iterData)
            {
                if (*iterData) delete *iterData;
            }
        }
    }

    QStringList FITKOFBoundaryTypeManager::filterBoundariesType(FITKOFSolverTypeEnum::FITKOFSolverType solverType, FITKOFSolverTypeEnum::FITKOFBoundaryType boundaryType, QString variableType)
    {
        QStringList boundaryTypes;
        if (variableType.contains('.'))
        {
            QStringList vStrList = variableType.split('.');
            if (!vStrList.isEmpty()) variableType = vStrList.at(0);
        }
        //边界类型信息遍历
        QList<FITKOFBoundaryTypeInfo*> bTyData = m_boundaryTypes.value(solverType);
        for (QList<FITKOFBoundaryTypeInfo*>::iterator iter = bTyData.begin(); iter != bTyData.end(); ++iter)
        {
            //过滤
            if ((*iter)->_enableBoundary.contains(boundaryType) && (*iter)->_enableBoundary[boundaryType].contains(variableType))
                boundaryTypes.append((*iter)->_boundaryTypeName);
        }
        return boundaryTypes;
    }

    FITKOFBoundaryTypeInfo FITKOFBoundaryTypeManager::getBoundaryTypeInfo(QString boundaryTypeName, FITKOFSolverTypeEnum::FITKOFSolverType solverType, QString variableType, FITKOFSolverTypeEnum::FITKOFBoundaryType boundaryType)
    {
        if (variableType.contains('.'))
        {
            QStringList vStrList = variableType.split('.');
            if (!vStrList.isEmpty()) variableType = vStrList.at(0);
        }
        //获取求解器对应的边界数据
        QList<FITKOFBoundaryTypeInfo*> bTyData = m_boundaryTypes.value(solverType);
        for (QList<FITKOFBoundaryTypeInfo*>::iterator iter = bTyData.begin(); iter != bTyData.end(); ++iter)
        {
            if ((*iter)->_boundaryTypeName == boundaryTypeName && (*iter)->_enableBoundary.contains(boundaryType) && (*iter)->_enableBoundary[boundaryType].contains(variableType))
                return *(*iter);
        }
        return FITKOFBoundaryTypeInfo();
    }

    void FITKOFBoundaryTypeManager::initSIMPLESolverBTy()
    {
        QList<FITKOFBoundaryTypeInfo*> bTypeInfos;
        //注册Symmetry
        FITKOFBoundaryTypeInfo* symmetryInfo = new FITKOFBoundaryTypeInfo;
        symmetryInfo->_boundaryTypeName = "Symmetry";
        symmetryInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BSymmetry, QStringList() << "p" << "U" << "k" << "omega" << "nut");
        symmetryInfo->_createFuns = [&] { return new FITKOFBTySymmetry; };
        bTypeInfos.append(symmetryInfo);

        //注册Wedge
        FITKOFBoundaryTypeInfo* wedgeInfo = new FITKOFBoundaryTypeInfo;
        wedgeInfo->_boundaryTypeName = "Wedge";
        wedgeInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWedge, QStringList() << "p" << "U" << "k" << "omega" << "nut");
        wedgeInfo->_createFuns = [&] { return new FITKOFBTyWedge; };
        bTypeInfos.append(wedgeInfo);

        //注册Zero Gradient
        FITKOFBoundaryTypeInfo* zeroGradientInfo = new FITKOFBoundaryTypeInfo;
        zeroGradientInfo->_boundaryTypeName = "Zero Gradient";
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "p" << "U" << "omega");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "p" << "U" << "k" << "Omega");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "k" << "omega");
        zeroGradientInfo->_createFuns = [&] { return new FITKOFBTyZeroGradient; };
        bTypeInfos.append(zeroGradientInfo);

        //注册No-Slip
        FITKOFBoundaryTypeInfo* noSlipInfo = new FITKOFBoundaryTypeInfo;
        noSlipInfo->_boundaryTypeName = "No-Slip";
        noSlipInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "U");
        noSlipInfo->_createFuns = [&] { return new FITKOFBTyNoSlip; };
        bTypeInfos.append(noSlipInfo);

        //注册Fixed Value湍流动能
        FITKOFBoundaryTypeInfo* fixedValueTKEInfo = new FITKOFBoundaryTypeInfo;
        fixedValueTKEInfo->_boundaryTypeName = "Fixed Value";
        fixedValueTKEInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "k");
        fixedValueTKEInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "k");
        fixedValueTKEInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "k");
        fixedValueTKEInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "p");
        fixedValueTKEInfo->_createFuns = [&] { return new FITKOFBTyTKEFixedValue; };
        bTypeInfos.append(fixedValueTKEInfo);

        //注册Fixed Value速度
        FITKOFBoundaryTypeInfo* fixedValueVInfo = new FITKOFBoundaryTypeInfo;
        fixedValueVInfo->_boundaryTypeName = "Fixed Value";
        fixedValueVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "U");
        fixedValueVInfo->_createFuns = [&] { return new FITKOFBTyVFixedValue; };
        bTypeInfos.append(fixedValueVInfo);

        //注册Standard Wall Function
        FITKOFBoundaryTypeInfo* standardWallFunctionInfo = new FITKOFBoundaryTypeInfo;
        standardWallFunctionInfo->_boundaryTypeName = "Standard Wall Function";
        standardWallFunctionInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "k" << "omega" << "nut");
        standardWallFunctionInfo->_createFuns = [&] { return new FITKOFBTyStandardWallFunction; };
        bTypeInfos.append(standardWallFunctionInfo);

        //注册Low Re Wall Function
        FITKOFBoundaryTypeInfo* lowReWallFunctionInfo = new FITKOFBoundaryTypeInfo;
        lowReWallFunctionInfo->_boundaryTypeName = "Low Re Wall Function";
        lowReWallFunctionInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "k" << "nut");
        lowReWallFunctionInfo->_createFuns = [&] { return new FITKOFBTyVLowReWallFunction; };
        bTypeInfos.append(lowReWallFunctionInfo);

        //注册Total Pressure - m2/s2
        FITKOFBoundaryTypeInfo* totalPressureInfo = new FITKOFBoundaryTypeInfo;
        totalPressureInfo->_boundaryTypeName = "Total Pressure";
        totalPressureInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p");
        totalPressureInfo->_createFuns = [&] { return new FITKOFBTyTKETotalPressure; };
        bTypeInfos.append(totalPressureInfo);

        //注册Pressure Inlet-Outlet Velocity
        FITKOFBoundaryTypeInfo* pIOVInfo = new FITKOFBoundaryTypeInfo;
        pIOVInfo->_boundaryTypeName = "Pressure Inlet-Outlet Velocity";
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "U");
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "U");
        pIOVInfo->_createFuns = [&] { return new FITKOFBTyPressureIOVelocity; };
        bTypeInfos.append(pIOVInfo);

        //注册Turbulent Mixing Length Inlet
        FITKOFBoundaryTypeInfo* tMLIInfo = new FITKOFBoundaryTypeInfo;
        tMLIInfo->_boundaryTypeName = "Turbulent Mixing Length Inlet";
        tMLIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "omega");
        tMLIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "omega");
        tMLIInfo->_createFuns = [&] { return new FITKOFBTyTurbulentMixingLengthInlet; };
        bTypeInfos.append(tMLIInfo);

        //注册Surface Normal Fixed Value
        FITKOFBoundaryTypeInfo* sNFVInfo = new FITKOFBoundaryTypeInfo;
        sNFVInfo->_boundaryTypeName = "Surface Normal Fixed Value";
        sNFVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "U");
        sNFVInfo->_createFuns = [&] { return new FITKOFBTySurNorFixedValue; };
        bTypeInfos.append(sNFVInfo);

        //注册Turbulent Intensity Inlet
        FITKOFBoundaryTypeInfo* turIIInfo = new FITKOFBoundaryTypeInfo;
        turIIInfo->_boundaryTypeName = "Turbulent Intensity Inlet";
        turIIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "k");
        turIIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "k");
        turIIInfo->_createFuns = [&] { return new FITKOFBTyTurbulentIntensityInlet; };
        bTypeInfos.append(turIIInfo);

        //将数据添加 
        m_boundaryTypes.insert(FITKOFSolverTypeEnum::FITKOFSolverType::SIMPLE, bTypeInfos);
    }

    void FITKOFBoundaryTypeManager::initInterSolverBTy()
    {
        QList<FITKOFBoundaryTypeInfo*> bTypeInfos;
        //注册Symmetry
        FITKOFBoundaryTypeInfo* symmetryInfo = new FITKOFBoundaryTypeInfo;
        symmetryInfo->_boundaryTypeName = "Symmetry";
        symmetryInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BSymmetry, QStringList() << "p_rgh" << "U" << "alpha" << "k" << "omega" << "nut");
        symmetryInfo->_createFuns = [&] { return new FITKOFBTySymmetry; };
        bTypeInfos.append(symmetryInfo);

        //注册Wedge
        FITKOFBoundaryTypeInfo* wedgeInfo = new FITKOFBoundaryTypeInfo;
        wedgeInfo->_boundaryTypeName = "Wedge";
        wedgeInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWedge, QStringList() << "p_rgh" << "U" << "alpha" << "k" << "omega" << "nut");
        wedgeInfo->_createFuns = [&] { return new FITKOFBTyWedge; };
        bTypeInfos.append(wedgeInfo);

        //注册Zero Gradient
        FITKOFBoundaryTypeInfo* zeroGradientInfo = new FITKOFBoundaryTypeInfo;
        zeroGradientInfo->_boundaryTypeName = "Zero Gradient";
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "p_rgh" << "U" << "alpha" << "k" << "omega" << "nut");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p_rgh" << "U" << "alpha" << "k" << "omega");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "p_rgh" << "U" << "alpha" << "k" << "omega");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "p_rgh" << "U" << "alpha" << "k" << "omega");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "p_rgh" << "U" << "alpha" << "k" << "omega");
        zeroGradientInfo->_createFuns = [&] { return new FITKOFBTyZeroGradient; };
        bTypeInfos.append(zeroGradientInfo);

        //注册Fixed Flux Pressure
        FITKOFBoundaryTypeInfo* fixedFluxPinfo = new FITKOFBoundaryTypeInfo;
        fixedFluxPinfo->_boundaryTypeName = "Fixed Flux Pressure";
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "p_rgh");
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p_rgh");
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "p_rgh");
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "p_rgh");
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "p_rgh");
        fixedFluxPinfo->_createFuns = [&] { return new FITKOFBTyFixedFluxPressure; };
        bTypeInfos.append(fixedFluxPinfo);

        //注册Standard Wall Function
        FITKOFBoundaryTypeInfo* standardWallFunctionInfo = new FITKOFBoundaryTypeInfo;
        standardWallFunctionInfo->_boundaryTypeName = "Standard Wall Function";
        standardWallFunctionInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "k" << "omega" << "nut");
        standardWallFunctionInfo->_createFuns = [&] { return new FITKOFBTyStandardWallFunction; };
        bTypeInfos.append(standardWallFunctionInfo);

        //注册No-Slip
        FITKOFBoundaryTypeInfo* noSlipInfo = new FITKOFBoundaryTypeInfo;
        noSlipInfo->_boundaryTypeName = "No-Slip";
        noSlipInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "U");
        noSlipInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "U");
        noSlipInfo->_createFuns = [&] { return new FITKOFBTyNoSlip; };
        bTypeInfos.append(noSlipInfo);

        //注册Total Pressure - Pa
        FITKOFBoundaryTypeInfo* totalPressurePInfo = new FITKOFBoundaryTypeInfo;
        totalPressurePInfo->_boundaryTypeName = "Total Pressure";
        totalPressurePInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p_rgh");
        totalPressurePInfo->_createFuns = [&] { return new FITKOFBTyPTotalPressure; };
        bTypeInfos.append(totalPressurePInfo);

        //注册Pressure Inlet-Outlet Velocity
        FITKOFBoundaryTypeInfo* pIOVInfo = new FITKOFBoundaryTypeInfo;
        pIOVInfo->_boundaryTypeName = "Pressure Inlet-Outlet Velocity";
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "U");
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "U");
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "U");
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "U");
        pIOVInfo->_createFuns = [&] { return new FITKOFBTyPressureIOVelocity; };
        bTypeInfos.append(pIOVInfo);

        //注册Surface Normal Fixed Value
        FITKOFBoundaryTypeInfo* sNFVInfo = new FITKOFBoundaryTypeInfo;
        sNFVInfo->_boundaryTypeName = "Surface Normal Fixed Value";
        sNFVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "U");
        sNFVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "U");
        sNFVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "U");
        sNFVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "U");
        sNFVInfo->_createFuns = [&] { return new FITKOFBTySurNorFixedValue; };
        bTypeInfos.append(sNFVInfo);

        //注册Inlet-Outlet
        FITKOFBoundaryTypeInfo* ioInfo = new FITKOFBoundaryTypeInfo;
        ioInfo->_boundaryTypeName = "Inlet-Outlet";
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "alpha");
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "alpha");
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "alpha");
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "alpha");
        ioInfo->_createFuns = [&] { return new FITKOFBTyInletOutlet; };
        bTypeInfos.append(ioInfo);

        //注册Fixed Value速度
        FITKOFBoundaryTypeInfo* fixedValueVInfo = new FITKOFBoundaryTypeInfo;
        fixedValueVInfo->_boundaryTypeName = "Fixed Value";
        fixedValueVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "U");
        fixedValueVInfo->_createFuns = [&] { return new FITKOFBTyVFixedValue; };
        bTypeInfos.append(fixedValueVInfo);

        //注册Fixed Value
        FITKOFBoundaryTypeInfo* fixedValueinfo = new FITKOFBoundaryTypeInfo;
        fixedValueinfo->_boundaryTypeName = "Fixed Value";
        fixedValueinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "alpha");
        fixedValueinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "alpha");
        fixedValueinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "alpha");
        fixedValueinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "alpha");
        fixedValueinfo->_createFuns = [&] { return new FITKOFBTyFixedValue; };
        bTypeInfos.append(fixedValueinfo);

        //注册Fixed Value湍流动能
        FITKOFBoundaryTypeInfo* fixedValueTKEInfo = new FITKOFBoundaryTypeInfo;
        fixedValueTKEInfo->_boundaryTypeName = "Fixed Value";
        fixedValueTKEInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "k");
        fixedValueTKEInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "k");
        fixedValueTKEInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "k");
        fixedValueTKEInfo->_createFuns = [&] { return new FITKOFBTyTKEFixedValue; };
        bTypeInfos.append(fixedValueTKEInfo);

        //注册Fixed Value - 压力
        FITKOFBoundaryTypeInfo* fixedValuePinfo = new FITKOFBoundaryTypeInfo;
        fixedValuePinfo->_boundaryTypeName = "Fixed Value";
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p_rgh");
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "p_rgh");
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "p_rgh");
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "p_rgh");
        fixedValuePinfo->_createFuns = [&] { return new FITKOFBTyPFixedValue; };
        bTypeInfos.append(fixedValuePinfo);

        //注册Outlet Phase Mean Velocity
        FITKOFBoundaryTypeInfo* outletPhaseMeanVinfo = new FITKOFBoundaryTypeInfo;
        outletPhaseMeanVinfo->_boundaryTypeName = "Outlet Phase Mean Velocity";
        outletPhaseMeanVinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "U");
        outletPhaseMeanVinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "U");
        outletPhaseMeanVinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "U");
        outletPhaseMeanVinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "U");
        outletPhaseMeanVinfo->_createFuns = [&] { return new FITKOFBTyOutletPhaseMeanVelocity; };
        bTypeInfos.append(outletPhaseMeanVinfo);

        //注册Variable Height
        FITKOFBoundaryTypeInfo* variableHeightinfo = new FITKOFBoundaryTypeInfo;
        variableHeightinfo->_boundaryTypeName = "Variable Height";
        variableHeightinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "alpha");
        variableHeightinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "alpha");
        variableHeightinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "alpha");
        variableHeightinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "alpha");
        variableHeightinfo->_createFuns = [&] { return new FITKOFBTyVariableHeight; };
        bTypeInfos.append(variableHeightinfo);

        //注册Turbulent Mixing Length Inlet
        FITKOFBoundaryTypeInfo* tMLIInfo = new FITKOFBoundaryTypeInfo;
        tMLIInfo->_boundaryTypeName = "Turbulent Mixing Length Inlet";
        tMLIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "omega");
        tMLIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "omega");
        tMLIInfo->_createFuns = [&] { return new FITKOFBTyTurbulentMixingLengthInlet; };
        bTypeInfos.append(tMLIInfo);

        //注册Turbulent Intensity Inlet
        FITKOFBoundaryTypeInfo* turIIInfo = new FITKOFBoundaryTypeInfo;
        turIIInfo->_boundaryTypeName = "Turbulent Intensity Inlet";
        turIIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "k");
        turIIInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "k");
        turIIInfo->_createFuns = [&] { return new FITKOFBTyTurbulentIntensityInlet; };
        bTypeInfos.append(turIIInfo);

        //注册Low Re Wall Function
        FITKOFBoundaryTypeInfo* lowReWallFunctionInfo = new FITKOFBoundaryTypeInfo;
        lowReWallFunctionInfo->_boundaryTypeName = "Low Re Wall Function";
        lowReWallFunctionInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "k" << "nut");
        lowReWallFunctionInfo->_createFuns = [&] { return new FITKOFBTyVLowReWallFunction; };
        bTypeInfos.append(lowReWallFunctionInfo);
        //将数据添加 
        m_boundaryTypes.insert(FITKOFSolverTypeEnum::FITKOFSolverType::INTER, bTypeInfos);
    }

    void FITKOFBoundaryTypeManager::initCHTMultiRegionSolverBTy()
    {
        QList<FITKOFBoundaryTypeInfo*> bTypeInfos;

        //注册Fixed Flux Pressure
        FITKOFBoundaryTypeInfo* fixedFluxPinfo = new FITKOFBoundaryTypeInfo;
        fixedFluxPinfo->_boundaryTypeName = "Fixed Flux Pressure";
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "p_rgh");
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BMappedWall, QStringList() << "p_rgh");
        fixedFluxPinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "p_rgh");
        fixedFluxPinfo->_createFuns = [&] { return new FITKOFBTyFixedFluxPressure; };
        bTypeInfos.append(fixedFluxPinfo);

        //注册No-Slip
        FITKOFBoundaryTypeInfo* noSlipInfo = new FITKOFBoundaryTypeInfo;
        noSlipInfo->_boundaryTypeName = "No-Slip";
        noSlipInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "U");
        noSlipInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BMappedWall, QStringList() << "U");
        noSlipInfo->_createFuns = [&] { return new FITKOFBTyNoSlip; };
        bTypeInfos.append(noSlipInfo);

        //注册Empty
        FITKOFBoundaryTypeInfo* emptyInfo = new FITKOFBoundaryTypeInfo;
        emptyInfo->_boundaryTypeName = "Empty";
        emptyInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BEmpty, QStringList() << "p_rgh" << "U" << "T");
        emptyInfo->_createFuns = [&] { return new FITKOFBTyEmpty; };
        bTypeInfos.append(emptyInfo);

        //注册Zero Gradient
        FITKOFBoundaryTypeInfo* zeroGradientInfo = new FITKOFBoundaryTypeInfo;
        zeroGradientInfo->_boundaryTypeName = "Zero Gradient";
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BWall, QStringList() << "p_rgh" << "U" << "T");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p_rgh" << "U" << "T");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "p_rgh" << "U" << "T");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "p_rgh" << "U" << "T");
        zeroGradientInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "p_rgh" << "U" << "T");
        zeroGradientInfo->_createFuns = [&] { return new FITKOFBTyZeroGradient; };
        bTypeInfos.append(zeroGradientInfo);

        //注册Total Pressure - Pa
        FITKOFBoundaryTypeInfo* totalPressurePInfo = new FITKOFBoundaryTypeInfo;
        totalPressurePInfo->_boundaryTypeName = "Total Pressure";
        totalPressurePInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p_rgh");
        totalPressurePInfo->_createFuns = [&] { return new FITKOFBTyPTotalPressure; };
        bTypeInfos.append(totalPressurePInfo);

        //注册Total Temperature
        FITKOFBoundaryTypeInfo* totalTemperatureInfo = new FITKOFBoundaryTypeInfo;
        totalTemperatureInfo->_boundaryTypeName = "Total Temperature";
        totalTemperatureInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "T");
        totalTemperatureInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "T");
        totalTemperatureInfo->_createFuns = [&] { return new FITKOFBTyTotalTemperature; };
        bTypeInfos.append(totalTemperatureInfo);

        //注册Fixed Value压力
        FITKOFBoundaryTypeInfo* fixedValuePinfo = new FITKOFBoundaryTypeInfo;
        fixedValuePinfo->_boundaryTypeName = "Fixed Value";
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "p_rgh");
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "p_rgh");
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "p_rgh");
        fixedValuePinfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "p_rgh");
        fixedValuePinfo->_createFuns = [&] { return new FITKOFBTyPFixedValue; };
        bTypeInfos.append(fixedValuePinfo);

        //注册Pressure Inlet-Outlet Velocity
        FITKOFBoundaryTypeInfo* pIOVInfo = new FITKOFBoundaryTypeInfo;
        pIOVInfo->_boundaryTypeName = "Pressure Inlet-Outlet Velocity";
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "U");
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "U");
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "U");
        pIOVInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "U");
        pIOVInfo->_createFuns = [&] { return new FITKOFBTyPressureIOVelocity; };
        bTypeInfos.append(pIOVInfo);

        //注册Inlet-Outlet
        FITKOFBoundaryTypeInfo* ioInfo = new FITKOFBoundaryTypeInfo;
        ioInfo->_boundaryTypeName = "Inlet-Outlet";
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "T");
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureOutlet, QStringList() << "T");
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BOutflow, QStringList() << "T");
        ioInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BPressureInlet, QStringList() << "T");
        ioInfo->_createFuns = [&] { return new FITKOFBTyInletOutlet; };
        bTypeInfos.append(ioInfo);

        //注册Coupled Temperature And Radiation
        FITKOFBoundaryTypeInfo* ctarInfo = new FITKOFBoundaryTypeInfo;
        ctarInfo->_boundaryTypeName = "Coupled Temperature and Radiation";
        ctarInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BMappedWall, QStringList() << "T");
        ctarInfo->_createFuns = [&] { return new FITKOFBTyCoupledTemperatureAndRadiation; };
        bTypeInfos.append(ctarInfo);

        //注册Surface Normal Fixed Value
        FITKOFBoundaryTypeInfo* snfvInfo = new FITKOFBoundaryTypeInfo;
        snfvInfo->_boundaryTypeName = "Surface Normal Fixed Value";
        snfvInfo->_enableBoundary.insert(FITKOFSolverTypeEnum::FITKOFBoundaryType::BVelocityInlet, QStringList() << "U");
        snfvInfo->_createFuns = [&] { return new FITKOFBTySurNorFixedValue; };
        bTypeInfos.append(snfvInfo);

        //将数据添加 
        m_boundaryTypes.insert(FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION, bTypeInfos);
    }
}

