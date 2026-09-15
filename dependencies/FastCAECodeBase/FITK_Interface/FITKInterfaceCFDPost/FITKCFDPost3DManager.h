/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief  流体三维后处理管理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 */
#ifndef __FITKSTRUPOST3DMANAGER_H__
#define __FITKSTRUPOST3DMANAGER_H__

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include <QList>

namespace Interface
{
    /**
     * @brief  流体三维后处理管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPost3DManager : public Core::FITKAbstractDataManager<FITKAbstractCFDPostData>
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPost3DManager() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPost3DManager();
    };
}


#endif
