/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKUnstructuredFluidMeshVTK.h"
#include "FITKUnstructuredMeshVTK.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITKMeshVTKMap.h"
#include <vtkUnstructuredGrid.h>

namespace Interface
{
    FITKUnstructuredFluidMeshVTK::FITKUnstructuredFluidMeshVTK()
    {
        //创建区域网格
        FITKFluidRegionsMesh* regionMesh = new FITKFluidRegionsMesh;
        this->appendDataObj(regionMesh);
    }

    FITKUnstructuredFluidMeshVTK::~FITKUnstructuredFluidMeshVTK()
    {
        //释放对象
    }

    FITKUnstructuredMeshVTK* FITKUnstructuredFluidMeshVTK::getFieldMesh(int index)
    {
        if (this->getDataCount() <= index || index < 0)
            return nullptr;
        if (!this->getDataByIndex(index))
            return nullptr;
        return this->getDataByIndex(index)->getFieldMesh();
    }

    FITKBoundaryMeshVTKManager* FITKUnstructuredFluidMeshVTK::getBoundaryMeshManager(int index)
    {
        if (this->getDataCount() <= index || index < 0)
            return nullptr;
        if (!this->getDataByIndex(index))
            return nullptr;
        return this->getDataByIndex(index)->getBoundaryMeshManager();
    }

    void FITKUnstructuredFluidMeshVTK::clearMesh()
    {
        this->clear();
        //创建区域网格
        FITKFluidRegionsMesh* regionMesh = new FITKFluidRegionsMesh;
        this->appendDataObj(regionMesh);
    }

    FITKBoundaryMeshVTK::FITKBoundaryMeshVTK(FITKUnstructuredMeshVTK* fieldmesh)
        :_fieldMesh(fieldmesh)
    {
        //获取节点
        _vtkDataSet = vtkUnstructuredGrid::New();
        if (_fieldMesh == nullptr) return;
        vtkUnstructuredGrid* fg = _fieldMesh->getGrid();
        if (fg == nullptr) return;
        //节点共用
        _vtkDataSet->SetPoints(fg->GetPoints());
    }

    void FITKBoundaryMeshVTK::appendElement(FITKAbstractElement* element)
    {
        if (element == nullptr || _fieldMesh == nullptr) return;
        Interface::FITKModelEnum::FITKEleType type = element->getEleType();
        if (!eleTypeHash.contains(type)) return;
        //追加到单元中
        FITKElementList::appendElement(element);

        int count = element->getNodeCount();
        vtkIdList* idList = vtkIdList::New();
        for (int i = 0; i < count; ++i)
        {
            //查找节点索引
            const int nodeID = element->getNodeID(i);
            const int index = _fieldMesh->getNodeIndexByID(nodeID);
            idList->InsertNextId(index);
        }
        _vtkDataSet->InsertNextCell(eleTypeHash[type], idList);
    }

    vtkUnstructuredGrid* FITKBoundaryMeshVTK::getGrid()
    {
        return _vtkDataSet;
    }

    FITKBoundaryMeshVTK::~FITKBoundaryMeshVTK()
    {
        if (_vtkDataSet != nullptr) _vtkDataSet->Delete();
    }

    FITKFluidRegionsMesh::FITKFluidRegionsMesh()
    {
        //初始化区域网格
        _fieldMesh = new FITKUnstructuredMeshVTK;
        _boundaryMeshManager = new FITKBoundaryMeshVTKManager;
    }

    FITKFluidRegionsMesh::~FITKFluidRegionsMesh()
    {
        if (_fieldMesh) delete _fieldMesh;
        if (_boundaryMeshManager) delete _boundaryMeshManager;
    }

    FITKUnstructuredMeshVTK * FITKFluidRegionsMesh::getFieldMesh()
    {
        return _fieldMesh;
    }

    FITKBoundaryMeshVTKManager * FITKFluidRegionsMesh::getBoundaryMeshManager()
    {
        return _boundaryMeshManager;
    }

}


