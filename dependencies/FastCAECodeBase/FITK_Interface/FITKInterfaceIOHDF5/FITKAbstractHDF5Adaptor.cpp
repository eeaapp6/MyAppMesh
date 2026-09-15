/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractHDF5Adaptor.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Kernel/FITKCore/FITKLibInfo.h"

FITKLIBINFOREGISTER(FITKInterfaceIOHDF5, FITKInterfaceIOHDF5Version);

//命名空间
namespace IO
{
    FITKAbstractHDF5Adaptor::~FITKAbstractHDF5Adaptor()
    {

    }

    //设置文件读取器
    void FITKAbstractHDF5Adaptor::setFileReader(FITKAbstractHDF5Reader* reader)
    {
        _reader = reader;
    }

    //设置文件写出器
    void FITKAbstractHDF5Adaptor::setFileWriter(FITKAbstractHDF5Writer* writer)
    {
        _writer = writer;
    }

    void FITKAbstractHDF5Adaptor::setH5GroupData(H5::Group & h5Group)
    {
        _h5Group = &h5Group;
    }

    bool FITKAbstractHDF5Adaptor::adaptR()
    {
        //auto object = dynamic_cast<Core::FITKAbstractNDataObject*>(_dataObj);
        //if (object == nullptr) return false;
        //return readNDataObject(object, *_h5Group);
        return false;
    }

    bool FITKAbstractHDF5Adaptor::adaptW()
    {
        //auto object = dynamic_cast<Core::FITKAbstractNDataObject*>(_dataObj);
        //if (object == nullptr) return false;
        //return writeNDataObject(object, *_h5Group);
        return false;
    }

    bool FITKAbstractHDF5Adaptor::readNDataObject(Core::FITKAbstractNDataObject* obj, H5::H5Object& h5Object)
    {
        if (!obj || h5Object.getId() == -1) return false;

        if (!h5Object.attrExists("DataObjectID")) return false;
        if (!h5Object.attrExists("DataObjectName")) return false;
        if (!h5Object.attrExists("ParentDataObjectID")) return false;

        int dataObjectID = readIntAttribute(h5Object, "DataObjectID");
        auto str = readStrAttribute(h5Object, "DataObjectName");
        QString dataObjectName = QString::fromStdString(str);
        int parentDataObjectID = readIntAttribute(h5Object, "ParentDataObjectID");

        if (h5Object.attrExists("IsEnable"))
        {
            bool isEnable = readBoolAttribute(h5Object, "IsEnable");
            obj->enable(isEnable);
        }

        //设置DataObjectID
        Core::FITKAbstractDataIDAlter dataIDAlter;
        dataIDAlter.modifyDataID(obj, dataObjectID);

        obj->setDataObjectName(dataObjectName);
        obj->setParentDataID(parentDataObjectID);
        return true;
    }

    bool FITKAbstractHDF5Adaptor::readDataObject(Core::FITKAbstractDataObject * obj, H5::H5Object & h5Object)
    {
        if (!obj || h5Object.getId() == -1) return false;

        if (!h5Object.attrExists("DataObjectID")) return false;
        if (!h5Object.attrExists("ParentDataObjectID")) return false;

        int dataObjectID = readIntAttribute(h5Object, "DataObjectID");
        int parentDataObjectID = readIntAttribute(h5Object, "ParentDataObjectID");
        if (h5Object.attrExists("IsEnable"))
        {
            bool isEnable = readBoolAttribute(h5Object, "IsEnable");
            obj->enable(isEnable);
        }

        //设置DataObjectID
        Core::FITKAbstractDataIDAlter dataIDAlter;
        dataIDAlter.modifyDataID(obj, dataObjectID);
        obj->setParentDataID(parentDataObjectID);
        return true;
    }

    bool FITKAbstractHDF5Adaptor::writeNDataObject(Core::FITKAbstractNDataObject* obj, H5::H5Object& h5Object)
    {
        int dataObjectID = obj->getDataObjectID();
        QString dataObjectName = obj->getDataObjectName();
        int parentDataID = obj->getParentDataID();
        bool isEnable = obj->isEnable();

        writeIntAttribute(h5Object, "DataObjectID", &dataObjectID);
        writeStrAttribute(h5Object, "DataObjectName", dataObjectName.toStdString());
        writeIntAttribute(h5Object, "ParentDataObjectID", &parentDataID);
        writeBoolAttribute(h5Object, "IsEnable", &isEnable);
        return true;
    }

