/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshAdaptorObject.h"
#include "FITKGmshAdaptorNodes.h"
#include "FITKGmshAdaptorElements.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"

namespace Gmsh
{
    QString FITKGmshAdaptorObject::getAdaptorClass()
    {
        return "FITKGmshAdaptorObject";
    }

    bool FITKGmshAdaptorObject::adaptR()
    {
        if (!_dataObj) return false;
        bool readOK = true;
        //开始读取
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            //获取关键字
            QString keywords = line.trimmed().toLower();
            if (!keywords.contains("$"))continue;
            keywords.remove("$");
            //发送计算进度信号
            _reader->sendCurrentPercent();

            //获取版本信息
            if (keywords == "meshformat")
                readOK &= this->readVersionInfo();
            //解析节点
            else if (keywords == "nodes")
                readOK &= this->readNodes();
            //解析单元
            else if (keywords == "elements")
                readOK &= this->readElements();
            //其余关键字
            else
                continue;
            
            if (readOK)
            {
                //调用解析函数后要执行 backLine函数。
                _reader->backLine();
            }
            else
            {
                //解析出错目前不做处理
                //return false;
            }
        }
        return readOK;
    }

    bool FITKGmshAdaptorObject::adaptW()
    {
        return true;
    }

    bool FITKGmshAdaptorObject::readVersionInfo()
    {
        if (!_reader) return false;
        /**
         * @brief    读取版本信息
         *           例：4.1 0 8
         *           4.1：文件格式版本号，目前是固定值4.1；
         *           0：文件类型，在ASCII文件中值为0；
         *           8：数据长度，即单精度浮点数所占字节数，通常为8。
         */
        m_versionInfo = _reader->readLine();
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            QString keywords = line.trimmed().toLower();
            if (keywords.startsWith("$"))break;
        }
        return true;
    }

    bool FITKGmshAdaptorObject::readNodes()
    {
        if (!_dataObj) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKGmshAdaptorNodes>("Gmsh", "FITKGmshAdaptorNodes");
        if (adaptor == nullptr)
        {
            _reader->consoleMessage(3, "Gmsh Nodes Read Error.");
            return false;
        }
        //设置读取器
        adaptor->setFileReader(_reader);
        adaptor->setDataObject(_dataObj);
        bool ok = adaptor->adaptR();

        if (adaptor != nullptr)
            delete adaptor;
        return ok;
    }

    bool FITKGmshAdaptorObject::readElements()
    {
        if (!_dataObj) return false;
        auto adaptor = FITKIOADAPTORFACTORY->createT<FITKGmshAdaptorElements>("Gmsh", "FITKGmshAdaptorElements");
        if (adaptor == nullptr)
        {
            _reader->consoleMessage(3, "Gmsh Elements Read Error.");
            return false;
        }
        //设置读取器
        adaptor->setFileReader(_reader);
        adaptor->setDataObject(_dataObj);
        bool ok = adaptor->adaptR();

        if (adaptor != nullptr)
            delete adaptor;
        return ok;
    }
}


