/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStaticStruPostVTK.h"
#include <QFile>

namespace Interface
{
    FITKStaticStruPostVTK::FITKStaticStruPostVTK(StruPostFileType type) :
        FITKAbstractStructuralPostVTK(type)
    {
    }
    FITKStaticStruPostVTK::~FITKStaticStruPostVTK()
    {
    }
    StruPostType FITKStaticStruPostVTK::getPostStructualType()
    {
        //静力分析类型
        return SPTStatic;
    }

    void FITKStaticStruPostVTK::setFile(const QString & file, const QString & fileResult)
    {
        if (!QFile::exists(file)) return;
        if (!fileResult.isEmpty() && !QFile::exists(fileResult)) return;
        _fileName = file;
        _resultFileName = fileResult;
        this->setInputFile(file, fileResult);

        // 初始化显示分组All。
        this->resetDisplayGroup();
    }

}



