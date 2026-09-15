/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorMaterial.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"

namespace IO
{
    QString FITKInterfaceHDF5AdaptorMaterial::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorMaterial";
    }

    bool FITKInterfaceHDF5AdaptorMaterial::adaptR()
    {
        auto material = dynamic_cast<Interface::FITKAbstractMaterial*>(_dataObj);
        if (!_reader || !_h5Group || !material) return false;
        bool isR = true;
        isR &= readMaterial(material, *_h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorMaterial::adaptW()
    {
        auto material = dynamic_cast<Interface::FITKAbstractMaterial*>(_dataObj);
        if (!_writer || !_h5Group || !material) return false;
        bool isW = true;
        isW &= writeMaterial(material, *_h5Group);
        return isW;
    }

    bool FITKInterfaceHDF5AdaptorMaterial::readMaterial(Interface::FITKAbstractMaterial * material, H5::Group & h5Group)
    {
        if (!material || h5Group.getLocId() == -1) return false;
        if (!h5Group.attrExists("MaterialID")) return false;
        bool isR = true;
        isR &= readNDataObject(material, h5Group);
        //读取materialID
        int materialID = readIntAttribute(h5Group, "MaterialID");

        auto describe = readStrAttribute(h5Group, "Describe");
        material->setMaterialDescribe(QString::fromStdString(describe));
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorMaterial::writeMaterial(Interface::FITKAbstractMaterial * material, H5::Group & h5Group)
    {
        if (!material) return false;
        bool isW = true;
        std::string groupName = createParentAttribute(material, h5Group);
        if (groupName.empty()) return false;
        _h5CreateGroup = h5Group.createGroup(groupName);
        isW &= writeNDataObject(material, _h5CreateGroup);
        int materialID = material->getMaterialID();
        writeIntAttribute(_h5CreateGroup, "MaterialID", &materialID);

        QString describe = material->getMaterialDescribe();
        writeStrAttribute(_h5CreateGroup, "Describe", describe.toStdString());
        return isW;
    }

}


