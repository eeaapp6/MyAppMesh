/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCShapeHDF5IOHelper.h"
#include "FITKOCCShapeIO.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKGeoHDF5KeyWords.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"

namespace OCC
{ 
    bool FITKOCCShapeHDF5IOHelper::writeShape(OCC::FITKAbstractOCCModel *model, IO::FITKAbstractHDF5Writer* w,
                                                 H5::Group &g)
    {
        if (model == nullptr || w == nullptr) return false;
        TopoDS_Shape* shape =  model->getShape();
        IO::FITKHDF5FileTool* h5tools = w ->getHDF5FileTool();
        if(shape == nullptr || h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> group = h5tools->createGp(g, FGKW_Shape );
        if(!std::get<0>(group)) return false;
        //写出形状数据
        uint len = -1;
        char* data;
        if (!IO::FITKOCCIOTool::ShapeToData(shape, len, data)) return false;
        bool ok = h5tools->writeGroupAttrInt(std::get<1>(group), FGKW_ShapeDataSize, len);
        ok &= h5tools->CreateOneDarrayDataSetStr (std::get<1>(group), FGKW_Shape, data, len);

        return ok;
    }

    bool FITKOCCShapeHDF5IOHelper::readShape(Interface::FITKAbsGeoCommand* command, IO::FITKAbstractHDF5Reader *reader, H5::Group &g)
    {
        if(command->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTDelete ||
            command->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTMultiDelete) return true;
        IO::FITKHDF5FileTool* h5tools = reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        int len = -1;
        std::tuple<bool, H5::Group> group = h5tools->openGp(g, FGKW_Shape);
        if (!std::get<0>(group)) return false;
        if(!h5tools->readGroupint(std::get<1>(group), FGKW_ShapeDataSize, &len)) return false;
        char* data = new char[len];
        if(!h5tools->ReadOneDArrayDataSetStr(std::get<1>(group), FGKW_Shape, data, len)) return false;
        if (!IO::FITKOCCIOTool::SetDataToCommand(command, len, data)) return false;
        delete[] data;
        return true;
    }

} // namespace OCC