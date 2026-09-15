/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFAbstractDictWriter.h"
#include "FITKDictObject.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <QTextStream>
#include <QFile>
#include <QDir>
//命名空间
namespace IO
{
    FITKOFAbstractDictWriter::FITKOFAbstractDictWriter()
    {
    }
    FITKOFAbstractDictWriter::~FITKOFAbstractDictWriter()
    {
        if (!m_objectDict)delete m_objectDict;
    }

    void FITKOFAbstractDictWriter::setFilePath(const QString& filePath)
    {
        m_filePath = filePath;
    }

    QString FITKOFAbstractDictWriter::getFileName()
    {
        return QString();
    }

    bool FITKOFAbstractDictWriter::run()
    {
        return false;
    }

    void FITKOFAbstractDictWriter::consoleMessage(int level, const QString& str)
    {
        switch (level)
        {
            case 1:AppFrame::FITKMessageNormal(str); break;
            case 2: AppFrame::FITKMessageWarning(str); break;
            case 3:AppFrame::FITKMessageError(str); break;
            default: AppFrame::FITKMessageError(str); break;
        }
    }

    void FITKOFAbstractDictWriter::openDictObject()
    {
        if (m_objectDict == nullptr)
            m_objectDict = new DICT::FITKDictObject;
    }

    bool FITKOFAbstractDictWriter::writeDictFile()
    {
        //获取文件名
        QString fileName = this->getFileName();
        //获取路径
        QDir dir;
        QString path = fileName.mid(0, fileName.lastIndexOf('/'));
        //确保路径存在
        if (!dir.exists(path))
        {
            bool res = dir.mkpath(path);
            if (!res)return false;
        }
        //打开文件
        QFile file(fileName);
        if (file.exists())file.remove();
        //设置写出
        if (!file.open(QIODevice::ReadWrite | QIODevice::NewOnly))
        {
            file.close();
            return false;
        }
        //打开文件流
        QTextStream stream(&file);
        //写出数据
        m_objectDict->writeObjectDict(&stream);
        //关闭文件
        file.close();
        return true;
    }

    void FITKOFAbstractDictWriter::closeDictObject()
    {
        if (!m_objectDict)
            delete m_objectDict;
        m_objectDict = nullptr;
    }

}



