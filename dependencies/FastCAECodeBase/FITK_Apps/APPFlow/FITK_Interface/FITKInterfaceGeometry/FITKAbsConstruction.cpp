/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsConstruction.h" 

namespace Interface
{
    FITKAbsConstruction::FITKAbsConstruction(Interface::FITKGeoEnum::FITKConstructionType type) :
        m_type(type)
    {

    }

    Interface::FITKGeoEnum::FITKGeometryComType FITKAbsConstruction::getGeometryCommandType()
    {
        return Interface::FITKGeoEnum::FGTConstruction;
    }

    void FITKAbsConstruction::setConstructionType(Interface::FITKGeoEnum::FITKConstructionType type)
    {
        m_type = type;
    }

    Interface::FITKGeoEnum::FITKConstructionType FITKAbsConstruction::getConstructionType()
    {
        // 获取类型。
        return m_type;
    }

    void FITKAbsConstruction::getPositions(double* point1, double* point2)
    {
        // 获取坐标。
        for (int i = 0; i < 3; i++)
        {
            point1[i] = m_point_1[i];
            point2[i] = m_point_2[i];
        }
    }

    void FITKAbsConstruction::setPositions(double* point1, double* point2)
    {
        // 保存坐标。
        for (int i = 0; i < 3; i++)
        {
            m_point_1[i] = point1[i];
            m_point_2[i] = point2[i];
        }
    }

    void FITKAbsConstruction::setPosition1(double x, double y, double z)
    {
        // 保存坐标。
        m_point_1[0] = x;
        m_point_1[1] = y;
        m_point_1[2] = z;
    }

    void FITKAbsConstruction::setPosition1(double* point)
    {
        // 保存坐标。
        for (int i = 0; i < 3; i++)
        {
            m_point_1[i] = point[i];
        }
    }

    void FITKAbsConstruction::setPosition2(double x, double y, double z)
    {
        // 保存坐标。
        m_point_2[0] = x;
        m_point_2[1] = y;
        m_point_2[2] = z;
    }

    void FITKAbsConstruction::setPosition2(double* point)
    {
        // 保存坐标。
        for (int i = 0; i < 3; i++)
        {
            m_point_2[i] = point[i];
        }
    }

    void FITKConstructionManager::appendDataObj(Core::FITKAbstractDataObject* item)
    {
        FITKAbsConstruction* cons = dynamic_cast<FITKAbsConstruction*>(item);
        if (!cons)
        {
            return;
        }

        // 检查是否存在名称。
        QString name = cons->getDataObjectName();
        if (name.isEmpty())
        {
            name = "Construction-1";
        }

        name = this->checkName(name);
        cons->setDataObjectName(name);

        Core::FITKAbstractDataManager<FITKAbsConstruction>::appendDataObj(cons);
    }

    FITKAbsConstruction* FITKConstructionManager::getLastConstruction(Interface::FITKGeoEnum::FITKConstructionType type)
    {
        int nData = this->getDataCount();
        if (nData == 0)
        {
            return nullptr;
        }

        // 返回任意类型。
        if (type == Interface::FITKGeoEnum::FITKConstructionType::FCTNone)
        {
            return this->getDataByIndex(nData - 1);
        }
        // 返回指定类型。
        else
        {
            for (int i = nData - 1; i >= 0; i--)
            {
                FITKAbsConstruction* cons = this->getDataByIndex(i);
                if (!cons)
                {
                    continue;
                }

                if (cons->getConstructionType() == type)
                {
                    return cons;
                }
            }
        }

        return nullptr;
    }
}