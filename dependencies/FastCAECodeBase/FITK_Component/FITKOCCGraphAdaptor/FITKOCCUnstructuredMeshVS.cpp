/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCUnstructuredMeshVS.h"

// OCC
#include <Standard.hxx>
#include <Standard_Type.hxx>
#include <Standard_Real.hxx>
#include <Standard_Address.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Integer.hxx>
#include <TColStd_PackedMapOfInteger.hxx>

// Data
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"

// Regist
IMPLEMENT_STANDARD_RTTIEXT(FITKOCCUnstructuredMeshVS, MeshVS_DataSource);

FITKOCCUnstructuredMeshVS::FITKOCCUnstructuredMeshVS(Interface::FITKUnstructuredMesh* meshData)
{
    // Check the mesh data.
    if (!meshData)
    {
        return;
    }

    // Generate node id map.
    //@{
    int nPts = meshData->getNodeCount();
    for (int i = 1; i <= nPts; i++)
    {
        m_nodeIds.Add(i);
    }
    //@}

    // Generate element id map.
    //@{
    int nElemnts = meshData->getElementCount();
    for (int i = 1; i <= nElemnts; i++)
    {
        m_elementIds.Add(i);
    }
    //@}

    // Save the mesh data.
    m_meshDataObject = meshData;
}

Standard_Boolean FITKOCCUnstructuredMeshVS::GetGeom(const Standard_Integer ID, const Standard_Boolean IsElement, 
    TColStd_Array1OfReal& Coords, Standard_Integer& NbNodes, MeshVS_EntityType& Type) const
{
    if (!m_meshDataObject)
    {
        return Standard_False;
    }

    // Element.
    if (IsElement)
    {
        // Get the mesh element.
        Interface::FITKAbstractElement* element = m_meshDataObject->getElementAt(ID - 1);
        if (!element)
        {
            return Standard_False;
        }

        // Save node count.
        NbNodes = element->getNodeCount();

        double pos[3]{ 0., 0., 0. };
        for (int i = 0; i < NbNodes; i++)
        {
            // Get the node and position.
            Core::FITKNode* node = m_meshDataObject->getNodeByID(element->getNodeID(i));
            node->getCoor(pos);

            // Save the position.
            Coords(i * 3 + 1) = pos[0];
            Coords(i * 3 + 2) = pos[1];
            Coords(i * 3 + 3) = pos[2];
        }

        // Save the geom type by element type. // MeshVS_ET_Volume MeshVS_ET_Face
        Interface::FITKModelEnum::FITKEleType elementType = element->getEleType();
        Type = getElementGeomType(elementType);

        return Standard_True;
    }
    // Node
    else
    {
        // Save node count.
        NbNodes = 1;

        // Get the node and position.
        Core::FITKNode* node = m_meshDataObject->getNodeAt(ID - 1);
        if (!node)
        {
            return Standard_False;
        }

        double pos[3]{ 0., 0., 0. };
        node->getCoor(pos);

        // Save the position.
        Coords(1) = pos[0];
        Coords(2) = pos[1];
        Coords(3) = pos[2];

        // Save the geom type.
        Type = MeshVS_EntityType::MeshVS_ET_Node;

        return Standard_True;
    }

    return Standard_False;
}

Standard_Boolean FITKOCCUnstructuredMeshVS::Get3DGeom (const Standard_Integer ID, Standard_Integer& NbNodes, Handle(MeshVS_HArray1OfSequenceOfInteger)& Data) const
{
    if (!m_meshDataObject)
    {
        return Standard_False;
    }

    // Get the mesh element.
    Interface::FITKAbstractElement* element = m_meshDataObject->getElementAt(ID - 1);
    if (!element)
    {
        return Standard_False;
    }

    // Save number of nodes.
    NbNodes = element->getNodeCount();

    // Get all faces.
    int nFaces = element->getFaceCount();
    Handle(MeshVS_HArray1OfSequenceOfInteger) faces = new MeshVS_HArray1OfSequenceOfInteger(1, nFaces);

    // Get element type.
    Interface::FITKModelEnum::FITKEleType type = element->getEleType();

    // Get face by element type.
    getElementTypeFaces(type, NbNodes, faces);

    Data = faces;

    return Standard_True;
}

