/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelBool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperNBodyUnite.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool  FITKAdaptorGeoModelBool::readParameters()
    {
        Interface::FITKAbsGeoOperBool* cmd = this->getDataObjectAs<Interface::FITKAbsGeoOperBool>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int targetArray[3] = {};
        int toolArray[3] = {};
        std::string type = {};
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Target, targetArray,1,3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolArray, toolArray,1,3)) return false;

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoOperBool::GeoBoolOperType >();
        Interface::FITKAbsGeoOperBool::GeoBoolOperType  cmdType = (Interface::FITKAbsGeoOperBool::GeoBoolOperType)metaEnum.keyToValue(type.c_str());
        cmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GeoBoolOperType(cmdType));
        Interface::VirtualShape vShapeTarget(targetArray[0], targetArray[1], targetArray[2]);
        cmd->setTarget(vShapeTarget);
        Interface::VirtualShape vShapeTool(toolArray[0], toolArray[1], toolArray[2]);
        cmd->setTool(vShapeTool);

        return true;
    }
    bool FITKAdaptorGeoModelBool::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoOperBool* cmd = this->getDataObjectAs<Interface::FITKAbsGeoOperBool>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        
        FITKAbsGeoOperBool::GeoBoolOperType t = cmd->getBoolOperType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoOperBool::GeoBoolOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(t, ok);
        if (!ok) return false;
        Interface::VirtualShape target = cmd->target();
        int targetArray[3]{ target.CmdId ,target.VirtualTopoId,target.VirtualTopoIndex };
        Interface::VirtualShape tool = cmd->tool();
        int toolArray[3]{ tool.CmdId ,tool.VirtualTopoId,tool.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Target, targetArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolArray, toolArray, 1, 3);

        return true;
    }


    bool  FITKAdaptorGeoOperNBodyUnite::readParameters()
    {
        Interface::FITKAbsGeoOperNBodyUnite* cmd = this->getDataObjectAs<Interface::FITKAbsGeoOperNBodyUnite>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int shapeArray[3] = {};
        int count = 0;
        VirtualShape shape = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), shapeArray, 1, 3)) return false;
            VirtualShape shape { shapeArray[0], shapeArray[1], shapeArray[2] };
            cmd->addShape(shape);
        }
        return true;
    }
    bool FITKAdaptorGeoOperNBodyUnite::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoOperNBodyUnite* cmd = this->getDataObjectAs<Interface::FITKAbsGeoOperNBodyUnite>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> shapes = cmd->getShapes();
        int count = shapes.count();
        //写出参数
        for (int i = 0; i < count; i++)
        {
            VirtualShape shape = shapes[i];
            int shapeArray[3]{ shape.CmdId ,shape.VirtualTopoId, shape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), shapeArray, 1, 3);
        }
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        return true;
    }
}



