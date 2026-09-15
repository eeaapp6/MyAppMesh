/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractHDF5Reader.h"
#include "FITKHDF5FileTool.h"
#include <QFile>
#include <thread>
#include <H5Cpp.h>

namespace IO
{

    FITKAbstractHDF5Reader::~FITKAbstractHDF5Reader()
    {
        //从其他位置传参形式的文件指针不进行处理
        if (!_createFileContext) return;

        this->closeFile();
        if (_h5File) delete _h5File;
        if (_tools) delete _tools;
    }

    void FITKAbstractHDF5Reader::setH5Root(H5::H5File* h5File)
    {
        _h5File = h5File;
    }

    void FITKAbstractHDF5Reader::setVersion(double v)
    {
        _version = v;
    }

    void FITKAbstractHDF5Reader::sendCurrentPercent(int p /*= -1*/)
    {
        //static int recordP = -1;
        //if (p < 0)
        //{
        //    //计算百分比
        //    const float pro = _currentLine / (float)_totalLine;
        //    p = pro * 100;
        //}
        ////避免闪烁不显示进度条
        //if (p == recordP) return;
        //recordP = p;
        //完成后等待主线程2秒
        if (p == 100) std::this_thread::sleep_for(std::chrono::seconds(2));
        emit sendProcessSig(this, p);
    }

    bool FITKAbstractHDF5Reader::createContext(QString & error)
    {
        if (!QFile::exists(_fileName))
        {
            //文件不存在
            error = QObject::tr("Invalid file name !");
            return false;
        }

        int size = QFile(_fileName).size();
        if (size <= 0)
        {
            error = QObject::tr("Empty file data !");
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
            _h5File = new H5::H5File(fnc, H5F_ACC_RDWR);
        }
        catch (const H5::FileIException& ex)
        {
            error = QString(ex.getCDetailMsg());
        }

        _createFileContext = true;
        //没有异常说明创建成功      
        return error.isEmpty();
    }

    void FITKAbstractHDF5Reader::closeFile()
    {
        if (_h5File)
            _h5File->close();
    }

    H5::H5File *FITKAbstractHDF5Reader::getHDF5File()
    {
        return _h5File;
    }

    IO::FITKHDF5FileTool* FITKAbstractHDF5Reader::getHDF5FileTool()
    {
        if (_tools) return _tools;
        if (_h5File == nullptr) return nullptr;
        _tools = new IO::FITKHDF5FileTool(_h5File);
        return _tools;
    }

}

