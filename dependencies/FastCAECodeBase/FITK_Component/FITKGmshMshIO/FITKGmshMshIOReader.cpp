/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshMshIOReader.h"
#include "FITKGmshAdaptorObject.h"
#include "FITKGmshMshDataProcessor.h"
#include "FITKGmshMshIOInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"

namespace Gmsh
{
    FITKGmshMshIOReader::~FITKGmshMshIOReader()
    {
    }

    void FITKGmshMshIOReader::setComponentManager(Interface::FITKComponentManager* componentManager)
    {
        //设置集合管理器
        _componentMgr = componentManager;
    }

    void FITKGmshMshIOReader::run()
    {
        //运行
        *_resultMark = false;
        if (_mshDataGmsh == nullptr) return;
        //提取文件数据
        this->sendCurrentPercent(1);
        bool ok = this->loadFile();
        if (!ok) return;
        //读取文件
        bool readOK = this->read();
        if (_resultMark != nullptr)
            *_resultMark = readOK;
        //映射数据
        _mshDataGmsh->update(dynamic_cast<Interface::FITKUnstructuredMesh*>(_data), _componentMgr);

        //读取完成
        this->sendCurrentPercent(100);
    }

    void FITKGmshMshIOReader::setReaderMshData(FITKGmshMshDataReaderProcessor * data)
    {
        //设置Gmsh Msh数据
        _mshDataGmsh = data;
    }

    FITKGmshMshDataReaderProcessor * FITKGmshMshIOReader::getReaderMshData()
    {
        //获取Gmsh Msh数据
        return _mshDataGmsh;
    }

    void FITKGmshMshIOReader::consoleMessage(int level, const QString& message)
    {
        //打印控制台消息
        switch (level)
        {
        case 1: AppFrame::FITKMessageNormal(message);
            break;
        case 2: AppFrame::FITKMessageWarning(message);
            break;
        case 3: AppFrame::FITKMessageError(message);
            break;
        default:
            break;
        }
    }
    bool FITKGmshMshIOReader::read()
    {
        //读取数据
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKGmshAdaptorObject>("Gmsh", "FITKGmshAdaptorObject");
        if (adaptor == nullptr) return false;
        adaptor->setFileReader(this);
        adaptor->setDataObject(_mshDataGmsh);
        bool readOK = adaptor->adaptR();
        delete adaptor;
        return readOK;
    }
}


