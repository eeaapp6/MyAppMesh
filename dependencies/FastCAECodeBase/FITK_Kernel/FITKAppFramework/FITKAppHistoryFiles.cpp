/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAppHistoryFiles.h"
#include "FITKAppFramework.h"
#include "FITKAppSettings.h"
#include <QSettings>

namespace AppFrame
{
    FITKAppHistoryFileInfo::FITKAppHistoryFileInfo(const QString& path,
        const HistoryFileType & type /*= HistoryFileType::HFTNone*/)
        :_filePath (path),_fileType(type)
    {
        _lastTime = QDateTime::currentDateTime();
    }

    QString FITKAppHistoryFileInfo::getFilePath() const
    {
        return _filePath;
    }

    void FITKAppHistoryFileInfo::setFileType(const HistoryFileType & type)
    {
        _fileType = type;
    }

    AppFrame::FITKAppHistoryFileInfo::HistoryFileType FITKAppHistoryFileInfo::getFileType() const
    {
        return _fileType;
    }

    void FITKAppHistoryFileInfo::updateTime(QDateTime* time /*= nullptr*/)
    {
        if (time == nullptr)
            _lastTime = QDateTime::currentDateTime();
        else
            _lastTime = *time;
    }

    QDateTime FITKAppHistoryFileInfo::getTime() const
    {
        return _lastTime;
    }



    FITKAppHistoryFiles::~FITKAppHistoryFiles()
    {
        //删除所有对象
        for (FITKAppHistoryFileInfo* info : _historyFiles)
        {
            if (info != nullptr) delete info;
        }
        _historyFiles.clear();
    }

    void FITKAppHistoryFiles::addFile(const QString& path, const FITKAppHistoryFileInfo::HistoryFileType & type /*= FITKAppHistoryFileInfo::HistoryFileType::HFTNone*/)
    {
        const int index = this->getIndex(path);
        //已经文件存在
        if (index >= 0)
        {
            //将文件信息移动到最后
            FITKAppHistoryFileInfo* finfo = this->getFileInfo(index);
            if (finfo == nullptr) return;
            _historyFiles.removeAt(index);
            _historyFiles.append(finfo);
            //更新时间
            finfo->updateTime();
        }
        //文件不存在
        else
        {
            FITKAppHistoryFileInfo* finfo = new FITKAppHistoryFileInfo(path,type);
            _historyFiles.append(finfo);
        }
        //调整链表大小
        this->resize();
    }



    void FITKAppHistoryFiles::removeFile(const QString & filePath)
    {
        FITKAppHistoryFileInfo* finfo = this->getFileInfo(filePath);
        if (finfo == nullptr) return;
        _historyFiles.removeAll(finfo);
        delete finfo;
    }

    void FITKAppHistoryFiles::setMaxFileNum(const int n)
    {
        _maxFileNumber = n;
        //重新调整大小
        this->resize();
    }

    int FITKAppHistoryFiles::getMaxFileNum() const
    {
        return _maxFileNumber;
    }

    const int FITKAppHistoryFiles::getFileCount() const
    {
        return _historyFiles.size();
    }

    AppFrame::FITKAppHistoryFileInfo* FITKAppHistoryFiles::getFileInfo(const int index)
    {
        if (index >= 0 && index < _historyFiles.size())
            return _historyFiles.at(index);
        return nullptr;
    }

    FITKAppHistoryFileInfo* FITKAppHistoryFiles::getFileInfo(const QString & filePath)
    {
        for (FITKAppHistoryFileInfo* info : _historyFiles)
        {
            //遍历查找文件名称
            if (info == nullptr) continue;
            if (info->getFilePath() == filePath) 
                return info;
        }
        return nullptr;
    }


    QList<QString> FITKAppHistoryFiles::getHistoryFiles() const
    {
        //全部文件列表
        QList<QString> fs;

        for (FITKAppHistoryFileInfo* info : _historyFiles)
        {
            //遍历查找文件名称
            if (info == nullptr) continue;
            
            fs.append(info->getFilePath());
        }
        return fs;
    }

