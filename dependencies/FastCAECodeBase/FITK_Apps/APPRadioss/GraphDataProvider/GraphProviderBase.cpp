/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphProviderBase.h"

#include "GraphDataVTKAdaptor/GraphVTKViewAdaptorBase.h"
#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

// Render VTK
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphRender.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

// Graph widget
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

// Adaptor

// Data 
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"

namespace GraphData
{
    GraphProviderBase::GraphProviderBase(Comp::FITKGraph3DWindowVTK* graphWidget) :
        m_graphWidget(graphWidget)
    {
        // 获取三维可视化窗口引擎类型。
        if (!m_graphWidget)
        {
            return;
        }
    }

    GraphProviderBase::~GraphProviderBase()
    {
        // 析构三维可视化对象。
    }

    QList<Exchange::GraphVTKObject3D*> GraphProviderBase::getCurrentVisibleGraphObjs()
    {
        //获取所有可视化对象数据。
        QList<Exchange::GraphVTKObject3D*> objs = getCurrentGraphObjs();

        QList<Exchange::GraphVTKObject3D*> objsVisible;
        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (!obj)
            {
                continue;
            }

            if (obj->getGraphWidget())
            {
                objsVisible.push_back(obj);
            }
        }

        // 子类重写。
        return objsVisible;
    }

    QString GraphProviderBase::getClassName()
    {
        return "GraphProviderBase";
    }

    // 批量析构Hash指针。
    void GraphProviderBase::deleteObjsHash(QHash<int, Exchange::GraphVTKObject3D*>& hash)
    {
        // 传入数据管理字典。
        for (Exchange::GraphVTKObject3D* obj : hash.values())
        {
            delete obj;
        }

        hash.clear();
    }

    // 批量析构双层Hash指针。
    void GraphProviderBase::deleteObjsHash(QHash<int, QHash<int, Exchange::GraphVTKObject3D*>>& hash)
    {
        // 传入数据管理字典。
        for (QHash<int, Exchange::GraphVTKObject3D* > subHash : hash.values())
        {
            for (Exchange::GraphVTKObject3D* obj : subHash.values())
            {
                if (obj)
                {
                    delete obj;
                }
            }

            subHash.clear();
        }

        hash.clear();
    }

    Exchange::GraphVTKObject3D* GraphProviderBase::getGraphObject(QString adaptorKeyName, QHash<int, Exchange::GraphVTKObject3D*>& objDict, Core::FITKAbstractDataObject* dataObj)
    {
        // 可视化对象。
        Exchange::GraphVTKObject3D* obj{ nullptr };

        // 异常处理。
        if (adaptorKeyName.isEmpty() || !dataObj)
        {
            return obj;
        }

        // 获取数据ID。
        int dataId = dataObj->getDataObjectID();

        // 创建过则返回已实例数据。
        if (objDict.contains(dataId))
        {
            return objDict[dataId];
        }

        // 生成可视化对象。
        Exchange::GraphVTKViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::GraphVTKViewAdaptorBase>(adaptorKeyName, dataObj);
        if (!adaptor)
        {
            return obj;
        }

        // 设置输入数据并更新获取三维可视化对象。
        adaptor->setDataObject(dataObj);
        adaptor->update();

        obj = adaptor->getOutputData();

        // 适配器析构。
        delete adaptor;

        if (!obj)
        {
            return obj;
        }

        objDict.insert(dataId, obj);

        // 检测数据析构对三维数据进行析构并移出数据管理。
        //@{
        connect(dataObj, &Core::FITKAbstractDataObject::dataObjectDestoried, this, [&](Core::FITKAbstractDataObject* objDelete)
        {
            Exchange::GraphVTKObject3D* gObj = objDict.take(objDelete->getDataObjectID());
            if (gObj){
                delete gObj;
                gObj = nullptr;
            }
        });
        //@}

        return obj;
    }

    Exchange::GraphVTKObject3D* GraphProviderBase::getGeoGraphObject(QString keyName, QHash<int, Exchange::GraphVTKObject3D*>& objDict, Interface::FITKAbsGeoCommand* geoCom)
    {
        // 可视化对象。
        Exchange::GraphVTKObject3D* obj{ nullptr };

        // 异常处理。
        if (keyName.isEmpty() || !geoCom)
        {
            return obj;
        }

        // 获取数据ID。
        int dataId = geoCom->getDataObjectID();

        // 创建过则返回已实例数据。
        if (objDict.contains(dataId))
        {
            return objDict[dataId];
        }
        //获取形状代理器
        Interface::FITKAbsGeoShapeAgent* shapeAgent = geoCom->getShapeAgent();
        if (shapeAgent == nullptr) {
            return nullptr;
        }

        // 生成可视化对象。
        Exchange::GraphVTKViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::GraphVTKViewAdaptorBase>(keyName, shapeAgent);
        if (!adaptor)
        {
            return obj;
        }

        // 设置输入数据并更新获取三维可视化对象。
        adaptor->setDataObject(geoCom);
        adaptor->update();

        obj = adaptor->getOutputData();

        // 适配器析构。
        delete adaptor;

        if (!obj)
        {
            return obj;
        }

        objDict.insert(dataId, obj);

        // 检测数据析构对三维数据进行析构并移出数据管理。
        //@{
        connect(geoCom, &Core::FITKAbstractDataObject::dataObjectDestoried, this, [&](Core::FITKAbstractDataObject* objDelete)
            {
                Exchange::GraphVTKObject3D* gObj = objDict.take(objDelete->getDataObjectID());
                if (gObj) {
                    delete gObj;
                    gObj = nullptr;
                }
            });
        //@}

        return obj;
    }

    Exchange::GraphVTKObject3D * GraphProviderBase::getSetOrSurfGraphObject(QString key, QHash<int, QHash<int, Exchange::GraphVTKObject3D*>>& objDict, Core::FITKAbstractDataObject * dataObj, int modeId)
    {
        // 可视化对象。
        Exchange::GraphVTKObject3D* obj{ nullptr };

        // 异常处理。
        if (key.isEmpty() || !dataObj)
        {
            return obj;
        }

        // 获取数据ID。
        int dataId = dataObj->getDataObjectID();

        // 创建过则返回已实例数据。
        if (objDict.contains(dataId)){
            if (objDict[dataId].contains(modeId)) {
                return objDict[dataId][modeId];
            }
        }

        // 生成可视化对象。
        Exchange::GraphVTKViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::GraphVTKViewAdaptorBase>(key, dataObj);
        if (!adaptor)
        {
            return obj;
        }

        // 设置输入数据并更新获取三维可视化对象。
        adaptor->setDataObject(dataObj);
        adaptor->setDetails(modeId);
        adaptor->update();

        obj = adaptor->getOutputData();

        // 适配器析构。
        delete adaptor;

        if (!obj)
        {
            return obj;
        }

        if (objDict.contains(dataId)) {
            objDict[dataId].insert(modeId, obj);
        }
        else {
            QHash<int, Exchange::GraphVTKObject3D*> tempObj;
            tempObj.insert(modeId, obj);
            objDict.insert(dataId, tempObj);
        }

        //检测数据析构对三维数据进行析构并移出数据管理。
        connect(dataObj, &Core::FITKAbstractDataObject::dataObjectDestoried, this, [&](Core::FITKAbstractDataObject* objDelete)
        {
            QHash<int, Exchange::GraphVTKObject3D*> gObjs = objDict.take(objDelete->getDataObjectID());
            for (Exchange::GraphVTKObject3D* gObj : gObjs) {
                if (gObj) {
                    delete gObj;
                    gObj = nullptr;
                }
            }
        });
        return obj;
    }

    Exchange::GraphVTKObject3D * GraphProviderBase::getGraphObjectBySet(QString adaptorKeyName, Core::FITKAbstractDataObject * dataObj, int parentId)
    {
        // 可视化对象。
        Exchange::GraphVTKObject3D* obj{ nullptr };

        // 异常处理。
        if (adaptorKeyName.isEmpty() || !dataObj){
            return obj;
        }

        // 获取数据ID。
        int dataId = dataObj->getDataObjectID();

        // 生成可视化对象。
        Exchange::GraphVTKViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::GraphVTKViewAdaptorBase>(adaptorKeyName, dataObj);
        if (!adaptor)
        {
            return obj;
        }

        // 设置输入数据并更新获取三维可视化对象。
        adaptor->setDetails(parentId);
        adaptor->setDataObject(dataObj);
        adaptor->update();

        obj = adaptor->getOutputData();

        // 适配器析构。
        delete adaptor;

        if (!obj){
            return obj;
        }

        return obj;
    }

    bool GraphProviderBase::updateObjById(int dataId, QVariant info)
    {
        Q_UNUSED(dataId);
        Q_UNUSED(info);
        return false;
    }

    void GraphProviderBase::updateVisibility()
    {
        // 仅获取当前已实例化可视化对象。
        QList<Exchange::GraphVTKObject3D*> objs = getCurrentGraphObjs();

        // 全部更新可见性。
        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (obj)
            {
                //obj->updateVisibility();
            }
        }
    }

    void GraphProviderBase::clearHighlight()
    {
        // 仅获取当前已实例化可视化对象。
        QList<Exchange::GraphVTKObject3D*> objs = getCurrentGraphObjs();

        // 全部尝试取消高亮。
        for (Exchange::GraphVTKObject3D* obj : objs)
        {
            if (obj)
            {
                //obj->disHighlight();
            }
        }
    }
}   // namespace GraphData
