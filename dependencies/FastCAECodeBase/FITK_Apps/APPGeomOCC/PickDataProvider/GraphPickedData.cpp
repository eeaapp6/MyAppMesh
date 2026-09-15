/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphPickedData.h"

// Graph
#include "FITK_Component/FITKOCCGraphAdaptor/FITKOCCGraphObject3D.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

// GUI
#include "GUIPickInfo.h"

namespace GraphData
{
    GraphPickedData::GraphPickedData(GUI::GUIPickInfoStru pickedInfo, Interface::FITKAbsGeoShapeAgent* agent, Interface::FITKAbsVirtualTopo* topoObject) :
        _pickedInfo(pickedInfo), _agent(agent)
    {
        _virtualTopo.append(topoObject);
    }

    GraphPickedData::GraphPickedData(GUI::GUIPickInfoStru pickedInfo, Interface::FITKAbsGeoShapeAgent* agent, QList<Interface::FITKAbsVirtualTopo*> topoObjects) :
        _pickedInfo(pickedInfo), _agent(agent)
    {
        _virtualTopo.append(topoObjects);
    }

    GraphPickedData::GraphPickedData(GUI::GUIPickInfoStru pickedInfo, int DatumID) :
        _pickedInfo(pickedInfo), _datumID(DatumID)
    {

    }

    GraphPickedData::GraphPickedData(GUI::GUIPickInfoStru pickedInfo, Exchange::FITKOCCGraphObject3D* obj3d) :
        _pickedInfo(pickedInfo), _graphObj(obj3d)
    {
        // 初始化拾取对象信息。
        if (obj3d)
        {
            Exchange::FITKOCCCommons::GraphInfo gInfo = obj3d->getGraphInfo();
            m_graphInfo.DataObjId = -1;
            m_graphInfo.Type = GraphData::GraphDataType(gInfo.Type);
        }
    }

    GUI::GUIPickInfoStru GraphPickedData::getPickedInfo()
    {
        return _pickedInfo;
    }

    Interface::FITKAbsVirtualTopo* GraphPickedData::getVirtualTopo()
    {
        if (_virtualTopo.size() < 1) return nullptr;
        return _virtualTopo.first();
    }

    QList<Interface::FITKAbsVirtualTopo*> GraphPickedData::getVirtualTopoAll()
    {
        return _virtualTopo;
    }

