/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKDataCaseManager.h
 * @brief 算例数据管理器
 * @author fulipeng (flipengqd@yeah.net)
 * @date 2024-03-06
 *
 */

#ifndef _FITKDATACASEMANAGER_H_
#define _FITKDATACASEMANAGER_H_

#include "FITKAbaqusDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace AbaqusData
{
    class FITKDataCase;

    /**
     * @brief 算例数据管理器
     * @author fulipeng (flipengqd@yeah.net)
     * @date 2024-03-06
     */
    class FITKAbaqusDataAPI FITKDataCaseManager
        : public Core::FITKAbstractDataManager< FITKDataCase >
    {
    public:
        FITKDataCaseManager() = default;
        ~FITKDataCaseManager() = default;
        /**
         * @brief 移除数据对象
         * @param[i]  obj            要移除的数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        void removeDataObj(Core::FITKAbstractDataObject* obj) override;

        /**
         * @brief 移除第index个对象
         * @param[i]  index          对象索引
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataByIndex(int index) override;

        /**
         * @brief 根据数据ID移除对象
         * @param[i]  id            数据ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void removeDataByID(int id) override;

        /**
         * @brief 根据名称移除对象
         * @param[i]  name           对象名称
         * @param[i]  CompSens       大小写敏感，false时，全部转为小写对比查找
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void removeDataByName(const QString &name, bool compSens = false) override;
        /**
         * @brief 清空管理器,重写为空，不会调用父类函数，
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        virtual void clear() override {};
 
        /**
         * @brief 清空全部算例
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void clearAllCase();
        /**
         * @brief 检查算例数量，如果为空，会自动补充一个case
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-22
         */
        void checkCaseNum();
        /**
        * @brief 追加对象
        * @param[i] item          追加的数据对象
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-02-22
        */
        void appendDataObj(Core::FITKAbstractDataObject* item) override;
      
    };
}

#endif