/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKNumberComplex.h
 * @brief 复数声明 
 * @author libaojun (libaojunqd@foxmail.com)
 * @date 2024-10-21
 * 
 */
#ifndef _FITK_COMPLEX_NUMBER_H___
#define _FITK_COMPLEX_NUMBER_H___
 
#include "FITKCoreAPI.h"

namespace Core
{
    /**
     * @brief 复数定义
     * @author libaojun (libaojunqd@foxmail.com)
     * @date 2024-10-21
     */
    class FITKCoreAPI FITKNumberComplex
    {
    public:
        /**
         * @brief Construct a new FITKNumberComplex object
         * @param[i]  R              实部
         * @param[i]  I              虚部
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        FITKNumberComplex(const double& R, const double & I);
        /**
         * @brief Construct a new FITKNumberComplex object
         * @param[i]  number         被拷贝对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        FITKNumberComplex(const FITKNumberComplex& number);
        /**
         * @brief Construct a new FITKNumberComplex object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        FITKNumberComplex() = default;
        /**
         * @brief Destroy the FITKNumberComplex object
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        virtual ~FITKNumberComplex() = default;
         /**
          * @brief 获取实部
          * @return double 
          * @author libaojun (libaojunqd@foxmail.com)
          * @date 2024-10-21
          */
        double getReal() const;
        /**
         * @brief 设置实部
         * @param[i]  r              实部
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        void setReal(const double& r);
        /**
         * @brief 获取虚部
         * @return double 
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        double getImaginary() const;
        /**
         * @brief 设置虚部
         * @param[i]  i              虚部
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        void setImaginary(const double& i);

        /**
       * @brief 获取模
       * @return double
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-10-21
       */
        double getNorm() const;
    private:
        /**
         * @brief 实部
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        double _real{ 0 };
        /**
         * @brief 虚部
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-10-21
         */
        double _imaginary{ 0 };
    };
}


#endif
