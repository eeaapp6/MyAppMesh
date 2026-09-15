/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 *
 * @file   FITKHDF5Reader.h
 * @brief  HDF5文件读取， 继承自线程任务。
 * @author fulipeng (flipengqd@yeah.net)
 * @date   2024-09-09
 *
 *********************************************************************/
#ifndef __FITKOCCHDF5Reader_H__
#define __FITKOCCHDF5Reader_H__

#include "FITKGeoOCCIOHDF5API.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"

ForwardDeclarNS(Core, FITKAbstractNDataObject);
ForwardDeclarNS(H5, Group);
ForwardDeclarNS(H5, DataType);

namespace IO
{
    /**
    * @brief HDF5文件读取
    * @author fulipeng (fulipengqd@yeah.net)
    * @date 2024-09-09
    */
    class FITKGEOOCCIOHDF5API FITKOCCHDF5Reader : public FITKAbstractHDF5Reader
    {
    public:
        /**
         * @brief Construct a new FITKOCCHDF5Reader object
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        explicit FITKOCCHDF5Reader() = default;
        virtual ~FITKOCCHDF5Reader() = default;

        /**
         * @brief 重写run函数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        void run() override;

        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 数据
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        void consoleMessage(int level, const QString& str) override;

    protected:


    private:

        /**
         * @brief 读取命令数据
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool readCommDatas();

        /**
         * @brief 读取基准数据
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool readDatumDatas();

        /**
         * @brief 读取信息
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool readInfo();

        /**
         * @brief 读取版本
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool readVersion(H5::Group* g);

        /**
         * @brief 根据枚举名称实例化该类 GeoModel
         * @param enumName 枚举转换的字符串名称
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        Core::FITKAbstractNDataObject* getGeoCommand(QString enumName);

        /**
         * @brief 根据枚举名称实例化该类 GeoDatum
         * @param enumName 枚举转换的字符串名称
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        Core::FITKAbstractNDataObject* getGeoDatum(QString enumName);

    private:
    };
}


#endif
