/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractCGNSReader.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <thread>
#include <cgnslib.h>

namespace IO
{
    void customReadCGNSErrorHandler(int error_code, char *message)
    {
        QString errStr = QObject::tr("Reading CGNS Error [%1] : %2").arg(error_code).arg(message);
        AppFrame::FITKMessageError(errStr);
    }

    FITKAbstractCGNSReader::~FITKAbstractCGNSReader()
    {
        //关闭CGNS文件
        this->closeFile();
    }

    bool FITKAbstractCGNSReader::openCGNSFile()
    {
        try
        {
            //CGNS 错误响应
            cg_error_handler(customReadCGNSErrorHandler);
        }
        catch (...)
        {
            this->consoleMessage(3, tr("Reading CGNS Error Handler invalid !"));
        }
        
        //打开CGNS文件
        try
        {
            //检查有效的 CGNS 文件 2:hdf5
            int file_type = 0;
            if (CG_OK != cg_is_cgns(_fileName.toStdString().c_str(), &file_type))
                return false;
            //打开 CGNS 文件
            if (CG_OK != cg_open(_fileName.toStdString().c_str(), CG_MODE_READ, &m_index_file))
                return false;
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    void FITKAbstractCGNSReader::closeFile()
    {
        if (m_index_file >= 0)
        {
            cg_close(m_index_file);
            m_index_file = -1;
        }
    }

    int FITKAbstractCGNSReader::getCGNSFileIndex()
    {
        return m_index_file;
    }

    void FITKAbstractCGNSReader::sendCurrentPercent(int p)
    {
        //完成后等待主线程2秒
        if (p == 100) std::this_thread::sleep_for(std::chrono::seconds(2));
        emit sendProcessSig(this, p);
    }

}
