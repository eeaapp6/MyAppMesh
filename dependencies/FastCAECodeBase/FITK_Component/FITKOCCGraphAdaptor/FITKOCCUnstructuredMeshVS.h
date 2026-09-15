/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCUnstructuredMeshVS.h
 * @brief       Unstructured mesh data source for OCC MeshVS_Mesh.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-13
 *********************************************************************/

#ifndef __FITKOCCUNSTRUCTUREDMESHVS_H__
#define __FITKOCCUNSTRUCTUREDMESHVS_H__

#include <MeshVS_DataSource.hxx>

// Forward declaration
class TColStd_PackedMapOfInteger;
class MeshVS_HArray1OfSequenceOfInteger;

namespace Interface
{
    class FITKUnstructuredMesh;
}

class FITKOCCUnstructuredMeshVS;;
DEFINE_STANDARD_HANDLE(FITKOCCUnstructuredMeshVS, MeshVS_DataSource);

/**
 * @brief       Unstructured mesh data source for OCC MeshVS_Mesh.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-13
 */
class FITKOCCUnstructuredMeshVS : public MeshVS_DataSource
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   meshData: The unstructured mesh data object
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    explicit FITKOCCUnstructuredMeshVS(Interface::FITKUnstructuredMesh* meshData);

    /**
     * @brief       Get the geom( Element or node ) by the given id.[const][override]
     * @param[in]   ID: The geom id
     * @param[in]   IsElement: Is element or node
     * @param[out]  Coords: The positions of the node or element nodes[quote]
     * @param[out]  NbNodes: The number of nodes of this geom[quote]
     * @param[out]  Type: The mesh geom type[quote]
     * @return      Is OK
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    Standard_Boolean GetGeom(const Standard_Integer ID, const Standard_Boolean IsElement, TColStd_Array1OfReal& Coords, Standard_Integer& NbNodes, MeshVS_EntityType& Type) const override;

    /**
     * @brief       Get 3D element by the given id.[const][override]
     * @param[in]   ID: The element id
     * @param[out]  NbNodes: The number of nodes of this element[quote]
     * @param[out]  Data: The faces' nodes indice[quote]
     * @return      Is OK
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    Standard_Boolean Get3DGeom(const Standard_Integer ID, Standard_Integer& NbNodes, Handle(MeshVS_HArray1OfSequenceOfInteger)& Data) const override;

    /**
     * @brief       Get the geom type by the given id and data type.[const][override]
     * @param[in]   ID: The geom id
     * @param[in]   IsElement: Is element or node
     * @param[out]  Type: The mesh geom type[quote]
     * @return      Is OK
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    Standard_Boolean GetGeomType(const Standard_Integer ID, const Standard_Boolean IsElement, MeshVS_EntityType& Type) const override;

    /**
     * @brief       Get the direct address of the geom by the given id.[const][override]
     * @param[in]   ID: The geom id
     * @param[in]   IsElement: Is element or node
     * @return      Is OK
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    Standard_Address GetAddr(const Standard_Integer ID, const Standard_Boolean IsElement) const override;

    /**
     * @brief       Get the nodes by the given element id.[const][override]
     * @param[in]   ID: The element id
     * @param[out]  NodeIDs: The element's node indice[quote]
     * @param[out]  NbNodes: The number of nodes[quote]
     * @return      Is OK
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    Standard_Boolean GetNodesByElement(const Standard_Integer ID, TColStd_Array1OfInteger& NodeIDs, Standard_Integer& NbNodes) const override;

    /**
     * @brief       Get all nodes indice.[const][override]
     * @return      The node indice.[const]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    const TColStd_PackedMapOfInteger& GetAllNodes() const override;

    /**
     * @brief       Get all elements indice.[const][override]
     * @return      The element indice.[const]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    const TColStd_PackedMapOfInteger& GetAllElements() const override;

    /**
     * @brief       Get the normal of the element by the given element id.[const][override]
     * @param[in]   Id: The element id
     * @param[in]   Max: The max number of nodes of each face
     * @param[out]  nx: The normal X[quote]
     * @param[out]  ny: The normal Y[quote]
     * @param[out]  nz: The normal Z[quote]
     * @return      Is OK
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    Standard_Boolean GetNormal(const Standard_Integer Id, const Standard_Integer Max, Standard_Real& nx, Standard_Real& ny, Standard_Real& nz) const override;

    // Standard_Boolean IsAdvancedSelectionEnabled () const Standard_OVERRIDE;

    DEFINE_STANDARD_RTTIEXT(FITKOCCUnstructuredMeshVS, MeshVS_DataSource);

private:
    /**
     * @brief       Get the element faces node index with the given element type and node count.[const]
     * @param[in]   type: The element type enum value
     * @param[in]   nNode: The number of node count
     * @param[out]  faces: The faces node indice[quote]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    void getElementTypeFaces(int type, int nNode, Handle(MeshVS_HArray1OfSequenceOfInteger) & faces) const;

    /**
     * @brief       Get the element geom type by the given element type.[const]
     * @param[in]   type: The element type enum value
     * @return      The geom type
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    MeshVS_EntityType getElementGeomType(int type) const;

private:
    /**
     * @brief       The unstructured mesh data object.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    Interface::FITKUnstructuredMesh* m_meshDataObject{ nullptr };

    /**
     * @brief       The node ids list.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    TColStd_PackedMapOfInteger m_nodeIds;

    /**
     * @brief       The element ids list.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    TColStd_PackedMapOfInteger m_elementIds;

};

#endif // __FITKOCCUNSTRUCTUREDMESHVS_H__
