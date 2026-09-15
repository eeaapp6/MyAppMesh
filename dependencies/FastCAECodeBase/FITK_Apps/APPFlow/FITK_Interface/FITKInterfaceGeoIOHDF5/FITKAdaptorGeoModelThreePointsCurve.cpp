/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelThreePointsCurve.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCurve.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperLine.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

namespace Interface
{
    bool  FITKAdaptorGeoModelThreePointsCircle::readParameters()
    {
        Interface::FITKAbsGeoModelThreePointsCircle* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsCircle>();
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
        int pointCount = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Radius, &radius)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &pointCount)) return false;
        // 设置读取的参数到cmd对象
        for (int i = 0; i < pointCount; i++)
        {
            double pointList[3] = {};
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), pointList, 1, 3)) return false;
            cmd->setPoint(i, pointList[0], pointList[1], pointList[2]);
        }
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setRadius(radius);
        return true;
    }
    bool FITKAdaptorGeoModelThreePointsCircle::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelThreePointsCircle* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsCircle>();
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
        const int pointCount = 3;
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Radius, radius);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, pointCount);
        for (int i = 0; i < pointCount; i++)
        {
            auto point = cmd->point(i);
            double pointList[3]{ point[0],point[1],point[2] };
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), pointList, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelThreePointsEllipse::readParameters()
    {
        Interface::FITKAbsGeoModelThreePointsEllipse* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsEllipse>();
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
        double majorPoint[3] = {};
        double minorPoint[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MajorPoint, majorPoint,1,3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MinorPoint, minorPoint,1,3)) return false;
        // 设置读取的参数到cmd对象

        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setMajorPoint(majorPoint[0], majorPoint[1], majorPoint[2]);
        cmd->setMinorPoint(minorPoint[0], minorPoint[1], minorPoint[2]);
        return true;
    }
    bool FITKAdaptorGeoModelThreePointsEllipse::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelThreePointsEllipse* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsEllipse>();
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
        std::array<double, 3> majorPoint = cmd->majorPoint();
        std::array<double, 3> minorPoint = cmd->minorPoint();

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MajorPoint, majorPoint.data(),1,3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MinorPoint, minorPoint.data(),1,3);

        return true;
    }

    bool  FITKAdaptorGeoModelThreePointsHyperbolae::readParameters()
    {
        Interface::FITKAbsGeoModelThreePointsHyperbola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsHyperbola>();
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
        double majorPoint[3] = {};
        double minorPoint[3] = {};
        double majorRadius = 0;
        double minorRadius = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MajorPoint, majorPoint, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MinorPoint, minorPoint, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MajorRadius, &majorRadius)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_MinorRadius, &minorRadius)) return false;
        // 设置读取的参数到cmd对象

        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setMajorPoint(majorPoint[0], majorPoint[1], majorPoint[2]);
        cmd->setMinorPoint(minorPoint[0], minorPoint[1], minorPoint[2]);
        cmd->setMajorRadius(majorRadius);
        cmd->setMinorRadius(minorRadius);
        return true;
    }
    bool FITKAdaptorGeoModelThreePointsHyperbolae::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelThreePointsHyperbola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsHyperbola>();
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
        std::array<double, 3> majorPoint = cmd->majorPoint();
        std::array<double, 3> minorPoint = cmd->minorPoint();
        double majorRadius = cmd->majorRadius();
        double minorRadius = cmd->minorRadius();
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MajorPoint, majorPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_MinorPoint, minorPoint.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MajorRadius, majorRadius);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_MinorRadius, minorRadius);
        return true;
    }

    bool  FITKAdaptorGeoModelThreePointsParabola::readParameters()
    {
        Interface::FITKAbsGeoModelThreePointsParabola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsParabola>();
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
        double focalLength =0;
        double pointList[3] = {};
        int pointCount = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_FocalLength, &focalLength)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &pointCount)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setFocalLength(focalLength);
        for (int i = 0; i < pointCount; i++)
        {
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), pointList, 1, 3)) return false;
            cmd->setPoint(i, pointList[0], pointList[1], pointList[2]);
        }
        return true;
    }
    bool FITKAdaptorGeoModelThreePointsParabola::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelThreePointsParabola* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelThreePointsParabola>();
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
        const int pointCount = 3;
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_FocalLength, focalLength);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, pointCount);
        for (int i = 0; i < pointCount; i++)
        {
            auto point = cmd->point(i);
            double pointList[3]{ point[0],point[1],point[2] };
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), pointList, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelCurveProjectionOnSurface::readParameters()
    {
        Interface::FITKAbsGeoModelCurveProjectionOnSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveProjectionOnSurface>();
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
        int sourceCurve[3] = {};
        int sourceSurface[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurve, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurface, 1, 3)) return false;

        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        Interface::VirtualShape vShape1(sourceCurve[0], sourceCurve[1], sourceCurve[2]);
        Interface::VirtualShape vShape2(sourceSurface[0], sourceSurface[1], sourceSurface[2]);
        cmd->setSourceCurve(vShape1);
        cmd->setSourceSurface(vShape2);
        return true;
    }
    bool FITKAdaptorGeoModelCurveProjectionOnSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCurveProjectionOnSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveProjectionOnSurface>();
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
        VirtualShape sourceCurveGet = cmd->sourceCurve();
        int sourceCurve[3]{ sourceCurveGet.CmdId, sourceCurveGet.VirtualTopoId, sourceCurveGet.VirtualTopoId };
        VirtualShape sourceSurfaceGet = cmd->sourceSurface();
        int sourceSurface[3]{ sourceSurfaceGet.CmdId, sourceSurfaceGet.VirtualTopoId, sourceSurfaceGet.VirtualTopoIndex };

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurve, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurface, 1, 3);

        return true;
    }

    bool  FITKAdaptorGeoModelTrimmedCurve::readParameters()
    {
        Interface::FITKAbsGeoModelTrimmedCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTrimmedCurve>();
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
        int id = 0;
        double startTrimmed = 0;
        double endTrimmed = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SourceCurveId,&id)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_StartTrimmed,&startTrimmed)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_EndTrimmed,&endTrimmed)) return false;
        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        cmd->setSourceCurveId(id);
        cmd->setStartTrimmed(startTrimmed);
        cmd->setEndTrimmed(endTrimmed);
        return true;
    }
    bool FITKAdaptorGeoModelTrimmedCurve::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelTrimmedCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTrimmedCurve>();
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
        int id = cmd->sourceCurveId();
        double startTrimmed = cmd->startTrimmed();
        double endTrimmed = cmd->endTrimmed();
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SourceCurveId, id);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_StartTrimmed, startTrimmed);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_EndTrimmed, endTrimmed);
        return true;
    }

    bool  FITKAdaptorGeoModelSurfaceIntersectionSurface::readParameters()
    {
        Interface::FITKAbsGeoModelSurfaceIntersectionSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSurfaceIntersectionSurface>();
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
        int sourceSurface1[3] = {};
        int sourceSurface2[3] = {};
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface1, sourceSurface1, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface2, sourceSurface2, 1, 3)) return false;

        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        Interface::VirtualShape vShape1(sourceSurface1[0], sourceSurface1[1], sourceSurface1[2]);
        Interface::VirtualShape vShape2(sourceSurface2[0], sourceSurface2[1], sourceSurface2[2]);
        cmd->setSourceSurface1(vShape1);
        cmd->setSourceSurface2(vShape2);
        return true;
    }
    bool FITKAdaptorGeoModelSurfaceIntersectionSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSurfaceIntersectionSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSurfaceIntersectionSurface>();
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
        VirtualShape sourceSurface1Get = cmd->sourceSurface1();
        int sourceSurface1[3]{ sourceSurface1Get.CmdId, sourceSurface1Get.VirtualTopoId, sourceSurface1Get.VirtualTopoId };
        VirtualShape sourceSurface2Get = cmd->sourceSurface2();
        int sourceSurface2[3]{ sourceSurface2Get.CmdId, sourceSurface2Get.VirtualTopoId, sourceSurface2Get.VirtualTopoIndex };

        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface1, sourceSurface1, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface2, sourceSurface2, 1, 3);

        return true;
    }

    bool  FITKAdaptorGeoModelBridgeCurve::readParameters()
    {
        Interface::FITKAbsGeoModelBridgeCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBridgeCurve>();
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
        int sourceCurve1[3] = {};
        int sourceCurve2[3] = {};
        bool useStartEnd1 = false;
        double parameter1 = 0;
        bool useStartEnd2 =false;
        double parameter2 = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface1, sourceCurve1, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface2, sourceCurve2, 1, 3)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_UseStartEnd1, &useStartEnd1)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_UseStartEnd2, &useStartEnd2)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Parameter1, &parameter1)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Parameter2, &parameter2)) return false;

        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        Interface::VirtualShape vShape1(sourceCurve1[0], sourceCurve1[1], sourceCurve1[2]);
        Interface::VirtualShape vShape2(sourceCurve2[0], sourceCurve2[1], sourceCurve2[2]);
        cmd->setSourceCurve1(vShape1);
        cmd->setSourceCurve2(vShape2);
        cmd->setUseStartEnd1(useStartEnd1);
        cmd->setUseStartEnd2(useStartEnd2);
        cmd->setParameter1(parameter1);
        cmd->setParameter1(parameter2);
        return true;
    }
    bool FITKAdaptorGeoModelBridgeCurve::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelBridgeCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBridgeCurve>();
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
        VirtualShape sourceCurve1Get = cmd->sourceCurve1();
        int sourceCurve1[3]{ sourceCurve1Get.CmdId, sourceCurve1Get.VirtualTopoId, sourceCurve1Get.VirtualTopoId };
        bool useStartEnd1 = cmd->useStartEnd1();
        double parameter1 = cmd->parameter1();
        VirtualShape sourceCurve2Get = cmd->sourceCurve2();
        int sourceCurve2[3]{ sourceCurve2Get.CmdId, sourceCurve2Get.VirtualTopoId, sourceCurve2Get.VirtualTopoIndex };
        bool useStartEnd2 = cmd->useStartEnd2();
        double parameter2 = cmd->parameter2();
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface1, sourceCurve1, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface2, sourceCurve2, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_UseStartEnd1, useStartEnd1);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_UseStartEnd2, useStartEnd2);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Parameter1, parameter1);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Parameter2, parameter2);

        return true;
    }
    bool  FITKAdaptorGeoModelSurfaceEdge::readParameters()
    {
        Interface::FITKAbsGeoModelSurfaceEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSurfaceEdge>();
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
        int sourceSurface[3] = {};
        int edgeVirtualTopoId = 0;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurface, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_EdgeVirtualTopoId, &edgeVirtualTopoId)) return false;

        // 设置读取的参数到cmd对象
        cmd->setOrigin(origin[0], origin[1], origin[2]);
        cmd->setYDirection(yDirection[0], yDirection[1], yDirection[2]);
        cmd->setZDirection(zDirection[0], zDirection[1], zDirection[2]);
        Interface::VirtualShape vShape1(sourceSurface[0], sourceSurface[1], sourceSurface[2]);
        cmd->setSourceSurface(vShape1);
        cmd->setEdgeVirtualTopoId(edgeVirtualTopoId);
        return true;
    }
    bool FITKAdaptorGeoModelSurfaceEdge::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSurfaceEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSurfaceEdge>();
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
        VirtualShape sourceSurfaceGet = cmd->sourceSurface();
        int sourceSurface[3]{ sourceSurfaceGet.CmdId, sourceSurfaceGet.VirtualTopoId, sourceSurfaceGet.VirtualTopoIndex };
        int edgeVirtualTopoId = cmd->edgeVirtualTopoId();
        // 写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Origin, origin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_XDirection, xDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_YDirection, yDirection.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_ZDirection, zDirection.data(), 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurface, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_EdgeVirtualTopoId, edgeVirtualTopoId);

        return true;
    }

    bool  FITKAdaptorGeoModelOperLine::readParameters()
    {
        Interface::FITKAbsGeoModelOperLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperLine>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 打开参数组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        // 读取参数
        std::string type = {};        
        double angle = 0;
        int count = 0;
        int sourceSurfaceArray[3] = {};
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Angle, &angle)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        // 设置读取的参数到cmd对象
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelOperLine::GeoLineOperType>();
        Interface::FITKAbsGeoModelOperLine::GeoLineOperType cmdType = (Interface::FITKAbsGeoModelOperLine::GeoLineOperType)metaEnum.keyToValue(type.c_str());
        cmd->setType(Interface::FITKAbsGeoModelOperLine::GeoLineOperType(cmdType));
        cmd->setAngle(angle);
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sourceSurfaceArray, 1, 3)) return false;
            Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
            cmd->add(vShape);
        }
        return true;
    }
    bool FITKAdaptorGeoModelOperLine::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOperLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperLine>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        FITKAbsGeoModelOperLine::GeoLineOperType type = cmd->getType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelOperLine::GeoLineOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        double angle = cmd->getAngle();
        auto shapes = cmd->getVShapes();
        int count = shapes.size();
        // 写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Angle, angle);
        for (int i = 0; i < count; i++)
        {
            int vShape[3]{ shapes[i].CmdId, shapes[i].VirtualTopoId, shapes[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShape, 1, 3);
        }
        return true;
    }
}



