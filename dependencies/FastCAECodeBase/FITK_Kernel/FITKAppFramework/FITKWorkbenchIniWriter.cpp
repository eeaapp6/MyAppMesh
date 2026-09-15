/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWorkbenchIniWriter.h"

#include <QSettings>

namespace
{
    void writeFileInfos(QSettings& settings, const QString& groupName,
        const QString& prefix, const QList<AppFrame::IOFileInfo>& infos)
    {
        settings.remove(groupName);
        settings.setValue(QString("%1/%2_Number").arg(groupName, prefix), infos.size());

        for (int i = 0; i < infos.size(); ++i)
        {
            const AppFrame::IOFileInfo& info = infos.at(i);
            const QString keyPrefix = QString("%1/%2%3").arg(groupName, prefix).arg(i + 1);
            settings.setValue(keyPrefix + "_Path", info._path);
            settings.setValue(keyPrefix + "_Name", info._name);
            settings.setValue(keyPrefix + "_Desc", info._describe);
            settings.setValue(keyPrefix + "_Suffix", info._suffix);
            settings.setValue(keyPrefix + "_Type", info._isFile ? "File" : "Folder");
        }
    }
}

namespace AppFrame
{
    FITKWorkbenchIniWriter::FITKWorkbenchIniWriter(const QString& iniFile) :
        _iniFile(iniFile)
    {
    }

    void FITKWorkbenchIniWriter::setIniFile(const QString& iniFile)
    {
        _iniFile = iniFile;
    }

    QString FITKWorkbenchIniWriter::getIniFile() const
    {
        return _iniFile;
    }

    void FITKWorkbenchIniWriter::appendInputFileInfo(const IOFileInfo& info)
    {
        _inputInfos.append(info);
    }

    void FITKWorkbenchIniWriter::appendOutputFileInfo(const IOFileInfo& info)
    {
        _outputInfos.append(info);
    }

    void FITKWorkbenchIniWriter::clearInputFileInfos()
    {
        _inputInfos.clear();
    }

    void FITKWorkbenchIniWriter::clearOutputFileInfos()
    {
        _outputInfos.clear();
    }

    bool FITKWorkbenchIniWriter::write()
    {
        if (_iniFile.isEmpty()) return false;

        QSettings settings(_iniFile, QSettings::IniFormat);
        writeFileInfos(settings, "Input_Files", "Input_File", _inputInfos);
        writeFileInfos(settings, "Output_Files", "Output_File", _outputInfos);
        settings.sync();
        return settings.status() == QSettings::NoError;
    }
}