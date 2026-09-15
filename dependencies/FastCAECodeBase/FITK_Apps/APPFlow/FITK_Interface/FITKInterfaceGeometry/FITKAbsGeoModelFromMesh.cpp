/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelFromMesh.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementTri.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementQuad.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Interface
{
    FITKAbsGeoModelFromMesh::~FITKAbsGeoModelFromMesh() = default;

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelFromMesh::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTModelFromMesh;
    }
    void FITKAbsGeoModelFromMesh::setMesh(const QList<Core::FITKNode *> &meshNodes, const QList<FITKAbstractElement *> &meshElements)
    {
        //转化存储节点坐标
        for (int i = 0; i < meshNodes.size(); ++i)
        {
            auto node = meshNodes.at(i);
            MeshNodeCoordinates coordinates;
            coordinates.x = node->x();
            coordinates.y = node->y();
            coordinates.z = node->z();
            m_meshNodeCoordinates.insert(node->getNodeID(), coordinates);
        }

        //转化存储三角形
        for (int i = 0; i < meshElements.size(); ++i)
        {
            auto element = meshElements.at(i);
            if(element == nullptr)
                continue;
            
            // 只处理三角形和四边形
            switch (element->getEleType())
            {
            case FITKModelEnum::FITKEleType::Tri3:
            {
                auto tri = dynamic_cast<FITKElementTri3*>(element);
                MeshTri triangle;
                triangle.node1 = tri->getNodeID(0);
                triangle.node2 = tri->getNodeID(1);
                triangle.node3 = tri->getNodeID(2);
                m_meshTriangles.append(triangle);
                this->calTriNormal(triangle);
                break;
            }
            case FITKModelEnum::FITKEleType::Quad4:
            {
                // 处理四边形时，将其拆分为两个三角形
                auto quad = dynamic_cast<FITKElementQuad4*>(element);
                MeshTri triangle1;
                triangle1.node1 = quad->getNodeID(0);
                triangle1.node2 = quad->getNodeID(1);
                triangle1.node3 = quad->getNodeID(2);
                m_meshTriangles.append(triangle1);
                this->calTriNormal(triangle1);

                MeshTri triangle2;
                triangle2.node1 = quad->getNodeID(0);
                triangle2.node2 = quad->getNodeID(2);
                triangle2.node3 = quad->getNodeID(3);
                m_meshTriangles.append(triangle2);
                this->calTriNormal(triangle2);
                break;
            }
            default:
                break;
            }
        }
    }

    bool FITKAbsGeoModelFromMesh::writeSTL(const QString &fileName) const
    {
        //打开文件
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Failed to open file for writing: " << fileName;
            return false;
        }
        QTextStream out(&file);


        //写入STL头部
        out << "solid STLModel" << endl;
        //写入三角形数据
        for (const MeshTri &tri : m_meshTriangles)
        {
            //坐标信息
            MeshNodeCoordinates coord1 = m_meshNodeCoordinates.value(tri.node1);
            MeshNodeCoordinates coord2 = m_meshNodeCoordinates.value(tri.node2);
            MeshNodeCoordinates coord3 = m_meshNodeCoordinates.value(tri.node3);

            out << "  facet normal " << tri.normal[0] << " " << tri.normal[1] << " " << tri.normal[2] << endl;
            out << "    outer loop"<<endl;
            out << "      vertex " << coord1.x << " " << coord1.y << " " << coord1.z << endl;
            out << "      vertex " << coord2.x << " " << coord2.y << " " << coord2.z << endl;
            out << "      vertex " << coord3.x << " " << coord3.y << " " << coord3.z << endl;
            out << "    endloop" << endl;
            out << "  endfacet" << endl;
        }
        //写入STL尾部
        out << "endsolid STLModel" << endl;
        //关闭文件
        file.close();
        return true;

    }

    const QHash<int, FITKAbsGeoModelFromMesh::MeshNodeCoordinates> &FITKAbsGeoModelFromMesh::getMeshNodeCoordinates() const
    {
        return m_meshNodeCoordinates;
    }
    const QList<FITKAbsGeoModelFromMesh::MeshTri> &FITKAbsGeoModelFromMesh::getMeshTriangles() const
    {
        return m_meshTriangles;
    }

    void FITKAbsGeoModelFromMesh::calTriNormal(MeshTri &tri)
    {
        MeshNodeCoordinates node1 = m_meshNodeCoordinates.value(tri.node1);
        MeshNodeCoordinates node2 = m_meshNodeCoordinates.value(tri.node2);
        MeshNodeCoordinates node3 = m_meshNodeCoordinates.value(tri.node3);

        Core::FITKVec3D v1(node2.x - node1.x, node2.y - node1.y, node2.z - node1.z);
        Core::FITKVec3D v2(node3.x - node1.x, node3.y - node1.y, node3.z - node1.z);

        Core::FITKVec3D normal = Core::CrossProduct(v2,v1);
        normal.normalize();

        tri.normal[0] = normal.x();
        tri.normal[1] = normal.y();
        tri.normal[2] = normal.z();
    }
}