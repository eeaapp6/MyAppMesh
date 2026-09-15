/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractModelComponent.h
 * @brief 模型组件类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-27
 *
 */
#ifndef __FITKABSTRACTCOMPONENT_H___
#define __FITKABSTRACTCOMPONENT_H___

#include <QColor>
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKModelEnum.h"
#include "FITKInterfaceModelAPI.h"

namespace Interface
{
    class FITKAbstractModel;
    /**
     * @brief 模型组件抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-02-27
     */
    class FITKInterfaceModelAPI FITKAbstractModelComponent : public Core::FITKAbstractNDataObject
    {
    public:
        /**
         * @brief Construct a new FITKAbstractModelComponent object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        explicit FITKAbstractModelComponent();
        /**
         * @brief Destroy the FITKAbstractModelComponent object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual ~FITKAbstractModelComponent() = 0;
        /**
         * @brief 通过ID设置属于的模型
         * @param[i]  模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        void setModel(const int & parentID);
        /**
         * @brief 通过模型指针设置属于的模型
         * @param[i]  model          设置属于的模型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        void setModel(FITKAbstractModel* model);
        /**
         * @brief 获取模型ID
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        int getAbsModelID() const;
        /**
         * @brief 获取模型
         * @return FITKAbstractModel*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        FITKAbstractModel* getAbstractModel();

        template <class T>
        /**
         * @brief 获取模型并进行强制类型转换
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        T* getAbstractModelAs()
        {
            return dynamic_cast<T*>(this->getAbstractModel());
        }

        /**
         * @brief 设置颜色
         * @param[i]  color          设置颜色
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-10
         */
        void setColor(const QColor& color) ;
        /**
         * @brief 获取颜色
         * @return QColor 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-10
         */
        QColor getColor() const;


    private:
        /**
         * @brief 模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        int _absModelID{-1};
        /**
         * @brief 渲染颜色
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-04-10
         */
        QColor _color{Qt::black}; 
    };


}


#endif
