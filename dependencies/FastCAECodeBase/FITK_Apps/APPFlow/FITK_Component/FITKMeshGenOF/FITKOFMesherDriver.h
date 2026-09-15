/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKOFMesherDriver.h
 * @brief  OpenFOAM网格驱动器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-18
 *
 */
#ifndef __FITK_MESHER_DRIVER_OF_H___
#define __FITK_MESHER_DRIVER_OF_H___

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"

namespace OF
{
    /**
     * @brief OpenFOAM网格驱动器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-18
     */
    class FITKOFMesherDriver : public Interface::FITKAbstractMesherDriver
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKOFMesherDriver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        explicit FITKOFMesherDriver() = default;
        /**
         * @brief Destroy the FITKOFMesherDriver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual ~FITKOFMesherDriver() = default;
        /**
         * @brief 启动网格划分程序
         * @param[i]  info           附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void startMesher(QStringList info = QStringList()) override;
        /**
         * @brief 结束网格划分程序
         * @param[i]  info           附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void stopMesher(QStringList info = QStringList()) override; 

    public slots:
        /**
         * @brief  blockMesh 命令行执行结束
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-28
         */
        void blockMeshSlot();
        /**
         * @brief    snappyHexMesh 命令行执行结束
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        void snappyHexMeshSlot();
        /**
         * @brief    createPatch 命令行执行结束
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        void createPatchSlot();

    protected:
    private:
    };
}


#endif
