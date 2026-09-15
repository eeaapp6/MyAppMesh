/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbaqusData.h"
#include "FITKDataCase.h"
#include "FITKDataCaseManager.h"
#include "FITKJob.h"
#include "FITKJobManager.h"
#include "FITK_Kernal/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernal/FITKAppFramework/FITKGlobalData.h"

//命名空间
namespace AbaqusData
{
    FITKAbaqusData::FITKAbaqusData()
    {
        _caseManager = new FITKDataCaseManager;
        _jobManager = new FITKJobManager;
        //默认有一个算例
        FITKDataCase* caseData = new FITKDataCase;
        caseData->setDataObjectName("Model-1");
        _currentCaseID = caseData->getDataObjectID();
        _caseManager->appendDataObj(caseData);
    }

    FITKAbaqusData* FITKAbaqusData::GetDataFromAppFrame()
    {
        if (FITKAPP == nullptr) return nullptr;
        auto  gloData = FITKAPP->getGlobalData();
        if (gloData == nullptr) return nullptr;
        //强制类型转换
        return gloData->getPhysicsData<FITKAbaqusData>();

    }

    FITKDataCase* AbaqusData::FITKAbaqusData::getCurrentCase()
    {
        auto dataCase = _caseManager->getDataByID(_currentCaseID);
        //算例不存在，可能被删除等情况
        if (dataCase == nullptr)
        {
            //默认置为第一个元素
            dataCase = _caseManager->getDataByIndex(0);
            if (dataCase)
                _currentCaseID = dataCase->getDataObjectID();
        }
        return dataCase;
    }

    bool FITKAbaqusData::setCurrentCase(FITKDataCase * currentData)
    {
        //算例不存在，退出
        if (!_caseManager->isContains(currentData))
            return false;
        _currentCaseID = currentData->getDataObjectID(); 
        return true;
    }

    int FITKAbaqusData::getCurrentCaseID()
    {
        auto dataCase = _caseManager->getDataByID(_currentCaseID);
        //算例不存在，可能被删除等情况
        if (dataCase == nullptr)
        {
            //默认置为第一个元素
            dataCase = _caseManager->getDataByIndex(0);
            if (dataCase)
                _currentCaseID = dataCase->getDataObjectID();
        }
        return _currentCaseID;
    }

    bool FITKAbaqusData::setCurrentCaseID(int currentDataID)
    {
        if (_caseManager->getDataByID(currentDataID) == nullptr)
            return false;
        //设置当前选择的算例数据ID
        _currentCaseID = currentDataID;
        return true;
    }

    FITKDataCaseManager * FITKAbaqusData::getDataCaseManager()
    {
        //返回算例数据管理器
        return _caseManager;
    }
    FITKJobManager * FITKAbaqusData::getJobManager()
    {
        //返回作业管理器
        return _jobManager;
    }

}
