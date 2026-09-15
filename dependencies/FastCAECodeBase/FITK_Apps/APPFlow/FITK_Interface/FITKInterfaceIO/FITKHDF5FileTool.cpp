/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKHDF5FileTool.h"
#include <QString>

namespace IO
{
    FITKHDF5FileTool::FITKHDF5FileTool(H5::H5File * h5Filep)
    {
        if (h5Filep == nullptr)
        {
            return;
        }
        m_h5Filep = h5Filep;
    }

    std::tuple<bool, H5::Group> FITKHDF5FileTool::createGp(const char*groupName)
    {
        if (groupName == nullptr)
        {
            return std::tuple<bool, H5::Group>(false, {});
        }
        try
        {
            H5::Group Gp = m_h5Filep->createGroup(groupName);//创建组
            return std::tuple<bool, H5::Group>(true, Gp);
        }
        catch (H5::GroupIException&error)
        {
            error.printErrorStack();
        }
        return std::tuple<bool, H5::Group>(false, {});
    }


    std::tuple<bool, H5::Group> FITKHDF5FileTool::createGp(H5::Group & group, QString name)
    {
        try
        {
            H5::Group subGroup = group.createGroup(name.toStdString());//创建子节点
            return std::tuple<bool, H5::Group>(true, subGroup);
        }
        catch (H5::GroupIException&error)
        {
            error.printErrorStack();
        }
        return std::tuple<bool, H5::Group>(false, {});
    }

    std::tuple<bool, H5::Group> FITKHDF5FileTool::openGp(const char * groupName)
    {
        if (groupName == nullptr)
        {
            return std::tuple<bool, H5::Group>(false, {});
        }
        try
        {
            H5::Group group = m_h5Filep->openGroup(groupName);
            return std::tuple<bool, H5::Group>(true, group);
        }
        catch (H5::GroupIException&error)
        {
            error.printErrorStack();
        }
        catch (H5::FileIException&error)
        {
            error.printErrorStack();
        }
        return std::tuple<bool, H5::Group>(false, {});
    }

    std::tuple<bool, H5::Attribute> FITKHDF5FileTool::openAttribut(H5::Group &group, const char * name)
    {
        if (name == nullptr)
        {
            return std::tuple<bool, H5::Attribute>(false, {});
        }
        try
        {
            H5::Attribute attr = group.openAttribute(name);
            return std::tuple<bool, H5::Attribute>(true, attr);
        }
        catch (H5::AttributeIException &error)
        {
            error.printErrorStack();
        }
        return std::tuple<bool, H5::Attribute>(false, {});
    }

    std::tuple<bool, H5::Group> FITKHDF5FileTool::openGp(H5::Group &group, const char * groupName)
    {
        if (groupName == nullptr)
        {
            return std::tuple<bool, H5::Group>(false, {});
        }
        if (group.nameExists(groupName) == false)
        {
            return std::tuple<bool, H5::Group>(false, {});
        }
        try
        {
            if (group.nameExists(groupName) == false)
            {
                std::tuple<bool, H5::Group>(false, {});
            }
            H5::Group subGroup = group.openGroup(groupName);
            return std::tuple<bool, H5::Group>(true, subGroup);
        }
        catch (H5::GroupIException&error)
        {
            error.printErrorStack();
        }
        return std::tuple<bool, H5::Group>(false, {});
    }

