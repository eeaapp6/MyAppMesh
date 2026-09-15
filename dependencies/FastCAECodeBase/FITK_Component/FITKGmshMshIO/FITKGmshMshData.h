/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGmshMshData.h
 * @brief  Gmsh Msh数据结构
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2026-02-03
 *********************************************************************/
#ifndef _FITK_GMSH_MSH_DATA_READER_H___
#define _FITK_GMSH_MSH_DATA_READER_H___

#include "FITKGmshMshIOAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKComponentManager;
}

namespace Gmsh
{
    /**
     * @brief  Gmsh Msh数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2026-01-28
     */
    class FITKGmshMshIOAPI FITKGmshMshData : public Core::FITKAbstractDataObject
    {
    public:
        FITKGmshMshData() = default;
        virtual ~FITKGmshMshData() = default;

        /**
         * @brief    初始化 将meshData数据初始化到Gmsh mshData里
         * @param[i] meshData 网格数据
         * @param[i] componentMgr 数组数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        void init(Interface::FITKUnstructuredMesh* meshData, Interface::FITKComponentManager* componentMgr);
        /**
         * @brief    将Gmsh mshData更新到meshData里
         * @param[i] meshData 网格数据
         * @param[i] componentMgr 数组数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-01-28
         */
        virtual void update(Interface::FITKUnstructuredMesh* meshData, Interface::FITKComponentManager* componentMgr = nullptr);

    public:
        /**
         * @brief  节点X坐标
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        QVector<double> _node_coordX{};
        /**
         * @brief  节点Y坐标
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        QVector<double> _node_coordY{};
        /**
         * @brief  节点Z坐标
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        QVector<double> _node_coordZ{};
        /**
         * @brief  单元类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        QVector<int> _element_type{};
        /**
         * @brief  单元
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        QVector<QList<int>> _element{};
        /**
         * @brief  单元数组维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        QVector<int> _array_dim{};
        /**
         * @brief  单元数组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-01-28
         */
        QVector<QList<int>> _array{};
    };
}

#endif