    QList<FITKAppHistoryFileInfo*> FITKAppHistoryFiles::getFileInfos(const FITKAppHistoryFileInfo::HistoryFileType & type)
    {
        QList<FITKAppHistoryFileInfo*> fs;
     
        for (FITKAppHistoryFileInfo* info : _historyFiles)
        {
            //遍历查找文件类型
            if (info != nullptr && info->getFileType() == type)
                fs.append(info);
        }
        return fs;
    }

    void FITKAppHistoryFiles::loadFromSetting()
    {
        //从配置文件获取
        FITKAppSettings* s = FITKAPP->getAppSettings();
        if (s == nullptr) return;
        QSettings* settins = s->getSettings();
        if (settins == nullptr) return;
        if (!settins->contains("FITKAppHistoryFiles/MaxNumFiles")) return;
        //最大数量
        _maxFileNumber = settins->value("FITKAppHistoryFiles/MaxNumFiles").toInt();
        //判断文件是否存在
        auto Contains = [&](int i)->bool
        {
            bool ok = settins->contains(QString("FITKAppHistoryFiles/FileName%1").arg(i));
            ok &= settins->contains(QString("FITKAppHistoryFiles/FileType%1").arg(i));
            ok &= settins->contains(QString("FITKAppHistoryFiles/FileTime%1").arg(i));
            return ok;
        };
        
        for (int i=0 ;i<_maxFileNumber; ++i)
        {
            if(!Contains(i)) continue;
            //解析信息
            QString fileName = settins->value(QString("FITKAppHistoryFiles/FileName%1").arg(i)).toString();
            int type = settins->value(QString("FITKAppHistoryFiles/FileType%1").arg(i)).toInt();
            QDateTime time = settins->value(QString("FITKAppHistoryFiles/FileTime%1").arg(i)).toDateTime();
           
            //生成信息
            FITKAppHistoryFileInfo* info = new FITKAppHistoryFileInfo(fileName,
                (AppFrame::FITKAppHistoryFileInfo::HistoryFileType)type);
            info->updateTime(&time);
            _historyFiles.append(info);
        }

        



    }

    void FITKAppHistoryFiles::write2Setting()
    {
        //往配置文件写出
        FITKAppSettings* s = FITKAPP->getAppSettings();
        if (s == nullptr) return;
        QSettings* settins = s->getSettings();
        if (settins == nullptr) return;
        //清空组
        settins->remove("FITKAppHistoryFiles");
        //数量
        settins->setValue("FITKAppHistoryFiles/MaxNumFiles", _maxFileNumber);
        for (int i = 0; i< _historyFiles.size(); ++i )
        {
            FITKAppHistoryFileInfo* info = _historyFiles.at(i);
            if(info == nullptr) continue;
            settins->setValue(QString("FITKAppHistoryFiles/FileName%1").arg(i), info->getFilePath());
            settins->setValue(QString("FITKAppHistoryFiles/FileType%1").arg(i), (int)info->getFileType());
            settins->setValue(QString("FITKAppHistoryFiles/FileTime%1").arg(i), info->getTime());
        }

    }

    int FITKAppHistoryFiles::getIndex(const QString &fileName)
    {
        int index = 0;
        for (FITKAppHistoryFileInfo* info : _historyFiles)
        {
            //遍历查找文件名称
            if (info != nullptr && info->getFilePath() == fileName) 
                return index;

            index++;
        }
        //没有找到，返回-1
        return -1;
    }

    void FITKAppHistoryFiles::resize()
    {
        while (_historyFiles.size() > _maxFileNumber)
        {
            //从头移除
            FITKAppHistoryFileInfo* finfo = _historyFiles.first();
            _historyFiles.removeFirst();
            delete finfo;
        }
    }

}

 

