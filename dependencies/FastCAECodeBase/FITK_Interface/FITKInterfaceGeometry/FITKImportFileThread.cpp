/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKImportFileThread.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Geometry
#include "FITKGeoInterfaceFactory.h"
#include "FITKGeoCommandList.h"
#include "FITKAbsGeoImportBase.h"

#include <QDebug>

namespace Interface
{
    void FITKImportFileThread::run()
    {
        m_isRunning = true;

        emit sendProcessSig(this, -1);

        m_newIds.clear();

        // Check the file path.
        if (m_filePath.isEmpty())
        {
            m_isRunning = false;
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
            AppFrame::FITKMessageError(tr("Invalid geometry component!"));
            emit sig_readFinished(false);
            return;
        }

        // Create the reader.
        Interface::FITKAbsGeoImportBase* importCmd = geoFac->createCommandT<Interface::FITKAbsGeoImportBase>
            (m_ioType);
        if (!importCmd)
        {
            m_isRunning = false;
            AppFrame::FITKMessageError(tr("Non-supported reading method!"));
            emit sig_readFinished(false);
            return;
        }

        importCmd->setFileName(m_filePath);
        
        bool flag = importCmd->update();
        if (!flag)
        {
            m_isRunning = false;
            AppFrame::FITKMessageError(tr("Failed reading geometry from %1.").arg(m_filePath));
            emit sig_readFinished(false);
            return;
        }

        // Save the ids.
        m_newIds = importCmd->getCreatedCommandIds();

        AppFrame::FITKMessageNormal(tr("Read geometry from %1.").arg(m_filePath));

        // Clear cache.
        m_filePath.clear();

        //// Update shape data for viz.
        //for (const int & cmdId : m_newIds)
        //{
        //    Interface::FITKAbsGeoCommand* newCmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(cmdId);
        //    if (!newCmd)
        //    {
        //        continue;
        //    }

        //    Interface::FITKAbsGeoShapeAgent* shapeAgent = newCmd->getShapeAgent();
        //    if (!shapeAgent)
        //    {
        //        continue;
        //    }

        //    shapeAgent->updateVS();
        //}

        emit sig_readFinished(true);

        emit sendProcessSig(this, 100);

        m_isRunning = false;
    }

    void FITKImportFileThread::setFilePath(QString filePath)
    {
        m_filePath = filePath;
    }

    void FITKImportFileThread::setImportedMethod(Interface::FITKGeoEnum::FITKGeometryComType type)
    {
        switch (type)
        {
        case Interface::FITKGeoEnum::FGTImport:
        case Interface::FITKGeoEnum::FGTImportFlattenAssembly:
            m_ioType = type;
            return;
        default:
            return;
        }
    }

    const QList<int> & FITKImportFileThread::newCmdIds()
    {
        return m_newIds;
    }

    bool FITKImportFileThread::isRunning()
    {
        return m_isRunning;
    }
}