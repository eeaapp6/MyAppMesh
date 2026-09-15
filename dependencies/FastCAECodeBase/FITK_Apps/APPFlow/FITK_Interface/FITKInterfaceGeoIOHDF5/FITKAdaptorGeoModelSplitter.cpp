/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelSplitter.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSplitter.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelCurveSplitter::readParameters()
    {
        Interface::FITKAbsGeoCurveSplitter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCurveSplitter>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        
        int sShape[3] = {};
        int tShape[3] = {};
        double toolPoint[3] = {};
        double pArray[3] = {};
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolShape, tShape, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ToolPoint, toolPoint, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, pArray, 1, 3))return false;
        //设置参数
        cmd->setSourceShape(Interface::VirtualShape(sShape[0], sShape[1], sShape[2]));
        cmd->setToolShape(Interface::VirtualShape(tShape[0], tShape[1], tShape[2]));
        std::array<double, 3> toolPointArray{ toolPoint[0],toolPoint[1],toolPoint[2] };
        cmd->setToolPoint(toolPointArray);
        cmd->setPoint(pArray[0], pArray[1], pArray[2]);
        return true;
    }
    bool FITKAdaptorGeoModelCurveSplitter::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoCurveSplitter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCurveSplitter>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        //获取参数
        int sShape[3]{ cmd->sourceShape().CmdId,cmd->sourceShape().VirtualTopoId,cmd->sourceShape().VirtualTopoIndex };
        int tShape[3]{ cmd->toolShape().CmdId,cmd->toolShape().VirtualTopoId,cmd->toolShape().VirtualTopoIndex };
        double toolPoint[3]{ cmd->toolPoint()[0],cmd->toolPoint()[1] ,cmd->toolPoint()[2] };
        std::array<double, 3> point = cmd->point();
        double pArray[3]{ point[0], point[1], point[2] };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolShape, tShape, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ToolPoint, toolPoint, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, pArray, 1, 3);
        return true;
    }

    bool  FITKAdaptorGeoModelSurfaceSplitter::readParameters()
    {
        Interface::FITKAbsGeoSurfaceSplitter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSurfaceSplitter>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int sShape[3] = {};
        int tShape[3] = {};
        double toolPoint[3] = {};
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolShape, tShape, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ToolPoint, toolPoint, 1, 3))return false;
        //设置参数
        cmd->setSourceShape(Interface::VirtualShape(sShape[0], sShape[1], sShape[2]));
        cmd->setToolShape(Interface::VirtualShape(tShape[0], tShape[1], tShape[2]));
        std::array<double, 3> toolPointArray{ toolPoint[0],toolPoint[1],toolPoint[2] };
        cmd->setToolPoint(toolPointArray);
        return true;
    }
    bool FITKAdaptorGeoModelSurfaceSplitter::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoSurfaceSplitter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSurfaceSplitter>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        int sShape[3]{ cmd->sourceShape().CmdId,cmd->sourceShape().VirtualTopoId,cmd->sourceShape().VirtualTopoIndex };
        int tShape[3]{ cmd->toolShape().CmdId,cmd->toolShape().VirtualTopoId,cmd->toolShape().VirtualTopoIndex };
        double toolPoint[3]{ cmd->toolPoint()[0],cmd->toolPoint()[1] ,cmd->toolPoint()[2] };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolShape, tShape, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ToolPoint, toolPoint, 1, 3);
        return true;
    }

    bool  FITKAdaptorGeoModelSolidSplitter::readParameters()
    {
        Interface::FITKAbsGeoSolidSplitter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSolidSplitter>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int sShape[3] = {};
        int tShape[3] = {};
        double toolPoint[3] = {};
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolShape, tShape, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ToolPoint, toolPoint, 1, 3))return false;
        //设置参数
        cmd->setSourceShape(Interface::VirtualShape(sShape[0], sShape[1], sShape[2]));
        cmd->setToolShape(Interface::VirtualShape(tShape[0], tShape[1], tShape[2]));
        std::array<double, 3> toolPointArray{ toolPoint[0],toolPoint[1],toolPoint[2] };
        cmd->setToolPoint(toolPointArray);
        return true;
    }
    bool FITKAdaptorGeoModelSolidSplitter::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoSolidSplitter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSolidSplitter>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        int sShape[3]{ cmd->sourceShape().CmdId,cmd->sourceShape().VirtualTopoId,cmd->sourceShape().VirtualTopoIndex };
        int tShape[3]{ cmd->toolShape().CmdId,cmd->toolShape().VirtualTopoId,cmd->toolShape().VirtualTopoIndex };
        double toolPoint[3]{ cmd->toolPoint()[0],cmd->toolPoint()[1] ,cmd->toolPoint()[2] };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sShape, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ToolShape, tShape, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ToolPoint, toolPoint, 1, 3);
        return true;
    }
}



