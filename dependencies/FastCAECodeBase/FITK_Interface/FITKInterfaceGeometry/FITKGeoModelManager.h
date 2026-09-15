/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKGeoModelManager.h
 * @brief 几何模型管理器声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-12
 *
 */
#ifndef _FITKGEOMODEL_MANAGER_H___
#define _FITKGEOMODEL_MANAGER_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
   /**
    * @brief  几何模型管理器
    * @author LiBaojun (libaojunqd@foxmail.com)
    * @date 2024-07-12
    */
    class FITKInterfaceGeometryAPI FITKGeoModelManager :
        public Core::FITKAbstractDataManager<FITKAbstractGeoModel>
    {
    public:
        /**
         * @brief Construct a new FITKGeoModelManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        explicit FITKGeoModelManager();
        /**
         * @brief Destroy the FITKGeoModelManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        virtual ~FITKGeoModelManager();

        template <class T>
        /**
         * @brief 获取第index个对象，并且进行类型转换
         * @param[i]  index          索引
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-12
         */
        T* getDataByIndexT(const int index)
        {
            return dynamic_cast<T*>(this->getDataByIndex(index));
        }


        /**
         * @brief 获取全部模型包围盒
         * @param[o] minPt 最小点位置
         * @param[o] maxPt 最大点位置
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-30
         */
        void getBoundaryBox(double* minPt, double* maxPt);
    };
}

#endif
