/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PhysicsFactoryWapper.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/PropertyWidget.h"
#include "GUIDialog/GUICalculateDialog/RunWidget.h"
#include "GUIDialog/GUICalculateDialog/CalculateDriver.h"
#include "FITK_Component/FITKOFDriver/FITKOFInputInfo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>

namespace
{
    bool clearVtkFiles(const QString& dirPath)
    {
        QDir dir(dirPath);
        if (!dir.exists()) return false;

        const QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
        for (const QFileInfo& entry : entries)
        {
            if (entry.isDir())
            {
                if (!clearVtkFiles(entry.absoluteFilePath())) return false;
                continue;
            }

            if (entry.suffix().compare("vtk", Qt::CaseInsensitive) == 0)
            {
                if (!QFile::remove(entry.absoluteFilePath())) return false;
            }
        }
        return true;
    }

    bool copyVtkFiles(const QString& sourcePath, const QString& targetPath)
    {
        QDir sourceDir(sourcePath);
        if (!sourceDir.exists()) return false;

        if (!Core::CreateDir(targetPath)) return false;

        const QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
        for (const QFileInfo& entry : entries)
        {
            const QString targetEntryPath = QDir(targetPath).filePath(entry.fileName());
            if (entry.isDir())
            {
                if (!copyVtkFiles(entry.absoluteFilePath(), targetEntryPath)) return false;
                continue;
            }

            if (entry.suffix().compare("vtk", Qt::CaseInsensitive) != 0) continue;

            if (QFile::exists(targetEntryPath) && !QFile::remove(targetEntryPath)) return false;
            if (!QFile::copy(entry.absoluteFilePath(), targetEntryPath)) return false;
        }
        return true;
    }
}

PhysicsFactory::PhysicsFactory()
    : PyInterface::PyPhysicsFactory()
{
}

PhysicsFactory::~PhysicsFactory()
{
}

PhysicsFactory* PhysicsFactoryWapper::new_PhysicsFactory()
{
    return new PhysicsFactory;
}

void PhysicsFactoryWapper::delete_PhysicsFactory(PhysicsFactory* factory)
{
    if (factory) delete factory;
}

QString PhysicsFactoryWapper::setSolver(PhysicsFactory* factory, int solverType)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setSolver(solverType))
    {
        return tr("Failed, invalid solver type %1 or solver factory unavailable").arg(solverType);
    }

    return tr("Success, solver type set to %1").arg(solverType);
}

QString PhysicsFactoryWapper::setTurbence(PhysicsFactory* factory, int turbulenceType)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setTurbence(turbulenceType))
    {
        return tr("Failed, invalid turbulence type %1 or current solver does not support it").arg(turbulenceType);
    }

    return tr("Success, turbulence type set to %1").arg(turbulenceType);
}

QString PhysicsFactoryWapper::setBoundary(PhysicsFactory* factory, const QString& regionName, const QString& boundaryName, int boundaryType, int regionType)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setBoundary(regionName, boundaryName, boundaryType, regionType))
    {
        return tr("Failed, boundary %1 in region %2 could not be created or already exists").arg(boundaryName, regionName);
    }

    return tr("Success, boundary %1 in region %2 configured").arg(boundaryName, regionName);
}

QString PhysicsFactoryWapper::setNewtonianModelParam(PhysicsFactory* factory, double v)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setNewtonianModelParam(v))
    {
        return tr("Failed, Newtonian model parameter could not be set to %1").arg(v);
    }

    return tr("Success, Newtonian model parameter set to %1").arg(v);
}

QString PhysicsFactoryWapper::setSurfaceNormalFixedValueBCParam(PhysicsFactory* factory, const QString& name, double v)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setSurfaceNormalFixedValueBCParam(name, v))
    {
        return tr("Failed, surfaceNormalFixedValue boundary parameter for %1 could not be set").arg(name);
    }

    return tr("Success, boundary %1 reference value set to %2").arg(name).arg(v);
}

QString PhysicsFactoryWapper::setTotalPressureBCParam(PhysicsFactory* factory, const QString& name, double p0)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setTotalPressureBCParam(name, p0))
    {
        return tr("Failed, total pressure boundary parameter for %1 could not be set").arg(name);
    }

    return tr("Success, boundary %1 total pressure set to %2").arg(name).arg(p0);
}

