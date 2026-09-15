/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMeshFeatureVTK.h"

// VTK
#include <vtkPolyData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkAlgorithmOutput.h>
#include <vtkIdFilter.h>
#include <vtkPassThrough.h>

// 过滤器
#include "FITK_Interface/FITKVTKAlgorithm/FITKSurfaceFilter.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKShellFeatureEdges.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKPolyDataNormals.h"

namespace Interface
{
    const char* FITKMeshFeatureVTK::GetPointIdArrayName()
    {
        return "PointIds";
    }

    const char* FITKMeshFeatureVTK::GetCellIdArrayName()
    {
        return "CellIds";
    }

    const char* FITKMeshFeatureVTK::GetFeatEdgeOriginalIdArrayName()
    {
        return "OriIds";
    }

    FITKMeshFeatureVTK::FITKMeshFeatureVTK(vtkDataSet* dataSet)
    {
        //  初始化管线。
        initialize();

        _dataSet = dataSet;
        _passThrough->SetInputData(dataSet);
    }

    FITKMeshFeatureVTK::FITKMeshFeatureVTK(vtkAlgorithmOutput* connection)
    {
        //  初始化管线。
        initialize();

        _connection = connection;
        _passThrough->SetInputConnection(connection);
    }

    FITKMeshFeatureVTK::~FITKMeshFeatureVTK()
    {
        // 手动删除所有过滤器。
        //@{
        deleteVtkObj(_normalFilter);
        deleteVtkObj(_featureEdgeFilter);
        deleteVtkObj(_edgeFilter);
        deleteVtkObj(_surfaceFilter);
        deleteVtkObj(_idFilter);
        deleteVtkObj(_passThrough);
        //@}
    }

    void FITKMeshFeatureVTK::initialize()
    {
        // 初始化。
        //@{
        // 输入传递过滤器。
        _passThrough = vtkPassThrough::New();
        _passThrough->SetAllowNullInput(true);

        // 添加单元与节点ID。
        _idFilter = vtkIdFilter::New();
        _idFilter->SetInputConnection(_passThrough->GetOutputPort());
        _idFilter->SetCellIds(true);
        _idFilter->SetPointIds(true);
        _idFilter->SetPointIdsArrayName(GetPointIdArrayName());
        _idFilter->SetCellIdsArrayName(GetCellIdArrayName());
        _idFilter->SetFieldData(true);

        // 提取表面数据过滤器。
        _surfaceFilter = FITKSurfaceFilter::New();
        _surfaceFilter->SetInputConnection(_idFilter->GetOutputPort());

        // 高阶单元划分级为0才能正常显示。
        _surfaceFilter->SetNonlinearSubdivisionLevel(0);

        // 法向提取过滤器。
        _normalFilter = FITKPolyDataNormals::New();
        _normalFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
        _normalFilter->SetComputeCellNormals(true);
        _normalFilter->SetComputePointNormals(false);

        // 特征边提取过滤器。
        _featureEdgeFilter = FITKShellFeatureEdges::New();
        _featureEdgeFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
        _featureEdgeFilter->BoundaryEdgesOn();
        _featureEdgeFilter->FeatureEdgesOn();

        // 网格边提取过滤器。
        _edgeFilter = FITKShellFeatureEdges::New();
        _edgeFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
        _edgeFilter->BoundaryEdgesOff();
        _edgeFilter->FeatureEdgesOff();
        _edgeFilter->NonManifoldEdgesOn();
        _edgeFilter->ManifoldEdgesOn();
        //@}

        // 默认关闭边线提取。
        setSurfEdgesEnable(false);
    }

    void FITKMeshFeatureVTK::update()
    {        
        if (!m_useMeshFeat)
        {
            return;
        }

        // 更新数据，提取表面用来提取法向与特征边等。
        //@{
        if (_dataSet)
        {
            _passThrough->SetInputData(_dataSet);
        }

        if (_connection)
        {
            _passThrough->SetInputConnection(_connection);
        }

        _passThrough->Update();
        _idFilter->Update();
        _surfaceFilter->Update();
        //@}

        // 更新法线、特征边。
        //@{
        _normalFilter->Update();
        _featureEdgeFilter->Update();
        //@}

        if (_dataSet)
        {
            _dataSet->Modified();
        }

        if (_connection)
        {
            _connection->Modified();
        }
    }