    bool FITKAbstractHDF5Adaptor::writeDataObject(Core::FITKAbstractDataObject * obj, H5::H5Object & h5Object)
    {
        int dataObjectID = obj->getDataObjectID();
        int parentDataID = obj->getParentDataID();
        bool isEnable = obj->isEnable();
        writeIntAttribute(h5Object, "DataObjectID", &dataObjectID);
        writeIntAttribute(h5Object, "ParentDataObjectID", &parentDataID);
        writeBoolAttribute(h5Object, "IsEnable", &isEnable);
        return true;
    }

    bool FITKAbstractHDF5Adaptor::readStrAttribute(H5::Group& h5group, std::string name, std::string& str)
    {
        if (!h5group.attrExists(name)) return false;
        H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
        H5::Attribute att = h5group.openAttribute(name);
        att.read(datatype, str);
        return true;
    }

    std::string FITKAbstractHDF5Adaptor::readStrAttribute(H5::H5Object& h5group, std::string name, bool* b /*= nullptr */)
    {
        if (nullptr != b) *b = false;
        if (!h5group.attrExists(name)) return "errorString";
        H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
        H5::Attribute att = h5group.openAttribute(name);
        std::string str{};
        att.read(datatype, str);
        if (nullptr != b) *b = true;
        return str;
    }

    bool FITKAbstractHDF5Adaptor::readAttribute(H5::H5Object& h5group, std::string name, const H5::DataType& type, void* buf)
    {
        if (!h5group.attrExists(name)) return false;
        H5::Attribute att = h5group.openAttribute(name);
        att.read(type, buf);
        return true;
    }

    bool FITKAbstractHDF5Adaptor::readBoolAttribute(H5::H5Object& h5group, std::string name, bool* b /*= nullptr */)
    {
        if (nullptr != b) *b = false;
        if (!h5group.attrExists(name)) return false;
        H5::Attribute att = h5group.openAttribute(name);
        bool buf = true;
        att.read(H5::PredType::NATIVE_HBOOL, &buf);
        if (nullptr != b) *b = true;
        return buf;
    }

    double FITKAbstractHDF5Adaptor::readDoubleAttribute(H5::H5Object& h5group, std::string name, bool* b /*= nullptr */)
    {
        if (nullptr != b) *b = false;
        if (!h5group.attrExists(name)) return -9999.9;
        H5::Attribute att = h5group.openAttribute(name);
        double buf = 0.0;
        att.read(H5::PredType::NATIVE_DOUBLE, &buf);
        if (nullptr != b) *b = true;
        return buf;
    }

    int FITKAbstractHDF5Adaptor::readIntAttribute(H5::H5Object& h5group, std::string name, bool* b /*= nullptr */)
    {
        if (nullptr != b) *b = false;
        if (!h5group.attrExists(name)) return -9999;
        H5::Attribute att = h5group.openAttribute(name);
        int buf = 0;
        att.read(H5::PredType::NATIVE_INT, &buf);
        if (nullptr != b) *b = true;
        return buf;
    }

    bool FITKAbstractHDF5Adaptor::readDoubleDataSet(H5::Group & h5group, std::string name, QList<double>& buff, int n, int m, bool * b)
    {
        if (!h5group.nameExists(name)) return false;
        if (h5group.childObjType(name) != H5O_TYPE_DATASET) return false;
        H5::DataSet dataSet = h5group.openDataSet(name);
        H5::DataSpace space = dataSet.getSpace();
        hsize_t dim[2];
        space.getSimpleExtentDims(dim);

        for (int i = 0; i < dim[0]; ++i)
        {
            H5::DataSpace memspace(1, dim);
            // hyperslab的开始
            hsize_t start[2] = { i, 0 };
            space.selectElements(H5S_SELECT_SET, 1, start);
            hsize_t dim2[] = { 1 };
            H5::DataSpace mspace2(1, dim2);
            double data;
            dataSet.read(&data, H5::PredType::NATIVE_DOUBLE, mspace2, space);
            buff.push_back(data);
        }
        return true;
    }