    bool FITKHDF5FileTool::writeGroupAttrDouble(H5::Group group, const char *name, double data)
    {
        QString err;//判断是否有错误
        hsize_t dim[2] = { 1, 1 };
        H5::DataSpace Space(2, dim);
        try
        {
            //创建数据结构
            H5::Attribute sub = group.createAttribute(name, H5::PredType::NATIVE_DOUBLE, Space);
            //写入一行数据
            sub.write(H5::PredType::NATIVE_DOUBLE, &data);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();//为空则没有错误
    }

    bool FITKHDF5FileTool::writeGroupAttrDoubleArray(H5::Group group, const char *name, void *data, int rows, int cols)
    {
        hsize_t       dim[2] = { rows, cols };
        H5::DataSpace SpaceThree(2, dim);
        //创建数据结构
        try
        {    
            H5::Attribute transAttr = group.createAttribute(name, H5::PredType::NATIVE_DOUBLE, SpaceThree);
            transAttr.write(H5::PredType::NATIVE_DOUBLE, data);
        }
        catch(const std::exception& e)
        {
            return false;
        }
        return true;
    }

    bool FITKHDF5FileTool::writeGroupAttrIntArray(H5::Group group, const char *name, void *data, int rows, int cols)
    {
          hsize_t       dim[2] = { rows, cols };
        H5::DataSpace SpaceThree(2, dim);
        //创建数据结构
        try
        {    
            H5::Attribute transAttr = group.createAttribute(name, H5::PredType::NATIVE_INT, SpaceThree);
            transAttr.write(H5::PredType::NATIVE_INT, data);
        }
        catch(const std::exception& e)
        {
            return false;
        }
        return true;
    }

    bool FITKHDF5FileTool::readGroupAttrDoubleArray(H5::Group group, const char *name, void *data, int rows, int cols)
    {
         if (!group.attrExists(name)) return false;
         try
         {
            H5::Attribute transAttr = group.openAttribute(name);
            hsize_t       dim[2] = { rows, cols };
            H5::DataSpace SpaceThree(2, dim);
             
            transAttr.read(H5::PredType::NATIVE_DOUBLE, data);
         }
         catch(const std::exception& e)
         {
            return false;
         }
         
        return true;
    }

    bool FITKHDF5FileTool::readGroupAttrIntArray(H5::Group group, const char *name, void *data, int rows, int cols)
    {
        if (!group.attrExists(name)) return false;
         try
         {
            H5::Attribute transAttr = group.openAttribute(name);
            hsize_t       dim[2] = { rows, cols };
            H5::DataSpace SpaceThree(2, dim);
             
            transAttr.read(H5::PredType::NATIVE_INT, data);
         }
         catch(const std::exception& e)
         {
            return false;
         }
         
        return true;
    }

    bool FITKHDF5FileTool::writeGroupAttrFloat(H5::Group group, const char *name, float data)
    {
        QString err;//判断是否有错误
        hsize_t dim[2] = { 1, 1 };
        H5::DataSpace Space(2, dim);
        try
        {
            //创建数据结构
            H5::Attribute sub = group.createAttribute(name, H5::PredType::NATIVE_FLOAT, Space);
            //写入一行数据
            sub.write(H5::PredType::NATIVE_FLOAT, &data);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());

        }
        return err.isEmpty();//为空则没有错误
    }

