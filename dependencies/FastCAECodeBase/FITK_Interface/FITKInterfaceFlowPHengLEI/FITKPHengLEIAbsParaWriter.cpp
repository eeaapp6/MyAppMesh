/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIAbsParaWriter.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>


namespace Interface
{

    bool FITKPHengLEIAbsParaWriter::writeParamFile()
    {
        //创建目录与空文件
        return Core::CreateFile(this->getFileName());
    }


    bool FITKPHengLEIAbsParaWriter::WriteCFDParam(const QString& fileName)
    {
        //创建目录与空文件
		if (!Core::CreateFile(fileName)) return false; 
        //从资源文件读取
        QFile file(":/cfd_para.hypara");
        if(!file.open(QIODevice::ReadOnly)) return false;
        QString s = file.readAll();
        file.close();
        //写出文件
        QFile hpfile(fileName);
        if (!hpfile.open(QIODevice::WriteOnly)) return false;
        hpfile.write(s.toLatin1());
        hpfile.close();
        return  true;
    }

	bool FITKPHengLEIAbsParaWriter::CopyPHengLEIprogram()
	{
		//从资源文件读取
		QFile file(":/PHengLEIv3d0.exe");

		QString PHengLEI = QApplication::applicationDirPath() + "/../PHengLEI/PHengLEIv3d0.exe";
		QFile destinationFile(PHengLEI);
		if (destinationFile.exists()) return false;
		file.copy(PHengLEI);
		return true;
	}

	void FITKPHengLEIAbsParaWriter::consoleMessage(int level, const QString& str)
	{
		return;
	}

	void FITKPHengLEIAbsParaWriter::run()
	{
		return;
	}

}