    bool FITKAbstractHDF5Adaptor::readDoubleAttrbuteDataSet(H5::Group & h5group, std::string name, QList<double>& buff, int n, int m, bool * b)
    {

        if (!h5group.attrExists(name)) return false;
        H5::Attribute transAttr = h5group.openAttribute(name);
        hsize_t       dim[2] = { n, m };
        H5::DataSpace SpaceThree(2, dim);
        double* data = new double[m];
        transAttr.read(H5::PredType::NATIVE_DOUBLE, data);
        for (int i = 0; i < m; i++)
        {
            buff.append(data[i]);
        }
        delete[] data;
        return true;
    }

    bool FITKAbstractHDF5Adaptor::readIntVectorAttribute(H5::Group& h5group, std::string name, void* buf)
    {
        if (!h5group.attrExists(name)) return false;
        H5::Attribute att = h5group.openAttribute(name);
        att.read(H5::PredType::NATIVE_INT, &buf);
        return true;
    }

    bool FITKAbstractHDF5Adaptor::getAttributeDim(H5::Group& h5group, std::string name, int& n, int& m)
    {
        if (!h5group.attrExists(name)) return false;
        H5::Attribute att = h5group.openAttribute(name);
        H5::DataSpace space = att.getSpace();
        hsize_t       dim[2];
        space.getSimpleExtentDims(dim);
        n = dim[0];
        m = dim[1];
        return true;
    }

    bool FITKAbstractHDF5Adaptor::getDataSetDim(H5::Group & h5group, std::string name, int & n, int & m)
    {
        if (!h5group.nameExists(name)) return false;
        H5::DataSet dataSet = h5group.openDataSet(name);
        H5::DataSpace space = dataSet.getSpace();
        hsize_t       dim[2];
        space.getSimpleExtentDims(dim);
        n = dim[0];
        m = dim[1];
        return true;
    }

    void FITKAbstractHDF5Adaptor::readStrDataSet(H5::Group& h5group, std::string name, std::vector< std::string >& rtStrs)
    {
        if (!h5group.exists(name)) return;
        if (h5group.childObjType(name) != H5O_TYPE_DATASET) return;
        H5::DataSet dataSet = h5group.openDataSet(name);
        H5::DataSpace space = dataSet.getSpace();
        hsize_t dim[2];
        space.getSimpleExtentDims(dim);
        hsize_t n = dim[0];
        // 创建字符串类型
        H5::StrType strType(H5::PredType::C_S1, H5T_VARIABLE);
        for (int i = 0; i < n; ++i)
        {
            H5::DataSpace memspace(1, dim);
            // hyperslab的开始
            hsize_t start[2] = { i, 0 };
            space.selectElements(H5S_SELECT_SET, 1, start);
            hsize_t dim2[] = { 1 };
            H5::DataSpace mspace2(1, dim2);
            std::string a;
            dataSet.read(a, strType, mspace2, space);
            rtStrs.push_back(a);
        }
    }

    void FITKAbstractHDF5Adaptor::readStrDataSet(H5::Group & h5group, std::string name, QList<QString>& rtStrs)
    {
        if (!h5group.exists(name)) return;
        if (h5group.childObjType(name) != H5O_TYPE_DATASET) return;
        H5::DataSet dataSet = h5group.openDataSet(name);
        H5::DataSpace space = dataSet.getSpace();
        hsize_t dim[2];
        space.getSimpleExtentDims(dim);
        hsize_t n = dim[0];
        // 创建字符串类型
        H5::StrType strType(H5::PredType::C_S1, H5T_VARIABLE);
        for (int i = 0; i < n; ++i)
        {
            H5::DataSpace memspace(1, dim);
            // hyperslab的开始
            hsize_t start[2] = { i, 0 };
            space.selectElements(H5S_SELECT_SET, 1, start);
            hsize_t dim2[] = { 1 };
            H5::DataSpace mspace2(1, dim2);
            std::string a;
            dataSet.read(a, strType, mspace2, space);
            rtStrs.append(QString::fromStdString(a));
        }
    }

