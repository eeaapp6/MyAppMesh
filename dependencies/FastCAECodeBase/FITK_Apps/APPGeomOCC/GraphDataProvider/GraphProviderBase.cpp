/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphProviderBase.h"

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

// Graph
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"

// Graph widget
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

// Adaptor
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCViewAdaptorBase.h"

// Data 
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

namespace GraphData
{
    GraphProviderBase::GraphProviderBase(Render::FITKGraph3DWindowOCC* graphWidget) :
        m_graphWidget(graphWidget)
    {
        // 获取几何数据仓库。
        m_geoData = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
    }

    GraphProviderBase::~GraphProviderBase()
    {
        // 析构三维可视化对象。
    }

    QList<Exchange::FITKOCCGraphObject3D*> GraphProviderBase::getCurrentVisibleGraphObjs()
    {
        //获取所有可视化对象数据。
        QList<Exchange::FITKOCCGraphObject3D*> objs = getCurrentGraphObjs();

        QList<Exchange::FITKOCCGraphObject3D*> objsVisible;
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
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
    void GraphProviderBase::deleteObjsHash(QHash<int, Exchange::FITKOCCGraphObject3D*>& hash)
    {
        // 传入数据管理字典。
        for (Exchange::FITKOCCGraphObject3D* obj : hash.values())
        {
            delete obj;
        }

        hash.clear();
    }

    // 批量析构双层Hash指针。
    void GraphProviderBase::deleteObjsHash(QHash<int, QHash<int, Exchange::FITKOCCGraphObject3D*>>& hash)
    {
        // 传入数据管理字典。
        for (QHash<int, Exchange::FITKOCCGraphObject3D* > subHash : hash.values())
        {
            for (Exchange::FITKOCCGraphObject3D* obj : subHash.values())
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

    Exchange::FITKOCCGraphObject3D* GraphProviderBase::getGraphObject(QString adaptorKeyName, QHash<int, Exchange::FITKOCCGraphObject3D*>& objDict, Core::FITKAbstractDataObject* dataObj)
    {
        // 可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

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

        // 生成可视化对象适配器。
        Exchange::FITKOCCViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::FITKOCCViewAdaptorBase>(adaptorKeyName, dataObj);
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
            Exchange::FITKOCCGraphObject3D* gObj = objDict.take(objDelete->getDataObjectID());
            if (gObj)
            {
                delete gObj;
            }
        });
        //@}

        return obj;
    }

    Exchange::FITKOCCGraphObject3D* GraphProviderBase::getGraphObject(QString adaptorKeyName, QString key, QVariant info)
    {
        // 可视化对象。
        Exchange::FITKOCCGraphObject3D* obj{ nullptr };

        // 异常处理。
        if (adaptorKeyName.isEmpty() || key.isEmpty())
        {
            return obj;
        }

        // 生成可视化对象。
        Exchange::FITKOCCViewAdaptorBase* adaptor = FITKVIEWADAPTORFACTORY->createT<Exchange::FITKOCCViewAdaptorBase>(adaptorKeyName, key);
        if (!adaptor)
        {
            return obj;
        }

        // 设置输入数据并更新获取三维可视化对象。
        adaptor->setDetails(info);
        adaptor->update();

        obj = adaptor->getOutputData();

        // 适配器析构。
        delete adaptor;

        return obj;
    }

    bool GraphProviderBase::updateObjById(int dataId, QVariant info)
    {
        Q_UNUSED(dataId);
        Q_UNUSED(info);
        return false;
    }

    //void GraphProviderBase::updateVisibility()
    //{
    //    // 仅获取当前已实例化可视化对象。
    //    QList<Exchange::FITKOCCGraphObject3D*> objs = getCurrentGraphObjs();

    //    // 全部更新可见性。
    //    for (Exchange::FITKOCCGraphObject3D* obj : objs)
    //    {
    //        if (obj)
    //        {
    //            obj->updateVisibility();
    //        }
    //    }
    //}

    void GraphProviderBase::clearHighlight()
    {
        // 仅获取当前已实例化可视化对象。
        QList<Exchange::FITKOCCGraphObject3D*> objs = getCurrentGraphObjs();

        // 全部尝试取消高亮。
        for (Exchange::FITKOCCGraphObject3D* obj : objs)
        {
            if (obj)
            {
                obj->disHighlight();
            }
        }
    }
}   // namespace GraphData
