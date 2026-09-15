/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModelSolid.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSolidFromShell.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoFeature.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"


namespace Interface
{
    bool  FITKAdaptorGeoModelChamferSolid::readParameters()
    {
        Interface::FITKAbsGeoModelChamferSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelChamferSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int inputID = 0;
        int edgeToposCount = 0;
        double distance = 0;
        QList<Interface::VirtualShape> vShapeList = {};

        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_InputCmdID, &inputID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &edgeToposCount))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Distance, &distance))return false;
        for (int i = 0; i < edgeToposCount; i++)
        {
            int vShapeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3)) return false;
            vShapeList.append(Interface::VirtualShape(vShapeArray[0], vShapeArray[1], vShapeArray[2]));
        }
        cmd->setEdgeTopos(vShapeList);
        cmd->setDistance(distance);
        cmd->setInputCmdId(inputID);
        return true;
    }
    bool FITKAdaptorGeoModelChamferSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelChamferSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelChamferSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;
        
        int inputID = cmd->getInputCmdId();
        QList<VirtualShape> edgeTopos = cmd->getEdgeTopos();
        int edgeToposCount = edgeTopos.size();
        double distance = cmd->getDistance();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_InputCmdID, inputID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, edgeToposCount);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Distance, distance);
        for (int i = 0; i < edgeToposCount; i++)
        {
            VirtualShape vShape = edgeTopos[i];
            int vShapeArray[3]{ vShape.CmdId, vShape.VirtualTopoId, vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelRemoveChamferSolid::readParameters()
    {
        Interface::FITKAbsGeoModelRemoveChamferSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRemoveChamferSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int inputID = 0;
        int faceToposCount = 0;
        QList<Interface::VirtualShape> vShapeList = {};

        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_InputCmdID, &inputID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &faceToposCount))return false;
        for (int i = 0; i < faceToposCount; i++)
        {
            int vShapeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3)) return false;
            vShapeList.append(Interface::VirtualShape(vShapeArray[0], vShapeArray[1], vShapeArray[2]));
        }
        cmd->setFaceTopos(vShapeList);
        cmd->setInputCmdId(inputID);
        return true;
    }
    bool FITKAdaptorGeoModelRemoveChamferSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelRemoveChamferSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRemoveChamferSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        int inputID = cmd->getInputCmdId();
        QList<VirtualShape> faceTopos = cmd->getFaceTopos();
        int faceToposCount = faceTopos.size();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_InputCmdID, inputID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, faceToposCount);
        for (int i = 0; i < faceToposCount; i++)
        {
            VirtualShape vShape = faceTopos[i];
            int vShapeArray[3]{ vShape.CmdId, vShape.VirtualTopoId, vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelFilletSolid::readParameters()
    {
        Interface::FITKAbsGeoModelFilletSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilletSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int inputID = 0;
        int edgeToposCount = 0;
        double radius = 0;
        QList<Interface::VirtualShape> vShapeList = {};

        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_InputCmdID, &inputID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &edgeToposCount))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Radius, &radius))return false;
        for (int i = 0; i < edgeToposCount; i++)
        {
            int vShapeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3)) return false;
            vShapeList.append(Interface::VirtualShape(vShapeArray[0], vShapeArray[1], vShapeArray[2]));
        }
        cmd->setEdgeTopos(vShapeList);
        cmd->setRadius(radius);
        cmd->setInputCmdId(inputID);
        return true;
    }
    bool FITKAdaptorGeoModelFilletSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelFilletSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFilletSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        int inputID = cmd->getInputCmdId();
        QList<VirtualShape> edgeTopos = cmd->getEdgeTopos();
        int edgeToposCount = edgeTopos.size();
        double radius = cmd->getRadius();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_InputCmdID, inputID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, edgeToposCount);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Radius, radius);
        for (int i = 0; i < edgeToposCount; i++)
        {
            VirtualShape vShape = edgeTopos[i];
            int vShapeArray[3]{ vShape.CmdId, vShape.VirtualTopoId, vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelRemoveFilletSolid::readParameters()
    {
        Interface::FITKAbsGeoModelRemoveFilletSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRemoveFilletSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int inputID = 0;
        int faceToposCount = 0;
        QList<Interface::VirtualShape> vShapeList = {};

        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_InputCmdID, &inputID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &faceToposCount))return false;
        for (int i = 0; i < faceToposCount; i++)
        {
            int vShapeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3)) return false;
            vShapeList.append(Interface::VirtualShape(vShapeArray[0], vShapeArray[1], vShapeArray[2]));
        }
        cmd->setFaceTopos(vShapeList);
        cmd->setInputCmdId(inputID);
        return true;
    }
    bool FITKAdaptorGeoModelRemoveFilletSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelRemoveFilletSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRemoveFilletSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        int inputID = cmd->getInputCmdId();
        QList<VirtualShape> faceTopos = cmd->getFaceTopos();
        int faceToposCount = faceTopos.size();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_InputCmdID, inputID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, faceToposCount);
        for (int i = 0; i < faceToposCount; i++)
        {
            VirtualShape vShape = faceTopos[i];
            int vShapeArray[3]{ vShape.CmdId, vShape.VirtualTopoId, vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelDefeatureSolid::readParameters()
    {
        Interface::FITKAbsGeoModelDefeatureSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelDefeatureSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int inputID = 0;
        int faceToposCount = 0;
        QList<Interface::VirtualShape> vShapeList = {};

        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_InputCmdID, &inputID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &faceToposCount))return false;
        for (int i = 0; i < faceToposCount; i++)
        {
            int vShapeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3)) return false;
            vShapeList.append(Interface::VirtualShape(vShapeArray[0], vShapeArray[1], vShapeArray[2]));
        }
        cmd->setFaceTopos(vShapeList);
        cmd->setInputCmdId(inputID);
        return true;
    }
    bool FITKAdaptorGeoModelDefeatureSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelDefeatureSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelDefeatureSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        int inputID = cmd->getInputCmdId();
        QList<VirtualShape> faceTopos = cmd->getFaceTopos();
        int faceToposCount = faceTopos.size();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_InputCmdID, inputID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, faceToposCount);
        for (int i = 0; i < faceToposCount; i++)
        {
            VirtualShape vShape = faceTopos[i];
            int vShapeArray[3]{ vShape.CmdId, vShape.VirtualTopoId, vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelFillHoleSolid::readParameters()
    {
        Interface::FITKAbsGeoModelFillHoleSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFillHoleSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int inputID = 0;
        int faceToposCount = 0;
        QList<Interface::VirtualShape> vShapeList = {};

        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_InputCmdID, &inputID)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &faceToposCount))return false;
        for (int i = 0; i < faceToposCount; i++)
        {
            int vShapeArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3)) return false;
            vShapeList.append(Interface::VirtualShape(vShapeArray[0], vShapeArray[1], vShapeArray[2]));
        }
        cmd->setFaceTopos(vShapeList);
        cmd->setInputCmdId(inputID);
        return true;
    }
    bool FITKAdaptorGeoModelFillHoleSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelFillHoleSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFillHoleSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        int inputID = cmd->getInputCmdId();
        QList<VirtualShape> faceTopos = cmd->getFaceTopos();
        int faceToposCount = faceTopos.size();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_InputCmdID, inputID);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, faceToposCount);
        for (int i = 0; i < faceToposCount; i++)
        {
            VirtualShape vShape = faceTopos[i];
            int vShapeArray[3]{ vShape.CmdId, vShape.VirtualTopoId, vShape.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShapeArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelClosedSurfaceSolid::readParameters()
    {
        Interface::FITKAbsGeoModelClosedSurfaceSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelClosedSurfaceSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int count = 0;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))return false;
        for (int i = 0; i < count; i++)
        {
            int faceArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), faceArray, 1, 3)) return false;
            cmd->addFace(Interface::VirtualShape(faceArray[0], faceArray[1], faceArray[2]));
        }
        return true;
    }
    bool FITKAdaptorGeoModelClosedSurfaceSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelClosedSurfaceSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelClosedSurfaceSolid>();
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
            VirtualShape face = faces[i];
            int faceArray[3]{ face.CmdId, face.VirtualTopoId, face.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), faceArray, 1, 3);
        }
        return true;
    }

    bool FITKAdaptorGeoModelSolidFromShell::readParameters()
    {
        Interface::FITKAbsGeoModelSolidFromShell* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSolidFromShell>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr)
        {
            return false;
        }

        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr)
        {
            return false;
        }

        // 打开参数组。
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup))
        {
            return false;
        }

        // 读取面虚拓扑列表。
        int count = 0;
        QList<Interface::VirtualShape> faces = {};
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count))
        {
            return false;
        }
        for (int i = 0; i < count; i++)
        {
            int faceArray[3] = {};
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), faceArray, 1, 3))
            {
                return false;
            }
            faces.append(Interface::VirtualShape(faceArray[0], faceArray[1], faceArray[2]));
        }

        cmd->setFaces(faces);
        return true;
    }

    bool FITKAdaptorGeoModelSolidFromShell::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSolidFromShell* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSolidFromShell>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr)
        {
            return false;
        }

        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr)
        {
            return false;
        }

        // 创建参数组。
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup))
        {
            return false;
        }

        // 写出面虚拓扑列表。
        QList<Interface::VirtualShape> faces = cmd->faces();
        int count = faces.size();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            Interface::VirtualShape face = faces[i];
            int faceArray[3]{ face.CmdId, face.VirtualTopoId, face.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), faceArray, 1, 3);
        }

        return true;
    }

    bool  FITKAdaptorGeoModelExtrudeSolid::readParameters()
    {
        Interface::FITKAbsGeoModelExtrudeSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtrudeSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int type = 0;
        int sourceSurfaceArray[3] = {};
        int upToFaceArray[3] = {};
        double directionArray[3] = {};
        double length = 0;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "Type", &type))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3))return false;
        if (type == 1)
        {
            if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, directionArray, 1, 3))return false;
            if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Length, &length))return false;
            cmd->setDirection(directionArray[0], directionArray[1], directionArray[2]);
            cmd->setLength(length);
        }
        else if (type == 2)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "UpToFace", upToFaceArray, 1, 3))return false;
            Interface::VirtualShape upToFace(upToFaceArray[0], upToFaceArray[1], upToFaceArray[2]);
            cmd->setUpToFace(upToFace);
        }
        Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
        cmd->setSourceSurface(vShape);
        cmd->setExtrudeType(type);
        return true;
    }
    bool FITKAdaptorGeoModelExtrudeSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelExtrudeSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtrudeSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        int type = cmd->extrudeType();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "Type", type);

        VirtualShape sourceSurface = cmd->sourceSurface();
        int sourceSurfaceArray[3]{ sourceSurface.CmdId,sourceSurface.VirtualTopoId, sourceSurface.VirtualTopoIndex };
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3);
        if (type == 1)
        {
            std::array<double, 3> direction = cmd->direction();
            double directionArray[3]{ direction[0],direction[1],direction[2] };
            double length = cmd->length();
            //写出参数
            h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_Direction, directionArray, 1, 3);
            h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Length, length);
        }
        else if (type == 2)
        {
            VirtualShape upToFace = cmd->upToFace();
            int upToFaceArray[3]{ upToFace.CmdId,upToFace.VirtualTopoId, upToFace.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "UpToFace", upToFaceArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelRevolSolid::readParameters()
    {
        Interface::FITKAbsGeoModelRevolSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRevolSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int sourceSurfaceArray[3] = {};
        double rotateAxisPoint1[3] = {};
        double rotateAxisPoint2[3] = {};
        int angle = 0;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint1, rotateAxisPoint1, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint2, rotateAxisPoint2, 1, 3))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "angle", &angle))return false;

        Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
        cmd->setSourceSurface(vShape);
        cmd->setRotateAxisPoint1(rotateAxisPoint1[0], rotateAxisPoint1[1], rotateAxisPoint1[2]);
        cmd->setRotateAxisPoint2(rotateAxisPoint2[0], rotateAxisPoint2[1], rotateAxisPoint2[2]);
        cmd->setAngle(angle);
        return true;
    }
    bool FITKAdaptorGeoModelRevolSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelRevolSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRevolSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        VirtualShape sourceSurface = cmd->sourceSurface();
        int sourceSurfaceArray[3]{ sourceSurface.CmdId,sourceSurface.VirtualTopoId, sourceSurface.VirtualTopoIndex };
        std::array<double, 3>  rotateAxisPoint1 = cmd->rotateAxisPoint1();
        double rotateAxisPoint1Array[3]{ rotateAxisPoint1[0], rotateAxisPoint1[1], rotateAxisPoint1[2] };
        std::array<double, 3>  rotateAxisPoint2 = cmd->rotateAxisPoint2();
        double rotateAxisPoint2Array[3]{ rotateAxisPoint2[0], rotateAxisPoint2[1], rotateAxisPoint2[2] };
        int angle = cmd->angle();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint1, rotateAxisPoint1Array, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint2, rotateAxisPoint2Array, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "angle", angle);

        return true;
    }

    bool  FITKAdaptorGeoModelSweepSolid::readParameters()
    {
        Interface::FITKAbsGeoModelSweepSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSweepSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int profileArray[3] = {};
        int curveArray[3] = {};
        bool flipPath = false;
        //读取参数
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), "flipPath", &flipPath))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Profile, profileArray, 1, 3))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Curve, curveArray, 1, 3))return false;

        Interface::VirtualShape profile(profileArray[0], profileArray[1], profileArray[2]);
        Interface::VirtualShape curve(curveArray[0], curveArray[1], curveArray[2]);
        cmd->setProfile(profile);
        cmd->setCurve(curve);
        cmd->setFlipPath(flipPath);
        return true;
    }
    bool FITKAdaptorGeoModelSweepSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSweepSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSweepSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        VirtualShape profile = cmd->profile();
        int profileArray[3]{ profile.CmdId,profile.VirtualTopoId, profile.VirtualTopoIndex };
        VirtualShape curve = cmd->curve();
        int curveArray[3]{ curve.CmdId,curve.VirtualTopoId, curve.VirtualTopoIndex };
        bool flipPath = cmd->flipPath();
        //写出参数
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), "flipPath", flipPath);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Profile, profileArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Curve, curveArray, 1, 3);

        return true;
    }

    bool  FITKAdaptorGeoModelMultiSectionSolid::readParameters()
    {
        Interface::FITKAbsGeoModelMultiSectionSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMultiSectionSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int count = 0;
        int sectionArray[3] = {};
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count,&count))return false;
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sectionArray, 1, 3)) return false;
            Interface::VirtualShape section(sectionArray[0], sectionArray[1], sectionArray[2]);
            cmd->addSection(section);
        }
        return true;
    }
    bool FITKAdaptorGeoModelMultiSectionSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelMultiSectionSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelMultiSectionSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<VirtualShape> sections = cmd->sections();
        int count = sections.size();
        //写出参数
        for (int i = 0; i < count; i++)
        {
            VirtualShape section = sections[i];
            int sectionArray[3]{ section.CmdId,section.VirtualTopoId, section.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sectionArray, 1, 3);
        }
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        return true;
    }

    bool  FITKAdaptorGeoModelStiffener::readParameters()
    {
        Interface::FITKAbsGeoStiffener* cmd = this->getDataObjectAs<Interface::FITKAbsGeoStiffener>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int source[3] = {};
        int profile[3] = {};
        double thickness1 = 0;
        double thickness2 = 0;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, source,1,3))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ProfileShape, source,1,3))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Thickness1, &thickness1))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Thickness2, &thickness2))return false;

        Interface::VirtualShape sourceShape(source[0], source[1], source[2]);
        Interface::VirtualShape profileShape(profile[0], profile[1], profile[2]);
        cmd->setSourceShape(sourceShape);
        cmd->setProfileShape(profileShape);
        cmd->setThickness1(thickness1);
        cmd->setThickness2(thickness2);
        return true;
    }
    bool FITKAdaptorGeoModelStiffener::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoStiffener* cmd = this->getDataObjectAs<Interface::FITKAbsGeoStiffener>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        VirtualShape sourceShape = cmd->sourceShape();
        int source[3]{ sourceShape.CmdId, sourceShape.VirtualTopoId, sourceShape.VirtualTopoIndex };
        VirtualShape profileShape = cmd->profileShape();
        int profile[3]{ profileShape.CmdId, profileShape.VirtualTopoId, profileShape.VirtualTopoIndex };
        double thickness1 = cmd->thickness1();
        double thickness2 = cmd->thickness2();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceShape, source,1,3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_ProfileShape, profile,1,3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Thickness1, thickness1);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Thickness2, thickness2);

        return true;
    }

    bool  FITKAdaptorGeoModelCircularHole::readParameters()
    {
        Interface::FITKAbsGeoModelCircularHoleSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircularHoleSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        double holePoint[3] = { };
        bool flip = {};
        double diameter = {};
        int faceArray[3]{  };
        int edge01Array[3]{ };
        double distanceLocateHole01 = {};
        int edge02Array[3]{  };
        double distanceLocateHole02 = {};

        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), "holePoint", holePoint, 1, 3)) return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), "flip", &flip)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), "diameter", &diameter)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "geoOperFace", faceArray, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "geoEdge01", edge01Array, 1, 3)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), "geoEdge02", edge02Array, 1, 3)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), "distanceLocateHole01", &distanceLocateHole01)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), "distanceLocateHole02", &distanceLocateHole02)) return false;

        VirtualShape geoOperFace{ faceArray[0],faceArray[1],faceArray[2] };
        VirtualShape geoEdge01{ edge01Array[0],edge01Array[1],edge01Array[2] };
        VirtualShape geoEdge02{ edge02Array[0],edge02Array[1],edge02Array[2] };

        cmd->setHolePoint(holePoint);
        cmd->setFlip(flip);
        cmd->setDiameter(diameter);
        cmd->setOperFace(geoOperFace);
        cmd->setOperEdge1(geoEdge01);
        cmd->setDistanceLocateHole1(distanceLocateHole01);
        cmd->setOperEdge2(geoEdge02);
        cmd->setDistanceLocateHole2(distanceLocateHole02);
        return true;
    }
    bool FITKAdaptorGeoModelCircularHole::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelCircularHoleSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelCircularHoleSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        double holePointX = {};
        double holePointY = {};
        double holePointZ = {};
        cmd->getHolePoint(holePointX, holePointY, holePointZ);
        double holePoint[3] = { holePointX, holePointY, holePointZ };
        bool flip = cmd->getFlip();
        double diameter = cmd->getDiameter();
        VirtualShape geoOperFace = cmd->getOperFace();
        int faceArray[3]{ geoOperFace.CmdId,geoOperFace.VirtualTopoId,geoOperFace.VirtualTopoIndex };
        VirtualShape geoEdge01 = cmd->getOperEdge1();
        int edge01Array[3]{ geoEdge01.CmdId,geoEdge01.VirtualTopoId,geoEdge01.VirtualTopoIndex };
        double distanceLocateHole01 = cmd->getDistanceLocateHole1();
        VirtualShape geoEdge02 = cmd->getOperEdge2();
        int edge02Array[3]{ geoEdge02.CmdId,geoEdge02.VirtualTopoId,geoEdge02.VirtualTopoIndex };
        double distanceLocateHole02 = cmd->getDistanceLocateHole2();

        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), "holePoint", holePoint, 1, 3);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), "flip", flip);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), "diameter", diameter);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "geoOperFace", faceArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "geoEdge01", edge01Array, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), "geoEdge02", edge02Array, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), "distanceLocateHole01", distanceLocateHole01);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), "distanceLocateHole02", distanceLocateHole02);
        return true;
    }
}



