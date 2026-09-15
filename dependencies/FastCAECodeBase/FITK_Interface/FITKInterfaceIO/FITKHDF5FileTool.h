/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_HDF5_FILE_TOOL_H___
#define _FITK_HDF5_FILE_TOOL_H___
/**
*
* @file FITKHDF5FileTool.h
* @brief HDF5封装
* @author yangjing(1195810980@qq.com)
* @date 2024-12-13
* 所有对HDF5的操作都需要进行try-catch的封装，否则程序在读写hdf5时会有未捕获的错误异常报告
*/

#include <H5Cpp.h>
#include <vector>
#include <string>
#include <QList>
#include <QString>
#include <tuple>
#include "FITKInterfaceIOAPI.h"

namespace IO
{
    class FITKInterfaceIOAPI FITKHDF5FileTool
    {

    public:

        /**
        * @brief 构造函数
        * @param @param[i]  HDF5 文件指针
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-20
        */
        explicit FITKHDF5FileTool(H5::H5File*h5Filep);

        virtual ~FITKHDF5FileTool() = default;


        /**
        * @brief 创建指定名称的group组
        * @param @param[i]  group hdf5组
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        std::tuple<bool, H5::Group> createGp(const char*groupName);


        /**
        * @brief 在指定Group下创建group组
        * @param @param[i]  group hdf5组
        * @param @param[i]  name 子group组名称
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        std::tuple<bool, H5::Group> createGp(H5::Group &group, QString name);


        /**
        * @brief 打开指定名称的group组,使用前先判断tuple是否为false
        * @param @param[i]  group hdf5组
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        std::tuple<bool, H5::Group>openGp(const char*groupName);



        /**
        * @brief 打开指定组下的属性数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name 属性名称
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-24
        */
        std::tuple<bool, H5::Attribute>openAttribut(H5::Group &group, const char*name);



        /**
        * @brief 在指定组下打开指定组
        * @param @param[i]  group hdf5组
        * @param @param[i]  groupName 组名称
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        std::tuple<bool, H5::Group>openGp(H5::Group& group, const char*groupName);


        /**
        * @brief 向指定hdf5Group组创建属性，并为该属性写入double数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool writeGroupAttrDouble(H5::Group group, const char * name, double data);
        /**
        * @brief 向指定hdf5Group组创建属性，并为该属性写入double数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @param @param[i]  rows  行数
        * @param @param[i]  cols  列数
        * @author libaojun 
        * @date 2025-04-22
        */
        bool writeGroupAttrDoubleArray(H5::Group group, const char * name, void* data, int rows, int cols);
        
        /**
        * @brief 从指定hdf5Group组读取数据，double数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @param @param[i]  rows  行数
        * @param @param[i]  cols  列数
        * @author libaojun 
        * @date 2025-04-22
        */
        bool readGroupAttrDoubleArray(H5::Group group, const char * name, void* data, int rows, int cols);
        /**
        * @brief 向指定hdf5Group组创建属性，并为该属性写入Int数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @param @param[i]  rows  行数
        * @param @param[i]  cols  列数
        * @author libaojun 
        * @date 2025-04-22
        */
        bool writeGroupAttrIntArray(H5::Group group, const char * name, void* data, int rows, int cols);
        /**
        * @brief 从指定hdf5Group组读取数据，INT数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @param @param[i]  rows  行数
        * @param @param[i]  cols  列数
        * @author libaojun 
        * @date 2025-04-22
        */
        bool readGroupAttrIntArray(H5::Group group, const char * name, void* data, int rows, int cols);
        
        /**
        * @brief 向指定hdf5Group组创建属性，并为该属性写入float数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool writeGroupAttrFloat(H5::Group group, const char * name, float data);
        

        /**
        * @brief 向指定hdf5Group组创建属性，并为该属性写入str数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool writeGroupAttrStr(H5::Group group, const char * name, std::string data);


        /**
        * @brief 向指定hdf5Group组创建属性，并为该属性写入int数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool writeGroupAttrInt(H5::Group group, const char *name, int data);



        /**
        * @brief 向指定hdf5Group组创建属性，并为该属性写入bool数据
        * @param @param[i]  group hdf5组
        * @param @param[i]  name  该组名称
        * @param @param[i]  data  数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-17
        */
        bool writeGroupAttrBool(H5::Group group, const char *name, bool data);


        /**
        * @brief 读取指定Group组下的bool数据
        * @param @param[i]  group 指定读取的组
        * @param @param[i]  name 该组内指定的那个名称
        * @param @param[out]  data 传出读取到的bool值
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-17
        */
        bool readGroupBool(H5::Group &group, const char * name, bool*data);



