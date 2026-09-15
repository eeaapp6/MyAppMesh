/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoDatum.h" 

#include "FITKGeoInterfaceFactory.h" 

namespace Interface
{
    // 基准元素抽象类。
    //@{
    void FITKAbsGeoDatum::setEditable(bool flag)
    {
        m_createdType = flag ? DatumCreatedType::DCT_UserDefine : DatumCreatedType::DCT_System;
    }

    void FITKAbsGeoDatum::setPosition(double* pos)
    {
        // 保存位置。
        if (pos)
        {
            m_pos[0] = pos[0];
            m_pos[1] = pos[1];
            m_pos[2] = pos[2];
        }
    }

    void FITKAbsGeoDatum::getPosition(double* pos)
    {
        if (pos)
        {
            pos[0] = m_pos[0];
            pos[1] = m_pos[1];
            pos[2] = m_pos[2];
        }
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoDatum::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTDatum;
    }

    bool FITKAbsGeoDatum::editable()
    {
        return m_createdType == DCT_UserDefine;
    }
    //@}

    // 基准点。
    //@{
    FITKGeoEnum::FITKDatumType FITKAbsGeoDatumPoint::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTPoint;
    }
    //@}

    // 基准线。
    //@{
    FITKGeoEnum::FITKDatumType FITKAbsGeoDatumLine::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTLine;
    }

    void FITKAbsGeoDatumLine::setFiniteLine(double* pos1, double* pos2)
    {
        // 保存线。
        for (int i = 0; i < 3; i++)
        {
            m_pos[i] = pos1[i];
            m_pos2[i] = pos2[i];
        }
    }

    void FITKAbsGeoDatumLine::getFiniteLine(double* pos1, double* pos2)
    {
        // 获取线。
        for (int i = 0; i < 3; i++)
        {
            pos1[i] = m_pos[i];
            pos2[i] = m_pos2[i];
        }
    }

    void FITKAbsGeoDatumLine::setInfiniteLine(double* pos, double* dir)
    {
        // 保存线。
        for (int i = 0; i < 3; i++)
        {
            m_pos[i] = pos[i];
            m_dir[i] = dir[i];
        }
    }

    void FITKAbsGeoDatumLine::getInfiniteLine(double* pos, double* dir)
    {
        // 获取线。
        for (int i = 0; i < 3; i++)
        {
            pos[i] = m_pos[i];
            dir[i] = m_dir[i];
        }
    }

    void FITKAbsGeoDatumLine::setPosition2(double* pos)
    {
        // 保存位置。
        if (pos)
        {
            m_pos2[0] = pos[0];
            m_pos2[1] = pos[1];
            m_pos2[2] = pos[2];
        }
    }

    void FITKAbsGeoDatumLine::getPosition2(double* pos)
    {
        if (pos)
        {
            pos[0] = m_pos2[0];
            pos[1] = m_pos2[1];
            pos[2] = m_pos2[2];
        }
    }

    void FITKAbsGeoDatumLine::getDirection(double* dir)
    {
        if (dir)
        {
            dir[0] = m_pos2[0] - m_pos[0];
            dir[1] = m_pos2[1] - m_pos[1];
            dir[2] = m_pos2[2] - m_pos[2];
        }
    }

    void FITKAbsGeoDatumLine::setIsInfinite(bool flag)
    {
        m_isInfinite = flag;
    }

    bool FITKAbsGeoDatumLine::getIsInfinite()
    {
        return m_isInfinite;
    }

    FITKGeoEnum::DatumAxisType FITKAbsGeoDatumLine::getAxisType()
    {
        return m_axisType;
    }

    void FITKAbsGeoDatumLine::setAxisType(FITKGeoEnum::DatumAxisType type)
    {
        m_axisType = type;
    }
    //@}

    FITKAbsGeoDatumPlane::~FITKAbsGeoDatumPlane()
    {
    }

    // 基准面。
    //@{
    FITKGeoEnum::FITKDatumType FITKAbsGeoDatumPlane::getDatumType()
    {
        return FITKGeoEnum::FITKDatumType::FDTPlane;
    }

    void FITKAbsGeoDatumPlane::setPlane(double* pos, double* normal, double* up)
    {
        // 设置全部信息。
        if (!pos || !normal || !up)
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            m_pos[i] = pos[i];
            m_nor[i] = normal[i];
            m_up[i] = up[i];
        }
    }

    void FITKAbsGeoDatumPlane::getPlane(double* pos, double* normal, double* up)
    {
        // 获取全部信息。
        if (!pos || !normal || !up)
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            pos[i] = m_pos[i];
            normal[i] = m_nor[i];
            up[i] = m_up[i];
        }
    }

    void FITKAbsGeoDatumPlane::setNormal(double* nor)
    {
        // 保存方向。
        if (nor)
        {
            m_nor[0] = nor[0];
            m_nor[1] = nor[1];
            m_nor[2] = nor[2];
        }
    }

    void FITKAbsGeoDatumPlane::getNormal(double* nor)
    {
        if (nor)
        {
            nor[0] = m_nor[0];
            nor[1] = m_nor[1];
            nor[2] = m_nor[2];
        }
    }

    void FITKAbsGeoDatumPlane::setUp(double* up)
    {
        // 保存方向。
        if (up)
        {
            m_up[0] = up[0];
            m_up[1] = up[1];
            m_up[2] = up[2];
        }
    }

    void FITKAbsGeoDatumPlane::getUp(double* up)
    {
        if (up)
        {
            up[0] = m_up[0];
            up[1] = m_up[1];
            up[2] = m_up[2];
        }
    }

    FITKGeoEnum::DatumPlnType FITKAbsGeoDatumPlane::getPlaneType()
    {
        return m_plnType;
    }

    void FITKAbsGeoDatumPlane::setPlaneType(FITKGeoEnum::DatumPlnType type)
    {
        m_plnType = type;
    }
    //@}

    // 系统默认基准元素管理器。
    //@{
    FITKAbsGeoDatumPlane* FITKPrincipalDatumList::getPrincipalPlane(FITKGeoEnum::DatumPlnType type)
    {
        FITKAbsGeoDatumPlane* princPln = nullptr;

        // 尝试获取基准面。
        int nDatums = this->getDataCount();
        for (int i = 0; i < nDatums; i++)
        {
            FITKAbsGeoDatumPlane* dPln = dynamic_cast<FITKAbsGeoDatumPlane*>(this->getDataByIndex(i));
            if (!dPln)
            {
                continue;
            }

            if (dPln->getPlaneType() == type)
            {
                princPln = dPln;
                break;
            }
        }

        if (princPln)
        {
            return princPln;
        }

        // 创建基准面。
        double pos3[3]{ 0., 0., 0. };
        double xDir3[3]{ 1., 0., 0. };
        double yDir3[3]{ 0., 1., 0. };
        double zDir3[3]{ 0., 0., 1. };

        switch (type)
        {
        case Interface::FITKGeoEnum::DPT_Principal_XOY:
        {
            // XOY.
            princPln = new FITKAbsGeoDatumPlane;
            princPln->setPlaneType(type);
            princPln->setDataObjectName("Plane XOY");
            princPln->setEditable(false);
            princPln->setPlane(pos3, zDir3, yDir3);
            princPln->update();
            break;
        }
        case Interface::FITKGeoEnum::DPT_Principal_YOZ:
        {
            // YOZ.
            princPln = new FITKAbsGeoDatumPlane;
            princPln->setPlaneType(type);
            princPln->setDataObjectName("Plane YOZ");
            princPln->setEditable(false);
            princPln->setPlane(pos3, xDir3, zDir3);
            princPln->update();
            break;
        }
        case Interface::FITKGeoEnum::DPT_Principal_ZOX:
        {
            // ZOX.
            princPln = new FITKAbsGeoDatumPlane;
            princPln->setPlaneType(type);
            princPln->setDataObjectName("Plane ZOX");
            princPln->setEditable(false);
            princPln->setPlane(pos3, yDir3, xDir3);
            princPln->update();
            break;
        }
        default:
            break;
        }

        return princPln;
    }

    FITKAbsGeoDatumLine* FITKPrincipalDatumList::getPrincipalAxis(FITKGeoEnum::DatumAxisType type)
    {
        FITKAbsGeoDatumLine* princLine = nullptr;

        // 尝试获取基准线。
        int nDatums = this->getDataCount();
        for (int i = 0; i < nDatums; i++)
        {
            FITKAbsGeoDatumLine* dLine = dynamic_cast<FITKAbsGeoDatumLine*>(this->getDataByIndex(i));
            if (!dLine)
            {
                continue;
            }

            if (dLine->getAxisType() == type)
            {
                princLine = dLine;
                break;
            }
        }

        if (princLine)
        {
            return princLine;
        }

        // 创建基准线。
        double pos3[3]{ 0., 0., 0. };
        double xDir3[3]{ 1., 0., 0. };
        double yDir3[3]{ 0., 1., 0. };
        double zDir3[3]{ 0., 0., 1. };

        switch (type)
        {
        case Interface::FITKGeoEnum::DAT_Principal_X:
        {
            // X.
            princLine = new FITKAbsGeoDatumLine;
            princLine->setAxisType(type);
            princLine->setDataObjectName("Axis X");
            princLine->setEditable(false);
            princLine->setPosition(pos3);
            princLine->setPosition2(xDir3);
            princLine->update();
            break;
        }
        case Interface::FITKGeoEnum::DAT_Principal_Y:
        {
            // Y.
            princLine = new FITKAbsGeoDatumLine;
            princLine->setAxisType(type);
            princLine->setDataObjectName("Axis Y");
            princLine->setEditable(false);
            princLine->setPosition(pos3);
            princLine->setPosition2(yDir3);
            princLine->update();
            break;
        }
        case Interface::FITKGeoEnum::DAT_Principal_Z:
        {
            // Z.
            princLine = new FITKAbsGeoDatumLine;
            princLine->setAxisType(type);
            princLine->setDataObjectName("Axis Z");
            princLine->setEditable(false);
            princLine->setPosition(pos3);
            princLine->setPosition2(zDir3);
            princLine->update();
            break;
        }
        default:
            break;
        }

        return princLine;
    }
    //@}

    // 基准元素管理器。
    //@{
    FITKDatumList::FITKDatumList()
    {
        // 初始化系统默认基准元素管理器。
        m_principalDatumMgr = new FITKPrincipalDatumList;
    }

    FITKDatumList::~FITKDatumList()
    {
        // 删除系统默认基准元素管理器。
        if (m_principalDatumMgr)
        {
            delete m_principalDatumMgr;
            m_principalDatumMgr = nullptr;
        }
    }

    Interface::FITKAbsGeoDatum* FITKDatumList::getDataByID(const int id)
    {
        // 查询系统默认基准元素。
        if (m_principalDatumMgr)
        {
            FITKAbsGeoDatum* datum = m_principalDatumMgr->getDataByID(id);
            if (datum)
            {
                return datum;
            }
        }

        // 从管理器获取。
        return Core::FITKAbstractDataManager<Interface::FITKAbsGeoDatum>::getDataByID(id);
    }

    FITKAbsGeoDatumPlane* FITKDatumList::getPrincipalPlane(FITKGeoEnum::DatumPlnType type)
    {
        if (!m_principalDatumMgr)
        {
            return nullptr;
        }

        return m_principalDatumMgr->getPrincipalPlane(type);
    }

    FITKAbsGeoDatumLine* FITKDatumList::getPrincipalAxis(FITKGeoEnum::DatumAxisType type)
    {
        if (!m_principalDatumMgr)
        {
            return nullptr;
        }

        return m_principalDatumMgr->getPrincipalAxis(type);
    }
    //@}
}