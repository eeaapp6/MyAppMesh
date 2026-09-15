/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAdaptorGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"


namespace Interface
{
    bool FITKAdaptorGeoDatum::adaptR()
    {
        //读取基本的属性
        bool ok = this->readNamedDataObject();
        if (!ok) return false;

        //读取命令数据对象,写出虚拓扑信息
        ok = this->readCommandDataObject();
        if (!ok) return false;

        //读取参数，调用虚函数
        ok = this->readParameters();
        if (!ok) return false;

        return ok;
    }

    bool FITKAdaptorGeoDatum::adaptW()
    {
        Interface::FITKAbsGeoDatum* cmd = this->getDataObjectAs<Interface::FITKAbsGeoDatum>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //创建命令根节点
        std::tuple<bool, H5::Group> group = h5tools->createGp(*_h5Group, cmd->getDataObjectName());
        if (!std::get<0>(group)) return false;
        //读取时候也要按照顺序读取
        const int n = _h5Group->getNumAttrs();
        h5tools->writeGroupAttrStr(*_h5Group,
            QString::number(n).toLatin1().data(), cmd->getDataObjectName().toStdString());

        //写出基本的属性
        bool ok = this->writeNamedDataObject(std::get<1>(group));
        if (!ok) return false;
        //写出命令数据对象,写出虚拓扑信息
        ok = this->writeCommandDataObject(std::get<1>(group));
        if (!ok) return false;
        //写出参数，调用虚函数
        ok = this->writeParameters(std::get<1>(group));
        if (!ok) return false;
        return ok;
    }

