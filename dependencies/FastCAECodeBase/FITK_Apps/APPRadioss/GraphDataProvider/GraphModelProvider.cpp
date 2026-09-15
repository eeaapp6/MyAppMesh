/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphModelProvider.h"

#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"
#include "GraphDataVTKAdaptor/GraphVTKViewAdaptorBase.h"

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractAssInstance.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKProbeAcceleration.h"
#include "GraphPostProvider.h"

namespace GraphData
{
    GraphModelProvider::GraphModelProvider(Comp::FITKGraph3DWindowVTK* graphWidget)
        : GraphProviderBase(graphWidget)
    {

    }

    GraphModelProvider::~GraphModelProvider()
    {
        // 析构三维可视化对象。
        deleteObjsHash(_modelObjHash);
        deleteObjsHash(_modelSetObjHash);
        deleteObjsHash(_instanceHash);
    }

    QString GraphModelProvider::getClassName()
    {
        return "GraphModelProvider";
    }

    QList<Exchange::GraphVTKObject3D*> GraphModelProvider::getCurrentGraphObjs()
    {
        // 当前所有模型可视化对象数据。
        QList<Exchange::GraphVTKObject3D*> objs;

        // 模型（几何）可视化对象。
        objs << _modelObjHash.values();

        return objs;
    }

    Exchange::GraphVTKObject3D* GraphModelProvider::getModelGraphObject(int dataObjId)
    {
        // 模型可视化对象。
        Exchange::GraphVTKObject3D* obj = nullptr;

        // 检查数据ID
        Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(dataObjId);
        if (geoCommand) {
            obj = getGeoGraphObject("PartGeo", _modelObjHash, geoCommand);
        }

        return obj;
    }

