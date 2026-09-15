/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorSectionAssign.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKSectionAssign.h"
namespace IO
{
    QString FITKInterfaceHDF5AdaptorSectionAssign::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorSectionAssgin";
    }

    bool FITKInterfaceHDF5AdaptorSectionAssign::adaptR()
    {
        auto sectionAssign = dynamic_cast<Interface::FITKSectionAssign*>(_dataObj);
        if (!_reader || !_h5Group || !sectionAssign) return false;
        bool isR = true;
        isR &= readSectionAssign(sectionAssign, *_h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorSectionAssign::adaptW()
    {
        auto sectionAssign = dynamic_cast<Interface::FITKSectionAssign*>(_dataObj);
        if (!_writer || !_h5Group || !sectionAssign) return false;
        bool isW = true;
        isW &= writeSectionAssign(sectionAssign, *_h5Group);
        return isW;
    }
    bool FITKInterfaceHDF5AdaptorSectionAssign::readSectionAssign(Interface::FITKSectionAssign * section, H5::Group & h5Group)
    {
        if (!section || h5Group.getLocId() == -1) return false;
        if (!readDataObject(section, h5Group))return false;
        if (!h5Group.attrExists("ModelID")) return false;
        if (!h5Group.attrExists("SectionID")) return false;
        int ModelID = readIntAttribute(h5Group, "ModelID");
        int SectionID = readIntAttribute(h5Group, "SectionID");
        section->assign(ModelID, SectionID,false);
        return true;

    }
    bool FITKInterfaceHDF5AdaptorSectionAssign::writeSectionAssign(Interface::FITKSectionAssign * section, H5::Group & h5Group)
    {
        if (!section || h5Group.getLocId() == -1) return false;
        int ModelID = section->getModelID();
        int SectionID = section->getSectionID();

        std::string groupName = createParentAttribute(section, h5Group);
        if (groupName.empty())return false;

        _h5CreateGroup = h5Group.createGroup(groupName);

        writeIntAttribute(_h5CreateGroup, "ModelID", &ModelID);
        writeIntAttribute(_h5CreateGroup, "SectionID", &SectionID);
        return writeDataObject(section, _h5CreateGroup);
    }
}