/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDelete.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoMultiDelete.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperImprintSolid.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImportFlattenAssembly.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExport.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperCompound.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelFromMesh.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoRepairSmallEdges.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoRepairSmallFaces.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCut.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceGeoIOHDF5/FITKAdaptorGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool  FITKAdaptorGeoModelDelete::readParameters()
    {
        Interface::FITKAbsGeoDelete* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDelete>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int deleteCommandID = 0;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_DeleteCommandID, &deleteCommandID)) return false;
        cmd->setDeleteCommandID(deleteCommandID);
        return true;
    }
    bool FITKAdaptorGeoModelDelete::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoDelete* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDelete>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool,H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if(!std::get<0>(paramGroup)) return false;

        int deleteCommandID = cmd->getDeleteCommandID();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_DeleteCommandID, deleteCommandID);
        return true;
    }

    bool  FITKAdaptorGeoModelMultiDelete::readParameters()
    {
        Interface::FITKAbsGeoMultiDelete* cmd = this->getDataObjectAs<Interface::FITKAbsGeoMultiDelete>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int deleteCmdIdCount = 0;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &deleteCmdIdCount)) return false;
        int* deleteCommandIDs = new int[deleteCmdIdCount];
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_DeleteCommandIDs, deleteCommandIDs, 1, deleteCmdIdCount))
        {
            delete deleteCommandIDs;
            return false;
        }
        QList<int> ids = {};
        for (int i = 0; i < deleteCmdIdCount; i++)
        {
            ids.append(deleteCommandIDs[i]);
        }
        cmd->setDeleteCommandIDs(ids);
        delete deleteCommandIDs;
        return true;
    }
    bool FITKAdaptorGeoModelMultiDelete::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoMultiDelete* cmd = this->getDataObjectAs<Interface::FITKAbsGeoMultiDelete>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QList<int> ids = cmd->getDeleteCommandIDs();
        int deleteCmdIdCount = ids.count();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, deleteCmdIdCount);
        int* deleteCommandIDs = new int[deleteCmdIdCount];
        for (int i = 0; i < deleteCmdIdCount; i++)
        {
            deleteCommandIDs[i] = ids[i];
        }
        if (!h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_DeleteCommandIDs, deleteCommandIDs, 1, deleteCmdIdCount))
        {
            delete deleteCommandIDs;
            return false;
        }
        delete deleteCommandIDs;
        return true;
    }

    bool  FITKAdaptorGeoImprintSolid::readParameters()
    {
        Interface::FITKAbsGeoModelOperImprintSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperImprintSolid>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        std::string type = {};
        //int type = 0;
        double tolerance = 0;
        int count = 0;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_Type,type)) return false;
        //if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Type,&type)) return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Tolerance, &tolerance)) return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;

        int sourceSurfaceArray[3] = {};
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), sourceSurfaceArray, 1, 3)) return false;
            cmd->add(Interface::VirtualShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]));
        }
        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKAbsGeoModelOperImprintSolid::GeoSolidOperType>();
        Interface::FITKAbsGeoModelOperImprintSolid::GeoSolidOperType cmdType = (Interface::FITKAbsGeoModelOperImprintSolid::GeoSolidOperType)metaEnum.keyToValue(type.c_str());
       // cmd->setType(Interface::FITKAbsGeoModelOperImprintSolid::GeoSolidOperType(type));
        cmd->setType(Interface::FITKAbsGeoModelOperImprintSolid::GeoSolidOperType(cmdType));
        cmd->setTolerance(tolerance);
        return true;
    }
    bool FITKAdaptorGeoImprintSolid::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOperImprintSolid* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperImprintSolid>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        Interface::FITKAbsGeoModelOperImprintSolid::GeoSolidOperType type = cmd->getType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKAbsGeoModelOperImprintSolid::GeoSolidOperType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(type, ok);
        if (!ok) return false;
        //int type = cmd->getType();
        double tolerance = cmd->getTolerance();
        QList<VirtualShape> shapes = cmd->getVShapes();
        int count = shapes.size();

        //写出参数
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_Type, cmdTypeStr.toStdString());
        //h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Type, type);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Tolerance, tolerance);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int vShape[3]{ shapes[i].CmdId, shapes[i].VirtualTopoId,shapes[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), vShape, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelImport::readParameters()
    {
        Interface::FITKAbsGeoModelImport* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelImport>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        std::string fileName = {};
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_FileName, fileName)) return false;
        cmd->setFileName(QString::fromStdString(fileName));
        return true;
    }
    bool FITKAdaptorGeoModelImport::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelImport* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelImport>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QString fileName = cmd->getFileName();
        //写出参数
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_FileName, fileName.toStdString());
        return true;
    }

    bool  FITKAdaptorGeoModelExport::readParameters()
    {
        Interface::FITKAbsGeoModelExport* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExport>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        std::string fileName = {};
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_FileName, fileName)) return false;
        cmd->setFileName(QString::fromStdString(fileName));
        return true;
    }
    bool FITKAdaptorGeoModelExport::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelExport* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExport>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QString fileName = cmd->getFileName();
        //写出参数
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_FileName, fileName.toStdString());
        return true;
    }

    bool  FITKAdaptorGeoModelCompound::readParameters()
    {
        Interface::FITKAbsGeoModelOperCompound* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperCompound>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        int inputCmdArray[3] = {};
        int childCmdArray[3] = {};
        int count = 0;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), FGKW_Count, &count)) return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_InputCmd, inputCmdArray,1,3)) return false;
        Interface::VirtualShape vShape(inputCmdArray[0], inputCmdArray[1], inputCmdArray[2]);
        QList<Interface::VirtualShape> vShapeChildList = {};
        for (int i = 0; i < count; i++)
        {
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), childCmdArray, 1, 3)) return false;

            Interface::VirtualShape vShapeChild(childCmdArray[0], childCmdArray[1], childCmdArray[2]);
            vShapeChildList.append(vShapeChild);
        }
        cmd->setInputCmdId(vShape);
        cmd->setShapeCmdIds(vShapeChildList);
        return true;
    }
    bool FITKAdaptorGeoModelCompound::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelOperCompound* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelOperCompound>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        Interface::VirtualShape vShape = cmd->getInputCmdId();
        int inputCmdArray[3]{ vShape.CmdId,vShape.VirtualTopoId, vShape.VirtualTopoIndex };
        QList<Interface::VirtualShape> cmdIDs = cmd->getShapeCmdIds();
        int count = cmdIDs.size();
        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_InputCmd, inputCmdArray,1,3);
        for (int i = 0; i < count; i++)
        {
            VirtualShape cmdID = cmdIDs[i];
            int cmdIDArray[3]{ cmdID.CmdId, cmdID.VirtualTopoId,cmdID.VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str() , cmdIDArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelImportFlattenAssembly::readParameters()
    {
        Interface::FITKAbsGeoModelImportFlattenAssembly* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelImportFlattenAssembly>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        std::string fileName = {};
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), FGKW_FileName, fileName)) return false;
        cmd->setFileName(QString::fromStdString(fileName));
        return true;
    }
    bool FITKAdaptorGeoModelImportFlattenAssembly::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelImportFlattenAssembly* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelImportFlattenAssembly>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        QString fileName = cmd->getFileName();
        //写出参数
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), FGKW_FileName, fileName.toStdString());
        return true;
    }

    bool  FITKAdaptorGeoPart::readParameters()
    {
        Interface::FITKAbsGeoPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoPart>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        bool autoMergeSolid = false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), "autoMergeSolid", &autoMergeSolid)) return false;
        cmd->setAutoMergeSolid(autoMergeSolid);
        if (!std::get<1>(paramGroup).nameExists("CommandManager"))return true;
        int count = 0;
        std::tuple<bool, H5::Group> childsGroup = h5tools->openGp(std::get<1>(paramGroup), "CommandManager");
        if (!h5tools->readGroupint(std::get<1>(childsGroup), FGKW_Count, &count)) return true;
        for (int i = 0; i < count; i++)
        {
            //根据索引映射读取几何名称
            H5::Attribute geoNameAttribute = std::get<1>(childsGroup).openAttribute(std::to_string(i));
            std::string geoName = {};
            H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
            geoNameAttribute.read(datatype, geoName);

            std::tuple<bool, H5::Group> geoGroup = h5tools->openGp(std::get<1>(childsGroup), geoName.c_str());

            std::string geoType = {};
            if (!h5tools->readGroupStr(std::get<1>(geoGroup), FGKW_CommandType, geoType)) return false;
            if (geoType.empty())return false;
            //实例几何命令类型
            Core::FITKAbstractNDataObject* geo = getCommand(QString::fromStdString(geoType));
            if (!geo) return false;
            Interface::FITKAbsGeoCommand* comm = dynamic_cast<Interface::FITKAbsGeoCommand*>(geo);
            if (!comm) return false;

            // 不能使用子类逻辑处理子命令。
            cmd->Interface::FITKAbsGeoPart::appendDataObj(comm);
            Interface::FITKAdaptorGeoCommand* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKAdaptorGeoCommand>("HDF5", comm);
            if (adaptor == nullptr)
            {
                delete comm;
                return false;
            }
            adaptor->setH5Group(&std::get<1>(geoGroup));
            adaptor->setDataObject(comm);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            delete adaptor;
            if (!r) return false;
        }
        return true;
    }

    Core::FITKAbstractNDataObject* FITKAdaptorGeoPart::getCommand(QString enumName)
    {
        if (enumName.isEmpty()) return nullptr;
        auto metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKGeometryComType>();
        int valueID = metaEnum.keyToValue(enumName.toLocal8Bit());
        if (valueID < 0) return nullptr;

        Interface::FITKInterfaceGeometryFactory* fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!fac) return nullptr;

        return fac->createCommand((Interface::FITKGeoEnum::FITKGeometryComType)valueID);
    }

    bool FITKAdaptorGeoPart::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoPart* cmd = this->getDataObjectAs<Interface::FITKAbsGeoPart>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;

        bool autoMergeSolid = cmd->getAutoMergeSolid();
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), "autoMergeSolid", autoMergeSolid);

        int count = cmd->getDataCount();
        if (count == 0) return true;
        std::tuple<bool, H5::Group> commandManagerGroup = h5tools->createGp(std::get<1>(paramGroup), "CommandManager");

        for (int i = 0; i < count; i++)
        {
            FITKAbsGeoCommand* comm = cmd->getDataByIndex(i);
            if (!comm) continue;
            // 获取适配器
            Interface::FITKAdaptorGeoCommand* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKAdaptorGeoCommand>("HDF5", comm);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&std::get<1>(commandManagerGroup));
            adaptor->setDataObject(comm);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            delete adaptor;
            if (!w) return false;
        }
        h5tools->writeGroupAttrInt(std::get<1>(commandManagerGroup), FGKW_Count, count);
        return true;
    }

    bool  FITKAdaptorGeoModelFromMesh::readParameters()
    {
        Interface::FITKAbsGeoModelFromMesh* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFromMesh>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false; 
        H5::Group paramGp = std::get<1>(paramGroup);
        // 读取节点数量
        int nodeCount = 0;
        if (!h5tools->readGroupint(paramGp, FGKW_NodeCount, &nodeCount)) return false;
        // 读取三角形数量
        int triCount = 0;
        if (!h5tools->readGroupint(paramGp, FGKW_TriangleCount, &triCount)) return false;
       
        QList<Core::FITKNode*> meshNodes = {};
        QList<FITKAbstractElement*> meshElements = {};

        std::function<void()> cleanUp = [&]() {
            qDeleteAll(meshNodes);
            qDeleteAll(meshElements);
        };
        // 读取节点数据
        if (nodeCount > 0) 
        {
            std::vector<int> nodeIDs = {};
            std::vector<double> coordArray = {};
            // 读取节点ID和坐标
            if (!h5tools->ReadOneDArrayDataSetInt(paramGp, FGKW_NodeIDs, nodeIDs) ||
                !h5tools->ReadOneDArrayDataSetDouble(paramGp, FGKW_NodeCoordinates, coordArray))
            {
                cleanUp();
                return false;
            }
            // 创建节点
            for (int i = 0; i < nodeCount; ++i) 
            {
                Core::FITKNode* newNode = new Core::FITKNode(nodeIDs[i], coordArray[i * 3], coordArray[i * 3 + 1], coordArray[i * 3 + 2]);
                meshNodes.append(newNode);
            }
        }
        // 读取三角形数据
        if (triCount > 0) 
        {
            std::vector<int> triNodes = {};
            // 读取三角形节点
            if (!h5tools->ReadOneDArrayDataSetInt(paramGp, FGKW_TriangleNodes, triNodes))
            {
                cleanUp();
                return false;
            }
            for (int i = 0; i < triCount; ++i) 
            {
                Interface::FITKAbstractElement* newElement = Interface::FITKElementFactory::createElement(FITKModelEnum::FITKEleType::Tri3);
                if (!newElement) 
                {
                    cleanUp();
                    return false;
                }
                newElement->setNodeID(0, triNodes[i * 3]);
                newElement->setNodeID(1, triNodes[i * 3 + 1]);
                newElement->setNodeID(2, triNodes[i * 3 + 2]);
                meshElements.append(newElement);
            }
        }
        cmd->setMesh(meshNodes, meshElements);
        cleanUp();
        return true;
    }

    bool FITKAdaptorGeoModelFromMesh::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelFromMesh* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelFromMesh>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        H5::Group paramGp = std::get<1>(paramGroup);

        QHash<int, FITKAbsGeoModelFromMesh::MeshNodeCoordinates> nodeCoords = cmd->getMeshNodeCoordinates();
        QList<FITKAbsGeoModelFromMesh::MeshTri> triangles = cmd->getMeshTriangles();
        const int nodeCount = nodeCoords.size();
        const int triCount = triangles.size();
        // 写入节点和三角形数量
        if (!h5tools->writeGroupAttrInt(paramGp, FGKW_NodeCount, nodeCount) ||
            !h5tools->writeGroupAttrInt(paramGp, FGKW_TriangleCount, triCount)) return false;
        // 写入节点数据
        if (nodeCount > 0) 
        {
            std::vector<int> nodeIDs = {};
            std::vector<double> coordArray = {};
            nodeIDs.reserve(nodeCount);
            coordArray.reserve(nodeCount * 3);
            for (auto it = nodeCoords.begin(); it != nodeCoords.end(); ++it) 
            {
                nodeIDs.push_back(it.key());
                coordArray.push_back(it.value().x);
                coordArray.push_back(it.value().y);
                coordArray.push_back(it.value().z);
            }
            // 写入节点ID和坐标
            if (!h5tools->CreateOneDarrayDataSetInt(paramGp, FGKW_NodeIDs, nodeIDs) ||
                !h5tools->CreateOneDarrayDataSetDouble(paramGp, FGKW_NodeCoordinates, coordArray)) return false;
        }
        // 写入三角形数据
        if (triCount > 0) 
        {
            std::vector<int> triNodes = {};
            std::vector<double> normals = {};
            triNodes.reserve(triCount * 3);
            normals.reserve(triCount * 3);
            for (const auto& tri : triangles) 
            {
                triNodes.push_back(tri.node1);
                triNodes.push_back(tri.node2);
                triNodes.push_back(tri.node3);
                normals.push_back(tri.normal[0]);
                normals.push_back(tri.normal[1]);
                normals.push_back(tri.normal[2]);
            }
            // 写入三角形节点和法向量
            if (!h5tools->CreateOneDarrayDataSetInt(paramGp, FGKW_TriangleNodes, triNodes) ||
                !h5tools->CreateOneDarrayDataSetDouble(paramGp, FGKW_TriangleNormals, normals)) return false;
        }
        return true;
    }

    bool  FITKAdaptorGeoModelModelRepairSmallEdges::readParameters()
    {
        Interface::FITKAbsGeoRepairSmallEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoRepairSmallEdges>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> edges = {};
        int count = 0;
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
    bool FITKAdaptorGeoModelModelRepairSmallEdges::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoRepairSmallEdges* cmd = this->getDataObjectAs<Interface::FITKAbsGeoRepairSmallEdges>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<VirtualShape> edges = cmd->edges();
        int count = edges.size();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int edge[3]{ edges[i].CmdId, edges[i].VirtualTopoId,edges[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), edge, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelModelRepairSmallFaces::readParameters()
    {
        Interface::FITKAbsGeoRepairSmallFaces* cmd = this->getDataObjectAs<Interface::FITKAbsGeoRepairSmallFaces>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        QList<Interface::VirtualShape> faces = {};
        int count = 0;
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
    bool FITKAdaptorGeoModelModelRepairSmallFaces::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoRepairSmallFaces* cmd = this->getDataObjectAs<Interface::FITKAbsGeoRepairSmallFaces>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        QList<VirtualShape> faces = cmd->faces();
        int count = faces.size();
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), FGKW_Count, count);
        for (int i = 0; i < count; i++)
        {
            int face[3]{ faces[i].CmdId, faces[i].VirtualTopoId,faces[i].VirtualTopoIndex };
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), std::to_string(i).c_str(), face, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelModelExtrudeCut::readParameters()
    {
        Interface::FITKAbsGeoModelExtrudeCut* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtrudeCut>();
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
    bool FITKAdaptorGeoModelModelExtrudeCut::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelExtrudeCut* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExtrudeCut>();
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
    bool  FITKAdaptorGeoModelModelRevolCut::readParameters()
    {
        Interface::FITKAbsGeoModelRevolCut* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRevolCut>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int sourceSurfaceArray[3] = {};
        double rotateAxisPoint1[3] = {};
        double rotateAxisPoint2[3] = {};
        double angle = 0;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint1, rotateAxisPoint1, 1, 3))return false;
        if (!h5tools->readGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint2, rotateAxisPoint2, 1, 3))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), FGKW_Angle, &angle))return false;

        Interface::VirtualShape vShape(sourceSurfaceArray[0], sourceSurfaceArray[1], sourceSurfaceArray[2]);
        cmd->setSourceSurface(vShape);
        cmd->setRotateAxisPoint1(rotateAxisPoint1[0], rotateAxisPoint1[1], rotateAxisPoint1[2]);
        cmd->setRotateAxisPoint2(rotateAxisPoint2[0], rotateAxisPoint2[1], rotateAxisPoint2[2]);
        cmd->setAngle(angle);
        return true;
    }
    bool FITKAdaptorGeoModelModelRevolCut::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelRevolCut* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelRevolCut>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        VirtualShape sourceSurface = cmd->sourceSurface();
        int sourceSurfaceArray[3]{ sourceSurface.CmdId,sourceSurface.VirtualTopoId, sourceSurface.VirtualTopoIndex };
        std::array<double, 3>  rotateAxisPoint1 = cmd->rotateAxisPoint1();
        double rotateAxisPoint1Array[3]{ rotateAxisPoint1[0], rotateAxisPoint1[1], rotateAxisPoint1[2] };
        std::array<double, 3>  rotateAxisPoint2 = cmd->rotateAxisPoint2();
        double rotateAxisPoint2Array[3]{ rotateAxisPoint2[0], rotateAxisPoint2[1], rotateAxisPoint2[2] };
        double angle = cmd->angle();
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_SourceSurface, sourceSurfaceArray, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint1, rotateAxisPoint1Array, 1, 3);
        h5tools->writeGroupAttrDoubleArray(std::get<1>(paramGroup), FGKW_RotateAxisPoint2, rotateAxisPoint2Array, 1, 3);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), FGKW_Angle, angle);

        return true;
    }

    bool  FITKAdaptorGeoModelModelSweepCut::readParameters()
    {
        Interface::FITKAbsGeoModelSweepCut* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSweepCut>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int profileArray[3] = {};
        int array[3] = {};
        int curveCount = {};
        QList<VirtualShape> curves{};
        //读取参数
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Profile, profileArray, 1, 3))return false;
        if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Curve, array, 1, 3))return false;
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "CurveCount", &curveCount))return false;
        for (int i = 0; i < curveCount; i++)
        {
            int curveArray[3] = {};
            std::string curveName = std::to_string(i) + std::string(FGKW_Curve);
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), curveName.c_str(), curveArray, 1, 3)) return false;
            VirtualShape shape{ curveArray[0], curveArray[1], curveArray[2] };
            curves.append(shape);
        }
        //设置参数
        Interface::VirtualShape profile(profileArray[0], profileArray[1], profileArray[2]);
        Interface::VirtualShape curve(array[0], array[1], array[2]);
        cmd->setProfile(profile);
        cmd->setCurves(curves);
        cmd->setCurve(curve);
        return true;
    }
    bool FITKAdaptorGeoModelModelSweepCut::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelSweepCut* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelSweepCut>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数
        VirtualShape profile = cmd->profile();
        int profileArray[3]{ profile.CmdId,profile.VirtualTopoId, profile.VirtualTopoIndex };
        QList<VirtualShape> curves = cmd->curves();
        int curveCount = curves.size();
        VirtualShape curve = cmd->curve();
        int array[3]{ curve.CmdId,curve.VirtualTopoId, curve.VirtualTopoIndex };
        //写出参数
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Profile, profileArray, 1, 3);
        h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), FGKW_Curve, array, 1, 3);
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "CurveCount", curveCount);
        for (int i = 0; i < curveCount; i++)
        {
            VirtualShape curve = curves[i];
            int curveArray[3]{ curve.CmdId ,curve.VirtualTopoId, curve.VirtualTopoIndex };
            std::string curveName = std::to_string(i) + std::string(FGKW_Curve);
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), curveName.c_str(), curveArray, 1, 3);
        }
        return true;
    }

    bool  FITKAdaptorGeoModelModelExportTopos::readParameters()
    {
        Interface::FITKAbsGeoModelExportTopos* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExportTopos>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //打开group
        std::tuple<bool, H5::Group> paramGroup = h5tools->openGp(*_h5Group, FGKW_Parameter);

        int count = 0;
        std::string exportFileName = "";
        bool enableStitch = false;
        double stitchTol = 0;
        QList<VirtualShape> topos{};
        //读取参数
        if (!h5tools->readGroupint(std::get<1>(paramGroup), "TopoCount", &count))return false;
        if (!h5tools->readGroupBool(std::get<1>(paramGroup), "EnableStitch", &enableStitch))return false;
        if (!h5tools->readGroupDouble(std::get<1>(paramGroup), "StitchTolerance", &stitchTol))return false;
        if (!h5tools->readGroupStr(std::get<1>(paramGroup), "FileName", exportFileName))return false;
        for (int i = 0; i < count; i++)
        {
            int topoArray[3] = {};
            std::string name = std::to_string(i) + std::string("Topo");
            if (!h5tools->readGroupAttrIntArray(std::get<1>(paramGroup), name.c_str(), topoArray, 1, 3)) return false;
            VirtualShape shape{ topoArray[0], topoArray[1], topoArray[2] };
            topos.append(shape);
        }
        //设置参数
        cmd->setFileName(QString::fromStdString(exportFileName));
        cmd->setEnableStitch(enableStitch);
        cmd->setStitchTolerance(stitchTol);
        cmd->setExportTopos(topos);
        return true;
    }
    bool FITKAdaptorGeoModelModelExportTopos::writeParameters(H5::Group &g)
    {
        Interface::FITKAbsGeoModelExportTopos* cmd = this->getDataObjectAs<Interface::FITKAbsGeoModelExportTopos>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建group组
        std::tuple<bool, H5::Group> paramGroup = h5tools->createGp(g, FGKW_Parameter);
        if (!std::get<0>(paramGroup)) return false;
        //获取参数

        QList<VirtualShape> topos = cmd->getExportTopos();
        int count = topos.size();
        std::string exportFileName = cmd->getFileName().toStdString();
        bool enableStitch = cmd->getEnableStitch();
        double stitchTol = cmd->getStitchTolerance();

        //写出参数
        h5tools->writeGroupAttrInt(std::get<1>(paramGroup), "TopoCount", count);
        h5tools->writeGroupAttrBool(std::get<1>(paramGroup), "EnableStitch", enableStitch);
        h5tools->writeGroupAttrDouble(std::get<1>(paramGroup), "StitchTolerance", stitchTol);
        h5tools->writeGroupAttrStr(std::get<1>(paramGroup), "FileName", exportFileName);
        for (int i = 0; i < count; i++)
        {
            VirtualShape topo = topos[i];
            int topoArray[3]{ topo.CmdId ,topo.VirtualTopoId, topo.VirtualTopoIndex };
            std::string name = std::to_string(i) + std::string("Topo");
            h5tools->writeGroupAttrIntArray(std::get<1>(paramGroup), name.c_str(), topoArray, 1, 3);
        }
        return true;
    }
}



