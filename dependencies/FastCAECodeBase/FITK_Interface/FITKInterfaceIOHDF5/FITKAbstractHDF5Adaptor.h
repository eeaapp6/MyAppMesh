/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file   FITKAbstractHDF5Adaptor.h
 * @brief  抽象的HDF5文件读写适配器
 * @author fulipeng (flipengqd@yeah.net)
 * @date   2024-05-28
 *
 */
#ifndef _FITKABSTRACTHDF5ADAPTOR_H__
#define _FITKABSTRACTHDF5ADAPTOR_H__

#include "FITKInterfaceIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"
#include "H5Cpp.h"

namespace Core
{
    class FITKAbstractNDataObject;
}

namespace IO
{
    class FITKAbstractHDF5Reader;
    class FITKAbstractHDF5Writer;

    /**
     * @brief 抽象的HDF5文件读写适配器
     * @author fulipeng (fulipengqd@yeah.net)
     * @date 2024-05-30
     */
    class FITKINTERFACEHDF5IOAPI FITKAbstractHDF5Adaptor : public Adaptor::FITKAbstractIOAdaptor
    {
        FITKCLASS(IO, FITKAbstractHDF5Adaptor);

    public:
        explicit FITKAbstractHDF5Adaptor() = default;
        virtual ~FITKAbstractHDF5Adaptor() = 0;

        /**
         * @brief 设置INP文件读取器
         * @param reader INP读取类
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        void setFileReader(FITKAbstractHDF5Reader* reader);

        /**
         * @brief 设置INP文件写出器
         * @param writer INP写出类
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        void setFileWriter(FITKAbstractHDF5Writer* writer);

        /**
         * @brief 设置H5GroupData
         * @param h5Group
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-28
         */
        void setH5GroupData(H5::Group& h5Group);

        /**
         * @brief 适配器读取
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        bool adaptR() override;

        /**
         * @brief 适配器写出
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        bool adaptW() override;

        /**
         * @brief 读取字符串型属性
         * @return true，读取成功；false，读取失败。
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param str 读到的字符串
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        bool readStrAttribute(H5::Group& h5group, std::string name, std::string& str);
        /**
         * @brief 读取字符串型属性
         * @return 读到的字符串
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param b 成功标记
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        std::string readStrAttribute(H5::H5Object& h5group, std::string name, bool* b = nullptr);
        /**
         * @brief 读取指定类型属性
         * @return true，读取成功；false，读取失败。
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param type 属性类型
         * @param buf 属性变量指针
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        bool readAttribute(H5::H5Object& h5group, std::string name, const H5::DataType& type, void* buf);
        /**
         * @brief 读取bool类型属性
         * @return 读取到的bool值
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param b 成功标记
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        bool readBoolAttribute(H5::H5Object& h5group, std::string name, bool* b = nullptr);

        /**
         * @brief 读取双精度浮点类型属性
         * @return 读取到的双精度浮点数
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param b 成功标记
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        double readDoubleAttribute(H5::H5Object& h5group, std::string name, bool* b = nullptr);

        /**
         * @brief 读取双精度浮点类型链表数据
         * @return 读取到的双精度浮点数
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param b 成功标记
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-13
         */
        bool readDoubleDataSet(H5::Group& h5group, std::string name, QList<double>& buff, int n = 1, int m = 1, bool* b = nullptr);

        /**
         * @brief 读取双精度浮点类型链表属性
         * @return 读取到的双精度浮点数
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param b 成功标记
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-13
         */
        bool readDoubleAttrbuteDataSet(H5::Group& h5group, std::string name, QList<double>& buff, int n = 1, int m = 1, bool* b = nullptr);


