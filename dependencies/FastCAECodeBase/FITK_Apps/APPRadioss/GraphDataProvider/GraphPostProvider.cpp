/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphPostProvider.h"

#include "FITK_Interface/FITKInterfaceStructuralPost/FITKAbstractStruPostVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStaticStruPostVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKDynamicStruPostVTK.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
namespace GraphData
{
    QList<Exchange::GraphVTKObject3D*> GraphPostProvider::getCurrentGraphObjs()
    {
        QList<Exchange::GraphVTKObject3D*> graphObjs;
        graphObjs.append(_postObjHash.values());
        return graphObjs;
    }
    Exchange::GraphVTKObject3D* GraphPostProvider::getModelGraphObject(int dataObjId, bool isInit)
    {
        //模型可视化对象
        Exchange::GraphVTKObject3D* obj = nullptr;
        if(_postObjHash.keys().contains(dataObjId))
        {
            return _postObjHash.value(dataObjId);
        }
        if (!isInit) return obj;

        //检查数据是否是静力分析
        Interface::FITKStaticStruPostVTK* staticModel = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKStaticStruPostVTK>(dataObjId);
        //检查数据是否是动态分析
        Interface::FITKDynamicStruPostVTK* dynamicModel = Core::FITKDataRepo::getInstance()->getTDataByID<Interface::FITKDynamicStruPostVTK>(dataObjId);

        if (staticModel)
        {
            obj = getGraphObject("PostModel", _postObjHash, staticModel);
        }
        else if (dynamicModel)
        {
            obj = getGraphObject("PostModel", _postObjHash, dynamicModel);
        }
        return obj;
    }
    GraphPostProvider::GraphPostProvider(Comp::FITKGraph3DWindowVTK* graphWidget):
        GraphProviderBase(graphWidget)
    {

    }

    GraphPostProvider::~GraphPostProvider()
    {
        _postObjHash.clear();
    }

}
