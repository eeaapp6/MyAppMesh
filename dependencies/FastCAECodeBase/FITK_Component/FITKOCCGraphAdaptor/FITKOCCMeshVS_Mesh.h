/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCMeshVS_Mesh.h
 * @brief       Subclass of MeshVS_Mesh, which can save user information.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-31
 *********************************************************************/

#ifndef __FITKOCCMESHVS_MESH_H__
#define __FITKOCCMESHVS_MESH_H__

#include <MeshVS_Mesh.hxx>
#include "FITKAIS_ObjectBase.h"

class FITKOCCMeshVS_Mesh;;
DEFINE_STANDARD_HANDLE(FITKOCCMeshVS_Mesh, MeshVS_Mesh);

/**
 * @brief       Subclass of MeshVS_Mesh, which can save user information.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-08-31
 */
class FITKOCCMeshVS_Mesh : public MeshVS_Mesh, public FITKAIS_ObjectBase
{
public:
    /**
     * @brief       Constructor.
     * @param[in]   theIsAllowOverlapped: If it is allowed to draw edges overlapped with beams
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-31
     */
    FITKOCCMeshVS_Mesh(const Standard_Boolean theIsAllowOverlapped = Standard_False);

    /**
     * @brief       Destructor.[virtual]
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-08-31
     */
    virtual ~FITKOCCMeshVS_Mesh() = default;
};

#endif // __FITKOCCMESHVS_MESH_H__
