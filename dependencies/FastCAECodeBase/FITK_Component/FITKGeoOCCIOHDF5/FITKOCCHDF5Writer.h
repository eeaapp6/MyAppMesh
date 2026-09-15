/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 *
 * @file   FITKHDF5Writer.h
 * @brief  HDF5文件写出， 继承自线程任务。
 * @author fulipeng (flipengqd@yeah.net)
 * @date   2024-09-09
 *
 *********************************************************************/
#ifndef __FITKABAQUSABAQUSWRITER_H__
#define __FITKABAQUSABAQUSWRITER_H__

#include "FITKGeoOCCIOHDF5API.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

ForwardDeclarNS(Core, FITKAbstractDataObject);
ForwardDeclarNS(H5, H5File);
ForwardDeclarNS(H5, Group);
ForwardDeclarNS(H5, H5Object);
ForwardDeclarNS(H5, DataType);

namespace IO
{
    /**
    * @brief HDF5文件写出
    * @author fulipeng (fulipengqd@yeah.net)
    * @date 2024-09-09
    */
    class FITKGEOOCCIOHDF5API FITKOCCHDF5Writer : public FITKAbstractHDF5Writer
    {
    public:
        /**
         * @brief Construct a new FITKOCCHDF5Writer object
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        explicit FITKOCCHDF5Writer() = default;
        virtual ~FITKOCCHDF5Writer() = default;

        /**
         * @brief 重写run函数
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        void run() override;

        /**
         * @brief 发送计算进度
         * @param Progress 进度百分比
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        void sendCalculateProgress(int Progress);

        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 数据
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        void consoleMessage(int level, const QString& str) override;

    private:

        /**
         * @brief 写出命令数据
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool writeCommDatas();

        /**
         * @brief 读取基准数据
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool writeDatumDatas();

        /**
         * @brief 写出信息
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool writeInfo();

        /**
         * @brief 写出版本
         * @return 状态 true成功， false失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-09-09
         */
        bool writeVersion(H5::Group* g);

    private:

    };
}


#endif
