/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5AbstractAdaptor.h
 * @brief  抽象的HDF5文件读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-07
 *********************************************************************/
#ifndef _FITKFLOWOFHDTF5ABSTRACTADAPTOR_H__
#define _FITKFLOWOFHDTF5ABSTRACTADAPTOR_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"

ForwardDeclarNS(H5, Group)

namespace Core
{
    class FITKAbstractNDataObject;
    class FITKParameter;
    class FITKParamBool;
    class FITKParamInt;
    class FITKParamDouble;
    class FITKParamString;
    class FITKParamBoolGroup;
    class FITKParamCombox;
    class FITKParamDoubleList;
    class FITKParamRadioGroup;
    class FITKParamLabel;
}

namespace IO
{
    class FITKAbstractHDF5Reader;
    class FITKAbstractHDF5Writer;
    /**
     * @brief 抽象的HDF5文件读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-09-07
     */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5AbstractAdaptor : public Adaptor::FITKAbstractIOAdaptor
    {
        FITKCLASS(IO, FITKFlowOFHDF5AbstractAdaptor);

    public:
        explicit FITKFlowOFHDF5AbstractAdaptor() = default;
        virtual ~FITKFlowOFHDF5AbstractAdaptor();

        /**
         * @brief 设置HDF5读取器
         * @param[in] reader HDF5读取器指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void setReader(IO::FITKAbstractHDF5Reader* reader);
        /**
         * @brief 设置HDF5写入器
         * @param[in] writer HDF5写入器指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void setWriter(IO::FITKAbstractHDF5Writer* writer);
        /**
         * @brief 设置HDF5组指针
         * @param[in] g HDF5组指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void setH5Group(H5::Group* g);
        /**
         * @brief 输出消息
         * @param errorLevel等级
         * @param str 输出内容
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        void consoleMessage(int errorLevel, QString str);

    protected:
        /**
         * @brief    写出NDataObject信息
         * @param[i] obj 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-06
         */
        bool writeNDataObject(Core::FITKAbstractNDataObject* obj, H5::Group& h5Group);
        /**
         * @brief    读取NDataObject信息
         * @param[i] obj 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-06
         */
        bool readNDataObject(Core::FITKAbstractNDataObject* obj, H5::Group & h5Group);
        /**
         * @brief    写出DataObject信息
         * @param[i] obj 
         * @param[i] h5Group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-06
         */
        bool writeDataObject(Core::FITKAbstractDataObject* obj, H5::Group & h5Group);
        /**
         * @brief    读取DataObject信息
         * @param[i] obj
         * @param[i] h5Group
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-06
         */
        bool readDataObject(Core::FITKAbstractDataObject* obj, H5::Group & h5Group);
        /**
         * @brief 写出Variant数据
         * @param datasetName
         * @param value
         * @param h5Group
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-19
         */
        bool writeVariantToHDF5(const QString &datasetName, const QVariant &value, H5::Group& h5Group);
        /**
         * @brief 读取Variant数据
         * @param datasetName
         * @param value
         * @param h5Group
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-26
         */
        bool readVariantFromHDF5(const QString &datasetName, QVariant &value, H5::Group& h5Group);
        /**
         * @brief    参数写出工具
         * @param[i] h5Group 
         * @param[i] paraData 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterToolW(H5::Group& h5Group, Core::FITKParameter* paraData);
        /**
         * @brief    Bool 参数写出
         * @param[i] h5Group 
         * @param[i] paraBool 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterBoolW(H5::Group& h5Group, Core::FITKParamBool* paraBool);
        /**
         * @brief    Combox 参数写出
         * @param[i] h5Group 
         * @param[i] paraCombox 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterComboxW(H5::Group& h5Group, Core::FITKParamCombox* paraCombox);
        /**
         * @brief    Double 参数写出
         * @param[i] h5Group 
         * @param[i] paraDouble 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterDoubleW(H5::Group& h5Group, Core::FITKParamDouble* paraDouble);
        /**
         * @brief    Int 参数写出
         * @param[i] h5Group 
         * @param[i] paraInt 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterIntW(H5::Group& h5Group, Core::FITKParamInt* paraInt);
        /**
         * @brief    Label 参数写出
         * @param[i] h5Group 
         * @param[i] paraLabel 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterLabelW(H5::Group& h5Group, Core::FITKParamLabel* paraLabel);
        /**
         * @brief    String 参数写出
         * @param[i] h5Group 
         * @param[i] paraString 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterStringW(H5::Group& h5Group, Core::FITKParamString* paraString);
        /**
         * @brief    RadioGroup 参数写出
         * @param[i] h5Group 
         * @param[i] paraRadioGroup 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterRadioGroupW(H5::Group& h5Group, Core::FITKParamRadioGroup* paraRadioGroup);
        /**
         * @brief    DoubleList 参数写出
         * @param[i] h5Group 
         * @param[i] paraDoubleList 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterDoubleListW(H5::Group& h5Group, Core::FITKParamDoubleList* paraDoubleList);
        /**
         * @brief    Bool Group参数写出
         * @param[i] h5Group 
         * @param[i] paraBoolGroup 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterBoolGroupW(H5::Group& h5Group, Core::FITKParamBoolGroup* paraBoolGroup);

        /**
         * @brief    参数读取工具
         * @param[i] h5Group 
         * @param[i] paraData 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterToolR(H5::Group& h5Group, Core::FITKParameter* paraData);
        /**
         * @brief    Bool参数读取
         * @param[i] h5Group 
         * @param[i] paraBool 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterBoolR(H5::Group& h5Group, Core::FITKParamBool* paraBool);
        /**
         * @brief    Combox参数读取
         * @param[i] h5Group 
         * @param[i] paraCombox 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterComboxR(H5::Group& h5Group, Core::FITKParamCombox* paraCombox);
        /**
         * @brief    Double参数读取
         * @param[i] h5Group 
         * @param[i] paraDouble 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterDoubleR(H5::Group& h5Group, Core::FITKParamDouble* paraDouble);
        /**
         * @brief    Int参数读取
         * @param[i] h5Group 
         * @param[i] paraInt 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterIntR(H5::Group& h5Group, Core::FITKParamInt* paraInt);
        /**
         * @brief    Label参数读取
         * @param[i] h5Group 
         * @param[i] paraLabel 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterLabelR(H5::Group& h5Group, Core::FITKParamLabel* paraLabel);
        /**
         * @brief    String参数读取
         * @param[i] h5Group 
         * @param[i] paraString 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterStringR(H5::Group& h5Group, Core::FITKParamString* paraString);
        /**
         * @brief    RadioGroup参数读取
         * @param[i] h5Group 
         * @param[i] paraRadioGroup 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterRadioGroupR(H5::Group& h5Group, Core::FITKParamRadioGroup* paraRadioGroup);
        /**
         * @brief    DoubleList参数读取
         * @param[i] h5Group 
         * @param[i] paraDoubleList 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterDoubleListR(H5::Group& h5Group, Core::FITKParamDoubleList* paraDoubleList);
        /**
         * @brief    Bool Group参数读取
         * @param[i] h5Group 
         * @param[i] paraBoolGroup 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-10
         */
        bool parameterBoolGroupR(H5::Group& h5Group, Core::FITKParamBoolGroup* paraBoolGroup);

    protected:
        /**
         * @brief HDF5读取器指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        IO::FITKAbstractHDF5Reader* _reader = nullptr;
        /**
         * @brief HDF5写入器指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        IO::FITKAbstractHDF5Writer* _writer = nullptr;
        /**
         * @brief HDF5组指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-07
         */
        H5::Group* _h5Group = nullptr;
    };
}


#endif
