/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorSection.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractMaterial.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"

//命名空间
namespace IO
{
    QString FITKInterfaceHDF5AdaptorSection::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorSection";
    }

    bool FITKInterfaceHDF5AdaptorSection::adaptR()
    {
        auto section = dynamic_cast<Interface::FITKAbstractSection*>(_dataObj);
        if (!_reader || !_h5Group || !section) return false;
        bool isR = true;
        isR &= readSection(section, *_h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorSection::adaptW()
    {
        auto section = dynamic_cast<Interface::FITKAbstractSection*>(_dataObj);
        if (!_writer || !_h5Group || !section) return false;
        bool isW = true;
        isW &= writeSection(section, *_h5Group);
        return isW;
    }

    bool FITKInterfaceHDF5AdaptorSection::readSection(Interface::FITKAbstractSection* section, H5::Group& h5Group)
    {
        if (!section)return false;
        bool isR = true;
        //读取基础信息
        isR &= readNDataObject(section, h5Group);
        QList<int> materialIDs;
        //MaterialIDs
        if (!h5Group.nameExists("MaterialIDs")) return false;
        readIntDataSet(h5Group, "MaterialIDs", materialIDs);
        //循环写出材料信息
        for (int i = 0; i < materialIDs.size(); ++i)
        {
            section->setMaterial(materialIDs[i], i);
        }
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorSection::writeSection(Interface::FITKAbstractSection* section, H5::Group& h5Group)
    {
        if (!section)return false;
        QList<int> materialIDs;
        bool isW = true;
        int materialCount = section->getMaterialCount();

        //节点名称检查
        std::string groupName = createParentAttribute(section, h5Group);
        if (groupName.empty())return false;
        //创建group
        _h5CreateGroup = h5Group.createGroup(groupName);

        for (int i = 0; i < materialCount; ++i)
        {
            materialIDs.append(section->getMaterialID(i));
        }
        //MaterialIDs
        writeIntDataSet(_h5CreateGroup, "MaterialIDs", materialIDs);
        //写出基础信息
        isW &= writeNDataObject(section, _h5CreateGroup);
        return isW;
    }

}


