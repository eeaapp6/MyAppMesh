/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbaqusPostData.h
 * @brief  Abaqus Post Data
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-05-19
 *
 */
#ifndef __FITKABAQUSPOST_DATA_H__
#define __FITKABAQUSPOST_DATA_H__

#include "FITKAbaqusDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface
{
    class FITKStruPostSetting;
    class FITKStruPost3DManager;
    class FITKPostGroupManager;
}

namespace AbaqusData
{
    /**
     * @brief Abaqus Post Data
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-05-19
     */
    class FITKAbaqusDataAPI FITKAbaqusPostData
        : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief Construct a new FITKAbaqusPostData object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        explicit FITKAbaqusPostData();
        /**
         * @brief Destroy the FITKAbaqusPostData object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        virtual ~FITKAbaqusPostData();
        /**
         * @brief 从app框架和获取傻乎乎句
         * @return FITKAbaqusPostData*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        static FITKAbaqusPostData* GetDataFromAPPFrame();
        /**
         * @brief Get the Post Setting
         * @return Interface::FITKStruPostSetting*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        Interface::FITKStruPostSetting* getPostSetting();
        /**
         * @brief 获取三维后处理数据管理
         * @return Interface::FITKStruPost3DManager*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        Interface::FITKStruPost3DManager* getPost3DManager();

        /**
         * @brief       获取后处理显示分组管理器。
         * @return      显示分组管理器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        Interface::FITKPostGroupManager* getPostDisplayGroupManager();

        /**
         * @brief       获取当前激活后处理显示分组数据ID。
         * @return      显示分组数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        int getCurrentPostDisplayGroupDataID();

        /**
         * @brief       设置当前激活后处理显示分组数据ID。
         * @param[in]   id：显示分组数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        void setCurrentPostDisplayGroupDataID(const int id);

        int getCurrentPostDataID();

        void setCurrentPostDataID(const int id);

    private:
         /**
          * @brief 后处理相关设置
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-05-19
          */
        Interface::FITKStruPostSetting* _postSetting{};
        /**
         * @brief 三维对象数据管理
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-05-19
         */
        Interface::FITKStruPost3DManager* _post3DManager{};

        /**
         * @brief       后处理分组管理器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        Interface::FITKPostGroupManager* _postGroupManager{ nullptr };

        int _currentDataID{ -1 };

    };
}

#endif