Standard_Boolean FITKOCCUnstructuredMeshVS::GetGeomType(const Standard_Integer, const Standard_Boolean IsElement, MeshVS_EntityType& Type) const
{
    if (IsElement)
    {
        Type = MeshVS_EntityType::MeshVS_ET_Element;
        return Standard_True;
    }
    else
    {
        Type = MeshVS_EntityType::MeshVS_ET_Node;
        return Standard_True;
    }
}

Standard_Address FITKOCCUnstructuredMeshVS::GetAddr(const Standard_Integer, const Standard_Boolean) const
{
    return NULL;
}

Standard_Boolean FITKOCCUnstructuredMeshVS::GetNodesByElement(const Standard_Integer ID, TColStd_Array1OfInteger& theNodeIDs, Standard_Integer& theNbNodes) const
{
    // Get the mesh element.
    Interface::FITKAbstractElement* element = m_meshDataObject->getElementAt(ID - 1);
    if (!element)
    {
        return Standard_False;
    }

    // Get number of element nodes.
    int nPts = element->getNodeCount();
    theNbNodes = nPts;

    double pos[3]{ 0., 0., 0. };
    for (int i = 0; i < nPts; i++)
    {
        // Get the node and position.
        int nodeIndex = m_meshDataObject->getNodeIndexByID(element->getNodeID(i)) + 1;
        theNodeIDs[i + 1] = nodeIndex;
    }

    return Standard_True;
}

const TColStd_PackedMapOfInteger& FITKOCCUnstructuredMeshVS::GetAllNodes() const
{
    return m_nodeIds;
}

const TColStd_PackedMapOfInteger& FITKOCCUnstructuredMeshVS::GetAllElements() const
{
    return m_elementIds;
}

Standard_Boolean FITKOCCUnstructuredMeshVS::GetNormal(const Standard_Integer Id, const Standard_Integer Max, Standard_Real& nx, Standard_Real& ny, Standard_Real& nz) const
{
    Q_UNUSED(Id);
    Q_UNUSED(Max);
    Q_UNUSED(nx);
    Q_UNUSED(ny);
    Q_UNUSED(nz);
    return Standard_False;
}

//Standard_Boolean FITKOCCUnstructuredMeshVS::IsAdvancedSelectionEnabled() const
//{
//    return true;
//}

