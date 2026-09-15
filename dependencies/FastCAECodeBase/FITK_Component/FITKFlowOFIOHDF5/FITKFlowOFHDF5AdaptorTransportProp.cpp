/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorTransportProp.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportModel.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorTransportProp::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorTransportProp";
    }

    bool FITKFlowOFHDF5AdaptorTransportProp::adaptR()
    {
        _transportProp = dynamic_cast<Interface::FITKOFTransportProp*>(_dataObj);
        if (!_reader || !_transportProp) return false;
        if (!readNDataObject(_transportProp, *_h5Group)) return false;
        bool isR = true;
        isR &= this->transportAdditionalDataR();

        isR &= this->phasesR();

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorTransportProp::adaptW()
    {
        _transportProp = dynamic_cast<Interface::FITKOFTransportProp*>(_dataObj);
        if (!_writer || !_transportProp) return false;
        if (!writeNDataObject(_transportProp, *_h5Group)) return false;
        bool isW = true;
        isW &= this->transportAdditionalDataW();

        isW &= this->phasesW();
        return true;
    }

    bool FITKFlowOFHDF5AdaptorTransportProp::transportAdditionalDataW()
    {
        if (!_writer || !_transportProp)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> h5Group = tool->createGp(*_h5Group, "TransportAdditionalData");
        if (std::get<0>(h5Group) == false) return false;
        //如果为空就证明没有数据
        Core::FITKParameter* additionalTran = _transportProp->getTransportAdditionalData();
        if (additionalTran)
            parameterToolW(std::get<1>(h5Group), additionalTran);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorTransportProp::transportAdditionalDataR()
    {
        if (!_reader || !_transportProp)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("TransportAdditionalData"))return false;
        H5::Group h5Group = _h5Group->openGroup("TransportAdditionalData");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* additionalTran = new Core::FITKParameter;
            parameterToolR(h5Group, additionalTran);
            _transportProp->setTransportAdditionalData(additionalTran);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorTransportProp::phasesW()
    {
        if (!_writer || !_transportProp)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Phases");
        int phaseCount = _transportProp->getPhasesCount();
        tool->writeGroupAttrInt(h5Group, "PhaseCount", phaseCount);
        for (int i = 0; i < phaseCount; ++i)
        {
            Interface::FITKOFTransportPhase* phase = _transportProp->getPhase(i);
            if (!phase)continue;
            std::string name = std::to_string(i);
            H5::Group phaseH5Group = h5Group.createGroup(name);
            //名称
            QString phaseName = phase->getPhaseName();
            tool->writeGroupAttrStr(phaseH5Group, "PhaseName", phaseName.toStdString());
            //其他数据
            H5::Group addH5Group = phaseH5Group.createGroup("AdditionalData");
            Core::FITKParameter* additionData = phase->getPhaseAdditionalData();
            if (additionData) parameterToolW(addH5Group, additionData);
            //模式数据
            Interface::FITKAbsOFTransportModel* model = phase->getTransportModel();
            if (model)
            {
                tool->writeGroupAttrStr(phaseH5Group, "ModelName", model->getDataObjectName().toStdString());
                H5::Group modelH5Group = phaseH5Group.createGroup("ModelData");
                writeNDataObject(model, modelH5Group);
                parameterToolW(modelH5Group, model->getTransportModelPara());
            }
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorTransportProp::phasesR()
    {
        if (!_reader || !_transportProp)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Phases"))return false;
        Interface::FITKFlowPhysicsHandlerFactory* factoryData = FITKAPP->getComponents()->getComponentTByName<Interface::FITKFlowPhysicsHandlerFactory>("FITKFlowPhysicsHandlerFactory");
        if (!factoryData) return false;
        H5::Group h5Group = _h5Group->openGroup("Phases");
        int phaseCount = 0;
        tool->readGroupint(h5Group, "PhaseCount", &phaseCount);
        if (_transportProp->getPhasesCount() != phaseCount) return false;
        for (int i = 0; i < phaseCount; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.nameExists(name))continue;
            Interface::FITKOFTransportPhase* phase = _transportProp->getPhase(i);
            if (!phase)return false;
            H5::Group phaseH5Group = h5Group.openGroup(name);
            //名称
            std::string v;
            tool->readGroupStr(phaseH5Group, "PhaseName", v);
            QString phaseName = QString::fromStdString(v);
            phase->setPhaseName(phaseName);
            //其他数据
            if (!phaseH5Group.nameExists("AdditionalData"))return false;
            H5::Group addH5Group = phaseH5Group.openGroup("AdditionalData");
            if (addH5Group.getNumAttrs() > 0)
            {
                Core::FITKParameter* additionData = new Core::FITKParameter;
                parameterToolR(addH5Group, additionData);
                phase->setPhaseAdditionalData(additionData);
            }
            //模式数据
            if (phaseH5Group.attrExists("ModelName"))
            {
                tool->readGroupStr(phaseH5Group, "ModelName", v);
                QString modelName = QString::fromStdString(v);
                factoryData->setTransportModel(i, modelName);
                H5::Group modelH5Group = phaseH5Group.openGroup("ModelData");
                Interface::FITKAbsOFTransportModel* model = phase->getTransportModel();
                readNDataObject(model, modelH5Group);
                model->getTransportModelPara()->clear();
                parameterToolR(modelH5Group, model->getTransportModelPara());
            }
        }
        return true;
    }

}
