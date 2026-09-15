/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKComponents.h
 * @brief  组件管理类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-04
 *
 */
#ifndef __FITK_COMPONENTS_H__
#define __FITK_COMPONENTS_H__

#include "FITKAppFrameworkAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include <QList>

namespace AppFrame
{
    class FITKComponentFactory;
    class FITKComponentInterface;
    /**
     * @brief 组件管理类声明
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-03-04
     */
    class FITKAppFrameworkAPI FITKCmponents : public Core::FITKAbstractObject
    {
        friend FITKComponentFactory;
    public:
        /**
         * @brief Construct a new FITKCmponents object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        explicit FITKCmponents() = default;
        /**
         * @brief Destroy the FITKCmponents object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        ~FITKCmponents();
        /**
         * @brief 获取组件数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        int getComponentCount() const;
        /**
         * @brief 根据索引获取组件
         * @param[i]  index         索引
         * @return FITKComponentInterface*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        FITKComponentInterface* getComponentAt(const int index);
        /**
         * @brief 根据名字获取组件
         * @param[i]  compName       组件名称
         * @return FITKComponentInterface*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        FITKComponentInterface* getComponentByName(const QString& compName);

        template<class T>
        /**
         * @brief 根据名字获取组件，并转换类型
         * @param[i]  compName       组件名称
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        T* getComponentTByName(const QString& compName)
        {
            return  dynamic_cast<T*> (this->getComponentByName(compName));
        }
   
        /**
         * @brief 追加组件
         * @param[i]  comp           组件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        void addComponent(FITKComponentInterface* comp);
        /**
         * @brief 移除组件
         * @param[i]  comp           组件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-03-06
         */
        void removeComponent(FITKComponentInterface* comp);

        /**
        * @brief 获取打开工程文件不释放的数据对象
        * @author libaojun (libaojunqd@foxmail.com)
        * @date 2025-08-18
        */
        QList<int> getUnResetDataObj();

    private:
        /**
         * @brief 组件管理列表
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-04
         */
        QList<FITKComponentInterface*> _components;

    };

}


#endif