QString PhysicsFactoryWapper::setRunTimeParam(PhysicsFactory* factory, double time)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setRunTimeParam(time))
    {
        return tr("Failed, run time parameter could not be set to %1").arg(time);
    }

    return tr("Success, run time parameter set to %1").arg(time);
}

QString PhysicsFactoryWapper::setRunOutputParam(PhysicsFactory* factory, double param)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (!factory->setRunOutputParam(param))
    {
        return tr("Failed, run output parameter could not be set to %1").arg(param);
    }

    return tr("Success, run output parameter set to %1").arg(param);
}

QString PhysicsFactoryWapper::startRunSolver(PhysicsFactory* factory)
{
    //刷新窗口
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionRun");
    if (oper == nullptr)   
    {
        return tr("Failed to update the interface");
    }
    //设置参数，与界面统一
    oper->execGUI();

    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
    if (mainWindow == nullptr)
    {
        return tr("Failed, main window is null");
    }

    GUI::PropertyWidget* propertyWidget = mainWindow->getPropertyWidget();
    if (propertyWidget == nullptr)
    {
        return tr("Failed, property widget is null");
    }



    GUI::RunWidget* runWidget = dynamic_cast<GUI::RunWidget*>(propertyWidget->getCurrentWidget());
    if (runWidget == nullptr)
    {
        return tr("Failed, current property widget is not RunWidget");
    }

    runWidget->on_pushButton_Run_clicked();
    return tr("Success, solver run started");
}

QString PhysicsFactoryWapper::exportVTK(PhysicsFactory* factory, const QString& outputPath)
{
    if (factory == nullptr)
    {
        return tr("Failed, object is null");
    }

    if (outputPath.isEmpty())
    {
        return tr("Failed, output path is empty");
    }

    AppFrame::FITKAppSettings* appSetting = FITKAPP->getAppSettings();
    AppFrame::FITKProgramTaskManeger* programManager = FITKAPP->getProgramTaskManager();
    if (appSetting == nullptr || programManager == nullptr)
    {
        return tr("Failed, application context is unavailable");
    }

    const QString workDir = appSetting->getWorkingDir();
    if (workDir.isEmpty())
    {
        return tr("Failed, working directory is empty");
    }

    const QString caseDir = QDir::cleanPath(workDir + "/case");
    const QString vtkDir = QDir::cleanPath(caseDir + "/VTK");
    if (!QFileInfo::exists(caseDir))
    {
        return tr("Failed, case directory does not exist: %1").arg(caseDir);
    }

    AppFrame::FITKProgramInputInfo* info = new FoamDriver::FITKOFInputInfo();
    QStringList args;
    args << "-ascii" << "-case" << caseDir;
    info->setArgs(args);

    auto program = programManager->createProgram(1, "CalculateDriver", info);
    if (program == nullptr)
    {
        return tr("Failed, export program could not be created");
    }

    GUI::CalculateDriver* driver = dynamic_cast<GUI::CalculateDriver*>(program);
    if (driver == nullptr)
    {
        return tr("Failed, export driver is invalid");
    }
    driver->setExecProgram("foamToVTK");

    QEventLoop loop;
    QObject::connect(program, &AppFrame::FITKAbstractProgramerDriver::sig_Finish, &loop, &QEventLoop::quit, Qt::QueuedConnection);
    program->start();
    loop.exec();

    QDir vtkSourceDir(vtkDir);
    if (!vtkSourceDir.exists())
    {
        return tr("Failed, VTK output directory was not generated: %1").arg(vtkDir);
    }

    QFileInfo outputInfo(outputPath);
    if (!outputInfo.exists())
    {
        return tr("Failed, output path does not exist: %1").arg(outputPath);
    }
    if (!outputInfo.isDir())
    {
        return tr("Failed, output path is not a directory: %1").arg(outputPath);
    }
    if (!clearVtkFiles(outputPath))
    {
        return tr("Failed, existing vtk files could not be cleared from: %1").arg(outputPath);
    }
    if (!copyVtkFiles(vtkDir, outputPath))
    {
        return tr("Failed, VTK files could not be copied to: %1").arg(outputPath);
    }

    return tr("Success, VTK files exported to %1").arg(QDir::cleanPath(outputPath));
}