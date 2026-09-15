/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaWire.h
 * @brief 线模型
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-06-24
 *
 */
#ifndef _FITKABA__WIRE_H_
#define _FITKABA__WIRE_H_

#include "FITKInterfaceStructuralAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKAbaConnectorElement.h"

namespace Interface
{
    /**
     * @brief 线模型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    class  FITKInterfaceStructuralAPI FITKAbaWire : public Core::FITKAbstractNDataObject,
        public Core::FITKAbstractDataManager<FITKAbaAssEle3D2>
    {
        FITKCLASS(Interface, FITKAbaWire);
    public:
        /**
         * @brief Construct a new FITKAbaWire object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        explicit FITKAbaWire() = default;
        /**
         * @brief Destroy the FITKAbaWire object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual ~FITKAbaWire() = default;
        /**
         * @brief 获取点的标签
         * @param[i]  index          索引
         * @param[o]  label1         第一点标签
         * @param[o]  label2         第二点标签
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void getTextLabel(const int index, QString& label1, QString &label2);
        /**
         * @brief 获取坐标
         * @param[i]  index          索引
         * @param[o]  coor1          第一点坐标
         * @param[o]  coor2          第二点坐标
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        void getCoor(const int index, double* coor1, double* coor2);

        /**
         * @brief 拷贝对象
         * @param[i]  obj         要拷贝的对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        bool copy(FITKAbstractDataObject* obj) override;
        /**
         * @brief 根据ID获取单元
         * @param[i]  id         单元id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-02
         */
        FITKAbaAssEle3D2* getElementByID(const int& id);

        /**
         * @brief       设置激活状态。
         * @param[in]   state：激活状态
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-18
         */
        void setActiveState(bool state);

        /**
         * @brief       获取激活状态。
         * @return      激活状态
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-18
         */
        bool getActiveState();

    protected:
        /**
         * @brief       激活状态。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-18
         */
        bool m_activeState = true;

    };

    /**
     * @brief 线模型管理器
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-06-24
     */
    class  FITKInterfaceStructuralAPI FITKAbaWireManager :
        public Core::FITKAbstractDataManager<FITKAbaWire>
    {
    public:
        /**
         * @brief Construct a new FITKAbaWireManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        explicit FITKAbaWireManager() = default;
        /**
         * @brief Destroy the FITKAbaWireManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-06-24
         */
        virtual ~FITKAbaWireManager() = default;
        /**
         * @brief 根据ID获取单元
         * @param[i]  id         单元id
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-02
         */
        FITKAbaAssEle3D2* getElementByID(const int& id);

    };
}

#endif