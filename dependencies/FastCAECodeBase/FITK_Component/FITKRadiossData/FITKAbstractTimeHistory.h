/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractTimeHistory.h
 * @brief  输出请求时间历程基类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-23
 *********************************************************************/
#ifndef FITK_ABSTRACT_TIMEHISTORY_H_RADIOSS
#define FITK_ABSTRACT_TIMEHISTORY_H_RADIOSS

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKAbstractRadiossData.h"

namespace Radioss
{
    /**
     * @brief  输出请求时间历程基类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKAbstractTimeHistory : public Core::FITKAbstractNDataObject
    {
        Q_OBJECT
    public:
        enum TimeHistoryType
        {
            TimeHistory_Unknown = 0,
            TimeHistory_ACCEL = 1,
            TimeHistory_INTER = 2,
            TimeHistory_RWALL = 3,
            TimeHistory_RBODY = 4,
            TimeHistory_SECTIO = 5,
            TimeHistory_SPHCEL = 6,
        };
        Q_ENUM(TimeHistoryType);
    public:
        explicit FITKAbstractTimeHistory() = default;
        virtual ~FITKAbstractTimeHistory() = 0;
        /**
         * @brief    获取时间历程类型
         * @return   TimeHistoryType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        virtual TimeHistoryType getTimeHistoryType() = 0;

        /**
         * @brief    设置对象ID
         * @param[i] objIDs 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setObjIDs(QList<int> objIDs);
        /**
         * @brief    获取对象ID
         * @return   QList<int>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        QList<int> getObjIDs();

        /**
         * @brief   获取变量数据字符串
         * @param[i] isRemoveInclude 是否剔除包含关系的字符串，默认剔除
         */
        virtual QStringList getVariableKeyWords(bool isRemoveInclude=true) = 0;

    protected:
        /**
         * @brief  初始为自定义变量组类型-不同类型的时间历程对应的变量组类型和表示的变量都不一样
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        int m_varlableGroupType{ -1 };
        /**
         * @brief  只有变量组为自定义类型时，才能设置变量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        QList<int> m_customizeVarlable{};
        /**
         * @brief  对象ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        QList<int> m_objID{};
    };

    /**
     * @brief  输出请求时间历程管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKTimeHistoryManager :
        public Core::FITKAbstractDataManager<FITKAbstractTimeHistory>
    {
    public:
        explicit FITKTimeHistoryManager() = default;
        virtual ~FITKTimeHistoryManager() = default;
        /**
         * @brief    通过类型获取时间历程
         * @param[i] type 
         * @return   QList<FITKAbstractTimeHistory *>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        QList<FITKAbstractTimeHistory*> getTimeHistoryByType(FITKAbstractTimeHistory::TimeHistoryType type);
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_TIMEHISTORY_H_RADIOSS