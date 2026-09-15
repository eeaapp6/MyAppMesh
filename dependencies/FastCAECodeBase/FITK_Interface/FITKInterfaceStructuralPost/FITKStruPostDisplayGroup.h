/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKStruPostDisplayGroup.h
 * @brief       后处理显示分组数据与管理器。

 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-10-17
 *********************************************************************/

#ifndef __FITKSTRUPOSTDISPLAYGROUP_H__
#define __FITKSTRUPOSTDISPLAYGROUP_H__

#include "FITKInterfaceStructuralPostAPI.h"

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

// 前置声明

namespace Interface
{
    class FITKPostGroupManager;

    /**
     * @brief       后处理显示分组数据类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-17
     */
    class FITKInterfaceStructuralPostAPI FITKStruPostDisplayGroup : public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Interface, FITKStruPostDisplayGroup);
    public:
        /**
         * @brief       显示分组类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        enum DisplayGroupType
        {
            DGT_All = 0,
            DGT_Selections
        };

        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        explicit FITKStruPostDisplayGroup();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        virtual ~FITKStruPostDisplayGroup();

        /**
         * @brief       设置显示分组单元索引列表。
         * @param[in]   indice：单元索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        void setDisplayGroupEleIndice(QList<int> indice);

        /**
         * @brief       获取显示分组单元索引列表。
         * @return      单元索引列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        QList<int> getDisplayGroupEleIndice();

        /**
         * @brief       获取显示分组单元数量。
         * @return      数量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        int getNumberOfDisplayGroupMember();

        /**
         * @brief       获取显示分组类型。
         * @return      显示分组类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        DisplayGroupType getDisplayGroupType();

        /**
         * @brief       获取后处理数据ID。
         * @return      后处理数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        int getPostDataID();

    protected:
        /**
         * @brief       设置后处理数据ID。
         * @param[in]   postDataID：后处理数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        void setPostDataID(int postDataID);

    protected:
        /**
         * @brief       显示分组类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-19
         */
        DisplayGroupType m_type = DisplayGroupType::DGT_Selections;

        /**
         * @brief       显示分组单元成员列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        QList<int> m_members;

        /**
         * @brief       后处理数据对象ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        int m_postDataID = -1;

        friend FITKPostGroupManager;
    };

    // Manager
    //@{
    /**
     * @brief       后处理显示分组管理器。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-10-17
     */
    class FITKInterfaceStructuralPostAPI FITKPostGroupManager : public Core::FITKAbstractDataManager<FITKStruPostDisplayGroup>
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        explicit FITKPostGroupManager();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        virtual ~FITKPostGroupManager() = default;

        /**
         * @brief       设置当前激活的显示分组。
         * @param[in]   groupID：显示分组ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        void setCurrentGroupID(int groupID);

        /**
         * @brief       设置当前激活的显示分组。
         * @param[in]   groupData：显示分组数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        void setCurrentGroup(FITKStruPostDisplayGroup* groupData);

        /**
         * @brief       获取当前正在激活的显示分组。
         * @return      当前显示分组ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        int getCurrentGroupID();

        /**
         * @brief       获取当前正在激活的显示分组。
         * @return      当前显示分组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        FITKStruPostDisplayGroup* getCurrentGroup();

    private:
        /**
         * @brief       当前显示分组ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-17
         */
        int m_currentGroupID = -1;

    };
    //@}
}

#endif
