/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCompareTool.h"

#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCell.h>
#include <vtkDataArray.h>

bool FITKCompareTool::CompareVTKData(vtkDataSet * data1, vtkDataSet * data2)
{
    //对比点数据
    if (ComparePoints(data1, data2) == false) {
        return false;
    }

    //对比单元数据
    if (CompareCells(data1, data2) == false) {
        return false;
    }

    return true;
}

bool FITKCompareTool::ComparePoints(vtkDataSet* data1, vtkDataSet* data2) {
    if (data1->GetNumberOfPoints() != data2->GetNumberOfPoints()) {
        return false;
    }
    for (vtkIdType i = 0; i < data1->GetNumberOfPoints(); ++i) {
        double p1[3], p2[3];
        data1->GetPoint(i, p1);
        data2->GetPoint(i, p2);
        if (std::abs(p1[0] - p2[0]) > 1e-6 ||
            std::abs(p1[1] - p2[1]) > 1e-6 ||
            std::abs(p1[2] - p2[2]) > 1e-6) {
            return false;
        }
    }
    return true;
}

bool FITKCompareTool::CompareCells(vtkDataSet* data1, vtkDataSet* data2) {
    if (data1->GetNumberOfCells() != data2->GetNumberOfCells()) {
        return false;
    }
    for (vtkIdType i = 0; i < data1->GetNumberOfCells(); ++i) {
        vtkCell* cell1 = data1->GetCell(i);
        vtkCell* cell2 = data2->GetCell(i);
        if (cell1->GetCellType() != cell2->GetCellType() ||
            cell1->GetNumberOfPoints() != cell2->GetNumberOfPoints()) {
            return false;
        }
        for (vtkIdType j = 0; j < cell1->GetNumberOfPoints(); ++j) {
            if (cell1->GetPointId(j) != cell2->GetPointId(j)) {
                return false;
            }
        }
    }
    return true;
}