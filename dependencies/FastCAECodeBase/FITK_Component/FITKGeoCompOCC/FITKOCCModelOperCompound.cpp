/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelOperCompound.h"

// OCC base
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Iterator.hxx>

// 倒角/倒圆
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// App
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Geometry
#include "FITKOCCVirtualTopoCreator.h"
#include <FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h>
#include <FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h>
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
namespace OCC
{
    // 基类构造。
    //@{
    FITKOCCModelOperCompound::FITKOCCModelOperCompound() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }
    //@}

    // 复合体对象添加。
    //@{
    FITKOCCModelOperAddCompound::FITKOCCModelOperAddCompound()
    {

    }
    //@}

    bool FITKOCCModelOperAddCompound::update()
    {
        TopoDS_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);

        if (m_solidCmdIds.size() < 1 || m_solidCmdId.CmdId < 0) return false;
        //判断选择的两个几何不能是同一个
        if (m_solidCmdIds[0].CmdId == m_solidCmdId.CmdId) return false;

        FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_solidCmdId.CmdId);
        if (comm == nullptr) return false;

        auto occShape = dynamic_cast<OCC::FITKAbstractOCCModel*>(comm->getShapeAgent());
        if (occShape == nullptr) return false;

        //如果是复合对象 则平铺一层
        TopoDS_Iterator it(*occShape->getShape());
        while (it.More())
        {
            TopoDS_Shape shape = it.Value();
            builder.Add(comp, shape);
            it.Next();
        }

        for (int i = 0; i < m_solidCmdIds.size(); ++i)
        {
            //数据获取
            comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_solidCmdIds[i].CmdId);
            if (comm == nullptr) return false;
            auto occShape = dynamic_cast<OCC::FITKAbstractOCCModel*>(comm->getShapeAgent());
            if (occShape == nullptr) return false;

            builder.Add(comp, *occShape->getShape());
        }
        _occShapeAgent->updateShape(comp);
        return true;
    }

    // 复合体对象添加。
    //@{
    FITKOCCModelOperRemoveCompound::FITKOCCModelOperRemoveCompound()
    {

    }

    bool FITKOCCModelOperRemoveCompound::update()
    {
        if (m_solidCmdIds.size() < 1 || m_solidCmdId.CmdId < 0) return false;

        TopoDS_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);

        FITKAbsGeoCommand* comm = FITKDATAREPO->getTDataByID<FITKAbsGeoCommand>(m_solidCmdId.CmdId);
        if (comm == nullptr) return false;

        Interface::FITKAbsVirtualTopo* vTopo = comm->getVirtualTopoByIndex(Interface::FITKGeoEnum::VTopoShapeType::VSAssembly, 0);
        if (vTopo == nullptr) return false;
        int count = vTopo->getSubTopoCount();
        for (int i = 0; i < count; i++)
        {
            auto vTopoShape = vTopo->getSubTopo(i);
            if (vTopoShape == nullptr) return false;
            bool isEqual = false;
            for (int j = 0; j < m_solidCmdIds.size(); j++)
            {
                if (m_solidCmdIds[j].VirtualTopoIndex != i) isEqual = true;
            }
            if (isEqual)
                builder.Add(comp, vTopoShape->getShapeT<OCC::FITKOCCTopoShape>()->getTopoShape());
        }
        if (comp.IsNull()) return false;
        _occShapeAgent->updateShape(comp);
        return true;
    }

    //@}
}
