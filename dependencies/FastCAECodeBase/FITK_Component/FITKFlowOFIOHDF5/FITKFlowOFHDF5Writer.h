/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5Writer.h
 * @brief  HDF5文件写出， 继承自线程任务。
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-07
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5WRITER_H__
#define __FITKFLOWOFHDF5WRITER_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

namespace IO
{
    /**
    * @brief HDF5文件写出
    * @author liuzhonghua (liuzhonghuaszch@163.com)
    * @date 2024-05-30
    */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5Writer : public FITKAbstractHDF5Writer
    {
    public:
        /**
         * @brief Construct a new FITKAbaqusHDF5Writer object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        explicit FITKFlowOFHDF5Writer() = default;
        virtual ~FITKFlowOFHDF5Writer() = default;

        /**
         * @brief 重写run函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-04-10
         */
        void run() override;

        /**
         * @brief 发送计算进度
         * @param Progress 进度百分比
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-04-10
         */
        void sendCalculateProgress(int Progress);

        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] str 数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-04-10
         */
        void consoleMessage(int level, const QString& str) override;

    private:

        /**
         * @brief 写出版本
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-04-10
         */
        bool writeVersion();
        /**
         * @brief    写出几何数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-07
         */
        bool writeDataGeometry();
        /**
         * @brief    写出几何分组
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-04
         */
        bool writeGeoComponent();
        /**
         * @brief    写出几何优化
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-04
         */
        bool writeGeometryRefine();
        /**
         * @brief    写出划分网格大小
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-04
         */
        bool writeGeoMeshSize();
        /**
         * @brief    写出材料点数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-04
         */
        bool writeMaterialPoint();
        /**
         * @brief     写出区域网格
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-04
         */
        bool writeRegionMesh();
        /**
         * @brief    写出网格数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool writeMesh();
        /**
         * @brief    写出物理属性数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool writePhysicsData();

    private:
    };
}


#endif
