/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGridContinuityAlg.h"

#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkDataSet.h>
#include <vtkCell.h>
#include <vtkObjectFactory.h>

vtkStandardNewMacro(FITKGridContinuityAlg);

void FITKGridContinuityAlg::PrintSelf(ostream & os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

bool FITKGridContinuityAlg::getContinuity()
{
    return _isContinuity;
}

FITKGridContinuityAlg::FITKGridContinuityAlg()
{

}

FITKGridContinuityAlg::~FITKGridContinuityAlg()
{

}

int FITKGridContinuityAlg::RequestData(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    _inputData = vtkDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (_inputData == nullptr) {
        return 0;
    }

    //获取点与单元数量
    int polyDataPointNum = _inputData->GetNumberOfPoints();
    int polyDataCellNum = _inputData->GetNumberOfCells();
    if (polyDataPointNum < 1 || polyDataCellNum < 1) {
        return 0;
    }

    //记录所有点被使用状态
    _pointUseInfo.resize(polyDataPointNum);
    for (int pointID = 0; pointID < polyDataPointNum; ++pointID) {
        _pointUseInfo[pointID] = false;
    }

    //记录所有点包含的单元与单元所包含的点
    _pointCellInfo.resize(polyDataPointNum);
    _cellPointInfo.resize(polyDataCellNum);
    for (int cellID = 0; cellID < polyDataCellNum; ++cellID) {
        vtkCell* cellTemp = _inputData->GetCell(cellID);
        if (cellTemp == nullptr) {
            continue;
        }
        vtkIdList* cellPointIDs = cellTemp->GetPointIds();
        if (cellPointIDs == nullptr) {
            continue;
        }
        int pointNum = cellPointIDs->GetNumberOfIds();
        for (int pointIndex = 0; pointIndex < pointNum; pointIndex++) {
            int pointIDTemp = cellPointIDs->GetId(pointIndex);
            if (pointIDTemp >= polyDataPointNum) {
                continue;
            }
            _pointCellInfo[pointIDTemp].push_back(cellID);
            _cellPointInfo[cellID].push_back(pointIDTemp);
        }
    }

    //获取有单元的第一个节点id
    std::vector<int> pointIDs = {};
    for (int i = 0; i < _pointCellInfo.size(); ++i) {
        if (_pointCellInfo[i].size() == 0) {
            continue;
        }
        pointIDs.push_back(i);
        break;
    }

    //执行递归查询，直至所有的点都被判断过
    findConnectedPoints(pointIDs);

    //判断是否是连续的
    for (int i = 0; i < polyDataPointNum; i++) {
        if (_pointUseInfo[i] == false && _pointCellInfo[i].size() != 0) {
            _isContinuity = false;
            return 1;
        }
    }
    _isContinuity = true;

    return 1;
}

void FITKGridContinuityAlg::findConnectedPoints(std::vector<int> pointIDs)
{
    if (pointIDs.size() == 0) {
        return;
    }

    std::vector<int> wavePoints = {};
    for (int curPointID : pointIDs) {
        std::vector<int> cellIDsOfCurPoint = _pointCellInfo[curPointID];
        for (int curCellID : cellIDsOfCurPoint) {
            std::vector<int> pointIDOfCurCell = _cellPointInfo[curCellID];
            for (int pointID : pointIDOfCurCell) {
                if (_pointUseInfo[pointID] == true) {
                    continue;
                }
                _pointUseInfo[pointID] = true;
                wavePoints.push_back(pointID);
            }
        }
    }

    findConnectedPoints(wavePoints);
}
