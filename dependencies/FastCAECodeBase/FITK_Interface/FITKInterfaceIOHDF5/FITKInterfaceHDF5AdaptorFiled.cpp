/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorFiled.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceModel/FITKCoordinateSystem.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKFieldDiscrete.h"
#include "FITK_Interface/FITKInterfacePhysics/FITKAnalyticalExpressField.h"

namespace IO
{
    QString FITKInterfaceHDF5AdaptorFiled::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorFiled";
    }

    bool FITKInterfaceHDF5AdaptorFiled::adaptR()
    {
        auto fd = dynamic_cast<Interface::FITKAbstractFiled*>(_dataObj);
        if (!_reader || !fd || !_h5Group) return false;
        bool isR = true;
        isR &= readFiled(fd, *_h5Group);
        return isR;
    }

    bool FITKInterfaceHDF5AdaptorFiled::adaptW()
    {
        auto fd = dynamic_cast<Interface::FITKAbstractFiled*>(_dataObj);
        if (!_writer || !fd || !_h5Group) return false;
        bool isW = true;
        isW &= writeFiled(fd, *_h5Group);
        return isW;
    }
    bool FITKInterfaceHDF5AdaptorFiled::readFiled(Interface::FITKAbstractFiled * filed, H5::Group & h5Group)
    {
        return false;
    }
    bool FITKInterfaceHDF5AdaptorFiled::writeFiled(Interface::FITKAbstractFiled * filed, H5::Group & h5Group)
    {
        if (!filed) return false;
        bool isW = true;
        bool isValid = false;

        std::string groupName = createParentAttribute(filed, h5Group);
        if (groupName.empty())return false;
        auto instanceGroup = h5Group.createGroup(groupName);
        if (!writeNDataObject(filed, instanceGroup)) return false;
        //type
        auto eFieldType = filed->getFieldType();
        //枚举转换字符 并写出
        Core::FITKEnumTransfer<Interface::FITKAbstractFiled::FITKFieldType> fitkTypeTrafer;
        QString TypeStr = fitkTypeTrafer.toString(eFieldType, isValid);
        if (!isValid) return false;
        writeStrAttribute(instanceGroup, "Type", TypeStr.toStdString());
        //modelID
        QString describe = filed->getDescribe();
        writeStrAttribute(instanceGroup, "Describe", describe.toStdString());
        if (eFieldType == Interface::FITKAbstractFiled::FITKFieldType::FFTAnalytical)
        {
            isW &= writeAnalyticalExpressField(filed, instanceGroup);
        }
        else if (eFieldType == Interface::FITKAbstractFiled::FITKFieldType::FFTDiscrete)
        {
            isW &= writeAnalyticalExpressField(filed, instanceGroup);
        }
        else
        {
            isW = false;
        }

        return isW;
    }
    bool FITKInterfaceHDF5AdaptorFiled::readAnalyticalExpressField(Interface::FITKAbstractFiled * filed, H5::Group & h5Group)
    {
        auto fd = dynamic_cast<Interface::FITKFieldAanlyticalExpress*>(filed);
        if (!fd) return false;

        auto sExpression = readStrAttribute(h5Group, "Expression");
        fd->setExpression(QString::fromStdString(sExpression));
        return true;
    }

    bool FITKInterfaceHDF5AdaptorFiled::writeAnalyticalExpressField(Interface::FITKAbstractFiled * filed, H5::Group & h5Group)
    {
        auto fd = dynamic_cast<Interface::FITKFieldAanlyticalExpress*>(filed);
        if (!fd) return false;

        QString sExpression = fd->getExpression();
        writeStrAttribute(h5Group, "Expression", sExpression.toStdString());
        return true;
    }

    bool FITKInterfaceHDF5AdaptorFiled::readDiscreteFiled(Interface::FITKAbstractFiled * filed, H5::Group & h5Group)
    {
        auto fd = dynamic_cast<Interface::FITKFieldDiscrete*>(filed);
        if (!fd) return false;
        bool isValid = false;

        auto sFieldLoc = readStrAttribute(h5Group, "FieldLoc");
        //枚举转换字符 并写出
        Core::FITKEnumTransfer<Interface::FITKFieldDiscrete::FITKDisFieldLoc> fitkTypeTrafer;
        auto eFieldLoc = fitkTypeTrafer.fromString(QString::fromStdString(sFieldLoc), isValid);
        if (!isValid) return false;
        fd->setDisFieldLoc(eFieldLoc);

        bool hasDefaultValue = readBoolAttribute(h5Group, "HasDefaultValue");
        fd->hasDefaultValue(hasDefaultValue);

        double dDefaultValue = readDoubleAttribute(h5Group, "DefaultValue");
        fd->setDefaultValue(dDefaultValue);

        int count = readIntAttribute(h5Group, "Count");
        fd->getFileDataCount();
        for (int i = 0; i < count; i++)
        {
            if (!h5Group.nameExists(std::to_string(i))) return false;
            auto fieldDataGroup = h5Group.openGroup(std::to_string(i));

            int id = readIntAttribute(fieldDataGroup, "ID");
            double value = readDoubleAttribute(fieldDataGroup, "Value");
            fd->appendFieldData(id, value);
        }
        return true;
    }

    bool FITKInterfaceHDF5AdaptorFiled::writeDiscreteFiled(Interface::FITKAbstractFiled * filed, H5::Group & h5Group)
    {
        auto fd = dynamic_cast<Interface::FITKFieldDiscrete*>(filed);
        if (!fd) return false;
        bool isValid = false;

        auto eFieldLoc = fd->getFieldLoc();
        //枚举转换字符 并写出
        Core::FITKEnumTransfer<Interface::FITKFieldDiscrete::FITKDisFieldLoc> fitkTypeTrafer;
        QString sFieldLoc = fitkTypeTrafer.toString(eFieldLoc, isValid);
        if (!isValid) return false;
        writeStrAttribute(h5Group, "FieldLoc", sFieldLoc.toStdString());

        bool hasDefaultValue = fd->hasDefaultValue();
        writeBoolAttribute(h5Group, "HasDefaultValue", &hasDefaultValue);

        double dDefaultValue = fd->getDefaultValue();
        writeDoubleAttribute(h5Group, "DefaultValue", &dDefaultValue);

        int count = fd->getFileDataCount();
        writeIntAttribute(h5Group, "Count", &count);

        for (int i = 0; i < count; i++)
        {
            auto fieldData = fd->getFieldData(i);
            auto fieldDataGroup = h5Group.createGroup(std::to_string(i));

            int id = fieldData._id;
            writeIntAttribute(fieldDataGroup, "ID", &id);

            double value = fieldData._value;
            writeDoubleAttribute(fieldDataGroup, "Value", &value);
        }
        return true;
    }
}