    void FITKAbstractHDF5Adaptor::readIntDataSet(H5::H5Object & h5group, std::string name, QList<int>& data)
    {
        if (!h5group.exists(name)) return;
        if (h5group.childObjType(name) != H5O_TYPE_DATASET) return;
        H5::DataSet dataSet = h5group.openDataSet(name);
        H5::DataSpace space = dataSet.getSpace();
        hsize_t dim[2];
        space.getSimpleExtentDims(dim);
        hsize_t n = dim[0];

        for (int i = 0; i < n; ++i)
        {
            H5::DataSpace memspace(1, dim);
            // hyperslab的开始
            hsize_t start[2] = { i, 0 };
            space.selectElements(H5S_SELECT_SET, 1, start);
            hsize_t dim2[] = { 1 };
            H5::DataSpace mspace2(1, dim2);
            int d;
            dataSet.read(&d, H5::PredType::NATIVE_INT, mspace2, space);
            data.push_back(d);
        }
    }

    void FITKAbstractHDF5Adaptor::writeAttribute(H5::H5Object& h5group, std::string name, const H5::DataType& type, const void* buf, int n /*= 1*/, int m /*= 1 */)
    {
        hsize_t       dim[2] = { n, m };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = h5group.createAttribute(name, type, SpaceThree);
        transAttr.write(type, buf);
    }

    void FITKAbstractHDF5Adaptor::writeStrAttribute(H5::H5Object& h5group, std::string name, std::string data)
    {
        H5::DataSpace dataspace;
        H5::StrType   datatypeStr(H5::PredType::C_S1, H5T_VARIABLE);
        // 创建字符串类属性
        H5::Attribute desAttr = h5group.createAttribute(name, datatypeStr, dataspace);
        desAttr.write(datatypeStr, data);
    }

    void FITKAbstractHDF5Adaptor::writeBoolAttribute(H5::H5Object& h5group, std::string name, const void* buf, int n /*= 1*/, int m /*= 1 */)
    {
        hsize_t       dim[2] = { n, m };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = h5group.createAttribute(name, H5::PredType::NATIVE_HBOOL, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_HBOOL, buf);
    }

    void FITKAbstractHDF5Adaptor::writeDoubleAttribute(H5::H5Object& h5group, std::string name, const void* buf, int n /*= 1*/, int m /*= 1 */)
    {
        hsize_t       dim[2] = { n, m };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = h5group.createAttribute(name, H5::PredType::NATIVE_DOUBLE, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_DOUBLE, buf);
    }

    void FITKAbstractHDF5Adaptor::writeDoubleAttributeDataset(H5::H5Object& h5group, std::string name, QList<double>& buf, int n /*= 1*/, int m /*= 1 */)
    {
        double* data = new double[m];
        for (int i = 0; i < m; i++)
        {
            data[i] = buf[i];
        }
        hsize_t       dim[2] = { n, m };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = h5group.createAttribute(name, H5::PredType::NATIVE_DOUBLE, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_DOUBLE, data);
        delete[] data;
    }

    void FITKAbstractHDF5Adaptor::writeIntAttribute(H5::H5Object& h5group, std::string name, const void* buf, int n /*= 1*/, int m /*= 1 */)
    {
        hsize_t       dim[2] = { n, m };
        H5::DataSpace SpaceThree(2, dim);
        H5::Attribute transAttr = h5group.createAttribute(name, H5::PredType::NATIVE_INT, SpaceThree);
        transAttr.write(H5::PredType::NATIVE_INT, buf);
    }

    void FITKAbstractHDF5Adaptor::writeStrDataSet(H5::H5Object& h5group, std::string name, std::vector< std::string > data)
    {
        int                        nStr = data.size();
        hsize_t                    dim[2] = { nStr, 1 };
        std::vector< const char* > cStrings(nStr);
        for (hsize_t i = 0; i < nStr; i++)
        {
            cStrings[i] = data[i].c_str();
        }
        // 创建字符串类型
        H5::StrType strType(H5::PredType::C_S1, H5T_VARIABLE);
        // 创建数据集
        H5::DataSpace dataSpace(2, dim);
        H5::DataSet   dataSet = h5group.createDataSet(name, strType, dataSpace);
        dataSet.write(cStrings.data(), strType);
    }

