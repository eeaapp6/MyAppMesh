/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorMaterialAssign.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKSectionAssign.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKMaterialAssign.h"

namespace IO
{
    QString FITKInterfaceHDF5AdaptorMaterialAssign::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorSection";
    }

    bool FITKInterfaceHDF5AdaptorMaterialAssign::adaptR()
    {
        auto materialAssign = dynamic_cast<Interface::FITKMaterialAssign*>(_dataObj);
        if (!_reader || !_h5Group || !materialAssign) return false;
        bool isR = true;
        isR &= readMaterialAssign(materialAssign, *_h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorMaterialAssign::adaptW()
    {
        auto materialAssign = dynamic_cast<Interface::FITKMaterialAssign*>(_dataObj);
        if (!_writer || !_h5Group || !materialAssign) return false;
        bool isW = true;
        isW &= writeMaterialAssign(materialAssign, *_h5Group);
        return isW;
    }
    bool FITKInterfaceHDF5AdaptorMaterialAssign::readMaterialAssign(Interface::FITKMaterialAssign* assign, H5::Group& h5Group)
    {
        if (!assign || h5Group.getLocId() == -1) return false;
        if (!h5Group.exists("MaterialID")) return false;
        if (!h5Group.exists("ModelID")) return false;

        int MaterialID = readIntAttribute(h5Group, "MaterialID");
        int ModelID = readIntAttribute(h5Group, "ModelID");
        assign->assign(MaterialID, ModelID);
        return readDataObject(assign, h5Group);

    }
    bool FITKInterfaceHDF5AdaptorMaterialAssign::writeMaterialAssign(Interface::FITKMaterialAssign* assign, H5::Group& h5Group)
    {
        if (!assign || h5Group.getLocId() == -1) return false;
        int MaterialID = assign->getMaterialID();
        int ModelID = assign->getModelID();

        std::string groupName = createParentAttribute(assign, h5Group);
        if (groupName.empty()) return false;

        _h5CreateGroup = h5Group.createGroup(groupName);

        writeIntAttribute(_h5CreateGroup, "MaterialID", &MaterialID);
        writeIntAttribute(_h5CreateGroup, "ModelID", &ModelID);

        return writeDataObject(assign, _h5CreateGroup);
    }
}