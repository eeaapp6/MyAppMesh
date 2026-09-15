/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFRadiationPropDictWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiation.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationAbsorptionModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationScatterModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationSunDirection.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationSunLoad.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamString.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include <QFileInfo>
#include <QRegularExpression>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFRadiationPropDictWriter::FITKOFRadiationPropDictWriter()
    {
    }
    FITKOFRadiationPropDictWriter::~FITKOFRadiationPropDictWriter()
    {
    }

    void FITKOFRadiationPropDictWriter::setFilePath(const QString& filePath)
    {
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        FITKOFAbstractDictWriter::setFilePath(filePath);
    }

    bool FITKOFRadiationPropDictWriter::run()
    {
        //获取物理数据和网格数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        Interface::FITKUnstructuredFluidMeshVTK* meshData = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
        if (!physicsData || !meshData) return false;
        if (!physicsData->getRadiation()) return true;
        //获取网格区域数据
        int count = meshData->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKFluidRegionsMesh* regionMesh = meshData->getDataByIndex(i);
            if (!regionMesh) continue;
            Interface::FITKUnstructuredMeshVTK* mesh = regionMesh->getFieldMesh();
            if (!mesh) continue;
            //获取路径和区域类型
            QString path = mesh->getUserData(Core::FITKUserData::FITKUserRole + 2).toString();
            Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType typeRegion = physicsData->getRegionMeshType(regionMesh->getDataObjectID());
            if (path.isEmpty() || typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::None) continue;
            QStringList str = path.split('/');
            if (str.size() < 3)continue;
            QString dirName = str.last().isEmpty() ? str.at(str.size() - 3) : str.at(str.size() - 2);
            if (dirName.toLower() == "constant")
                m_fileName = QString("%1/constant/radiationProperties").arg(m_filePath);
            else
                m_fileName = QString("%1/constant/%2/radiationProperties").arg(m_filePath).arg(dirName);
            //判断文件是否存在
            if (m_fileName.isEmpty())return false;
            //获取字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //写出FoamFile
            if (dirName.toLower() == "constant")
                this->dictObjectFoamFile(QString("constant"));
            else
                this->dictObjectFoamFile(QString("constant/%1").arg(dirName));
            //设置写出的网格区域数据
            bool Ok = false;
            if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Fluid)
                Ok = this->radiationFluidPropDictW();
            else if (typeRegion == Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType::Solid)
                Ok = this->radiationSolidPropDictW();

            //写出字典文件
            if (Ok)
                Ok = this->writeDictFile();

            //关闭字典工程
            this->closeDictObject();
            if (!Ok)
                return false;
        }
        return true;
    }

    QString FITKOFRadiationPropDictWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFRadiationPropDictWriter::dictObjectFoamFile(QString location)
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", QString("\"%1\"").arg(location));
        group->append("object", "radiationProperties");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFRadiationPropDictWriter::radiationFluidPropDictW()
    {
        //获取辐射数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData) return false;
        Interface::FITKOFRadiation* radiation = physicsData->getRadiation();
        if (!radiation) return false;
        Core::FITKParameter* radiationPara = radiation->getRadiationParam();
        if (!radiationPara) return false;
        //获取是否启用辐射
        Core::FITKParamBoolGroup* enablepara = dynamic_cast<Core::FITKParamBoolGroup*>(radiationPara->getDataByName("Enable Radiation"));
        if (!enablepara) return false;
        bool isEnable = enablepara->getValue();
        Core::FITKParameter* enablePara = enablepara->getValueGroup();
        if (!enablePara) return false;
        bool ok = false;
        int freqValue = enablePara->getIntParaByName("Solution Frequency", ok);
        if (!ok) return false;
        //写出启用辐射和频率有关参数
        m_objectDict->append("radiation", isEnable);
        m_objectDict->append("solverFreq", freqValue);
        //写出流体的辐射模型
        this->radiationModelParaW();
        m_objectDict->append("transmissivityModel", "none");
        //写出太阳源辐射数据参数
        this->solarLoadPropW();
        return true;
    }

    bool FITKOFRadiationPropDictWriter::radiationSolidPropDictW()
    {
        //获取辐射数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData) return false;
        Interface::FITKOFRadiation* radiation = physicsData->getRadiation();
        if (!radiation) return false;
        Core::FITKParameter* radiationPara = radiation->getRadiationParam();
        if (!radiationPara) return false;
        //获取是否启用辐射
        Core::FITKParamBoolGroup* enablepara = dynamic_cast<Core::FITKParamBoolGroup*>(radiationPara->getDataByName("Enable Radiation"));
        if (!enablepara) return false;
        bool isEnable = enablepara->getValue();
        Core::FITKParameter* enablePara = enablepara->getValueGroup();
        if (!enablePara) return false;
        bool ok = false;
        int freqValue = enablePara->getIntParaByName("Solution Frequency", ok);
        if (!ok) return false;
        //写出启用辐射和频率有关参数
        m_objectDict->append("radiation", isEnable);
        m_objectDict->append("solverFreq", freqValue);
        //写出固体的固定参数
        m_objectDict->append("radiationModel", "opaqueSolid");
        m_objectDict->append("absorptionEmissionModel", "none");
        m_objectDict->append("scatterModel", "none");
        m_objectDict->append("transmissivityModel", "none");
        return true;
    }

    bool FITKOFRadiationPropDictWriter::radiationModelParaW()
    {
        //获取辐射数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData) return false;
        Interface::FITKOFRadiation* radiation = physicsData->getRadiation();
        if (!radiation) return false;
        Interface::FITKOFAbsRadiationModel* radModel = radiation->getRadiationModel();
        Interface::FITKOFAbsRadiationAbsorptionModel* radAbsorptionModel = radiation->getAbsorptionModel();
        Interface::FITKOFAbsRadiationScatterModel* radScatterModel = radiation->getScatterModel();
        Interface::FITKOFRadiationSolarLoad * solarLoad = radiation->getSolarLoad();
        if (!radModel || !radAbsorptionModel || !radScatterModel) return false;
        //写出辐射模型参数
        QString nameRadModel = radModel->getDataObjectName();
        QString radModelType;
        if (nameRadModel == "none")
            radModelType = "node";
        else if (nameRadModel == "P1")
            radModelType = "P1";
        else if (nameRadModel == "Discrete Ordinates")
            radModelType = "fvDOM";
        else if (nameRadModel == "Surface to Surface")
            radModelType = "viewFactor";
        m_objectDict->append("radiationModel", radModelType);
        if (radModel->getRadiationModelPara() && radModel->getRadiationModelPara()->getDataCount() > 0)
        {
            bool isUseSolarLoad = false;
            if (solarLoad)
            {
                bool ok = false;
                Core::FITKParameter * solarLoadPara = solarLoad->getRadiationSolarLoadParams();
                if (solarLoadPara)
                    isUseSolarLoad = solarLoadPara->getBoolParaByName("Enable Solar load", ok);
            }

            DICT::FITKDictGroup* coeffsGroup = new DICT::FITKDictGroup;
            this->parametersW(radModel->getRadiationModelPara(), coeffsGroup);
            coeffsGroup->append("useSolarLoad", isUseSolarLoad);
            m_objectDict->append(QString("%1Coeffs").arg(radModelType), coeffsGroup);
        }
        //写出吸收模型数据
        QString nameAbsorptionModel = radAbsorptionModel->getDataObjectName();
        QString radAbsorptionModelType;
        if (nameAbsorptionModel == "none")
            radAbsorptionModelType = "node";
        else if (nameAbsorptionModel == "Constant")
            radAbsorptionModelType = "constantAbsorptionEmission";
        m_objectDict->append("absorptionEmissionModel", radAbsorptionModelType);
        if (radAbsorptionModel->getRadiationAbsorptionModelPara() && radAbsorptionModel->getRadiationAbsorptionModelPara()->getDataCount() > 0)
        {
            DICT::FITKDictGroup* coeffsGroup = new DICT::FITKDictGroup;
            this->parametersW(radAbsorptionModel->getRadiationAbsorptionModelPara(), coeffsGroup);
            m_objectDict->append(QString("%1Coeffs").arg(radAbsorptionModelType), coeffsGroup);
        }
        //写出散射模型数据
        QString nameScatterModel = radScatterModel->getDataObjectName();
        QString radScatterModelType;
        if (nameScatterModel == "none")
            radScatterModelType = "node";
        else if (nameScatterModel == "Constant")
            radScatterModelType = "constantScatter";
        m_objectDict->append("scatterModel", radScatterModelType);
        if (radScatterModel->getRadiationScatterModelPara() && radScatterModel->getRadiationScatterModelPara()->getDataCount() > 0)
        {
            DICT::FITKDictGroup* coeffsGroup = new DICT::FITKDictGroup;
            this->parametersW(radScatterModel->getRadiationScatterModelPara(), coeffsGroup);
            m_objectDict->append(QString("%1Coeffs").arg(radScatterModelType), coeffsGroup);
        }
        return true;
    }

    bool FITKOFRadiationPropDictWriter::solarLoadPropW()
    {
        //获取辐射数据
        Interface::FITKOFPhysicsData* physicsData = FITKAPP->getGlobalData()->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData) return false;
        Interface::FITKOFRadiation* radiation = physicsData->getRadiation();
        if (!radiation) return false;
        Interface::FITKOFRadiationSolarLoad* solarLoad = radiation->getSolarLoad();
        if (!solarLoad) return false;
        //获取太阳源参数数据
        Core::FITKParameter * solarLoadPara = solarLoad->getRadiationSolarLoadParams();
        if (!solarLoadPara) return false;
        bool ok = false;
        bool isEnable = solarLoadPara->getBoolParaByName("Enable Solar load", ok);
        bool reflectedRays = solarLoadPara->getBoolParaByName("Reflected Rays", ok);
        m_objectDict->append("useSolarLoad", isEnable);
        DICT::FITKDictGroup* coeffsGroup = new DICT::FITKDictGroup;
        m_objectDict->append("solarLoadCoeffs", coeffsGroup);
        //写出太阳源方向
        Interface::FITKOFAbsRadiationSunDirection * direSun = solarLoad->getRadiationSunDirection();
        if (!direSun) return false;
        QString direSunModelName = direSun->getDataObjectName();
        QString direSunModelType;
        if (direSunModelName == "Constant")
            direSunModelType = "sunDirConstant";
        else if (direSunModelName == "Sun Tracking")
            direSunModelType = "sunDirTracking";
        coeffsGroup->append("sunDirectionModel", direSunModelType);
        this->sunDirectionParaW(direSun, coeffsGroup);
        //写出太阳源载荷模型
        Interface::FITKOFAbsRadiationSunLoad* sunLoad = solarLoad->getRadiationSunLoad();
        if (!sunLoad) return false;
        QString sunLoadName = sunLoad->getDataObjectName();
        QString sunLoadType;
        if (sunLoadName == "Constant")
            sunLoadType = "sunLoadConstant";
        coeffsGroup->append("sunLoadModel", sunLoadType);
        this->sunLoadParaW(sunLoad, coeffsGroup);
        //写出其他参数
        coeffsGroup->append("C", 0.0);
        coeffsGroup->append("useVFbeamToDiffuse", false);
        coeffsGroup->append("spectralDistribution", "(1)");
        coeffsGroup->append("wallCoupled", true);
        coeffsGroup->append("solidCoupled", false);
        //写出reflected Rays
        coeffsGroup->append("useReflectedRays", reflectedRays);
        if (reflectedRays)
        {
            DICT::FITKDictGroup* reflectingGroup = new DICT::FITKDictGroup;
            coeffsGroup->append("reflecting", reflectingGroup);
        }
        return false;
    }

    bool FITKOFRadiationPropDictWriter::sunDirectionParaW(Interface::FITKOFAbsRadiationSunDirection * direSun, DICT::FITKDictGroup * group)
    {
        if (!group || !direSun) return false;
        //匹配名称
        auto unitConversion = [&](QString name) {
            if (name == "Direction")
                return "sunDirection";
            else if (name == "Local Meridian [h]")
                return "localStandardMeridian";
            else if (name == "Latitude [deg]")
                return "latitude";
            else if (name == "Up Direction [-]")
                return "gridUp";
            else if (name == "East Direction [-]")
                return "gridEast";
            else if (name == "Longitude [deg]")
                return "longitude";
            else if (name == "Start Day [day]")
                return "startDay";
            else if (name == "Start Time [h]")
                return "startTime";
            else if (name == "Tracking Update interval [h]")
                return "sunTrackingUpdateInterval";
        };
        //写出参数
        Core::FITKParameter* para = direSun->getRadiationSunDirectionPara();
        if (!para)return false;
        int count = para->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Core::FITKAbstractEasyParam* paraData = para->getDataByIndex(i);
            if (!paraData) continue;
            QString name = unitConversion(paraData->getDataObjectName());
            if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            {
                Core::FITKParamDouble* p = dynamic_cast<Core::FITKParamDouble*>(paraData);
                if (!p)continue;
                group->append(name, p->getValue());
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
            {
                Core::FITKParamDoubleList* p = dynamic_cast<Core::FITKParamDoubleList*>(paraData);
                if (!p)continue;
                int pCount = p->getCount();
                QStringList valueList;
                for (int i = 0; i < pCount; ++i)
                    valueList.append(DICT::FITKDictValue::doubleAdaptationTool(p->getValue(i)));
                group->append(name, QString("(%1)").arg(valueList.join(' ')));
            }
        }

        return true;
    }

    bool FITKOFRadiationPropDictWriter::sunLoadParaW(Interface::FITKOFAbsRadiationSunLoad * loadSun, DICT::FITKDictGroup * group)
    {
        if (!loadSun || !group) return false;
        //匹配名称
        auto unitConversion = [&](QString name) {
            if (name == "Direct [W/m2]")
                return "directSolarRad";
            else if (name == "Diffuse [W/m2]")
                return "diffuseSolarRad";
        };
        //写出参数
        Core::FITKParameter* para = loadSun->getRadiationSunLoadPara();
        if (!para)return false;
        int count = para->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Core::FITKAbstractEasyParam* paraData = para->getDataByIndex(i);
            if (!paraData) continue;
            QString name = unitConversion(paraData->getDataObjectName());
            if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            {
                Core::FITKParamDouble* p = dynamic_cast<Core::FITKParamDouble*>(paraData);
                if (!p)continue;
                group->append(name, p->getValue());
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
            {
                Core::FITKParamDoubleList* p = dynamic_cast<Core::FITKParamDoubleList*>(paraData);
                if (!p)continue;
                int pCount = p->getCount();
                QStringList valueList;
                for (int i = 0; i < pCount; ++i)
                    valueList.append(DICT::FITKDictValue::doubleAdaptationTool(p->getValue(i)));
                group->append(name, QString("(%1)").arg(valueList.join(' ')));
            }
        }
        return false;
    }

    bool FITKOFRadiationPropDictWriter::parametersW(Core::FITKParameter * para, DICT::FITKDictGroup * group)
    {
        if (!para || !group)return false;
        //提取名称和单位数据
        auto unitConversion = [&](QString name, QString& vName, QString& vValue) {
            if (name.isEmpty()) return false;
            QRegExp unitRex("(?:\\[)(.*)(?:\\])");
            QRegExp nameRex("(.*) [");
            QString unitStr;
            QStringList nameStrList = name.split(" [");
            int pos = 0;
            if (name.indexOf(unitRex) >= 1) //带中括号为0，不带中括号为1
                unitStr = unitRex.cap(1);
            else
                return false;
            if (nameStrList.size() >= 1) {
                vName = nameStrList.at(0);
            }
            //获取单位
            if (unitStr == "1/m")
                vValue = "[0 -1 0 0 0 0 0]";
            else if (unitStr == "W/m3")
                vValue = "[1 -1 -3 0 0 0 0]";
            else if (unitStr == "-")
                vValue = "[0 0 0 0 0 0 0]";


            return true;
        };

        int count = para->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Core::FITKAbstractEasyParam* paraData = para->getDataByIndex(i);
            if (!paraData)continue;
            QString paraName = paraData->getDataObjectName();
            QString paraValue;
            if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPBool)
            {
                Core::FITKParamBool* p = dynamic_cast<Core::FITKParamBool*>(paraData);
                if (!p)continue;
                if (p->getValue())
                    paraValue = "true";
                else
                    paraValue = "false";
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            {
                Core::FITKParamDouble* p = dynamic_cast<Core::FITKParamDouble*>(paraData);
                if (!p)continue;
                paraValue = DICT::FITKDictValue::doubleAdaptationTool(p->getValue());
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPInt)
            {
                Core::FITKParamInt* p = dynamic_cast<Core::FITKParamInt*>(paraData);
                if (!p)continue;
                paraValue = QString::number(p->getValue());
            }
            else if (paraData->getParamType() == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPString)
            {
                Core::FITKParamString* p = dynamic_cast<Core::FITKParamString*>(paraData);
                if (!p)continue;
                paraValue = p->getValue();
            }
            QString vName, vValue;
            if (!unitConversion(paraName, vName, vValue))
            {
                group->append(paraData->getDataObjectName(), paraValue);
            }
            else
            {
                group->append(vName, QString("%1 %2 %3").arg(vName).arg(vValue).arg(paraValue));
            }
        }
        return true;
    }

}



