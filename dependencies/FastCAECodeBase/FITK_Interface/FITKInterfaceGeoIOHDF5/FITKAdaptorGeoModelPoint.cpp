/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelPoint.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPoint.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExtractCenter.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelNearestPointLine.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool FITKAdaptorGeoPoint::readParameters()
    {
        Interface::FITKAbsGeoModelPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPoint>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        return true;
    }
    bool FITKAdaptorGeoPoint::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPoint>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        
        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz,1,3);
        return true;
    }
    bool FITKAdaptorGeoCurveEnd::readParameters()
    {
        Interface::FITKAbsGeoModelCurveEnd* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveEnd>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceCurveArray[3] = { 0,0,0 };
        bool useStartEnd = false;
        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_UseStartEnd,&useStartEnd)) return false;
        Interface::VirtualShape vShape(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSourceCurve(vShape);
        cmd->setUseStartEnd(useStartEnd);
        return true;
    }
    bool FITKAdaptorGeoCurveEnd::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCurveEnd* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveEnd>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        VirtualShape sourceCurve = cmd->sourceCurve();
        int sourceCurveArray[3]{ sourceCurve.CmdId,sourceCurve.VirtualTopoId,sourceCurve.VirtualTopoIndex };
        bool useStartEnd = cmd->useStartEnd();
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_UseStartEnd, useStartEnd);
        return true;
    }

    bool FITKAdaptorGeoCurveEndDistance::readParameters()
    {
        Interface::FITKAbsGeoModelCurveEndDistance* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveEndDistance>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceCurveArray[3] = { 0,0,0 };
        double distance =0;
        bool useStartEnd = false;
        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), FGKW_UseStartEnd, &useStartEnd)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Distance, &distance)) return false;
        Interface::VirtualShape vShape(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSourceCurve(vShape);
        cmd->setUseStartEnd(useStartEnd);
        cmd->setDistance(distance);
        return true;
    }
    bool FITKAdaptorGeoCurveEndDistance::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCurveEndDistance* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveEndDistance>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        VirtualShape sourceCurve = cmd->sourceCurve();
        int sourceCurveArray[3]{ sourceCurve.CmdId,sourceCurve.VirtualTopoId,sourceCurve.VirtualTopoIndex };
        bool useStartEnd = cmd->useStartEnd();
        double distance = cmd->distance();
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), FGKW_UseStartEnd, useStartEnd);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Distance, distance);
        return true;
    }

    bool FITKAdaptorGeoCurveRadio::readParameters()
    {
        Interface::FITKAbsGeoModelCurveRadio* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveRadio>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceCurveArray[3] = { 0,0,0 };
        double radio = 0;
        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Radio, &radio)) return false;

        Interface::VirtualShape vShape(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSourceCurve(vShape);
        cmd->setRadio(radio);
        return true;
    }
    bool FITKAdaptorGeoCurveRadio::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCurveRadio* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveRadio>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        VirtualShape sourceCurve = cmd->sourceCurve();
        int sourceCurveArray[3]{ sourceCurve.CmdId,sourceCurve.VirtualTopoId,sourceCurve.VirtualTopoIndex };
        double radio = cmd->radio();
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Radio, radio);

        return true;
    }

    bool FITKAdaptorGeoPointProjectionOnCurve::readParameters()
    {
        Interface::FITKAbsGeoModelPointProjectionOnCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPointProjectionOnCurve>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceCurveArray[3] = { 0,0,0};
        int sourcePointArray[3] = { 0,0,0 };

        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourcePoint, sourcePointArray, 1, 3)) return false;

        Interface::VirtualShape vShape(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]);
        Interface::VirtualShape vShapePoint(sourcePointArray[0], sourcePointArray[1], sourcePointArray[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSourceCurve(vShape);
        cmd->setSourcePoint(vShapePoint);
        return true;
    }
    bool FITKAdaptorGeoPointProjectionOnCurve::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPointProjectionOnCurve* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPointProjectionOnCurve>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        VirtualShape sourceCurve = cmd->sourceCurve();
        int sourceCurveArray[3]{ sourceCurve.CmdId,sourceCurve.VirtualTopoId,sourceCurve.VirtualTopoIndex };
        VirtualShape sourcePoint = cmd->sourcePoint();
        int sourcePointArray[3]{ sourcePoint.CmdId,sourcePoint.VirtualTopoId,sourcePoint.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourcePoint, sourcePointArray, 1, 3);

        return true;
    }

    bool FITKAdaptorGeoTwoCurveIntersection::readParameters()
    {
        Interface::FITKAbsGeoModelTwoCurveIntersection* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTwoCurveIntersection>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceCurve1Array[3] = { 0,0,0 };
        int sourceCurve2Array[3] = {0,0,0};
        bool isNearest = false;
        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve1, sourceCurve1Array, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve2, sourceCurve2Array, 1, 3)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), "isNearest", &isNearest)) return false;

        Interface::VirtualShape sourceCurve1(sourceCurve1Array[0], sourceCurve1Array[1], sourceCurve1Array[2]);
        Interface::VirtualShape sourceCurve2(sourceCurve2Array[0], sourceCurve2Array[1], sourceCurve2Array[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSourceCurve(sourceCurve1, sourceCurve2);
        cmd->setNearestPoint(isNearest);
        return true;
    }
    bool FITKAdaptorGeoTwoCurveIntersection::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelTwoCurveIntersection* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelTwoCurveIntersection>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        VirtualShape sourceCurve1 = cmd->sourceCurve1();
        int sourceCurve1Array[3]{ sourceCurve1.CmdId,sourceCurve1.VirtualTopoId,sourceCurve1.VirtualTopoIndex };
        VirtualShape sourceCurve2 = cmd->sourceCurve2();
        int sourceCurve2Array[3]{ sourceCurve2.CmdId,sourceCurve2.VirtualTopoId,sourceCurve2.VirtualTopoIndex };
        bool isNearest = cmd->isNearestPoint();
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve1, sourceCurve1Array, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve2, sourceCurve2Array, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), "isNearest", isNearest);

        QVector<std::array<double, 3>> points = cmd->getResultPoints();
        int count = points.size();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            auto point = points[i];
            double pArray[3]{ point[0], point[1], point[2] };
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), std::to_string(i).c_str(), pArray, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoPointProjectionOnSurface::readParameters()
    {
        Interface::FITKAbsGeoModelPointProjectionOnSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPointProjectionOnSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceSurfaceArray[3] = { 0,0,0};
        int sourcePointArray[3] = { 0,0,0};

        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourcePoint, sourcePointArray, 1, 3)) return false;

        Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
        Interface::VirtualShape vShapePoint(sourcePointArray[0], sourcePointArray[1], sourcePointArray[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSourceSurface(vShape);
        cmd->setSourcePoint(vShapePoint);
        return true;
    }
    bool FITKAdaptorGeoPointProjectionOnSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPointProjectionOnSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPointProjectionOnSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        VirtualShape sourceSurface = cmd->sourceSurface();
        int sourceSurfaceArray[3]{ sourceSurface.CmdId,sourceSurface.VirtualTopoId,sourceSurface.VirtualTopoIndex };
        VirtualShape sourcePoint = cmd->sourcePoint();
        int sourcePointArray[3]{ sourcePoint.CmdId,sourcePoint.VirtualTopoId,sourcePoint.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourcePoint, sourcePointArray, 1, 3);

        return true;
    }

    bool FITKAdaptorGeoCurveIntersectionSurface::readParameters()
    {
        Interface::FITKAbsGeoModelCurveIntersectionSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveIntersectionSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceSurfaceArray[3] = {0,0,0 };
        int sourceCurveArray[3] = { 0,0,0 };

        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3)) return false;

        Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
        Interface::VirtualShape vShapeCurve(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSourceSurface(vShape);
        cmd->setSourceCurve(vShapeCurve);
        return true;
    }
    bool FITKAdaptorGeoCurveIntersectionSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCurveIntersectionSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCurveIntersectionSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };
        VirtualShape sourceSurface = cmd->sourceSurface();
        int sourceSurfaceArray[3]{ sourceSurface.CmdId,sourceSurface.VirtualTopoId,sourceSurface.VirtualTopoIndex };
        VirtualShape sourceCurve = cmd->sourceCurve();
        int sourceCurveArray[3]{ sourceCurve.CmdId,sourceCurve.VirtualTopoId,sourceCurve.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3);

        return true;
    }

    bool FITKAdaptorGeoInteractionPoint::readParameters()
    {
        Interface::FITKAbsGeoModelInteractionPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelInteractionPoint>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int sourceArray[3] = {0,0,0 };
        int type = 0;
        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sourceArray, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Type, &type)) return false;
        Interface::VirtualShape source(sourceArray[0], sourceArray[1], sourceArray[2]);
        cmd->setCoord(xyz[0], xyz[1], xyz[2]);
        cmd->setSource(source);
        cmd->setType(type);
        return true;
    }
    bool FITKAdaptorGeoInteractionPoint::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelInteractionPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelInteractionPoint>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3]{ cmd->x(),cmd->y(), cmd->z() };

        VirtualShape source = cmd->source();
        int sourceArray[3]{ source.CmdId,source.VirtualTopoId,source.VirtualTopoIndex };
        int type = cmd->type();
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, sourceArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Type, type);

        return true;
    }

    bool FITKAdaptorGeoExtractCenter::readParameters()
    {
        Interface::FITKAbsGeoModelExtractCenter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtractCenter>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = { 0.0, 0.0, 0.0 };
        int shapeArray[3] = { 0,0,0 };
        std::string type = {};
        //读取参数
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Center, xyz, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, shapeArray, 1, 3)) return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        Interface::VirtualShape shape(shapeArray[0], shapeArray[1], shapeArray[2]);
        cmd->setCenter(xyz);
        cmd->setShape(shape);
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelExtractCenter::ShapeType>();
        Interface::FITKAbsGeoModelExtractCenter::ShapeType cmdType = (Interface::FITKAbsGeoModelExtractCenter::ShapeType)metaEnum.keyToValue(type.c_str());
        cmd->setShapeType(Interface::FITKAbsGeoModelExtractCenter::ShapeType(cmdType));
        return true;
    }
    bool FITKAdaptorGeoExtractCenter::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelExtractCenter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtractCenter>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        double xyz[3] = {};
        cmd->getCenter(xyz);
        VirtualShape shape = cmd->getShape();
        int shapeArray[3]{ shape.CmdId,shape.VirtualTopoId,shape.VirtualTopoIndex };
        FITKAbsGeoModelExtractCenter::ShapeType type = cmd->getShapeType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelExtractCenter::ShapeType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Center, xyz, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, shapeArray, 1, 3);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        return true;
    }

}
