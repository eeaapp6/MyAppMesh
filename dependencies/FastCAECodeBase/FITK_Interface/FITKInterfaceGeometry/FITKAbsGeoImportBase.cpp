/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoImportBase.h"

namespace Interface
{
    void FITKAbsGeoImportBase::setFileName(const QString& fileName)
    {
        // 保存文件名称。
        _importFileName = fileName;
    }

    QString FITKAbsGeoImportBase::getFileName() const
    {
        // 获取文件名称。
        return _importFileName;
    }

    void FITKAbsGeoImportBase::setBuildVirtualTopo(bool build)
    {
        _buildVirtualTopo = build;
    }

    bool FITKAbsGeoImportBase::getBuildVirtualTopo() const
    {
        return _buildVirtualTopo;
    }

    QList<int> FITKAbsGeoImportBase::getCreatedCommandIds()
    {
        QList<int> subIds;
        int nSubCmds = this->getDataCount();
        for (int i = 0; i < nSubCmds; i++)
        {
            FITKAbsGeoCommand* sub = this->getDataByIndex(i);
            if (sub)
            {
                subIds.push_back(sub->getDataObjectID());
            }
        }

        return subIds;
    }
}

