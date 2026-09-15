/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorGeometryComponent.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorGeometryComponent::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorGeometryComponent";
    }

    bool FITKFlowOFHDF5AdaptorGeometryComponent::adaptR()
    {
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (!_reader || !geoCommand) return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        Interface::FITKAbsGeoShapeAgent* geoModel = geoCommand->getShapeAgent();
        if (!geoModel)return false;
        //获取组件管理器
        _geoComponentMgr = geoModel->getGeoComponentManager();
        int count = 0;
        bool isR = tool->readGroupint(*_h5Group, "componentCount", &count);
        if (!isR) return false;
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!_h5Group->attrExists(name))continue;
            if (!_h5Group->nameExists(name))continue;
            int type = 0;
            bool ok = tool->readGroupint(*_h5Group, name.c_str(), &type);
            if (!ok) continue;
            H5::Group h5Group = _h5Group->openGroup(name);
            Interface::FITKGeoComponent* component = new Interface::FITKGeoComponent(Interface::FITKModelEnum::FITKModelSetType(type));
            isR &= this->geoComponentR(component, h5Group);
            _geoComponentMgr->appendDataObj(component);
        }

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorGeometryComponent::adaptW()
    {
        Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(_dataObj);
        if (!_writer || !geoCommand) return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        Interface::FITKAbsGeoShapeAgent* geoModel = geoCommand->getShapeAgent();
        if (!geoModel)return false;
        //获取组件管理器
        _geoComponentMgr = geoModel->getGeoComponentManager();
        int count = _geoComponentMgr->getDataCount();
        bool isW = tool->writeGroupAttrInt(*_h5Group, "componentCount", count);
        if (!isW) return isW;
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKGeoComponent* component = _geoComponentMgr->getDataByIndex(i);
            if (!component)continue;
            int type = component->getGeoType();
            std::string name = std::to_string(i);
            bool ok = tool->writeGroupAttrInt(*_h5Group, name.c_str(), type);
            if (!ok) continue;
            H5::Group h5Group = _h5Group->createGroup(name);
            isW &= this->geoComponentW(component, h5Group);
        }
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorGeometryComponent::geoComponentW(Interface::FITKGeoComponent* component, H5::Group& h5Group)
    {
        if (!_writer || !component) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出数据
        if (!writeNDataObject(component, h5Group))return false;
        int absModelId = component->getAbsModelID();
        if (!tool->writeGroupAttrInt(h5Group, "absModelID", absModelId))
            return false;
        std::vector<int> allMember = component->getMember().toVector().toStdVector();
        if (!tool->CreateOneDarrayDataSetInt(h5Group, "member", allMember))
            return false;
        return true;
    }
    bool FITKFlowOFHDF5AdaptorGeometryComponent::geoComponentR(Interface::FITKGeoComponent* component, H5::Group& h5Group)
    {
        if (!_reader || !component)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取数据
        if (!readNDataObject(component, h5Group))return false;
        int absModelId = 0;
        if (!tool->readGroupint(h5Group, "absModelID", &absModelId))
            return false;
        component->setModel(absModelId);
        std::vector<int> allMember;
        if (!tool->ReadOneDArrayDataSetInt(h5Group, "member", allMember))
            return false;
        QList<int> member = QVector<int>::fromStdVector(allMember).toList();
        component->setMember(member);
        return true;
    }
}
