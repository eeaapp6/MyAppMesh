/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKPostFieldsData.cpp
 * @brief       后处理场数据管理类实现。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-03-30
 *********************************************************************/

#include "FITKPostFieldsData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

namespace Interface
{
    FITKPostFieldsData::~FITKPostFieldsData()
    {
        clear();
    }

    void FITKPostFieldsData::addPointData(const QString & name, const QVector<double> & data, int components)
    {
        // 异常处理。
        if (name.isEmpty())
        {
            return;
        }

        if (components <= 0)
        {
            return;
        }

        m_pointData[name] = data;
        m_pointDataComponents[name] = components;
    }

    void FITKPostFieldsData::addCellData(const QString & name, const QVector<double> & data, int components)
    {
        // 异常处理。
        if (name.isEmpty())
        {
            return;
        }

        if (components <= 0)
        {
            return;
        }

        m_cellData[name] = data;
        m_cellDataComponents[name] = components;
    }

    QVector<double> FITKPostFieldsData::getPointData(const QString & name) const
    {
        // 异常处理。
        if (name.isEmpty())
        {
            return QVector<double>();
        }

        if (!m_pointData.contains(name))
        {
            return QVector<double>();
        }

        return m_pointData.value(name);
    }

    QVector<double> FITKPostFieldsData::getCellData(const QString & name) const
    {
        // 异常处理。
        if (name.isEmpty())
        {
            return QVector<double>();
        }

        if (!m_cellData.contains(name))
        {
            return QVector<double>();
        }

        return m_cellData.value(name);
    }

    int FITKPostFieldsData::getPointDataComponents(const QString & name) const
    {
        // 异常处理。
        if (name.isEmpty())
        {
            return 0;
        }

        if (!m_pointDataComponents.contains(name))
        {
            return 0;
        }

        return m_pointDataComponents.value(name);
    }

    int FITKPostFieldsData::getCellDataComponents(const QString & name) const
    {
        // 异常处理。
        if (name.isEmpty())
        {
            return 0;
        }

        if (!m_cellDataComponents.contains(name))
        {
            return 0;
        }

        return m_cellDataComponents.value(name);
    }

    QStringList FITKPostFieldsData::getPointDataNames() const
    {
        return m_pointData.keys();
    }

    QStringList FITKPostFieldsData::getCellDataNames() const
    {
        return m_cellData.keys();
    }

    void FITKPostFieldsData::clear()
    {
        // 清空数据。
        m_pointData.clear();
        m_cellData.clear();
        m_pointDataComponents.clear();
        m_cellDataComponents.clear();
    }
}
