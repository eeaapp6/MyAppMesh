/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractHDF5AdaptorMesh.h"

namespace IO
{

    QString FITKAbstractHDF5AdaptorMesh::getAdaptorClass()
    {
        return QString();
    }

    bool FITKAbstractHDF5AdaptorMesh::adaptR()
    {
        return false;
    }

    bool FITKAbstractHDF5AdaptorMesh::adaptW()
    {
        return false;
    }

}