    Exchange::GraphVTKObject3D * GraphModelProvider::getPartGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        Radioss::FITKRadiossPart* part = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKRadiossPart>(dataObjId);
        if (part)
        {
            obj = getGraphObject("RadiossPart", _meshObjHash, part);
        }
        return obj;
    }

    Exchange::GraphVTKObject3D* GraphModelProvider::getRWallGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        Radioss::FITKRWallPlane* planeWall = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKRWallPlane>(dataObjId);
        Radioss::FITKRWallCylinder* cylWall = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKRWallCylinder>(dataObjId);
        if (planeWall)
        {
            obj = getGraphObject("RadiossPlaneWall", _rWallObjHash, planeWall);
        }
        else if (cylWall)
        {
            obj = getGraphObject("RadiossCylWall", _rWallObjHash, cylWall);
        }
        return obj;
    }
    Exchange::GraphVTKObject3D * GraphModelProvider::getProbeGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        Radioss::FITKProbeSection* section = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKProbeSection>(dataObjId);
        Radioss::FITKProbeAcceleration* accel = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKProbeAcceleration>(dataObjId);
        if (section)
        {
            obj = getGraphObject("RadiossProbeSection", _probeHash, section);
        }
        else if (accel)
        {
            obj = getGraphObject("RadiossProbeAccel", _probeHash, accel);
        }
        return obj;
    }
    Exchange::GraphVTKObject3D* GraphModelProvider::getGravityGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        Radioss::FITKGravity* gravity = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKGravity>(dataObjId);
        if (gravity)
        {
            obj = getGraphObject("RadiossGravity", _gravityObjHash, gravity);
        }
        return obj;
    }

    Exchange::GraphVTKObject3D* GraphModelProvider::getFiledGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        Radioss::FITKAbstractInitialField* field = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKAbstractInitialField>(dataObjId);
        if (field)
        {
            auto type = field->getType();
            if(type == Radioss::FITKAbstractInitialField::InitialFieldType::TRA)
            {
                obj = getGraphObject("RadiossFieldTRA", _fieldObjHash, field);
            }
            else if(type == Radioss::FITKAbstractInitialField::InitialFieldType::AXIS)
            {
                obj = getGraphObject("RadiossFieldAXIS", _fieldObjHash, field);
            }
        }
        return obj;
    }

    Exchange::GraphVTKObject3D* GraphModelProvider::getBCSGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        //检查数据ID
        Radioss::FITKAbstractBCS* bc = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKAbstractBCS>(dataObjId);
        obj = getGraphObject("RadiossBCS", _bcsObjHash, bc);
        return obj;
    }

    Exchange::GraphVTKObject3D* GraphModelProvider::getInteractionGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        //检查数据ID
        Radioss::FITKAbstractInteraction* interaction = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKAbstractInteraction>(dataObjId);
        Radioss::FITKAbstractInteraction::InteractionType type = interaction->getInteractionType();
        if (interaction)
        {
            if(type == Radioss::FITKAbstractInteraction::InteractionType::IT_KinematicTied)
            {
                obj = getGraphObject("RadiossIKT", _interactionObjHash, interaction);
            }
            else if(type == Radioss::FITKAbstractInteraction::InteractionType::IT_MultiUsageImpact)
            {
                obj = getGraphObject("RadiossIMI", _interactionObjHash, interaction);
            }
        }
        return obj;
    }

    Exchange::GraphVTKObject3D* GraphModelProvider::getConnectionGraphObject(int dataObjId)
    {
        Exchange::GraphVTKObject3D* obj = nullptr;
        //检查数据ID
        Radioss::FITKAbstractConnection* connection = Core::FITKDataRepo::getInstance()->getTDataByID<Radioss::FITKAbstractConnection>(dataObjId);
        Radioss::FITKAbstractConnection::ConnectionType type = connection->getType();
        if (connection)
        {
            if (type == Radioss::FITKAbstractConnection::ConnectionType::Connection_RBODY)
            {
                obj = getGraphObject("RadiossRBODY", _connectionObjHash, connection);
            }
        }
        return obj;
    }

    Exchange::GraphVTKObject3D * GraphModelProvider::getModelSetGraphObject(int dataObjId, int parentId)
    {
        // 模型可视化对象。
        Exchange::GraphVTKObject3D* obj = nullptr;

        // 检查数据ID。
        Interface::FITKModelSet* modelSet = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKModelSet>(dataObjId);
        if (!modelSet) {
            return obj;
        }

        // 判断数据是否已经存在
        if (_modelSetObjHash.keys().contains(dataObjId)) {
            if (_modelSetObjHash.value(dataObjId).keys().contains(parentId)) {
                return _modelSetObjHash.value(dataObjId).value(parentId);
            }
        }

        // 模型可视化对象。
        obj = getGraphObjectBySet("ModelSet", modelSet, parentId);

        //存储渲染对象
        QHash<int, Exchange::GraphVTKObject3D*> objs = _modelSetObjHash.value(dataObjId);
        objs.insert(parentId, obj);
        _modelSetObjHash.insert(dataObjId, objs);

        //检测数据析构对三维数据进行析构并移出数据管理。
        connect(modelSet, &Core::FITKAbstractDataObject::dataObjectDestoried, this, [=](Core::FITKAbstractDataObject* objDelete)
            {
                Q_UNUSED(objDelete);
                modelSetDeleteEvent(parentId, dataObjId);
            });

        return obj;
    }

    Exchange::GraphVTKObject3D * GraphModelProvider::getInstanceGraphObject(int dataObjId)
    {
        // 模型可视化对象。
        Exchange::GraphVTKObject3D* obj = nullptr;

        // 检查数据ID。
        Interface::FITKAbstractAssInstance* instance = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKAbstractAssInstance>(dataObjId);
        if (!instance) {
            return obj;
        }

        // 模型可视化对象。
        obj = getGraphObject("Instance", _instanceHash, instance);

        return obj;
    }

    Exchange::GraphVTKObject3D * GraphModelProvider::getGraphObjSetOrSurface(int dataObjId, int modeId)
    {
        // 模型可视化对象。
        Exchange::GraphVTKObject3D* obj = nullptr;

        Interface::FITKModelSet* set = FITKDATAREPO->getTDataByID<Interface::FITKModelSet>(dataObjId);
        Interface::FITKMeshSurfaceElement* meshSurfEle = FITKDATAREPO->getTDataByID<Interface::FITKMeshSurfaceElement>(dataObjId);
        Interface::FITKMeshSurfaceNode* meshSurfNode = FITKDATAREPO->getTDataByID<Interface::FITKMeshSurfaceNode>(dataObjId);
        if (set) {
            // 检查数据ID。
            Interface::FITKAbstractAssInstance* instance = FITKDATAREPO->getTDataByID<Interface::FITKAbstractAssInstance>(modeId);
            if (instance == nullptr) {
                modeId = set->getAbsModelID();
            }
            if(set->getModelSetType() == Interface::FITKModelEnum::FMSNode)
            {
                obj = getSetOrSurfGraphObject("SetNode", _setSurfObjHash, set, modeId);
            }
            else if(set->getModelSetType() == Interface::FITKModelEnum::FMSElem)
            {
                obj = getSetOrSurfGraphObject("SetEle", _setSurfObjHash, set, modeId);
            }
        }
        else if (meshSurfEle) {
            // 检查数据ID。
            Interface::FITKAbstractAssInstance* instance = FITKDATAREPO->getTDataByID<Interface::FITKAbstractAssInstance>(modeId);
            if (instance == nullptr) {
                modeId = meshSurfEle->getAbsModelID();
            }
            //检查是否为SurfaceSeg
            Radioss::FITKRadiossSurfaceSeg* seg = FITKDATAREPO->getTDataByID<Radioss::FITKRadiossSurfaceSeg>(dataObjId);
            if (seg)
            {
                obj = getSetOrSurfGraphObject("SurfEleSeg", _setSurfObjHash, seg, modeId);
            }
            // 模型可视化对象。
            obj = getSetOrSurfGraphObject("SurfEle", _setSurfObjHash, meshSurfEle, modeId);
        }
        else if (meshSurfNode) {
            // 检查数据ID。
            Interface::FITKAbstractAssInstance* instance = FITKDATAREPO->getTDataByID<Interface::FITKAbstractAssInstance>(modeId);
            if (instance == nullptr) {
                modeId = meshSurfNode->getAbsModelID();
            }

            // 模型可视化对象。
            obj = getSetOrSurfGraphObject("SurfNode", _setSurfObjHash, meshSurfNode, modeId);
        }
        return obj;
    }

    QList<Exchange::GraphVTKObject3D*> GraphModelProvider::getAllGeoGraphObjects()
    {
        return _modelObjHash.values();
    }

    QList<Exchange::GraphVTKObject3D*> GraphModelProvider::getAllMeshGraphObjects()
    {
        return _meshObjHash.values();
    }

    QList<Exchange::GraphVTKObject3D*> GraphModelProvider::getAllInstanceGraphObjects()
    {
        return _instanceHash.values();
    }

    QHash<int, QHash<int, Exchange::GraphVTKObject3D*>> GraphModelProvider::getAllModelSetGraphObjects()
    {
        return _modelSetObjHash;
    }

    QList<Exchange::GraphVTKObject3D*> GraphModelProvider::getAllGraphObjects()
    {
        QList<Exchange::GraphVTKObject3D*> objs;
        objs.append(_modelObjHash.values());
        objs.append(_meshObjHash.values());
        objs.append(_instanceHash.values());
        objs.append(_rWallObjHash.values());
        objs.append(_gravityObjHash.values());
        objs.append(_fieldObjHash.values());
        objs.append(_bcsObjHash.values());
        objs.append(_connectionObjHash.values()); 
        objs.append(_interactionObjHash.values());
        objs.append(_probeHash.values());
        for (int key : _setSurfObjHash.keys()) {
            objs.append(_setSurfObjHash.value(key).values());
        }

        for (QHash<int, Exchange::GraphVTKObject3D*> setHash : _modelSetObjHash.values()) {
            objs << setHash.values();
        }

        return objs;
    }

    bool GraphModelProvider::updateObjById(int dataObjId, QVariant info)
    {
        // 参数预留。
        Q_UNUSED(info);

        if (!_modelObjHash.contains(dataObjId))
        {
            return false;
        }

        // 获取可视化对象并更新。
        Core::FITKAbstractGraphObject* obj = _modelObjHash[dataObjId];
        if (!obj)
        {
            return false;
        }

        obj->update();

        return true;
    }

    void GraphModelProvider::setGeoVertPickable(int dataObjId)
    {
        // 开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _modelObjHash.values();
        objs << _instanceHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (!obj)
            {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1)
            {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else
            {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickGeoVertex);
            }
        }
    }

    void GraphModelProvider::setGeoEdgePickable(int dataObjId)
    {
        // 开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _modelObjHash.values();
        objs << _instanceHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (!obj) {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickGeoEdge);
            }
        }
    }

    void GraphModelProvider::setGeoFacePickable(int dataObjId)
    {
        // 开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _modelObjHash.values();
        objs << _instanceHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (!obj) {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickGeoFace);
            }
        }
    }

    void GraphModelProvider::setGeoSolidPickable(int dataObjId)
    {
        // 开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _modelObjHash.values();
        objs << _instanceHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (!obj) {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickGeoSolid);
            }
        }
    }

    void GraphModelProvider::setMeshVerPickable(int dataObjId)
    {
        //开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _meshObjHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickMeshVertex);
            }
        }
    }

    void GraphModelProvider::setMeshVerSurfPickable(int dataObjId)
    {
        //开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _meshObjHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickMeshVertexSurf);
            }
        }
    }

    void GraphModelProvider::setMeshCellPickable(int dataObjId)
    {
        // 开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _meshObjHash.values();
        objs << _instanceHash.values();
        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (obj == nullptr) {
                continue;
            }
            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickMeshCell);
            }
        }
    }

    void GraphModelProvider::setMeshCellSurfPickable(int dataObjId)
    {
        //开启可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _meshObjHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs) {
            if (obj == nullptr) {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickMeshCellSurf);
            }
        }
    }

    void GraphModelProvider::setNonePickable(int dataObjId)
    {
        // 关闭可拾取状态。
        QList<Exchange::GraphVTKObject3D*> objs = {};
        objs << _modelObjHash.values();
        objs << _instanceHash.values();
        objs << _meshObjHash.values();

        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (!obj) {
                continue;
            }

            if (obj->getDataId() != dataObjId && dataObjId != -1) {
                // 其他数据不取消拾取。
                continue;
            }
            else {
                obj->setPickMode(Exchange::GraphVTKCommons::ShapePickMode::PickNone);
            }
        }
    }

    void GraphModelProvider::modelSetDeleteEvent(int parentId, int dataObjId)
    {
        if (_modelSetObjHash.contains(dataObjId)) {
            Exchange::GraphVTKObject3D* graphObj1 = _modelSetObjHash[dataObjId].take(parentId);
            if (graphObj1) {
                delete graphObj1;
                graphObj1 = nullptr;
            }
        }
    }
}   // namespace GraphData