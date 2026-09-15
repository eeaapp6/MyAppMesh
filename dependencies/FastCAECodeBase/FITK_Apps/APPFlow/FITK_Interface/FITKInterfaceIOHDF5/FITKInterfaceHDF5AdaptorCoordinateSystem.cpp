/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorCoordinateSystem.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceModel/FITKCoordinateSystem.h"


namespace IO
{
    QString FITKInterfaceHDF5AdaptorCoordinateSystem::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorCoordinateSystem";
    }

    bool FITKInterfaceHDF5AdaptorCoordinateSystem::adaptR()
    {
        auto coor = dynamic_cast<Interface::FITKCoordinateSystem*>(_dataObj);
        if (!_reader || !coor || !_h5Group) return false;
        bool isR = true;
        isR &= readCoordinateSystem(coor, *_h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorCoordinateSystem::adaptW()
    {
        auto coor = dynamic_cast<Interface::FITKCoordinateSystem*>(_dataObj);
        if (!_writer || !coor || !_h5Group) return false;
        bool isW = true;
        isW &= writeCoordinateSystem(coor, *_h5Group);
        return isW;
    }

    bool FITKInterfaceHDF5AdaptorCoordinateSystem::readCoordinateSystem(Interface::FITKCoordinateSystem * coor, H5::Group & h5Group)
    {
        return false;
    }

    bool FITKInterfaceHDF5AdaptorCoordinateSystem::writeCoordinateSystem(Interface::FITKCoordinateSystem * coor, H5::Group & h5Group)
    {
        if (!coor)return false;
        QList<int> materialIDs;
        bool isW = true;

        //double origin[3];
        //coor->getOrigin(origin);
        //double direction1[3];
        //coor->getDirection1(direction1);

        return isW;
    }

}


