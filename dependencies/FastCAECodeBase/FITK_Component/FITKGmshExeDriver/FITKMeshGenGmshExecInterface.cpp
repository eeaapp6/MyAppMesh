/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshGenGmshExecInterface.h"
#include "FITKMeshGenerateProcessorGmshExec.h"
#include "FITKMeshSizeGeneratorGmshExec.h"
#include "FITKMeshAlgorithmGeneratorGmshExec.h"
#include "FITKMesherDriverGmshExec.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKZonePoints.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"
#include <QApplication>
#include <QFile>

FITKLIBINFOREGISTER(FITKGmshExeDriver, FITKGMSHEXEDRIVERVERSION);

namespace Gmsh
{
    FITKMeshGenGmshExecInterface::FITKMeshGenGmshExecInterface()
    {
        //拷贝文件
        QString gmshExecPath = QApplication::applicationDirPath() + QString("/gmsh");
        if(Core::CreateDir(gmshExecPath))
        {
            //拷贝gmsh.exe
            QString gmshExeFile = gmshExecPath + QString("/gmsh.exe");
            if (!QFile::exists(gmshExeFile))
            {
                qWarning("FITKMeshGenGmshExecInterface: Copy gmsh.exe to %s", qPrintable(gmshExeFile));
            }
            //拷贝License.txt
            QString licenseFile = gmshExecPath + QString("/License.txt");
            if (!QFile::exists(licenseFile))
            {
                QFile::copy("://gmsh/License.txt", licenseFile);
            }

            QString sourceGmshExe = gmshExecPath+ QString("/gmsh.exe");
            //linux 系统 修改文件名称
            #ifdef Q_OS_LINUX
            sourceGmshExe = gmshExecPath + QString("/gmsh");
            #endif
            if (!QFile::exists(sourceGmshExe))
            {
                if (QFile::exists(sourceGmshExe))
                    qWarning("FITKMeshGenGmshExecInterface: Source gmsh.exe %s not exist!", qPrintable(sourceGmshExe));

            }
        }
        else
        {
            qWarning("FITKMeshGenGmshExecInterface: Failed to create directory %s", qPrintable(gmshExecPath));
        }
        

        //获取工厂
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (nullptr == mf) return;
        //注册相关类
        mf->regMeshSizeGenerator(new FITKMeshSizeGeneratorGmshExec, "GmshExec");
        mf->regMesherDriver(new FITKMesherDriverGmshExec, "GmshExec");
        mf->regMeshProcessor(new FITKMeshGenerateProcessorGmshExec, "GmshExec");
        mf->regMeshAlgorithmGenerator(new FITKMeshAlgorithmGeneratorGmshExec, "GmshExec");
    }


    QString FITKMeshGenGmshExecInterface::getComponentName()
    {
        //名称
        return QString("FITKMeshGenGmshExec");
    }

    bool FITKMeshGenGmshExecInterface::exec(const int indexPort)
    {
        return false;
    }

    QList<int> FITKMeshGenGmshExecInterface::getUnResetDataObj()
    {
        QList<int> ids;
        Interface::FITKMeshGenInterface* mgInter = Interface::FITKMeshGenInterface::getInstance();
        if (!mgInter)
        {
            return ids;
        }

        // 该管理器在网格划分抽象层维护，且继承自数据对象类型，读取工程文件清除数据时会变成野指针。
        Interface::FITKZonePointManager* zptMgr = mgInter->getZonePointManager("GmshExec");
        if (zptMgr)
        {
            ids.push_back(zptMgr->getDataObjectID());
        }

        Interface::FITKGlobalMeshSizeInfo* msi = mgInter->getGlobalMeshSizeInfo();
        if (msi)
        {
            ids.push_back(msi->getDataObjectID());
        }

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* mai = mgInter->getGlobalMeshGenerateAlgorithmInfo();
        if (mai)
        {
            ids.push_back(mai->getDataObjectID());
        }

        return ids;
    }
}



