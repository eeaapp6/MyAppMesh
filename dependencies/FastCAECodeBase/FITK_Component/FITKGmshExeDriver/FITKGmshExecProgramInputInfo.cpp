/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshExecProgramInputInfo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"

#include <QDir>

namespace GmshExe
{
    FITKGmshExecProgramInputInfo::FITKGmshExecProgramInputInfo()
    {
        
    }

    FITKGmshExecProgramInputInfo::~FITKGmshExecProgramInputInfo()
    {

    }

    void FITKGmshExecProgramInputInfo::setGeoScriptFile(QString geoScriptFile)
    {
        _geoScriptFile = geoScriptFile;
    }

    void FITKGmshExecProgramInputInfo::setMeshFile(QString type, QString meshFile)
    {
        _meshFile = QPair<QString, QString>(type, meshFile);
    }

    void FITKGmshExecProgramInputInfo::setThreadNumber(int threadNum)
    {
        if (threadNum > 0)
            _threadNumber = threadNum;
    }

    QStringList FITKGmshExecProgramInputInfo::args()
    {
        QString command = QString("%1 -format %2 -o %3 -save -nt %4").arg(_geoScriptFile).arg(_meshFile.first).arg(_meshFile.second).arg(_threadNumber);
        return command.split(' ');
    }

    void FITKGmshExecProgramInputInfo::setMonitorFiles(QStringList &monitorFiles)
    {

    }

    void FITKGmshExecProgramInputInfo::setMonitorPath(QStringList &monitorpath)
    {

    }
}
