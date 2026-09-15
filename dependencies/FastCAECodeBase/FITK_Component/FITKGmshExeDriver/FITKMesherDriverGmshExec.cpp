/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMesherDriverGmshExec.h"

#include "FITKGmshExecProgramInputInfo.h"
#include "FITKGmshExecProgramDriver.h"
#include "GUIGmshSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKThreadPool.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"

#include <QFileInfo>
#include <QThread>
#include <QTimer>
#include <QDir>

namespace Gmsh
{
    FITKMesherDriverGmshExec::FITKMesherDriverGmshExec()
    {
        
    }

    FITKMesherDriverGmshExec::~FITKMesherDriverGmshExec()
    {

    }
    /**
     * @brief    开始网格划分
     */
    void FITKMesherDriverGmshExec::startMesher(QStringList info /*= QStringList()*/)
    {
        // TODO: 实现网格生成的启动逻辑
        Q_UNUSED(info);
        /**
         * @brief    Method(划分方式）
         *           0或者空（直接驱动Gmsh划分所需参数)
         *           1(通过setting界面拾取模型并进行划分设置)
         *           2(通过传递geoCommendIDs多并行网格划分)
         */
        int method = 0;
        if (!this->getValue("Method").isNull())
            method = this->getValueT<int>("Method");
        //驱动方式
        switch (method)
        {
        case 0: {//自动生成脚本驱动网格划分：模型文件、指定网格生成路径、指定脚本生成路径即可，该方式会自动根据FITK网格划分参数数据结构自动生成脚本
            this->startGmshMeshGenerationDriven();
            break;
        }
        case 1: {//自动调用组件内部网格划分配置窗口驱动网格划分：指定网格生成路径即可，该方式会自动根据配置窗口（拾取几何数据与配置网格划分信息）自动生成几何文件与脚本文件
            this->gmshMeshGenSetting();
            break;
        }
        case 2: {//直接驱动网格划分：模型文件、脚本文件、指定网格生成文件即可，该方式通过外部导入模型和划分脚本，直接驱动网格划分
            this->startGmshMeshGenerationDriven(false);
            break;
        }
        case 3: {//多模型网格划分：多个模型文件、指定对应生成的网格文件即可，该方式通过并行驱动gmsh网格划分程序，快速生成网格
            this->startMultiModelGenDriven();
            break;
        }
        default:
            break;
        }
    }
    
    void FITKMesherDriverGmshExec::stopMesher(QStringList info /*= QStringList()*/)
    {
        Q_UNUSED(info);

        //停止任务管理器中仍在运行/等待的Gmsh驱动
        AppFrame::FITKProgramTaskManeger* programTaskMgr = FITKAPP->getProgramTaskManager();
        if (programTaskMgr)
        {
            const int count = programTaskMgr->getDataCount();
            for (int i = 0; i < count; ++i)
            {
                AppFrame::FITKAbstractProgramerDriver* driver = programTaskMgr->getDataByIndex(i);
                GmshExe::FITKGmshExecProgramDriver* gmshExe = dynamic_cast<GmshExe::FITKGmshExecProgramDriver*>(driver);
                if (!gmshExe) continue;
                gmshExe->stop();
            }
        }

        //停止并清空当前驱动内记录的多模型任务队列
        for (AppFrame::FITKExecProgramDriver* gmshExe : _process)
        {
            if (!gmshExe) continue;
            gmshExe->stop();
        }
        _process.clear();
        _processNumRecord = 0;

        AppFrame::FITKMessageNormal(QString("Stop Gmsh mesher."));
    }

    void FITKMesherDriverGmshExec::gmshMeshGenSetting()
    {
        //gmsh网格生成设置
        GUI::GUIGmshSettings* dialog = new GUI::GUIGmshSettings(this, FITKAPP->getGlobalData()->getMainWindow());
        //当界面执行完毕后执行
        connect(dialog, &QDialog::accepted, this, &FITKMesherDriverGmshExec::writerModelGeoScriptFileDriven);
        dialog->show();
    }

