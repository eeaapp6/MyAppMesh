/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelFilledRectangle.h
 * @brief  几何填充矩形适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-25
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELFILLEDRECTANGLE_H
#define FITKADAPTORGEOMODELFILLEDRECTANGLE_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  几何填充矩形适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-25
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelFilledRectangle : public FITKAdaptorGeoCommand
    {
    public:
        
        /**
         * @brief    构造
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-25
         */
        explicit FITKAdaptorGeoModelFilledRectangle() = default;
        
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-25
         */
        virtual ~FITKAdaptorGeoModelFilledRectangle() = default;

   
    protected:
        /**
         * @brief    参数数据写出
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-25
         */
        virtual bool writeParameters(H5::Group & g) override;
        /**
         * @brief    参数数据读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-25
         */
        virtual bool readParameters() override;
         

    private:
    };
}

#endif // FITKADAPTORGEOMODELFILLEDRECTANGLE_H
