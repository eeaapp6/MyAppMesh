/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCGNSWriter.h"
#include "FITKCGNSIOInterface.h"
#include "FITKMeshDataProcessToCGNSData.h"

#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKStructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"

#include <QDateTime>

namespace IO
{
    FITKCGNSWriter::FITKCGNSWriter(FITKCGNSIOInterface* io) :
        _compInterface(io)
    {
        _tool = new CGNS::FITKMeshDataProcessToCGNSData();
    }

    FITKCGNSWriter::~FITKCGNSWriter()
    {
        if (_tool)
            delete _tool;
    }

    void FITKCGNSWriter::run()
    {
        if (!_resultMark) return;
        *_resultMark = false;
        this->consoleMessage(1, tr("Writing CGNS File : %1").arg(_fileName));
        //打开cgns文件
        if (!this->openCGNSFile())
        {
            this->consoleMessage(3, tr("Writing CGNS Error ! Open file err %1").arg(_fileName));
            this->closeFile();
            return;
        }
        //读取cgns数据
        *_resultMark = write();
        //信息打印
        if (*_resultMark)
            this->consoleMessage(1, tr("Writing CGNS File Complete : %1").arg(_fileName));
        else
            this->consoleMessage(3, tr("Writing CGNS File Err : %1").arg(_fileName));
        this->closeFile();
    }

    void FITKCGNSWriter::consoleMessage(int level, const QString& str)
    {
        //判断消息等级
        switch (level)
        {
        case 1:AppFrame::FITKMessageNormal(str); break;
        case 2: AppFrame::FITKMessageWarning(str); break;
        case 3:AppFrame::FITKMessageError(str); break;
        default: AppFrame::FITKMessageError(str); break;
        }
    }

    bool FITKCGNSWriter::write()
    {
        if (!_compInterface) return false;
        //读取CGNS信息
        this->writeCGNSInfo();
        // 写出 CGNS 基节点
        bool ok = this->writeBase("Base");
        return ok;
    }

