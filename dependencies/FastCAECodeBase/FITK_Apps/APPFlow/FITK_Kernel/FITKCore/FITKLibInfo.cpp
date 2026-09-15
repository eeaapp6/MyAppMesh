/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKLibInfo.cpp
 * @brief FITK库信息类实现
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2025-04-09
 *
 */
#include "FITKLibInfo.h"
#include <QStringList>
#include <QDebug>
#include <QFile>

namespace Core
{
    FITKLIBINFOREGISTER(FITKCore, FITKCoreVersion);

    //静态变量初始化
    FITKLibInfo* FITKLibInfo::_instance = nullptr;
    QMutex FITKLibInfo::m_mutex;

    void FITKLibInfo::registerLibrary(const QString& name, const QString& version, const QString& buildDate)
    {
        m_libraries.insert(name, LibraryInfo(name, version, buildDate));
    }

    LibraryInfo FITKLibInfo::getLibraryInfo(const QString& name) const
    {
        return m_libraries.value(name, LibraryInfo());
    }

    QStringList FITKLibInfo::getRegisteredLibraries() const
    {
        return m_libraries.keys();
    }

    QString FITKLibInfo::getVersion(const QString& name) const
    { 
        return m_libraries.value(name).version;
    }

    QString FITKLibInfo::getBuildDate(const QString& name) const
    { 
        return m_libraries.value(name).buildDate;
    }

    QStringList FITKLibInfo::getAllLibNames() const
    {
        return m_libraries.keys();
    }

    LibraryInfo FITKLibInfo::getLibraryInfoByName(const QString &name) const
    {
        return m_libraries.value(name, LibraryInfo());
    }

    void FITKLibInfo::printAllLibraryInfo() const
    { 
        qDebug() << "==========FITK Library Information ==========";
        QStringList sortedKeys = m_libraries.keys();
        //sortedKeys.sort();
        int index = 0;
        for (const QString& key : sortedKeys) 
        {
            const LibraryInfo& lib = m_libraries.value(key);
            qDebug() << "Library #   :" << ++index << "/" << sortedKeys.size();
            qDebug() << "Library Name:" << lib.name;
            qDebug() << "Version     :" << lib.version;
            qDebug() << "Build Time  :" << lib.buildDate;
            qDebug() << "Build Type  :" << lib.buildType;
            qDebug() << "----------------------------------------";
        }
        qDebug() << "======================================";
    }
    void FITKLibInfo::printAllLibraryInfoToFile(const QString &fileName) const
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
        {
            qWarning() << "Failed to open file for writing:" << fileName;
            return;
        }
        QTextStream out(&file);
        QStringList sortedKeys = m_libraries.keys();
        //sortedKeys.sort();
        out << "========== FITK Library Information ==========" << endl;
        out << "Total Libraries: " << sortedKeys.size() << endl;
        out << "----------------------------------------" << endl;
        int index = 0;
        for (const QString& key : sortedKeys) 
        {
            const LibraryInfo& lib = m_libraries.value(key);
            out << "Library #   :" << ++index << "/" << sortedKeys.size() << endl;
            out << "Library Name: " << lib.name << endl;
            out << "Version     : " << lib.version << endl;
            out << "Build Time  : " << lib.buildDate << endl;
            out << "Build Type  : " << lib.buildType << endl;
            out << "----------------------------------------" << endl;
        }
        out << "======================================" << endl;
    }
    QStringList FITKLibInfo::toStringList() const
    { 
        QStringList result;
        QStringList sortedKeys = m_libraries.keys();
        sortedKeys.sort();
        for (const QString& key : sortedKeys) 
        {
            const LibraryInfo& lib = m_libraries.value(key);
            QString libInfo = QString("Library: %1 | Version: %2 | Build Time: %3")
                .arg(lib.name)
                .arg(lib.version)
                .arg(lib.buildDate);
            result.append(libInfo);
        }
        return result;
    }

    void FITKLibInfo::initialize()
    {
         
    }

    void FITKLibInfo::finalize()
    {
        m_libraries.clear();
    }

    FITKLibInfoDeleator::~FITKLibInfoDeleator()
    {
        if (FITKLibInfo::_instance)
            delete FITKLibInfo::_instance;
        FITKLibInfo::_instance = nullptr;
    }

    static FITKLibInfoDeleator __LIBINFO_DELEATOR__;
}
