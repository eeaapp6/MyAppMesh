/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataCaseAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKPropAbstract.hpp"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodeGroup.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasJohns.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasTab.h"
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"
#include "FITK_Component/FITKRadiossData/FITKInteractionTied.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialPlasBrit.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialHydro.h"
#include "FITK_Component/FITKRadiossData/FITKPropSPH.h"
#include "FITK_Component/FITKRadiossData/FITKPropSandwichShell.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKEquationOfStatePolynomial.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelBIQUAD.h"
#include "FITK_Component/FITKRadiossData/FITKFailureModelJOHNSON.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeam.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossGeomPart.h"

#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataPartAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataNodeAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataSetAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataSubSetAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataSurfaceAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataNonLinerCurveAdaptor.h"
#include "FITK_Component/FITKRadiossDataIOHDF5/FITKRadiossDataGeomPartAdaptor.h"

namespace Interface
{
    bool FITKRadiossDataCaseAdaptor::adaptR()
    {
        _caseData = Radioss::FITKRadiossCase::SafeDownCastFITKRadiossCase(_dataObj);
        if (_reader == nullptr || _caseData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        _caseData->clearData();
        if (!readDataObject(_caseData, *_h5Group)) return false;
        bool ok = true;
        ok &= readGeomPart(*_h5Group);
        //写出材料(材料要在Part 前面)
        ok &= readMaterials(*_h5Group);
        //写出属性(截面)(属性要在Part 前面)
        ok &= readProperty(*_h5Group);
        //读取状态方程
        ok &= readEquationOfState(*_h5Group);
        //写出曲线(曲线要在重力，材料(/MAT/PLAS_TAB)，属性(/PROP/SPR_BEAM)前面)
        ok &= readCurve(*_h5Group);
        //读取节点
        ok &= readNode(*_h5Group);
        //读取part（Part要在surf前面）
        ok &= readPart(*_h5Group);
        //读取节点组(节点组要在初始场，重力，刚性墙，边界，相互作用前面)
        ok &= readNodeSet(*_h5Group);
        //读取子集
        ok &= readSubSet(*_h5Group);
        //读取surface（surface要在相互作用前面）
        ok &= readSurface(*_h5Group);
        ok &= readFailureModel(*_h5Group);
        ok &= readSolution(*_h5Group);
        return ok;
    }

    bool FITKRadiossDataCaseAdaptor::adaptW()
    {
        _caseData = Radioss::FITKRadiossCase::SafeDownCastFITKRadiossCase(_dataObj);
        if (_writer == nullptr || _caseData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeDataObject(_caseData, *_h5Group))return false;
        bool ok = true;
        ok &= writeGeomPart(*_h5Group);
        //写出材料(材料要在Part 前面)
        ok &= writeMaterials(*_h5Group);
        //写出属性(截面)(属性要在Part 前面)
        ok &= writeProperty(*_h5Group);
        //写出状态方程
        ok &= writeEquationOfState(*_h5Group);
        //写出曲线(曲线要在重力，材料(/MAT/PLAS_TAB)，属性(/PROP/SPR_BEAM)前面)
        ok &= writeCurve(*_h5Group);
        //写出节点
        ok &= writeNode(*_h5Group);
        //写出part（Part要在surf前面）
        ok &= writePart(*_h5Group);
        //写出节点组(节点组要在初始场，重力，刚性墙，边界，相互作用前面)
        ok &= writeNodeSet(*_h5Group);
        //写出子集
        ok &= writeSubSet(*_h5Group);
        //写出surface（surface要在相互作用前面）
        ok &= writeSurface(*_h5Group);
        ok &= writeFailureModel(*_h5Group);
        ok &= writeSolution(*_h5Group);
        return ok;
    }

    bool FITKRadiossDataCaseAdaptor::readGeomPart(H5::Group& g)
    {
        H5::Group group = g.openGroup("GeomPart");
        Radioss::FITKRadiossGeomPartManager* geomPartMgr = _caseData->getGeomPartManager();
        if (!geomPartMgr)return false;
        bool isR = true;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group partGroup = group.openGroup(objName);
            Radioss::FITKRadiossGeomPart* part = new Radioss::FITKRadiossGeomPart();
            //part->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataGeomPartAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataGeomPartAdaptor>("HDF5", part);
            if (adaptor == nullptr) {
                isR = false;
                delete part;
                continue;
            }
            adaptor->setH5Group(&partGroup);
            adaptor->setDataObject(part);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
                geomPartMgr->appendDataObj(part);
            else
                delete part;
            delete adaptor;
        }
        return isR;
    }
    bool FITKRadiossDataCaseAdaptor::writeGeomPart(H5::Group& g)
    {
        H5::Group group = g.createGroup("GeomPart");
        bool isW = true;
        Radioss::FITKRadiossGeomPartManager* geomPartMgr = _caseData->getGeomPartManager();
        if (!geomPartMgr)return false;
        int count = geomPartMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKRadiossGeomPart* part = geomPartMgr->getDataByIndex(i);
            if (!part) return false;
            //QString name = part->getDataObjectName();
            H5::Group partGroup = group.createGroup(QString::number(i).toStdString());
            Interface::FITKRadiossDataGeomPartAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataGeomPartAdaptor>("HDF5", part);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&partGroup);
            adaptor->setDataObject(part);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readPart(H5::Group& g)
    {
        H5::Group group = g.openGroup("Part");
        bool isReadSuccess = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel)return false;
        Radioss::FITKRadiossPartManager* radiossPartManager = meshModel->getPartsManager();
        if (!radiossPartManager)return false;
        bool isR = true;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group partGroup = group.openGroup(objName);
            Radioss::FITKRadiossPart* part = new Radioss::FITKRadiossPart();
            part->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataPartAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataPartAdaptor>("HDF5", part);
            if (adaptor == nullptr) {
                isR = false;
                delete part;
                continue;
            }
            adaptor->setH5Group(&partGroup);
            adaptor->setDataObject(part);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r) 
                radiossPartManager->appendDataObj(part);
            else 
                delete part;
            delete adaptor;
        }
        Radioss::FITKRadiossMeshTopo* meshTopo = meshModel->getMeshTopo();
        if(meshTopo)
            meshTopo->updateAllMeshTopo();
        return isR;
    }

    bool FITKRadiossDataCaseAdaptor::writePart(H5::Group& g)
    {
        H5::Group group =g.createGroup("Part");
        bool isW = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel)return false;
        Radioss::FITKRadiossPartManager* radiossPartManager = meshModel->getPartsManager();
        if (!radiossPartManager) return false;
        int count = radiossPartManager->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKRadiossPart* part = radiossPartManager->getDataByIndex(i);
            if (!part) return false;
            QString name = part->getDataObjectName();
            H5::Group partGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataPartAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataPartAdaptor>("HDF5", part);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&partGroup);
            adaptor->setDataObject(part);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readNode(H5::Group& g)
    {
        H5::Group group = g.openGroup("Node");
        bool isR = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel)return false;
        Radioss::FITKRadiossNodes* radiossNodes = meshModel->getNodes();
        if (!radiossNodes)return false;
        Interface::FITKRadiossDataNodeAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataNodeAdaptor>("HDF5", radiossNodes);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&group);
        adaptor->setDataObject(radiossNodes);
        adaptor->setReader(_reader);
        bool r = adaptor->adaptR();
        isR &= r;
        delete adaptor;
        group.close();
        return isR;
    }

    bool FITKRadiossDataCaseAdaptor::writeNode(H5::Group& g)
    {
        H5::Group group = g.createGroup("Node");
        bool isW = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel)return false;
        Radioss::FITKRadiossNodes* radiossNodes = meshModel->getNodes();
        if (!radiossNodes)return false;
        Interface::FITKRadiossDataNodeAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataNodeAdaptor>("HDF5", radiossNodes);
        if (adaptor == nullptr) return false;
        adaptor->setH5Group(&group);
        adaptor->setDataObject(radiossNodes);
        adaptor->setWriter(_writer);
        bool w = adaptor->adaptW();
        isW &= w;
        delete adaptor;
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readNodeSet(H5::Group& g)
    {
        H5::Group group = g.openGroup("Set");
        bool isR = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel) return false;
        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (!componentManager) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group setGroup = group.openGroup(objName);
            Radioss::FITKRadiossNodeGroup* set = new Radioss::FITKRadiossNodeGroup();
            set->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataSetAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSetAdaptor>("HDF5", set);
            if (adaptor == nullptr) {
                isR = false;
                delete set;
                continue;
            }
            adaptor->setH5Group(&setGroup);
            adaptor->setDataObject(set);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                //set->setModel(meshModel->getDataObjectID());
                componentManager->appendDataObj(set);
            }
            else
                delete set;
            delete adaptor;
        }
        group.close();
        return isR;
    }

    bool FITKRadiossDataCaseAdaptor::writeNodeSet(H5::Group& g)
    {
        H5::Group group = g.createGroup("Set");
        bool isW = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel)return false;
        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (!componentManager)return false;
        QList<FITKModelSet*> setList = componentManager->getAllSet(false);
        int count = setList.size();
        for (Interface::FITKModelSet* set: setList)
        {
            if (!set) return false;
            Radioss::FITKRadiossNodeGroup* nodeGroup = dynamic_cast<Radioss::FITKRadiossNodeGroup*>(set);
            if (!nodeGroup) return false;
            QString name = nodeGroup->getDataObjectName();
            H5::Group setGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataSetAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSetAdaptor>("HDF5", nodeGroup);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&setGroup);
            adaptor->setDataObject(nodeGroup);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readSubSet(H5::Group& g)
    {
        H5::Group group = g.openGroup("SubSet");
        bool isR = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel) return false;
        Radioss::FITKRadiossSubSetManager* subSetManager = meshModel->getSubSetManager();
        if (!subSetManager)return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group subSetGroup = group.openGroup(objName);
            Radioss::FITKRadiossSubSet* subSet = new Radioss::FITKRadiossSubSet();
            subSet->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataSubSetAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSubSetAdaptor>("HDF5", subSet);
            if (adaptor == nullptr) {
                isR = false;
                delete subSet;
                continue;
            }
            adaptor->setH5Group(&subSetGroup);
            adaptor->setDataObject(subSet);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                subSetManager->appendDataObj(subSet);
            }
            else
                delete subSet;
            delete adaptor;
        }
        group.close();
        return isR;
    }

    bool FITKRadiossDataCaseAdaptor::writeSubSet(H5::Group& g)
    {
        H5::Group group = g.createGroup("SubSet");
        bool isW = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel)return false;
        Radioss::FITKRadiossSubSetManager* subSetManager = meshModel->getSubSetManager();
        if (!subSetManager)return false;
        int count = subSetManager->getDataCount();
        for (int i=0;i<count;i++)
        {
            Radioss::FITKRadiossSubSet* subSet = subSetManager->getDataByIndex(i);
            if (!subSet) return false;
            QString name = subSet->getDataObjectName();
            H5::Group subSetGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataSubSetAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSubSetAdaptor>("HDF5", subSet);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&subSetGroup);
            adaptor->setDataObject(subSet);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readSurface(H5::Group& g)
    {
        H5::Group group = g.openGroup("Surface");
        bool isR = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel) return false;
        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (!componentManager) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group surfaceGroup = group.openGroup(objName);
            Radioss::FITKRadiossSurfaceSeg* surfaceSeg = new Radioss::FITKRadiossSurfaceSeg();
            surfaceSeg->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataSurfaceAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSurfaceAdaptor>("HDF5", surfaceSeg);
            if (adaptor == nullptr) {
                isR = false;
                delete surfaceSeg;
                continue;
            }
            adaptor->setH5Group(&surfaceGroup);
            adaptor->setDataObject(surfaceSeg);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                surfaceSeg->setModel(meshModel->getDataObjectID());
                componentManager->appendDataObj(surfaceSeg);
            }
            else
                delete surfaceSeg;
            delete adaptor;
        }
        group.close();
        return isR;
    }

    bool FITKRadiossDataCaseAdaptor::writeSurface(H5::Group& g)
    {
        H5::Group group = g.createGroup("Surface");
        bool isW = true;
        Radioss::FITKRadiossMeshModel* meshModel = _caseData->getMeshModel();
        if (!meshModel)return false;
        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (!componentManager)return false;
        QList<FITKMeshSurface*> surfaceList = componentManager->getAllMeshSurface();
        int count = surfaceList.size();
        for (Interface::FITKMeshSurface* surface : surfaceList)
        {
            if (!surface) return false;
            Radioss::FITKRadiossSurfaceSeg* surfaceSeg = dynamic_cast<Radioss::FITKRadiossSurfaceSeg*>(surface);
            if (!surfaceSeg) return false;
            QString name = surfaceSeg->getDataObjectName();
            H5::Group surfaceGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataSurfaceAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataSurfaceAdaptor>("HDF5", surfaceSeg);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&surfaceGroup);
            adaptor->setDataObject(surfaceSeg);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readMaterials(H5::Group& g)
    {
        H5::Group group = g.openGroup("Materials");
        bool isR = true;
        Interface::FITKMaterialManager* materialsMgr = _caseData->getMaterialManager();
        if (materialsMgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group materialGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(materialGroup, "MaterialType", type);
            Interface::FITKAbstractMaterial* material = nullptr;
            if (type == "/MAT/LAW0")
                material = new Radioss::FITKMaterialVoid();
            else if (type == "/MAT/LAW1")
                material = new Radioss::FITKMaterialElastic();
            else if (type == "/MAT/LAW2")
                material = new Radioss::FITKMaterialPlasJohns();
            else if (type == "/MAT/LAW36")
                material = new Radioss::FITKMaterialPlasTab();
            else if (type == "/MAT/LAW27")
                material = new Radioss::FITKMaterialPlasBrit();
            else if (type == "/MAT/LAW6")
                material = new Radioss::FITKMaterialHydro();
            //else if (type == "/FAIL/BIQUAD")
            //    material = new Radioss::FITKMaterialFailBiquad();

            if (!material) return false;
            material->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", material);
            if (adaptor == nullptr) {
                isR = false;
                delete material;
                continue;
            }
            adaptor->setH5Group(&materialGroup);
            adaptor->setDataObject(material);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                materialsMgr->appendDataObj(material);
            }
            else
                delete material;
            delete adaptor;
        }
        group.close();
        return isR;
    }

    bool FITKRadiossDataCaseAdaptor::writeMaterials(H5::Group& g)
    {
        H5::Group group = g.createGroup("Materials");
        bool isW = true;
        Interface::FITKMaterialManager* materialsMgr = _caseData->getMaterialManager();
        if (materialsMgr == nullptr) return false;
        int count = materialsMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Interface::FITKAbstractMaterial* material = materialsMgr->getDataByIndex(i);
            if (!material) return false;
            QString name = material->getDataObjectName();
            H5::Group materialGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", material);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&materialGroup);
            adaptor->setDataObject(material);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readProperty(H5::Group& g)
    {
        H5::Group group = g.openGroup("Prop");
        bool isR = true;
        Interface::FITKAbstractSectionManager* propMgr = _caseData->getSectionManager();
        if (propMgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group sectionlGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(sectionlGroup, "PropertyType", type);
            Interface::FITKAbstractSection* section = nullptr;
            if (type == "/PROP/TYPE1")
                section = new Radioss::FITKPropShell();
            else if (type == "/PROP/TYPE14")
                section = new Radioss::FITKPropSolid();
            else if (type == "/PROP/TYPE13")
                section = new Radioss::FITKPropBeamSPR();
            else if (type == "/PROP/TYPE34")
                section = new Radioss::FITKPropSPH();
            else if (type == "/PROP/TYPE11")
                section = new Radioss::FITKPropSandwichShell();
            else if (type == "/PROP/TYPE3")
                section = new Radioss::FITKPropBeam();

            if (!section) return false;
            section->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", section);
            if (adaptor == nullptr) {
                isR = false;
                delete section;
                continue;
            }
            adaptor->setH5Group(&sectionlGroup);
            adaptor->setDataObject(section);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                propMgr->appendDataObj(section);
            }
            else
                delete section;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataCaseAdaptor::writeProperty(H5::Group& g)
    {
        H5::Group group = g.createGroup("Prop");
        bool isW = true;
        Interface::FITKAbstractSectionManager* propMgr = _caseData->getSectionManager();
        if (propMgr == nullptr) return false;
        int count = propMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Interface::FITKAbstractSection* section = propMgr->getDataByIndex(i);
            if (!section) return false;
            QString name = section->getDataObjectName();
            H5::Group sectionGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", section);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&sectionGroup);
            adaptor->setDataObject(section);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readCurve(H5::Group& g)
    {
        H5::Group group = g.openGroup("Curve");
        bool isR = true;
        Radioss::FITKNonLinerCurveManager*  curveMgr = _caseData->getNonLinerCurveManager();
        if (curveMgr == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group curveGroup = group.openGroup(objName);
            Radioss::FITKNonLinerCurve* curve = new Radioss::FITKNonLinerCurve();
            curve->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataNonLinerCurveAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataNonLinerCurveAdaptor>("HDF5", curve);
            if (adaptor == nullptr) {
                isR = false;
                delete curve;
                continue;
            }
            adaptor->setH5Group(&curveGroup);
            adaptor->setDataObject(curve);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                curveMgr->appendDataObj(curve);
            }
            else
                delete curve;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataCaseAdaptor::writeCurve(H5::Group& g)
    {
        H5::Group group = g.createGroup("Curve");
        bool isW = true;
        Radioss::FITKNonLinerCurveManager*  curveMgr = _caseData->getNonLinerCurveManager();
        if (curveMgr == nullptr) return false;
        int count = curveMgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKNonLinerCurve* curve = curveMgr->getDataByIndex(i);
            if (!curve) return false;
            QString name = curve->getDataObjectName();
            H5::Group curveGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataNonLinerCurveAdaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataNonLinerCurveAdaptor>("HDF5", curve);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&curveGroup);
            adaptor->setDataObject(curve);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readEquationOfState(H5::Group& g)
    {
        H5::Group group = g.openGroup("EquationOfState");
        bool isR = true;
        Radioss::FITKEquationOfStateManager* mgr = _caseData->getEquationOfState();
        if (mgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group equationGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(equationGroup, "EOSType", type);
            Radioss::FITKAbstractEquationOfState* equationOfState = nullptr;
            if (type == "/EOS/POLYNOMIAL")
                equationOfState = new Radioss::FITKEquationOfStatePolynomial();

            if (!equationOfState) return false;
            equationOfState->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", equationOfState);
            if (adaptor == nullptr) {
                isR = false;
                delete equationOfState;
                continue;
            }
            adaptor->setH5Group(&equationGroup);
            adaptor->setDataObject(equationOfState);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                mgr->appendDataObj(equationOfState);
            }
            else
                delete equationOfState;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataCaseAdaptor::writeEquationOfState(H5::Group& g)
    {
        H5::Group group = g.createGroup("EquationOfState");
        bool isW = true;
        Radioss::FITKEquationOfStateManager* mgr = _caseData->getEquationOfState();
        if (mgr == nullptr) return false;
        int count = mgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractEquationOfState* equationOfState = mgr->getDataByIndex(i);
            if (!equationOfState) return false;
            QString name = equationOfState->getDataObjectName();
            H5::Group equationGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", equationOfState);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&equationGroup);
            adaptor->setDataObject(equationOfState);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readSolution(H5::Group& g)
    {
        H5::Group group = g.openGroup("Solution");
        bool isR = true;
        Radioss::FITKRadiossSolutionManager* mgr = _caseData->getSolutionManager();
        if (mgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group solutionGroup = group.openGroup(objName);

            Radioss::FITKRadiossSolution* solution = new Radioss::FITKRadiossSolution();
            if (!solution) return false;

            solution->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", solution);
            if (adaptor == nullptr) {
                isR = false;
                delete solution;
                continue;
            }
            adaptor->setH5Group(&solutionGroup);
            adaptor->setDataObject(solution);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                mgr->appendDataObj(solution);
            }
            else
                delete solution;
            delete adaptor;
        }
        int currentSolutionID = -1;
        if (!h5tools->readGroupint(group, "currentSolutionID", &currentSolutionID)) return false;
        mgr->setCurrentSolution(currentSolutionID);
        group.close();
        return isR;
    }
    bool FITKRadiossDataCaseAdaptor::writeSolution(H5::Group& g)
    {
        H5::Group group = g.createGroup("Solution");
        bool isW = true;
        Radioss::FITKRadiossSolutionManager* mgr = _caseData->getSolutionManager();
        if (mgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        int currentSolutionID = mgr->getCurrentSolutionID();
        h5tools->writeGroupAttrInt(group, "currentSolutionID", currentSolutionID);
        int count = mgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKRadiossSolution* solution = mgr->getDataByIndex(i);
            if (!solution) return false;
            QString name = solution->getDataObjectName();
            H5::Group solutionGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", solution);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&solutionGroup);
            adaptor->setDataObject(solution);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }

    bool FITKRadiossDataCaseAdaptor::readFailureModel(H5::Group& g)
    {
        H5::Group group = g.openGroup("FailureModel");
        bool isR = true;
        Radioss::FITKRadiossFailureModelManager* mgr = _caseData->getFailureModelManager();
        if (mgr == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        // 获取所有子组
        hsize_t numObjs = group.getNumObjs();
        for (hsize_t i = 0; i < numObjs; i++)
        {
            // 获取子组名称
            std::string objName = group.getObjnameByIdx(i);
            H5::Group failureModelGroup = group.openGroup(objName);
            std::string type{};
            h5tools->readGroupStr(failureModelGroup, "FailureModelType", type);
            Radioss::FITKAbstractFailureModel* failureModel = nullptr;
            if (type == "/FAIL/BIQUAD")
                failureModel = new Radioss::FITKFailureModelBIQUAD();
            else if (type == "/FAIL/JOHNSON")
                failureModel = new Radioss::FITKFailureModelJOHNSON();

            if (!failureModel) return false;
            failureModel->setDataObjectName(QString::fromStdString(objName));
            // 创建并配置适配器
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", failureModel);
            if (adaptor == nullptr) {
                isR = false;
                delete failureModel;
                continue;
            }
            adaptor->setH5Group(&failureModelGroup);
            adaptor->setDataObject(failureModel);
            adaptor->setReader(_reader);
            bool r = adaptor->adaptR();
            isR &= r;
            if (r)
            {
                mgr->appendDataObj(failureModel);
            }
            else
                delete failureModel;
            delete adaptor;
        }
        group.close();
        return isR;
    }
    bool FITKRadiossDataCaseAdaptor::writeFailureModel(H5::Group& g)
    {
        H5::Group group = g.createGroup("FailureModel");
        bool isW = true;
        Radioss::FITKRadiossFailureModelManager* mgr = _caseData->getFailureModelManager();
        if (mgr == nullptr) return false;
        int count = mgr->getDataCount();
        for (int i = 0; i < count; i++)
        {
            Radioss::FITKAbstractFailureModel* failureModel = mgr->getDataByIndex(i);
            if (!failureModel) return false;
            QString name = failureModel->getDataObjectName();
            H5::Group failureModelGroup = group.createGroup(name.toStdString());
            Interface::FITKRadiossDataHDF5Adaptor* adaptor = FITKIOADAPTORFACTORY->createT<Interface::FITKRadiossDataHDF5Adaptor>("HDF5", failureModel);
            if (adaptor == nullptr) return false;
            adaptor->setH5Group(&failureModelGroup);
            adaptor->setDataObject(failureModel);
            adaptor->setWriter(_writer);
            bool w = adaptor->adaptW();
            isW &= w;
            delete adaptor;
        }
        return isW;
    }
}
