/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshGenerateProcessorGmshExec.h
 * @brief  gmsh网格划分处理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-10-12
 *********************************************************************/
#ifndef _FITKMESHGENERATEPROCESSORGMSHEXEC_H_
#define _FITKMESHGENERATEPROCESSORGMSHEXEC_H_

#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKComponentManager;
};

namespace Gmsh
{
    class FITKGmshMshData;
    /**
     * @brief  gmsh网格划分处理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-10-12
     */
    class FITKMeshGenerateProcessorGmshExec : public Interface::FITKAbstractMeshProcessor
    {
    public:
        /**
         * @brief Construct a new FITKMeshGenerateGmsh object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        explicit FITKMeshGenerateProcessorGmshExec();
        /**
         * @brief    Destroy the FITKMeshGenerateGmsh object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-10-12
         */
        virtual ~FITKMeshGenerateProcessorGmshExec();

        /**
         * @brief 开始执行处理
         * @param[i]  info           附加信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-10-12
         */
        virtual void start(QStringList info = QStringList());

    private:
        /**
         * @brief    开始执行网格映射几何信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-09
         */
        void startGridMapGeoInfo(FITKGmshMshData* data, int geoId);

    };
}


#endif // !_FITKMESHGENERATEPROCESSORGMSH_H_
