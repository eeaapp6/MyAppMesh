/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "CommandLineHandler.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <QDir>
#include <QFileInfo>
#include <QCollator>

void CommandLineHandler::exec()
{
    //查找参数
    int index = _args.indexOf("-i");
    if (index >= 0)importVTK(index + 1);
}

void CommandLineHandler::importVTK(const int index)
{
    //获取文件名称
    QString  files = this->argValue(index);
    if (files.isEmpty()) return;
    QFileInfo fileInfo(files);
    if (!fileInfo.exists())return;

    //操作器
    Core::FITKActionOperator* oper = nullptr;
    if (fileInfo.isDir()) {
        oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpen");
        files = getFilesToPath(files, "vtk");
    }
    else if (fileInfo.isFile()) {
        oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionOpen");
    }

    if (files.isEmpty())return;
    if (oper == nullptr) return;
    oper->setArgs("files", files);
    oper->execProfession();
}

QString CommandLineHandler::getFilesToPath(QString path, QString fileType)
{
    QDir dir(path);
    // 设置过滤器，过滤出文件
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    // 设置名称过滤器，只选择 .vtk 扩展名的文件
    QString type = QString("*.%1").arg(fileType);
    dir.setNameFilters(QStringList() << type);
    // 获取文件信息列表
    QFileInfoList fileInfoList = dir.entryInfoList();
    if (fileInfoList.isEmpty())return "";
    //获取所有的文件
    QStringList fileList = {};
    for (auto fileInfo : fileInfoList) {
        fileList.append(fileInfo.filePath());
    }

    //对文件进行大小排序
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(fileList.begin(), fileList.end(),
        [&collator](const QString & str1, const QString & str2)
    {
        return collator.compare(str1, str2) < 0;
    });

    //对文件进行处理
    QString fileName = fileList.at(0);
    QString fileHead, fileHeadExt, NodeText;
    int endPos = fileName.length() - fileType.length() - 2;
    int beginPos;
    for (beginPos = endPos; beginPos >= 0; --beginPos) {
        if (fileName[beginPos] > '9' || fileName[beginPos] < '0') break;
    }
    fileHead = fileName.mid(0, beginPos + 1);
    NodeText = fileHead + "..." + fileType;

    return NodeText;
}
