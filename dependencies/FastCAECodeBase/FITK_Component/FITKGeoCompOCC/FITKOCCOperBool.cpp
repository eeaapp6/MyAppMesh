/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCOperBool.h"
#include <TopoDS_Shape.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <TopoDS_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <ShapeFix_Shape.hxx>
#include "FITKOCCComandCommon.h"

#include <FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h>
#include <FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h>
#include "FITKOCCVirtualTopoCreator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSurface.h"

#include <FITKOCCExtendTool.h>
namespace OCC
{

    TopoDS_Shape removeSolidFromCompsolid(const TopoDS_CompSolid& compsolid, const TopoDS_Solid& solid) {

        TopoDS_Builder builder;
        TopoDS_CompSolid result;
        builder.MakeCompSolid(result);

        for (TopoDS_Iterator iter(compsolid); iter.More(); iter.Next()) {
            TopoDS_Shape s = iter.Value();
            if (s.ShapeType() != TopAbs_ShapeEnum::TopAbs_SOLID || !TopoDS::Solid(s).IsSame(solid)) {
                builder.Add(result, s);
            }
            /*else
            {
                std::cout << "Removed!" << std::endl;
            }*/
        }

        return result;
    }

    TopoDS_Shape removeSolidFromComp(const TopoDS_Shape& comp, const TopoDS_Solid& solid) {

        if (comp.ShapeType() != TopAbs_ShapeEnum::TopAbs_COMPOUND && comp.ShapeType() != TopAbs_ShapeEnum::TopAbs_COMPSOLID) return TopoDS_Shape();

        TopoDS_Builder builder;
        TopoDS_Compound result;
        builder.MakeCompound(result);

        for (TopoDS_Iterator iter(comp); iter.More(); iter.Next()) {
            TopoDS_Shape s = iter.Value();
            if (s.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND) {
                builder.Add(result, removeSolidFromComp(TopoDS::Compound(s), solid));
            }
            else if (s.ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPSOLID) {
                builder.Add(result, removeSolidFromCompsolid(TopoDS::CompSolid(s), solid));
            }
            else if (s.ShapeType() == TopAbs_ShapeEnum::TopAbs_SOLID) {
                if (!TopoDS::Solid(s).IsSame(solid)) {
                    builder.Add(result, s);
                }
                //else
                //{
                //    std::cout << "Removed!" << std::endl;
                //}
            }
            else {
                builder.Add(result, s);
            }
        }

        return result;
    }