    bool FITKAdaptorGeoDatum::writeNamedDataObject(H5::Group& gp)
    {
        Core::FITKAbstractNDataObject* data = this->getDataObjectAs<Core::FITKAbstractNDataObject>();
        if (data == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //写出数据对象的基本信息
        h5tools->writeGroupAttrStr(gp, FGKW_DataObjectName, data->getDataObjectName().toStdString());
        h5tools->writeGroupAttrInt(gp, FGKW_DataObjectID, data->getDataObjectID());
        h5tools->writeGroupAttrBool(gp, FGKW_DataObjectEnable, data->isEnable());
        h5tools->writeGroupAttrInt(gp, FGKW_DataObjectParentID, data->getParentDataID());

        return true;
    }

    bool FITKAdaptorGeoDatum::readNamedDataObject()
    {
        Core::FITKAbstractNDataObject* data = this->getDataObjectAs<Core::FITKAbstractNDataObject>();
        if (data == nullptr || _reader == nullptr || _h5Group == nullptr) return false;

        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        // 读取数据对象的基本信息
        std::string name = {};
        int id = 0;
        int parentID = 0;
        bool enable = false;

        if (!h5tools->readGroupStr(*_h5Group, FGKW_DataObjectName, name)) return false;
        if (!h5tools->readGroupint(*_h5Group, FGKW_DataObjectID, &id)) return false;
        if (!h5tools->readGroupBool(*_h5Group, FGKW_DataObjectEnable, &enable)) return false;
        if (!h5tools->readGroupint(*_h5Group, FGKW_DataObjectParentID, &parentID)) return false;

        data->setDataObjectName(QString::fromStdString(name));
        data->enable(enable);
        data->setParentDataID(parentID);
        //设置DataObjectID
        Core::FITKAbstractDataIDAlter dataIDAlter;
        dataIDAlter.modifyDataID(data, id);
        return true;
    }
    bool FITKAdaptorGeoDatum::writeCommandDataObject(H5::Group& g)
    {
        Interface::FITKAbsGeoCommand* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCommand>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        //写出命令数据对象
        Interface::FITKGeoEnum::FITKGeometryComType commandType = cmd->getGeometryCommandType();
        bool ok = false;
        Core::FITKEnumTransfer<Interface::FITKGeoEnum::FITKGeometryComType> enumTransfer;
        QString cmdTypeStr = enumTransfer.toString(commandType, ok);
        if (!ok) return false;
        //写出命令类型
        h5tools->writeGroupAttrStr(g, FGKW_CommandType, cmdTypeStr.toStdString());
        //命令状态
        Interface::FITKGeoEnum::FITKGeoStatus status = cmd->getCommandStatus();
        Core::FITKEnumTransfer<Interface::FITKGeoEnum::FITKGeoStatus> enumTransfer2;
        QString cmdStatusStr = enumTransfer2.toString(status, ok);
        if (!ok) return false;
        h5tools->writeGroupAttrStr(g, FGKW_CommandStatus, cmdStatusStr.toStdString());

        h5tools->writeGroupAttrInt(g, FGKW_ModifiedCmdID, cmd->getModifiedCmdID());
        h5tools->writeGroupAttrBool(g, FGKW_EnableRefList, cmd->getEnableReferenceList());
        h5tools->writeGroupAttrInt(g, FGKW_RefCommandCount, cmd->getReferenceCmdCount());

        std::tuple<bool, H5::Group> referenceIDGroup = h5tools->createGp(g, FGKW_ReferenceCmdID);
        std::tuple<bool, H5::Group> referenceTypeGroup = h5tools->createGp(g, FGKW_ReferenceCmdType);
        if (!std::get<0>(referenceIDGroup) || !std::get<0>(referenceTypeGroup)) return false;
        for (int i = 0; i < cmd->getReferenceCmdCount(); i++)
        {
            Interface::FITKAbsGeoCommand* refComm = cmd->getReferenceCmdByIndex(i);
            if (refComm == nullptr) return false; //判断异常
            int refID = refComm->getDataObjectID();
            std::string str = std::to_string(i);
            const char* constStr = str.c_str();
            h5tools->writeGroupAttrInt(std::get<1>(referenceIDGroup), constStr, refID);
        }
        Interface::FITKAbsGeoShapeAgent* agent = cmd->getShapeAgent();
        if (agent == nullptr) return false;
        Interface::FITKVirtualTopoManager* manager = agent->getVirtualTopoManager();
        if (manager == nullptr) return false;
        FITKAbsVirtualTopo* topo = manager->getRootObj();
        if (topo == nullptr) return false;
        const int id = topo->getDataObjectID();
        h5tools->writeGroupAttrInt(g, FGKW_VirtualTopoMgrID, id);
        return this->writeVirtualShape(g);
    }

    bool FITKAdaptorGeoDatum::readCommandDataObject()
    {
        Interface::FITKAbsGeoCommand* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCommand>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;

        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        // 读取命令类型
        std::string cmdStatusStr = {};
        //std::string cmdTypeStr;
        //if (!h5tools->readGroupStr(*_h5Group, FGKW_CommandType, cmdTypeStr)) return false;
        if (!h5tools->readGroupStr(*_h5Group, FGKW_CommandStatus, cmdStatusStr)) return false;

        QMetaEnum metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKGeoStatus>();
        Interface::FITKGeoEnum::FITKGeoStatus status = (Interface::FITKGeoEnum::FITKGeoStatus)metaEnum.keyToValue(cmdStatusStr.c_str());
        cmd->setCommandStatus(status);

        /*metaEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::FITKGeometryComType>();
        Interface::FITKGeoEnum::FITKGeometryComType type = (Interface::FITKGeoEnum::FITKGeometryComType)metaEnum.keyToValue(cmdTypeStr.c_str());
        cmd->setCommandStatus(status);*/

        // 读取其他属性
        int modifiedCmdID = 0;
        int refCmdCount = 0;
        int virtualTopoMgrID = 0;
        bool enableRefList = false;

        if (!h5tools->readGroupint(*_h5Group, FGKW_ModifiedCmdID, &modifiedCmdID)) return false;
        if (!h5tools->readGroupBool(*_h5Group, FGKW_EnableRefList, &enableRefList)) return false;
        if (!h5tools->readGroupint(*_h5Group, FGKW_RefCommandCount, &refCmdCount)) return false;

        cmd->setModifiedCmdID(modifiedCmdID);
        cmd->enableReferenceList(enableRefList);
        cmd->setModifiedCmdID(modifiedCmdID);
        //打开子节点
        std::tuple<bool, H5::Group> _h5ReferenceCmdIDGroup = h5tools->openGp(*_h5Group, FGKW_ReferenceCmdID);
        std::tuple<bool, H5::Group> _h5ReferenceCmdTypeGroup = h5tools->openGp(*_h5Group, FGKW_ReferenceCmdType);
        int refID = 0;
        for (int i = 0; i < refCmdCount; i++)
        {
            std::string str = std::to_string(i);
            const char* constStr = str.c_str();
            if (!h5tools->readGroupint(std::get<1>(_h5ReferenceCmdIDGroup), constStr, &refID)) return false;
            cmd->addReferenceCmdID(refID);
        }
        if (!h5tools->readGroupint(*_h5Group, FGKW_VirtualTopoMgrID, &virtualTopoMgrID)) return false;
        return this->readVirtualShape();
    }
    bool FITKAdaptorGeoDatum::writeVirtualShape(H5::Group& g)
    {
        Interface::FITKAbsGeoCommand* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCommand>();
        if (cmd == nullptr || _writer == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        Interface::FITKAbsGeoShapeAgent* agent = cmd->getShapeAgent();
        if (agent == nullptr) return false;
        Interface::FITKVirtualTopoManager* manager = agent->getVirtualTopoManager();
        if (manager == nullptr) return false;

        std::tuple<bool, H5::Group> virtualShapeGroup = h5tools->createGp(g, FGKW_vTopoShapeManager);
        if (!std::get<0>(virtualShapeGroup)) return false;

        QMetaEnum m = QMetaEnum::fromType<Interface::FITKGeoEnum::VTopoShapeType>();
        int vTopoShapeTypeCount = m.keyCount();
        //实际存储类型
        int shapeTypeCount = 0;
        int dataObjectInfo[4] = {};
        for (int i = 1; i < vTopoShapeTypeCount; i++)
        {
            Interface::FITKShapeVirtualTopoManager* shapeManager = manager->getShapeVirtualTopoManager((Interface::FITKGeoEnum::VTopoShapeType)i);
            if (shapeManager == nullptr) continue;
            shapeTypeCount++;
            QString name = m.valueToKey(i);
            std::tuple<bool, H5::Group> vTopoShapeTypeGroup = h5tools->createGp(std::get<1>(virtualShapeGroup), name);
            if (!std::get<0>(vTopoShapeTypeGroup)) return false;
            int shapeManagerCount = shapeManager->getDataCount();
            h5tools->writeGroupAttrInt(std::get<1>(vTopoShapeTypeGroup), FGKW_Count, shapeManagerCount);
            for (int j = 0; j < shapeManagerCount; j++)
            {
                std::string dataObjectResult = std::to_string(j) + std::string(FGKW_DataObject);
                std::string childVTopoResult = std::to_string(j) + std::string(FGKW_ChildVTopo);
                const char* dataObject = dataObjectResult.c_str();
                const char* childVTopo = childVTopoResult.c_str();
                auto shape = shapeManager->getDataByIndex(j);
                if (shape == nullptr) continue;
                int indexLabel = shape->getIndexLabel();

                //写出虚拓扑数据对象信息
                dataObjectInfo[0] = shape->getDataObjectID();
                dataObjectInfo[1] = shape->getParentDataID();
                dataObjectInfo[2] = shape->isEnable();
                dataObjectInfo[3] = indexLabel;
                h5tools->writeGroupAttrIntArray(std::get<1>(vTopoShapeTypeGroup), dataObject, dataObjectInfo, 1, 4);

                std::string dataCreatedCmdId = std::to_string(j) + std::string("_CreatedCmdId");
                std::string dataTag = std::to_string(j) + std::string("_Tag");

                std::string dataCombinedTopoCount = std::to_string(j) + std::string("_CombinedTopoCount");
                int createdCmdId = shape->getCreatedGeoCmdID();
                int tag = shape->getTag();
                FITKAbsVirtualTopo* combinedToTopo = shape->getCombinedToTopo();
                if (combinedToTopo != nullptr)
                {
                    std::string dataID = std::to_string(j) + std::string("_CombinedToTopoID");
                    std::string combinedToTopoTypeStr = std::to_string(j) + std::string("_CombinedToTopoType");
                    FITKGeoEnum::VTopoShapeType combinedToTopoType = combinedToTopo->getShapeType();
                    bool ok = false;
                    Core::FITKEnumTransfer<Interface::FITKGeoEnum::VTopoShapeType> enumTransfer;
                    QString cmdTypeStr = enumTransfer.toString(combinedToTopoType, ok);
                    if (!ok) return false;
                    int combinedToTopoID = combinedToTopo->getDataObjectID();
                    h5tools->writeGroupAttrInt(std::get<1>(vTopoShapeTypeGroup), dataID.c_str(), combinedToTopoID);
                    h5tools->writeGroupAttrStr(std::get<1>(vTopoShapeTypeGroup), combinedToTopoTypeStr.c_str(), cmdTypeStr.toStdString());
                }

                QList<FITKAbsVirtualTopo*> combinedBasicToposRecur = shape->getCombinedBasicToposRecur();
                int combinedTopoCount = combinedBasicToposRecur.size();
                h5tools->writeGroupAttrInt(std::get<1>(vTopoShapeTypeGroup), dataCreatedCmdId.c_str(), createdCmdId);
                h5tools->writeGroupAttrInt(std::get<1>(vTopoShapeTypeGroup), dataTag.c_str(), tag);
                h5tools->writeGroupAttrInt(std::get<1>(vTopoShapeTypeGroup), dataCombinedTopoCount.c_str(), combinedTopoCount);

                for (int k = 0; k < combinedTopoCount; k++)
                {
                    std::string dataCombinedChildTopoID = std::to_string(j) + std::string("_CombinedChildTopoID_") + std::to_string(k);
                    std::string dataCombinedChildTopoType = std::to_string(j) + std::string("_CombinedChildTopoType_") + std::to_string(k);
                    FITKAbsVirtualTopo* combinedChildTopo = combinedBasicToposRecur[k];
                    int combinedChildTopoID = combinedChildTopo->getDataObjectID();
                    FITKGeoEnum::VTopoShapeType combinedChildTopoType = combinedChildTopo->getShapeType();
                    bool childOk = false;
                    Core::FITKEnumTransfer<Interface::FITKGeoEnum::VTopoShapeType> childEnumTransfer;
                    QString childTypeStr = childEnumTransfer.toString(combinedChildTopoType, childOk);
                    if (!childOk) return false;
                    h5tools->writeGroupAttrInt(std::get<1>(vTopoShapeTypeGroup), dataCombinedChildTopoID.c_str(), combinedChildTopoID);
                    h5tools->writeGroupAttrStr(std::get<1>(vTopoShapeTypeGroup), dataCombinedChildTopoType.c_str(), childTypeStr.toStdString());
                }
                int subTopoCount = shape->getSubTopoCount();
                std::string dataSubTopoCount = std::to_string(j) + std::string("_SubTopoCount");
                h5tools->writeGroupAttrInt(std::get<1>(vTopoShapeTypeGroup), dataSubTopoCount.c_str(), subTopoCount);
                int* childTopoData = new int[subTopoCount + 1];
                for (int k = 0; k < subTopoCount; k++)
                {
                    int vChildTopoID = shape->getSubTopo(k)->getDataObjectID();
                    childTopoData[k + 1] = vChildTopoID;
                    FITKGeoEnum::VTopoShapeType childTopoType = shape->getSubTopo(k)->getShapeType();
                    bool childTopoOk = false;
                    Core::FITKEnumTransfer<Interface::FITKGeoEnum::VTopoShapeType> childEnum;
                    QString childType = childEnum.toString(childTopoType, childTopoOk);
                    if (!childTopoOk) return false;
                    std::string dataChildTopoType = std::to_string(j) + std::string("_ChildTopoType_") + std::to_string(k);
                    h5tools->writeGroupAttrStr(std::get<1>(vTopoShapeTypeGroup), dataChildTopoType.c_str(), childType.toStdString());
                }
                if (subTopoCount > 0)
                {
                    childTopoData[0] = subTopoCount;
                    h5tools->writeGroupAttrIntArray(std::get<1>(vTopoShapeTypeGroup), childVTopo, childTopoData, 1, subTopoCount + 1);
                }
                delete[] childTopoData;
            }
            int numCount = std::get<1>(virtualShapeGroup).getNumAttrs();
            std::string attrCount = std::to_string(numCount);
            const char* numAttrCount = attrCount.c_str();
            h5tools->writeGroupAttrStr(std::get<1>(virtualShapeGroup), numAttrCount, name.toStdString());
        }

        std::tuple<bool, H5::Group> rootObjGroup = h5tools->createGp(std::get<1>(virtualShapeGroup), "rootObj");
        if (!std::get<0>(rootObjGroup)) return false;
        int rootObjCount = manager->getNumberOfRootObjs();
        h5tools->writeGroupAttrInt(std::get<1>(rootObjGroup), "rootObjCount", rootObjCount);
        for (int i = 0; i < rootObjCount; i++)
        {
            FITKAbsVirtualTopo* root = manager->getRootObj(i);
            if (!root) return false;

            std::string rootDataType = std::to_string(i) + std::string("_RootType");
            FITKGeoEnum::VTopoShapeType rootType = root->getShapeType();
            bool rootTypeOk = false;
            Core::FITKEnumTransfer<Interface::FITKGeoEnum::VTopoShapeType> rootTypeEnumTransfer;
            QString rootTypeStr = rootTypeEnumTransfer.toString(rootType, rootTypeOk);
            if (!rootTypeOk) return false;
            h5tools->writeGroupAttrStr(std::get<1>(rootObjGroup), rootDataType.c_str(), rootTypeStr.toStdString());

            std::string rootTopoID = std::to_string(i) + std::string("_RootID");
            int rootID = root->getDataObjectID();
            h5tools->writeGroupAttrInt(std::get<1>(rootObjGroup), rootTopoID.c_str(), rootID);
        }

        h5tools->writeGroupAttrInt(std::get<1>(virtualShapeGroup), FGKW_Count, shapeTypeCount);
        return this->writeShape(g);
    }
    bool FITKAdaptorGeoDatum::readVirtualShape()
    {
        Interface::FITKAbsGeoCommand* cmd = this->getDataObjectAs<Interface::FITKAbsGeoCommand>();
        if (cmd == nullptr || _reader == nullptr || _h5Group == nullptr) return false;

        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        // 获取几何形状代理
        Interface::FITKAbsGeoShapeAgent* agent = cmd->getShapeAgent();
        if (agent == nullptr) return false;

        Interface::FITKVirtualTopoManager* manager = agent->getVirtualTopoManager();
        if (manager == nullptr) return false;

        // 打开虚拟拓扑形状管理器组
        int virtualTopoManagerRootID = h5tools->readGroupint(*_h5Group, FGKW_VirtualTopoMgrID);
        std::tuple<bool, H5::Group> virtualShapeGroup = h5tools->openGp(*_h5Group, FGKW_vTopoShapeManager);
        if (!std::get<0>(virtualShapeGroup)) return false;

        std::tuple<bool, H5::Group> rootGroup = h5tools->openGp(std::get<1>(virtualShapeGroup), "rootObj");
        if (!std::get<0>(rootGroup)) return false;

        // 遍历虚拟拓扑形状类型
        QMetaEnum m = QMetaEnum::fromType<Interface::FITKGeoEnum::VTopoShapeType>();
        int vTopoShapeTypeCount = m.keyCount();
        int shapeTypeCount = 0;
        int dataObjectInfo[4] = {};
        std::string vTopoTypeStr = {};
        for (int i = vTopoShapeTypeCount - 1; i > 0; i--)
        {
            Interface::FITKShapeVirtualTopoManager* shapeManager = manager->getShapeVirtualTopoManager((Interface::FITKGeoEnum::VTopoShapeType)i);
            if (shapeManager == nullptr) return false;
            std::string str = std::to_string(i - 1);
            const char* vTopoType = str.c_str();
            if (!h5tools->readGroupStr(std::get<1>(virtualShapeGroup), vTopoType, vTopoTypeStr)) return false;
            if (!std::get<1>(virtualShapeGroup).nameExists(vTopoTypeStr)) continue;
            std::tuple<bool, H5::Group> vTopoTypeGroup = h5tools->openGp(std::get<1>(virtualShapeGroup), vTopoTypeStr.c_str());
            int count = 0;
            if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), FGKW_Count, &count)) return false;
            for (int j = 0; j < count; j++)
            {
                std::string dataObjectResult = std::to_string(j) + std::string(FGKW_DataObject);
                const char* dataObject = dataObjectResult.c_str();
                Interface::FITKAbsVirtualTopo* vt = new Interface::FITKAbsVirtualTopo();
                vt->setShapeType((Interface::FITKGeoEnum::VTopoShapeType)i);
                shapeManager->appendDataObj(vt);

                if (!h5tools->readGroupAttrIntArray(std::get<1>(vTopoTypeGroup), dataObject, dataObjectInfo, 1, 4))
                {
                    delete vt;
                    return false;
                }
                Core::FITKAbstractDataIDAlter dataIDAlter;
                dataIDAlter.modifyDataID(vt, dataObjectInfo[0]);
                vt->setParentDataID(dataObjectInfo[1]);
                vt->enable(dataObjectInfo[2]);
                vt->setIndexLabel(dataObjectInfo[3]);

                std::string dataCreatedCmdId = std::to_string(j) + std::string("_CreatedCmdId");
                std::string dataTag = std::to_string(j) + std::string("_Tag");
                std::string dataID = std::to_string(j) + std::string("_CombinedToTopoID");
                std::string dataCombinedTopoCount = std::to_string(j) + std::string("_CombinedTopoCount");
                std::string combinedToTopoTypeStr = std::to_string(j) + std::string("_CombinedToTopoType");
                if (std::get<1>(vTopoTypeGroup).nameExists(dataID) && std::get<1>(vTopoTypeGroup).nameExists(combinedToTopoTypeStr))
                {
                    int combinedToTopoID = 0;
                    std::string combinedToTopoType = {};
                    if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), dataID.c_str(), &combinedToTopoID)) return false;
                    if (!h5tools->readGroupStr(std::get<1>(vTopoTypeGroup), combinedToTopoTypeStr.c_str(), combinedToTopoType)) return false;
                    QMetaEnum combinedToTopoEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::VTopoShapeType>();
                    Interface::FITKGeoEnum::VTopoShapeType combinedToTopoTypeEnum = (Interface::FITKGeoEnum::VTopoShapeType)combinedToTopoEnum.keyToValue(combinedToTopoType.c_str());
                    Interface::FITKShapeVirtualTopoManager* combinedToTopoManager = manager->getShapeVirtualTopoManager(combinedToTopoTypeEnum);
                    Interface::FITKAbsVirtualTopo* combinedToTopo = combinedToTopoManager->getDataByID(combinedToTopoID);
                    vt->setCombinedToTopo(combinedToTopo);
                }
                int tag = 0;
                int createdCmdId = 0;
                int combinedTopoCount = 0;
                if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), dataTag.c_str(), &tag)) return false;
                if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), dataCreatedCmdId.c_str(), &createdCmdId)) return false;
                if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), dataCombinedTopoCount.c_str(), &combinedTopoCount)) return false;
                for (int k = 0; k < combinedTopoCount; k++)
                {
                    int childTopoID = 0;
                    std::string childTopoType = {};
                    std::string dataCombinedChildTopoID = std::to_string(j) + std::string("_CombinedChildTopoID_") + std::to_string(k);
                    std::string dataCombinedChildTopoType = std::to_string(j) + std::string("_CombinedChildTopoType_") + std::to_string(k);
                    if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), dataCombinedChildTopoID.c_str(), &childTopoID)) return false;
                    if (!h5tools->readGroupStr(std::get<1>(vTopoTypeGroup), dataCombinedChildTopoType.c_str(), childTopoType)) return false;
                    QMetaEnum combinedChildToTopoEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::VTopoShapeType>();
                    Interface::FITKGeoEnum::VTopoShapeType combinedToTopoChildTypeEnum = (Interface::FITKGeoEnum::VTopoShapeType)combinedChildToTopoEnum.keyToValue(childTopoType.c_str());
                    Interface::FITKShapeVirtualTopoManager* combinedChildTopoManager = manager->getShapeVirtualTopoManager(combinedToTopoChildTypeEnum);
                    Interface::FITKAbsVirtualTopo* combinedChildTopo = combinedChildTopoManager->getDataByID(childTopoID);
                    vt->addCombinedTopo(combinedChildTopo);
                }

                vt->setTag(tag);
                vt->setCreatedGeoCmdID(createdCmdId);
                if (dataObjectInfo[0] == virtualTopoManagerRootID)
                {
                    manager->setRootObj(vt);
                }
            }
        }
        for (int i = vTopoShapeTypeCount - 1; i > 0; i--)
        {
            Interface::FITKShapeVirtualTopoManager* shapeManager = manager->getShapeVirtualTopoManager((Interface::FITKGeoEnum::VTopoShapeType)i);
            if (shapeManager == nullptr) return false;
            /* std::tuple<bool, H5::Group> virtualShapeGroup = h5tools->openGp(*_h5Group, FGKW_vTopoShapeManager);
             if (!std::get<0>(virtualShapeGroup)) return false;*/
            std::string str = std::to_string(i - 1);
            const char* vTopoType = str.c_str();
            if (!h5tools->readGroupStr(std::get<1>(virtualShapeGroup), vTopoType, vTopoTypeStr)) return false;
            if (!std::get<1>(virtualShapeGroup).nameExists(vTopoTypeStr)) continue;
            std::tuple<bool, H5::Group> vTopoTypeGroup = h5tools->openGp(std::get<1>(virtualShapeGroup), vTopoTypeStr.c_str());
            int count = 0;
            if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), FGKW_Count, &count)) return false;
            for (int j = 0; j < count; j++)
            {
                std::string childVTopoResult = std::to_string(j) + std::string(FGKW_ChildVTopo);
                std::string dataObjectResult = std::to_string(j) + std::string(FGKW_DataObject);
                const char* childVTopo = childVTopoResult.c_str();
                const char* dataObject = dataObjectResult.c_str();
                if (!h5tools->readGroupAttrIntArray(std::get<1>(vTopoTypeGroup), dataObject, dataObjectInfo, 1, 4)) return false;
                auto vTopo = shapeManager->getDataByID(dataObjectInfo[0]);
                if (vTopo == nullptr) return false;
                int subTopoCount = 0;
                std::string dataSubTopoCount = std::to_string(j) + std::string("_SubTopoCount");
                if (!h5tools->readGroupint(std::get<1>(vTopoTypeGroup), dataSubTopoCount.c_str(), &subTopoCount))return false;;
                if (subTopoCount < 0) continue;
                int* subTopoIDs = new int[subTopoCount + 1];
                for (int k = 0; k < subTopoCount; k++)
                {
                    if (!h5tools->readGroupAttrIntArray(std::get<1>(vTopoTypeGroup), childVTopo, subTopoIDs, 1, subTopoCount + 1)) return false;
                    int vChildTopoID = subTopoIDs[k + 1];

                    std::string childType = {};
                    std::string dataChildTopoType = std::to_string(j) + std::string("_ChildTopoType_") + std::to_string(k);
                    if (!h5tools->readGroupStr(std::get<1>(vTopoTypeGroup), dataChildTopoType.c_str(), childType)) return false;
                    QMetaEnum childToTopoEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::VTopoShapeType>();
                    Interface::FITKGeoEnum::VTopoShapeType childTypeEnum = (Interface::FITKGeoEnum::VTopoShapeType)childToTopoEnum.keyToValue(childType.c_str());
                    Interface::FITKShapeVirtualTopoManager* childTopoManager = manager->getShapeVirtualTopoManager(childTypeEnum);
                    Interface::FITKAbsVirtualTopo* childTopo = childTopoManager->getDataByID(vChildTopoID);

                    if (childTopo == nullptr) return false;
                    vTopo->addSubTopo(childTopo);
                }
                delete[] subTopoIDs;
            }
        }
        int rootCount = 0;
        if (!h5tools->readGroupint(std::get<1>(rootGroup), "rootObjCount", &rootCount)) return false;
        for (int i = 0; i < rootCount; i++)
        {
            std::string rootDataType = std::to_string(i) + std::string("_RootType");
            std::string rootDataID = std::to_string(i) + std::string("_RootID");

            std::string rootTypeStr = {};
            int rootID = 0;

            if (!h5tools->readGroupStr(std::get<1>(rootGroup), rootDataType.c_str(), rootTypeStr)) return false;
            if (!h5tools->readGroupint(std::get<1>(rootGroup), rootDataID.c_str(), &rootID)) return false;

            QMetaEnum rootTypEnum = QMetaEnum::fromType<Interface::FITKGeoEnum::VTopoShapeType>();
            Interface::FITKGeoEnum::VTopoShapeType rootType = (Interface::FITKGeoEnum::VTopoShapeType)rootTypEnum.keyToValue(rootTypeStr.c_str());

            Interface::FITKShapeVirtualTopoManager* rootTopoManager = manager->getShapeVirtualTopoManager(rootType);
            Interface::FITKAbsVirtualTopo* root = rootTopoManager->getDataByID(rootID);
            if (root == nullptr) return false;
            manager->addRootObj(root);
        }
        return this->readShape();
    }
    bool FITKAdaptorGeoDatum::writeShape(H5::Group& g)
    {
        return false;
    }
    bool FITKAdaptorGeoDatum::readShape()
    {
        return false;
    }
    bool  FITKAdaptorGeoDatum::readParameters()
    {
        return false;
    }
    bool FITKAdaptorGeoDatum::writeParameters(H5::Group &g)
    {
        return false;
    }
}
