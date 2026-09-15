/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoVirtualTopoCombine.h"

namespace Interface {
    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoVirtualTopoCombineFaces::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTVirtualTopoCombineFaces;
    }


    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoVirtualTopoCombineEdges::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTVirtualTopoCombineEdges;
    }

    void FITKAbsGeoVirtualTopoCombine::removeFromPart(FITKAbsGeoCommand* part)
    {
        if (!part)
        {
            return;
        }

        this->setGeoCommandManager(nullptr);

        // 获取部件拓扑管理器。
        FITKAbsGeoShapeAgent* shapeAgent = part->getShapeAgent();
        if (!shapeAgent)
        {
            return;
        }

        FITKVirtualTopoManager* vMgr = shapeAgent->getVirtualTopoManager();
        if (!vMgr)
        {
            return;
        }

        // 清除被隐藏对象属性。
        for (const VirtualShape & vShape : m_hiddenTopos)
        {
            FITKShapeVirtualTopoManager* vTopoMgr = vMgr->getShapeVirtualTopoManager(vShape.Type);
            if (!vTopoMgr)
            {
                continue;
            }

            // 拓扑不存在则无需处理。
            FITKAbsVirtualTopo* vTopo = vTopoMgr->getDataByID(vShape.VirtualTopoId);
            if (!vTopo)
            {
                continue;
            }

            vTopo->setCombinedToTopo(nullptr);
        }

        // 移除创建的拓扑对象。
        for (const VirtualShape & vShape : m_resultTopos)
        {
            FITKShapeVirtualTopoManager* vTopoMgr = vMgr->getShapeVirtualTopoManager(vShape.Type);
            if (!vTopoMgr)
            {
                continue;
            }

            // 拓扑不存在则无需移除。
            FITKAbsVirtualTopo* vTopo = vTopoMgr->getDataByID(vShape.VirtualTopoId);
            if (!vTopo)
            {
                continue;
            }

            if (vTopo->getCombinedTopoCount() == 0)
            {
                continue;
            }

            // 清除合并拓扑关联性。
            vTopo->clearCombinedTopo();

            int nParents = vTopo->getParentTopoCount();
            for (int i = nParents - 1; i >= 0; i--)
            {
                FITKAbsVirtualTopo* vTopoParent = vTopo->getParentTopo(i);
                if (vTopoParent)
                {
                    vTopoParent->removeSubTopo(vTopo);
                }
            }

            int nChildren = vTopo->getSubTopoCount();
            for (int i = nChildren - 1; i >= 0; i--)
            {
                FITKAbsVirtualTopo* vTopoSub = vTopo->getSubTopo(i);
                if (vTopoSub)
                {
                    vTopo->removeSubTopo(vTopoSub);
                }
            }

            vTopoMgr->removeDataObj(vTopo);
        }
    }
}