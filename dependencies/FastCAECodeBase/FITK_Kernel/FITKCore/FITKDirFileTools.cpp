/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDirFileTools.h"
#include <QDir>
#include <QFileInfo>

namespace Core
{
    bool RemoveDir(const QString& dirPath)
    {
        QDir dir(dirPath);
        if (!dir.exists()) return true;

        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);  // 设置过滤
        QFileInfoList fileList = dir.entryInfoList();             // 获取所有的文件信息
        for (QFileInfo file : fileList)
        {
            if (file.isFile())
            {  // 是文件，删除
                bool ok = file.dir().remove(file.fileName());
                if (!ok) return false;
            }
            else
            {  // 递归删除目录
                bool ok = RemoveDir(file.absoluteFilePath());
                if (!ok) return false;
            }
        }
        return dir.rmdir(dir.path());  // 删除文件夹
    }

    bool ClearDir(const QString& dirPath)
    {
        QDir dir(dirPath);
        if (!dir.exists())
        {
            CreateDir(dirPath);
            return true;
        }

        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);  // 设置过滤
        QFileInfoList fileList = dir.entryInfoList();             // 获取所有的文件信息
        for (QFileInfo file : fileList)
        {
            if (file.isFile())
            {  // 是文件，删除
                bool ok = file.dir().remove(file.fileName());
                if (!ok) return false;
            }
            else
            {  // 递归删除目录
                bool ok = RemoveDir(file.absoluteFilePath());
                if (!ok) return false;
            }
        }
        return true;
    }

    bool CreateDir(const QString& dirPath)
    {
        QDir dir(dirPath);
        if (dir.exists()) return true;

        QString     path = dir.path();
        QStringList creatPath{ path };

        while (true)
        {
            QString uppath = path.left(path.lastIndexOf("/"));
            if (uppath.isEmpty()) break;
            if (path == uppath) break;
            if (dir.exists(uppath)) break;
            path = uppath;
            creatPath.insert(0, path);
        }

        for (QString p : creatPath)
        {
            bool ok = dir.mkdir(p);
            if (!ok) return false;
        }
        return true;
    }

    bool FITKCoreAPI CreateFile(const QString& fileName)
    {
        //文件存在则返回成功
        if (QFile::exists(fileName)) return true;
        QFileInfo finfo(fileName);
        const QString dir = finfo.absolutePath();
        //创建目录
        if (!CreateDir(dir)) return false;
        //创建文件
        QFile f(fileName);
        f.open(QIODevice::WriteOnly);
        f.close();
        //创建是否成功
        return QFile::exists(fileName);
    }

}  // namespace Common