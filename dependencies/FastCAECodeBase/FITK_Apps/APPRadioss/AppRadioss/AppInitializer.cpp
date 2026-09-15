/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "AppInitializer.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>

bool AppInitializer::init()
{
    bool isInitFinish = true;
    //初始化软件运行时的配置
    this->initRunTimeSetting();

    if (!this->initAIAssistantSkillResources())
    {
        QMessageBox::warning(
            nullptr,
            QString::fromLatin1("FastCAE-OpenRadioss"),
            QString::fromLatin1("AI assistant skills initialization failed."));
    }

    return isInitFinish;
}

bool AppInitializer::initAIAssistantSkillResources()
{
    const QString basePath = QApplication::applicationDirPath();
    const QString targetRootPath = this->getAIAssistantTargetRootPath();

    Core::CreateDir(basePath + "/FastCAEAssistantPy");
    Core::CreateDir(targetRootPath);

    return this->copyDirectoryFromResource(
        ":/FastCAEAssistantPy/.FastCAEAI",
        targetRootPath);
}

bool AppInitializer::copyDirectoryFromResource(
    const QString& resourcePath,
    const QString& targetPath) const
{
    const QDir resourceDir(resourcePath);
    if (!resourceDir.exists())
    {
        return false;
    }

    QDir targetDir;
    if (!targetDir.mkpath(targetPath))
    {
        return false;
    }

    const QFileInfoList entryInfos = resourceDir.entryInfoList(
        QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
        QDir::DirsFirst | QDir::Name);

    for (int index = 0; index < entryInfos.size(); ++index)
    {
        const QFileInfo entryInfo = entryInfos.at(index);
        const QString targetEntryPath = targetPath + "/" + entryInfo.fileName();

        if (entryInfo.isDir())
        {
            if (!this->copyDirectoryFromResource(entryInfo.filePath(), targetEntryPath))
            {
                return false;
            }
            continue;
        }

        if (!this->copyFileFromResource(entryInfo.filePath(), targetEntryPath))
        {
            return false;
        }
    }

    return true;
}

bool AppInitializer::copyFileFromResource(
    const QString& resourceFilePath,
    const QString& targetFilePath) const
{
    QFile inFile(resourceFilePath);
    if (!inFile.open(QIODevice::ReadOnly))
    {
        return false;
    }

    const QFileInfo targetInfo(targetFilePath);
    QDir targetDir;
    if (!targetDir.mkpath(targetInfo.path()))
    {
        inFile.close();
        return false;
    }

    const QByteArray fileContent = inFile.readAll();
    inFile.close();

    QFile outFile(targetFilePath);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    const qint64 bytesWritten = outFile.write(fileContent);
    outFile.close();

    return bytesWritten == fileContent.size();
}

QString AppInitializer::getAIAssistantTargetRootPath() const
{
    return QApplication::applicationDirPath() + "/FastCAEAssistantPy/.FastCAEAI";
}

void AppInitializer::initRunTimeSetting()
{
    AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
    if (!globalData) return;
    //获取运行时的设置
    AppFrame::FITKRunTimeSetting* runTimeSetting = globalData->getRunTimeSetting();
    if (!runTimeSetting) return;
    //设置软件运行时的初始化配置
    runTimeSetting->setValue("GeoPoint", true);
    runTimeSetting->setValue("GeoLine", true);
    runTimeSetting->setValue("Geo", true);
    runTimeSetting->setValue("MeshPoint", true);
    runTimeSetting->setValue("MeshLine", true);
    runTimeSetting->setValue("Part", true);
}

