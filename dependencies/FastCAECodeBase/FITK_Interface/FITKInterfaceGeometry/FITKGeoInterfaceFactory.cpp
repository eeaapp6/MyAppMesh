/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGeoInterfaceFactory.h"
#include "FITKAbsGeomTools.h"
//#include "FITKAbsVirtualTopoCreator.h"

namespace Interface
{
    //静态变量初始化
    QMutex FITKInterfaceGeometryFactory::m_mutex;
    FITKInterfaceGeometryFactory* FITKInterfaceGeometryFactory::_instance = nullptr;

    void FITKInterfaceGeometryFactory::insertCommandCreateFun(FITKGeoEnum::FITKGeometryComType t, GeoCommandFuns f)
    {
        if(f == nullptr) return;
        //插入命令创建函数,
        _interfaceFuns.insert(t, f);
    }

    void FITKInterfaceGeometryFactory::insertDatumCreateFun(FITKGeoEnum::FITKDatumType t, GeoDatumFuns f)
    {
        if (f == nullptr)
        {
            return;
        }

        // 插入基准元素创建函数。
        _interfaceDatumFuns.insert(t, f);
    }

    FITKAbsGeoCommand* FITKInterfaceGeometryFactory::createCommand(FITKGeoEnum::FITKGeometryComType t)
    {
        //获取创建函数指针
        if (!_interfaceFuns.contains(t)) return nullptr;
        auto fun = _interfaceFuns.value(t);
        if (fun == nullptr) return nullptr;
        //执行函数指针，创建对象
        return fun();
    }

    void FITKInterfaceGeometryFactory::clearAllInterface()
    {
        _interfaceFuns.clear();
        _interfaceDatumFuns.clear();

        if (_geomToolsCreator) delete _geomToolsCreator;
        _geomToolsCreator = nullptr;
    }

    bool FITKInterfaceGeometryFactory::getCommandSupported(FITKGeoEnum::FITKGeometryComType type)
    {
        return _interfaceFuns.contains(type);
    }

    bool FITKInterfaceGeometryFactory::getDatumSupported(FITKGeoEnum::FITKDatumType type)
    {
        return _interfaceDatumFuns.contains(type);
    }

    FITKAbsGeoDatum* FITKInterfaceGeometryFactory::createDatum(FITKGeoEnum::FITKDatumType t)
    {
        // 获取构造函数指针。
        if (!_interfaceDatumFuns.contains(t))
        {
            return nullptr;
        }

        GeoDatumFuns fun = _interfaceDatumFuns.value(t);
        if (fun == nullptr)
        {
            return nullptr;
        }

        return fun();
    }

    void FITKInterfaceGeometryFactory::setGeomToolsCreator(FITKAbstractGeomToolsCreator* c)
    {
        _geomToolsCreator = c;
    }

    FITKAbstractGeomToolsCreator* FITKInterfaceGeometryFactory::getGeomToolsCreator()
    {
        return _geomToolsCreator;
    }

// 
//   FITKAbsVirtualTopo* FITKInterfaceGeometryFactory::createVirtualTopo(FITKAbsVirtualTopo* parent, FITKAbsVirtualTopo::VTopoShapeType t)
//  {
//       if (_virsTopoCreator != nullptr)
//           return _virsTopoCreator->createVirtualTopo(parent, t);
//       return nullptr;
//  }
// 
//  void FITKInterfaceGeometryFactory::regVirtualTopoCreator(FITKAbsVirtualTopoCreator* creator)
//  {
//      _virsTopoCreator =  creator;
//  }

    void FITKInterfaceGeometryFactory::initialize()
    {
        //清空hash
        this->clearAllInterface();
    }



    void FITKInterfaceGeometryFactory::finalize()
    {
        this->clearAllInterface();
    }

    FITKInterfaceGeometryFactoryDeleator::~FITKInterfaceGeometryFactoryDeleator()
    {
        if (FITKInterfaceGeometryFactory::_instance)
            delete FITKInterfaceGeometryFactory::_instance;
        FITKInterfaceGeometryFactory::_instance = nullptr;
    }
    static FITKInterfaceGeometryFactoryDeleator _FIGEOMFACDELEATOR__;

}