void FITKOCCUnstructuredMeshVS::getElementTypeFaces(int type, int nNode, Handle(MeshVS_HArray1OfSequenceOfInteger) & faces) const
{
    // Face node lists.
    QList<QList<int>> lists;

    switch (type)
    {
    // Line.
    case Interface::FITKModelEnum::Line2:
    {
        lists.push_back(QList<int>{ 0, 1 });
        break;
    }
    // Three points high-ordered line.
    case Interface::FITKModelEnum::Line3:
    {
        lists.push_back(QList<int>{ 0, 1 });
        lists.push_back(QList<int>{ 1, 2 });
        break;
    }
    // Triangle.
    case Interface::FITKModelEnum::Tri3:
    {
        lists.push_back(QList<int>{ 0, 1, 2 });
        break;
    }
    // Six points high-ordered triangle.
    case Interface::FITKModelEnum::Tri6:
    {
        lists.push_back(QList<int>{ 0, 1, 2, 3, 4, 5 });
        //lists.push_back(QList<int>{ 0, 1, 3 });
        //lists.push_back(QList<int>{ 1, 2, 4 });
        //lists.push_back(QList<int>{ 2, 0, 5 });
        break;
    }
    // Quadrilateral.
    case Interface::FITKModelEnum::Quad4:
    {
        lists.push_back(QList<int>{ 0, 1, 2, 3 });
        break;
    }
    // Eight points high-ordered quadrilateral.
    case Interface::FITKModelEnum::Quad8:
    {
        lists.push_back(QList<int>{ 0, 1, 2, 3, 4, 5, 6, 7 });
        //lists.push_back(QList<int>{ 0, 1, 4 });
        //lists.push_back(QList<int>{ 1, 2, 5 });
        //lists.push_back(QList<int>{ 2, 3, 6 });
        //lists.push_back(QList<int>{ 3, 0, 7 });
        break;
    }
    // Tetrahedron.
    case Interface::FITKModelEnum::Tet4:
    {
        lists.push_back(QList<int>{ 2, 1, 0 });
        lists.push_back(QList<int>{ 0, 1, 3 });
        lists.push_back(QList<int>{ 1, 2, 3 });
        lists.push_back(QList<int>{ 0, 2, 3 });
        break;
    }
    // Ten points high-ordered tetrahedron.
    case Interface::FITKModelEnum::Tet10:
    {
        lists.push_back(QList<int>{ 2, 1, 0, 5, 4, 6 });
        lists.push_back(QList<int>{ 0, 1, 3, 4, 8, 7 });
        lists.push_back(QList<int>{ 1, 2, 3, 5, 9, 8 });
        lists.push_back(QList<int>{ 0, 2, 3, 6, 9, 7 });
        break;
    }
    // Wedge.
    case Interface::FITKModelEnum::Wedge6:
    {
        lists.push_back(QList<int>{ 2, 1, 0 });
        lists.push_back(QList<int>{ 3, 4, 5 });
        lists.push_back(QList<int>{ 4, 3, 0, 1 });
        lists.push_back(QList<int>{ 5, 4, 1, 2 });
        lists.push_back(QList<int>{ 0, 3, 5, 2 });
        break;
    }
    // Hexahedron.
    case Interface::FITKModelEnum::Hex8:
    {
        lists.push_back(QList<int>{ 3, 2, 1, 0 });
        lists.push_back(QList<int>{ 4, 5, 6, 7 });
        lists.push_back(QList<int>{ 0, 1, 5, 4 });
        lists.push_back(QList<int>{ 1, 2, 6, 5 });
        lists.push_back(QList<int>{ 2, 3, 7, 6 });
        lists.push_back(QList<int>{ 0, 4, 7, 3 });
        break;
    }
    // Polygon.
    case Interface::FITKModelEnum::Polygon:
    {
        // Trianglate.
        for (int i = 1; i < nNode - 1; i++)
        {
            lists.push_back(QList<int>{ 0, i, i + 1 });
        }

        break;
    }
    case Interface::FITKModelEnum::EleNone:
    // Not support for now.
    // Twenty points high-ordered Hexahedron.
    case Interface::FITKModelEnum::Hex20:
    default:
        break;
    }

    // Fill the faces array.
    for (int i = 0; i < lists.count(); i++)
    {
        QList<int> list = lists[i];
        for (const int & index : list)
        {
            faces->ChangeValue(i + 1).Append(index);
        }
    }
}

MeshVS_EntityType FITKOCCUnstructuredMeshVS::getElementGeomType(int type) const
{
    switch (type)
    {
    // Line.
    case Interface::FITKModelEnum::Line2:
        return MeshVS_EntityType::MeshVS_ET_Link;
    // Three points high-ordered line.
    case Interface::FITKModelEnum::Line3:
        return MeshVS_EntityType::MeshVS_ET_Link;
    // Triangle.
    case Interface::FITKModelEnum::Tri3:
        return MeshVS_EntityType::MeshVS_ET_Face;
    // Six points high-ordered triangle.
    case Interface::FITKModelEnum::Tri6:
        return MeshVS_EntityType::MeshVS_ET_Face;
    // Quadrilateral.
    case Interface::FITKModelEnum::Quad4:
        return MeshVS_EntityType::MeshVS_ET_Face;
    // Eight points high-ordered quadrilateral.
    case Interface::FITKModelEnum::Quad8:
        return MeshVS_EntityType::MeshVS_ET_Face;
    // Tetrahedron.
    case Interface::FITKModelEnum::Tet4:
        return MeshVS_EntityType::MeshVS_ET_Volume;
    // Ten points high-ordered tetrahedron.
    case Interface::FITKModelEnum::Tet10:
        return MeshVS_EntityType::MeshVS_ET_Volume;
    // Wedge.
    case Interface::FITKModelEnum::Wedge6:
        return MeshVS_EntityType::MeshVS_ET_Volume;
    // Hexahedron.
    case Interface::FITKModelEnum::Hex8:
        return MeshVS_EntityType::MeshVS_ET_Volume;
    case Interface::FITKModelEnum::EleNone:
    // Twenty points high-ordered Hexahedron.
    case Interface::FITKModelEnum::Hex20:
        return MeshVS_EntityType::MeshVS_ET_Volume;
    default:
        return MeshVS_EntityType::MeshVS_ET_NONE;
    }

    return MeshVS_EntityType::MeshVS_ET_NONE;
}
