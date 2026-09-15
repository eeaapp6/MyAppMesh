/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostColorLibaryDataManager.h
 * @brief  流体后处理色库管理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-25
 *********************************************************************/
#ifndef __FITKCFDPostColorLibaryDataManager_H_
#define __FITKCFDPostColorLibaryDataManager_H_

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKCFDPostColorLibaryData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    /**
     * @brief  流体后处理色库管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-25
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostColorLibaryDataManager : public Core::FITKAbstractDataManager<FITKCFDPostColorLibaryData>
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPostColorLibaryDataManager() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostColorLibaryDataManager();
        /**
         * @brief    设置当期色库id
         * @param[i] id 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void setCurColorLibID(int id);
        /**
         * @brief    获取当前色库id
         * @return   int
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        int getCurColorLibID();
    protected:
        int _currentColoLibDataID = -1;
    };
}
#endif
