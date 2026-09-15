/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphModelProvider.h"

// Graph widget
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"

// Graph
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"

// Adaptor
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCViewAdaptorBase.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoSketch2D.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

namespace GraphData
{
    GraphModelProvider::GraphModelProvider(Render::FITKGraph3DWindowOCC* graphWidget)
        : GraphProviderBase(graphWidget)
    {

    }

    GraphModelProvider::~GraphModelProvider()
    {
        // 析构三维可视化对象。
        deleteObjsHash(m_modelObjHash);
        deleteObjsHash(m_previewObjHash);
        deleteObjsHash(m_datumObjHash);
        deleteObjsHash(m_sketchObjHash);
        deleteObjsHash(m_componentObjHash);    }

    QString GraphModelProvider::getClassName()
    {
        return "GraphModelProvider";
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphModelProvider::getCurrentGraphObjs()
    {
        // 当前所有可视化对象数据。
        QList<Exchange::FITKOCCGraphObject3D*> objs;

        // 模型（几何）可视化对象。
        objs << m_modelObjHash.values();

        // 基准元素可视化对象。
        objs << m_datumObjHash.values();

        // 2D草图可视化对象。
        objs << m_sketchObjHash.values();

        // 全局几何分组可视化对象。
        objs << m_componentObjHash.values();
            
        return objs;
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getModelGraphObject(int dataObjId)
    {
        // 模型可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };
        if (!m_geoData)
        {
            return obj;
        }

        // 检查数据ID。
        Interface::FITKAbsGeoCommand* model = m_geoData->getDataByID(dataObjId);
        Interface::FITKAbsGeoCommand* modelUndoed = m_geoData->getRedoList()->getDataByID(dataObjId);
        if (!model && !modelUndoed)
        {
            return obj;
        }

        // 模型可视化对象。
        obj = getGraphObject("GeoModelShapeOCC", m_modelObjHash, model == nullptr ? modelUndoed->getShapeAgent() : model->getShapeAgent());

        return obj;
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getModelPreviewGraphObject(int dataObjId)
    {
        // 模型可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        // 检查数据ID。
        Interface::FITKAbsGeoCommand* model = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(dataObjId);
        if (!model)
        {
            return obj;
        }

        // 模型可视化对象。
        obj = getGraphObject("GeoModelShapePreviewOCC", m_previewObjHash, model->getShapeAgent());

        return obj;
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphModelProvider::getAllModelGraphObjects(bool createGraph)
    {
        if (createGraph && m_geoData)
        {
            QList<Interface::FITKAbsGeoCommand*> cmdList = m_geoData->getRootCommandList();
            for (Interface::FITKAbsGeoCommand* cmd : cmdList)
            {
                if (!cmd)
                {
                    continue;
                }

                // 跳过草图对象。
                if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
                {
                    continue;
                }

                // 尝试创建可视化对象。
                getGraphObject("GeoModelShapeOCC", m_modelObjHash, cmd->getShapeAgent());
            }
        }

        // 模型可视化对象列表。
        return m_modelObjHash.values();
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getSketch2DGraphObject(int dataObjId)
    {
        // 草图可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };
        if (!m_geoData)
        {
            return obj;
        }

        // 检查数据ID。
        Interface::FITKAbsGeoSketch2D* sketch = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(m_geoData->getDataByID(dataObjId));
        Interface::FITKAbsGeoSketch2D* sketchUndoed = dynamic_cast<Interface::FITKAbsGeoSketch2D*>(m_geoData->getRedoList()->getDataByID(dataObjId));
        if (!sketch && !sketchUndoed)
        {
            return obj;
        }

        // 草图可视化对象。
        obj = getGraphObject("GeoSketchShapeOCC", m_sketchObjHash, sketch == nullptr ? sketchUndoed->getShapeAgent() : sketch->getShapeAgent());

        return obj;
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphModelProvider::getAllSkecth2DGraphObjects(bool createGraph)
    {
        if (createGraph && m_geoData)
        {
            QList<Interface::FITKAbsGeoCommand*> cmdList = m_geoData->getRootCommandList();
            for (Interface::FITKAbsGeoCommand* cmd : cmdList)
            {
                if (!cmd)
                {
                    continue;
                }

                // 跳过非草图对象。
                if (cmd->getGeometryCommandType() != Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
                {
                    continue;
                }

                // 尝试创建可视化对象。
                getGraphObject("GeoSketchShapeOCC", m_sketchObjHash, cmd->getShapeAgent());
            }
        }

        // 草图可视化对象列表。
        return m_sketchObjHash.values();
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphModelProvider::getAllCommandGraphObjects(bool createGraph)
    {
        if (createGraph && m_geoData)
        {
            QList<Interface::FITKAbsGeoCommand*> cmdList = m_geoData->getRootCommandList();
            for (Interface::FITKAbsGeoCommand* cmd : cmdList)
            {
                if (!cmd)
                {
                    continue;
                }

                // 草图对象。
                if (cmd->getGeometryCommandType() == Interface::FITKGeoEnum::FITKGeometryComType::FGTSketch2D)
                {
                    getGraphObject("GeoSketchShapeOCC", m_sketchObjHash, cmd->getShapeAgent());
                }
                // 普通建模对象。
                else
                {
                    getGraphObject("GeoModelShapeOCC", m_modelObjHash, cmd->getShapeAgent());
                }
            }
        }

        // 模型 & 草图可视化对象列表。
        return m_modelObjHash.values() + m_sketchObjHash.values();
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getDatumGraphObject(int dataObjId)
    {
        // 基准元素可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };
        if (!m_geoData)
        {
            return obj;
        }

        Interface::FITKDatumList* datumMgr = m_geoData->getDatumManager();
        if (!datumMgr)
        {
            return obj;
        }

        // 检查数据ID。
        Interface::FITKAbsGeoDatum* datum = dynamic_cast<Interface::FITKAbsGeoDatum*>(datumMgr->getDataByID(dataObjId));
        if (!datum)
        {
            return obj;
        }

        // 基准元素可视化对象。
        int dt = datum->getDatumType();
        if (dt >= 0 && dt < 1000)
        {
            obj = getGraphObject("GeoDatumPointOCC", m_datumObjHash, datum);
        }
        else if (dt >= 1000 && dt < 2000)
        {
            obj = getGraphObject("GeoDatumLineOCC", m_datumObjHash, datum);
        }
        else if (dt >= 2000)
        {
            obj = getGraphObject("GeoDatumPlaneOCC", m_datumObjHash, datum);
        }    

        return obj;
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getDatumPreviewGraphObject(int dataObjId)
    {
        // 基准元素可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        // 检查数据ID。     
        Interface::FITKAbsGeoDatum* datum = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoDatum>(dataObjId);
        if (!datum)
        {
            return obj;
        }

        // 基准元素可视化对象。
        int dt = datum->getDatumType();
        if (dt >= 0 && dt < 1000)
        {
            obj = getGraphObject("GeoDatumPointOCC", m_datumObjHash, datum);
        }
        else if (dt >= 1000 && dt < 2000)
        {
            obj = getGraphObject("GeoDatumLineOCC", m_datumObjHash, datum);
        }
        else if (dt >= 2000)
        {
            obj = getGraphObject("GeoDatumPlaneOCC", m_datumObjHash, datum);
        }

        return obj;
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphModelProvider::getAllDatumGraphObjects(bool createGraph)
    {
        if (createGraph && m_geoData)
        {
            Interface::FITKDatumList* datumMgr = m_geoData->getDatumManager();
            if (datumMgr)
            {
                int nDatum = datumMgr->getDataCount();
                for (int i = 0; i < nDatum; i++)
                {
                    Interface::FITKAbsGeoDatum* datum = dynamic_cast<Interface::FITKAbsGeoDatum*>(datumMgr->getDataByIndex(i));
                    if (!datum)
                    {
                        continue;
                    }

                    // 尝试创建可视化对象。
                    int dt = datum->getDatumType();
                    if (dt >= 0 && dt < 1000)
                    {
                        getGraphObject("GeoDatumPointOCC", m_datumObjHash, datum);
                    }
                    else if (dt >= 1000 && dt < 2000)
                    {
                        getGraphObject("GeoDatumLineOCC", m_datumObjHash, datum);
                    }
                    else if (dt >= 2000)
                    {
                        getGraphObject("GeoDatumPlaneOCC", m_datumObjHash, datum);
                    }
                }
            }
        }

        // 基准元素可视化对象列表。
        return m_datumObjHash.values();
    }


    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getGlobalComponentGraphObject(int dataObjId)
    {
        // 全局几何分组可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };
        if (!m_geoData)
        {
            return obj;
        }

        Interface::FITKGlobalGeoComponentManager* globalCompMgr = m_geoData->getGlobalGeoCompManager();
        if (!globalCompMgr)
        {
            return obj;
        }

        // 检查数据ID。     
        Interface::FITKGlobalGeoComponent* globalComp = globalCompMgr->getDataByID(dataObjId);
        if (!globalComp)
        {
            return obj;
        }

        // 全局几何分组可视化对象。
        bool hasObj = m_componentObjHash.contains(dataObjId);
        obj = getGraphObject("GlobalGeoComp", m_componentObjHash, globalComp);
        if (!hasObj && obj)
        {
            obj->update();
        }

        return obj;
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphModelProvider::getGlobalComponentGraphObjects(bool createGraph)
    {
        if (createGraph && m_geoData)
        {
            Interface::FITKGlobalGeoComponentManager* globalCompMgr = m_geoData->getGlobalGeoCompManager();
            if (globalCompMgr)
            {
                int nComp = globalCompMgr->getDataCount();
                for (int i = 0; i < nComp; i++)
                {   
                    Interface::FITKGlobalGeoComponent* globalComp = globalCompMgr->getDataByIndex(i);
                    if (!globalComp)
                    {
                        continue;
                    }

                    // 尝试创建可视化对象。
                    getGraphObject("GlobalGeoComp", m_componentObjHash, globalComp);
                }
            }
        }

        // 几何分组可视化对象列表。
        return m_componentObjHash.values();
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getTopoShapePreviewGraphObject(int dataObjId)
    {
        // 虚拓扑形状预览可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        // 检查数据ID。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(dataObjId);
        if (!vTopo)
        {
            return obj;
        }

        // 虚拓扑形状可视化对象。
        obj = getGraphObject("TopoShapeOCC", m_previewObjHash, vTopo);

        return obj;
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getTopoLabelPreviewGraphObject(int dataObjId)
    {
        // 虚拓扑形状索引标签预览可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        // 检查数据ID。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(dataObjId);
        if (!vTopo)
        {
            return obj;
        }

        // 虚拓扑形状索引可视化对象。
        obj = getGraphObject("TopoIndexLabelsOCC", m_previewObjHash, vTopo);

        return obj;
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getTopoDirectionPreviewGraphObject(int dataObjId)
    {
        // 虚拓扑形状方向预览可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        // 检查数据ID。
        Interface::FITKAbsVirtualTopo* vTopo = FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(dataObjId);
        if (!vTopo)
        {
            return obj;
        }

        // 虚拓扑形状方向可视化对象。
        obj = getGraphObject("TopoDirectionOCC", m_previewObjHash, vTopo);

        return obj;
    }

    Exchange::FITKOCCGraphObject3D* GraphModelProvider::getCurrentGraphObjByDataId(int dataObjId)
    {
        // 查找模型。
        if (m_modelObjHash.contains(dataObjId))
        {
            return m_modelObjHash[dataObjId];
        }

        // 查找基准元素。
        if (m_datumObjHash.contains(dataObjId))
        {
            return m_datumObjHash[dataObjId];
        }

        return nullptr;
    }

    bool GraphModelProvider::updateObjById(int dataObjId, QVariant info)
    {
        // 参数预留。
        Q_UNUSED(info);

        Core::FITKAbstractGraphObject* obj{ nullptr };
        if (m_modelObjHash.contains(dataObjId))
        {
            obj = m_modelObjHash[dataObjId];
        }

        if (m_datumObjHash.contains(dataObjId))
        {
            obj = m_datumObjHash[dataObjId];
        }

        // 获取可视化对象并更新。
        if (!obj)
        {
            return false;
        }

        obj->update();

        return true;
    }
    
    void GraphModelProvider::setModelSelectMode(CommonOCC::SelectMode mode, int dataObjectId)
    {
        // 为全部对象开启拾取状态。
        if (dataObjectId == -1)
        {
            // 模型可视化对象。
            QList<Exchange::FITKOCCGraphObject3D*> modelObjs = getAllModelGraphObjects();
            for (Exchange::FITKOCCGraphObject3D* obj : modelObjs)
            {
                if (obj)
                {
                    obj->clearSelectMode();
                    obj->addSelectMode(mode);
                }
            }
        }
        // 为指定对象开启拾取状态。
        else
        {
            Exchange::FITKOCCGraphObject3D* obj = getModelGraphObject(dataObjectId);
            if (obj)
            {
                obj->clearSelectMode();
                obj->addSelectMode(mode);
            }
        }
    }

    void GraphModelProvider::setSketchSelectMode(CommonOCC::SelectMode mode, int dataObjectId)
    {
        // 为全部对象开启拾取状态。
        if (dataObjectId == -1)
        {
            QList<Exchange::FITKOCCGraphObject3D*> sketchObjs = getAllSkecth2DGraphObjects();
            for (Exchange::FITKOCCGraphObject3D* obj : sketchObjs)
            {
                if (obj)
                {
                    obj->clearSelectMode();
                    obj->addSelectMode(mode);
                }
            }
        }
        // 为指定对象开启拾取状态。
        else
        {
            Exchange::FITKOCCGraphObject3D* obj = getSketch2DGraphObject(dataObjectId);
            if (obj)
            {
                obj->clearSelectMode();
                obj->addSelectMode(mode);
            }
        }
    }

    void GraphModelProvider::setDatumSelectMode(CommonOCC::SelectMode mode, int dataObjectId)
    {
        // 为全部对象开启拾取状态。
        if (dataObjectId == -1)
        {
            QList<Exchange::FITKOCCGraphObject3D*> datumObjs = getAllDatumGraphObjects(true);
            for (Exchange::FITKOCCGraphObject3D* obj : datumObjs)
            {
                if (obj)
                {
                    obj->clearSelectMode();
                    obj->addSelectMode(mode);
                }
            }
        }
        // 为指定对象开启拾取状态。
        else
        {
            Exchange::FITKOCCGraphObject3D* obj = getDatumGraphObject(dataObjectId);
            if (obj)
            {
                obj->clearSelectMode();
                obj->addSelectMode(mode);
            }
        }
    }

    void GraphModelProvider::setSketchSelectEdgeMode(int dataObjectId)
    {
        // 为全部对象开启拾取状态。
        if (dataObjectId == -1)
        {
            QList<Exchange::FITKOCCGraphObject3D*> sketchObjs = getAllSkecth2DGraphObjects();
            for (Exchange::FITKOCCGraphObject3D* obj : sketchObjs)
            {
                if (obj)
                {
                    obj->clearSelectMode();
                    obj->addSelectMode(CommonOCC::SelectMode::S_ShapeEdge, true);
                }
            }
        }
        // 为指定对象开启拾取状态。
        else
        {
            Exchange::FITKOCCGraphObject3D* obj = getSketch2DGraphObject(dataObjectId);
            if (obj)
            {
                obj->clearSelectMode();
                obj->addSelectMode(CommonOCC::SelectMode::S_ShapeEdge, true);
            }
        }
    }

    void GraphModelProvider::clearPreviewObject(int dataObjId)
    {
        // 清除全部。
        if (dataObjId == -1)
        {
            for (Exchange::FITKOCCGraphObject3D* obj : m_previewObjHash.values())
            {
                if (obj)
                {
                    obj->removeFromGraphWidget();
                    delete obj;
                }
            }

            m_previewObjHash.clear();
        }
        // 清除指定对象。
        else
        {
            if (m_previewObjHash.contains(dataObjId))
            {
                Exchange::FITKOCCGraphObject3D* obj = m_previewObjHash.take(dataObjId);
                if (obj)
                {
                    obj->removeFromGraphWidget();
                    delete obj;
                }
            }
        }
    }
}   // namespace GraphData