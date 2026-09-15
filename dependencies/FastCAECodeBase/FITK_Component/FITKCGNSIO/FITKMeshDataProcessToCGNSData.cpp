/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

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

#include <QtAlgorithms>
#include <QVector3D>
#include <QFile>

namespace CGNS
{
    bool FITKMeshDataProcessToCGNSData::init(Interface::FITKAbstractMesh * mesh, QList<Interface::FITKModelSet*> componentMgr)
    {
        this->dataClear();
        //初始化数据
        _mesh = mesh;
        _componentMgr = componentMgr;
        if (!mesh) return false;
        //数据转化
        m_name_zone = _mesh->getDataObjectName();
        bool processOK = false;
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(mesh);
        Interface::FITKStructuredMesh* structuredMesh = dynamic_cast<Interface::FITKStructuredMesh*>(mesh);
        if (unstructuredMesh)
            processOK = this->processUnstructuredData();
        else if (structuredMesh)
            processOK = this->processStructuredData();
        return processOK;
    }

    bool FITKMeshDataProcessToCGNSData::processUnstructuredData()
    {
        m_mesh_type = CGNS_ENUMV(Unstructured);
        //非结构网格数据处理
        this->processUnstructuredMesh_Node();
        this->processUnstructuredMesh_Element();
        this->processUnstructuredMesh_BC();
        this->processUnstructuredMesh_NoBCElement_Section();
        this->processUnstructuredMesh_Element_BC();
        if (m_node_coord.node_coordX.isEmpty() || m_element_section.isEmpty())
            return false;
        return true;
    }

    bool FITKMeshDataProcessToCGNSData::processStructuredData()
    {
        m_mesh_type = CGNS_ENUMV(Structured);
        return false;
    }

