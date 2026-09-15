/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKPostFieldsData.h
 * @brief       后处理场数据管理类声明。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-03-30
 *********************************************************************/

#ifndef __FITKPOSTFIELDSDATA_H__
#define __FITKPOSTFIELDSDATA_H__

#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKInterfaceModelAPI.h"

// Qt
#include <QMap>
#include <QVector>

namespace Interface
{
    /**
     * @brief       后处理场数据管理类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-03-30
     */
    class FITKInterfaceModelAPI FITKPostFieldsData : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        explicit FITKPostFieldsData() = default;

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        virtual ~FITKPostFieldsData();

        /**
         * @brief       添加节点场数据。
         * @param[in]   name：数据名称
         * @param[in]   data：数据数组
         * @param[in]   components：数据维度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        void addPointData(const QString & name, const QVector<double> & data, int components);

        /**
         * @brief       添加单元场数据。
         * @param[in]   name：数据名称
         * @param[in]   data：数据数组
         * @param[in]   components：数据维度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        void addCellData(const QString & name, const QVector<double> & data, int components);

        /**
         * @brief       获取节点场数据。
         * @param[in]   name：数据名称
         * @return      数据数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QVector<double> getPointData(const QString & name) const;

        /**
         * @brief       获取单元场数据。
         * @param[in]   name：数据名称
         * @return      数据数组
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QVector<double> getCellData(const QString & name) const;

        /**
         * @brief       获取节点场数据维度。
         * @param[in]   name：数据名称
         * @return      数据维度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        int getPointDataComponents(const QString & name) const;

        /**
         * @brief       获取单元场数据维度。
         * @param[in]   name：数据名称
         * @return      数据维度
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        int getCellDataComponents(const QString & name) const;

        /**
         * @brief       获取所有节点场数据名称。
         * @return      数据名称列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QStringList getPointDataNames() const;

        /**
         * @brief       获取所有单元场数据名称。
         * @return      数据名称列表。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QStringList getCellDataNames() const;

        /**
         * @brief       清除所有数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        void clear();

    private:
        /**
         * @brief       节点场数据存储。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QMap<QString, QVector<double>> m_pointData;

        /**
         * @brief       单元场数据存储。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QMap<QString, QVector<double>> m_cellData;

        /**
         * @brief       节点场数据维度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QMap<QString, int> m_pointDataComponents;

        /**
         * @brief       单元场数据维度。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-03-30
         */
        QMap<QString, int> m_cellDataComponents;

    };
}

#endif // !__FITKPOSTFIELDSDATA_H__