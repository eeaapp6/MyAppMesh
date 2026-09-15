/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelExport.h"

namespace Interface
{

    FITKAbsGeoModelExport::~FITKAbsGeoModelExport()
    {

    }

    void FITKAbsGeoModelExport::setFileName(const QString& fileName)
    {
        _exportFileName = fileName;
    }

    QString FITKAbsGeoModelExport::getFileName() const
    {
        return _exportFileName;
    }

    void FITKAbsGeoModelExport::addExportCommandID(int id)
    {
        if (_exportIds.contains(id))
        {
            return;
        }

        _exportIds.push_back(id);
    }

    void FITKAbsGeoModelExport::removeExportCommandID(int id)
    {
        _exportIds.removeOne(id);
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelExport::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTExport;
    }

}