    bool FITKMeshDataProcessToCGNSData::processUnstructuredMesh_Node()
    {
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_mesh);
        if (!unstructuredMesh) return false;
        //获取节点数据
        int id = 0;
        int nodeCount = unstructuredMesh->getNodeCount();
        if (nodeCount == 0) return false;
        //数据提取
        QList<double> coordX;
        QList<double> coordY;
        QList<double> coordZ;
        for (int i = 0; i < nodeCount; ++i)
        {
            Core::FITKNode* node = unstructuredMesh->getNodeAt(i);
            if (!node) return false;
            int nodeID = node->getNodeID();
            coordX.append(node->x());
            coordY.append(node->y());
            coordZ.append(node->z());
            //添加到映射表
            ++id;
            _nodeMapping.insert(nodeID, id);
        }
        m_node_coord.node_coordX = coordX.toVector();
        m_node_coord.node_coordY = coordY.toVector();
        m_node_coord.node_coordZ = coordZ.toVector();
        return true;
    }

    bool FITKMeshDataProcessToCGNSData::processUnstructuredMesh_Element()
    {
        Interface::FITKUnstructuredMesh* unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_mesh);
        if (!unstructuredMesh) return false;
        //遍历单元数据
        int elementCount = unstructuredMesh->getElementCount();
        if (elementCount == 0) return false;
        for (int i = 0; i < elementCount; ++i)
        {
            Interface::FITKAbstractElement* element = unstructuredMesh->getElementAt(i);
            if (!element) continue;
            //获取单元数据
            Interface::FITKModelEnum::FITKEleType elementType = element->getEleType();
            int elementID = element->getEleID();
            QList<int> nodeIDs = element->getAllNodes();
            QList<cgsize_t> nodeNewIDs;
            for (int id : nodeIDs)
            {
                if (!_nodeMapping.contains(id))break;
                nodeNewIDs.append(_nodeMapping[id]);
            }
            if (nodeIDs.size() != nodeNewIDs.size())continue;
            //单元类型
            if (!_eleType_eleOldIDs_Mapping.contains(elementType))
            {
                int dim = element->getElementDim();
                if (m_element_max_dim < dim) m_element_max_dim = dim;
                if (!_eleDim_eleType_Mapping.contains(dim))_eleDim_eleType_Mapping.insert(dim, QList<Interface::FITKModelEnum::FITKEleType>());
                _eleDim_eleType_Mapping[dim].append(elementType);
                _eleType_eleOldIDs_Mapping.insert(elementType, QSet<int>());
            }
            _eleType_eleOldIDs_Mapping[elementType].insert(elementID);
            _eleOldID_nodeNewIDs_Mapping.insert(elementID, nodeNewIDs);
        }
        return true;
    }

    bool FITKMeshDataProcessToCGNSData::processUnstructuredMesh_BC()
    {
        int count = _componentMgr.size();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKModelSet* set = _componentMgr[i];
            if (!set) continue;
            QString name = set->getDataObjectName();
            if (name.isEmpty()) name = QString("defset_%1").arg(i + 1);
            QList<int> ids = set->getAbsoluteMember();
            _bcNameList.append(name);
            _bcName_eleOIldIDs_Mapping.insert(name, ids.toSet());
        }
        return true;
    }

    bool FITKMeshDataProcessToCGNSData::processUnstructuredMesh_NoBCElement_Section()
    {
        //section创建
        auto sectionCreate = [&](QList<int> elementIDs, CGNS_ENUMT(ElementType_t) elementType, QString section_name)
        {

            int section_index = m_element_section.size();
            //添加单元数据Section
            m_element_section.append(ElementCGNSSectionData());
            m_element_section[section_index].element_type = elementType;
            m_element_section[section_index].name_section = section_name;
            QList<cgsize_t> element_allnodes;
            for (int eID : elementIDs)
            {
                QList<cgsize_t> nodeNewIDs = _eleOldID_nodeNewIDs_Mapping[eID];
                element_allnodes.append(nodeNewIDs);
            }
            m_element_section[section_index].elements = element_allnodes.toVector();
            ++_element_max_ID;
            m_element_section[section_index].pnts.push_back(_element_max_ID);
            _element_max_ID = _element_max_ID + elementIDs.size() - 1;
            m_element_section[section_index].pnts.push_back(_element_max_ID);
        };
        //有边界的单元ID
        QSet<int> bc_element_ids;
        for (QHash<QString, QSet<int>>::iterator iter = _bcName_eleOIldIDs_Mapping.begin(); iter != _bcName_eleOIldIDs_Mapping.end(); ++iter)
            bc_element_ids = bc_element_ids.unite(iter.value());
        //3D数据
        QList<Interface::FITKModelEnum::FITKEleType> typeList;
        if (_eleDim_eleType_Mapping.contains(3))
            typeList = _eleDim_eleType_Mapping[3];
        for (Interface::FITKModelEnum::FITKEleType type : typeList)
        {
            QSet<int> elementIDs = _eleType_eleOldIDs_Mapping[type];
            if (elementIDs.isEmpty()) continue;
            QList<int> eIDs = elementIDs.toList();
            qSort(eIDs.begin(), eIDs.end());
            QString name;
            CGNS_ENUMT(ElementType_t) eType = this->getCGNSElementType(type, name);
            //添加单元数据Section
            sectionCreate(eIDs, eType, name);
        }
        //2D数据
        typeList.clear();
        if (_eleDim_eleType_Mapping.contains(2))
            typeList = _eleDim_eleType_Mapping[2];
        for (Interface::FITKModelEnum::FITKEleType type : typeList)
        {
            QSet<int> elementIDs = _eleType_eleOldIDs_Mapping[type];
            QSet<int> subElementIDs = elementIDs.subtract(bc_element_ids);
            if (subElementIDs.isEmpty()) continue;
            QList<int> eIDs = elementIDs.toList();
            qSort(eIDs.begin(), eIDs.end());
            QString name;
            CGNS_ENUMT(ElementType_t) eType = this->getCGNSElementType(type, name);
            //添加单元数据Section
            sectionCreate(eIDs, eType, QString("%1Elements").arg(name));
        }
        //1D数据
        typeList.clear();
        if (_eleDim_eleType_Mapping.contains(1))
            typeList = _eleDim_eleType_Mapping[1];
        for (Interface::FITKModelEnum::FITKEleType type : typeList)
        {
            QSet<int> elementIDs = _eleType_eleOldIDs_Mapping[type];
            QSet<int> subElementIDs = elementIDs.subtract(bc_element_ids);
            if (subElementIDs.isEmpty()) continue;
            QList<int> eIDs = elementIDs.toList();
            qSort(eIDs.begin(), eIDs.end());
            QString name;
            CGNS_ENUMT(ElementType_t) eType = this->getCGNSElementType(type, name);
            //添加单元数据Section
            sectionCreate(eIDs, eType, QString("%1Elements").arg(name));
        }
        return true;
    }

    bool FITKMeshDataProcessToCGNSData::processUnstructuredMesh_Element_BC()
    {
        //section创建
        auto sectionCreate = [&](QList<int> elementIDs, CGNS_ENUMT(ElementType_t) elementType, QString section_name)
        {
            int section_index = m_element_section.size();
            //添加单元数据Section
            m_element_section.append(ElementCGNSSectionData());
            m_element_section[section_index].element_type = elementType;
            m_element_section[section_index].name_section = section_name;
            QList<cgsize_t> element_allnodes;
            for (int eID : elementIDs)
            {
                QList<cgsize_t> nodeNewIDs = _eleOldID_nodeNewIDs_Mapping[eID];
                element_allnodes.append(nodeNewIDs);
            }
            m_element_section[section_index].elements = element_allnodes.toVector();
            ++_element_max_ID;
            m_element_section[section_index].pnts.push_back(_element_max_ID);
            _element_max_ID = _element_max_ID + elementIDs.size() - 1;
            m_element_section[section_index].pnts.push_back(_element_max_ID);
        };
        //边界
        for (int i = 0; i < _bcNameList.size(); ++i)
        {
            QString bcName = _bcNameList[i];
            QSet<int> setNodeIDs = _bcName_eleOIldIDs_Mapping[bcName];
            int start_section_index = m_element_section.size() - 1;
            int start = _element_max_ID;
            //2D数据
            QList<Interface::FITKModelEnum::FITKEleType> typeList;
            if (_eleDim_eleType_Mapping.contains(2))
                typeList = _eleDim_eleType_Mapping[2];
            for (Interface::FITKModelEnum::FITKEleType type : typeList)
            {
                QSet<int> elementIDs = _eleType_eleOldIDs_Mapping[type];
                QSet<int> subElementIDs = elementIDs.intersect(setNodeIDs);
                if (subElementIDs.isEmpty()) continue;
                QList<int> eIDs = elementIDs.toList();
                qSort(eIDs.begin(), eIDs.end());
                QString name;
                CGNS_ENUMT(ElementType_t) eType = this->getCGNSElementType(type, name);
                //添加单元数据Section
                sectionCreate(eIDs, eType, QString("%1_%2").arg(name).arg(bcName));
            }
            //1D数据
            typeList.clear();
            if (_eleDim_eleType_Mapping.contains(1))
                typeList = _eleDim_eleType_Mapping[1];
            for (Interface::FITKModelEnum::FITKEleType type : typeList)
            {
                QSet<int> elementIDs = _eleType_eleOldIDs_Mapping[type];
                QSet<int> subElementIDs = elementIDs.intersect(setNodeIDs);
                if (subElementIDs.isEmpty()) continue;
                QList<int> eIDs = elementIDs.toList();
                qSort(eIDs.begin(), eIDs.end());
                QString name;
                CGNS_ENUMT(ElementType_t) eType = this->getCGNSElementType(type, name);
                //添加单元数据Section
                sectionCreate(eIDs, eType, QString("%1_%2").arg(name).arg(bcName));
            }
            int end_section_index = m_element_section.size() - 1;
            int end = _element_max_ID;
            if (start == end) continue;
            if (end_section_index - 1 == start_section_index)
            {
                m_element_section[end_section_index].name_section = bcName;
            }
            //创建边界
            int bc_index = m_set_BC.size();
            m_set_BC.append(SetCGNSBCData());
            m_set_BC[bc_index].name_bc = bcName;
            m_set_BC[bc_index].pnts.push_back(start + 1);
            m_set_BC[bc_index].pnts.push_back(end);
            m_set_BC[bc_index].location = CGNS_ENUMV(FaceCenter);
        }

        return true;
    }

    CGNS_ENUMT(ElementType_t) FITKMeshDataProcessToCGNSData::getCGNSElementType(int type, QString & eleName)
    {
        Interface::FITKModelEnum::FITKEleType eleType = Interface::FITKModelEnum::FITKEleType(type);
        switch (eleType)
        {
        case Interface::FITKModelEnum::EleNone:
        {
            return CGNS_ENUMT(ElementTypeNull);
        }
        case Interface::FITKModelEnum::Line2:
        {
            eleName = "bar";
            return CGNS_ENUMT(BAR_2);
        }
        case Interface::FITKModelEnum::Line3:
        {
            eleName = "bar_3";
            return CGNS_ENUMT(BAR_3);
        }
        case Interface::FITKModelEnum::Tri3:
        {
            eleName = "tri";
            return CGNS_ENUMT(TRI_3);
        }
        case Interface::FITKModelEnum::Tri6:
        {
            eleName = "tri_6";
            return CGNS_ENUMT(TRI_6);
        }
        case Interface::FITKModelEnum::Quad4:
        {
            eleName = "quad";
            return CGNS_ENUMT(QUAD_4);
        }
        case Interface::FITKModelEnum::Quad8:
        {
            eleName = "quad_8";
            return CGNS_ENUMT(QUAD_8);
        }
        case Interface::FITKModelEnum::Tet4:
        {
            eleName = "TetElements";
            return CGNS_ENUMT(TETRA_4);
        }
        case Interface::FITKModelEnum::Tet10:
        {
            eleName = "Tet_10Elements";
            return CGNS_ENUMT(TETRA_10);
        }
        case Interface::FITKModelEnum::Wedge6:
        {
            eleName = "PrismElements";
            return CGNS_ENUMT(PENTA_6);
        }
        case Interface::FITKModelEnum::Hex8:
        {
            eleName = "HexElements";
            return CGNS_ENUMT(HEXA_8);
        }
        case Interface::FITKModelEnum::Hex20:
        {
            eleName = "Hex_20Elements";
            return CGNS_ENUMT(HEXA_20);
        }
        default:
            break;
        }
        return CGNS_ENUMT(ElementType_t)();
    }

    bool FITKMeshDataProcessToCGNSData::calCellVolume(QString filePath)
    {
        bool result = true;
        int sectionCount = m_element_section.size();
        QStringList infoStringList;
        for (int i = 0; i < sectionCount; ++i)
        {
            if (m_element_section[i].element_type != CGNS_ENUMV(TETRA_4)) continue;
            int minID = m_element_section[i].pnts[0];
            int elementCount = m_element_section[i].pnts[1] - m_element_section[i].pnts[0] + 1;
            int positiveCount = 0, negativeCount = 0;
            for (int j = 0; j < elementCount; ++j)
            {
                int id1 = m_element_section[i].elements[j * 4 + 0];
                int id2 = m_element_section[i].elements[j * 4 + 1];
                int id3 = m_element_section[i].elements[j * 4 + 2];
                int id4 = m_element_section[i].elements[j * 4 + 3];
                QVector3D a(m_node_coord.node_coordX[id1 - 1], m_node_coord.node_coordY[id1 - 1], m_node_coord.node_coordZ[id1 - 1]);
                QVector3D b(m_node_coord.node_coordX[id2 - 1], m_node_coord.node_coordY[id2 - 1], m_node_coord.node_coordZ[id2 - 1]);
                QVector3D c(m_node_coord.node_coordX[id3 - 1], m_node_coord.node_coordY[id3 - 1], m_node_coord.node_coordZ[id3 - 1]);
                QVector3D p(m_node_coord.node_coordX[id4 - 1], m_node_coord.node_coordY[id4 - 1], m_node_coord.node_coordZ[id4 - 1]);
                QString aStr = QString("point1(%1 %2 %3)").arg(a.x()).arg(a.y()).arg(a.z());
                QString bStr = QString("point2(%1 %2 %3)").arg(b.x()).arg(b.y()).arg(b.z());
                QString cStr = QString("point3(%1 %2 %3)").arg(c.x()).arg(c.y()).arg(c.z());
                QString pStr = QString("point4(%1 %2 %3)").arg(p.x()).arg(p.y()).arg(p.z());
                // 计算从顶点p到三个底面点的向量
                QVector3D vecA = a - p;
                QVector3D vecB = b - p;
                QVector3D vecC = c - p;
                // 计算向量b和c的叉积
                QVector3D crossBC = QVector3D::crossProduct(vecB, vecC);
                // 计算向量a与叉积结果的点积
                double dotProduct = QVector3D::dotProduct(vecA, crossBC);
                // 体积 = 1/6 * |点积结果|，保留正负号则不取绝对值
                double volume = dotProduct / 6.0;
                infoStringList << QString("Element %1 : Volume %2").arg(minID + j).arg(volume);
                infoStringList << QString("            coord : %1 %2 %3 %4").arg(aStr).arg(bStr).arg(cStr).arg(pStr);
                if (volume > 0)
                    ++positiveCount;
                else
                    ++negativeCount;
            }
            if (positiveCount > 0 && negativeCount > 0)
            {
                result = false;
                infoStringList << QString("Check element err !!!");
            }
        }
        QFile file(filePath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        for (QString str : infoStringList)
        {
            file.write(str.toUtf8());
            file.write("\n");
        }
        file.close();
        return result;
    }

    void FITKMeshDataProcessToCGNSData::dataClear()
    {
        //数据清理
        m_mesh_type = CGNS_ENUMV(ZoneTypeNull);
        m_name_zone.clear();
        m_element_max_dim = 0;
        m_node_coord = NodeCGNSCoordData();
        m_element_section.clear();
        m_set_BC.clear();
        _mesh = nullptr;
        _componentMgr.clear();
        _nodeMapping.clear();
        _eleType_eleOldIDs_Mapping.clear();
        _eleDim_eleType_Mapping.clear();
        _eleOldID_nodeNewIDs_Mapping.clear();
        _bcNameList.clear();
        _bcName_eleOIldIDs_Mapping.clear();
        _element_max_ID = 0;
    }
}

