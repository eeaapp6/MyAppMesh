/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 *
 * @file   FITKAbaqusData.h
 * @brief  Abaqus 数据管理
 * @author fulipeng (flipengqd@yeah.net)
 * @date   2024-03-15
 *
 *********************************************************************/

#ifndef _ABAQUS_DATA_H__
#define _ABAQUS_DATA_H__

#include "FITKAbaqusDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace AbaqusData
{
    class FITKDataCaseManager;
    class FITKDataCase;
    class FITKJobManager;

    /**
     * @brief 数据类附加信息
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2025-02-13
     */
    struct AbaqusDataInfo 
    {
        QString _author{};    /// < 作者
        QString _describe{};   /// < 描述
    };

    /**
     * @brief Abaqus 数据管理
     * @author fulipeng (flipengqd@yeah.net)
     * @date 2024-03-06
     */
    class FITKAbaqusDataAPI FITKAbaqusData : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief 构造函数
         * @param initDefaultCase 是否初始化默认算例
         * @author libaojun
         * @date 2025-10-24
         */
        explicit FITKAbaqusData(bool initDefaultCase = true);
        virtual ~FITKAbaqusData();

        /**
         * @brief 获取数据管理
         * @return 数据管理
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-27
         */
        static FITKAbaqusData* GetDataFromAppFrame();

        /**
         * @brief 获取算例数据对象
         * @return 当前选择的算例数据对象
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        FITKDataCase* getCurrentCase();

        /**
         * @brief 设置当前选择的算例数据对象
         * @currentData 当前选择的算例数据对象
         * @return 设置状态 true 成功 false 失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-15
         */
        bool setCurrentCase(FITKDataCase* currentData);

        /**
         * @brief 获取算例数据ID
         * @return 当前选择的算例数据对象ID
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-26
         */
        int getCurrentCaseID();
        
        /**
         * @brief 设置当前选择的算例数据ID
         * @currentDataID 当前选择的算例数据对象ID
         * @return 设置状态 true 成功 false 失败
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-26
         */
        bool setCurrentCaseID(int currentDataID);


        /**
         * @brief 获取算例数据管理器
         * @return 算例数据管理器
         * @author fulipeng (fulipengqd@yeah.net)
         * @date 2024-03-26
         */
        FITKDataCaseManager* getDataCaseManager();
        /**
         * @brief 获取作业管理器
         * @return FITKJobManager* 
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        FITKJobManager* getJobManager();
        /**
         * @brief 获取数据附加信息
         * @return 数据附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-13
         */
        const AbaqusDataInfo& getAbaqusDataInfo() const;
        /**
         * @brief 设置数据附加信息
         * @param info 数据附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-13
         */
        void setAbaqusDataInfo(const AbaqusDataInfo& info);

    private:
        /**
         * @brief 数据附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-02-13
         */
        AbaqusDataInfo _abaDataInfo{};
        /**
         * @brief 当前选择的算例ID
         * @author fulipeng (flipengqd@yeah.net)
         * @date 2024-03-06
         */
        int _currentCaseID{ -1 };

        /**
         * @brief 算例管理器
         * @author fulipeng (flipengqd@yeah.net)
         * @date 2024-03-06
         */
        FITKDataCaseManager* _caseManager{};
        /**
         * @brief 作业管理器
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-04-01
         */
        FITKJobManager* _jobManager = nullptr;
    };
}



#endif
