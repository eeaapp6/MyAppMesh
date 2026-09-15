/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKExportFileThread.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

// Geometry
#include "FITKGeoInterfaceFactory.h"
#include "FITKGeoCommandList.h"
#include "FITKAbsGeoModelExport.h"

namespace Interface
{
    void FITKExportFileThread::run()
    {
        // Check the file path.
        if (m_filePath.isEmpty())
        {
            m_isRunning = false;
            m_cmdIds.clear();
            AppFrame::FITKMessageError(tr("Invalid geometry file path!"));
            emit sig_readFinished(false);
            return;
        }

        // Check the geometry component.
        Interface::FITKGeoCommandList* geoList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        Interface::FITKInterfaceGeometryFactory* geoFac = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFac || !geoList)
        {
            m_isRunning = false;
            m_cmdIds.clear();
            AppFrame::FITKMessageError(tr("Invalid geometry component!"));
            emit sig_readFinished(false);
            return;
        }

        // Create the reader.
        Interface::FITKAbsGeoModelExport* exportCmd = geoFac->createCommandT<Interface::FITKAbsGeoModelExport>(m_ioType);
        if (!exportCmd)
        {
            m_isRunning = false;
            m_cmdIds.clear();
            AppFrame::FITKMessageError(tr("Non-supported writeing method!"));
            emit sig_readFinished(false);
            return;
        }

        for (const int & cmdId : m_cmdIds)
        {
            exportCmd->addExportCommandID(cmdId);
        }

        m_isRunning = true;
        emit sendProcessSig(this, -1);
        exportCmd->setFileName(m_filePath);
        bool flag = exportCmd->update();
        if (!flag)
        {
            AppFrame::FITKMessageError(tr("Failed to write geometry to %1.").arg(m_filePath));
            emit sig_readFinished(false);
        }
        else
        {
            // Save the ids.
            AppFrame::FITKMessageNormal(tr("Write geometry to %1.").arg(m_filePath));
            emit sig_readFinished(true);
        }

        // Clear cache.
        m_filePath.clear();
        m_cmdIds.clear();
        emit sendProcessSig(this, 100);
        m_isRunning = false;
    }

    void FITKExportFileThread::setFilePath(QString filePath)
    {
        m_filePath = filePath;
    }

    void FITKExportFileThread::setExportedMethod(Interface::FITKGeoEnum::FITKGeometryComType type)
    {
        switch (type)
        {
        case Interface::FITKGeoEnum::FGTExport:
            m_ioType = type;
            return;
        default:
            return;
        }
    }

    void FITKExportFileThread::addExportCmdId(int cmdId)
    {
        if (!m_cmdIds.contains(cmdId))
        {
            m_cmdIds.push_back(cmdId);
        }
    }

    bool FITKExportFileThread::isRunning()
    {
        return m_isRunning;
    }
}