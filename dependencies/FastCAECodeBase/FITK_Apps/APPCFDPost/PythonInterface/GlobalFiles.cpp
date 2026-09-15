/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GlobalFiles.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpPythonDriverInterface.h"
#include "FITK_Component/FITKHttpPythonDriver/HttpClient.h"
#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMap>

Files* FilesWapper::new_Files()
{
    return new Files;
}

void FilesWapper::delete_Files(Files* F)
{
    if (F) delete F;
}

void FilesWapper::static_Files_OpenVTKFiles(const QString& fileName)
{


    QFileInfo fileInfo(fileName);

    // 判断是文件还是目录
    if (fileInfo.isFile()) {
        // fileName 是完整的 vtk 文件路径
        // 提取目录路径和文件名
        QString filePath = fileInfo.absolutePath();  // "D:/project/APPCFDPost/output/Solution-2"
        QString file = fileInfo.absoluteFilePath();  // "D:/project/APPCFDPost/output/Solution-2/Solution-2_001.vtk"

        auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpen");
        if (oper == nullptr) return;

        oper->setArgs("files", file);
        oper->setArgs("filePath", filePath);
        oper->execProfession();
        this->waitThreadPool();
        HttpPython::FITKHttpCommons::sendMessageToServer("Import vtk files finish.");
        return;
    }
    // fileName 是目录
    QDir dir(fileName);
    if (!dir.exists()) return;
    // 获取目录下所有 .vtk 文件
    QStringList filters;
    filters << "*.vtk";
    dir.setNameFilters(filters);
    QFileInfoList fileInfoList = dir.entryInfoList();
    if (fileInfoList.isEmpty()) return;
    // 提取前缀并按前缀分组
    // 前缀定义：文件名中最后一个下划线之前的部分
    // 例如 Solution-2_001.vtk → prefix = "Solution-2"
    QMap<QString, QStringList> prefixFilesMap;
    for (const QFileInfo& fileInfo : fileInfoList) {
        QString fileNameOnly = fileInfo.baseName();  // 不带后缀的文件名，如 "Solution-2_001"
        int lastUnderscoreIndex = fileNameOnly.lastIndexOf("_");
        if (lastUnderscoreIndex > 0) {
            QString prefix = fileNameOnly.left(lastUnderscoreIndex);
            prefixFilesMap[prefix].append(fileInfo.absoluteFilePath());
        }
    }

    // 获取操作器
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpen");
    if (oper == nullptr) return;

    oper->setArgs("filePath", fileName);
    // 如果只有一个前缀且只有一个文件，直接导入单个文件
    if (prefixFilesMap.size() == 1) {
        QString prefix = prefixFilesMap.keys().first();
        QStringList files = prefixFilesMap[prefix];
        if (files.size() == 1) {
            // 单个文件，直接设置文件路径
            oper->setArgs("files", files.first());
            oper->execProfession();
            this->waitThreadPool();
            HttpPython::FITKHttpCommons::sendMessageToServer("Import vtk files finish.");
            return;
        }
    }

    // 多个文件，按前缀分组分别导入
    for (auto it = prefixFilesMap.begin(); it != prefixFilesMap.end(); ++it) {
        QString prefix = it.key();
        QString filePath = fileName + QString("/%1_...vtk").arg(prefix);
        oper->setArgs("files", filePath);
        oper->execProfession();
        this->waitThreadPool();
    }
    HttpPython::FITKHttpCommons::sendMessageToServer("Import vtk files finish.");
}
void FilesWapper::waitThreadPool()
{
    Core::FITKThreadPool* pool = Core::FITKThreadPool::getInstance();
    if (pool == nullptr)return;
    pool->wait();
}

 