    void FITKMesherDriverGmshExec::startGmshMeshGenerationDriven(bool isGenScript)
    {
        QString meshPath = FITKAPP->getTempDir(false, "Gmsh");
        //获取划分维度、模型文件和网格文件
        QString shapeFile = this->getValueT<QString>("ShapeFile");
        QString meshFile = this->getValueT<QString>("MeshFile");
        QString geoScriptFile = this->getValueT<QString>("ScriptFile");
        if (isGenScript && !meshPath.isEmpty())
            geoScriptFile = QString("%1/%2").arg(meshPath).arg("script.geo");
        //创建Gmsh驱动程序，并关联信号
        AppFrame::FITKExecProgramDriver* gmshExe = createGmshExecProgramDriver(shapeFile, meshFile, geoScriptFile, isGenScript);
        if (!gmshExe) return;
        connect(gmshExe, SIGNAL(sig_Finish()), this, SIGNAL(mesherFinished()));
        //开始驱动Gmsh进行网格划分
        gmshExe->start();
    }

    void FITKMesherDriverGmshExec::startMultiModelGenDriven()
    {
        QString meshPath = FITKAPP->getTempDir(false, "Gmsh");
        if (meshPath.isEmpty()) return;
        //进程数
        QVariant value = this->getValue("ProcessNum");
        int processNum = QThread::idealThreadCount() / 2;
        processNum = processNum < 1 ? 1 : processNum;
        if (!value.isNull())
            processNum = value.toInt() < 1 || value.toInt() > processNum ? processNum : value.toInt();
        //获取划分维度、模型文件和网格文件
        QStringList shapeFileList = this->getValueT<QStringList>("ShapeFiles");
        QStringList meshFileList = this->getValueT<QStringList>("MeshFiles");
        //获取工作目录
        if (shapeFileList.isEmpty() || shapeFileList.size() != meshFileList.size()) return;
        for (int i = 0; i < shapeFileList.size(); ++i)
        {
            QString shapeFile = shapeFileList[i];
            QString meshFile = meshFileList[i];
            QString scriptFile = QString("%1/script_%2.geo").arg(meshPath).arg(i + 1);
            AppFrame::FITKExecProgramDriver* gmshExe = createGmshExecProgramDriver(shapeFile, meshFile, scriptFile);
            if (!gmshExe) continue;
            connect(gmshExe, &AppFrame::FITKAbstractProgramerDriver::sig_Finish, this, &FITKMesherDriverGmshExec::meshGenDriven_Solt);
            _process.append(gmshExe);
        }
        //开始驱动gmsh划分
        _processNumRecord = _process.size();
        while (!_process.isEmpty() && processNum > 0)
        {
            AppFrame::FITKExecProgramDriver* gmshExe = _process.first();
            _process.removeFirst();
            if (gmshExe == nullptr) continue;
            --processNum;
            gmshExe->start();
        }
    }

    AppFrame::FITKExecProgramDriver * FITKMesherDriverGmshExec::createGmshExecProgramDriver(const QString & shapeFile, const QString & meshFile, const QString & geoScriptFile, bool isGenScript)
    {
        if (shapeFile.isEmpty() || meshFile.isEmpty() || geoScriptFile.isEmpty()) return nullptr;
        //获取是否打印网格信息和线程数
        bool sendMessage = true;
        if (!this->getValue("EnableSendMessage").isNull())
            sendMessage = this->getValueT<bool>("EnableSendMessage");
        int threadNum = 1;
        if (!this->getValue("ThreadNum").isNull())
            threadNum = this->getValueT<int>("ThreadNum");
        //移除网格文件，避免干扰
        QFile::remove(meshFile);
        //创建Gmsh可执行程序驱动输入信息
        GmshExe::FITKGmshExecProgramInputInfo* inputInfo = new GmshExe::FITKGmshExecProgramInputInfo;
        inputInfo->setGeoScriptFile(geoScriptFile);
        inputInfo->setThreadNumber(threadNum);
        inputInfo->setMeshFile("msh", meshFile);
        //获取可执行程序驱动管理器
        AppFrame::FITKProgramTaskManeger* programTaskMgr = FITKAPP->getProgramTaskManager();
        GmshExe::FITKGmshExecProgramDriver* gmshExe = dynamic_cast<GmshExe::FITKGmshExecProgramDriver*>(programTaskMgr->createProgram(1, "FITKGmshExecProgramDriver", inputInfo));
        if (!gmshExe)return nullptr;
        if (isGenScript)
            gmshExe->writeConfigFile(shapeFile, geoScriptFile, meshFile);
        gmshExe->enableSendMessage(sendMessage);
        //判断Gmsh驱动是否存在
        if (!gmshExe->isExistExeProgram())
        {
            delete gmshExe;
            return nullptr;
        }
        return gmshExe;
    }