        /**
        * @brief 读取指定Group组下的double数据
        * @param @param[i]  group 指定读取的组
        * @param @param[i]  name 该组内指定的那个名称
        * @param @param[out]  db 传出读取到的double值
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool readGroupDouble(H5::Group &group, const char * name, double *db = nullptr);

        /**
        * @brief 读取指定Group组下的float数据
        * @param @param[i]  group 指定读取的组
        * @param @param[i]  name 该组内指定的那个名称
        * @param @param[out]  db 传出读取到的double值
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool readGroupFloat(H5::Group &group, const char * name, float *fl = nullptr);


        /**
        * @brief 读取指定Group组下的int数据
        * @param @param[i]  group 指定读取的组
        * @param @param[i]  name 该组内指定的那个名称
        * @param @param[out]  i 传出读取到的int值
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool readGroupint(H5::Group &group, const char * name, int *i = nullptr);

        /**
        * @brief 读取指定Group组下的Str数据
        * @param @param[i]  group 指定读取的组
        * @param @param[i]  name 该组内指定的那个名称
        * @param @param[out]  str 输出str
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool readGroupStr(H5::Group &group, const char * name, std::string &str);

        /**
        * @brief 读取指定Group组下的Str数据
        * @param @param[i]  group 指定读取的组
        * @param @param[i]  i 该属性名称（以int做str）
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-20
        */
        std::string readGroupAttrName(H5::Group group, int i);



        /**
        * @brief 为某个组创建一个子组，并为该子组创建属性
        * @param @param[i]  group 需要在其下方创建组的父组
        * @param @param[i]  groupName 该子组名称
        * @param @param[i]  AttribteName 该子组属性名称
        * @param @param[i]  AttribteCount 该子组属性数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool CreateGroupAndAttr(H5::Group &group, const char*groupName, const char*AttribteName, const int &AttribteCount);

        /**
        * @brief 获取到指定路径下的组
        * @param @param[i]  path 组的路径
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        H5::Group GetPathGroup(const H5std_string& path);



        /**
        * @brief 在指定组下创建属性并给出名称
        * @param @param[i]  group 组
        * @param @param[i]  i 该组名称属性名字(以int为名称)
        * @param @param[i]  data 该组属性的数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-19
        */
        bool CreateNameFromAttr(H5::Group group, const int i, std::string data);



        /**
        * @brief 在指定Gourp下创建DataSet,并为该DataSet存储数据,数据要求是一维float数组
        * @param @param[i]  group 组
        * @param @param[i]  number 该一位数组的个数
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[i]  buf  要写入的数据指针
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        bool CreateOneDArrayDataSetFloat(H5::Group group, int number, QString Name, void *buf);


        /**
        * @brief 在指定Gourp下创建DataSet,并为该DataSet存储数据,数据要求是一维int数组
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[i]  data 该DataSet的数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-26
        */
        bool CreateOneDarrayDataSetInt(H5::Group group, QString Name, std::vector<int>&data);


        /**
        * @brief 在指定Gourp下创建DataSet,并为该DataSet存储数据,数据要求是一维double数组
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[i]  data 该DataSet的数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-26
        */
        bool CreateOneDarrayDataSetDouble(H5::Group group, QString Name, std::vector<double>&data);

        /**
        * @brief 在指定Gourp下创建DataSet,并为该DataSet存储数据,数据要求是一维数组
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[i]  data 该DataSet的数据
        * @param @param[i]  size data的长度
        * @author libaojun
        * @date 2024-04-23
        */
        bool CreateOneDarrayDataSetStr(H5::Group group, QString Name, char* data, int size);

        /**
        * @brief 在指定Gourp下创建DataSet,并为该DataSet存储数据,数据要求是二维double数组,注意每个元素的大小需要一致,否则写入不成功
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[i]  data 该DataSet的数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-6
        */
        bool CreateTwoArrayDataSetDouble(H5::Group group, QString Name, std::vector<std::vector<double>>&data);

        /**
        * @brief 在指定Gourp下创建DataSet,并为该DataSet存储数据,数据要求是二维int数组,注意每个元素的大小需要一致,否则写入不成功
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[i]  data 该DataSet的数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-17
        */
        bool CreateTwoArrayDataSetInt(H5::Group group, QString Name, std::vector<std::vector<int>>&data);




        /**
        * @brief 在指定Gourp下创建DataSet,并为该DataSet存储数据,数据要求是二维int数组,每个元素的大小不需要一致
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[i]  data 该DataSet的数据
        * @param @param[i]  suffix 存储偏移的后缀
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-17
        */
        bool CreateTwoArrayDataSetInt(H5::Group&group, QString Name,std::vector<std::vector<int>>&data, QString suffix);


        /**
        * @brief 在指定Gourp下读取DataSet,要求该DataSet为二维double数组
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[out]  data 该DataSet的数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-6
        */
        bool ReadTwoArrayDataSetDouble(H5::Group group, QString Name, std::vector<std::vector<double>>&data);



        /**
        * @brief 在指定Gourp下读取DataSet,要求给DataSet为二维int数组
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[out]  data 该DataSet的数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-6
        */
        bool ReadTwoArrayDataSetInt(H5::Group group, QString Name, std::vector<std::vector<int>>&data);



