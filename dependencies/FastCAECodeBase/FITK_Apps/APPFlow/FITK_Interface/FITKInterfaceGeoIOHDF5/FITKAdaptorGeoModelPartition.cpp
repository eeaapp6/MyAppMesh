/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelPartition.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPartitionEdge.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPartitionFace.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelPartitionSolid.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoReferencePlane.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool FITKAdaptorGeoModelPartitionEdge::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdge>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<Interface::VirtualShape> edges = {};
        int count = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int edge[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3)) return false;
            VirtualShape shape{ edge[0], edge[1], edge[2] };
            edges.append(shape);
        }
        cmd->setEdges(edges);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionEdge::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdge>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<Interface::VirtualShape> edges = cmd->edges();
        int count = edges.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int edge[3]{ edges[i].CmdId, edges[i].VirtualTopoId,edges[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3);
        }
        return true;
    }
    bool FITKAdaptorGeoModelPartitionEdgeWithParameter::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionEdgeWithParameter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdgeWithParameter>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> edges = {};
        int count = {};
        double parameter = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), "parameter", &parameter)) return false;
        for (int i = 0; i < count; i++)
        {
            int edge[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3)) return false;
            VirtualShape shape{ edge[0], edge[1], edge[2] };
            edges.append(shape);
        }
        cmd->setParameter(parameter);
        cmd->setEdges(edges);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionEdgeWithParameter::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionEdgeWithParameter* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdgeWithParameter>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> edges = cmd->edges();
        int count = edges.size();
        double parameter = cmd->parameter();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int edge[3]{ edges[i].CmdId, edges[i].VirtualTopoId,edges[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3);
        }
        //写出参数
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), "parameter", parameter);
        return true;
    }
    
    bool FITKAdaptorGeoModelPartitionEdgeWithPoint::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionEdgeWithPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdgeWithPoint>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> edges = {};
        int count = {};
        double parameter = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int edge[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3)) return false;
            VirtualShape shape{ edge[0], edge[1], edge[2] };
            edges.append(shape);
        }
        QVector<double> point;
        double pointData[3] = { 0.0 };
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, pointData, 1, 3))return false;
        point = QVector<double>(pointData, pointData + 3);
        // 设置参数
        cmd->setEdges(edges);
        cmd->setPoint(point);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionEdgeWithPoint::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionEdgeWithPoint* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdgeWithPoint>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> edges = cmd->edges();
        int count = edges.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int edge[3]{ edges[i].CmdId, edges[i].VirtualTopoId,edges[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3);
        }
        QVector<double> point = cmd->point();
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Point, point.data(), 1, 3);

        return true;
    }

    bool FITKAdaptorGeoModelPartitionEdgeWithDatumPlane::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionEdgeWithDatumPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdgeWithDatumPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        FITKAbsGeoDatumPlane* datumPlane = new FITKAbsGeoDatumPlane;
        double position[3] = { };
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Up, up, 1, 3)) return false;

        datumPlane->setPosition(position);
        datumPlane->setPlane(pos, normal, up);
        QList<Interface::VirtualShape> edges = {};
        int count = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int edge[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3)) return false;
            VirtualShape shape{ edge[0], edge[1], edge[2] };
            edges.append(shape);
        }
        cmd->setEdges(edges);
        cmd->setDatumPlane(datumPlane);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionEdgeWithDatumPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionEdgeWithDatumPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionEdgeWithDatumPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> edges = cmd->edges();
        int count = edges.size();
        FITKAbsGeoDatumPlane* datumPlane = cmd->datumPlane();
        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        datumPlane->getPosition(position);
        datumPlane->getPlane(pos, normal, up);
        Interface::FITKGeoEnum::FITKDatumType t = datumPlane->getDatumType();

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

        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int edge[3]{ edges[i].CmdId, edges[i].VirtualTopoId,edges[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionFace::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFace>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<Interface::VirtualShape> faces = {};
        int count = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        cmd->setFaces(faces);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFace::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFace>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionFaceWithSketch::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFaceWithSketch* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithSketch>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = {};
        int sketchId = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "sketchId", &sketchId)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        cmd->setFaces(faces);
        cmd->setSketch(sketchId);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithSketch::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFaceWithSketch* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithSketch>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        int sketchId = cmd->sketch();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "sketchId", sketchId);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithTwoPoints::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFaceWithTwoPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithTwoPoints>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        double startPointData[3] = { 0.0 };
        double endPointData[3] = { 0.0 };
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "startPoint", startPointData, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "endPoint", endPointData, 1, 3))return false;
        QVector<double> startPoint = QVector<double>(startPointData, startPointData + 3);
        QVector<double> endPoint = QVector<double>(endPointData, endPointData + 3);
        cmd->setFaces(faces);
        cmd->setStartPoint(startPoint);
        cmd->setEndPoint(endPoint);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithTwoPoints::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFaceWithTwoPoints* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithTwoPoints>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        QVector<double> startPoint = cmd->startPoint();
        QVector<double> endPoint = cmd->endPoint();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "startPoint", startPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "endPoint", endPoint.data(), 1, 3);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionFaceWithDatumPlane::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFaceWithDatumPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithDatumPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = {};

        FITKAbsGeoDatumPlane* datumPlane = new FITKAbsGeoDatumPlane;
        double position[3] = { };
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};

        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position, position, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Position1, pos, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Normal, normal, 1, 3)) return false;
        if (!h5tools->readGroupAttrDoubleArray(*_h5Group, FGKW_Up, up, 1, 3)) return false;

        datumPlane->setPosition(position);
        datumPlane->setPlane(pos, normal, up);
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        cmd->setFaces(faces);
        cmd->setDatumPlane(datumPlane);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithDatumPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFaceWithDatumPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithDatumPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        FITKAbsGeoDatumPlane* datumPlane = cmd->datumPlane();
        double position[3] = {};
        double pos[3] = {};
        double normal[3] = {};
        double up[3] = {};
        datumPlane->getPosition(position);
        datumPlane->getPlane(pos, normal, up);
        Interface::FITKGeoEnum::FITKDatumType t = datumPlane->getDatumType();

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

        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionFaceWithCurvedPath::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFaceWithCurvedPath* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithCurvedPath>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = {};
        double firstPointData[3] = { 0.0 };
        double secondPointData[3] = { 0.0 };
        int firstEdgeArray[3] = { };
        int secondEdgeArray[3] = {  };
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "firstPoint", firstPointData, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "secondPoint", secondPointData, 1, 3))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "firstEdge", firstEdgeArray, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "secondEdge", secondEdgeArray, 1, 3)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        QVector<double> firstPoint = QVector<double>(firstPointData, firstPointData + 3);
        QVector<double> secondPoint = QVector<double>(secondPointData, secondPointData + 3);
        VirtualShape firstEdge{ firstEdgeArray[0], firstEdgeArray[1], firstEdgeArray[2] };
        VirtualShape secondEdge{ secondEdgeArray[0], secondEdgeArray[1], secondEdgeArray[2] };
        cmd->setFaces(faces);
        cmd->setFirstEdge(firstEdge);
        cmd->setFirstPoint(firstPoint);
        cmd->setSecondEdge(secondEdge);
        cmd->setSecondPoint(secondPoint);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithCurvedPath::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFaceWithCurvedPath* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithCurvedPath>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        QVector<double> firstPoint = cmd->firstPoint();
        QVector<double> secondPoint = cmd->secondPoint();
        VirtualShape firstEdge = cmd->firstEdge();
        VirtualShape secondEdge = cmd->secondEdge();
        int firstEdgeArray[3]{ firstEdge.CmdId, firstEdge.VirtualTopoId,firstEdge.VirtualTopoIndex };
        int secondEdgeArray[3]{ secondEdge.CmdId, secondEdge.VirtualTopoId,secondEdge.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "firstPoint", firstPoint.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "secondPoint", secondPoint.data(), 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "firstEdge", firstEdgeArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "secondEdge", secondEdgeArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionFaceWithExtendFace::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFaceWithExtendFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithExtendFace>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = {};
        int faceArray[3] = {  };
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "ExtendFace", faceArray, 1, 3)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        VirtualShape extendFace{ faceArray[0], faceArray[1], faceArray[2] };
        cmd->setFaces(faces);
        cmd->setExtendFace(extendFace);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithExtendFace::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFaceWithExtendFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithExtendFace>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        VirtualShape faceShape = cmd->face();
        int faceArray[3]{ faceShape.CmdId, faceShape.VirtualTopoId,faceShape.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "ExtendFace", faceArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionFaceWithIntersectFace::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFaceWithIntersectFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithIntersectFace>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = {};
        QList<Interface::VirtualShape> toolFaces = {};
        int toolFacesCount = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "toolFacesCount", &toolFacesCount)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        for (int i = 0; i < toolFacesCount; i++)
        {
            std::string keyWord = std::to_string(i) + std::string(FGKW_Curve);
            int face[3]{  };
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), keyWord.c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            toolFaces.append(shape);
        }
        cmd->setFaces(faces);
        cmd->setToolFaces(toolFaces);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithIntersectFace::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFaceWithIntersectFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithIntersectFace>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        QList<Interface::VirtualShape> toolFaces = cmd->toolFace();
        int toolFacesCount = toolFaces.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "toolFacesCount", toolFacesCount);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        for (int i = 0; i < toolFacesCount; i++)
        {
            std::string keyWord = std::to_string(i) + std::string(FGKW_Curve);
            int face[3]{ toolFaces[i].CmdId, toolFaces[i].VirtualTopoId,toolFaces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), keyWord.c_str(), face, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionFaceWithProjectEdges::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionFaceWithProjectEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithProjectEdges>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = {};
        QList<Interface::VirtualShape> projectEdges = {};
        int projectEdgesCount = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "projectEdgesCount", &projectEdgesCount)) return false;
        for (int i = 0; i < count; i++)
        {
            int face[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3)) return false;
            VirtualShape shape{ face[0], face[1], face[2] };
            faces.append(shape);
        }
        for (int i = 0; i < projectEdgesCount; i++)
        {
            std::string keyWord = std::to_string(i) + std::string(FGKW_Curve);
            int edge[3]{  };
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), keyWord.c_str(), edge, 1, 3)) return false;
            VirtualShape shape{ edge[0], edge[1], edge[2] };
            projectEdges.append(shape);
        }
        cmd->setFaces(faces);
        cmd->setProjectEdges(projectEdges);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionFaceWithProjectEdges::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionFaceWithProjectEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionFaceWithProjectEdges>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        QList<Interface::VirtualShape> projectEdges = cmd->projectEdges();
        int projectEdgesCount = projectEdges.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "projectEdgesCount", projectEdgesCount);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        for (int i = 0; i < projectEdgesCount; i++)
        {
            std::string keyWord = std::to_string(i) + std::string(FGKW_Curve);
            int edge[3]{ projectEdges[i].CmdId, projectEdges[i].VirtualTopoId,projectEdges[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), keyWord.c_str(), edge, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionSolid::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<Interface::VirtualShape> solids = {};
        int count = {};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int solid[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3)) return false;
            VirtualShape shape{ solid[0], solid[1], solid[2] };
            solids.append(shape);
        }
        cmd->setSolids(solids);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<Interface::VirtualShape> solids = cmd->solids();
        int count = solids.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int solid[3]{ solids[i].CmdId, solids[i].VirtualTopoId,solids[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionSolidWithExtendFace::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionSolidWithExtendFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithExtendFace>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> solids = {};
        int count = {};
        int faceArray[3] = {  };
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "ExtendFace", faceArray, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int solid[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3)) return false;
            VirtualShape shape{ solid[0], solid[1], solid[2] };
            solids.append(shape);
        }
        VirtualShape extendFace{ faceArray[0], faceArray[1], faceArray[2] };
        cmd->setSolids(solids);
        cmd->setExtendFace(extendFace);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionSolidWithExtendFace::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionSolidWithExtendFace* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithExtendFace>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> solids = cmd->solids();
        int count = solids.size();
        VirtualShape extendFace = cmd->extendFace();
        int extendFaceArray[3]{ extendFace.CmdId, extendFace.VirtualTopoId,extendFace.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "ExtendFace", extendFaceArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int solid[3]{ solids[i].CmdId, solids[i].VirtualTopoId,solids[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionSolidWithSketchPlanar::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionSolidWithSketchPlanar* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithSketchPlanar>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> solids = {};
        int count = {};
        int sketchArray[3] = {  };
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "sketch", sketchArray, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int solid[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3)) return false;
            VirtualShape shape{ solid[0], solid[1], solid[2] };
            solids.append(shape);
        }
        VirtualShape sketch{ sketchArray[0], sketchArray[1], sketchArray[2] };
        cmd->setSolids(solids);
        cmd->setSketch(sketch);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionSolidWithSketchPlanar::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionSolidWithSketchPlanar* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithSketchPlanar>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> solids = cmd->solids();
        int count = solids.size();
        VirtualShape sketch = cmd->sketch();
        int extendFaceArray[3]{ sketch.CmdId, sketch.VirtualTopoId,sketch.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "sketch", extendFaceArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int solid[3]{ solids[i].CmdId, solids[i].VirtualTopoId,solids[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelPartitionSolidWithSweepEdge::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> solids = {};
        int count = {};
        QList<Interface::VirtualShape> edges = {};
        int edgesCount = {};
        int sweepPathArray[3] = {  };
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "sweepPath", sweepPathArray, 1, 3)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "edgesCount", &edgesCount)) return false;

        for (int i = 0; i < count; i++)
        {
            int solid[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3)) return false;
            VirtualShape shape{ solid[0], solid[1], solid[2] };
            solids.append(shape);
        }
        for (int i = 0; i < edgesCount; i++)
        {
            std::string keyWord = std::to_string(i) + std::string(FGKW_Curve);
            int edge[3]{  };
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), keyWord.c_str(), edge, 1, 3)) return false;
            VirtualShape shape{ edge[0], edge[1], edge[2] };
            edges.append(shape);
        }

        double sweepDirectionData[3] = { 0.0 };
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "sweepDirection", sweepDirectionData, 1, 3))return false;
        QVector<double> sweepDirection = QVector<double>(sweepDirectionData, sweepDirectionData + 3);
        VirtualShape sweepPath{ sweepPathArray[0], sweepPathArray[1], sweepPathArray[2] };
        cmd->setSolids(solids);
        cmd->setEdges(edges);
        cmd->setSweepPath(sweepPath);
        cmd->setSweepDirection(sweepDirection);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionSolidWithSweepEdge::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithSweepEdge>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> solids = cmd->solids();
        int count = solids.size();
        QList<Interface::VirtualShape> edges = cmd->edges();
        int edgesCount = edges.size();
        VirtualShape sweepPath = cmd->sweepPath();
        int sweepPathArray[3]{ sweepPath.CmdId, sweepPath.VirtualTopoId,sweepPath.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "sweepPath", sweepPathArray, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "edgesCount", edgesCount);
        for (int i = 0; i < count; i++)
        {
            int solid[3]{ solids[i].CmdId, solids[i].VirtualTopoId,solids[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3);
        }
        for (int i = 0; i < edgesCount; i++)
        {
            std::string keyWord = std::to_string(i) + std::string(FGKW_Curve);
            int edge[3]{ edges[i].CmdId, edges[i].VirtualTopoId,edges[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), keyWord.c_str(), edge, 1, 3);
        }
        QVector<double> sweepDirection = cmd->sweepDirection();
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "sweepDirection", sweepDirection.data(), 1, 3);
        return true;
    }

    bool FITKAdaptorGeoModelPartitionSolidWithPlane::readParameters()
    {
        Interface::FITKAbsGeoModelPartitionSolidWithPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithPlane>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> solids = {};
        int count = {};
        double planeOriginData[3] = { 0.0 };
        double planeNormalData[3] = { 0.0 };

        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        for (int i = 0; i < count; i++)
        {
            int solid[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3)) return false;
            VirtualShape shape{ solid[0], solid[1], solid[2] };
            solids.append(shape);
        }
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "planeOrigin", planeOriginData, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "planeNormal", planeNormalData, 1, 3))return false;
        QVector<double> planeOrigin = QVector<double>(planeOriginData, planeOriginData + 3);
        QVector<double> planeNormal = QVector<double>(planeNormalData, planeNormalData + 3);
        cmd->setSolids(solids);
        cmd->setPlane(planeOrigin, planeNormal);
        return true;
    }
    bool FITKAdaptorGeoModelPartitionSolidWithPlane::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelPartitionSolidWithPlane* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelPartitionSolidWithPlane>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<Interface::VirtualShape> solids = cmd->solids();
        int count = solids.size();
        QVector<double> planeOrigin = cmd->planeOrigin();
        QVector<double> planeNormal = cmd->planeNormal();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "planeOrigin", planeOrigin.data(), 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "planeNormal", planeNormal.data(), 1, 3);
        for (int i = 0; i < count; i++)
        {
            int solid[3]{ solids[i].CmdId, solids[i].VirtualTopoId,solids[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), solid, 1, 3);
        }
        return true;
    }
}
