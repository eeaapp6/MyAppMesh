/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractGeoHDF5Adaptor.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKInterfaceGeoIOHDF5, FITKINTERFACEGEOIOHDF5VERSION);

namespace Interface
{
	FITKAbstractGeoHDF5Adaptor::~FITKAbstractGeoHDF5Adaptor()
	{}

    void FITKAbstractGeoHDF5Adaptor::setReader(IO::FITKAbstractHDF5Reader* reader)
    {
        _reader = reader;
    }

    void FITKAbstractGeoHDF5Adaptor::setWriter(IO::FITKAbstractHDF5Writer* writer)
    {
        _writer = writer;
    }
    void FITKAbstractGeoHDF5Adaptor::setH5Group(H5::Group *g)
    {
        _h5Group = g;
    }
}
