/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCGNSIOInterface.h"
#include "FITKCGNSReader.h"
#include "FITKCGNSWriter.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace IO
{
    QString FITKCGNSIOInterface::getComponentName()
    {
        //返回类型名
        return "IO::FITKCGNSIOInterface";
    }

    void FITKCGNSIOInterface::setFileName(const QString& fileName)
    {
        m_fileName = fileName;
    }

    bool FITKCGNSIOInterface::exec(const int indexPort)
    {
        m_success = false;
        if (m_fileName.isEmpty()) return false;
        //判断indexPort 1：读取 2：写出
        switch (indexPort)
        {
        case 1:
            m_io = new FITKCGNSReader(this);
            break;
        case 2:
            m_io = new FITKCGNSWriter(this);
            break;
        default: break;
        }
        if (m_io == nullptr) return false;
        m_io->setFileName(m_fileName);
        m_io->setResultMark(&m_success);
        m_io->run();
        emit ioThreadFinishedSig();
        return m_success;
    }

    void FITKCGNSIOInterface::setCGNSDataIO(CGNSDataIO* data)
    {
        m_data = *data;
    }

    CGNSDataIO& FITKCGNSIOInterface::getCGNSDataIO()
    {
        return m_data;
    }

    bool FITKCGNSIOInterface::isSuccess()
    {
        return m_success;
    }

    bool FITKCGNSIOInterface::isRunning()
    {
        return  m_io != nullptr;
    }

    void FITKCGNSIOInterface::setRunInThread(bool r /*= true*/)
    {
        m_runInThread = r;
    }

    void FITKCGNSIOInterface::clearCGNSDataIO()
    {
        m_data._mesh.clear();
        m_data._componentManager.clear();
    }

    void FITKCGNSIOInterface::ioThreadFinishedSlot()
    {
        emit this->ioThreadFinishedSig();
    }
}