    void FITKMeshFeatureVTK::setEnabledFeature(bool flag)
    {
        m_useMeshFeat = flag;

        if (m_useMeshFeat)
        {
            _idFilter->SetInputConnection(_passThrough->GetOutputPort());
        }
        else
        {
            _idFilter->RemoveAllInputs();
            _idFilter->RemoveAllInputConnections(0);
        }
    }

    bool FITKMeshFeatureVTK::getEnabledFeature()
    {
        return m_useMeshFeat;
    }

    void FITKMeshFeatureVTK::reset(vtkDataSet* dataSet)
    {
        _dataSet = dataSet;
        _connection = nullptr;
        _idFilter->SetInputData(_dataSet);

        if (!m_useMeshFeat)
        {
            return;
        }

        update();
    }

    void FITKMeshFeatureVTK::reset(vtkAlgorithmOutput* connection)
    {
        _dataSet = nullptr;
        _connection = connection;
        _idFilter->SetInputConnection(_connection);

        if (!m_useMeshFeat)
        {
            return;
        }

        update();
    }

    void FITKMeshFeatureVTK::setCellAndPointIdsEnable(bool pointIdsOn, bool cellIdsOn)
    {
        if (!_idFilter)
        {
            return;
        }

        _idFilter->SetPointIds(pointIdsOn);
        _idFilter->SetCellIds(cellIdsOn);
    }

    void FITKMeshFeatureVTK::setSurfEdgesEnable(bool isOn)
    {
        if (!_edgeFilter)
        {
            return;
        }

        if (isOn && _surfaceFilter)
        {
            _edgeFilter->SetInputConnection(_surfaceFilter->GetOutputPort());
        }
        else
        {
            _edgeFilter->RemoveAllInputs();
            _edgeFilter->RemoveAllInputConnections(0);
        }
    }

    vtkPassThrough* FITKMeshFeatureVTK::getMeshPassThrough()
    {
        return _passThrough;
    }

    vtkAlgorithm* FITKMeshFeatureVTK::getRenderingFilter()
    {
        if (m_useMeshFeat)
        {
            return getSurfaceFilter();
        }
        else
        {
            return _passThrough;
        }

        return nullptr;
    }

    vtkIdFilter* FITKMeshFeatureVTK::getIdFilter()
    {
        return _idFilter;
    }

    FITKSurfaceFilter* FITKMeshFeatureVTK::getSurfaceFilter()
    {
        return _surfaceFilter;
    }

    FITKShellFeatureEdges* FITKMeshFeatureVTK::getFeatureEdgeFilter()
    {
        return _featureEdgeFilter;
    }

    FITKShellFeatureEdges* FITKMeshFeatureVTK::getShellEdgeFilter()
    {
        return _edgeFilter;
    }

    FITKPolyDataNormals* FITKMeshFeatureVTK::getNormalsFilter()
    {
        return _normalFilter;
    }

    vtkPolyData* FITKMeshFeatureVTK::getFeatureMesh(int type)
    {
        if (!m_useMeshFeat)
        {
            return nullptr;
        }

        // 1 - 表面 2 - 法向 3 - 特征边 4 - 表面网格边
        switch (type)
        {
        case 1: 
            _surfaceFilter->Update();
            return _surfaceFilter->GetOutput();
        case 2: 
            _normalFilter->Update();
            return _normalFilter->GetOutput();
        case 3: 
            _featureEdgeFilter->Update();
            return _featureEdgeFilter->GetOutput();
        case 4:
            _edgeFilter->Update();
            return _edgeFilter->GetOutput();
        default: return nullptr;
        }

        return nullptr;
    }

    vtkDataArray* FITKMeshFeatureVTK::getCellNormals()
    {
        vtkPolyData* normalMesh = getFeatureMesh(2);
        if (!normalMesh)
        {
            return nullptr;
        }
            
        vtkCellData* cellData = normalMesh->GetCellData();
        if (!cellData)
        {
            return nullptr;
        }

        return cellData->GetNormals();
    }
}