    void FITKAbstractHDF5Adaptor::writeStrDataSet(H5::H5Object& h5group, std::string name, const QList< QString >& data)
    {
        int                        nStr = data.size();
        hsize_t                    dim[2] = { nStr, 1 };
        std::vector< const char* > cStrings(nStr);
        std::vector< std::string > strs;
        for (const QString& str1 : data)
        {
            strs.push_back(str1.toStdString());
        }

        for (hsize_t i = 0; i < nStr; i++)
        {
            cStrings[i] = strs[i].c_str();
        }
        // 创建字符串类型
        H5::StrType strType(H5::PredType::C_S1, H5T_VARIABLE);
        // 创建数据集
        H5::DataSpace dataSpace(2, dim);
        H5::DataSet   dataSet = h5group.createDataSet(name, strType, dataSpace);
        dataSet.write(cStrings.data(), strType);
    }

    void FITKAbstractHDF5Adaptor::writeIntDataSet(H5::H5Object& h5group, std::string name, const QList< int >& data)
    {
        int dataCount = data.size();

        hsize_t       dim1[] = { 1 };
        H5::DataSpace mspace1(1, dim1);
        hsize_t dim[2] = { dataCount, 1 };
        H5::DataSpace ptIDDataSpace(1, dim);
        // 创建数据集
        H5::DataSpace dataSpace(1, dim);
        H5::DataSet   dataSet = h5group.createDataSet(name, H5::PredType::NATIVE_INT, dataSpace);
        //表格
        hsize_t       coordPtData[1][2];
        coordPtData[0][1] = 0;
        for (int i = 0; i < dataCount; i++)
        {
            coordPtData[0][0] = i;
            dataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            dataSet.write(&data[i], H5::PredType::NATIVE_INT, mspace1, dataSpace);
        }
    }

    void FITKAbstractHDF5Adaptor::writeDoubleDataSet(H5::H5Object& h5group, std::string name, const QList< double >& data)
    {
        int dataCount = data.size();

        hsize_t       dim1[] = { 1 };
        H5::DataSpace mspace1(1, dim1);
        hsize_t dim[2] = { dataCount, 1 };
        H5::DataSpace ptIDDataSpace(1, dim);
        // 创建数据集
        H5::DataSpace dataSpace(1, dim);
        H5::DataSet   dataSet = h5group.createDataSet(name, H5::PredType::NATIVE_DOUBLE, dataSpace);
        //表格
        hsize_t       coordPtData[1][2];
        coordPtData[0][1] = 0;
        for (int i = 0; i < dataCount; i++)
        {
            coordPtData[0][0] = i;
            dataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            dataSet.write(&data[i], H5::PredType::NATIVE_DOUBLE, mspace1, dataSpace);
        }
    }

#ifdef QT_DEBUG

    std::string FITKAbstractHDF5Adaptor::createParentAttribute(Core::FITKAbstractNDataObject * obj, H5::H5Object & h5Object)
    {
        std::string name;
        if (!obj) return name;
        QString qName = obj->getDataObjectName();
        if (qName.isEmpty()) return name;
        name = qName.replace("/", "").toStdString();

        while (true)
        {
            if (h5Object.nameExists(name)) name += "|";
            else break;
        }

        //存储名称的链表 用index作为属性名称添加
        int numAttrCount = h5Object.getNumAttrs();
        writeStrAttribute(h5Object, std::to_string(numAttrCount), name);
        return name;
    }

#endif

    std::string FITKAbstractHDF5Adaptor::createParentAttribute(Core::FITKAbstractDataObject * obj, H5::H5Object & h5Object)
    {
        std::string name;
        if (!obj) return name;

        name = std::to_string(obj->getDataObjectID());

        //存储名称的链表 用index作为属性名称添加
        int numAttrCount = h5Object.getNumAttrs();
        writeStrAttribute(h5Object, std::to_string(numAttrCount), name);
        return name;
    }

}