    FITKOCCOperBool::FITKOCCOperBool() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }



    bool FITKOCCOperBool::update()
    {
        const TopoDS_Shape* s1{};
        const TopoDS_Shape* s2{};

        TopoDS_Builder builder;
        TopoDS_Compound comp;
        builder.MakeCompound(comp);


        if (!m_Target.isNull() && !m_Tool.isNull()) {
            if (m_Target.VirtualTopoId == m_Tool.VirtualTopoId && m_Target.CmdId == m_Tool.CmdId) {
                printLog(tr("The target shape cannot be the same as the tool shape"), 3);
                return false;
            }
            auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

            auto targetCmd = geoCmdList->getDataByID(m_Target.CmdId);
            if (targetCmd == nullptr) return false;

            auto vTargetShape = targetCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSSolid, m_Target.VirtualTopoIndex);
            if (vTargetShape == nullptr) return false;
            s1 = &(vTargetShape->getTopoShape());
            if (s1 == nullptr || s1->ShapeType() != TopAbs_ShapeEnum::TopAbs_SOLID) return false;

            TopoDS_Shape* targetShape = targetCmd->getTShapeAgent<FITKAbstractOCCModel>()->getShape();
            if (targetShape == nullptr) return false;
            if (targetShape->ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND || targetShape->ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPSOLID) {
                builder.Add(comp, removeSolidFromComp(*targetShape, TopoDS::Solid(*s1)));
            }
                 
            auto toolCmd = geoCmdList->getDataByID(m_Tool.CmdId);
            if (toolCmd == nullptr) return false;
            auto vToolShape = toolCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(Interface::FITKGeoEnum::VTopoShapeType::VSSolid, m_Tool.VirtualTopoIndex);
            if (vToolShape == nullptr) return false;
            s2 = &(vToolShape->getTopoShape());

            TopoDS_Shape* toolShape = toolCmd->getTShapeAgent<FITKAbstractOCCModel>()->getShape();
            if (toolShape == nullptr) return false;
            if (toolShape->ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPOUND || toolShape->ShapeType() == TopAbs_ShapeEnum::TopAbs_COMPSOLID) {
                if (m_Target.CmdId != m_Tool.CmdId)
                {
                    builder.Add(comp, removeSolidFromComp(*toolShape, TopoDS::Solid(*s2)));
                }
                else
                {
                    comp = TopoDS::Compound(removeSolidFromComp(comp, TopoDS::Solid(*s2)));
                    // builder.Add(comp, );
                }
            }

        }
        // 兼容旧接口
        else if (_shape1 != nullptr && _shape2 != nullptr) {
            FITKAbstractOCCModel* model1 = dynamic_cast<FITKAbstractOCCModel*>(_shape1);
            FITKAbstractOCCModel* model2 = dynamic_cast<FITKAbstractOCCModel*>(_shape2);
            if (model2 == nullptr || model1 == nullptr) return false;
            s1 = model1->getShape();
            s2 = model2->getShape();
        }
        else {
            return false;
        }

        TopoDS_Shape result;
        switch (_boolOperType)
        {
        case Interface::FITKAbsGeoOperBool::GBTCommon:
            result = this->common(s1, s2);
            break;
        case Interface::FITKAbsGeoOperBool::GBTCut:
            result = this->cut(s1, s2);
            break;
        case Interface::FITKAbsGeoOperBool::GBTAdd:
            result = this->add(s1, s2);
            break;
        default:
            return false;
        }

        if (FITKOCCExtendTool::IsEmptyShape(result)) {
            printLog(tr("Failed to perform a Boolean operation!"), 3);
            return false;
        }

        if (comp.IsNull()) {
            // 修复异常形状。
            ShapeFix_Shape fix(result);
            fix.Perform();

            const TopoDS_Shape & fixedShape = fix.Shape();

            _occShapeAgent->updateShape(fixedShape);
        }
        else {
            builder.Add(comp, result);

            // 修复异常形状。
            ShapeFix_Shape fix(comp);
            fix.Perform();

            const TopoDS_Shape & fixedShape = fix.Shape();

            _occShapeAgent->updateShape(fixedShape);
        }

        return true;
    }

    TopoDS_Shape FITKOCCOperBool::cut(const TopoDS_Shape* shape1, const TopoDS_Shape* shape2)
    {
        try {
            auto aCuttedShape = BRepAlgoAPI_Cut(*shape1, *shape2);
            return aCuttedShape;
        }
        catch (...)
        {
            printLog(tr("Failed to perform a Boolean operation!"), 3);
            return TopoDS_Shape();
        }
    }

    TopoDS_Shape FITKOCCOperBool::common(const TopoDS_Shape* shape1, const TopoDS_Shape* shape2)
    {
        try {
            if (shape1 == nullptr || shape2 == nullptr) return TopoDS_Shape();
            const TopoDS_Shape &ashape = BRepAlgoAPI_Common(*shape1, *shape2);
            if (!ashape.IsSame(TopoDS_Shape()))
            {
                return ashape;
            }

            BRepAlgoAPI_Section S(*shape1, *shape2, false);
            S.Build();
            if (S.IsDone())
            {
                return S.Shape();
            }
            return TopoDS_Shape();
        }
        catch (...)
        {
            printLog(tr("Failed to perform a Boolean operation!"), 3);
            return TopoDS_Shape();
        }
    }

    TopoDS_Shape FITKOCCOperBool::add(const TopoDS_Shape* shape1, const TopoDS_Shape* shape2)
    {
        try {
            BRepAlgoAPI_Fuse fau(*shape1, *shape2);

            if (!fau.IsDone()) return TopoDS_Shape();
            const TopoDS_Shape &aFusedShape = fau.Shape();

            if (aFusedShape.IsNull()) return TopoDS_Shape();
            TopoDS_Shape shape = FITKOCCComandCommon::removeSplitter(aFusedShape);
            return shape;
        }
        catch (...)
        {
            printLog(tr("Failed to perform a Boolean operation!"), 3);
            return TopoDS_Shape();
        }

    }

}