        /**
         * @brief 读取整数类型属性
         * @return 成功标记
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param b 成功标记
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        bool readIntVectorAttribute(H5::Group& h5group, std::string name, void* buff);
        /**
         * @brief 读取整数类型属性
         * @return 读取到的整数
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param b 成功标记
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        int readIntAttribute(H5::H5Object& h5group, std::string name, bool* b = nullptr);

        /**
         * @brief 获取属性的行列数
         * @return true，读取成功；false，读取失败。
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param n 属性行数
         * @param m 属性列数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        bool getAttributeDim(H5::Group& h5group, std::string name, int& n, int& m);

        /**
         * @brief 获取数据集的行列数
         * @return true，读取成功；false，读取失败。
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param n 属性行数
         * @param m 属性列数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        bool getDataSetDim(H5::Group& h5group, std::string name, int& n, int& m);

        /**
         * @brief 读取字符串序列
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param rtStrs 读取到的字符串序列
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        void readStrDataSet(H5::Group& h5group, std::string name, std::vector< std::string >& rtStrs);

        /**
         * @brief 读取字符串序列
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param rtStrs 读取到的字符串序列
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        void readStrDataSet(H5::Group& h5group, std::string name, QList<QString>& rtStrs);

        /**
         * @brief 读取int序列
         * @param h5group 要读取的group
         * @param name 属性名称
         * @param data 读取到的数据链表
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-03
         */
        void readIntDataSet(H5::H5Object& h5group, std::string name, QList< int >& data);

        /**
         * @brief 向h5group中写入属性
         * @param h5group，要写入的group；name，属性名称；type，属性类型；buf，数据指针；n，m，属性的2维尺寸。
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-29
         */
        void writeAttribute(H5::H5Object& h5group, std::string name, const H5::DataType& type, const void* buf, int n = 1, int m = 1);
        void writeStrAttribute(H5::H5Object& h5group, std::string name, std::string data);
        void writeBoolAttribute(H5::H5Object& h5group, std::string name, const void* buf, int n = 1, int m = 1);
        void writeDoubleAttribute(H5::H5Object& h5group, std::string name, const void* buf, int n = 1, int m = 1);
        void writeDoubleAttributeDataset(H5::H5Object& h5group, std::string name, QList<double>& buf, int n = 1, int m = 1);
        void writeIntAttribute(H5::H5Object& h5group, std::string name, const void* buf, int n = 1, int m = 1);
        void writeStrDataSet(H5::H5Object& h5group, std::string name, std::vector< std::string > data);
        void writeStrDataSet(H5::H5Object& h5group, std::string name, const QList< QString >& data);
        void writeIntDataSet(H5::H5Object& h5group, std::string name, const QList< int >& data);
        void writeDoubleDataSet(H5::H5Object& h5group, std::string name, const QList< double >& data);


    protected:

#ifdef QT_DEBUG
        std::string createParentAttribute(Core::FITKAbstractNDataObject* obj, H5::H5Object & h5Object);
#endif
        std::string createParentAttribute(Core::FITKAbstractDataObject* obj, H5::H5Object & h5Object);

        /**
         * @brief 读取NDataObject信息
         * @param obj:写出对象
         * @return
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-04
         */
        bool readNDataObject(Core::FITKAbstractNDataObject* obj, H5::H5Object & h5Object);

        /**
         * @brief 读取NDataObject信息
         * @param obj:写出对象
         * @return
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-06-04
         */
        bool readDataObject(Core::FITKAbstractDataObject* obj, H5::H5Object & h5Object);

        /**
         * @brief 写出NDataObject信息
         * @param obj:写出对象
         * @return
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-31
         */
        bool writeNDataObject(Core::FITKAbstractNDataObject* obj, H5::H5Object & h5Object);


        /**
         * @brief 写出NDataObject信息
         * @param obj:写出对象
         * @return
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-31
         */
        bool writeDataObject(Core::FITKAbstractDataObject* obj, H5::H5Object & h5Object);

    protected:

        /**
         * @brief INP文件读取器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        FITKAbstractHDF5Reader* _reader{};

        /**
         * @brief INP文件写出器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        FITKAbstractHDF5Writer* _writer{};

        /**
         * @brief H5Group
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        H5::Group* _h5Group{};

        /**
         * @brief _h5Group下创建的第一个Group
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-05-30
         */
        H5::Group _h5CreateGroup;
    };
}


#endif
