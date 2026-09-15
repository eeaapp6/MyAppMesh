/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoDatumModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReferencePoint.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReferenceLine.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReferencePlane.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include <QMetaEnum>

namespace Interface
{
    bool  FITKAdaptorGeoDatumPoint::readParameters()
    {
        Interface::FITKAbsGeoDatumPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDatumPoint>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        double position[3] = { };

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        cmd->setPosition(position);
        
    }
    bool FITKAdaptorGeoDatumPoint::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoDatumPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDatumPoint>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        double position[3] = {};
        cmd->getPosition(position);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        return true;
    }
    bool  FITKAdaptorGeoDatumLine::readParameters()
    {
        Interface::FITKAbsGeoDatumLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDatumLine>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        double position[3] = { };
        double pos1[3] = {};
        double pos2[3] = {};
        bool isInfinite = false;

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos1, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Position2", pos2, 1, 3)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "IsInfinite", &isInfinite)) return false;

        cmd->setPosition(position);
        cmd->setFiniteLine(pos1, pos2);
        cmd->setIsInfinite(isInfinite);
        return true;
    }
    bool FITKAdaptorGeoDatumLine::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoDatumLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDatumLine>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        double position[3] = {};
        double pos1[3] = {};
        double pos2[3] = {};
        cmd->getPosition(position);
        cmd->getFiniteLine(pos1, pos2);
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        bool isInfinite = cmd->getIsInfinite();

        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos1, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "Position2", pos2, 1, 3);
        h5tools->writeGroupAttrBool(g, "IsInfinite", isInfinite);
        return true;
    }

    bool  FITKAdaptorModelReferenceLineByIntersectPlanes::readParameters()
    {
        Interface::FITKAbsGeoReferenceLineByIntersectPlanes* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceLineByIntersectPlanes>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        double position[3] = { };
        double pos1[3] = {};
        double pos2[3] = {};
        double plane1[9] = {};
        double plane2[9] = {};
        bool isInfinite = false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos1, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Position2", pos2, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "plane1", plane1, 1, 9)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "plane2", plane2, 1, 9)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "IsInfinite", &isInfinite)) return false;
        QVector<double> vec_plane1(plane1, plane1 + 9);
        QVector<double> vec_plane2(plane2, plane2 + 9);
        cmd->setPosition(position);
        cmd->setFiniteLine(pos1, pos2);
        cmd->setPlane1(vec_plane1);
        cmd->setPlane2(vec_plane2);
        cmd->setIsInfinite(isInfinite);
        return true;
    }
    bool FITKAdaptorModelReferenceLineByIntersectPlanes::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReferenceLineByIntersectPlanes* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceLineByIntersectPlanes>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        double position[3] = {};
        double pos1[3] = {};
        double pos2[3] = {};
        cmd->getPosition(position);
        cmd->getFiniteLine(pos1, pos2);
        QVector<double> plane1 = cmd->plane1();
        QVector<double> plane2 = cmd->plane2();
        bool isInfinite = cmd->getIsInfinite();
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos1, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "Position2", pos2, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "plane1", plane1.data(), 1, 9);
        h5tools->writeGroupAttrDoubleArray(g, "plane2", plane2.data(), 1, 9);
        h5tools->writeGroupAttrBool(g, "IsInfinite", isInfinite);

        return true;
    }

    bool  FITKAdaptorModelReferenceLineByRotateLine::readParameters()
    {
        Interface::FITKAbsGeoReferenceLineByRotateLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceLineByRotateLine>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        double position[3] = { };
        double pos1[3] = {};
        double pos2[3] = {};
        double lineStart[3] = {};
        double lineEnd[3] = {};
        double axisStart[3] = {};
        double axisEnd[3] = {};
        double angle = {};
        bool isInfinite = false;

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos1, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Position2", pos2, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "lineStart", lineStart, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "lineEnd", lineEnd, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "axisStart", axisStart, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "axisEnd", axisEnd, 1, 3)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, "angle", &angle)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "IsInfinite", &isInfinite)) return false;

        QVector<double> vecLineStart(lineStart, lineStart + 3);
        QVector<double> vecLineEnd(lineEnd, lineEnd + 3);
        QVector<double> vecAxisStart(axisStart, axisStart + 3);
        QVector<double> vecAxisEnd(axisEnd, axisEnd + 3);

        cmd->setPosition(position);
        cmd->setFiniteLine(pos1, pos2);
        cmd->setLine(vecLineStart, vecLineEnd);
        cmd->setAxis(vecAxisStart, vecAxisEnd);
        cmd->setAngle(angle);
        cmd->setIsInfinite(isInfinite);
        return true;
    }
    bool FITKAdaptorModelReferenceLineByRotateLine::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReferenceLineByRotateLine* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceLineByRotateLine>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        double position[3] = {};
        double pos1[3] = {};
        double pos2[3] = {};
        cmd->getPosition(position);
        cmd->getFiniteLine(pos1, pos2);
        std::array<double, 3> lineStart = cmd->lineStart();
        std::array<double, 3> lineEnd = cmd->lineEnd();
        std::array<double, 3> axisStart = cmd->axisStart();
        std::array<double, 3> axisEnd = cmd->axisEnd();
        double angle = cmd->angle();
        bool isInfinite = cmd->getIsInfinite();

        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos1, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "Position2", pos2, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "lineStart", lineStart.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "lineEnd", lineEnd.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "axisStart", axisStart.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "axisEnd", axisEnd.data(), 1, 3);
        h5tools->writeGroupAttrDouble(g, "angle", angle);
        h5tools->writeGroupAttrBool(g, "IsInfinite", isInfinite);
        return true;
    }

    bool  FITKAdaptorGeoDatumPlane::readParameters()
    {
        Interface::FITKAbsGeoDatumPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDatumPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = { };
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Up, up, 1, 3)) return false;

        cmd->setPosition(position);
        cmd->setPlane(pos, normal, up);

        return true;
    }
    bool FITKAdaptorGeoDatumPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoDatumPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDatumPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        cmd->getPosition(position);
        cmd->getPlane(pos, normal, up);
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();

        //Object Type
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Normal, normal, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Up, up, 1, 3);
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);

        return true;
    }

    bool  FITKAdaptorGeoReferenceOffsetPlane::readParameters()
    {
        Interface::FITKAbsGeoReferenceOffsetPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceOffsetPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        double direction[3] = {};
        int sourceSurface[3]{  };
        double offset = 0;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Up, up, 1, 3)) return false;
        //if (!h5tools->readGroupint(*_h5Group, FGKW_cmdID, &cmdId)) return false;
        //if (!h5tools->readGroupint(*_h5Group, FGKW_vTopoID, &virtualTopoId)) return false;
        //if (!h5tools->readGroupint(*_h5Group, FGKW_vTopoIndex, &virtualTopoIndex)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, FGKW_Offset, &offset)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Direction, direction, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(*_h5Group, "SourceSurface", sourceSurface, 1, 3)) return false;
        VirtualShape shape{ sourceSurface[0], sourceSurface[1], sourceSurface[2] };
        cmd->setSourceSurface(shape);
        cmd->setPosition(position[0], position[1], position[2]);
        cmd->setPlane(pos, normal, up);

        cmd->setOffset(offset);
        cmd->setDirection(std::array<double, 3>{direction[0], direction[1], direction[2]});

        return true;
    }
    bool FITKAdaptorGeoReferenceOffsetPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReferenceOffsetPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceOffsetPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        //获取参数
        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        cmd->getPosition(position);
        cmd->getPlane(pos, normal, up);
        std::array<double, 3> direction = cmd->direction();
        double offset = cmd->offset();
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();
        VirtualShape shape = cmd->sourceSurface();
        int sourceSurface[3]{ shape.CmdId, shape.VirtualTopoId,shape.VirtualTopoIndex };
        //Object Type
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Normal, normal, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Up, up, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Direction, direction.data(), 1, 3);
        h5tools->writeGroupAttrDouble(g, FGKW_Offset, offset);
        h5tools->writeGroupAttrIntArray(g, "SourceSurface", sourceSurface, 1, 3);

        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        return true;
    }

    bool  FITKAdaptorGeoReferenceThreePointsPlane::readParameters()
    {
        Interface::FITKAbsGeoReferenceThreePointsPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceThreePointsPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = { };
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Up, up, 1, 3)) return false;
        for (int i = 0; i < 3; i++)
        {
            double P_i[3] = {};
            if (!h5tools->readGroupAttrDoubleArray(*_h5Group, std::to_string(i).c_str(), P_i, 1, 3)) return false;
            cmd->setPoint(i, P_i[0], P_i[1], P_i[2]);
        }
        cmd->setPosition(position[0], position[1], position[2]);
        cmd->setPlane(pos, normal, up);

        return true;
    }
    bool FITKAdaptorGeoReferenceThreePointsPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReferenceThreePointsPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceThreePointsPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //获取参数
        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        cmd->getPosition(position);
        cmd->getPlane(pos, normal, up);
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();

        //Object Type
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        //写出参数
        for (int i = 0; i < 3; i++)
        {
            std::array<double, 3> point = cmd->point(i);
            double P_i[3]{ point[0],point[1],point[2] };
            h5tools->writeGroupAttrDoubleArray(g, std::to_string(i).c_str(), P_i, 1, 3);
        }
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Normal, normal, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Up, up, 1, 3);
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        return true;
    }

    bool  FITKAdaptorGeoReferenceEquationPlane::readParameters()
    {
        Interface::FITKAbsGeoReferenceEquationPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceEquationPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = { };
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        double a = {};
        double b = {};
        double c = {};
        double d = {};
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Up, up, 1, 3)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, FGKW_A, &a)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, FGKW_B, &b)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, FGKW_C, &c)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, FGKW_D, &d)) return false;

        cmd->setPosition(position[0], position[1], position[2]);
        cmd->setPlane(pos, normal, up);
        cmd->setA(a);
        cmd->setB(b);
        cmd->setC(c);
        cmd->setD(d);
        return true;
    }
    bool FITKAdaptorGeoReferenceEquationPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReferenceEquationPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceEquationPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        cmd->getPosition(position);
        cmd->getPlane(pos, normal, up);
        double a = cmd->a();
        double b = cmd->b();
        double c = cmd->c();
        double d = cmd->d();
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();

        //Object Type
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Normal, normal, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Up, up, 1, 3);
        h5tools->writeGroupAttrDouble(g, FGKW_A, a);
        h5tools->writeGroupAttrDouble(g, FGKW_B, b);
        h5tools->writeGroupAttrDouble(g, FGKW_C, c);
        h5tools->writeGroupAttrDouble(g, FGKW_D, d);
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);

        return true;
    }

    bool  FITKAdaptorGeoReferencePointAndDirectionPlane::readParameters()
    {
        Interface::FITKAbsGeoReferencePointAndDirectionPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferencePointAndDirectionPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = { };
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        double point[3] = {};
        double direction[3] = {};

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Up, up, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Point, point, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Direction, direction, 1, 3)) return false;

        cmd->setPosition(position[0], position[1], position[2]);
        cmd->setPlane(pos, normal, up);
        cmd->setPoint(point[0], point[1], point[2]);
        cmd->setDirection(direction[0], direction[1], direction[2]);

        return true;
    }
    bool FITKAdaptorGeoReferencePointAndDirectionPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReferencePointAndDirectionPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferencePointAndDirectionPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        cmd->getPosition(position);
        cmd->getPlane(pos, normal, up);
        std::array<double, 3> point = cmd->point();
        std::array<double, 3> direction = cmd->direction();
        
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();

        //Object Type
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position, position, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Position1, pos, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Normal, normal, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Up, up, 1, 3);
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Point, point.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, FGKW_Direction, direction.data(), 1, 3);

        return true;
    }
    bool  FITKAdaptorModelReferenceRotateFromPlanePlane::readParameters()
    {
        Interface::FITKAbsGeoReferenceRotateFromPlanePlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceRotateFromPlanePlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        double pos1[3] = { };
        double pos[3] = {};
        double nor[3] = {};
        double nor1[3] = {};
        double up[3] = {};
        double up1[3] = {};
        double axisStart[3] = {  };
        double axisEnd[3] = { };
        double angle = {};

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Position", pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Normal", nor, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "UP", up, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_RotateAxisPoint1, axisStart, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_RotateAxisPoint2, axisEnd, 1, 3)) return false;
        if (!h5tools->readGroupDouble(*_h5Group, FGKW_Angle, &angle)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Position1", pos1, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "Normal1", nor1, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, "UP1", up1, 1, 3)) return false;
        cmd->setPosition(pos1[0], pos1[1], pos1[2]);
        cmd->setNormal(nor1[0], nor1[1], nor1[2]);
        cmd->setUp(up1[0], up1[1], up1[2]);
        // 构造平面参数向量并设置
        QVector<double> planeParams;
        planeParams.append(pos[0]);
        planeParams.append(pos[1]);
        planeParams.append(pos[2]);
        planeParams.append(nor[0]);
        planeParams.append(nor[1]);
        planeParams.append(nor[2]);
        planeParams.append(up[0]);
        planeParams.append(up[1]);
        planeParams.append(up[2]);
        cmd->setPlane(planeParams);
        // 构造旋转轴参数并设置
        QVector<double> startVec = { axisStart[0], axisStart[1], axisStart[2] };
        QVector<double> endVec = { axisEnd[0], axisEnd[1], axisEnd[2] };
        cmd->setAxis(startVec, endVec);
        // 设置旋转角度
        cmd->setAngle(angle);
        return true;
    }
    bool FITKAdaptorModelReferenceRotateFromPlanePlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReferenceRotateFromPlanePlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReferenceRotateFromPlanePlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        double pos1[3] = {};
        double nor1[3] = {};
        double up1[3] = {};
        std::array<double, 3> pos = cmd->planePosition();
        std::array<double, 3> normal = cmd->planeNormal();
        std::array<double, 3> up = cmd->planeUp();
        cmd->getPosition(pos1);
        cmd->getNormal(nor1);
        cmd->getUp(up1);
        Interface::FITKGeoEnum::FITKDatumType t = cmd->getDatumType();
        std::array<double, 3> axisStart = cmd->axisStart();
        std::array<double, 3> axisEnd = cmd->axisEnd();
        double angle = cmd->angle();
        //Object Type
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKDatumType>();
        std::string strDatumType = metaEnum.valueToKey(t);
        if (strDatumType.empty()) return false;
        //写出参数
        h5tools->writeGroupAttrDoubleArray(g, "Position", pos.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "Normal", normal.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "UP", up.data(), 1, 3);
        if (!h5tools->writeGroupAttrDoubleArray(g, FGKW_RotateAxisPoint1, axisStart.data(), 1, 3)) return false;
        if (!h5tools->writeGroupAttrDoubleArray(g, FGKW_RotateAxisPoint2, axisEnd.data(), 1, 3)) return false;
        if (!h5tools->writeGroupAttrDouble(g, FGKW_Angle, angle)) return false;
        h5tools->writeGroupAttrStr(g, FGKW_Type, strDatumType);

        h5tools->writeGroupAttrDoubleArray(g, "Position1", pos1, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "Normal1", nor1, 1, 3);
        h5tools->writeGroupAttrDoubleArray(g, "UP1", up1, 1, 3);
        return true;
    }
}
