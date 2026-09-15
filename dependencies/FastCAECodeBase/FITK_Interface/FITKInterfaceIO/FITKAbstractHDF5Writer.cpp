/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractHDF5Writer.h"
#include "FITKHDF5FileTool.h"
#include <QString>
#include <H5Cpp.h>


namespace IO
{

    FITKAbstractHDF5Writer::~FITKAbstractHDF5Writer()
    {
        //从其他位置传参形式的文件指针不进行处理
        if (!_createFileContext) return;
        
        this->closeFile();
        if (_h5File) delete _h5File;
        if (_tools) delete _tools;
    }

    void FITKAbstractHDF5Writer::setH5Root(H5::H5File* h5File)
    {
        //设置文件指针
        _h5File = h5File;
        _createFileContext = false;
    }

    void FITKAbstractHDF5Writer::setVersion(double v)
    {
        //设置版本号
        _version = v;
    }
    H5::H5File* FITKAbstractHDF5Writer::getHDF5File()
    {
        //获取HDF5文件指针
        return _h5File;
    }

    bool FITKAbstractHDF5Writer::createContext(QString & error)
    {
        if (_fileName.isEmpty())
        {
            error = QObject::tr("Invalid file name !");
            return false;
        }

        if (_h5File != nullptr)
        {
            error = QObject::tr("HDF5 root already exist !");
            return false;
        }

        error.clear();

        //文件名类型转换、
        QByteArray ba = _fileName.toLocal8Bit();
        const char* fnc = ba.data();

        try
        {          
            _h5File = new H5::H5File(fnc, H5F_ACC_TRUNC);
        }
        catch (const H5::FileIException& ex)
        {
            error = QString(ex.getCDetailMsg());
        }

        _createFileContext = true;
        //没有异常说明创建成功      
        return error.isEmpty();
    }

    void FITKAbstractHDF5Writer::closeFile()
    {
        if (_h5File)
            _h5File->close();
    }

    IO::FITKHDF5FileTool* FITKAbstractHDF5Writer::getHDF5FileTool()
    {
        if (_tools) return _tools;
        if (_h5File == nullptr) return nullptr;
        _tools = new IO::FITKHDF5FileTool(_h5File);
        return _tools;
    }

}