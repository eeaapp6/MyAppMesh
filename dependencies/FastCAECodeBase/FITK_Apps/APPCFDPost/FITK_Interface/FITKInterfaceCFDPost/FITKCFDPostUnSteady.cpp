/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostUnSteady.h"
#include "FITKCFDPostEnum.h"

#include <QDir>
#include <QDebug>
#include <QCollator>
#include <QFile>
#include <QFileInfo>

namespace Interface
{
    QStringList getSelectGroupFiles(const QString path)
    {
        QStringList groupFiles;

        //截取目录路径
        int indexSlash = path.lastIndexOf("/");
        QString directory = path.left(indexSlash);

        QDir dir(directory);
        if (!dir.exists())
        {
            //path error
            return QStringList();
        }

        if (dir.cd(directory) == false)return groupFiles;

        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);

        //后缀名过滤
        int indexDot = path.lastIndexOf(".");
        QString ext = path.mid(indexDot, path.length() - indexDot);
        QStringList fiter{};
        fiter.append("*" + ext);
        dir.setNameFilters(fiter);

        //截取组名
        QString regGroupName = path.mid(indexSlash + 1, path.length() - indexSlash);
        int indexFirstDot = regGroupName.indexOf(".");
        QString prefix = regGroupName.left(indexFirstDot);

        QFileInfoList list = dir.entryInfoList();

        //文件列表为空，返回
        if (list.count() <= 0)return groupFiles;

        //进行文件识别
        for (int i = 0; i < list.count(); i++) {
            QFileInfo fileInfo = list.at(i);
            QString fileName = fileInfo.fileName();
            //判断文件是否符合分组
            if (fileName.mid(0, prefix.length()) == prefix) {
                //判断文件后缀是否只包含时间步
                QString fileStep = fileName.mid(prefix.length(), fileName.size());
                fileStep = fileStep.remove(ext);
                bool isOnlyIncludeNumber = false;
                isOnlyIncludeNumber = fileStep.toDouble();
                if(isOnlyIncludeNumber == false)continue;
                groupFiles.append(fileInfo.absoluteFilePath());
            }
        }


        //对文件列表进行排序
        QCollator collator;
        collator.setNumericMode(true);
        std::sort(groupFiles.begin(), groupFiles.end(),
            [&collator](const QString & str1, const QString & str2)
        {
            return collator.compare(str1, str2) < 0;
        }
        );
        

        return groupFiles;
    }

    FITKCFDPostUnSteady::FITKCFDPostUnSteady(QString fileType, QString& file) :
        FITKCFDPostSteady(fileType)
    {
        _files = getSelectGroupFiles(file);
        if (_files.size() == 0)return;
    }

    FITKCFDPostUnSteady::~FITKCFDPostUnSteady()
    {

    }

    FITKPostDataType FITKCFDPostUnSteady::getPostDataType()
    {
        return FITKPostDataType::Post_UnSteady;
    }

    void FITKCFDPostUnSteady::setFile(QString& fileName)
    {
        if (fileName.isEmpty())return;
        if (!_files.contains(fileName))return;
        _currentIndex = _files.indexOf(fileName);
        FITKCFDPostSteady::setFile(fileName);
    }

    QStringList FITKCFDPostUnSteady::getFiles()
    {
        return _files;
    }

    int FITKCFDPostUnSteady::getCurrentIndex()
    {
        return _currentIndex;
    }

    double FITKCFDPostUnSteady::getCurrentTime()
    {
        QString filePath = _files[_currentIndex];
        if (filePath.isEmpty())return 0.0;
        QFile file(filePath);
        QFileInfo fileInfo(file);
        QString fileName = fileInfo.fileName();
        QString dataName = this->getDataObjectName();
        int pos = dataName.lastIndexOf("...");
        fileName = fileName.remove(0, pos);
        dataName = dataName.remove(0, pos);
        dataName = dataName.remove(0, 3);
        pos = fileName.lastIndexOf(dataName);
        if (pos <= 0)return 0.0;
        QString time = fileName.mid(0, pos - 1);
        return time.toDouble();
    }
}