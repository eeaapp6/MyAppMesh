/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorAssembly.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITK_Interface/FITKInterfaceModel/FITKAssembly.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractAssInstance.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"

#include "FITK_Interface/FITKInterfaceIOHDF5/FITKInterfaceHDF5AdaptorSet.h"
#include "FITK_Interface/FITKInterfaceIOHDF5/FITKInterfaceHDF5AdaptorSurface.h"
namespace IO
{
    QString FITKInterfaceHDF5AdaptorAssembly::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorAssembly";
    }

    bool FITKInterfaceHDF5AdaptorAssembly::adaptR()
    {
        auto assembly = dynamic_cast<Interface::FITKAssembly*>(_dataObj);
        if (!assembly || !_reader || !_h5Group) return false;
        return readAssembly(assembly, *_h5Group);
    }

    bool FITKInterfaceHDF5AdaptorAssembly::adaptW()
    {
        auto assembly = dynamic_cast<Interface::FITKAssembly*>(_dataObj);
        if (!assembly || !_writer || !_h5Group) return false;
        return writeAssembly(assembly, *_h5Group);
    }

    bool FITKInterfaceHDF5AdaptorAssembly::readAssembly(Interface::FITKAssembly* assembly, H5::Group& h5Group)
    {
        if (!assembly || !_reader || h5Group.getLocId() == -1) return false;
        bool isR = true;
        isR &= readNDataObject(assembly, h5Group);
        auto componentManager = assembly->getComponentManager();
        if (!componentManager) return false;

        if (h5Group.nameExists("Instances"))
        {
            auto instancesGroup = h5Group.openGroup("Instances");
            //Instance
            int instanceCount = instancesGroup.getNumAttrs();
            for (int i = 0; i < instanceCount; ++i)
            {
                QString indexName = QString::number(i);
                auto strName = readStrAttribute(instancesGroup, indexName.toStdString());
                if (instancesGroup.nameExists(strName))
                {
                    auto instanceGroup = instancesGroup.openGroup(strName);
                    isR &= readInstance(instanceGroup);
                }
            }
        }

        if (h5Group.nameExists("Components"))
        {
            auto componentsGroup = h5Group.openGroup("Components");
            isR &= readSetSurf(componentManager, componentsGroup);
        }

        return isR;
    }


    bool FITKInterfaceHDF5AdaptorAssembly::readSetSurf(Interface::FITKComponentManager* setSurfManager, H5::Group& h5Group)
    {
        if (!setSurfManager)return false;
        bool isR = true;
        //打开set子节点
        int componentCount = h5Group.getNumAttrs();
        for (int i = 0; i < componentCount; ++i)
        {
            QString componentIndex = QString::number(i);
            auto componentName = readStrAttribute(h5Group, componentIndex.toStdString());
            if (!h5Group.nameExists(componentName))
            {
                _reader->consoleMessage(3, QString("Read Component Error, name : %1").arg(QString::fromStdString(componentName)));
                continue;
            }
            auto componentGroup = h5Group.openGroup(componentName);

            auto uniqueType = readStrAttribute(componentGroup, "UniqueType");
            auto TypeStr = readStrAttribute(componentGroup, "Type");
            if (uniqueType == "Set")
            {
                Interface::FITKModelSet *cSet = new Interface::FITKModelSet();
                bool r = readSet(cSet, componentGroup);
                setSurfManager->appendDataObj(cSet);
                if (!r) delete cSet;
                isR &= r;
            }
            else if (uniqueType == "MeshSurface")
            {
                Interface::FITKMeshSurface* surface{};

                //枚举转换字符
                Core::FITKEnumTransfer<Interface::FITKMeshSurface::MeshSurfaceType> fitkSurfTypeTrafer;
                bool isValid = false;
                //写出类型
                auto Type = fitkSurfTypeTrafer.fromString(QString::fromStdString(TypeStr), isValid);
                if (!isValid) return false;

                if (Type == Interface::FITKMeshSurface::MeshSurfaceType::SurNode)
                    surface = new Interface::FITKMeshSurfaceNode;
                else if (Type == Interface::FITKMeshSurface::MeshSurfaceType::SurEle)
                    surface = new Interface::FITKMeshSurfaceElement;
                else
                    return false;
                if (!surface) return false;
                bool r = readSurface(surface, componentGroup);
                setSurfManager->appendDataObj(surface);
                if (!r) delete surface;
                isR &= r;
            }
            else if (uniqueType == "GeoSurface")
            {
                Interface::FITKGeometrySurface* surface = new Interface::FITKGeometrySurface();
                setSurfManager->appendDataObj(surface);
                bool r = readSurface(surface, componentGroup);
                if (!r) delete surface;
                isR &= r;
            }
            else
            {
                _reader->consoleMessage(3, QString("Read Assembly Component Error , UniqueType Error :%1").arg(QString::fromStdString(componentName)));
                continue;
            }
        }

        return isR;
    }


    bool FITKInterfaceHDF5AdaptorAssembly::readSet(Interface::FITKModelSet * set, H5::Group& h5Group)
    {
        auto assembly = dynamic_cast<Interface::FITKAssembly*>(_dataObj);
        if (!set || !assembly) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKInterfaceHDF5AdaptorSet>("HDF5", set);
        if (adaptor == nullptr) return false;

        adaptor->setCompomentManager(assembly->getComponentManager());
        adaptor->setFileReader(_reader);
        adaptor->setDataObject(set);
        adaptor->setH5GroupData(h5Group);
        bool ok = adaptor->adaptR();
        delete adaptor;
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Assembly Set Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return ok;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::readSurface(Interface::FITKModelSurface* surface, H5::Group& h5Group)
    {
        if (!surface) return false;
        //调用surface适配器读写
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKInterfaceHDF5AdaptorSurface>("HDF5", "Interface::FITKModelSurface");
        if (adaptor == nullptr) return false;
        adaptor->setFileReader(_reader);
        adaptor->setDataObject(surface);
        adaptor->setH5GroupData(h5Group);
        bool ok = adaptor->adaptR();
        delete adaptor;
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Assembly Surface Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return ok;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::readInstance(H5::Group& h5Group)
    {
        auto assembly = dynamic_cast<Interface::FITKAssembly*>(_dataObj);
        if (!assembly) return false;
        bool isR = true;
        bool isValid = false;

        //type
        if (!h5Group.attrExists("AbsModelType")) return false;
        auto absModelTypeStr = readStrAttribute(h5Group, "AbsModelType");
        Core::FITKEnumTransfer<Interface::FITKModelEnum::AbsModelType> fitkInstanceTypeTrafer;
        auto absModelType = fitkInstanceTypeTrafer.fromString(QString::fromStdString(absModelTypeStr), isValid);
        if (!isValid) return false;

        Interface::FITKAbstractAssInstance* assInstance{};
        if (absModelType == Interface::FITKModelEnum::AssInstance)
        {
            assInstance = new Interface::FITKAbstractAssInstance();
        }
        else
        {
            return false;
        }
        if (!assInstance) return false;
        isR &= readNDataObject(assInstance, h5Group);
        assembly->appendDataObj(assInstance);
        //modelID
        if (!h5Group.attrExists("ModelID")) return false;
        int modelID = readIntAttribute(h5Group, "ModelID");
        assInstance->setModel(modelID);
        isR &= readInstanceTR(assInstance, h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::readInstanceTR(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group)
    {
        auto instanceTR = dynamic_cast<Interface::FITKAbstractAssInstance*>(assInstance);
        if (!instanceTR) return false;
        bool isR = true;

        QList<double> tr;
        if (!readDoubleAttrbuteDataSet(h5Group, "Transform", tr, 1, 3)) return false;


        QList<double> p1List, p2List;
        //2025更改 如果读取不到P1 P2 则算作默认旋转
        if (!readDoubleAttrbuteDataSet(h5Group, "P1", p1List, 1, 3) || !readDoubleAttrbuteDataSet(h5Group, "P2", p2List, 1, 3)) return false;
        double transform[3]{ tr[0], tr[1], tr[2] };
        double p1[3]{ p1List[0],p1List[1], p1List[2] };
        double p2[3]{ p2List[0],p2List[1], p2List[2] };
        double angle = readDoubleAttribute(h5Group, "Angle");
        instanceTR->setTransform(transform, p1, p2, angle);

        return isR;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::readInstanceM4(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group)
    {

        return false;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::writeAssembly(Interface::FITKAssembly* assembly, H5::Group& h5Group)
    {
        if (!assembly || !_writer) return false;
        bool isW = true;
        isW &= writeNDataObject(assembly, h5Group);
        auto componentManager = assembly->getComponentManager();
        if (!componentManager) return false;

        auto componentGroup = h5Group.createGroup("Components");
        isW &= writeSetSurf(componentManager, componentGroup);

        //set
        //QList<Interface::FITKModelSet*> allSet = componentManager->getAllSet();
        //auto setGroup = h5Group.createGroup("Components");
        //for (int i = 0; i < allSet.count(); ++i)
        //{
        //    isW &= writeSet(allSet[i], setGroup);
        //}
        ////Surface
        //QList<Interface::FITKMeshSurface*> allSurface = componentManager->getAllSurface();
        //for (int i = 0; i < allSurface.count(); ++i)
        //{
        //    isW &= writeSurface(assembly->getComponentManager(), allSurface[i], setGroup);
        //}
        //Instance
        int instanceCount = assembly->getDataCount();
        auto instanceGroup = h5Group.createGroup("Instances");
        for (int i = 0; i < instanceCount; ++i)
        {
            auto instance = assembly->getDataByIndex(i);
            isW &= writeInstance(instance, instanceGroup);
        }
        return isW;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::writeSet(Interface::FITKModelSet * set, H5::Group& h5Group)
    {
        if (!set) return false;

        bool ok = false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKInterfaceHDF5AdaptorSet>("HDF5", set);
        if (adaptor == nullptr) return false;
        adaptor->setFileWriter(_writer);
        adaptor->setDataObject(set);
        adaptor->setH5GroupData(h5Group);
        ok = adaptor->adaptW();
        delete adaptor;
        if (!ok) _writer->consoleMessage(3, QString("Write Assembly Set Error,File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        return ok;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::writeSurface(Interface::FITKModelSurface* surface, H5::Group& h5Group)
    {
        if (!surface) return false;
        //调用surface适配器读写
        bool ok = false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKInterfaceHDF5AdaptorSurface>("HDF5", "Interface::FITKModelSurface");
        if (adaptor == nullptr) return false;
        adaptor->setFileWriter(_writer);
        adaptor->setDataObject(surface);
        adaptor->setH5GroupData(h5Group);
        ok = adaptor->adaptW();
        delete adaptor;
        if (!ok) _writer->consoleMessage(3, QString("Write Assembly Surface Error,File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        return ok;
    }

    bool FITKInterfaceHDF5AdaptorAssembly::writeSetSurf(Interface::FITKComponentManager* setSurfManager, H5::Group& h5Group)
    {
        if (!setSurfManager)return false;
        //创建sets子节点
        int count = setSurfManager->getDataCount();
        bool ok = true;
        for (int i = 0; i < count; ++i)
        {
            QList<Interface::FITKModelSet*> setCombinationList;
            auto data = setSurfManager->getDataByIndex(i);
            Interface::FITKModelSet* set = dynamic_cast<Interface::FITKModelSet*>(data);
            Interface::FITKModelSurface* surface = dynamic_cast<Interface::FITKModelSurface*>(data);

            if (set)
            {
                ok &= writeSet(set, h5Group);
            }
            else if (surface)
            {
                ok &= writeSurface(surface, h5Group);
            }
        }
        return ok;
    }


    bool FITKInterfaceHDF5AdaptorAssembly::writeInstance(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group)
    {
        if (!assInstance || h5Group.getLocId() == -1) return false;
        bool isW = true;

        std::string groupName = createParentAttribute(assInstance, h5Group);
        if (groupName.empty())return false;

        auto instanceGroup = h5Group.createGroup(groupName);
        //type
        auto absModelType = assInstance->getAbsModelType();

        //枚举转换字符 并写出
        Core::FITKEnumTransfer<Interface::FITKModelEnum::AbsModelType> fitkSurfTypeTrafer;
        bool isValid = false;
        QString TypeStr = fitkSurfTypeTrafer.toString(absModelType, isValid);
        if (!isValid) return false;
        writeStrAttribute(instanceGroup, "AbsModelType", TypeStr.toStdString());

        //modelID
        int modelID = assInstance->getModelID();
        writeIntAttribute(instanceGroup, "ModelID", &modelID);

        //component
        //auto nSetGroup = instanceGroup.createGroup("NodeSets");
        //auto elSetGroup = instanceGroup.createGroup("ElementSets");
        //auto cManager = assInstance->getComponentManager();
        //auto allSet = cManager->getAllSet();
        //for (int i = 0; i < allSet.size(); ++i)
        //{
        //    if (allSet[i]->getModelSetType() == Interface::FITKModelEnum::FITKModelSetType::FMSNode)
        //        isW &= writeSet(allSet[i], nSetGroup);
        //    else if (allSet[i]->getModelSetType() == Interface::FITKModelEnum::FITKModelSetType::FMSElem)
        //        isW &= writeSet(allSet[i], elSetGroup);
        //    else return false;
        //}

        //QList<Interface::FITKMeshSurface*> allSurface = cManager->getAllSurface();
        //auto surfacesGroup = instanceGroup.createGroup("Surfaces");
        //for (int i = 0; i < allSurface.count(); ++i)
        //{
        //    isW &= writeSurface(cManager, allSurface[i], surfacesGroup);
        //}
        isW &= writeInstanceTR(assInstance, instanceGroup);
        isW &= writeNDataObject(assInstance, instanceGroup);
        return isW;
    }


    bool FITKInterfaceHDF5AdaptorAssembly::writeInstanceTR(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group)
    {
        auto instanceTR = dynamic_cast<Interface::FITKAbstractAssInstance*>(assInstance);
        if (!instanceTR) return false;
        bool isW = true;

        double tr[3];
        instanceTR->getTransfom(tr);
        writeDoubleAttribute(h5Group, "Transform", &tr, 1, 3);

        double p1[3]{}, p2[3]{};
        double angle = 0;
        bool isRotate = instanceTR->getRotateAxis(p1, p2, &angle);
        writeDoubleAttribute(h5Group, "P1", &p1, 1, 3);
        writeDoubleAttribute(h5Group, "P2", &p2, 1, 3);
        writeDoubleAttribute(h5Group, "Angle", &angle);
        //if (instanceTR->getRotateAxis(p1, p2, &angle))
        //{
        //    writeDoubleAttribute(h5Group, "P1", &p1, 1, 3);
        //    writeDoubleAttribute(h5Group, "P2", &p2, 1, 3);
        //    writeDoubleAttribute(h5Group, "Angle", &angle);
        //}
        return isW;
    }

    //bool FITKInterfaceHDF5AdaptorAssembly::writeInstanceM4(Interface::FITKAbstractAssInstance* assInstance, H5::Group& h5Group)
    //{
    //    auto instanceM4 = dynamic_cast<Interface::FITKAbstractAssInstanceM4*>(assInstance);
    //    if (!instanceM4) return false;

    //    bool isW = true;


    //    return isW;
    //}
}


