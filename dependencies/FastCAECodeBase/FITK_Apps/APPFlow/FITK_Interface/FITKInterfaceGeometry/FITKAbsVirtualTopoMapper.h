/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbsVirtualTopoMapper.h
 * @brief  几何拓扑查询
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-08-20
 *
 */
#ifndef _FITK_VIRTUAL_TOPO_MAPPER_H___
#define _FITK_VIRTUAL_TOPO_MAPPER_H___

#include "FITKInterfaceGeometryAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObject.hpp"
#include "FITKGeoEnum.h"
#include <QList>

namespace Interface
{
    class FITKAbsVirtualTopo;
    /**
     * @brief 拓扑关系查询
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-08-20
     */
    class FITKInterfaceGeometryAPI FITKVirtualTopoMapper : public Core::FITKAbstractObject
    {
    public:
        /**
         * @brief Construct a new FITKVirtualTopoMapper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        explicit FITKVirtualTopoMapper() = default;
        /**
         * @brief Destroy the FITKVirtualTopoMapper object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        virtual ~FITKVirtualTopoMapper() = default;
        /**
         * @brief 查询拓扑关系
         * @param[i]  vtp            查询的拓扑对象
         * @param[i]  shapeType      查询形状类型
         * @param[i]  basicTopoOnly  仅查询基础未合并拓扑
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        void mapTopo(FITKAbsVirtualTopo* vtp, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly = true);
        /**
          * @brief 查询拓扑关系
          * @param[i]  vtpID          查询的拓扑ID
          * @param[i]  shapeType      查询形状类型
          * @param[i]  basicTopoOnly  仅查询基础未合并拓扑
          * @author LiBaojun (libaojunqd@foxmail.com)
          * @date 2024-08-20
          */
        void mapTopo(int vtpID, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly = true);
        /**
         * @brief 查询到的长度，拓扑的数量
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        int length();
        /**
         * @brief 查询到的拓扑对象
         * @param[i]  index          查询的索引
         * @return FITKAbsVirtualTopo*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        FITKAbsVirtualTopo* virtualTopo(const int index);

    private:
        /**
         * @brief 向下迭代递归查找
         * @param[i]  topo           拓扑对象
         * @param[i]  shapeType      类型
         * @param[i]  basicTopoOnly  仅查询基础未合并拓扑
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        void iteratorLower(FITKAbsVirtualTopo* topo, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly);
        /**
         * @brief 向上迭代递归查找
         * @param[i]  topo           拓扑对象
         * @param[i]  shapeType      类型
         * @param[i]  basicTopoOnly  仅查询基础未合并拓扑
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        void iteratorUpper(FITKAbsVirtualTopo* topo, FITKGeoEnum::VTopoShapeType shapeType, bool basicTopoOnly);

    private:
        /**
         * @brief 查找到的拓扑对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-08-20
         */
        QList<FITKAbsVirtualTopo*> _mappedTopo{};
    };
}
#endif
