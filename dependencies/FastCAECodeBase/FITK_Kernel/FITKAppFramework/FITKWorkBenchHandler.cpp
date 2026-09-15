/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKWorkBenchHandler.h"
#include <QCoreApplication>
#include <QFile>
#include <QSettings>

namespace AppFrame
{

    FITKWorkBenchHandler::FITKWorkBenchHandler()
    {
        //查找ini文件
        QStringList args = qApp->arguments();
        if (!args.contains("-FITKWB")) return;
        int index = args.indexOf("-FITKWB") + 1;
        if (index < 0 || index >= args.size()) return;
        //ini文件是否存在判断
        _iniFile = args.at(index);
        if (!QFile::exists(_iniFile)) return;

        this->read();
        _workingInWorkBench = true;
    }

    bool FITKWorkBenchHandler::workingInWorkBench()
    {
        return _workingInWorkBench;
    }

    void FITKWorkBenchHandler::execHandler()
    {

    }

    void FITKWorkBenchHandler::execOutput()
    {
        this->write();
    }

    QString FITKWorkBenchHandler::getIniFile() const
    {
        return _iniFile;
    }

    int FITKWorkBenchHandler::getInputFilesCount()
    {
        QVariant v = this->getVarient("Input_Files/Input_File_Number");
        if (!v.isValid()) return -1;
        return v.toInt();
    }

    int FITKWorkBenchHandler::getOutputFilesCount()
    {
        QVariant v = this->getVarient("Output_Files/Output_File_Number");
        if (!v.isValid())
            v = this->getVarient("Output_Files/Ouput_File_Number");
        if (!v.isValid()) return -1;
        return v.toInt();
    }

    IOFileInfo FITKWorkBenchHandler::getInputFileInfo(const int index)
    {
        IOFileInfo info;
        info._path = this->getValue<QString>(QString("Input_Files/Input_File%1_Path").arg(index+1));
        info._name = this->getValue<QString>(QString("Input_Files/Input_File%1_Name").arg(index+1));
        info._describe = this->getValue<QString>(QString("Input_Files/Input_File%1_Desc").arg(index+1));
        info._suffix = this->getValue<QString>(QString("Input_Files/Input_File%1_Suffix").arg(index+1));
        QString s = this->getValue<QString>(QString("Input_Files/Input_File%1_Type").arg(index + 1));
        info._isFile = s.toLower() == "file";
        return info;
    }

    IOFileInfo FITKWorkBenchHandler::getOututFileInfo(const int index)
    {
        IOFileInfo info;
        info._path = this->getValue<QString>(QString("Output_Files/Output_File%1_Path").arg(index + 1));
        info._name = this->getValue<QString>(QString("Output_Files/Output_File%1_Name").arg(index + 1));
        info._describe = this->getValue<QString>(QString("Output_Files/Output_File%1_Desc").arg(index + 1));
        info._suffix = this->getValue<QString>(QString("Output_Files/Output_File%1_Suffix").arg(index + 1));
        QString s = this->getValue<QString>(QString("Output_Files/Output_File%1_Type").arg(index + 1));
        info._isFile = s.toLower() == "file";
        return info;
    }

    QString FITKWorkBenchHandler::getOutputPath()
    {
        return this->getValue<QString>(QString("Basic_Information/Output_Base_Path"));

    }

    bool FITKWorkBenchHandler::isProjectFileMode()
    {
        QString v = this->getValue<QString>(QString("Basic_Information/Input_Mode"));
        return (v.simplified().toLower() == "project");
    }

    void FITKWorkBenchHandler::appendOutputInfo(const IOFileInfo& info)
    {
        _outputInfos.append(info);
    }

    int FITKWorkBenchHandler::getOutputInfoCount()
    {
        return _outputInfos.size();
    }

    IOFileInfo FITKWorkBenchHandler::getOutputInfoAt(const int index)
    {
        if (index < 0 || index >= _outputInfos.size()) return IOFileInfo();
        return _outputInfos.at(index);
    }

    void FITKWorkBenchHandler::removeOutputInfo(int index)
    {
        if (index < 0 || index >= _outputInfos.size()) return;
        _outputInfos.removeAt(index);
    }

    void FITKWorkBenchHandler::clearOutputInfo()
    {
        _outputInfos.clear();
    }

    void FITKWorkBenchHandler::write()
    {
        _settings->remove("Output_Files");

        const int n = _outputInfos.size();
        _settings->setValue(QString("Output_Files/Output_File_Number"), n);

        for (int i = 1 ; i <= n ; ++i)
        {
            IOFileInfo info = _outputInfos.at(i-1);

            _settings->setValue(QString("Output_Files/Output_File%1_Path").arg(i), info._path);
            _settings->setValue(QString("Output_Files/Output_File%1_Name").arg(i), info._name);
            _settings->setValue(QString("Output_Files/Output_File%1_Suffix").arg(i), info._suffix);
            _settings->setValue(QString("Output_Files/Output_File%1_Desc").arg(i), info._describe);
            if(info._isFile)
                _settings->setValue(QString("Output_Files/Output_File%1_Type").arg(i), "File");
            else
                _settings->setValue(QString("Output_Files/Output_File%1_Type").arg(i), "Folder");
        }
        _settings->sync();
    }

    void FITKWorkBenchHandler::read()
    {
        //读取文件
        _settings = new QSettings(_iniFile, QSettings::IniFormat);
    }

}



