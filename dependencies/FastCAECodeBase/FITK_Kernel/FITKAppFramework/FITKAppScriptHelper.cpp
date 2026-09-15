/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAppScriptHelper.h"
#include "FITK_Kernel/FITKCore/FITKScriptTextRepo.h"

namespace AppFrame
{
    Core::FITKScriptTextRepo* FITKAppScriptHelper::getScriptTextRepo()
    {
        return Core::FITKScriptTextRepo::getInstance();
    }
    void FITKAppScriptHelper::clearTextRepo()
    {
        //调用仓库
        Core::FITKScriptTextRepo* r = this->getScriptTextRepo();
        if (r)
            r->clearRepo();
    }

    bool FITKAppScriptHelper::exportScriptFile(const QString& fileName)
    {
        //调用仓库
        Core::FITKScriptTextRepo* r = this->getScriptTextRepo();
        if (r)
            return r->exportScriptFile(fileName);
        return  false;
    }

    void FITKAppScriptHelper::execScriptFile(const QString& fileName)
    {
        if (_execFun == nullptr) return;
        //执行函数
        _execFun(fileName);
    }
    void FITKAppScriptHelper::enableRecord(bool en)
    {
        //调用仓库
        Core::FITKScriptTextRepo* r = this->getScriptTextRepo();
        if (r)
            r->enableRecord(en);
    }
    bool FITKAppScriptHelper::isRecordEnabled()
    {
        //调用仓库
        Core::FITKScriptTextRepo* r = this->getScriptTextRepo();
        if (r)
            return r->isRecordEnabled();
        return false;
    }

    void FITKAppScriptHelper::regExecScriptFun(std::function< void(const QString&) > f)
    {
        _execFun = f;
    }
}


