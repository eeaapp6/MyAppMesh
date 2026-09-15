/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFChangeDictionaryDictW.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundary.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFBoundaryType.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFInitialConditions.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFChangeDictionaryDictW::FITKOFChangeDictionaryDictW()
    {
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return;
        //获取求解器
        _solver = physicsData->getSolver();
        //获取离散数据
        _boundaryMgr = physicsData->getBoundaryManager();
        //获取initial
        _initialCond = physicsData->getInitialConditions();
    }
    FITKOFChangeDictionaryDictW::~FITKOFChangeDictionaryDictW()
    {
    }

    void FITKOFChangeDictionaryDictW::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/changeDictionaryDict").arg(filePath);
    }

    bool FITKOFChangeDictionaryDictW::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取字典工程
        this->openDictObject();
        if (!m_objectDict)return false;
        //写出FoamFile
        this->dictObjectFoamFile();
        //设置写出的网格区域数据
        bool Ok = this->changeDictBoundaryFieldW();
        //写出字典文件
        if (Ok)
            Ok = this->writeDictFile();

        //关闭字典工程
        this->closeDictObject();
        return Ok;
    }

    QString FITKOFChangeDictionaryDictW::getFileName()
    {
        return m_fileName;
    }

    void FITKOFChangeDictionaryDictW::dictObjectFoamFile()
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"system\"");
        group->append("object", "changeDictionaryDict");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFChangeDictionaryDictW::changeDictBoundaryFieldW()
    {
        if (!m_objectDict || !_boundaryMgr || !_initialCond) return false;
        int count = _boundaryMgr->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFBoundary* boundary = _boundaryMgr->getDataByIndex(i);
            if (!boundary)continue;
            //获取网格边界组
            Interface::FITKBoundaryMeshVTK* boundaryMesh = boundary->getMeshBoundary();
            if (!boundaryMesh)continue;
            //获取组名
            QString boundMeshName = boundaryMesh->getDataObjectName();

            //写出flow
            int count = boundary->getFlowCount();
            for (int i = 0; i < count; ++i)
            {
                QString nameV = boundary->getFlowVariableName(i);
                Interface::FITKOFAbsBoundaryType* para = boundary->getFlowVBType(i);
                this->boundaryFieldW(boundMeshName, nameV, para);
            }
            //写出turbulence
            count = boundary->getTurbulenceCount();
            for (int i = 0; i < count; ++i)
            {
                QString nameV = boundary->getTurbulenceVariableName(i);
                Interface::FITKOFAbsBoundaryType* para = boundary->getTurbulenceVBType(i);
                this->boundaryFieldW(boundMeshName, nameV, para);
            }
            //写出phases
            count = boundary->getPhasesCount();
            for (int i = 0; i < count; ++i)
            {
                QString nameV = boundary->getPhasesVariableName(i);
                Interface::FITKOFAbsBoundaryType* para = boundary->getPhasesVBType(i);
                this->boundaryFieldW(boundMeshName, nameV, para);
            }
        }
        return true;
    }

    bool FITKOFChangeDictionaryDictW::boundaryFieldW(QString boundMeshName, QString nameV, Interface::FITKOFAbsBoundaryType* boundPara)
    {
        if (!m_objectDict || !boundPara)return false;
        DICT::FITKDictValue* vPara = m_objectDict->value(nameV);
        DICT::FITKDictGroup* boundaryField = nullptr;
        bool ok = false;
        if (!vPara)
        {
            DICT::FITKDictGroup* variable = new DICT::FITKDictGroup;
            m_objectDict->append(nameV, variable);
            boundaryField = new DICT::FITKDictGroup;
            variable->append("boundaryField", boundaryField);
        }
        else
        {
            DICT::FITKDictGroup* variable = vPara->toGroup(ok);
            if (!variable)return false;
            DICT::FITKDictValue* boundF = variable->value("boundaryField");
            boundaryField = boundF->toGroup(ok);
        }
        if (!boundaryField)return false;
        DICT::FITKDictGroup* boundaryG = new DICT::FITKDictGroup;
        boundaryField->append(boundMeshName, boundaryG);
        //写出参数
        this->paraBTyW(boundPara, boundaryG, nameV);
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* boundaryG, QString nameV, int regionMeshID, int isBasic)
    {
        if (!boundPara || !boundaryG || nameV.isEmpty())return false;
        QString boundTypeName = boundPara->getDataObjectName();
        bool ok = false;
        if (boundTypeName == "Zero Gradient")
            ok = this->paraZeroGradientBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Fixed Flux Pressure")
            ok = this->paraFixedFluxPressureBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Fixed Value")
            ok = this->paraFixedValueBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Standard Wall Function")
            ok = this->paraStandardWallFunctionBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        else if (boundTypeName == "Low Re Wall Function")
            ok = this->paraVLowReWallFunctionBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        else if (boundTypeName == "No-Slip")
            ok = this->paraNoSlipBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Turbulent Intensity Inlet")
            ok = this->paraTurbulentIntensityInletBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        else if (boundTypeName == "Turbulent Mixing Length Inlet")
            ok = this->paraTurbulentMixingLengthInletBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        else if (boundTypeName == "Total Pressure")
            ok = this->paraTotalPressureBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Pressure Inlet-Outlet Velocity")
            ok = this->paraPressureIOVelocityBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        else if (boundTypeName == "Surface Normal Fixed Value")
            ok = this->paraSurNorFixedValueBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Symmetry")
            ok = this->paraSymmetryBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Outlet Phase Mean Velocity")
            ok = this->paraOutletPhaseMeanVelocityBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        else if (boundTypeName == "Inlet-Outlet")
            ok = this->paraInletOutletBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        else if (boundTypeName == "Variable Height")
            ok = this->paraVariableHeightBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Total Temperature")
            ok = this->paraTotalTemperatureBTyW(boundPara, boundaryG, nameV);
        else if (boundTypeName == "Coupled Temperature and Radiation")
            ok = this->paraCoupledTemperatureAndRadiationBTyW(boundPara, boundaryG, nameV, regionMeshID, isBasic);
        return ok;
    }

    bool FITKOFChangeDictionaryDictW::paraZeroGradientBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "zeroGradient");
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraFixedFluxPressureBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "fixedFluxPressure");
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraFixedValueBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "fixedValue");
        Core::FITKParamDoubleList* value = dynamic_cast<Core::FITKParamDoubleList*>(para->getDataByName("Value [m/s]"));
        if (value)
        {
            QString uniformValue = QString("uniform (%1 %2 %3)").arg(value->getValue(0)).arg(value->getValue(1)).arg(value->getValue(2));
            group->append("value", uniformValue);
        }
        else
        {
            bool ok = false;
            group->append("value", QString("uniform %1").arg(para->getDoubleParaByName("Value [m2/s2]", ok)));
        }
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraStandardWallFunctionBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        if (vName == "k")
            group->append("type", "kqRWallFunction");
        else if (vName == "omega")
            group->append("type", "omegaWallFunction");
        else if (vName == "nut")
            group->append("type", "nutUWallFunction");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        group->append("value", value);
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraVLowReWallFunctionBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        if (vName == "nut")
            group->append("type", "nutLowReWallFunction");
        else if (vName == "k")
            group->append("type", "kLowReWallFunction");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        group->append("value", value);
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraNoSlipBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "noSlip");
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraTurbulentIntensityInletBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "turbulentIntensityKineticEnergyInlet");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        group->append("value", value);
        bool ok = false;
        group->append("intensity", para->getDoubleParaByName("Intensity [-]", ok));
        if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
        {
            group->append("phi", "rhoPhi");
        }
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraTurbulentMixingLengthInletBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "turbulentMixingLengthFrequencyInlet");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        group->append("value", value);
        bool ok = false;
        group->append("mixingLength", para->getDoubleParaByName("Mixing Length [m]", ok));
        if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
        {
            group->append("phi", "rhoPhi");
        }
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraTotalPressureBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty() || !_solver)return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "totalPressure");
        bool ok = false;
        double p0 = para->getDoubleParaByName("p0 [m2/s2]", ok);
        group->append("p0", QString("uniform %1").arg(p0));
        group->append("gamma", 0);
        if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
        {
            group->append("rho", "rho");
            group->append("phi", "rhoPhi");
        }
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraPressureIOVelocityBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "pressureInletOutletVelocity");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        group->append("value", value);
        if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
        {
            group->append("phi", "rhoPhi");
        }
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraSurNorFixedValueBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "surfaceNormalFixedValue");
        bool ok = false;
        double refValue = para->getDoubleParaByName("Reference Value [m/s]", ok);
        group->append("refValue", QString("uniform %1").arg(-refValue));
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraSymmetryBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        group->append("type", "symmetry");
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraOutletPhaseMeanVelocityBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "outletPhaseMeanVelocity");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        group->append("value", value);
        bool ok = false;
        group->append("UnMean", para->getDoubleParaByName("Umean [m/s]", ok));
        if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
        {
            group->append("alpha", "alpha.phase1");
        }
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraInletOutletBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "inletOutlet");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        bool ok = false;
        group->append("inletValue", QString("uniform %1").arg(para->getDoubleParaByName("Inlet Value [-]", ok)));
        if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
            group->append("phi", "rhoPhi");
        else if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::CHT_MULTI_REGION)
            group->append("value", value);
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraVariableHeightBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "variableHeightFlowRate");
        if (_solver->getSolverType() == Interface::FITKOFSolverTypeEnum::FITKOFSolverType::INTER)
        {
            group->append("phi", "rhoPhi");
        }
        bool ok = false;
        group->append("upperBound", para->getDoubleParaByName("Upper Bound [-]", ok));
        group->append("lowerBound", para->getDoubleParaByName("Lower Bound [-]", ok));
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraTotalTemperatureBTyW(Interface::FITKOFAbsBoundaryType * boundPara, DICT::FITKDictGroup * group, QString vName)
    {
        if (!boundPara || !group || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "totalTemperature");
        bool ok = false;
        group->append("T0", QString("uniform %1").arg(para->getDoubleParaByName("T0[K]", ok)));
        group->append("gamma", "1.124");
        return true;
    }

    bool FITKOFChangeDictionaryDictW::paraCoupledTemperatureAndRadiationBTyW(Interface::FITKOFAbsBoundaryType * boundPara, DICT::FITKDictGroup * group, QString vName, int regionMeshID, int isBasic)
    {
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!boundPara || !group || !physicsData || vName.isEmpty())return false;
        Core::FITKParameter* para = boundPara->getBoundaryTypePara();
        if (!para)return false;
        group->append("type", "compressible::turbulentTemperatureRadCoupledMixed");
        //初始化边界参数
        QString value;
        if (regionMeshID > 0 && isBasic == false)
            value = this->getInitialRegionsParaValue(vName, regionMeshID);
        else
            value = this->getInitialBasicParaValue(vName);
        //设置数据
        group->append("value", value);
        group->append("qrNbr", "none");
        group->append("qr", "none");
        group->append("neighbourFieldName", "T");
        group->append("alphaAni", "Anialpha");
        Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType regionType = physicsData->getRegionMeshType(regionMeshID);
        if (regionType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
            group->append("kappaMethod", "fluidThermo");
        else if (regionType == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
            group->append("kappaMethod", "solidThermo");
        group->append("kappa", "none");
        group->append("Tnbr", "T");
        bool ok = false;
        if (para->getBoolParaByName("Resistance", ok))
        {
            group->append("thicknessLayers", QString("(%1)").arg(para->getDoubleParaByName("thicknessLayers", ok)));
            group->append("kappaLayers", QString("(%1)").arg(para->getDoubleParaByName("kappaLayers", ok)));
        }
        return true;
    }


    QString FITKOFChangeDictionaryDictW::getInitialBasicParaValue(QString vName)
    {
        if (!_initialCond) return QString();
        Core::FITKParameter* vPara = _initialCond->getBasicData();
        if (!vPara)return QString();
        Core::FITKAbstractEasyParam* p = vPara->getDataByName(vName);
        if (!p) return QString();
        Core::FITKAbstractEasyParam::FITKEasyParamType type = p->getParamType();
        if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
        {
            Core::FITKParamDouble* pData = dynamic_cast<Core::FITKParamDouble*>(p);
            if (!pData)return QString();
            return QString("uniform %1").arg(pData->getValue());
        }
        else if (p->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
        {
            Core::FITKParamDoubleList* pData = dynamic_cast<Core::FITKParamDoubleList*>(p);
            if (!pData)return QString();
            int count = pData->getCount();
            QStringList strList;
            for (int i = 0; i < count; ++i)
                strList.append(QString::number(pData->getValue(i)));
            return QString("uniform (%1)").arg(strList.join(' '));
        }
        return QString();
    }

    QString FITKOFChangeDictionaryDictW::getInitialRegionsParaValue(QString vName, int regionMeshID)
    {
        if (!_initialCond) return QString();
        Interface::FITKOFInitPropRegions* regionsProp = _initialCond->getInitPropRegionByRegionMeshID(regionMeshID);
        if (!regionsProp) return QString();
        Core::FITKParameter *vPara = regionsProp->getRegionPara();
        if (!vPara)return QString();
        Core::FITKAbstractEasyParam* p = vPara->getDataByName(vName);
        if (!p) return QString();
        Core::FITKAbstractEasyParam::FITKEasyParamType type = p->getParamType();
        if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
        {
            Core::FITKParamDouble* pData = dynamic_cast<Core::FITKParamDouble*>(p);
            if (!pData)return QString();
            return QString("uniform %1").arg(pData->getValue());
        }
        else if (p->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
        {
            Core::FITKParamDoubleList* pData = dynamic_cast<Core::FITKParamDoubleList*>(p);
            if (!pData)return QString();
            int count = pData->getCount();
            QStringList strList;
            for (int i = 0; i < count; ++i)
                strList.append(QString::number(pData->getValue(i)));
            return QString("uniform (%1)").arg(strList.join(' '));
        }
        return QString();
    }
}



