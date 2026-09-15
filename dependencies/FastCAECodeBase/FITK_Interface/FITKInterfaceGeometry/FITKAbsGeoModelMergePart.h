/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKAbsGeoModelMergePart.h
 * @brief       模型合并命令抽象类。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-04-13
 *********************************************************************/

#ifndef  __FITKABSGEOMODELMERGEPART_H__
#define  __FITKABSGEOMODELMERGEPART_H__

#include "FITKAbsGeoCommand.h"
#include "FITKGeoEnum.h"
#include "FITKInterfaceGeometryAPI.h"

#include <QList>

namespace Interface
{
    /**
     * @brief       模型合并命令抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-04-13
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMergePart : public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoModelMergePart);
        Q_OBJECT
    public:
        /**
         * @brief       布尔运算类型枚举。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        enum GeoBoolOperType
        {
            GBT_None = 0,     ///< 无操作
            GBT_Common,       ///< 求交（Common）
            GBT_Cut,          ///< 求差（Cut）
            GBT_Add,          ///< 求和（Add/Unite）
        };
        Q_ENUM(GeoBoolOperType)

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKAbsGeoModelMergePart() = default;

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        virtual ~FITKAbsGeoModelMergePart() = default;

        /**
         * @brief       获取数据类型。[重写]
         * @return      数据类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        /**
         * @brief       设置布尔运算类型。
         * @param[in]   type：布尔运算类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setMergeOperType(GeoBoolOperType type);

        /**
         * @brief       获取布尔运算类型。
         * @return      布尔运算类型
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        GeoBoolOperType getMergeOperType() const;

        /**
         * @brief       设置要合并的命令ID列表。
         * @param[in]   commandIds：命令ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void setCommandIds(const QList<int>& commandIds);

        /**
         * @brief       获取要合并的命令ID列表。
         * @return      命令ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        QList<int> getCommandIds() const;

        /**
         * @brief       添加单个命令ID。
         * @param[in]   commandId：命令ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void addCommandId(int commandId);

        /**
         * @brief       清除所有命令ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        void clearCommandIds();

    protected:
        /**
         * @brief       布尔运算类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        GeoBoolOperType m_boolOperType{ GBT_None };

        /**
         * @brief       要合并的命令ID列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        QList<int> m_cmdIds;

        /**
         * @brief       暂存上次保存的命令ID列表，用于撤销恢复时还原引用关系。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-04-13
         */
        QList<int> m_lastSavedCmdIds;

    };
}

#endif // !__FITKABSGEOMODELMERGEPART_H__
