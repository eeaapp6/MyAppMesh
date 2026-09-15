/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractCGNSWriter.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <cgnslib.h>

namespace IO
{
    void customWriteCGNSErrorHandler(int error_code, char *message)
    {
        QString errStr = QObject::tr("Writing CGNS Error [%1] : %2").arg(error_code).arg(message);
        AppFrame::FITKMessageError(errStr);
    }

    FITKAbstractCGNSWriter::~FITKAbstractCGNSWriter()
    {
        //关闭CGNS文件
        this->closeFile();
    }

    bool FITKAbstractCGNSWriter::openCGNSFile()
    {
        try
        {
            //CGNS 错误响应
            cg_error_handler(customWriteCGNSErrorHandler);
        }
        catch (...)
        {
            this->consoleMessage(3, tr("Writing CGNS Error Handler invalid !"));
        }

        //打开CGNS文件
        try
        {
            //创建 CGNS 文件
            if (CG_OK != cg_open(_fileName.toUtf8().constData(), CG_MODE_WRITE, &m_index_file))
                return false;
            //设置写出文件类型HDF5
            if (CG_OK != cg_set_file_type(CG_FILE_HDF5))
                return false;
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    void FITKAbstractCGNSWriter::closeFile()
    {
        if (m_index_file >= 0)
        {
            cg_close(m_index_file);
            m_index_file = -1;
        }
    }

    int FITKAbstractCGNSWriter::getCGNSFileIndex()
    {
        return m_index_file;
    }

}
