/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_RAD__CASE_MANAGER_H__
#define _FITK_RAD__CASE_MANAGER_H__


#include "FITKRadiossDataAPI.h"
#include "FITKRadiossCase.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
    /**
     * @brief Radioss算例管理器类
     * @details 此类用于管理多个Radioss算例
     * @author libaojun
     * @date 2025-04-25
     */
    class FITKRadiossDataAPI FITKRadiossCaseManager : public Core::FITKAbstractDataObject,
         public Core::FITKAbstractDataManager<FITKRadiossCase>
    {
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         * @date 2025-04-25
         */
        explicit FITKRadiossCaseManager() = default;

        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2025-04-25
         */
        virtual ~FITKRadiossCaseManager() = default;
        /**
         * @brief 设置当前算例ID
         * @param[i]  caseID        当前算例ID
         * @author libaojun
         * @date 2025-09-09
         */
        void setCurrentCaseID(int caseID);
        /**
         * @brief 获取当前算例ID
         * @return int 当前算例ID
         * @author libaojun
         * @date 2025-09-09
         */
        int getCurrentCaseID() const;
        /**
         * @brief 获取当前算例
         * @return FITKRadiossCase* 当前算例指针，若无效则返回nullptr
         * @author libaojun 
         */
        FITKRadiossCase* getCurrentCase();
 
    private:
        int _currentCaseID{-1}; //!< 当前算例ID
    };

} // namespace Radioss


#endif // __FITK_RAD__CASE_MANAGER_H__