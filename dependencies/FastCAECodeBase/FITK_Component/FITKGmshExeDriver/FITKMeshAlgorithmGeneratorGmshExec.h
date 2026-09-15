/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshAlgorithmGeneratorGmshExec.h
 * @brief  网格算法生成器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-15
 *********************************************************************/
#ifndef _FITKMESHALGORITHMGENERATORGMSHEXEC_H___
#define _FITKMESHALGORITHMGENERATORGMSHEXEC_H___


#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshAlgorithmInfoGenerator.h"

namespace Gmsh
{
    /**
     * @brief  网格算法生成器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-12
     */
    class FITKMeshAlgorithmGeneratorGmshExec :
        public Interface::FITKAbstractMeshAlgorithmInfoGenerator
    {
    public:
        /**
         * @brief Construct a new FITKMeshAlgorithmGeneratorGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        explicit FITKMeshAlgorithmGeneratorGmshExec() = default;
        /**
         * @brief Destroy the FITKMeshAlgorithmGeneratorGmshExec object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        virtual ~FITKMeshAlgorithmGeneratorGmshExec() = default;
        /**
        * @brief 生成全局网格算法信息
        * @return FITKGlobalMeshSizeInfo*
        * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
        */
        virtual Interface::FITKGlobalMeshGenerateAlgorithmInfo* generateGlobalMeshAlgorithmInfo() override;
    };
}


#endif
