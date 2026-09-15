/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshAbstractAdaptor.h"

namespace Gmsh
{
    void FITKGmshAbstractAdaptor::setFileReader(Interface::FITKAbstractTextReader* reader)
    {
        //设置读取器
        _reader = reader;
    }
    void FITKGmshAbstractAdaptor::setFileWriter(Interface::FITKAbstractTextWriter* writer)
    {
        //设置写出器
        _writer = writer;
    }
}