    void FITKMesherDriverGmshExec::meshGenDriven_Solt()
    {
        --_processNumRecord;
        //判断是否还有未进行的进程
        while (!_process.isEmpty())
        {
            AppFrame::FITKExecProgramDriver* gmshExe = _process.first();
            _process.removeFirst();
            if (gmshExe == nullptr) continue;
            gmshExe->start();
            break;
        }
        if (_processNumRecord == 0)
            emit mesherFinished();
    }

    void FITKMesherDriverGmshExec::writerModelGeoScriptFileDriven()
    {
        //获取工作目录
        QString meshPath = FITKAPP->getTempDir(false, "Gmsh");
        if (meshPath.isEmpty()) return;
        //获取指定网格划分生成的网格文件路径
        QString meshFile = this->getValueT<QString>("MeshFile");
        if (meshFile.isEmpty()) return;
        //获取几何虚拓扑数据
        QVariant v = {};
        v = this->getValue("virtualTopos");
        QList<Interface::VirtualShape> topos = v.value<QList<Interface::VirtualShape>>();
        if (topos.size() == 0) return;
        //指定网格划分路径
        QString shapeFile = QString("%1/%2").arg(meshPath).arg("geometryFile.brep");//指定软件生成的几何文件
        QString scriptFile = QString("%1/%2").arg(meshPath).arg("script.geo");      //指定网格划分驱动程序所需的脚本文件路径
        //确保网格划分的工作路径存在
        QDir meshDir(meshPath);
        if (!meshDir.exists())
            meshDir.mkdir(meshPath);
        //移除旧网格划分结果，避免干扰新结果
        Core::ClearDir(meshPath);
        //获取网格划分接口
        Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
        if (!mf) return;
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenAlgInfo = mf->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (!meshGenAlgInfo) return;
        //创建几何文件生成工具
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        Interface::FITKAbsGeoModelExportTopos* geoExport = dynamic_cast<Interface::FITKAbsGeoModelExportTopos*>
            (geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTExportTopos));
        if (!geoExport) return;
        //设置几何数据并执行模型文件写出
        if (meshGenAlgInfo->getMeshGenerateDimension() == 2)//最高维度仅是2维才进行几何缝合
            geoExport->setEnableStitch(meshGenAlgInfo->getGeometryStitch());
        geoExport->setExportTopos(topos);
        geoExport->setFileName(shapeFile);
        bool isExportSuccess = geoExport->update();
        delete geoExport;
        if (!isExportSuccess) {
            AppFrame::FITKMessageError("Failed to export geometry file for meshing.");
            return;
        }
        //创建Gmsh驱动程序，并关联信号
        AppFrame::FITKExecProgramDriver* gmshExe = createGmshExecProgramDriver(shapeFile, meshFile, scriptFile);
        if (!gmshExe) return;
        connect(gmshExe, SIGNAL(sig_Finish()), this, SIGNAL(mesherFinished()));
        //开始驱动Gmsh进行网格划分
        gmshExe->start();
    }

}
