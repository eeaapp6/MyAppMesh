/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKStruPost3DManager.h
 * @brief 三维后处理数据管理
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-19
 *
 */
#ifndef __FITKSTRUPOST3DMANAGER_H__
#define __FITKSTRUPOST3DMANAGER_H__

#include "FITKInterfaceStructuralPostAPI.h"
#include "FITKAbstractStruPostVTK.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include <QList>

namespace Interface
{
    /**
     * @brief 后处理数据管理
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    class FITKInterfaceStructuralPostAPI FITKStruPost3DManager
        :public Core::FITKAbstractDataManager<FITKAbstractStructuralPostVTK>
    {
    public:
        /**
         * @brief Construct a new FITKStruPost3DManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        explicit FITKStruPost3DManager() = default;
        /**
         * @brief Destroy the FITKStruPost3DManager object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual ~FITKStruPost3DManager() = default;
        /**
         * @brief 根据ID获取数据
         * @param[i]  id
         * @return FITKAbstractStructuralPostVTK*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual FITKAbstractStructuralPostVTK* getDataByID(const int id) override;
        /**
         * @brief 获取独立的数据，独立于前处理模型
         * @return QList<FITKAbstractStructuralPostVTK*>
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        QList<FITKAbstractStructuralPostVTK*> getDataIndependence();

    protected:
    private:
    };
}


#endif
