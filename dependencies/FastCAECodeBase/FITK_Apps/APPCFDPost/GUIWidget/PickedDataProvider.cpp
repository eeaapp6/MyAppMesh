/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PickedDataProvider.h"
#include "GUIPickInfo.h"
#include "PickedData.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKRunTimeSetting.h"

#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkExtractSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkAlgorithmOutput.h>
#include <vtkMapper.h>

#include <QDebug>

namespace GraphData
{
    PickedDataProvider* PickedDataProvider::_instance = nullptr;

    PickedDataProvider * PickedDataProvider::getInstance()
    {
        if (_instance == nullptr) {
            _instance = new PickedDataProvider();
        }
        return _instance;
    }

    void PickedDataProvider::clearAll()
    {
        clearPickedDataPreview();
        clearPickedData();
    }

    void PickedDataProvider::clearPickedData()
    {
        _pickedData->clear();
    }

    void PickedDataProvider::clearPickedDataPreview()
    {
        _pickedPreviewData->clear();
    }

    bool PickedDataProvider::setPreviewPicked(vtkActor* actor, int index, double * pickedWorldPos)
    {
        if (actor == nullptr || _pickedPreviewData == nullptr)return false;
        vtkDataSet* dataSet = actor->GetMapper()->GetInput();
        if (dataSet == nullptr)return false;

        auto RTsetting = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (RTsetting == nullptr) return false;
        // 设置运行时设置中的变量值
        GUI::GUIPickInfo::PickObjType objType = RTsetting->getValue("PickObjType").value<GUI::GUIPickInfo::PickObjType>();

        //清空预选数据
        clearPickedDataPreview();

        //创建提取器
        vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
        vtkSmartPointer<vtkSelectionNode> selectNode = vtkSmartPointer<vtkSelectionNode>::New();
        vtkSmartPointer<vtkSelection> section = vtkSmartPointer<vtkSelection>::New();
        //设置通过索引来选择
        selectNode->SetContentType(vtkSelectionNode::INDICES);
        //设置选择类型
        selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        section->AddNode(selectNode);
        extractSelection->SetInputConnection(0, actor->GetMapper()->GetInputConnection(0, 0));
        extractSelection->SetInputData(1, section);
        vtkSmartPointer<vtkIdTypeArray> idArray = vtkSmartPointer<vtkIdTypeArray>::New();
        selectNode->SetSelectionList(idArray);

        //数据提取
        switch (objType){
        case GUI::GUIPickInfo::POBJNone:return false;
        case GUI::GUIPickInfo::POBJVert: {
            selectNode->SetFieldType(vtkSelectionNode::SelectionField::POINT);
            idArray->SetNumberOfComponents(1);
            idArray->InsertNextValue(index);
            _pickedPreviewData->insertPointID(index);
            break;
        }
        case GUI::GUIPickInfo::POBJEdge: {
            selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
            idArray->SetNumberOfComponents(1);
            idArray->InsertNextValue(index);
            _pickedPreviewData->insertCellID(index);
            break;
        }
        case GUI::GUIPickInfo::POBJFace:
            break;
        case GUI::GUIPickInfo::POBJSolid:
            break;
        }
        extractSelection->Update();
        vtkUnstructuredGrid* resurtGrid = vtkUnstructuredGrid::SafeDownCast(extractSelection->GetOutput());
        if (resurtGrid == nullptr)return false;
        _pickedPreviewData->getGrid()->DeepCopy(resurtGrid);
        //qDebug() << resurtGrid->GetNumberOfPoints();
        //qDebug() << resurtGrid->GetNumberOfCells();
        //qDebug() << idArray->GetNumberOfValues();
        //qDebug() << "PreviewID  = " << index;
        //qDebug() << resurtGrid->GetPoint(0)[0];
        //qDebug() << resurtGrid->GetPoint(0)[1];
        //qDebug() << resurtGrid->GetPoint(0)[2];
        return true;
    }

    void PickedDataProvider::addPicked(vtkActor* actor, int index, double* pickedWorldPos)
    {
        if (actor == nullptr)return;
        vtkDataSet* dataSet = actor->GetMapper()->GetInput();
        if (dataSet == nullptr)return;

        auto RTsetting = FITKAPP->getGlobalData()->getRunTimeSetting();
        if (RTsetting == nullptr) return;
        // 设置运行时设置中的变量值
        GUI::GUIPickInfo::PickObjType objType = RTsetting->getValue("PickObjType").value<GUI::GUIPickInfo::PickObjType>();

        //清空之前的拾取数据
        clearPickedData();

        //创建提取器
        vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
        vtkSmartPointer<vtkSelectionNode> selectNode = vtkSmartPointer<vtkSelectionNode>::New();
        vtkSmartPointer<vtkSelection> section = vtkSmartPointer<vtkSelection>::New();
        //设置通过索引来选择
        selectNode->SetContentType(vtkSelectionNode::INDICES);
        //设置选择类型
        selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
        section->AddNode(selectNode);
        extractSelection->SetInputConnection(0, actor->GetMapper()->GetInputConnection(0, 0));
        extractSelection->SetInputData(1, section);
        vtkSmartPointer<vtkIdTypeArray> idArray = vtkSmartPointer<vtkIdTypeArray>::New();
        selectNode->SetSelectionList(idArray);

        switch (objType) {
        case GUI::GUIPickInfo::POBJNone:return;
        case GUI::GUIPickInfo::POBJVert: {
            selectNode->SetFieldType(vtkSelectionNode::SelectionField::POINT);
            idArray->SetNumberOfComponents(1);
            idArray->InsertNextValue(index);
            _pickedData->insertPointID(index);
            break;
        }
        case GUI::GUIPickInfo::POBJEdge:
            selectNode->SetFieldType(vtkSelectionNode::SelectionField::CELL);
            idArray->SetNumberOfComponents(1);
            idArray->InsertNextValue(index);
            _pickedData->insertCellID(index);
            break;
        case GUI::GUIPickInfo::POBJFace:
            break;
        case GUI::GUIPickInfo::POBJSolid:
            break;
        }
        extractSelection->Update();
        vtkUnstructuredGrid* resurtGrid = vtkUnstructuredGrid::SafeDownCast(extractSelection->GetOutput());
        if (resurtGrid == nullptr)return;
        _pickedData->getGrid()->DeepCopy(resurtGrid);
        //qDebug() << "PickID  = " << index;
        emit sigPickDataOver();
    }

    PickedData * PickedDataProvider::getPickedPreview()
    {
        return _pickedPreviewData;
    }

    PickedData* PickedDataProvider::getPickDatas()
    {
        return _pickedData;
    }

    PickedDataProvider::PickedDataProvider()
    {
        _pickedData = new PickedData();
        _pickedPreviewData = new PickedData();
    }

    PickedDataProvider::~PickedDataProvider()
    {
        if (_pickedPreviewData) {
            delete _pickedPreviewData;
            _pickedPreviewData = nullptr;
        }

        if (_pickedData) {
            delete _pickedData;
            _pickedData = nullptr;
        }
    }
}

