/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataHDF5Adaptor.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRadiossData.h"
#include <QVariant>
#include <QColor>
namespace Interface
{
    FITKRadiossDataHDF5Adaptor::~FITKRadiossDataHDF5Adaptor()
	{}

    void FITKRadiossDataHDF5Adaptor::setReader(IO::FITKAbstractHDF5Reader* reader)
    {
        _reader = reader;
    }

    void FITKRadiossDataHDF5Adaptor::setWriter(IO::FITKAbstractHDF5Writer* writer)
    {
        _writer = writer;
    }
    void FITKRadiossDataHDF5Adaptor::setH5Group(H5::Group *g)
    {
        _h5Group = g;
    }
    bool FITKRadiossDataHDF5Adaptor::readDataObject(Core::FITKAbstractNDataObject * obj, H5::Group & h5Group)
    {
        if (!_reader || !obj) return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::string dataObjectName;
        if (!tool->readGroupStr(h5Group, "DataObjectName", dataObjectName))return false;
        //读取名称
        QString name = QString::fromStdString(dataObjectName);
        obj->setDataObjectName(name);
        //读取数据
        int dataObjectID = 0;
        int parentDataID = 0;
        bool isEnableDataObject = false;
        if (!tool->readGroupint(h5Group, "DataObjectID", &dataObjectID)) return false;
        if (!tool->readGroupint(h5Group, "ParentDataObjectID", &parentDataID)) return false;
        if (!tool->readGroupBool(h5Group, "IsEnableDataObject", &isEnableDataObject)) return false;

        obj->setParentDataID(parentDataID);
        obj->enable(isEnableDataObject);
        //设置DataObjectID
        Core::FITKAbstractDataIDAlter dataIDAlter;
        dataIDAlter.modifyDataID(obj, dataObjectID);
        //用户数据
        std::tuple<bool, H5::Group> group = tool->openGp(h5Group, "DataObjectUserData");
        if (std::get<0>(group))
        {
            int numAttrs = std::get<1>(group).getNumAttrs();
            for (int i = 0; i < numAttrs; ++i)
            {
                H5::Attribute attr = std::get<1>(group).openAttribute(i);
                QString attrName = QString::fromStdString(attr.getName());
                QVariant v;
                if (!this->readVariantFromHDF5(attrName, v, std::get<1>(group))) continue;
                obj->setUserData(attrName.toInt(), v);
            }
        }
        return true;
    }
    bool FITKRadiossDataHDF5Adaptor::writeDataObject(Core::FITKAbstractNDataObject * obj, H5::Group & h5Group)
    {
        if (!_writer || !obj) return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出名称
        QString dataObjectName = obj->getDataObjectName();
        if (!tool->writeGroupAttrStr(h5Group, "DataObjectName", dataObjectName.toStdString())) return false;

        int dataObjectID = obj->getDataObjectID();
        int parentDataID = obj->getParentDataID();
        bool isEnableDataObject = obj->isEnable();
        //写出数据
        if (!tool->writeGroupAttrInt(h5Group, "DataObjectID", dataObjectID)) return false;
        if (!tool->writeGroupAttrInt(h5Group, "ParentDataObjectID", parentDataID)) return false;
        if (!tool->writeGroupAttrBool(h5Group, "IsEnableDataObject", isEnableDataObject)) return false;
        //用户数据
        QList<int> userKeys = obj->getUserDataKeys();
        if (userKeys.isEmpty()) return true;
        std::tuple<bool, H5::Group> group = tool->createGp(h5Group, "DataObjectUserData");
        if (std::get<0>(group))
        {
            for (int key : userKeys)
            {
                QVariant data = obj->getUserData(key);
                this->writeVariantToHDF5(QString::number(key), data, std::get<1>(group));
            }
        }
        return true;
    }
    bool FITKRadiossDataHDF5Adaptor::readRadiossData(Radioss::FITKAbstractRadiossData * obj, H5::Group & h5Group)
    {
        if (!_reader || !obj) return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        bool isVisible{ false };

        if (!tool->readGroupBool(h5Group, "IsVisible", &isVisible)) return false;
        obj->setVisible(isVisible);

    }
    bool FITKRadiossDataHDF5Adaptor::writeRadiossData(Radioss::FITKAbstractRadiossData * obj, H5::Group & h5Group)
    {
        if (!_writer || !obj) return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;

        bool isVisible = obj->getVisible();
        if (!tool->writeGroupAttrBool(h5Group, "IsVisible", isVisible)) return false;
    }

    bool FITKRadiossDataHDF5Adaptor::writeVariantToHDF5(const QString & datasetName, const QVariant & value, H5::Group & h5Group)
    {
        if (!_writer) return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出数据
        QString vStr;
        if (value.type() == QVariant::Type::Int) { // 整数类型
            int v = value.toInt();
            vStr = QString("%1:%2").arg("Int").arg(v);
        }
        else if (value.type() == QVariant::Type::Double) { // 双浮点数类型
            double v = value.toDouble();
            vStr = QString("%1:%2").arg("Double").arg(v);
        }
        else if (value.type() == QVariant::Type::Bool) { // 波尔类型
            bool v = value.toBool();
            vStr = QString("%1:%2").arg("Bool").arg(v);
        }
        else if (value.type() == QVariant::Type::String) { // 字符串类型
            QString v = value.toString();
            vStr = QString("%1:%2").arg("String").arg(v);
        }
        else if (value.type() == QVariant::Type::Color) { // 颜色类型
            QColor v = value.value<QColor>();
            vStr = QString("%1:%2").arg("Color").arg(v.name());
        }
        if (vStr.isEmpty()) return false;
        if (!tool->writeGroupAttrStr(h5Group, datasetName.toStdString().c_str(), vStr.toStdString()))
            return false;
        return true;
    }

    bool FITKRadiossDataHDF5Adaptor::readVariantFromHDF5(const QString & datasetName, QVariant & value, H5::Group & h5Group)
    {
        if (!_reader) return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;

        std::string vStr;
        if (!tool->readGroupStr(h5Group, datasetName.toStdString().c_str(), vStr))
            return false;

        QString str = QString::fromStdString(vStr);
        int index = str.indexOf(':');
        if (index <= 0) return false;
        QString type = str.mid(0, index);
        QString data = str.mid(index + 1, str.size() - index - 1);
        //获取数据
        if (type == "Int") { // 整数类型
            int v = data.toInt();
            value = QVariant(v);
        }
        else if (type == "Double") { // 双浮点数类型
            double v = data.toDouble();
            value = QVariant(v);
        }
        else if (type == "Bool") { // 波尔类型
            bool v = QVariant(data).toBool();
            value = QVariant(v);
        }
        else if (type == "String") { // 字符串类型
            QString v = data;
            value = QVariant(v);
        }
        else if (type == "Color") { // 颜色类型
            QColor v = QColor(data);
            value = QVariant(v);
        }

        return true;
    }

}
