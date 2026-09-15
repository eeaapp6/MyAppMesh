/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AbstractAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include <typeinfo>
#include <QColor>

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKAbstractEasyParam.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamString.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamRadioGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamLabel.h"
//命名空间
namespace IO
{
    FITKFlowOFHDF5AbstractAdaptor::~FITKFlowOFHDF5AbstractAdaptor()
    {

    }

    void FITKFlowOFHDF5AbstractAdaptor::setReader(IO::FITKAbstractHDF5Reader* reader)
    {
        _reader = reader;
    }

    void FITKFlowOFHDF5AbstractAdaptor::setWriter(IO::FITKAbstractHDF5Writer* writer)
    {
        _writer = writer;
    }

    void FITKFlowOFHDF5AbstractAdaptor::setH5Group(H5::Group* g)
    {
        _h5Group = g;
    }

    void FITKFlowOFHDF5AbstractAdaptor::consoleMessage(int errorLevel, QString str)
    {
        //判断是读取还是写出
        QString isState = errorLevel ? "True" : "False";
        if (_reader)
        {
            _reader->consoleMessage(errorLevel, str + isState);
        }
        else if(_writer)
        {
            _writer->consoleMessage(errorLevel, str + isState);
        }
    }

    bool FITKFlowOFHDF5AbstractAdaptor::writeNDataObject(Core::FITKAbstractNDataObject * obj, H5::Group & h5Group)
    {
        if (!_writer || !obj) return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出名称
        QString dataObjectName = obj->getDataObjectName();
        if (!tool->writeGroupAttrStr(h5Group, "DataObjectName", dataObjectName.toStdString()))
            return false;

        if (!this->writeDataObject(obj, h5Group))
            return false;
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::readNDataObject(Core::FITKAbstractNDataObject * obj, H5::Group & h5Group)
    {
        if (!_reader || !obj) return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::string dataObjectName;
        if (!tool->readGroupStr(h5Group, "DataObjectName", dataObjectName))
            return false;
        //读取名称
        QString name = QString::fromStdString(dataObjectName);
        obj->setDataObjectName(name);

        if (!this->readDataObject(obj, h5Group))
            return false;
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::writeDataObject(Core::FITKAbstractDataObject * obj, H5::Group & h5Group)
    {
        if (!_writer || !obj) return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        int dataObjectID = obj->getDataObjectID();
        int parentDataID = obj->getParentDataID();
        bool isEnableDataObject = obj->isEnable();
        //写出数据
        if (!tool->writeGroupAttrInt(h5Group, "DataObjectID", dataObjectID))
            return false;
        if (!tool->writeGroupAttrInt(h5Group, "ParentDataObjectID", parentDataID))
            return false;
        if (!tool->writeGroupAttrBool(h5Group, "IsEnableDataObject", isEnableDataObject))
            return false;
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

    bool FITKFlowOFHDF5AbstractAdaptor::readDataObject(Core::FITKAbstractDataObject * obj, H5::Group & h5Group)
    {
        if (!_reader || !obj) return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取数据
        int dataObjectID = 0;
        int parentDataID = 0;
        bool isEnableDataObject = false;
        if (!tool->readGroupint(h5Group, "DataObjectID", &dataObjectID))
            return false;
        if (!tool->readGroupint(h5Group, "ParentDataObjectID", &parentDataID))
            return false;
        if (!tool->readGroupBool(h5Group, "IsEnableDataObject", &isEnableDataObject))
            return false;

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

    bool FITKFlowOFHDF5AbstractAdaptor::writeVariantToHDF5(const QString & datasetName, const QVariant & value, H5::Group & h5Group)
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

    bool FITKFlowOFHDF5AbstractAdaptor::readVariantFromHDF5(const QString & datasetName, QVariant & value, H5::Group & h5Group)
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

    bool FITKFlowOFHDF5AbstractAdaptor::parameterToolW(H5::Group & h5Group, Core::FITKParameter * paraData)
    {
        if (!paraData || !_writer)return false;
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出参数数量
        int count = paraData->getDataCount();
        tool->writeGroupAttrInt(h5Group, "paraCount", count);
        for (int i = 0; i < count; ++i)
        {
            //写出参数数据
            Core::FITKAbstractEasyParam* para = paraData->getDataByIndex(i);
            if (!para)continue;
            int paraType = para->getParamType();
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> childH5Group = tool->createGp(h5Group, QString::fromStdString(name));
            if (std::get<0>(childH5Group) == false) return false;
            tool->writeGroupAttrInt(std::get<1>(childH5Group), "paraType", paraType);
            //设置参数
            writeNDataObject(para, std::get<1>(childH5Group));
            if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPBool)
            {
                Core::FITKParamBool* paraBool = dynamic_cast<Core::FITKParamBool*>(para);
                if (!paraBool) return false;
                this->parameterBoolW(std::get<1>(childH5Group), paraBool);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPCombox)
            {
                Core::FITKParamCombox* paraCombox = dynamic_cast<Core::FITKParamCombox*>(para);
                if (!paraCombox) return false;
                this->parameterComboxW(std::get<1>(childH5Group), paraCombox);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            {
                Core::FITKParamDouble* paraDouble = dynamic_cast<Core::FITKParamDouble*>(para);
                if (!paraDouble) return false;
                this->parameterDoubleW(std::get<1>(childH5Group), paraDouble);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPInt)
            {
                Core::FITKParamInt* paraInt = dynamic_cast<Core::FITKParamInt*>(para);
                if (!paraInt) return false;
                this->parameterIntW(std::get<1>(childH5Group), paraInt);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPLabel)
            {
                Core::FITKParamLabel* paraLabel = dynamic_cast<Core::FITKParamLabel*>(para);
                if (!paraLabel) return false;
                this->parameterLabelW(std::get<1>(childH5Group), paraLabel);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPString)
            {
                Core::FITKParamString* paraString = dynamic_cast<Core::FITKParamString*>(para);
                if (!paraString) return false;
                this->parameterStringW(std::get<1>(childH5Group), paraString);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPRadioGroup)
            {
                Core::FITKParamRadioGroup* paraRadioGroup = dynamic_cast<Core::FITKParamRadioGroup*>(para);
                if (!paraRadioGroup) return false;
                this->parameterRadioGroupW(std::get<1>(childH5Group), paraRadioGroup);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
            {
                Core::FITKParamDoubleList* paraDoubleList = dynamic_cast<Core::FITKParamDoubleList*>(para);
                if (!paraDoubleList) return false;
                this->parameterDoubleListW(std::get<1>(childH5Group), paraDoubleList);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPBoolGroup)
            {
                Core::FITKParamBoolGroup* paraBoolGroup = dynamic_cast<Core::FITKParamBoolGroup*>(para);
                if (!paraBoolGroup) return false;
                this->parameterBoolGroupW(std::get<1>(childH5Group), paraBoolGroup);
            }
        }
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterBoolW(H5::Group& h5Group, Core::FITKParamBool* paraBool)
    {
        if (!paraBool || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        bool value = paraBool->getValue();
        tool->writeGroupAttrBool(h5Group, "Value", value);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterComboxW(H5::Group& h5Group, Core::FITKParamCombox* paraCombox)
    {
        if (!paraCombox || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        std::string value = paraCombox->getValue().join(';').toStdString();
        int currentIndex = paraCombox->getIndex();
        tool->writeGroupAttrInt(h5Group, "CurrentIndex", currentIndex);
        tool->writeGroupAttrStr(h5Group, "Value", value);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterDoubleW(H5::Group& h5Group, Core::FITKParamDouble* paraDouble)
    {
        if (!paraDouble || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        double value = paraDouble->getValue();
        double range[2] = { 0 };
        paraDouble->getRange(range);
        tool->writeGroupAttrDouble(h5Group, "Value", value);
        tool->writeGroupAttrDoubleArray(h5Group, "Range", range, 1, 2);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterIntW(H5::Group& h5Group, Core::FITKParamInt* paraInt)
    {
        if (!paraInt || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        int value = paraInt->getValue();
        int range[2] = { 0 };
        paraInt->getRange(range);
        tool->writeGroupAttrInt(h5Group, "Value", value);
        tool->writeGroupAttrIntArray(h5Group, "Range", range, 1, 2);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterLabelW(H5::Group& h5Group, Core::FITKParamLabel* paraLabel)
    {
        if (!paraLabel || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        std::string value = paraLabel->getValue().join(';').toStdString();
        tool->writeGroupAttrStr(h5Group, "Value", value);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterStringW(H5::Group& h5Group, Core::FITKParamString* paraString)
    {
        if (!paraString || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        std::string value = paraString->getValue().toStdString();
        tool->writeGroupAttrStr(h5Group, "Value", value);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterRadioGroupW(H5::Group& h5Group, Core::FITKParamRadioGroup* paraRadioGroup)
    {
        if (!paraRadioGroup || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        H5::Group subGroupH5 = h5Group.createGroup("SubValue");
        //共有数据
        Core::FITKParameter* subValue = paraRadioGroup->getSubValuePara();
        if (subValue) parameterToolW(subGroupH5, subValue);
        //数据组
        H5::Group radioGroupH5 = h5Group.createGroup("RadioGroup");
        QList<Core::FITKRadioGroupValue> radioGroupList = paraRadioGroup->getRadioValues();
        int valueCount = radioGroupList.size();
        tool->writeGroupAttrInt(radioGroupH5, "ValueCount", valueCount);
        for (int j = 0; j < valueCount; ++j)
        {
            std::string nameRadio = std::to_string(j);
            H5::Group radioH5Group = radioGroupH5.createGroup(nameRadio);
            Core::FITKRadioGroupValue radioGroup = radioGroupList.at(j);
            tool->writeGroupAttrStr(radioH5Group, "ValueName", radioGroup._name.toStdString());
            //数据组
            H5::Group valueH5Group = radioH5Group.createGroup("Value");
            if (radioGroup._value) parameterToolW(valueH5Group, radioGroup._value);
            //标签
            QStringList label;
            for (int z = 0; z < radioGroup._labelValue.size(); ++z)
            {
                if (radioGroup._labelValue.at(z))
                    label.append("true");
                else
                    label.append("false");
            }
            tool->writeGroupAttrStr(radioH5Group, "LabelValue", label.join(';').toStdString());
        }
        //当前索引
        int currentIndex = paraRadioGroup->getCurrentValueIndex();
        tool->writeGroupAttrInt(h5Group, "CurrentIndex", currentIndex);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterDoubleListW(H5::Group& h5Group, Core::FITKParamDoubleList* paraDoubleList)
    {
        if (!paraDoubleList || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        int valueCount = paraDoubleList->getCount();
        tool->writeGroupAttrInt(h5Group, "ValueCount", valueCount);
        for (int j = 0; j < valueCount; ++j)
        {
            Core::FITKParamDouble* paraDouble = paraDoubleList->getDoubleData(j);
            if (!paraDouble)continue;
            std::string nameDouble = std::to_string(j);
            H5::Group doubleH5Group = h5Group.createGroup(nameDouble);
            writeNDataObject(paraDouble, doubleH5Group);
            double value = paraDouble->getValue();
            double range[2] = { 0 };
            paraDouble->getRange(range);
            tool->writeGroupAttrDouble(doubleH5Group, "Value", value);
            tool->writeGroupAttrDoubleArray(doubleH5Group, "Range", range, 1, 2);
        }
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterBoolGroupW(H5::Group& h5Group, Core::FITKParamBoolGroup* paraBoolGroup)
    {
        if (!paraBoolGroup || !_writer) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        bool value = paraBoolGroup->getValue();
        tool->writeGroupAttrBool(h5Group, "Value", value);
        H5::Group boolGroupH5 = h5Group.createGroup("BoolGroup");
        Core::FITKParameter* boolGroup = paraBoolGroup->getValueGroup();
        if (boolGroup) parameterToolW(boolGroupH5, boolGroup);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterToolR(H5::Group& h5Group, Core::FITKParameter* paraData)
    {
        if (!paraData || !_reader)return false;
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        paraData->clear();
        //读取参数数量
        int paraCount = 0;
        if (!tool->readGroupint(h5Group, "paraCount", &paraCount))
            return false;
        for (int i = 0; i < paraCount; ++i)
        {
            //读取参数数据
            std::string name = std::to_string(i);
            int paraType = 0;
            std::tuple<bool, H5::Group> childH5Group = tool->openGp(h5Group, name.c_str());
            if (std::get<0>(childH5Group) == false) return false;
            if (!tool->readGroupint(std::get<1>(childH5Group), "paraType", &paraType))
                return false;
            Core::FITKAbstractEasyParam* para = nullptr;
            if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPBool)
            {
                para = new Core::FITKParamBool;
                Core::FITKParamBool* paraBool = dynamic_cast<Core::FITKParamBool*>(para);
                this->parameterBoolR(std::get<1>(childH5Group), paraBool);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPCombox)
            {
                para = new Core::FITKParamCombox;
                Core::FITKParamCombox* paraCombox = dynamic_cast<Core::FITKParamCombox*>(para);
                this->parameterComboxR(std::get<1>(childH5Group), paraCombox);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            {
                para = new Core::FITKParamDouble;
                Core::FITKParamDouble* paraDouble = dynamic_cast<Core::FITKParamDouble*>(para);
                this->parameterDoubleR(std::get<1>(childH5Group), paraDouble);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPInt)
            {
                para = new Core::FITKParamInt;
                Core::FITKParamInt* paraInt = dynamic_cast<Core::FITKParamInt*>(para);
                this->parameterIntR(std::get<1>(childH5Group), paraInt);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPLabel)
            {
                para = new Core::FITKParamLabel;
                Core::FITKParamLabel* paraLabel = dynamic_cast<Core::FITKParamLabel*>(para);
                this->parameterLabelR(std::get<1>(childH5Group), paraLabel);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPString)
            {
                para = new Core::FITKParamString;
                Core::FITKParamString* paraString = dynamic_cast<Core::FITKParamString*>(para);
                this->parameterStringR(std::get<1>(childH5Group), paraString);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPRadioGroup)
            {
                para = new Core::FITKParamRadioGroup;
                Core::FITKParamRadioGroup* paraRadioGroup = dynamic_cast<Core::FITKParamRadioGroup*>(para);
                this->parameterRadioGroupR(std::get<1>(childH5Group), paraRadioGroup);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
            {
                para = new Core::FITKParamDoubleList;
                Core::FITKParamDoubleList* paraDoubleList = dynamic_cast<Core::FITKParamDoubleList*>(para);
                this->parameterDoubleListR(std::get<1>(childH5Group), paraDoubleList);
            }
            else if (paraType == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPBoolGroup)
            {
                para = new Core::FITKParamBoolGroup;
                Core::FITKParamBoolGroup* paraBoolGroup = dynamic_cast<Core::FITKParamBoolGroup*>(para);
                this->parameterBoolGroupR(std::get<1>(childH5Group), paraBoolGroup);
            }
            if (!para)return false;
            readNDataObject(para, std::get<1>(childH5Group));
            paraData->appendDataObj(para);
        }
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterBoolR(H5::Group& h5Group, Core::FITKParamBool* paraBool)
    {
        if (!paraBool || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        bool value = false;
        tool->readGroupBool(h5Group, "Value", &value);
        paraBool->setValue(value);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterComboxR(H5::Group& h5Group, Core::FITKParamCombox* paraCombox)
    {
        if (!paraCombox || !_reader) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::string v;
        tool->readGroupStr(h5Group, "Value", v);
        QString value = QString::fromStdString(v);
        paraCombox->setValue(value.split(";"));
        int currentIndex = 0;
        tool->readGroupint(h5Group, "CurrentIndex", &currentIndex);
        paraCombox->setIndex(currentIndex);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterDoubleR(H5::Group& h5Group, Core::FITKParamDouble* paraDouble)
    {
        if (!paraDouble || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        double value = 0.0;
        tool->readGroupDouble(h5Group, "Value", &value);
        paraDouble->setValue(value);
        double range[2] = { 0 };
        tool->readGroupAttrDoubleArray(h5Group, "Value", range, 1, 2);
        paraDouble->setRange(range);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterIntR(H5::Group& h5Group, Core::FITKParamInt* paraInt)
    {
        if (!paraInt || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        int value = 0;
        tool->readGroupint(h5Group, "Value", &value);
        paraInt->setValue(value);
        int range[2] = { 0 };
        tool->readGroupAttrIntArray(h5Group, "Value", range, 1, 2);
        paraInt->setRange(range);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterLabelR(H5::Group& h5Group, Core::FITKParamLabel* paraLabel)
    {
        if (!paraLabel || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::string v;
        tool->readGroupStr(h5Group, "Value", v);
        QString value = QString::fromStdString(v);
        paraLabel->setValue(value.split(';'));
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterStringR(H5::Group& h5Group, Core::FITKParamString* paraString)
    {
        if (!paraString || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        std::string v;
        tool->readGroupStr(h5Group, "Value", v);
        paraString->setValue(QString::fromStdString(v));
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterRadioGroupR(H5::Group& h5Group, Core::FITKParamRadioGroup* paraRadioGroup)
    {
        if (!paraRadioGroup || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!h5Group.nameExists("SubValue"))return false;
        H5::Group subGroupH5 = h5Group.openGroup("SubValue");
        if (subGroupH5.getNumAttrs() > 0)
        {
            Core::FITKParameter* subValue = new Core::FITKParameter;
            parameterToolR(subGroupH5, subValue);
            paraRadioGroup->setSubValue(subValue);
        }
        if (!h5Group.nameExists("RadioGroup"))return false;
        H5::Group radioGroupH5 = h5Group.openGroup("RadioGroup");
        int valueCount = 0;
        tool->readGroupint(radioGroupH5, "ValueCount", &valueCount);
        for (int j = 0; j < valueCount; ++j)
        {
            std::string nameRadio = std::to_string(j);
            if (!radioGroupH5.nameExists(nameRadio))return false;

            H5::Group radioH5Group = radioGroupH5.openGroup(nameRadio);
            std::string v;
            tool->readGroupStr(radioH5Group, "ValueName", v);
            QString valueName = QString::fromStdString(v);
            //标签
            tool->readGroupStr(radioH5Group, "LabelValue", v);
            QStringList label = QString::fromStdString(v).split(';');
            QList<bool> labelValue;
            for (int z = 0; z < label.size(); ++z)
            {
                if (label.at(z) == "false")
                    labelValue.append(false);
                else if (label.at(z) == "true")
                    labelValue.append(true);
            }
            //数据组
            if (!radioH5Group.nameExists("Value")) return false;
            H5::Group valueH5Group = radioH5Group.openGroup("Value");
            Core::FITKParameter* radioValue = nullptr;
            if (valueH5Group.getNumAttrs() > 0)
            {
                radioValue = new Core::FITKParameter;
                parameterToolR(valueH5Group, radioValue);
            }
            paraRadioGroup->appendValue(valueName, radioValue, labelValue);
        }
        //当前索引
        int currentIndex = 0;
        tool->readGroupint(h5Group, "CurrentIndex", &currentIndex);
        paraRadioGroup->setCurrentIndex(currentIndex);
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterDoubleListR(H5::Group& h5Group, Core::FITKParamDoubleList* paraDoubleList)
    {
        if (!paraDoubleList || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        int valueCount = 0;
        tool->readGroupint(h5Group, "ValueCount", &valueCount);
        for (int j = 0; j < valueCount; ++j)
        {
            std::string nameDouble = std::to_string(j);
            if (!h5Group.nameExists(nameDouble))continue;
            H5::Group doubleH5Group = h5Group.openGroup(nameDouble);
            Core::FITKParamDouble* paraDouble = new Core::FITKParamDouble;
            readNDataObject(paraDouble, doubleH5Group);
            double value = 0.0;
            tool->readGroupDouble(doubleH5Group, "Value", &value);
            paraDouble->setValue(value);
            double range[2] = { 0 };
            tool->readGroupAttrDoubleArray(doubleH5Group, "Value", range, 1, 2);
            paraDouble->setRange(range);
            paraDoubleList->pushDoubleData(paraDouble);
        }
        return true;
    }

    bool FITKFlowOFHDF5AbstractAdaptor::parameterBoolGroupR(H5::Group& h5Group, Core::FITKParamBoolGroup* paraBoolGroup)
    {
        if (!paraBoolGroup || !_reader)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        bool value = false;
        tool->readGroupBool(h5Group, "Value", &value);
        paraBoolGroup->setValue(value);
        if (!h5Group.nameExists("BoolGroup"))return false;
        H5::Group boolGroupH5 = h5Group.openGroup("BoolGroup");
        if (boolGroupH5.getNumAttrs() > 0)
        {
            Core::FITKParameter* boolGroup = new Core::FITKParameter;
            parameterToolR(boolGroupH5, boolGroup);
            paraBoolGroup->setValueGroup(boolGroup);
        }
        return true;
    }

}