        /**
        * @brief 在指定Gourp下读取DataSet,要求是使用CreateTwoArrayDataSetInt+偏移后缀创建的数据
        * @param @param[i]  group 组
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[out]  data 该DataSet的数据
        * @param @param[out]  suffix 偏移名称
        * @author yangjing (1195810980@qq.com)
        * @date 2024-1-6
        */
        bool ReadTwoArrayDataSetInt(H5::Group &group, QString Name, std::vector<std::vector<int>>&data, QString suffix);


        /**
        * @brief 在指定Gourp下读取Dataset 要求该DataSet为一维数组，类型float
        * @param @param[i]  group 组
        * @param @param[i]  Name 要读取的DataSet名称
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[out]  data 传出数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        bool ReadOneDArrayDataSetFloat(H5::Group group, QString Name, std::vector<float>& data);

        /**
        * @brief 在指定Gourp下读取Dataset 要求该DataSet为一维数组，类型string
        * @param @param[i]  group 组
        * @param @param[i]  Name 要读取的DataSet名称
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[out]  data 传出数据
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2024-04-27
        */
        bool ReadOneDArrayDataSetStr(H5::Group group, QString Name, char* data, int size);



        /**
        * @brief 在指定Gourp下读取Dataset 要求该DataSet为一维数组，类型Int
        * @param @param[i]  group 组
        * @param @param[i]  Name 要读取的DataSet名称
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[out]  data 传出数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        bool ReadOneDArrayDataSetInt(H5::Group group, QString Name, std::vector<int>& data);



        /**
        * @brief 在指定Gourp下读取Dataset 要求该DataSet为一维数组，类型Double
        * @param @param[i]  group 组
        * @param @param[i]  Name 要读取的DataSet名称
        * @param @param[i]  Name 该DataSet的名称
        * @param @param[out]  data 传出数据
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-23
        */
        bool ReadOneDArrayDataSetDouble(H5::Group group, QString Name, std::vector<double>& data);

        /**
        * @brief 在指定Gourp下判断是否有某个属性
        * @param @param[i]  group 组
        * @param @param[i]  Name 要判断的属性名称
        * @return true有该属性
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-26
        */
        bool CheckGoupHaveAttribute(H5::Group group, QString Name);


        /**
        * @brief 在指定Gourp下创建属性并赋值，值为int
        * @param @param[i]  group 组
        * @param @param[i]  Name 要添加的属性名称
        * @param @param[i]  data 该属性的名称
        * @return true有该属性
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-26
        */
        bool createAttribute(H5::Group &group, QString name, int data);



        /**
        * @brief 在指定 Group 下创建一维 Dataset，类型自定义
        * 理解为创建一个一维数组，其中每一个元素都是自定义类型
        * @param @param[i]  group 组
        * @param @param[i]  name 该Datset的名称
        * @param @param[i]  Customtype 该Datset的自定义类型
        * @param @param[i]  data 该数据指针
        * @param @param[i]  size 该数据个数
        * @details 该数据的个数，也就是该一维数组元素的个数
        * @return 是否存储成功
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-26
        */
        bool writeCustomtypesOnedim(H5::Group &group, QString name, H5::CompType &Customtype, void *data, int size);


        /**
        * @brief 在指定 Group 下读取一维 Dataset，类型自定义
        * 理解为读取一个一维数组，其中每一个元素都是自定义类型
        * @param @param[i]  group 组
        * @param @param[i]  name 该Datset的名称
        * @param @param[i]  Customtype 该Datset的自定义类型
        * @param @param[out]  data 读取的数据传出
        * @return 是否读取成功
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-26
        */
        template<typename T>
        bool readCustomtypesOnedim(H5::Group &group, QString name, H5::CompType &Customtype, std::vector<T>&data)
        {
            //检查是否该Dataset存在于该Groupo中，注意，该检查只能适用于非跨路径A/B/C 只能检查A下是否有B  B是否有C 不能检查A是否有C
            htri_t status = H5Lexists(group.getId(), name.toStdString().c_str(), H5P_DEFAULT);
            if (status <= 0)
            {
                //没有该dataset
                return false;
            }
            QString err;
            try
            {
                H5::DataSet dataset = group.openDataSet(name.toStdString());//打开数据集
                H5::DataSpace dataspace = dataset.getSpace();//确定空间形状
                hsize_t dims[1];//个数 
                dataspace.getSimpleExtentDims(dims);
                data.resize(dims[0]);
                dataset.read(data.data(), Customtype);
            }
            catch (const H5::Exception& error) {
                err = QString(error.getCDetailMsg());
            }
            catch (const H5::DataSetIException&error)
            {
                err = QString(error.getCDetailMsg());
            }
            return err.isEmpty();
        }

        static void CheckString(QString & s);

        static void RestoreString(QString & s);

    private:

        /**
        * @brief hdf5指针
        * @author yangjing (1195810980@qq.com)
        * @date 2024-12-20
        */
        H5::H5File *m_h5Filep{};
    };
}
#endif