    bool FITKHDF5FileTool::writeGroupAttrInt(H5::Group group, const char *name, int data)
    {
        QString err;
        hsize_t dim[2] = { 1, 1 };
        H5::DataSpace Space(2, dim);
        try
        {
            //创建数据结构
            H5::Attribute sub = group.createAttribute(name, H5::PredType::NATIVE_INT, Space);
            //写入一行数据
            sub.write(H5::PredType::NATIVE_INT, &data);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }


    bool FITKHDF5FileTool::writeGroupAttrStr(H5::Group group, const char * name, std::string data)
    {
        QString err;
        H5::StrType str_type(H5::PredType::C_S1, H5T_VARIABLE);//hdf5字符串类型
        hsize_t dim[2] = { 1, 1 };
        H5::DataSpace Space(2, dim);
        try
        {
            H5::Attribute srtSub = group.createAttribute(name, str_type, Space);
            srtSub.write(str_type, data);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }


    bool FITKHDF5FileTool::writeGroupAttrBool(H5::Group group, const char * name, bool data)
    {
        QString err;
        hsize_t dim[2] = { 1, 1 };
        H5::DataSpace Space(2, dim);
        try
        {
            //创建数据结构
            H5::Attribute sub = group.createAttribute(name, H5::PredType::NATIVE_HBOOL, Space);
            //写入一行数据
            sub.write(H5::PredType::NATIVE_HBOOL, &data);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::readGroupBool(H5::Group & group, const char * name, bool * data)
    {
        if (!group.attrExists(name)) return false;
        if (data == nullptr)return false;
        QString err;
        try
        {
            H5::Attribute sub = group.openAttribute(name);
            sub.read(H5::PredType::NATIVE_HBOOL, data);
            return true;
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::readGroupDouble(H5::Group & group, const char * name, double *db)
    {

        if (!group.attrExists(name)) return false;
        if (db == nullptr)return false;
        QString err;
        try
        {
            H5::Attribute sub = group.openAttribute(name);
            sub.read(H5::PredType::NATIVE_DOUBLE, db);
            return true;
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::readGroupFloat(H5::Group & group, const char * name, float * fl)
    {
        if (!group.attrExists(name)) return false;
        if (fl == nullptr)return false;
        QString err;
        try
        {
            H5::Attribute sub = group.openAttribute(name);
            sub.read(H5::PredType::NATIVE_FLOAT, fl);
            return true;
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::readGroupint(H5::Group & group, const char * name, int *i)
    {
        if (!group.attrExists(name)) return false;
        if (i == nullptr)return false;
        QString err;
        try
        {
            H5::Attribute sub = group.openAttribute(name);
            sub.read(H5::PredType::NATIVE_INT, i);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::readGroupStr(H5::Group & group, const char * name, std::string &str)
    {
        if (!group.attrExists(name)) return false;
        QString err;
        H5::StrType str_type(H5::PredType::C_S1, H5T_VARIABLE);//hdf5字符串类型
        try
        {
            H5::Attribute sub = group.openAttribute(name);
            sub.read(str_type, str);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }



    std::string FITKHDF5FileTool::readGroupAttrName(H5::Group group, int i)
    {
        char subNameBuf[256];
        sprintf(subNameBuf, "%d", i);
        try
        {
            H5::Attribute subNameAttr = group.openAttribute(subNameBuf);
            H5::StrType str_type(H5::PredType::C_S1, H5T_VARIABLE);
            std::string subGroupName;//具体名称
            subNameAttr.read(str_type, subGroupName);
            return subGroupName;
        }
        catch (H5::AttributeIException&error)
        {
            error.printErrorStack();
        }

    }


    bool FITKHDF5FileTool::CreateGroupAndAttr(H5::Group &group, const char*groupName, const char*AttribteName, const int &AttribteCount)
    {
        QString err;
        try
        {
            H5::Group SubGroup = group.createGroup(groupName);//在该组下创建另一个组
            //属性的数据结构
            hsize_t dim[2] = { 1, 1 };
            H5::DataSpace SpaceThree(2, dim);
            H5::Attribute CountAttr = SubGroup.createAttribute(AttribteName, H5::PredType::NATIVE_INT, SpaceThree);
            //将属性写入该组的属性空间
            CountAttr.write(H5::PredType::NATIVE_INT, &AttribteCount);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::GroupIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    H5::Group FITKHDF5FileTool::GetPathGroup(const H5std_string& path)
    {
        try
        {
            H5::Group FindGroup = m_h5Filep->openGroup(path);
            return FindGroup;
        }
        catch (H5::FileIException&error)
        {
            error.printErrorStack();
        }
        return {};
    }

    bool FITKHDF5FileTool::CreateNameFromAttr(H5::Group group, const int  i, std::string data)
    {
        try
        {
            QByteArray subName = QString::number(i).toLocal8Bit();
            H5::StrType str_type(H5::PredType::C_S1, H5T_VARIABLE);//hdf5字符串类型
            hsize_t dim[2] = { 1, 1 };
            H5::DataSpace SpaceThree(2, dim);
            H5::Attribute CountAttr = group.createAttribute(subName.data(), str_type, SpaceThree);
            CountAttr.write(str_type, data);
            return true;
        }
        catch (H5::AttributeIException&error)
        {
            error.printErrorStack();
            return false;
        }
        return false;

    }

    bool FITKHDF5FileTool::CreateOneDArrayDataSetFloat(H5::Group group, int number, QString Name, void *buf)
    {
        QString err;
        if (buf == nullptr || number <= 0)
        {
            //为空或者小于等于0
            return false;
        }
        try
        {
            //写入DataSet
            //1D数组，长度number
            hsize_t dims[1] = { number };
            H5::DataSpace dataspace(1, dims);
            H5::DataSet dataSet = group.createDataSet(Name.toStdString(), H5::PredType::NATIVE_FLOAT, dataspace);
            dataSet.write(buf, H5::PredType::NATIVE_FLOAT);
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error) {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataTypeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::CreateOneDarrayDataSetInt(H5::Group group, QString Name, std::vector<int>& data)
    {
        QString err;
        if (data.size() == 0)
        {
            //为空或者小于等于0
            return false;
        }
        try
        {
            //写入DataSet
            //1D数组，长度number
            hsize_t dims[1] = { data.size() };
            H5::DataSpace dataspace(1, dims);
            H5::DataSet dataSet = group.createDataSet(Name.toStdString(), H5::PredType::NATIVE_INT, dataspace);
            dataSet.write(data.data(), H5::PredType::NATIVE_INT);
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error) {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataTypeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::CreateOneDarrayDataSetDouble(H5::Group group, QString Name, std::vector<double>& data)
    {
        QString err;
        if (data.size() == 0)
        {
            //为空或者小于等于0
            return false;
        }
        try
        {
            //写入DataSet
            //1D数组，长度number
            hsize_t dims[1] = { data.size() };
            H5::DataSpace dataspace(1, dims);
            H5::DataSet dataSet = group.createDataSet(Name.toStdString(), H5::PredType::NATIVE_DOUBLE, dataspace);
            dataSet.write(data.data(), H5::PredType::NATIVE_DOUBLE);
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error) {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataTypeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::CreateOneDarrayDataSetStr(H5::Group group, QString Name, char *data, int size)
    {
       try
       {
            hsize_t dim[2] = { size, 1 };
            // 创建数据集
            H5::DataSpace dataSpace(2, dim);
            H5::DataSet   dataSet = group.createDataSet(Name.toStdString(), H5::PredType::NATIVE_CHAR, dataSpace);
            dataSet.write(data, H5::PredType::NATIVE_CHAR);
       }
       catch(const std::exception& e)
       {
          return false;
       }
       return true;
    }

    bool FITKHDF5FileTool::CreateTwoArrayDataSetDouble(H5::Group group, QString Name, std::vector<std::vector<double>> &data)
    {
        QString err;
        if (data.size() == 0)
        {
            //为空或者小于等于0
            return false;
        }
        try
        {
            if (data[0].empty())
            {
                return false;
            }
            //检查每一个元素的大小
            int colos = data[0].size();
            for (int i = 0; i < data.size(); ++i)
            {
                if (colos != data.at(i).size())
                {
                    return false;
                }
            }

            int rows = data.size();

            //创建连续内存
            std::vector<double>contiguousData(rows*colos);
            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < colos; j++)
                {
                    contiguousData[i*colos + j] = data[i][j];
                }
            }
            //写入DataSet
            //2D数组
            hsize_t dims[2] = { rows,colos };
            H5::DataSpace dataspace(2, dims);

            H5::DataSet dataSet = group.createDataSet(Name.toStdString(), H5::PredType::NATIVE_DOUBLE, dataspace);
            dataSet.write(contiguousData.data(), H5::PredType::NATIVE_DOUBLE);
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error) {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataTypeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::CreateTwoArrayDataSetInt(H5::Group group, QString Name, std::vector<std::vector<int>>& data)
    {
        QString err;
        if (data.size() == 0)
        {
            //为空或者小于等于0
            return false;
        }
        try
        {
            if (data[0].empty())
            {
                return false;
            }
            //检查每一个元素的大小
            int colos = data[0].size();
            for (int i = 0; i < data.size(); ++i)
            {
                if (colos != data.at(i).size())
                {
                    return false;
                }
            }

            int rows = data.size();

            //创建连续内存
            std::vector<int>contiguousData(rows*colos);
            for (int i = 0; i < rows; ++i)
            {
                for (int j = 0; j < colos; j++)
                {
                    contiguousData[i*colos + j] = data[i][j];
                }
            }
            //写入DataSet
            //2D数组
            hsize_t dims[2] = { rows,colos };
            H5::DataSpace dataspace(2, dims);

            H5::DataSet dataSet = group.createDataSet(Name.toStdString(), H5::PredType::NATIVE_INT, dataspace);
            dataSet.write(contiguousData.data(), H5::PredType::NATIVE_INT);
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error) {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataTypeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::CreateTwoArrayDataSetInt(H5::Group & group, QString Name, std::vector<std::vector<int>>& data, QString suffix)
    {
        QString err;
        if (data.size() == 0)
        {
            //为空或者小于等于0
            return false;
        }
        try
        {
            std::vector<int>offsetVec;//偏移数据
            std::vector<int>flatData;//平坦数据
            for (const auto &vec : data)
            {
                flatData.insert(flatData.end(), vec.begin(), vec.end());
                offsetVec.push_back(vec.size());
            }
            hsize_t flatDataSize = flatData.size();
            hsize_t sizesSize = offsetVec.size();
            H5::DataSpace flatDataSpace(1, &flatDataSize);
            H5::DataSpace sizesSpace(1, &sizesSize);

            H5::DataSet flatDataSet = group.createDataSet(Name.toStdString(), H5::PredType::NATIVE_INT, flatDataSpace);
            H5::DataSet sizesDataSet = group.createDataSet((Name + suffix).toStdString(), H5::PredType::NATIVE_INT, sizesSpace);
            flatDataSet.write(flatData.data(), H5::PredType::NATIVE_INT);
            sizesDataSet.write(offsetVec.data(), H5::PredType::NATIVE_INT);
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error) {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataTypeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::ReadTwoArrayDataSetDouble(H5::Group group, QString Name, std::vector<std::vector<double>>& data)
    {
        QString err;
        if (Name.size() == 0)
        {
            return false;
        }
        htri_t status = H5Lexists(group.getId(), Name.toStdString().c_str(), H5P_DEFAULT);
        if (status <= 0)
        {
            //没有该dataset
            return false;
        }
        try
        {
            if (group.nameExists(Name.toStdString()) == false)
            {
                return false;
            }
            H5::DataSet dataset = group.openDataSet(Name.toStdString());
            H5::DataSpace dataspace = dataset.getSpace();
            hsize_t dims[2];
            dataspace.getSimpleExtentDims(dims, nullptr);

            unsigned int rows = dims[0];
            unsigned int cols = dims[1];
            std::vector<double>contiguousData(rows*cols);

            //清空
            data.clear();

            //设置新的大小
            data.resize(rows);
            for (unsigned int i = 0; i < rows; ++i)
            {
                data[i].resize(cols);
            }

            //读取数据
            dataset.read(contiguousData.data(), H5::PredType::NATIVE_DOUBLE);

            //写入二维数组
            for (unsigned int i = 0; i < rows; ++i)
            {
                for (unsigned int j = 0; j < cols; ++j)
                {
                    data[i][j] = contiguousData[i*cols + j];
                }
            }
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::ReadTwoArrayDataSetInt(H5::Group group, QString Name, std::vector<std::vector<int>>& data)
    {
        QString err;
        if (Name.size() == 0)
        {
            return false;
        }
        htri_t status = H5Lexists(group.getId(), Name.toStdString().c_str(), H5P_DEFAULT);
        if (status <= 0)
        {
            //没有该dataset
            return false;
        }
        try
        {
            if (group.nameExists(Name.toStdString()) == false)
            {
                return false;
            }
            H5::DataSet dataset = group.openDataSet(Name.toStdString());
            H5::DataSpace dataspace = dataset.getSpace();
            hsize_t dims[2];
            dataspace.getSimpleExtentDims(dims, nullptr);

            unsigned int rows = dims[0];
            unsigned int cols = dims[1];
            std::vector<int>contiguousData(rows*cols);

            //清空
            data.clear();

            //设置新的大小
            data.resize(rows);
            for (unsigned int i = 0; i < rows; ++i)
            {
                data[i].resize(cols);
            }

            //读取数据
            dataset.read(contiguousData.data(), H5::PredType::NATIVE_INT);

            //写入二维数组
            for (unsigned int i = 0; i < rows; ++i)
            {
                for (unsigned int j = 0; j < cols; ++j)
                {
                    data[i][j] = contiguousData[i*cols + j];
                }
            }
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::ReadTwoArrayDataSetInt(H5::Group &group, QString Name, std::vector<std::vector<int>>& data, QString suffix)
    {
        QString err;
        if (Name.size() == 0)
        {
            return false;
        }
        if (suffix.size()==0)
        {
            return false;
        }
        htri_t status = H5Lexists(group.getId(), Name.toStdString().c_str(), H5P_DEFAULT);
        if (status <= 0)
        {
            //没有该dataset
            return false;
        }
        try
        {
            // 读取展平后的数据
            H5::DataSet flatDataSet = group.openDataSet(Name.toStdString());
            H5::DataSpace flatDataSpace = flatDataSet.getSpace();
            hsize_t flatDataSize;
            flatDataSpace.getSimpleExtentDims(&flatDataSize);
            std::vector<int> flatData(flatDataSize);
            flatDataSet.read(flatData.data(), H5::PredType::NATIVE_INT);


            //读偏移数据
            H5::DataSet sizesDataSet = group.openDataSet((Name + suffix).toStdString());
            H5::DataSpace sizesSpace = sizesDataSet.getSpace();
            hsize_t sizesSize;
            sizesSpace.getSimpleExtentDims(&sizesSize);
            std::vector<int> sizes(sizesSize);
            sizesDataSet.read(sizes.data(), H5::PredType::NATIVE_INT);
            data.clear();
            int offset = 0;
            for (int size : sizes) {
                data.push_back(std::vector<int>(flatData.begin() + offset, flatData.begin() + offset + size));
                offset += size;
            }
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::ReadOneDArrayDataSetFloat(H5::Group group, QString Name, std::vector<float>& data)
    {
        QString err;
        if (Name.size() == 0)
        {
            return false;
        }
        htri_t status = H5Lexists(group.getId(), Name.toStdString().c_str(), H5P_DEFAULT);
        if (status <= 0)
        {
            //没有该dataset
            return false;
        }
        try
        {
            H5::DataSet dataset = group.openDataSet(Name.toStdString());
            //获取结构
            H5::DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims, nullptr);
            float*temp = new float[dims[0]];
            dataset.read(temp, H5::PredType::NATIVE_FLOAT);
            data.clear();//清空一下
            for (int i = 0; i < dims[0]; ++i)
            {
                data.push_back(temp[i]);
            }
            delete[]temp;//释放内存
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::ReadOneDArrayDataSetStr(H5::Group group, QString Name, char *data, int size)
    {

        try
        {
            if (!group.exists(Name.toStdString())) return false;
            hsize_t dim[2] = { size, 1 };
            // 创建数据集
            H5::DataSpace dataSpace(2, dim);
            H5::DataSet   dataSet = group.openDataSet(Name.toStdString());
            dataSet.read(data, H5::PredType::NATIVE_CHAR, dataSpace);
        }
        catch(const std::exception& e)
        {
            return false;
        }
        
        return true;
    }

    bool FITKHDF5FileTool::ReadOneDArrayDataSetInt(H5::Group group, QString Name, std::vector<int>& data)
    {
        QString err;
        if (Name.size() == 0)
        {
            return false;
        }
        htri_t status = H5Lexists(group.getId(), Name.toStdString().c_str(), H5P_DEFAULT);
        if (status <= 0)
        {
            //没有该dataset
            return false;
        }
        try
        {
            H5::DataSet dataset = group.openDataSet(Name.toStdString());
            //获取结构
            H5::DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims, nullptr);
            int*temp = new int[dims[0]];
            dataset.read(temp, H5::PredType::NATIVE_INT);
            data.clear();//清空一下
            for (int i = 0; i < dims[0]; ++i)
            {
                data.push_back(temp[i]);
            }
            delete[]temp;//释放内存
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::ReadOneDArrayDataSetDouble(H5::Group group, QString Name, std::vector<double>& data)
    {
        QString err;
        if (Name.size() == 0)
        {
            return false;
        }
        htri_t status = H5Lexists(group.getId(), Name.toStdString().c_str(), H5P_DEFAULT);
        if (status <= 0)
        {
            //没有该dataset
            return false;
        }
        try
        {
            H5::DataSet dataset = group.openDataSet(Name.toStdString());
            //获取结构
            H5::DataSpace dataspace = dataset.getSpace();
            hsize_t dims[1];
            dataspace.getSimpleExtentDims(dims, nullptr);
            double*temp = new double[dims[0]];
            dataset.read(temp, H5::PredType::NATIVE_DOUBLE);
            data.clear();//清空一下
            for (int i = 0; i < dims[0]; ++i)
            {
                data.push_back(temp[i]);
            }
            delete[]temp;//释放内存
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    bool FITKHDF5FileTool::CheckGoupHaveAttribute(H5::Group group, QString Name)
    {
        QString err;
        try
        {
            int Ret = H5Aexists(group.getId(), Name.toStdString().data());
            if (Ret > 0)
            {
                return true;
            }
            return false;
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::GroupIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();

    }

    bool FITKHDF5FileTool::createAttribute(H5::Group &group, QString name, int data) {

        QString err;
        try
        {
            //属性的数据结构
            hsize_t dim[2] = { 1, 1 };
            H5::DataSpace SpaceThree(2, dim);
            H5::Attribute CountAttr = group.createAttribute(name.toStdString(), H5::PredType::NATIVE_INT, SpaceThree);
            //将属性写入该组的属性空间
            CountAttr.write(H5::PredType::NATIVE_INT, &data);
        }
        catch (H5::AttributeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::GroupIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }


    bool FITKHDF5FileTool::writeCustomtypesOnedim(H5::Group &group, QString name, H5::CompType &Customtype, void *data, int size)
    {
        QString err;
        if (data == nullptr)
        {
            return false;
        }
        if (size <= 0)
        {
            return false;
        }
        try
        {
            hsize_t dims[1] = { size };
            //维度
            H5::DataSpace dataspace(1, dims);
            auto dataset = group.createDataSet(name.toStdString(), Customtype, dataspace);
            dataset.write(data, Customtype);
        }
        catch (H5::GroupIException &error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSetIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataSpaceIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        catch (H5::DataTypeIException&error)
        {
            err = QString(error.getCDetailMsg());
        }
        return err.isEmpty();
    }

    void FITKHDF5FileTool::CheckString(QString & s)
    {
        if (!s.isEmpty())
        {
            s = s.replace("/", "%SPRIT%");
        }
        else
        {
            return;
        }
    }

    void FITKHDF5FileTool::RestoreString(QString & s)
    {
        s = s.replace("%SPRIT%", "/");
    }

}