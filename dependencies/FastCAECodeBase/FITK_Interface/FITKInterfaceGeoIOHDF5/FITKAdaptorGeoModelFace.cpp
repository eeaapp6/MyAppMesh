/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelFace.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperFace.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCoverEdges.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoStitch.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReplaceFaces.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOffsetFace.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool  FITKAdaptorGeoModelBridgeSurface::readParameters()
    {
        Interface::FITKAbsGeoModelBridgeSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBridgeSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sourceSurface1Array[3] = {};
        int sourceSurface2Array[3] = {};
        int edgeVirtualTopoId1 = 0;
        int edgeVirtualTopoId2 = 0;
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface1, sourceSurface1Array, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface2, sourceSurface2Array, 1, 3))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_EdgeVirtualTopoId1, &edgeVirtualTopoId1))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_EdgeVirtualTopoId2, &edgeVirtualTopoId2))return false;
        //设置参数
        cmd->setSourceSurface1(Interface::VirtualShape(sourceSurface1Array[0], sourceSurface1Array[1], sourceSurface1Array[2]));
        cmd->setSourceSurface2(Interface::VirtualShape(sourceSurface2Array[0], sourceSurface2Array[1], sourceSurface2Array[2]));
        cmd->setEdgeVirtualTopoId1(edgeVirtualTopoId1);
        cmd->setEdgeVirtualTopoId2(edgeVirtualTopoId2);
        return true;
    }
    bool FITKAdaptorGeoModelBridgeSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelBridgeSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelBridgeSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        //获取参数
        Interface::VirtualShape sourceSurface1 = cmd->sourceSurface1();
        int sourceSurface1Array[3]{ sourceSurface1.CmdId ,sourceSurface1.VirtualTopoId,sourceSurface1.VirtualTopoIndex };
        int edgeVirtualTopoId1 = cmd->edgeVirtualTopoId1();
        Interface::VirtualShape sourceSurface2 = cmd->sourceSurface2();
        int sourceSurface2Array[3]{ sourceSurface2.CmdId ,sourceSurface2.VirtualTopoId,sourceSurface2.VirtualTopoIndex };
        int edgeVirtualTopoId2 = cmd->edgeVirtualTopoId2();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface1, sourceSurface1Array,1,3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface2, sourceSurface2Array, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_EdgeVirtualTopoId1, edgeVirtualTopoId1);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_EdgeVirtualTopoId2, edgeVirtualTopoId2);
        return true;
    }

    bool  FITKAdaptorGeoModelSolidSurface::readParameters()
    {
        Interface::FITKAbsGeoModelSolidSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSolidSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sourceSolidArray[3] = {};
        int faceVirtualTopoId = 0;
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSolid, sourceSolidArray, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_FaceVirtualTopoId, &faceVirtualTopoId))return false;
        //设置参数
        cmd->setSourceSolid(Interface::VirtualShape(sourceSolidArray[0], sourceSolidArray[1], sourceSolidArray[2]));
        cmd->setFaceVirtualTopoId(faceVirtualTopoId);
        return true;
    }
    bool FITKAdaptorGeoModelSolidSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSolidSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSolidSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        Interface::VirtualShape sourceSolid = cmd->sourceSolid();
        int sourceSolidArray[3]{ sourceSolid.CmdId ,sourceSolid.VirtualTopoId,sourceSolid.VirtualTopoIndex };
        int faceVirtualTopoId = cmd->faceVirtualTopoId();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSolid, sourceSolidArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_FaceVirtualTopoId, faceVirtualTopoId);
        return true;
    }

    bool  FITKAdaptorGeoModelMultiSectionSurface::readParameters()
    {
        Interface::FITKAbsGeoModelMultiSectionSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMultiSectionSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int count = 0;
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        for (int i = 0; i < count; i++)
        {
            int curveArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), curveArray, 1, 3)) return false;
            cmd->addSection(Interface::VirtualShape(curveArray[0], curveArray[1], curveArray[2]));
        }
        return true;
    }
    bool FITKAdaptorGeoModelMultiSectionSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelMultiSectionSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMultiSectionSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<VirtualShape> sections = cmd->sections();
        int count = sections.size();
        //写出参数
        for (int i = 0; i < count; i++)
        {
            VirtualShape section = sections[i];
            int curveArray[3]{ section.CmdId ,section.VirtualTopoId, section.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(),curveArray, 1, 3);
        }
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        return true;
    }

    bool  FITKAdaptorGeoModelSweepSurface::readParameters()
    {
        Interface::FITKAbsGeoModelSweepSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSweepSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int profileArray[3] = {};
        int curveArray[3] = {};
        bool flipPath = false;
        //读取参数
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), "flipPath", &flipPath))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Profile, profileArray,1,3))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Curve, curveArray,1,3))return false;
        //设置参数
        cmd->setProfile(Interface::VirtualShape(profileArray[0], profileArray[1], profileArray[2]));
        cmd->setCurve(Interface::VirtualShape(curveArray[0], curveArray[1], curveArray[2]));
        cmd->setFlipPath(flipPath);
        return true;
    }
    bool FITKAdaptorGeoModelSweepSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSweepSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSweepSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        bool flipPath = cmd->flipPath();
        //写出参数
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), "flipPath", flipPath);
        Interface::VirtualShape profile = cmd->profile();
        int profileArray[3]{ profile.CmdId ,profile.VirtualTopoId, profile.VirtualTopoIndex };
        Interface::VirtualShape curve = cmd->curve();
        int curveArray[3]{ curve.CmdId ,curve.VirtualTopoId, curve.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Profile, profileArray,1,3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Curve, curveArray,1,3);
        return true;
    }

    bool  FITKAdaptorGeoModelRevolSurface::readParameters()
    {
        Interface::FITKAbsGeoModelRevolSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRevolSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sourceCurveArray[3] = {};
        double rotateAxisPoint1Array[3] = {};
        double rotateAxisPoint2Array[3] = {};
        int angle = 0;
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint1, rotateAxisPoint1Array, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint2, rotateAxisPoint2Array, 1, 3))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Angle, &angle))return false;
        //设置参数
        cmd->setSourveCurve(Interface::VirtualShape(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]));
        cmd->setRotateAxisPoint1(rotateAxisPoint1Array[0], rotateAxisPoint1Array[1], rotateAxisPoint1Array[2]);
        cmd->setRotateAxisPoint2(rotateAxisPoint2Array[0], rotateAxisPoint2Array[1], rotateAxisPoint2Array[2]);
        cmd->setAngle(angle);
        return true;
    }
    bool FITKAdaptorGeoModelRevolSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelRevolSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRevolSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        Interface::VirtualShape sourceCurve = cmd->sourceCurve();
        int sourceCurveArray[3]{ sourceCurve.CmdId ,sourceCurve.VirtualTopoId, sourceCurve.VirtualTopoIndex };
        auto rotateAxisPoint1 = cmd->rotateAxisPoint1();
        double rotateAxisPoint1Array[3]{ rotateAxisPoint1[0],rotateAxisPoint1[1],rotateAxisPoint1[2] };
        auto rotateAxisPoint2 = cmd->rotateAxisPoint2();
        double rotateAxisPoint2Array[3]{ rotateAxisPoint2[0],rotateAxisPoint2[1],rotateAxisPoint2[2] };
        int angle = cmd->angle();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceCurve, sourceCurveArray, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint1, rotateAxisPoint1Array, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint2, rotateAxisPoint2Array, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Angle, angle);
        return true;
    }

    bool  FITKAdaptorGeoModelExtrudeSurface::readParameters()
    {
        Interface::FITKAbsGeoModelExtrudeSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtrudeSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sourceCurveArray[3] = {};
        int extrudeType = 0;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ExtrudeSurface, sourceCurveArray, 1, 3))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "extrudeType", &extrudeType))return false;
        if (extrudeType == 1)
        {
            double directionArray[3] = {};
            double length = 0;
            //读取参数
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, directionArray, 1, 3))return false;
            if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Length, &length))return false;
            //设置参数
            cmd->setSourceCurve(Interface::VirtualShape(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]));
            cmd->setDirection(directionArray[0], directionArray[1], directionArray[2]);
            cmd->setLength(length);
        }
        else
        {
            int upToFaceArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "upToFaceArray", upToFaceArray, 1, 3))return false;
            //设置参数
            cmd->setSourceCurve(Interface::VirtualShape(sourceCurveArray[0], sourceCurveArray[1], sourceCurveArray[2]));
            cmd->setUpToFace(Interface::VirtualShape(upToFaceArray[0], upToFaceArray[1], upToFaceArray[2]));
        }
        return true;
    }
    bool FITKAdaptorGeoModelExtrudeSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelExtrudeSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtrudeSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        Interface::VirtualShape sourceCurve = cmd->sourceCurve();
        int sourceCurveArray[3]{ sourceCurve.CmdId ,sourceCurve.VirtualTopoId, sourceCurve.VirtualTopoIndex };
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ExtrudeSurface, sourceCurveArray, 1, 3);
        int extrudeType = cmd->extrudeType();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "extrudeType", extrudeType);
        if (extrudeType == 1)
        {
            auto direction = cmd->direction();
            double directionArray[3]{ direction[0],direction[1],direction[2] };
            double length = cmd->length();
            //写出参数
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, directionArray, 1, 3);
            h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Length, length);
        }
        else
        {
            Interface::VirtualShape upToFace = cmd->upToFace();
            int upToFaceArray[3]{ upToFace.CmdId ,upToFace.VirtualTopoId, upToFace.VirtualTopoIndex };
            //写出参数
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "upToFaceArray", upToFaceArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelOffsetSurface::readParameters()
    {
        Interface::FITKAbsGeoModelOffsetSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOffsetSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int sourceSurfaceArray[3] = {};
        double offset = 0;
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Offset, &offset))return false;
        //设置参数
        cmd->setSourceSurface(Interface::VirtualShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]));
        cmd->setOffset(offset);
        return true;
    }
    bool FITKAdaptorGeoModelOffsetSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOffsetSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOffsetSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        Interface::VirtualShape sourceSurface = cmd->sourceSurface();
        int sourceSurfaceArray[3]{ sourceSurface.CmdId ,sourceSurface.VirtualTopoId, sourceSurface.VirtualTopoIndex };
        double offset = cmd->offset();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Offset, offset);
        return true;
    }

    bool  FITKAdaptorGeoModelClosedWireSurface::readParameters()
    {
        Interface::FITKAbsGeoModelClosedWireSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelClosedWireSurface>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int edgeArray[3] = {};
        int count = 0;
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edgeArray, 1, 3)) return false;
            cmd->addEdge(Interface::VirtualShape(edgeArray[0], edgeArray[1], edgeArray[2]));
        }
        return true;
    }
    bool FITKAdaptorGeoModelClosedWireSurface::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelClosedWireSurface* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelClosedWireSurface>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> edges = cmd->edges();
        int count = edges.count();
        //写出参数
        for (int i = 0; i < count; i++)
        {
            VirtualShape edge = edges[i];
            int edgeArray[3]{ edge.CmdId ,edge.VirtualTopoId, edge.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(),edgeArray, 1, 3);
        }
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        return true;
    }

    bool  FITKAdaptorGeoModelOperFaceFillHoles::readParameters()
    {
        Interface::FITKAbsGeoModelOperFaceFillHoles* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceFillHoles>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        std::string type = {};
        double tolerance = 0;
        int count = 0;
        int sourceSurfaceArray[3] = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Tolerance, &tolerance))return false;

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType>();
        Interface::FITKAbsGeoModelOperFace::GeoFaceOperType cmdType = (Interface::FITKAbsGeoModelOperFace::GeoFaceOperType)metaEnum.keyToValue(type.c_str());
        cmd->setType(Interface::FITKAbsGeoModelOperFace::GeoFaceOperType(cmdType));
        cmd->setTolerance(tolerance);
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sourceSurfaceArray, 1, 3)) return false;
            Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
            cmd->add(vShape);
        }
        return true;
    }
    bool FITKAdaptorGeoModelOperFaceFillHoles::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOperFaceFillHoles* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceFillHoles>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        FITKAbsGeoModelOperFace::GeoFaceOperType type = cmd->getType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        double tolerance = cmd->getTolerance();
        QList<VirtualShape> shapes = cmd->getVShapes();
        int count = shapes.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Tolerance, tolerance);
        for (int i = 0; i < count; i++)
        {
            int vShape[3]{ shapes[i].CmdId, shapes[i].VirtualTopoId,shapes[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShape, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelOperFaceFillGaps::readParameters()
    {
        Interface::FITKAbsGeoModelOperFaceFillGaps* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceFillGaps>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        std::string type = {};
        double tolerance = 0;
        int count = 0;
        int sourceSurfaceArray[3] = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Tolerance, &tolerance))return false;

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType>();
        Interface::FITKAbsGeoModelOperFace::GeoFaceOperType cmdType = (Interface::FITKAbsGeoModelOperFace::GeoFaceOperType)metaEnum.keyToValue(type.c_str());
        cmd->setType(Interface::FITKAbsGeoModelOperFace::GeoFaceOperType(cmdType));
        cmd->setTolerance(tolerance);
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sourceSurfaceArray, 1, 3)) return false;
            Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
            cmd->add(vShape);
        }
        return true;
    }
    bool FITKAdaptorGeoModelOperFaceFillGaps::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOperFaceFillGaps* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceFillGaps>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        FITKAbsGeoModelOperFace::GeoFaceOperType type = cmd->getType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        double tolerance = cmd->getTolerance();
        QList<VirtualShape> shapes = cmd->getVShapes();
        int count = shapes.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Tolerance, tolerance);
        for (int i = 0; i < count; i++)
        {
            int vShape[3]{ shapes[i].CmdId, shapes[i].VirtualTopoId,shapes[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShape, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelOperFaceDeleteFloatingEdge::readParameters()
    {
        Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        std::string type = {};
        double tolerance = 0;
        int count = 0;
        int sourceSurfaceArray[3] = {};
        int vFaceArray[3] = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Tolerance, &tolerance))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_vFace, vFaceArray,1,3))return false;
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType>();
        Interface::FITKAbsGeoModelOperFace::GeoFaceOperType cmdType = (Interface::FITKAbsGeoModelOperFace::GeoFaceOperType)metaEnum.keyToValue(type.c_str());
        cmd->setType(Interface::FITKAbsGeoModelOperFace::GeoFaceOperType(cmdType));
        cmd->setTolerance(tolerance);
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sourceSurfaceArray, 1, 3)) return false;
            Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
            cmd->add(vShape);
        }
        Interface::VirtualShape vShape(vFaceArray[0], vFaceArray[1], vFaceArray[2]);
        cmd->setMFace(vShape);
        return true;
    }
    bool FITKAdaptorGeoModelOperFaceDeleteFloatingEdge::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceDeleteFloatingEdge>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        FITKAbsGeoModelOperFace::GeoFaceOperType type = cmd->getType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        double tolerance = cmd->getTolerance();
        QList<VirtualShape> shapes = cmd->getVShapes();
        int count = shapes.size();
        VirtualShape vFace = cmd->getMFace();
        int vFaceArray[3]{ vFace.CmdId,vFace.VirtualTopoId, vFace.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Tolerance, tolerance);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_vFace, vFaceArray,1,3);
        for (int i = 0; i < count; i++)
        {
            int vShape[3]{ shapes[i].CmdId, shapes[i].VirtualTopoId,shapes[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShape, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelCoverEdges::readParameters()
    {
        Interface::FITKAbsGeoCoverEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCoverEdges>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int count = 0;
        QList<VirtualShape> edges = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        for (int i = 0; i < count; i++)
        {
            int edgeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edgeArray, 1, 3)) return false;
            edges.append(Interface::VirtualShape(edgeArray[0], edgeArray[1], edgeArray[2]));
        }
        cmd->setEdges(edges);
        return true;
    }
    bool FITKAdaptorGeoModelCoverEdges::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoCoverEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCoverEdges>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<VirtualShape> edges = cmd->edges();
        int count = edges.size();
        //写出参数
        for (int i = 0; i < count; i++)
        {
            VirtualShape edge = edges[i];
            int edgeArray[3]{ edge.CmdId ,edge.VirtualTopoId, edge.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edgeArray, 1, 3);
        }
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        return true;
    }

    bool  FITKAdaptorGeoStitch::readParameters()
    {
        Interface::FITKAbsGeoStitch* cmd = this->getDataObjectAs<Interface::FITKAbsGeoStitch>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int count = 0;
        double tolerance = 0;
        QList<VirtualShape> edges = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), "tolerance", &tolerance))return false;
        for (int i = 0; i < count; i++)
        {
            int edgeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edgeArray, 1, 3)) return false;
            edges.append(Interface::VirtualShape(edgeArray[0], edgeArray[1], edgeArray[2]));
        }
        cmd->setEdges(edges);
        cmd->setTolerance(tolerance);
        return true;
    }
    bool FITKAdaptorGeoStitch::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoStitch* cmd = this->getDataObjectAs<Interface::FITKAbsGeoStitch>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        double tolerance = cmd->tolerance();
        QList<VirtualShape> edges = cmd->edges();
        int count = edges.size();
        //写出参数
        for (int i = 0; i < count; i++)
        {
            VirtualShape edge = edges[i];
            int edgeArray[3]{ edge.CmdId ,edge.VirtualTopoId, edge.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edgeArray, 1, 3);
        }
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), "tolerance", tolerance);
        return true;
    }

    bool  FITKAdaptorModelReplaceFaces::readParameters()
    {
        Interface::FITKAbsGeoReplaceFaces* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReplaceFaces>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        QList<Interface::VirtualShape> faces = {};
        int count = 0;
        bool extend = false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), "extend", &extend)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        cmd->setFaces(faces);
        cmd->setExtend(extend);
        return true;
    }
    bool FITKAdaptorModelReplaceFaces::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoReplaceFaces* cmd = this->getDataObjectAs<Interface::FITKAbsGeoReplaceFaces>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<VirtualShape> faces = cmd->faces();
        int count = faces.size();
        bool extend = cmd->isExtend();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), "extend", extend);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorModelRemoveFace::readParameters()
    {
        Interface::FITKAbsGeoModelOperFaceRemoveFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceRemoveFace>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        std::string type = {};
        double tolerance = 0;
        int count = 0;
        int sourceSurfaceArray[3] = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type, type)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Tolerance, &tolerance))return false;

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType>();
        Interface::FITKAbsGeoModelOperFace::GeoFaceOperType cmdType = (Interface::FITKAbsGeoModelOperFace::GeoFaceOperType)metaEnum.keyToValue(type.c_str());
        cmd->setType(Interface::FITKAbsGeoModelOperFace::GeoFaceOperType(cmdType));
        cmd->setTolerance(tolerance);
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sourceSurfaceArray, 1, 3)) return false;
            Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
            cmd->add(vShape);
        }
        return true;
    }
    bool FITKAdaptorModelRemoveFace::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOperFaceRemoveFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperFaceRemoveFace>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        FITKAbsGeoModelOperFace::GeoFaceOperType type = cmd->getType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelOperFace::GeoFaceOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        double tolerance = cmd->getTolerance();
        QList<VirtualShape> shapes = cmd->getVShapes();
        int count = shapes.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Tolerance, tolerance);
        for (int i = 0; i < count; i++)
        {
            int vShape[3]{ shapes[i].CmdId, shapes[i].VirtualTopoId,shapes[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShape, 1, 3);
        }
        return true;
    }
    // 偏移面适配器。
    //@{
    bool FITKAdaptorGeoModelOffsetFace::readParameters()
    {
        Interface::FITKAbsGeoOffsetFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoOffsetFace>();
        if (!cmd || !_reader || !_h5Group)
        {
            return false;
        }
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (!h5tools)
        {
            return false;
        }

        // 打开Parameter子组。
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup))
        {
            return false;
        }

        // 读取偏移模式枚举。
        std::string offsetModeStr;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_OffsetMode, offsetModeStr))
        {
            return false;
        }
        QMetaEnum offsetModeEnum =
            QMetaEnum::fromType<Interface::FITKAbsGeoOffsetFace::OffsetMode>();
        Interface::FITKAbsGeoOffsetFace::OffsetMode mode =
            (Interface::FITKAbsGeoOffsetFace::OffsetMode)offsetModeEnum.keyToValue(offsetModeStr.c_str());
        cmd->setOffsetMode(mode);

        // 读取源面列表。
        int srcCount = 0;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_SrcFaceCount, &srcCount))
        {
            return false;
        }

        QList<Interface::VirtualShape> srcFaces;
        for (int i = 0; i < srcCount; ++i)
        {
            int vsArr[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vsArr, 1, 3))
            {
                return false;
            }
            srcFaces.append(Interface::VirtualShape(vsArr[0], vsArr[1], vsArr[2]));
        }
        cmd->setSourceFaces(srcFaces);

        // 根据偏移模式读取对应参数。
        if (mode == Interface::FITKAbsGeoOffsetFace::DirectDistance)
        {
            double distance = 0.0;
            if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_OffsetDistance, &distance))
            {
                return false;
            }
            cmd->setOffsetDistance(distance);
        }
        else
        {
            int tgtArr[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Target, tgtArr, 1, 3))
            {
                return false;
            }
            cmd->setTargetFace(Interface::VirtualShape(tgtArr[0], tgtArr[1], tgtArr[2]));

            std::string relModeStr;
            if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_RelativeOffsetMode, relModeStr))
            {
                return false;
            }
            QMetaEnum relModeEnum =
                QMetaEnum::fromType<Interface::FITKAbsGeoOffsetFace::RelativeOffsetMode>();
            Interface::FITKAbsGeoOffsetFace::RelativeOffsetMode relMode =
                (Interface::FITKAbsGeoOffsetFace::RelativeOffsetMode)relModeEnum.keyToValue(relModeStr.c_str());
            cmd->setRelativeMode(relMode);

            if (relMode == Interface::FITKAbsGeoOffsetFace::RONearestPointRatio
                || relMode == Interface::FITKAbsGeoOffsetFace::ROFarthestPointRatio)
            {
                double ratio = 0.0;
                if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Ratio, &ratio))
                {
                    return false;
                }
                cmd->setRatio(ratio);
            }
        }

        return true;
    }
    bool FITKAdaptorGeoModelOffsetFace::writeParameters(H5::Group& g)
    {
        Interface::FITKAbsGeoOffsetFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoOffsetFace>();
        if (!cmd || !_writer || !_h5Group)
        {
            return false;
        }
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (!h5tools)
        {
            return false;
        }

        // 创建Parameter子组。
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup))
        {
            return false;
        }

        // 获取参数。
        Interface::FITKAbsGeoOffsetFace::OffsetMode mode = cmd->offsetMode();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoOffsetFace::OffsetMode> modeTransfer;
        QString modeStr = modeTransfer.toString(mode, ok);
        if (!ok)
        {
            return false;
        }

        QList<Interface::VirtualShape> srcFaces = cmd->sourceFaces();
        int srcCount = srcFaces.size();

        // 写出偏移模式与源面列表。
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_OffsetMode, modeStr.toStdString());
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_SrcFaceCount, srcCount);
        for (int i = 0; i < srcCount; ++i)
        {
            int vsArr[3] = { srcFaces[i].CmdId, srcFaces[i].VirtualTopoId, srcFaces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vsArr, 1, 3);
        }

        // 根据偏移模式写出对应参数。
        if (mode == Interface::FITKAbsGeoOffsetFace::DirectDistance)
        {
            double distance = cmd->offsetDistance();
            h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_OffsetDistance, distance);
        }
        else
        {
            Interface::VirtualShape tgtFace = cmd->targetFace();
            int tgtArr[3] = { tgtFace.CmdId, tgtFace.VirtualTopoId, tgtFace.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Target, tgtArr, 1, 3);

            Interface::FITKAbsGeoOffsetFace::RelativeOffsetMode relMode = cmd->relativeMode();
            ok = false;
            Core::FITKEnumTransfer<Interface::FITKAbsGeoOffsetFace::RelativeOffsetMode> relModeTransfer;
            QString relModeStr = relModeTransfer.toString(relMode, ok);
            if (!ok)
            {
                return false;
            }
            h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_RelativeOffsetMode, relModeStr.toStdString());

            if (relMode == Interface::FITKAbsGeoOffsetFace::RONearestPointRatio
                || relMode == Interface::FITKAbsGeoOffsetFace::ROFarthestPointRatio)
            {
                double ratio = cmd->ratio();
                h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Ratio, ratio);
            }
        }

        return true;
    }
    //@}
}