    bool FITKCGNSWriter::writeBase(const char * base_name)
    {
        if (!_compInterface) return false;
        if (CG_OK != cg_base_write(m_index_file, base_name, 3, 3, &m_index_base))return false;
        //写出描述
        cg_goto(m_index_file, m_index_base, NULL);
        QString descr_text = QString("Exported from FastCAE.  %1").arg(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss"));
        if (CG_OK != cg_descriptor_write("Information", descr_text.toUtf8().constData()))
            return false;
        //写出体族
        {
            if (CG_OK != cg_family_write(m_index_file, m_index_base, "Unspecified", &m_index_family))
                return false;
            //写出族的描述
            cg_goto(m_index_file, m_index_base, "Family_t", m_index_family, "end");
            if (CG_OK != cg_descriptor_write("FamBC_TypeId", "0"))
                return false;
            if (CG_OK != cg_descriptor_write("FamBC_TypeName", "Unspecified"))
                return false;
            if (CG_OK != cg_descriptor_write("FamBC_UserId", "2147483647"))
                return false;
            if (CG_OK != cg_descriptor_write("FamBC_UserName", "Unspecified"))
                return false;
        }
        //获取区域数量
        int meshElement_max_dim = 0;
        int nzones = _compInterface->getCGNSDataIO()._mesh.size();
        for (int iZone = 0; iZone < nzones; ++iZone)
        {
            ++m_current_index_mesh;
            bool processOK = _tool->init(_compInterface->getCGNSDataIO()._mesh[m_current_index_mesh], _compInterface->getCGNSDataIO()._componentManager[m_current_index_mesh]);
            if (!processOK) continue;
            if (meshElement_max_dim < _tool->m_element_max_dim)
                meshElement_max_dim = _tool->m_element_max_dim;
            //获取网格类型
            CGNS_ENUMT(ZoneType_t) zoneType = _tool->m_mesh_type;
            bool ok = false;
            if (zoneType == CGNS_ENUMV(Unstructured))
            {
                ok = this->writeBase_ZoneUnstructured();
            }
            else if (zoneType == CGNS_ENUMV(Structured))
            {
                ok = this->writeBase_ZoneStructured();
            }
            if (!ok)
                return false;
        }
        return true;
    }

    bool FITKCGNSWriter::writeBase_ZoneStructured()
    {
        return false;
    }

    bool FITKCGNSWriter::writeBase_ZoneUnstructured()
    {
        if (!_tool || _tool->m_element_section.isEmpty()) return false;
        //获取Zone信息
        QString zone_name = _tool->m_name_zone;
        if (zone_name.isEmpty())
            zone_name = QString("Custom_Zone_%1").arg(m_current_index_mesh + 1);
        int nodeCount = _tool->m_node_coord.node_coordX.size();
        int elementCount = _tool->m_element_section[0].pnts[1];
        const cgsize_t zone_ijk[3] = { nodeCount, elementCount, 0 };
        if (CG_OK != cg_zone_write(m_index_file, m_index_base, zone_name.toUtf8().constData(), zone_ijk, CGNS_ENUMV(Unstructured), &m_index_zone))
            return false;
        if (!this->writeGridCoordinate()) return false;
        if (!this->writeBase_ZoneUnstructured_Section()) return false;
        if (!this->writeBase_ZoneUnstructured_BC()) return false;
        return true;
    }

    bool FITKCGNSWriter::writeGridCoordinate()
    {
        if (!_tool) return false;
        int index_coor = 0;
        if (CG_OK != cg_coord_write(m_index_file, m_index_base, m_index_zone, CGNS_ENUMV(RealDouble), "CoordinateX", _tool->m_node_coord.node_coordX.constData(), &index_coor)) return false;
        if (CG_OK != cg_coord_write(m_index_file, m_index_base, m_index_zone, CGNS_ENUMV(RealDouble), "CoordinateY", _tool->m_node_coord.node_coordY.constData(), &index_coor)) return false;
        if (CG_OK != cg_coord_write(m_index_file, m_index_base, m_index_zone, CGNS_ENUMV(RealDouble), "CoordinateZ", _tool->m_node_coord.node_coordZ.constData(), &index_coor)) return false;

        return true;
    }

    bool FITKCGNSWriter::writeBase_ZoneUnstructured_Section()
    {
        if (!_tool) return false;
        for (int i = 0; i < _tool->m_element_section.size(); ++i)
        {
            int index_section = 0;
            if (CG_OK != cg_section_write(m_index_file, m_index_base, m_index_zone, _tool->m_element_section[i].name_section.toUtf8().constData(), _tool->m_element_section[i].element_type,
                _tool->m_element_section[i].pnts[0], _tool->m_element_section[i].pnts[1], 0, _tool->m_element_section[i].elements.constData(), &index_section))
                return false;
        }

        return true;
    }

    bool FITKCGNSWriter::writeBase_ZoneUnstructured_BC()
    {
        if (!_tool) return false;
        //写出边界shuju
        for (int i = 0; i < _tool->m_set_BC.size(); ++i)
        {
            int index_BC = 0;
            CGNS_ENUMT(BCType_t) bcType = CGNS_ENUMV(FamilySpecified);
            if (CG_OK != cg_boco_write(m_index_file, m_index_base, m_index_zone, _tool->m_set_BC[i].name_bc.toUtf8().constData(), bcType, CGNS_ENUMV(PointRange),
                _tool->m_set_BC[i].pnts.size(), _tool->m_set_BC[i].pnts.constData(), &index_BC))
                return false;
            if (CG_OK != cg_boco_gridlocation_write(m_index_file, m_index_base, m_index_zone, index_BC, _tool->m_set_BC[i].location))
                return false;
            if (bcType == CGNS_ENUMV(FamilySpecified))
            {
                cg_goto(m_index_file, m_index_base, "Zone_t", m_index_zone, "ZoneBC_t", 1, "BC_t", index_BC, "end");
                cg_famname_write(_tool->m_set_BC[i].name_bc.toUtf8().constData());
                //写出边界族
                if (!this->writeFamily_BC(CGNS_ENUMV(BCWall), _tool->m_set_BC[i].name_bc, i + 1, index_BC))
                    return false;
            }
        }
        
        //写出体单元的族数据
        int max_dim = _tool->m_element_max_dim;
        if (max_dim == 3)
        {
            cg_goto(m_index_file, m_index_base, "Zone_t", m_index_zone, "end");
            cg_famname_write("Unspecified");
        }

        return true;
    }

    bool FITKCGNSWriter::writeFamily_BC(int type, QString bc_name, int bcID, int& index_bc)
    {
        QString type_name = this->getBCTypeName(type);
        if (type_name.isEmpty()) return false;
        if (CG_OK != cg_family_write(m_index_file, m_index_base, bc_name.toUtf8().constData(), &m_index_family)) return false;
        //写出族边界类型
        if (CG_OK != cg_fambc_write(m_index_file, m_index_base, m_index_family, "FamBC", CGNS_ENUMT(BCType_t)(type), &index_bc)) return false;
        //写出族的描述
        cg_goto(m_index_file, m_index_base, "Family_t", m_index_family, "end");
        if (CG_OK != cg_descriptor_write("Fam_Descr_Name", bc_name.toUtf8().constData()))
            return false;
        if (CG_OK != cg_descriptor_write("FamBC_TypeId", QString::number(type).toUtf8().constData()))
            return false;
        if (CG_OK != cg_descriptor_write("FamBC_TypeName", type_name.toUtf8().constData()))
            return false;
        if (CG_OK != cg_descriptor_write("FamBC_UserId", QString::number(bcID).toUtf8().constData()))
            return false;
        if (CG_OK != cg_descriptor_write("FamBC_UserName", bc_name.toUtf8().constData()))
            return false;
        return true;
    }

    QString FITKCGNSWriter::getBCTypeName(int type)
    {
        CGNS_ENUMT(BCType_t) bcType = CGNS_ENUMT(BCType_t)(type);
        switch (bcType)
        {
        case CGNS_ENUMV(BCOutflow):return "Outflow";
        case CGNS_ENUMV(BCSymmetryPlane):return "SymmetryPlane";
        case CGNS_ENUMV(BCInflow):return "Inflow";
        case CGNS_ENUMV(BCWall):return "Wall";
        default:
            break;
        }
        return QString();
    }

    bool FITKCGNSWriter::writeCGNSInfo()
    {
        //获取 文件的CGNS 版本
        float fileVersion = -1;
        if (CG_OK != cg_version(m_index_file, &fileVersion))return false;
        //获取 CGNS文件精度
        int precision = -1;
        if (CG_OK != cg_precision(m_index_file, &precision))return false;
        //设置不启用压缩算法
        if (CG_OK != cg_set_compress(0)) return false;

        this->consoleMessage(1, tr("Writing CGNS File Version : %1, Precision : %2").arg(fileVersion).arg(precision));
        return true;
    }
}

