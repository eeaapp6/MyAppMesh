/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAdaptorGeoModelBox.h
 * @brief  几何box适配器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-04-28
 *********************************************************************/
#ifndef FITKADAPTORGEOMODELBOX_H
#define FITKADAPTORGEOMODELBOX_H

#include "FITKAdaptorGeoCommand.h"
#include "FITKInterfaceGeoIOHDF5API.h"

namespace Interface
{
    /**
     * @brief  几何box适配器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-04-28
     */
    class FITKINTERFACEGEOIOHDF5API FITKAdaptorGeoModelBox : public FITKAdaptorGeoCommand
    {
    public:
        
        /**
         * @brief    构造
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        explicit FITKAdaptorGeoModelBox() = default;
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual ~FITKAdaptorGeoModelBox() = default;

   
    protected:
        /**
         * @brief    参数数据写出
         * @param[i] g 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool writeParameters(H5::Group & g) override;
        /**
         * @brief    参数数据读取
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-04-28
         */
        virtual bool readParameters() override;
         

    private:
        // 添加需要的私有成员
    };
}

#endif // FITKADAPTORGEOMODELBOX_H
