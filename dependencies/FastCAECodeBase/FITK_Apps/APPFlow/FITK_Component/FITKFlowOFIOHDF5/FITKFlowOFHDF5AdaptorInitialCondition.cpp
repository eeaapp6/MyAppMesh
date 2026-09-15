/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorInitialCondition.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFInitialConditions.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include "FITK_Interface/FITKInterfaceFlowOF/FITKFlowPhysicsHandlerFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"



#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorInitialCondition::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorInitialCondition";
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::adaptR()
    {
        _initialCond = dynamic_cast<Interface::FITKOFInitialConditions*>(_dataObj);
        if (!_reader || !_initialCond) return false;
        if (!readNDataObject(_initialCond, *_h5Group)) return false;
        bool isR = true;
        //读取数据
        isR &= basicDataR();
        isR &= patchDataR();
        isR &= regionPropDataR();
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::adaptW()
    {
        _initialCond = dynamic_cast<Interface::FITKOFInitialConditions*>(_dataObj);
        if (!_writer || !_initialCond) return false;
        if (!writeNDataObject(_initialCond, *_h5Group)) return false;
        bool isW = true;
        //写出数据
        isW &= basicDataW();
        isW &= patchDataW();
        isW &= regionPropDataW();
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::basicDataW()
    {
        if (!_writer || !_initialCond) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Basic");
        if (_initialCond->getBasicData())
            parameterToolW(h5Group, _initialCond->getBasicData());
        return true;
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::basicDataR()
    {
        if (!_reader || !_initialCond)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Basic"))return false;
        H5::Group h5Group = _h5Group->openGroup("Basic");
        if (h5Group.getNumAttrs() > 0)
        {
            Core::FITKParameter* limits = _initialCond->getBasicData();
            if (!limits) return false;
            limits->clear();
            parameterToolR(h5Group, limits);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::patchDataW()
    {
        if (!_writer || !_initialCond)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        H5::Group h5Group = _h5Group->createGroup("Patchs");
        int count = _initialCond->getPatchCount();
        tool->writeGroupAttrInt(h5Group, "PatchCount", count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFGeometryPatch* patchData = _initialCond->getPatch(i);
            if (!patchData) continue;
            std::string name = std::to_string(i);
            H5::Group patchH5Group = h5Group.createGroup(name);
            //几何id
            int geometryId = patchData->getGeometryId();
            tool->writeGroupAttrInt(patchH5Group, "geometryId", geometryId);
            //类型
            int applyType = patchData->getAppltTo();
            tool->writeGroupAttrInt(patchH5Group, "ApplyToType", applyType);
            //场数据
            Core::FITKParameter* fieldPara = patchData->getFieldPara();
            if (fieldPara) parameterToolW(patchH5Group, fieldPara);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::patchDataR()
    {
        if (!_reader || !_initialCond)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!_h5Group->nameExists("Patchs"))return false;
        H5::Group h5Group = _h5Group->openGroup("Patchs");
        //清空数据
        int count = _initialCond->getPatchCount();
        for (int i = 0; i < count; ++i)
            _initialCond->removePatch(i);
        //读取数据并设置
        tool->readGroupint(h5Group, "PatchCount", &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            if (!h5Group.nameExists(name))continue;
            H5::Group patchH5Group = h5Group.openGroup(name);
            //几何id
            int geometryId = 0;
            tool->readGroupint(patchH5Group, "geometryId", &geometryId);
            //类型
            int applyType = 0;
            tool->readGroupint(patchH5Group, "ApplyToType", &applyType);
            //场数据
            Interface::FITKOFGeometryPatch* patchData = new Interface::FITKOFGeometryPatch(geometryId, Interface::FITKOFSolverInitialEnum::FITKPatchApplyObjectType(applyType));
            parameterToolR(patchH5Group, patchData->getFieldPara());
            _initialCond->appendPatch(patchData);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::regionPropDataW()
    {
        if (!_writer || !_initialCond)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->createGp(*_h5Group, "RegionProp");
        if (std::get<0>(subH5Group) == false) return false;
        int count = _initialCond->getInitPropRegionCount();
        tool->writeGroupAttrBool(std::get<1>(subH5Group), "IsEnableRegionProp", _initialCond->getInitPropRegions());
        tool->writeGroupAttrInt(std::get<1>(subH5Group), "RegionPropCount", count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFInitPropRegions* regionsProp = _initialCond->getInitPropRegionByIndex(i);
            if (!regionsProp) continue;
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> regionsPropH5Group = tool->createGp(std::get<1>(subH5Group), QString::fromStdString(name));
            if (std::get<0>(regionsPropH5Group) == false) continue;
            //区域id
            int regionMeshId = regionsProp->getRegionMeshID();
            tool->writeGroupAttrInt(std::get<1>(regionsPropH5Group), "regionMeshId", regionMeshId);
            //类型
            int regionMeshType = regionsProp->getRegionMeshType();
            tool->writeGroupAttrInt(std::get<1>(regionsPropH5Group), "regionMeshType", regionMeshType);
            //场数据
            Core::FITKParameter* regionPara = regionsProp->getRegionPara();
            if (regionPara) parameterToolW(std::get<1>(regionsPropH5Group), regionPara);
        }
        return true;
    }

    bool FITKFlowOFHDF5AdaptorInitialCondition::regionPropDataR()
    {
        if (!_reader || !_initialCond)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::tuple<bool, H5::Group> subH5Group = tool->openGp(*_h5Group, "RegionProp");
        if (std::get<0>(subH5Group) == false) return false;
        bool isEnableRegionProp = false;
        tool->readGroupBool(std::get<1>(subH5Group), "IsEnableRegionProp", &isEnableRegionProp);
        _initialCond->setInitPropRegions(isEnableRegionProp);
        //清空数据
        _initialCond->clearAllInitPropRegion();
        //读取数据并设置
        int count = 0;
        tool->readGroupint(std::get<1>(subH5Group), "RegionPropCount", &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> regionsPropH5Group = tool->openGp(std::get<1>(subH5Group), name.c_str());
            if (std::get<0>(regionsPropH5Group) == false) continue;
            Interface::FITKOFInitPropRegions* regionsProp = new Interface::FITKOFInitPropRegions;
            _initialCond->addInitPropRegion(regionsProp);
            //区域id和区域类型
            int regionMeshId = 0, regionMeshType = 0;
            tool->readGroupint(std::get<1>(regionsPropH5Group), "regionMeshId", &regionMeshId);
            tool->readGroupint(std::get<1>(regionsPropH5Group), "regionMeshType", &regionMeshType);
            regionsProp->setRegionMeshID(regionMeshId);
            regionsProp->setRegionMeshType(Interface::FITKOFSolverTypeEnum::FITKOFRegionMeshType(regionMeshType));
            //场数据
            Core::FITKParameter* regionPara = regionsProp->getRegionPara();
            if (regionPara) parameterToolR(std::get<1>(regionsPropH5Group), regionPara);
        }
        return true;
    }

}