    QList<int> GraphPickedData::getVirtualTopoIndice()
    {
        QList<int> indice;

        if (!_agent)
        {
            return indice;
        }

        Interface::FITKVirtualTopoManager* vMgr = _agent->getVirtualTopoManager();
        if (!vMgr)
        {
            return indice;
        }

        // 寻找拾取类型对应的虚拓扑形状管理器。
        Interface::FITKShapeVirtualTopoManager* vsMgr{ nullptr };
        GUI::GUIPickInfo::PickObjType objType = _pickedInfo._pickObjType;
        switch (objType)
        {
        case GUI::GUIPickInfo::POBJVert:
            vsMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSPoint);
            break;
        case GUI::GUIPickInfo::POBJEdge:
        case GUI::GUIPickInfo::POBJDirecEdge:
        case GUI::GUIPickInfo::POBJEdgeProjPos:
            vsMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSEdge);
            break;
        case GUI::GUIPickInfo::POBJWire:
            vsMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSWire);
            break;
        case GUI::GUIPickInfo::POBJFace:
        case GUI::GUIPickInfo::POBJDirecFace:
        case GUI::GUIPickInfo::POBJFaceProjPos:
            vsMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            break;
        case GUI::GUIPickInfo::POBJShell:
            vsMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSShell);
            break;
        case GUI::GUIPickInfo::POBJSolid:
            vsMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
            break;
        case GUI::GUIPickInfo::POBJAssembly:
            vsMgr = vMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSAssembly);
            break;
        case GUI::GUIPickInfo::POBJShape:
        {
            // 获取根节点虚拓扑。
            Interface::FITKAbsVirtualTopo* vTopo = vMgr->getRootObj();
            if (!vTopo)
            {
                return indice;
            }

            // 获取形状虚拓扑管理器。
            vsMgr = vMgr->getShapeVirtualTopoManager(vTopo->getShapeType());

            break;
        }
        case GUI::GUIPickInfo::POBJNone:
        case GUI::GUIPickInfo::POBJCustom:
        default:
            return indice;
        }

        if (!vsMgr)
        {
            return indice;
        }

        // 遍历虚拓扑对象获取索引。
        for (Interface::FITKAbsVirtualTopo* vTopo : _virtualTopo)
        {
            if (!vTopo)
            {
                continue;
            }

            int vTopoIndex = vsMgr->getDataIndex(vTopo);
            if (vTopoIndex >= 0)
            {
                indice.push_back(vTopoIndex);
            }
        }

        return indice;
    }

    QList<Interface::VirtualShape> GraphPickedData::getVirtualShapes()
    {
        QList<Interface::VirtualShape> virtualShapes;

        if (!_agent)
        {
            return virtualShapes;
        }

        Interface::FITKAbsGeoCommand* cmd = _agent->getGeoCommand();
        Interface::FITKVirtualTopoManager* vMgr = _agent->getVirtualTopoManager();
        if (!cmd || !vMgr)
        {
            return virtualShapes;
        }

        int cmdId = cmd->getDataObjectID();

        // 遍历虚拓扑对象获取索引。
        for (Interface::FITKAbsVirtualTopo* vTopo : _virtualTopo)
        {
            if (!vTopo)
            {
                continue;
            }

            // 寻找拾取类型对应的虚拓扑形状管理器。
            Interface::FITKGeoEnum::VTopoShapeType sType = vTopo->getShapeType();
            Interface::FITKShapeVirtualTopoManager* vsMgr = vMgr->getShapeVirtualTopoManager(sType);
            if (!vsMgr)
            {
                return virtualShapes;
            }

            int vTopoIndex = vsMgr->getDataIndex(vTopo);
            if (vTopoIndex >= 0)
            {
                Interface::VirtualShape vs;
                vs.CmdId = cmdId;
                vs.Type = vTopo->getShapeType();
                vs.VirtualTopoId = vTopo->getDataObjectID();
                vs.VirtualTopoIndex = vTopoIndex;
                virtualShapes.push_back(vs);
            }
        }

        return virtualShapes;
    }

    Interface::FITKAbsGeoShapeAgent* GraphPickedData::getGeoShapeAgent()
    {
        return _agent;
    }

    int GraphPickedData::getDatumID()
    {
        return _datumID;
    }

    void GraphPickedData::setDatumID(int datumID)
    {
        _datumID = datumID;
    }

    void GraphPickedData::setGraphObject(Exchange::FITKOCCGraphObject3D* obj)
    {
        _graphObj = obj;
    }

    QVariant GraphPickedData::getGraphOutputInfo(int outputRole)
    {
        // 从可视化对象获取信息。
        if (!_graphObj)
        {
            return QVariant();
        }

        return _graphObj->getOutputVariantData(outputRole);
    }

    bool GraphPickedData::isEmpty()
    {
        return _virtualTopo.size() < 1;
    }

    void GraphPickedData::setGraphInfo(GraphInfo graphInfo)
    {
        m_graphInfo = graphInfo;
    }
    GraphInfo GraphPickedData::getGraphInfo()
    {
        return m_graphInfo;
    }

    void GraphPickedData::appendVirtualTopo(Interface::FITKAbsVirtualTopo* vTopo)
    {
        // 追加虚拓扑（合并拾取数据使用）。
        if (_virtualTopo.contains(vTopo))
        {
            return;
        }

        _virtualTopo.push_back(vTopo);
    }
}