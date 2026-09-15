/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PickedData.h"

#include "PostGraphAdaptor/PostGraphObjectPick.h"
// Qt
#include <QVector>

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Graph
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor2D.h"
#include "PostGraphAdaptor/PostGraphObjectPick.h"

// Filter ( Algorithm )
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
//#include "FITK_Interface/FITKVTKAlgorithm/FITKDataSetReader.h"

// GUI
#include "GUIPickInfo.h"

// Operator
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include <vtkUnstructuredGrid.h>

namespace GraphData
{
    PickedData::PickedData()
    {
        _resultGrid = vtkUnstructuredGrid::New();

        _graphPick = new Interface::PostGraphObjectPick(nullptr, nullptr);
        _graphPick->setPickeData(_resultGrid);
    }

    PickedData::~PickedData()
    {
        if (_resultGrid) {
            _resultGrid->Delete();
            _resultGrid = nullptr;
        }

        if (_graphPick) {
            _graphPick->removeFromGraphWidget();
            delete _graphPick;
            _graphPick = nullptr;
        }
    }

    void PickedData::insertPointID(int pointID)
    {
        _pointsID.append(pointID);
    }

    QList<int> PickedData::getPointIDs()
    {
        return _pointsID;
    }

    void PickedData::insertCellID(int cellID)
    {
        _cellsID.append(cellID);
    }

    QList<int> PickedData::getCellIDs()
    {
        return _cellsID;
    }

    vtkUnstructuredGrid * PickedData::getGrid()
    {
        return _resultGrid;
    }

    Interface::PostGraphObjectPick * PickedData::getGraphPickObject()
    {
        return _graphPick;
    }

    void PickedData::clear()
    {
        if (_resultGrid == nullptr || _graphPick == nullptr)return;
        _resultGrid->Reset();
        _graphPick->removeFromGraphWidget();
        _pointsID.clear();
        _cellsID.clear();
    }
}