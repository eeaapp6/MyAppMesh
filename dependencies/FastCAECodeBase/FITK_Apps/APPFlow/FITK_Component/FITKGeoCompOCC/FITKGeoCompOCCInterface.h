/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGeoCompOCCInterface.h
 * @brief       OCC几何组件接口类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-10-16
 *********************************************************************/

#ifndef  _FITKGEOCOMPOCCINTERAFACE_H___
#define  _FITKGEOCOMPOCCINTERAFACE_H___

#include "FITKGeoCompOCCAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"

namespace OCC
{
    /**
     * @brief       OCC几何组件接口类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-10-16
     */
    class FITKGeoCompOCCAPI FITKGeoCompOCCInterface :
        public AppFrame::FITKComponentInterface
    {
    public:
        explicit FITKGeoCompOCCInterface();
        virtual ~FITKGeoCompOCCInterface();

        
        /**
        * @brief 获取部件名称，不能重复  return "FITKGeoCompOCC"
        * @return QString
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-03-04
        */
        virtual QString getComponentName() override;
 
        /**
         * @brief 执行读取或写出
         * @param indexPort    
         * @return
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-07
         */
        virtual bool exec(const int indexPort) override;
         
        
    };
}


#endif // 