/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAssembly.h
 * @brief 声明装配体
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-22
 *
 */
#ifndef __FITKASSEMBLY_H__
#define __FITKASSEMBLY_H__

#include "FITKInterfaceModelAPI.h"
#include "FITKAbstractModel.h"
#include "FITKAbstractAssInstance.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief 装配体
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-22
     */
    class FITKInterfaceModelAPI FITKAssembly : public FITKAbstractModel,
        public Core::FITKAbstractDataManager<FITKAbstractAssInstance>
    {
        FITKCLASS(Interface, FITKAssembly);
    public:
       /**
        * @brief Construct a new FITKAssembly object
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-22
        */
        explicit FITKAssembly();
        /**
         * @brief Destroy the FITKAssembly object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        virtual ~FITKAssembly();

        /**
         * @brief 获取模型类型
         * @return AbsModelType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual FITKModelEnum::AbsModelType getAbsModelType() override;


        template<class T>
        /**
         * @brief 获取装配实例并进行类型转换
         * @param[i]  index         索引
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        T* getInstanceTAt(const int index)
        {
            auto d = this->getDataByIndex(index);
            return dynamic_cast<T*>(d);
        }
        
        /**
         * @brief 通过移除
         * @param[i]  index         索引
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        virtual void removeInstanceByModel(const int modelID);

    private:


    };
}


#endif // !__FITKASSEMBLY_H__
