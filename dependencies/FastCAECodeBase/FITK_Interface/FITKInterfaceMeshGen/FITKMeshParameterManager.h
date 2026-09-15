/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKMeshParameterManager.h
 * @brief  网格划分参数管理器
 * @author BaGuijun (baguijun@163.com)
 * @date   2026-01-25
 *********************************************************************/
#ifndef _FITKMeshParameterManager_H_
#define _FITKMeshParameterManager_H_

#include "FITKInterfaceMeshGenAPI.h"
#include "FITKMeshParameterManager.h"
#include "FITKMeshParameterBase.h"

#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Interface
{
    class FITKMeshParameterLine;
    class FITKMeshParameterSurface;
    class FITKMeshParameterVolume;
    class FITKAbsGeoCommand;
    /**
     * @brief  网格划分参数管理器
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-01-25
     */
    class FITKInterfaceMeshGenAPI FITKMeshParameterManager : public Core::FITKAbstractDataObject /*Core::FITKAbstractDataManager<FITKMeshParameterBase>*/
    {
        FITKCLASS(Interface, FITKMeshParameterManager);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-25
         */
        explicit FITKMeshParameterManager() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2026-01-25
         */
        virtual ~FITKMeshParameterManager() = default;
        /**
         * @brief  清空所有
         * @author BaGuijun (baguijun@163.com)
         * @date   2026-01-27
         */
        void clearAll();
        /**
         * @brief       获取线网格划分参数施加的线ID列表。
         * @return      线ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-10
         */
        QList<int> getExistLineTopoIds();

        /**
         * @brief       获取面网格划分参数施加的面ID列表。
         * @return      面ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        QList<int> getExistSurfaceTopoIds();

        /**
         * @brief       获取体网格划分参数施加的体ID列表。
         * @return      体ID列表
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-08-25
         */
        QList<int> getExistVolumeTopoIds();
        /**
         * @brief       获取线单元划分参数。
         * @param[in]   vTopoID：虚拓扑ID
         * @param[in]   isCreate：是否创建[缺省]
         * @return      线单元划分参数
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        FITKMeshParameterLine* getLineMeshingParameters(int vTopoID, bool isCreate = true);

        /**
         * @brief       清除线单元划分参数。
         * @param[in]   cmd
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        void clearLineMeshingParameters(Interface::FITKAbsGeoCommand* cmd = nullptr);

        /**
         * @brief   获取面网格划分参数 如果链表中有则直接返回 如果没有则创建一个
         * @param   vTopoID   虚拓扑ID
         * @param   isCreate  是否创建
         * @return  面网格划分参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        FITKMeshParameterSurface* getSurfaceMeshingParameters(int vTopoID, bool isCreate = true);
        /**
         * @brief   清除所有面网格参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-05-14
         */
        void clearSurfaceMeshingParameters(Interface::FITKAbsGeoCommand* cmd = nullptr);
        /**
         * @brief   获取体网格划分参数 如果链表中有则直接返回 如果没有则创建一个
         * @param   vTopoID   虚拓扑ID
         * @param   isCreate  是否创建
         * @return  面网格划分参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        FITKMeshParameterVolume* getVolumeMeshingParameters(int vTopoID, bool isCreate = true);

        /**
         * @brief   清除所有体网格参数
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-05-14
         */
        void clearVolumeMeshingParameters(Interface::FITKAbsGeoCommand* cmd = nullptr);
    protected:
        /**
         * @brief       线单元划分参数，key为虚拓扑ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-04
         */
        QHash<int, FITKMeshParameterLine*> _lineMesingParameter{};

        /**
         * @brief   面网格划分参数 key为虚拓扑ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        QHash<int, FITKMeshParameterSurface*> _surfaceMesingParameter{};

        /**
         * @brief   体网格划分参数 key为虚拓扑ID
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2025-04-10
         */
        QHash<int, FITKMeshParameterVolume*> _volumeMesingParameter{};
    };
}

#endif