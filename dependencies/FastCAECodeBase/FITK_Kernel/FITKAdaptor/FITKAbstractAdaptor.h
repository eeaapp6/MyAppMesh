/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractAdaptor.h
 * @brief  适配器抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-03
 *
 */
#ifndef __FITKABSTRACTADAPTOR_H__
#define __FITKABSTRACTADAPTOR_H__

#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"
#include "FITKAdaptorAPI.h"
#include <QString>

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Adaptor
{
    /**
     * @brief 适配器类型
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-03
     */
    enum FITKAdaptorType
    {
        AdaTNone,
        AdaTIO,
        AdaTView,
    };
    /**
     * @brief 抽象适配器类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-03
     */
    class FITKAdaptorAPI FITKAbstractAdaptor : public Core::FITKAbstractObject
    {
        FITKCLASS(Adaptor,FITKAbstractAdaptor);
    public:
        /**
         * @brief Construct a new FITKAbstractAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        explicit FITKAbstractAdaptor() = default;
        /**
         * @brief Destroy the FITKAbstractAdaptor object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual ~FITKAbstractAdaptor() = 0;
        /**
         * @brief 适配器对应的数据类型
         * @return QString
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual QString getAdaptorClass() = 0;
        /**
         * @brief 获取适配器类型
         * @return FITKAdaptorType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual FITKAdaptorType getAdaptorType() = 0;
        /**
         * @brief 设置适配器对应的数据对象
         * @param[i] data 数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual void setDataObject(Core::FITKAbstractDataObject* data);
        /**
         * @brief 适配数据，执行业务逻辑
         * @return bool
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-03
         */
        virtual bool adapt();
         
        template<typename T>
        /**
         * @brief 获取适配器对应的数据对象
         * @return Core::FITKAbstractDataObject* 数据对象指针
         * @author LiBaojun (libaojunqd@foxmail.com)    
         * @date 2024-03-03
         */
        T* getDataObjectAs()
        {
            return dynamic_cast<T*>(_dataObj);
        }
        

    protected:
        /**
         * @brief 数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-13
         */
        Core::FITKAbstractDataObject* _dataObj{};

    };

}



#endif
