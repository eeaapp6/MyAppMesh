/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelCurve.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelNearestPointLine.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSampleCurve.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsConstruction.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoMergeEdges.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

namespace Interface
{
    bool  FITKAdaptorGeoModelLine::readParameters()
    {
        Interface::FITKAbsGeoModelLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelLine>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double dir[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Dir, dir, 1, 3)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setDir(dir[0], dir[1], dir[2]);
        return true;
    }
    bool FITKAdaptorGeoModelLine::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelLine>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        
        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        std::array<double, 3> dir = cmd->dir();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Dir, dir.data(), 1, 3);
        return true;
    }

    bool  FITKAdaptorGeoModelSegment::readParameters()
    {
        Interface::FITKAbsGeoModelSegment* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSegment>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double sPoint[3] = {};
        double ePoint[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint, 1, 3)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setStartPoint(sPoint[0], sPoint[1], sPoint[2]);
        cmd->setEndPoint(ePoint[0], ePoint[1], ePoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelSegment::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSegment* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSegment>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        std::array<double, 3> sPoint = cmd->startPoint();
        std::array<double, 3> ePoint = cmd->endPoint();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint.data(), 1, 3);
        return true;
    }

    bool  FITKAdaptorGeoModelWire::readParameters()
    {
        Interface::FITKAbsGeoModelWire* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelWire>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double p[3] = {};
        int count = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count,&count)) return false;
        // 设置读取的参数到cmd对象
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), p, 1, 3)) return false;
            cmd->addPoint(p[0], p[1], p[2]);
        }
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);

        return true;
    }
    bool FITKAdaptorGeoModelWire::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelWire* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelWire>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        int count = cmd->count();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            auto stdDoubleArray = cmd->point(i);
            double p[3]{ stdDoubleArray[0], stdDoubleArray[1], stdDoubleArray[2] };
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), p, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelCircle::readParameters()
    {
        Interface::FITKAbsGeoModelCircle* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircle>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double radius = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Radius, &radius)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setRadius(radius);
        return true;
    }
    bool FITKAdaptorGeoModelCircle::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCircle* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircle>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        double radius = cmd->radius();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Radius, radius);
        return true;
    }

    bool  FITKAdaptorGeoModelCircleArc::readParameters()
    {
        Interface::FITKAbsGeoModelCircleArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircleArc>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double radius = 0;
        double sPoint[3] = {};
        double ePoint[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Radius, &radius)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setRadius(radius);
        cmd->setStartPoint(sPoint[0], sPoint[1], sPoint[2]);
        cmd->setEndPoint(ePoint[0], ePoint[1], ePoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelCircleArc::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCircleArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircleArc>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        double radius = cmd->radius();
        std::array<double, 3> sPoint = cmd->startPoint();
        std::array<double, 3> ePoint = cmd->endPoint();
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Radius, radius);
        return true;
    }

    bool  FITKAdaptorGeoModelEllipse::readParameters()
    {
        Interface::FITKAbsGeoModelEllipse* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelEllipse>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double majorRadius =0;
        double minorRadius = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MajorRadius, &majorRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MinorRadius, &minorRadius)) return false;
        // 设置读取的参数到cmd对象

        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setMajorRadius(majorRadius);
        cmd->setMinorRadius(minorRadius);
        return true;
    }
    bool FITKAdaptorGeoModelEllipse::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelEllipse* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelEllipse>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        double majorRadius = cmd->majorRadius();
        double minorRadius = cmd->minorRadius();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MajorRadius, majorRadius);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MinorRadius, minorRadius);

        return true;
    }

    bool  FITKAdaptorGeoModelEllipseArc::readParameters()
    {
        Interface::FITKAbsGeoModelEllipseArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelEllipseArc>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double majorRadius = 0;
        double minorRadius = 0;
        double sPoint[3] = {};
        double ePoint[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MajorRadius, &majorRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MinorRadius, &minorRadius)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint, 1, 3)) return false;
        // 设置读取的参数到cmd对象

        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setMajorRadius(majorRadius);
        cmd->setMinorRadius(minorRadius);
        cmd->setStartPoint(sPoint[0], sPoint[1], sPoint[2]);
        cmd->setEndPoint(ePoint[0], ePoint[1], ePoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelEllipseArc::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelEllipseArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelEllipseArc>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        std::array<double, 3> sPoint = cmd->startPoint();
        std::array<double, 3> ePoint = cmd->endPoint();
        double majorRadius = cmd->majorRadius();
        double minorRadius = cmd->minorRadius();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MajorRadius, majorRadius);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MinorRadius, minorRadius);

        return true;
    }

    bool  FITKAdaptorGeoModelHyperbola::readParameters()
    {
        Interface::FITKAbsGeoModelHyperbola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelHyperbola>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double majorRadius = 0;
        double minorRadius = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MajorRadius, &majorRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MinorRadius, &minorRadius)) return false;
        // 设置读取的参数到cmd对象

        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setMajorRadius(majorRadius);
        cmd->setMinorRadius(minorRadius);
        return true;
    }
    bool FITKAdaptorGeoModelHyperbola::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelHyperbola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelHyperbola>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        double majorRadius = cmd->majorRadius();
        double minorRadius = cmd->minorRadius();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MajorRadius, majorRadius);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MinorRadius, minorRadius);

        return true;
    }

    bool  FITKAdaptorGeoModelHyperbolaArc::readParameters()
    {
        Interface::FITKAbsGeoModelHyperbolaArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelHyperbolaArc>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double majorRadius = 0;
        double minorRadius = 0;
        double sPoint[3] = {};
        double ePoint[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MajorRadius, &majorRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MinorRadius, &minorRadius)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint, 1, 3)) return false;
        // 设置读取的参数到cmd对象

        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setMajorRadius(majorRadius);
        cmd->setMinorRadius(minorRadius);
        cmd->setStartPoint(sPoint[0], sPoint[1], sPoint[2]);
        cmd->setEndPoint(ePoint[0], ePoint[1], ePoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelHyperbolaArc::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelHyperbolaArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelHyperbolaArc>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        std::array<double, 3> sPoint = cmd->startPoint();
        std::array<double, 3> ePoint = cmd->endPoint();
        double majorRadius = cmd->majorRadius();
        double minorRadius = cmd->minorRadius();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MajorRadius, majorRadius);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MinorRadius, minorRadius);

        return true;
    }

    bool  FITKAdaptorGeoModelParabola::readParameters()
    {
        Interface::FITKAbsGeoModelParabola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelParabola>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double focalLength = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FocalLength,&focalLength)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setFocalLength(focalLength);
        return true;
    }
    bool FITKAdaptorGeoModelParabola::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelParabola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelParabola>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        double focalLength = cmd->focalLength();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FocalLength, focalLength);
        return true;
    }

    bool  FITKAdaptorGeoModelParabolaArc::readParameters()
    {
        Interface::FITKAbsGeoModelParabolaArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelParabolaArc>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double focalLength = 0;
        double sPoint[3] = {};
        double ePoint[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FocalLength, &focalLength)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setFocalLength(focalLength);
        cmd->setStartPoint(sPoint[0], sPoint[1], sPoint[2]);
        cmd->setEndPoint(ePoint[0], ePoint[1], ePoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelParabolaArc::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelParabolaArc* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelParabolaArc>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        std::array<double, 3> sPoint = cmd->startPoint();
        std::array<double, 3> ePoint = cmd->endPoint();
        double focalLength = cmd->focalLength();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_StartPoint, sPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_EndPoint, ePoint.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FocalLength, focalLength);
        return true;
    }

    bool  FITKAdaptorGeoModelOffsetCurve::readParameters()
    {
        Interface::FITKAbsGeoModelOffsetCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOffsetCurve>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double dir[3] = {};
        double offset = {};
        int cmdID = 0;
        int  vTopoID = 0;
        int  vTopoIndex = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Dir, dir, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Offset, &offset)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_cmdID, &cmdID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_vTopoID, &vTopoID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_vTopoIndex, &vTopoIndex)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setDir(dir[0], dir[1], dir[2]);
        cmd->setOffset(offset);
        Interface::VirtualShape vShape1 = {};
        vShape1.CmdId = cmdID;
        vShape1.VirtualTopoId = vTopoID;
        vShape1.VirtualTopoIndex = vTopoIndex;
        cmd->setSourceCurve(vShape1);
        return true;
    }
    bool FITKAdaptorGeoModelOffsetCurve::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOffsetCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOffsetCurve>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        std::array<double, 3> dir = cmd->dir();
        double offset = cmd->offset();
        VirtualShape vshape = cmd->sourceCurve();
        int cmdID = vshape.CmdId;
        int  vTopoID = vshape.VirtualTopoId;
        int  vTopoIndex = vshape.VirtualTopoIndex;
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Dir, dir.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_cmdID, cmdID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_vTopoID, vTopoID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_vTopoIndex, vTopoIndex);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Offset, offset);
        return true;
    }

    bool  FITKAdaptorGeoModelBeizeByControlPoints::readParameters()
    {
        Interface::FITKAbsGeoModelBeizeByControlPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBeizeByControlPoints>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double p[3] = {};
        int count = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        // 设置读取的参数到cmd对象
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), p, 1, 3)) return false;
            cmd->addControlPoint(p[0], p[1], p[2]);
        }
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);

        return true;
    }
    bool FITKAdaptorGeoModelBeizeByControlPoints::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelBeizeByControlPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBeizeByControlPoints>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        QList<std::array<double, 3>> points = cmd->controlPoints();
        int count = points.size();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            std::array<double, 3> point = points[i];
            double pointList[3]{ point[0],point[1],point[2] };
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), pointList, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelBsplineByThroughPoints::readParameters()
    {
        Interface::FITKAbsGeoModelBsplineByThroughPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBsplineByThroughPoints>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double origin[3] = {};
        double yDirection[3] = {};
        double zDirection[3] = {};
        double p[3] = {};
        int count = 0;
        int ThroughPointShapesCount = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_ThroughPointShapesCount, &ThroughPointShapesCount)) return false;
        // 设置读取的参数到cmd对象
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), p, 1, 3)) return false;
            cmd->addThroughPoint(p[0], p[1], p[2]);
        }
        QList<Interface::VirtualShape> throughPointShapes = {};
        for (int i = 0; i < ThroughPointShapesCount; i++)
        {
            std::string throughShapes = std::string(FGKW_ThroughPointShapes) + std::to_string(i);
            double ThroughPointShapeArray[3] = {};
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), throughShapes.c_str(), ThroughPointShapeArray, 1, 3)) return false;
            throughPointShapes.append(Interface::VirtualShape(ThroughPointShapeArray[0], ThroughPointShapeArray[1], ThroughPointShapeArray[2]));
        }
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setThroughPointShapes(throughPointShapes);
        return true;
    }
    bool FITKAdaptorGeoModelBsplineByThroughPoints::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelBsplineByThroughPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBsplineByThroughPoints>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        std::array<double, 3> origin = cmd->origin();
        std::array<double, 3> xDirection = cmd->xDirection();
        std::array<double, 3> yDirection = cmd->yDirection();
        std::array<double, 3> zDirection = cmd->zDirection();
        QList<std::array<double, 3>> points = cmd->throughPoints();
        int count = points.size();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            std::array<double, 3> point = points[i];
            double pointList[3]{ point[0],point[1],point[2] };
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), pointList, 1, 3);
        }
        QList<Interface::VirtualShape> throughPointShapes = cmd->throughPointShapes();
        int ThroughPointShapesCount = throughPointShapes.size();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_ThroughPointShapesCount, ThroughPointShapesCount);
        for (int i = 0; i < ThroughPointShapesCount; i++)
        {
            std::string throughShapes = std::string(FGKW_ThroughPointShapes) + std::to_string(i);
            VirtualShape vShape = throughPointShapes[i];
            double ThroughPointShapeArray[3]{ vShape.CmdId,vShape.VirtualTopoId,vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), throughShapes.c_str(), ThroughPointShapeArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelNearestPointLine::readParameters()
    {
        Interface::FITKAbsGeoModelNearestPointLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelNearestPointLine>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double point[3] = {};
        double nearestPoint[3] = {};
        int shapeArray[3] = {};
        std::string sShapeType = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, point, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_NearestPoint, nearestPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Shape, shapeArray, 1, 3)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_ShapeType, sShapeType)) return false;
        // 设置读取的参数到cmd对象
        Interface::VirtualShape shape(shapeArray[0], shapeArray[1], shapeArray[2]);
        bool ok = false;
        Interface::FITKAbsGeoModelNearestPointLine::ShapeType shapeType =
            Core::FITKEnumTransfer<Interface::FITKAbsGeoModelNearestPointLine::ShapeType>().
            fromString(QString::fromStdString(sShapeType), ok);
        if (!ok) return false;
        cmd->setPoint(point);
        cmd->setNearestPoint(nearestPoint);
        cmd->setShape(shape);
        cmd->setShapeType(shapeType);
        return true;
    }
    bool FITKAdaptorGeoModelNearestPointLine::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelNearestPointLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelNearestPointLine>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double point[3] = {};
        cmd->getPoint(point);
        double nearestPoint[3] = {};
        cmd->getNearestPoint(nearestPoint);
        Interface::VirtualShape shape = cmd->getShape();
        int shapeArray[3] = { shape.CmdId, shape.VirtualTopoId, shape.VirtualTopoIndex };
        bool ok = false;
        Interface::FITKAbsGeoModelNearestPointLine::ShapeType shapeType = cmd->getShapeType();
        QString sShapeType =
            Core::FITKEnumTransfer<Interface::FITKAbsGeoModelNearestPointLine::ShapeType>().
            toString(shapeType, ok);
        if (!ok) return false;
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, point, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_NearestPoint, nearestPoint, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Shape, shapeArray, 1, 3);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_ShapeType, sShapeType.toStdString());
        return true;
    }

    bool  FITKAdaptorGeoModelSampleCurve::readParameters()
    {
        Interface::FITKAbsGeoSampleCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSampleCurve>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        int curveArray[3] = { 0,0,0 };
        std::string smethod = {};
        int count = 0;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_SampleMethod, smethod)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SampleCurve, curveArray, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SampleCount, &count)) return false;
        bool ok = false;
        Interface::FITKAbsGeoSampleCurve::SampleMethod me =
            Core::FITKEnumTransfer<Interface::FITKAbsGeoSampleCurve::SampleMethod>().
            fromString(QString::fromStdString(smethod), ok);
        if (!ok) return false;
        Interface::VirtualShape curve = {};
        curve.CmdId = curveArray[0];
        curve.VirtualTopoId = curveArray[1];
        curve.VirtualTopoIndex = curveArray[2];
        // 设置读取的参数到cmd对象
        cmd->setSampleMethod(me);
        cmd->setSampleCurve(curve);
        cmd->setSamplePointCount(count);
        return true;
    }
    bool FITKAdaptorGeoModelSampleCurve::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoSampleCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoSampleCurve>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        Interface::FITKAbsGeoSampleCurve::SampleMethod me = cmd->getSampleMethod();
        // 使用 FITKEnumTransformer 将 me 变量转为字符串
        bool ok = false;
        QString meStr = Core::FITKEnumTransfer<Interface::FITKAbsGeoSampleCurve::SampleMethod>().
            toString(me, ok);
        if (!ok) return false;
        Interface::VirtualShape curve = cmd->getSampleCurve();
        int curveArray[3] = { curve.CmdId,curve.VirtualTopoId,curve.VirtualTopoIndex };
        int count = cmd->getSamplePointCount();
        // 写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SampleCurve, curveArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SampleCount, count);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_SampleMethod, meStr.toStdString());
        return true;
    }

    bool  FITKAdaptorGeoModelConstruction::readParameters()
    {
        Interface::FITKAbsConstruction* cmd = this->getDataObjectAs<Interface::FITKAbsConstruction>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        double point1[3] = {};
        double point2[3] = {};
        std::string type = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "point1", point1, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "point2", point2, 1, 3)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        // 设置读取的参数到cmd对象
        cmd->setPosition1(point1);
        cmd->setPosition2(point2);
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKConstructionType>();
        Interface::FITKGeoEnum::FITKConstructionType cmdType = (Interface::FITKGeoEnum::FITKConstructionType)metaEnum.keyToValue(type.c_str());
        cmd->setConstructionType(cmdType);
        return true;
    }
    bool FITKAdaptorGeoModelConstruction::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsConstruction* cmd = this->getDataObjectAs<Interface::FITKAbsConstruction>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        double point1[3] = {};
        double point2[3] = {};
        cmd->getPositions(point1, point2);

        Interface::FITKGeoEnum::FITKConstructionType t = cmd->getConstructionType();
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKConstructionType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "point1", point1, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "point2", point2, 1, 3);
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        return true;
    }

    bool  FITKAdaptorGeoModelMergeEdges::readParameters()
    {
        Interface::FITKAbsGeoMergeEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoMergeEdges>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int toposCount = 0;
        QList<Interface::VirtualShape> vShapeList = {};

        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &toposCount))return false;
        for (int i = 0; i < toposCount; i++)
        {
            int vShapeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3)) return false;
            vShapeList.append(Interface::VirtualShape(vShapeArray[0], vShapeArray[1], vShapeArray[2]));
        }
        cmd->setEdges(vShapeList);
        return true;
    }
    bool FITKAdaptorGeoModelMergeEdges::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoMergeEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoMergeEdges>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<VirtualShape> topos = cmd->edges();
        int toposCount = topos.size();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, toposCount);
        for (int i = 0; i < toposCount; i++)
        {
            VirtualShape vShape = topos[i];
            int vShapeArray[3]{ vShape.CmdId, vShape.VirtualTopoId, vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3);
        }
        return true;
    }

}



