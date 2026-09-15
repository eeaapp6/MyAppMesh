/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataGeomPartAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossGeomPart.h"

namespace Interface
{
    bool FITKRadiossDataGeomPartAdaptor::adaptR()
    {
        _data = dynamic_cast<Radioss::FITKRadiossGeomPart*>(_dataObj);
        if (_reader == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readDataObject(_data, *_h5Group)) return false;
        int geomCommandID = -1;
        if(!h5tools->readGroupint(*_h5Group, "GeomCommandID", &geomCommandID)) return false;
        _data->setGeoPartID(geomCommandID);
        return true;
    }

    bool FITKRadiossDataGeomPartAdaptor::adaptW()
    {
        _data = dynamic_cast<Radioss::FITKRadiossGeomPart*>(_dataObj);
        if (_writer == nullptr || _data == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeDataObject(_data, *_h5Group))return false;
        h5tools->writeGroupAttrInt(*_h5Group, "GeomCommandID", _data->getGeoPartID());
        return true;
    }
}