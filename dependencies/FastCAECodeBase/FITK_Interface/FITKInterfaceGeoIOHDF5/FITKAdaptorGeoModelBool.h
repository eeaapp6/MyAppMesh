/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelBool.h
 * @brief  几何Bool运算适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELBOOL_H
#define FITKADAPTORGEOMODELBOOL_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  几何Bool运算适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelBool : public FITKAdaptorGeoCommand
    {
    public:
        /**
         * @brief    构造
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        explicit FITKAdaptorGeoModelBool() = default;
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual ~FITKAdaptorGeoModelBool() = default;

   
    protected:
        /**
         * @brief    写出参数数据
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeParameters(H5::Group & g) override;
        /**
         * @brief    读取参数数据
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readParameters() override;
         

    private:
        // 添加需要的私有成员
    };

    /**
     * @brief  多实体布尔求和适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-08-05
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoOperNBodyUnite : public FITKAdaptorGeoCommand
    {
    public:
        explicit FITKAdaptorGeoOperNBodyUnite() = default;
        virtual ~FITKAdaptorGeoOperNBodyUnite() = default;

    protected:
        virtual bool writeParameters(H5::Group & g) override;
        virtual bool readParameters() override;

    private:
        // 添加需要的私有成员
    };
}

#endif // FITKADAPTORGEOMODELBOOL_H
