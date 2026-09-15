/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCGeoSketchDeleter.h"

// OCC
#include <BRepTools_ReShape.hxx>

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITKOCCVirtualTopoCreator.h"
#include "FITKOCCModelSketch2D.h"

namespace OCC
{
    FITKOCCGeoSketchDeleter::FITKOCCGeoSketchDeleter() : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCGeoSketchDeleter::update()
    {
        Interface::FITKGeoCommandList* cmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        FITKOCCModelSketch2D* sketch = _occShapeAgent->getSketch();
        if (!sketch || !cmdList)
        {
            return false;
        }

        if (m_ToBeRemovedEntities.isEmpty()) 
        {
            return false;
        }

        BRepTools_ReShape reShape;
        QHash<FITKAbstractOCCModel*, TopoDS_Shape*> subShapeHash;

        for (const Interface::VirtualShape & entity : m_ToBeRemovedEntities)
        {
            Interface::FITKAbsGeoCommand* entityCmd = cmdList->getDataByID(entity.CmdId);
            if (!entityCmd)
            {
                return false;
            }

            // 获取形状。
            FITKOCCTopoShape* occShape = entityCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(
                Interface::FITKGeoEnum::VTopoShapeType::VSEdge, entity.VirtualTopoIndex);
            if (!occShape) 
            {
                occShape = entityCmd->getShapeTopoByIndexT<FITKOCCTopoShape>(
                    Interface::FITKGeoEnum::VTopoShapeType::VSPoint, entity.VirtualTopoIndex);
            }

            if (!occShape)
            {
                return false;
            }

            FITKAbstractOCCModel* occSubModel = entityCmd->getTShapeAgent<FITKAbstractOCCModel>();            
            if (!occSubModel)
            {
                return false;
            }

            // 从模型中移除数据。
            TopoDS_Shape* subShape = occSubModel->getShape();
            if (!subShape)
            {
                return false;
            }

            reShape.Remove(occShape->getTopoShape());
            subShapeHash.insert(occSubModel, subShape);
        }

        // 刷新模型。
        QHashIterator<FITKAbstractOCCModel*, TopoDS_Shape*> iter(subShapeHash);
        while (iter.hasNext())
        {
            iter.next();

            const TopoDS_Shape & newShape = reShape.Apply(*iter.value());
            iter.key()->updateShape(newShape);
        }

        return true;
    }
}
