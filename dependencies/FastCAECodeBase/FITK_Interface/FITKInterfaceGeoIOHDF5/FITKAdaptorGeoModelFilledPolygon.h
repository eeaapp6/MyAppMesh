/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelFilledPolygon.h
 * @brief  几何填充多边形适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2026-05-25
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELFILLEDPOLYGON_H
#define FITKADAPTORGEOMODELFILLEDPOLYGON_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  几何填充多边形适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2026-05-25
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelFilledPolygon : public FITKAdaptorGeoCommand
    {
    public:
        
        /**
         * @brief    构造
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-25
         */
        explicit FITKAdaptorGeoModelFilledPolygon() = default;
        
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2026-05-25
         */
        virtual ~FITKAdaptorGeoModelFilledPolygon() = default;

   
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

#endif // FITKADAPTORGEOMODELFILLEDPOLYGON_H
