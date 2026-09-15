/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorRadiation.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiation.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationAbsorptionModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationScatterModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationSunDirection.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationSunLoad.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRadiationParaManager.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorRadiation::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorRadiation";
    }

    bool FITKFlowOFHDF5AdaptorRadiation::adaptR()
    {
        _radiation = dynamic_cast<Interface::FITKOFRadiation*>(_dataObj);
        if (!_reader || !_radiation) return false;
        if (!readNDataObject(_radiation, *_h5Group)) return false;
        bool isR = true;
        //读取数据
        isR &= this->radiationParamR();
        isR &= this->radiationModelR();
        isR &= this->absorptionModelR();
        isR &= this->scatterModelR();
        isR &= this->solarLoadR();
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::adaptW()
    {
        _radiation = dynamic_cast<Interface::FITKOFRadiation*>(_dataObj);
        if (!_writer || !_radiation) return false;
        if (!writeNDataObject(_radiation, *_h5Group)) return false;
        bool isW = true;
        //写出数据
        isW &= this->radiationParamW();
        isW &= this->radiationModelW();
        isW &= this->absorptionModelW();
        isW &= this->scatterModelW();
        isW &= this->solarLoadW();
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::radiationParamW()
    {
        if (!_radiation || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出辐射参数
        Core::FITKParameter * param = _radiation->getRadiationParam();
        if (!param) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "RadiationPara");
        if (std::get<0>(subH5Group) == false) return false;
        parameterToolW(std::get<1>(subH5Group), param);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::radiationParamR()
    {
        if (!_radiation || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取辐射参数
        Core::FITKParameter * param = _radiation->getRadiationParam();
        if (!param) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "RadiationPara");
        if (std::get<0>(subH5Group) == false) return false;
        parameterToolR(std::get<1>(subH5Group), param);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::radiationModelW()
    {
        if (!_radiation || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出辐射模型
        Interface::FITKOFAbsRadiationModel * radiationModel = _radiation->getRadiationModel();
        if (!radiationModel) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "RadiationModel");
        if (std::get<0>(subH5Group) == false) return false;
        //写出辐射模型类型
        QString name = radiationModel->getDataObjectName();
        tool->writeGroupAttrStr(std::get<1>(subH5Group), "ModelType", name.toStdString());
        this->writeNDataObject(radiationModel, std::get<1>(subH5Group));
        Core::FITKParameter * param = radiationModel->getRadiationModelPara();
        if (param == nullptr) return false;
        parameterToolW(std::get<1>(subH5Group), param);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::radiationModelR()
    {
        if (!_radiation || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取创建工具管理器
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsManager) return false;
        Interface::FITKOFRadiationParaManager* radiationParaMgr = physicsManager->getRadiationParaManager();
        if (!radiationParaMgr) return false;
        //读取辐射模型参数数据
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "RadiationModel");
        if (std::get<0>(subH5Group) == false) return false;
        std::string modelTypeName;
        tool->readGroupStr(std::get<1>(subH5Group), "ModelType", modelTypeName);
        Interface::FITKOFRadiationModelInfo info = radiationParaMgr->getRadiationModelInfo(QString::fromStdString(modelTypeName));
        if (info._radiationModelName.isEmpty()) return false;
        Interface::FITKOFAbsRadiationModel* radiationModel = info._createFuns();
        this->readNDataObject(radiationModel, std::get<1>(subH5Group));
        parameterToolR(std::get<1>(subH5Group), radiationModel->getRadiationModelPara());
        _radiation->setRadiationModel(radiationModel);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::absorptionModelW()
    {
        if (!_radiation || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出辐射模型
        Interface::FITKOFAbsRadiationAbsorptionModel * absorptionModel = _radiation->getAbsorptionModel();
        if (!absorptionModel) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "AbsorptionModel");
        if (std::get<0>(subH5Group) == false) return false;
        //写出吸收系数类型
        QString name = absorptionModel->getDataObjectName();
        tool->writeGroupAttrStr(std::get<1>(subH5Group), "ModelType", name.toStdString());
        this->writeNDataObject(absorptionModel, std::get<1>(subH5Group));
        Core::FITKParameter * param = absorptionModel->getRadiationAbsorptionModelPara();
        if (param == nullptr) return false;
        parameterToolW(std::get<1>(subH5Group), param);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::absorptionModelR()
    {
        if (!_radiation || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取创建工具管理器
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsManager) return false;
        Interface::FITKOFRadiationParaManager* radiationParaMgr = physicsManager->getRadiationParaManager();
        if (!radiationParaMgr) return false;
        //读取辐射模型参数数据
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "AbsorptionModel");
        if (std::get<0>(subH5Group) == false) return false;
        std::string modelTypeName;
        tool->readGroupStr(std::get<1>(subH5Group), "ModelType", modelTypeName);
        Interface::FITKOFRadiationAbsorptionModelInfo info = radiationParaMgr->getRadiationAbsorptionModelInfo(QString::fromStdString(modelTypeName));
        if (info._radiationAbsorptionModelName.isEmpty()) return false;
        Interface::FITKOFAbsRadiationAbsorptionModel* absorptionModel = info._createFuns();
        this->readNDataObject(absorptionModel, std::get<1>(subH5Group));
        parameterToolR(std::get<1>(subH5Group), absorptionModel->getRadiationAbsorptionModelPara());
        _radiation->setAbsorptionModel(absorptionModel);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::scatterModelW()
    {
        if (!_radiation || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出辐射模型
        Interface::FITKOFAbsRadiationScatterModel * scatterModel = _radiation->getScatterModel();
        if (!scatterModel) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "ScatterModel");
        if (std::get<0>(subH5Group) == false) return false;
        //写出吸收系数类型
        QString name = scatterModel->getDataObjectName();
        tool->writeGroupAttrStr(std::get<1>(subH5Group), "ModelType", name.toStdString());
        this->writeNDataObject(scatterModel, std::get<1>(subH5Group));
        Core::FITKParameter * param = scatterModel->getRadiationScatterModelPara();
        if (param == nullptr) return false;
        parameterToolW(std::get<1>(subH5Group), param);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::scatterModelR()
    {
        if (!_radiation || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取创建工具管理器
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsManager) return false;
        Interface::FITKOFRadiationParaManager* radiationParaMgr = physicsManager->getRadiationParaManager();
        if (!radiationParaMgr) return false;
        //读取辐射模型参数数据
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "ScatterModel");
        if (std::get<0>(subH5Group) == false) return false;
        std::string modelTypeName;
        tool->readGroupStr(std::get<1>(subH5Group), "ModelType", modelTypeName);
        Interface::FITKOFRadiationScatterModelInfo info = radiationParaMgr->getRadiationScatterModelInfo(QString::fromStdString(modelTypeName));
        if (info._radiationScatterModelName.isEmpty()) return false;
        Interface::FITKOFAbsRadiationScatterModel* scatterModel = info._createFuns();
        this->readNDataObject(scatterModel, std::get<1>(subH5Group));
        parameterToolR(std::get<1>(subH5Group), scatterModel->getRadiationScatterModelPara());
        _radiation->setScatterModel(scatterModel);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::solarLoadW()
    {
        if (!_radiation || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出辐射模型
        Interface::FITKOFRadiationSolarLoad * solarLoad = _radiation->getSolarLoad();
        if (!solarLoad) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "SolarLoad");
        if (std::get<0>(subH5Group) == false) return false;
        //写出辐射参数
        Core::FITKParameter * solarLoadPara = solarLoad->getRadiationSolarLoadParams();
        if (!solarLoad) return false;
        std::tuple<bool, H5::Group> solarLoadParaGroup = tool->createGp(std::get<1>(subH5Group), "SolarLoadPara");
        if (std::get<0>(solarLoadParaGroup) == false) return false;
        parameterToolW(std::get<1>(solarLoadParaGroup), solarLoadPara);
        //写出太阳辐射方向参数
        Interface::FITKOFAbsRadiationSunDirection * sunDirection = solarLoad->getRadiationSunDirection();
        if (!sunDirection) return false;
        std::tuple<bool, H5::Group> sunDirectionGroup = tool->createGp(std::get<1>(subH5Group), "SunDirection");
        if (std::get<0>(sunDirectionGroup) == false) return false;
        this->writeNDataObject(sunDirection, std::get<1>(sunDirectionGroup));
        tool->writeGroupAttrStr(std::get<1>(sunDirectionGroup), "ModelType", sunDirection->getDataObjectName().toStdString());
        parameterToolW(std::get<1>(sunDirectionGroup), sunDirection->getRadiationSunDirectionPara());
        //写出太阳辐射载荷参数
        Interface::FITKOFAbsRadiationSunLoad * sunLoad = solarLoad->getRadiationSunLoad();
        if (!sunLoad) return false;
        std::tuple<bool, H5::Group> sunLoadGroup = tool->createGp(std::get<1>(subH5Group), "SunLoad");
        if (std::get<0>(sunLoadGroup) == false) return false;
        this->writeNDataObject(sunLoad, std::get<1>(sunLoadGroup));
        tool->writeGroupAttrStr(std::get<1>(sunLoadGroup), "ModelType", sunLoad->getDataObjectName().toStdString());
        parameterToolW(std::get<1>(sunLoadGroup), sunLoad->getRadiationSunLoadPara());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRadiation::solarLoadR()
    {
        if (!_radiation || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //获取创建工具管理器
        Interface::FITKOFPhysicsManager* physicsManager = Interface::FITKOFPhysicsManager::getInstance();
        if (!physicsManager) return false;
        Interface::FITKOFRadiationParaManager* radiationParaMgr = physicsManager->getRadiationParaManager();
        if (!radiationParaMgr) return false;
        Interface::FITKOFRadiationSolarLoad * solarLoad = _radiation->getSolarLoad();
        if (!solarLoad) return false;
        //读取辐射模型参数数据
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "SolarLoad");
        if (std::get<0>(subH5Group) == false) return false;
        //读取辐射参数
        Core::FITKParameter * solarLoadPara = solarLoad->getRadiationSolarLoadParams();
        if (!solarLoad) return false;
        std::tuple<bool, H5::Group> solarLoadParaGroup = tool->openGp(std::get<1>(subH5Group), "SolarLoadPara");
        if (std::get<0>(solarLoadParaGroup) == false) return false;
        parameterToolR(std::get<1>(solarLoadParaGroup), solarLoadPara);
        //读取太阳辐射方向参数
        std::tuple<bool, H5::Group> sunDirectionGroup = tool->openGp(std::get<1>(subH5Group), "SunDirection");
        if (std::get<0>(sunDirectionGroup) == false) return false;
        std::string modelTypeName;
        tool->readGroupStr(std::get<1>(sunDirectionGroup), "ModelType", modelTypeName);
        Interface::FITKOFRadiationSunDirectionInfo infoSunDirection = radiationParaMgr->getRadiationSunDirectionInfo(QString::fromStdString(modelTypeName));
        if (infoSunDirection._radiationSunDirectionName.isEmpty()) return false;
        Interface::FITKOFAbsRadiationSunDirection* sunDirection = infoSunDirection._createFuns();
        this->readNDataObject(sunDirection, std::get<1>(sunDirectionGroup));
        parameterToolR(std::get<1>(sunDirectionGroup), sunDirection->getRadiationSunDirectionPara());
        solarLoad->setRadiationSunDirection(sunDirection);
        //读取太阳辐射载荷参数
        std::tuple<bool, H5::Group> sunLoadGroup = tool->openGp(std::get<1>(subH5Group), "SunLoad");
        if (std::get<0>(sunLoadGroup) == false) return false;
        modelTypeName.clear();
        tool->readGroupStr(std::get<1>(sunLoadGroup), "ModelType", modelTypeName);
        Interface::FITKOFRadiationSunLoadInfo infoSunLoad = radiationParaMgr->getRadiationSunLoadInfo(QString::fromStdString(modelTypeName));
        if (infoSunLoad._radiationSunLoadName.isEmpty()) return false;
        Interface::FITKOFAbsRadiationSunLoad* sunLoad = infoSunLoad._createFuns();
        this->readNDataObject(sunLoad, std::get<1>(sunLoadGroup));
        parameterToolR(std::get<1>(sunLoadGroup), sunLoad->getRadiationSunLoadPara());
        solarLoad->setRadiationSunLoad(sunLoad);
        return true;
    }

}
