/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGlobalMeshSizeInfo.h
 * @brief 全局网格尺寸类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-17
 *
 */
#ifndef __FITK_ABSTRACT_GLOBALINFO_H__
#define __FITK_ABSTRACT_GLOBALINFO_H__

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"

namespace Interface
{
    /**
     * @brief 全局网格尺寸
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-17
     */
    class FITKInterfaceMeshGenAPI FITKGlobalMeshSizeInfo :
        public Core::FITKAbstractNDataObject, public Core::FITKVarientParams
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKGlobalMeshSizeInfo object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        explicit FITKGlobalMeshSizeInfo();
        /**
         * @brief Destroy the FITKGlobalMeshSizeInfo object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        virtual ~FITKGlobalMeshSizeInfo() = default;
        /**
         * @brief 设置全局尺寸
         * @param[i]  s              全局尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void setGlobalSize(const double s);
        /**
         * @brief 获取全局尺寸
         * @return double 全局尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double getGlobalSize() const;
        /**
         * @brief 设置最小尺寸
         * @param[i]  s              最小尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void setMinSize(const double s);
        /**
         * @brief 获取最小尺寸
         * @return double 最小尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double getMinSize() const;
        /**
         * @brief 设置最大尺寸
         * @param[i]  s              最大尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void setMaxSize(const double s);
        /**
         * @brief 获取最大尺寸
         * @return double 最大尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double getMaxSize() const;
        /**
         * @brief 设置尺寸因子
         * @param[i]  f              尺寸因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void setSizeFactor(const double f);
        /**
         * @brief 获取尺寸因子
         * @return double 尺寸因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double getSizeFactor() const;
        /**
         * @brief 设置用户自定义标志
         * @param[i]  isCustom  是否用户自定义
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        void setUserCustom(const bool isCustom);
        /**
         * @brief 获取用户自定义标志
         * @return bool 是否用户自定义
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        bool isUserCustom() const;

    protected:
        /**
         * @brief 全局尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double _globalSize{ 0 };
        /**
         * @brief 最小尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double _minSize{ 1.0 };
        /**
         * @brief 最大尺寸
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double _maxSize{ 5.0 };
        /**
         * @brief 尺寸因子
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-17
         */
        double _sizeFactor{ 1.0 };
        /**
         * @brief  是否用户自定义
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date   2024-07-17
         */
        bool _isUserCustom{ true };
    };
}


#endif
