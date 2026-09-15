/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataSolutionAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionMultiUsageImpact.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryACCEL.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryINTER.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistoryRWALL.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySECTIO.h"
#include "FITK_Component/FITKRadiossData/FITKTimeHistorySPHCEL.h"

#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataInitialFieldTraAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataGravityAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataSolverSettingsAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataResultRequestAdaptor.h"

namespace Interface
{
    bool FITKRadiossDataSolutionAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKRadiossSolution*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        bool ok = true;

        ok &= readBCS(*_h5Group);
        //读取相互作用（）
        ok &= readInteraction(*_h5Group);
        ok &= readConnection(*_h5Group);
        //读取初始场
        ok &= readInitialField(*_h5Group);
        //读取重力
        ok &= readGravity(*_h5Group);
        //读取刚性墙
        ok &= readRigidWall(*_h5Group);
        ok &= readProbe(*_h5Group);
        ok &= readSolverSetting(*_h5Group);
        ok &= readResultRequest(*_h5Group);
        ok &= readTimeHistory(*_h5Group);
        return ok;
    }

    bool FITKRadiossDataSolutionAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKRadiossSolution*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        bool ok = true;

        ok &= writeBCS(*_h5Group);
        //写出相互作用（）
        ok &= writeInteraction(*_h5Group);
        ok &= writeConnection(*_h5Group);
        //写出初始场
        ok &= writeInitialField(*_h5Group);
        //写出重力
        ok &= writeGravity(*_h5Group);
        //写出刚性墙
        ok &= writeRigidWall(*_h5Group);
        ok &= writeProbe(*_h5Group);
        ok &= writeSolverSetting(*_h5Group);
        ok &= writeResultRequest(*_h5Group);
        ok &= writeTimeHistory(*_h5Group);
        return ok;
    }
 
    bool FITKRadiossDataSolutionAdaptor::readInteraction(H5::Group& g)
    {
        H5::Group group = g.openGroup("Interaction");
        bool isR = true;
        Radioss::FITKInteractionManager* interactionMgr = _data->getInteractionManager();
        if (interactionMgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group interactionGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(interactionGroup, "InteractionType", type);
            Radioss::FITKAbstractInteraction* interaction = nullptr;
            if (type == "/INTER/TYPE2")
                interaction = new Radioss::FITKInteractionKinematicTied();
            else if(type == "/INTER/TYPE7")
                interaction = new Radioss::FITKInteractionMultiUsageImpact();

            if (!interaction) return false;
            interaction->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", interaction);
            if (adaptor == nullptr) {
                isR = false;
                delete interaction;
                continue;
            }
            adaptor->setH5Group(&interactionGroup);
            adaptor->setDataObject(interaction);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                interactionMgr->appendDataObj(interaction);
            }
            else
                delete interaction;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeInteraction(H5::Group& g)
    {
        H5::Group group = g.createGroup("Interaction");
        bool isW = true;
        Radioss::FITKInteractionManager* interactionMgr = _data->getInteractionManager();
        if (interactionMgr == nullptr) return false;
        int count = interactionMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractInteraction* interaction = interactionMgr->getDataByIndex(i);
            if (!interaction) return false;
            QString name = interaction->getDataObjectName();
            H5::Group interactionGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", interaction);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&interactionGroup);
            adaptor->setDataObject(interaction);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

 
    bool FITKRadiossDataSolutionAdaptor::readInitialField(H5::Group& g)
    {
        H5::Group group = g.openGroup("InitialField");
        bool isR = true;
        Radioss::FITKInitialFieldManager* initialFieldMgr = _data->getInitialFieldManager();
        if (initialFieldMgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group initialFieldGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(initialFieldGroup, "InitialFieldKeyWord", type);
            Radioss::FITKAbstractInitialField* initialField = nullptr;
            if (type == "/INIVEL/TRA")
                initialField = new Radioss::FITKInitialFieldTra();

            if (!initialField) return false;
            initialField->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataInitialFieldTraAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataInitialFieldTraAdaptor>("HDF5", initialField);
            if (adaptor == nullptr) {
                isR = false;
                delete initialField;
                continue;
            }
            adaptor->setH5Group(&initialFieldGroup);
            adaptor->setDataObject(initialField);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                initialFieldMgr->appendDataObj(initialField);
            }
            else
                delete initialField;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeInitialField(H5::Group& g)
    {
        H5::Group group = g.createGroup("InitialField");
        bool isW = true;
        Radioss::FITKInitialFieldManager* initialFieldMgr = _data->getInitialFieldManager();
        if (initialFieldMgr == nullptr) return false;
        int count = initialFieldMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractInitialField* initialField = initialFieldMgr->getDataByIndex(i);
            if (!initialField) return false;
            QString name = initialField->getDataObjectName();
            H5::Group initialFieldGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataInitialFieldTraAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataInitialFieldTraAdaptor>("HDF5", initialField);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&initialFieldGroup);
            adaptor->setDataObject(initialField);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataSolutionAdaptor::readGravity(H5::Group& g)
    {
        H5::Group group = g.openGroup("Gravity");
        bool isR = true;
        Radioss::FITKGravityManager* gravityMgr = _data->getGravityManager();
        if (gravityMgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group gravityGroup = group.openGroup(objName);
            
            Radioss::FITKGravity* gravity = new Radioss::FITKGravity();
            if (!gravity) return false;

            gravity->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataGravityAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataGravityAdaptor>("HDF5", gravity);
            if (adaptor == nullptr) {
                isR = false;
                delete gravity;
                continue;
            }
            adaptor->setH5Group(&gravityGroup);
            adaptor->setDataObject(gravity);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                gravityMgr->appendDataObj(gravity);
            }
            else
                delete gravity;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeGravity(H5::Group& g)
    {
        H5::Group group = g.createGroup("Gravity");
        bool isW = true;
        Radioss::FITKGravityManager* gravityMgr = _data->getGravityManager();
        if (gravityMgr == nullptr) return false;
        int count = gravityMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKGravity* gravity = gravityMgr->getDataByIndex(i);
            if (!gravity) return false;
            QString name = gravity->getDataObjectName();
            H5::Group gravityGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataGravityAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataGravityAdaptor>("HDF5", gravity);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&gravityGroup);
            adaptor->setDataObject(gravity);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataSolutionAdaptor::readRigidWall(H5::Group& g)
    {
        H5::Group group = g.openGroup("RigidWall");
        bool isR = true;
        Radioss::FITKRadiossRWallManager* wallMgr = _data->getRWallManager();
        if (wallMgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group wallGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(wallGroup, "RWallKeyWord", type);
            Radioss::FITKAbstractRWall* wall = nullptr;
            if (type == "/RWALL/PLANE")
                wall = new Radioss::FITKRWallPlane();
            else if (type == "/RWALL/CYL")
                wall = new Radioss::FITKRWallCylinder();
            if (!wall) return false;
            wall->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", wall);
            if (adaptor == nullptr) {
                isR = false;
                delete wall;
                continue;
            }
            adaptor->setH5Group(&wallGroup);
            adaptor->setDataObject(wall);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                wallMgr->appendDataObj(wall);
            }
            else
                delete wall;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeRigidWall(H5::Group& g)
    {
        H5::Group group = g.createGroup("RigidWall");
        bool isW = true;
        Radioss::FITKRadiossRWallManager*  wallMgr = _data->getRWallManager();
        if (wallMgr == nullptr) return false;
        int count = wallMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractRWall* wall = wallMgr->getDataByIndex(i);
            if (!wall) return false;
            QString name = wall->getDataObjectName();
            H5::Group wallGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", wall);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&wallGroup);
            adaptor->setDataObject(wall);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }
    bool FITKRadiossDataSolutionAdaptor::readSolverSetting(H5::Group& g)
    {
        H5::Group group = g.openGroup("SolverSetting");
        bool isR = true;
        Radioss::FITKRadiossSolverSettings* solverSettings = _data->getSolverSettings();
        if (!solverSettings)return false;
        Interface::FITKRadiossDataSolverSettingsAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSolverSettingsAdaptor>("HDF5", "Radioss::FITKRadiossSolverSettings");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&group);
        adaptor->setDataObject(solverSettings);
        adaptor->setReader(_reader);
        bool r = adaptor->adaptR();
        isR &= r;
        delete adaptor;
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeSolverSetting(H5::Group& g)
    {
        H5::Group group = g.createGroup("SolverSetting");
        bool isW = true;
        Radioss::FITKRadiossSolverSettings* solverSettings = _data->getSolverSettings();
        if (!solverSettings)return false;
        Interface::FITKRadiossDataSolverSettingsAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSolverSettingsAdaptor>("HDF5", "Radioss::FITKRadiossSolverSettings");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&group);
        adaptor->setDataObject(solverSettings);
        adaptor->setWriter(_writer);
        bool w = adaptor->adaptW();
        isW &= w;
        delete adaptor;
        return isW;
    }

    bool FITKRadiossDataSolutionAdaptor::readResultRequest(H5::Group& g)
    {
        H5::Group group = g.openGroup("ResultRequest");
        bool isR = true;
        Radioss::FITKRadiossResultRequest* resultRequest = _data->getResultRequest();
        if (!resultRequest)return false;
        Interface::FITKRadiossDataResultRequestAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataResultRequestAdaptor>("HDF5", "Radioss::FITKRadiossResultRequest");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&group);
        adaptor->setDataObject(resultRequest);
        adaptor->setReader(_reader);
        bool r = adaptor->adaptR();
        isR &= r;
        delete adaptor;
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeResultRequest(H5::Group& g)
    {
        H5::Group group = g.createGroup("ResultRequest");
        bool isW = true;
        Radioss::FITKRadiossResultRequest* resultRequest = _data->getResultRequest();
        if (!resultRequest)return false;
        Interface::FITKRadiossDataResultRequestAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataResultRequestAdaptor>("HDF5", "Radioss::FITKRadiossResultRequest");
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&group);
        adaptor->setDataObject(resultRequest);
        adaptor->setWriter(_writer);
        bool w = adaptor->adaptW();
        isW &= w;
        delete adaptor;
        return isW;
    }

    bool FITKRadiossDataSolutionAdaptor::readBCS(H5::Group& g)
    {
        H5::Group group = g.openGroup("BCS");
        bool isR = true;
        Radioss::FITKRadiossBCSManager*  mgr = _data->getBCSManager();
        if (mgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group bcsGroup = group.openGroup(objName);
            Radioss::FITKRadiossBCS* bcs = new Radioss::FITKRadiossBCS();
            if (!bcs) return false;
            bcs->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", bcs);
            if (adaptor == nullptr) {
                isR = false;
                delete bcs;
                continue;
            }
            adaptor->setH5Group(&bcsGroup);
            adaptor->setDataObject(bcs);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                mgr->appendDataObj(bcs);
            }
            else
                delete bcs;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeBCS(H5::Group& g)
    {
        H5::Group group = g.createGroup("BCS");
        bool isW = true;
        Radioss::FITKRadiossBCSManager*  mgr = _data->getBCSManager();
        if (mgr == nullptr) return false;
        int count = mgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractBCS* bcs = mgr->getDataByIndex(i);
            if (!bcs) return false;
            QString name = bcs->getDataObjectName();
            H5::Group bcsGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", bcs);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&bcsGroup);
            adaptor->setDataObject(bcs);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataSolutionAdaptor::readConnection(H5::Group& g)
    {
        H5::Group group = g.openGroup("Connection");
        bool isR = true;
        Radioss::FITKConnectionManager*  mgr = _data->getConnectionManager();
        if (mgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group connectionGroup = group.openGroup(objName);
            Radioss::FITKConnectionRBODY* connection = new Radioss::FITKConnectionRBODY();
            if (!connection) return false;
            connection->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", connection);
            if (adaptor == nullptr) {
                isR = false;
                delete connection;
                continue;
            }
            adaptor->setH5Group(&connectionGroup);
            adaptor->setDataObject(connection);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                mgr->appendDataObj(connection);
            }
            else
                delete connection;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeConnection(H5::Group& g)
    {
        H5::Group group = g.createGroup("Connection");
        bool isW = true;
        Radioss::FITKConnectionManager*  mgr = _data->getConnectionManager();
        if (mgr == nullptr) return false;
        int count = mgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractConnection* connection = mgr->getDataByIndex(i);
            if (!connection) return false;
            QString name = connection->getDataObjectName();
            H5::Group connectionGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", connection);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&connectionGroup);
            adaptor->setDataObject(connection);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataSolutionAdaptor::readProbe(H5::Group& g)
    {
        H5::Group group = g.openGroup("Probe");
        bool isR = true;
        Radioss::FITKProbeManager* mgr = _data->getProbeManager();
        if (mgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group probeGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(probeGroup, "ProbeKeyWord", type);
            Radioss::FITKAbstractProbe* probe = nullptr;
            if (type == "/ACCEL")
                probe = new Radioss::FITKProbeAcceleration();
            else if (type == " /SECT")
                probe = new Radioss::FITKProbeSection();
            if (!probe) return false;
            probe->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", probe);
            if (adaptor == nullptr) {
                isR = false;
                delete probe;
                continue;
            }
            adaptor->setH5Group(&probeGroup);
            adaptor->setDataObject(probe);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                mgr->appendDataObj(probe);
            }
            else
                delete probe;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataSolutionAdaptor::writeProbe(H5::Group& g)
    {
        H5::Group group = g.createGroup("Probe");
        bool isW = true;
        Radioss::FITKProbeManager* mgr = _data->getProbeManager();
        if (mgr == nullptr) return false;
        int count = mgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractProbe* probe = mgr->getDataByIndex(i);
            if (!probe) return false;
            QString name = probe->getDataObjectName();
            H5::Group probeGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", probe);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&probeGroup);
            adaptor->setDataObject(probe);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataSolutionAdaptor::readTimeHistory(H5::Group& g)
    {
        H5::Group group = g.openGroup("TimeHistory");
        bool isR = true;
        Radioss::FITKTimeHistoryManager* timeHistoryMgr = _data->getTimeHietoryManager();
        if (timeHistoryMgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group timeHistoryGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(timeHistoryGroup, "TimeHistoryType", type);
            // 根据类型创建相应的时间历史历程对象
            Radioss::FITKAbstractTimeHistory* timeHistory = nullptr;
            if (type == "/TH/ACCEL")
                timeHistory = new Radioss::FITKTimeHistoryACCEL();
            else if (type == "/TH/INTER")
                timeHistory = new Radioss::FITKTimeHistoryINTER();
            else if (type == "/TH/RBODY")
                timeHistory = new Radioss::FITKTimeHistoryRBODY();
            else if (type == "/TH/RWALL")
                timeHistory = new Radioss::FITKTimeHistoryRWALL();
            else if (type == "/TH/SECTIO")
                timeHistory = new Radioss::FITKTimeHistorySECTIO();
            else if (type == "/TH/SPHCEL")
                timeHistory = new Radioss::FITKTimeHistorySPHCEL();

            if (!timeHistory)
            {
                isR = false;
                continue;
            }
            timeHistory->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", timeHistory);
            if (adaptor == nullptr) {
                isR = false;
                delete timeHistory;
                continue;
            }
            adaptor->setH5Group(&timeHistoryGroup);
            adaptor->setDataObject(timeHistory);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                timeHistoryMgr->appendDataObj(timeHistory);
            }
            else
                delete timeHistory;
            delete adaptor;
        }
        group.close();
        return isR;
    }

    bool FITKRadiossDataSolutionAdaptor::writeTimeHistory(H5::Group& g)
    {
        H5::Group group = g.createGroup("TimeHistory");
        bool isW = true;
        Radioss::FITKTimeHistoryManager* timeHistoryMgr = _data->getTimeHietoryManager();
        if (timeHistoryMgr == nullptr) return false;
        int count = timeHistoryMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractTimeHistory* timeHistory = timeHistoryMgr->getDataByIndex(i);
            if (!timeHistory) return false;
            QString name = timeHistory->getDataObjectName();
            H5::Group timeHistoryGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", timeHistory);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&timeHistoryGroup);
            adaptor->setDataObject(timeHistory);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }
}
