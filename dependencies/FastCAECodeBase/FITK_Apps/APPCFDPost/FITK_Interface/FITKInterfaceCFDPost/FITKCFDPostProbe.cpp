/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostProbe.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkProbeFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkQuad.h>
#include <vtkIdList.h>
#include <vtkCell.h>
#include <vtkExtractSelection.h>
#include <vtkSelection.h>
#include <vtkLookupTable.h>

#include <QDebug>

namespace Interface
{
    Interface::FITKCFDPostProbe::FITKCFDPostProbe(int parentID)
    {
        _CFDPostParentID = parentID;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        _gridData = vtkUnstructuredGrid::New();

        _probeFilter = vtkProbeFilter::New();
        _probeFilter->SetInputData(0, _gridData);
        _probeFilter->SetInputData(1, parentData->getOutput());

        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);
        mapper->SetInputConnection(_probeFilter->GetOutputPort());
        _mappers.append(mapper);
    }

    FITKCFDPostProbe::~FITKCFDPostProbe()
    {
        if (_gridData) {
            _gridData->Delete();
            _gridData = nullptr;
        }

        if (_probeFilter) {
            _probeFilter->Delete();
            _probeFilter = nullptr;
        }

        reset();
    }
    FITKPostDataType FITKCFDPostProbe::getPostDataType()
    {
        return FITKPostDataType::Post_Probe;
    }

    vtkDataSet* FITKCFDPostProbe::getOutput()
    {
        if (_probeFilter == nullptr)return nullptr;
        return _probeFilter->GetOutput();
    }

    vtkAlgorithmOutput* FITKCFDPostProbe::getOutputPort()
    {
        if (_probeFilter == nullptr)return nullptr;
        return _probeFilter->GetOutputPort();
    }

    void FITKCFDPostProbe::update()
    {
        appendCellToSource();
        appendPointToSource();
        FITKAbstractCFDPostData::update();
    }

    void FITKCFDPostProbe::appendPoint(double* point, int pointID)
    {
        QPair<int, double[3]> pointM = {};
        pointM.first = pointID;
        pointM.second[0] = point[0];
        pointM.second[1] = point[1];
        pointM.second[2] = point[2];
        _points.append(pointM);
    }

    void FITKCFDPostProbe::appendCell(int cellID)
    {
        if (cellID < 0)return;
        _cells.append(cellID);
    }

    int FITKCFDPostProbe::getPointCount()
    {
        return _points.size();
    }

    int FITKCFDPostProbe::getPointAtInter(double* point, int index)
    {
        if (index < 0 || index >= getPointCount())return -1;
        QPair<int, double[3]> pointM = _points[index];
        point[0] = pointM.second[0];
        point[1] = pointM.second[1];
        point[2] = pointM.second[2];

        return pointM.first;
    }

    QList<int> FITKCFDPostProbe::getCellAll()
    {
        return _cells;
    }

    void FITKCFDPostProbe::reset()
    {
        if (_gridData) {
            _gridData->Initialize();
        }
        _points.clear();
        _cells.clear();
    }

    void FITKCFDPostProbe::appendPointToSource()
    {
        if (_gridData == nullptr)return;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;

        vtkPoints* points = _gridData->GetPoints();
        if (points == nullptr) {
            vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
            _gridData->SetPoints(points);
            points = _gridData->GetPoints();
        }

        for (auto p : _points) {
            if (p.first == -1) {
                _gridData->GetPoints()->InsertNextPoint(p.second);
            }
            else {
                double* point = parentData->getOutput()->GetPoint(p.first);
                _gridData->GetPoints()->InsertNextPoint(point);
            }
        }
    }

    void FITKCFDPostProbe::appendCellToSource()
    {
        if (_gridData == nullptr)return;
        Interface::FITKCFDPost3DManager* dataManger = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (dataManger == nullptr)return;
        Interface::FITKAbstractCFDPostData* parentData = dataManger->getDataByID(_CFDPostParentID);
        if (parentData == nullptr)return;
        //创建提取器
        vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
        vtkSmartPointer<vtkSelectionNode> selectNode = vtkSmartPointer<vtkSelectionNode>::New();
        vtkSmartPointer<vtkSelection> section = vtkSmartPointer<vtkSelection>::New();
        //设置通过索引来选择
        selectNode->SetContentType(vtkSelectionNode::INDICES);
        //设置选择类型
        selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        section->AddNode(selectNode);
        extractSelection->SetInputConnection(0, parentData->getOutputPort());
        extractSelection->SetInputData(1, section);
        vtkSmartPointer<vtkIdTypeArray> idArray = vtkSmartPointer<vtkIdTypeArray>::New();
        selectNode->SetSelectionList(idArray);

        selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        idArray->SetNumberOfComponents(_cells.size());
        for (int cellID : _cells) {
            idArray->InsertNextValue(cellID);
        }
        extractSelection->Update();
        vtkUnstructuredGrid* grid = vtkUnstructuredGrid::SafeDownCast(extractSelection->GetOutput());
        if (grid == nullptr)return;
        _gridData->DeepCopy(grid);
    }
}

