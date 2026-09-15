/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGlobalData.h"
#include "FITKRunTimeSetting.h"
#include "FITKKeyMouseStates.h"
#include "FITKAppFramework.h"
#include "FITKProgramTaskManager.h"
#include "FITKAppHistoryFiles.h"
#include "FITKCore/FITKAbstractDataObject.h"
#include <QWidget>

namespace AppFrame
{
    QMutex FITKGlobalData::_mutex;

    FITKGlobalData::FITKGlobalData()
    {
        _runtimeSetting = new FITKRunTimeSetting;
        _keyMouseStates = new FITKKeyMouseStates;
        _historyFiles = new FITKAppHistoryFiles;
    }

    FITKGlobalData::~FITKGlobalData()
    {
        //释放全部内存
        auto gloData = _gData.values();
        _gData.clear();
        for (auto d : gloData)
            delete d; 

        if (_runtimeSetting) delete _runtimeSetting;
        if (_keyMouseStates) delete _keyMouseStates;
        if (_historyFiles) delete _historyFiles;
    }


    void FITKGlobalData::setMainWindow(QWidget* mw)
    {
        // 将mw赋值给私有成员变量_mainwindow，保存主窗口的指针。
        _mainwindow = mw;
        if (_mainwindow)
            connect(_mainwindow, &QWidget::destroyed, this, [&] {_mainwindow = nullptr; });
    }

    QWidget* FITKGlobalData::getMainWindow()
    {
        // 直接返回私有成员变量_mainwindow的值，即当前主窗口的指针。
        return _mainwindow;
    }

    AppFrame::FITKRunTimeSetting* FITKGlobalData::getRunTimeSetting()
    {
        // 直接返回私有成员变量_runtimeSetting的值，即指向运行时设置对象的指针。
        return _runtimeSetting;
    }

    FITKKeyMouseStates* FITKGlobalData::getKeyMouseStates()
    {
        return _keyMouseStates;
    }

    QList<int> FITKGlobalData::getGlobalDataIDs()
    {
        //本身的全局数据
        const QList<Core::FITKAbstractDataObject*> objs = _gData.values();
        QList<int> ids;
        for (Core::FITKAbstractDataObject* obj : objs)
        {
            if(obj == nullptr) continue;
            ids.append(obj->getDataObjectID());
        }
        //framework中的全局数据
        FITKProgramTaskManeger* ta = FITKAPP->getProgramTaskManager();
        if (ta)
        {
            ids.append(ta->getDataObjectID());
        }
        return ids;
    }

    AppFrame::FITKAppHistoryFiles* FITKGlobalData::getHistoryFiles()
    {
        return _historyFiles;
    }

    void FITKGlobalData::insertData(int dt, Core::FITKAbstractDataObject* d)
    {
        //空指针，不操作
        if (d == nullptr) return;
        //数据存在，先删除原来数据
        if (_gData.contains(dt))
        {
            auto d = _gData.value(dt);
            _gData.remove(dt);
            if (d) delete d;
        }
        //插入数据管理
        _gData.insert(dt, d);
    }

//     void FITKGlobalData::mainWindowDestorySlot(QObject* obj)
//     {
//         if(_mainwindow == obj)
//             _mainwindow = nullptr;
//     }

}


