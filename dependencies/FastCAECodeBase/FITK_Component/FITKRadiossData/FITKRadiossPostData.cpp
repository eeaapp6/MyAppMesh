/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossPostData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"

namespace Radioss
{

    FITKRadiossPostData::FITKRadiossPostData()
    {
        _post3DManager = new Interface::FITKStruPost3DManager;
        _post2DManager = new FITKRadiossPost2DManager;
    }

    FITKRadiossPostData::~FITKRadiossPostData()
    {
        //释放内存
        if (_post3DManager) delete _post3DManager;
        if (_post2DManager) delete _post2DManager;
    }

    FITKRadiossPostData* FITKRadiossPostData::GetDataFromAPPFrame()
    {
        if (FITKAPP == nullptr) return nullptr;
        auto  gloData = FITKAPP->getGlobalData();
        if (gloData == nullptr) return nullptr;
        //强制类型转换
        return gloData->getPostData<FITKRadiossPostData>();
    }

    Interface::FITKStruPost3DManager* FITKRadiossPostData::getPost3DManager()
    {
        //获取三维后处理数据管理
        return _post3DManager;
    }

    FITKRadiossPost2DManager * FITKRadiossPostData::getPost2DManager()
    {
        // 获取二维后处理数据管理
        return _post2DManager;
    }

    int FITKRadiossPostData::getCurrentPostDataID()
    {
        //获取当前后处理3维数据ID
        return _current3DDataID;
    }

    void FITKRadiossPostData::setCurrentPostDataID(const int id)
    {
        //设置当前后处理3维数据ID
        _current3DDataID = id;
    }

    int FITKRadiossPostData::getAnimTimeInterval()
    {
        //获取动画时间间隔
        return _timeInterval;
    }

    void FITKRadiossPostData::setAnimTimeInterval(int time)
    {
        //设置动画时间间隔
        _timeInterval = time;
    }




    void FITKRadiossPost2DData::setCaseDataID(int id)
    {
        //设置算例ID
        _caseDataID = id;
    }

    int FITKRadiossPost2DData::getCaseDataID()
    {
        //获取算例ID
        return _caseDataID;
    }

    void FITKRadiossPost2DData::setSolutionID(int id)
    {
        //设置解决方案ID
        _solutionID = id;
    }

    int FITKRadiossPost2DData::getSolutionID()
    {
        //获取解决方案ID
        return _solutionID;
    }

    void FITKRadiossPost2DData::setFileName(QString file)
    {
        //设置文件名称
        _fileName = file;
    }

    QString FITKRadiossPost2DData::getFileName()
    {
        //设置文件名称
        return _fileName;
    }

}
