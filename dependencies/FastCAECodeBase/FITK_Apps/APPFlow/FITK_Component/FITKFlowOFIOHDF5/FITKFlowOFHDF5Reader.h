/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKFlowOFHDF5Reader.h
 * @brief  HDF5文件读取， 继承自线程任务。
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-07
 *********************************************************************/
#ifndef __FITKFLOWOFHDF5READER_H__
#define __FITKFLOWOFHDF5READER_H__

#include "FITKFlowOFIOHDF5API.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"

namespace IO
{
    /**
    * @brief HDF5文件读取
    * @author liuzhonghua (liuzhonghuaszch@163.com)
    * @date 2024-05-30
    */
    class FITKFLOWOFIOHDF5API FITKFlowOFHDF5Reader : public FITKAbstractHDF5Reader
    {
    public:
        /**
         * @brief Construct a new FITKAbaqusHDF5Reader object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        explicit FITKFlowOFHDF5Reader() = default;

        virtual ~FITKFlowOFHDF5Reader() = default;

        /**
         * @brief 重写run函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
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
         * @date 2024-05-30
         */
        void consoleMessage(int level, const QString& str) override;

    protected:

        /**
         * @brief 读取版本号
         * @return 状态 true成功， false失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-05-30
         */
        bool readVersion();
        /**
         * @brief    读取几何数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-09
         */
        bool readDataGeometry();
        /**
         * @brief    读取几何分组
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool readGeoComponent();
        /**
         * @brief    读取几何优化
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool readGeometryRefine();
        /**
         * @brief    读取划分网格大小
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool readGeoMeshSize();
        /**
         * @brief    读取材料点数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool readMaterialPoint();
        /**
         * @brief    读取区域网格
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool readRegionMesh();
        /**
         * @brief    读取网格数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool readMesh();
        /**
         * @brief    读取物理属性数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-05
         */
        bool readPhysicsData();

        /**
         * @brief    重置数据仓库
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-11
         */
        void resetDataObject();

    private:
    };
}


#endif
