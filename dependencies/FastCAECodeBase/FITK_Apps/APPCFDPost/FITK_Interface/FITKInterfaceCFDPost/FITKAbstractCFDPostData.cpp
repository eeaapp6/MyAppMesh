/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractCFDPostData.h"
#include "FITKCFDPostData.h"
#include "FITKCFDPost3DManager.h"
#include "FITKCFDPostColorLibaryData.h"
#include "FITKCFDPostColorLibaryDataManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include <vtkDataSetMapper.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkTextProperty.h>
#include <vtkScalarBarWidget.h>
#include <vtkLookupTable.h>
#include <vtkBorderRepresentation.h>
#include <vtkScalarBarActor.h>
#include <vtkAssembly.h>
#include <vtkActor.h>
#include <vtkFeatureEdges.h>
#include <vtkDataSetSurfaceFilter.h>

#include <QDebug>

namespace Interface {

    FITKAbstractCFDPostData::FITKAbstractCFDPostData()
    {
        _textProperty = vtkTextProperty::New();
        _textProperty->SetBold(0);
        _textProperty->SetItalic(0);
        _textProperty->SetShadow(0);
        _textProperty->SetJustification(VTK_TEXT_LEFT);
        _textProperty->SetColor(0, 0, 0);
        _textProperty->SetFontSize(14);

        _scalarBarWidget = vtkScalarBarWidget::New();
        _scalarBarWidget->GetScalarBarActor()->SetNumberOfLabels(_defaultScalarBarNum);
        _scalarBarWidget->GetScalarBarActor()->SetVerticalTitleSeparation(1);
        _scalarBarWidget->GetScalarBarActor()->SetBarRatio(0.2);
        _scalarBarWidget->GetBorderRepresentation()->SetPosition(0.90, 0.05);
        _scalarBarWidget->GetBorderRepresentation()->SetPosition2(0.08, 0.45);
        _scalarBarWidget->GetBorderRepresentation()->SetShowBorderToOff();
        _scalarBarWidget->GetScalarBarActor()->SetTitleTextProperty(_textProperty);
        _scalarBarWidget->GetScalarBarActor()->SetLabelTextProperty(_textProperty);
        _scalarBarWidget->GetScalarBarActor()->SetUnconstrainedFontSize(true);

        _lookUpTable = vtkLookupTable::New();
        _lookUpTable->SetHueRange(_defaultHue.first, _defaultHue.second);
        _lookUpTable->SetNumberOfColors(255);
        _lookUpTable->Build();
        _scalarBarWidget->GetScalarBarActor()->SetLookupTable(_lookUpTable);

        //创建默认色库
        _colorLibData = new FITKCFDPostColorLibaryData();
        QMap<double, FITKPostColorStr> colorPoints;
        FITKPostColorStr str;
        str._R = 0;
        str._G = 0;
        str._B = 1;
        colorPoints.insert(0, str);
        str._R = 0;
        str._G = 1;
        str._B = 0;
        colorPoints.insert(0.5, str);
        str._R = 1;
        str._G = 0;
        str._B = 0;
        colorPoints.insert(1, str);
        _colorLibData->setColorPoints(colorPoints);

        //判断当前是否使用色库，如果使用设置当前色库
        Interface::FITKCFDPostData* postData = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>();
        if (postData == nullptr)return;
        Interface::FITKCFDPostColorLibaryData* currentLibData = 
            postData->getPostColorLibManager()->getDataByID(postData->getPostColorLibManager()->getCurColorLibID());
        if (currentLibData != nullptr) {
            setColorData(currentLibData);
        }

        _featureEdge = vtkFeatureEdges::New();
        _featureEdge->SetFeatureAngle(60.0);
        _featureEdge->ExtractAllEdgeTypesOn();
        //_featureEdge->SetFeatureEdges(true);
        //_featureEdge->SetNonManifoldEdges(false);
        //_featureEdge->SetBoundaryEdges(true);
        //_featureEdge->SetManifoldEdges(false);

        _featureEdgeMapper = vtkDataSetMapper::New();
        _featureEdgeMapper->SetInputConnection(_featureEdge->GetOutputPort());
    }

    FITKAbstractCFDPostData::~FITKAbstractCFDPostData()
    {
        int mapperSize = _mappers.size();
        for (int i = mapperSize - 1; i >= 0; i--) {
            _mappers[i]->Delete();
        }
        _mappers.clear();

        if (_textProperty) {
            _textProperty->Delete();
            _textProperty = nullptr;
        }
        if (_scalarBarWidget) {
            _scalarBarWidget->Delete();
            _scalarBarWidget = nullptr;
        }
        if (_lookUpTable) {
            _lookUpTable->Delete();
            _lookUpTable = nullptr;
        }
        if (_featureEdge) {
            _featureEdge->Delete();
            _featureEdge = nullptr;
        }
        if (_featureEdgeMapper) {
            _featureEdgeMapper->Delete();
            _featureEdgeMapper = nullptr;
        }
        if (_colorLibData) {
            delete _colorLibData;
            _colorLibData = nullptr;
        }
    }

    void FITKAbstractCFDPostData::update()
    {
        //更新当前数据对象
        for (vtkMapper* mapper : _mappers) {
            if(mapper == nullptr)continue;
            mapper->Update();
        }
        emit sigCFDPostDataUpdate();
        //更新子数据
        Interface::FITKCFDPost3DManager* postDataManager = FITKAPP->getGlobalData()->getPostData<Interface::FITKCFDPostData>()->getPostDataManager();
        if (postDataManager == nullptr)return;
        for (int i = 0; i < postDataManager->getDataCount(); i++) {
            Interface::FITKAbstractCFDPostData* subData = postDataManager->getDataByIndex(i);
            if (subData == nullptr)continue;
            if (subData->getCFDPostParentID() != this->getDataObjectID())continue;
            subData->update();
        }
    }

    QList<vtkMapper*> FITKAbstractCFDPostData::getMappers()
    {
        return _mappers;
    }

    vtkDataSet * FITKAbstractCFDPostData::getOutput()
    {
        //虚函数，子类重写
        return nullptr;
    }

    vtkAlgorithmOutput * FITKAbstractCFDPostData::getOutputPort()
    {
        //虚函数，子类重写
        return nullptr;
    }

    int FITKAbstractCFDPostData::getCFDPostParentID()
    {
        return _CFDPostParentID;
    }

    void FITKAbstractCFDPostData::setInteractor(vtkRenderWindowInteractor * interactor)
    {
        Q_UNUSED(interactor);
    }

    void FITKAbstractCFDPostData::setScalarBarWidgetInteractor(vtkRenderWindowInteractor * interactor)
    {
        _scalarBarWidget->SetInteractor(interactor);
    }

    QStringList FITKAbstractCFDPostData::getPointDataArray()
    {
        auto dataSet = this->getOutput();
        if (dataSet == nullptr) return QStringList();
        auto pointData = dataSet->GetPointData();
        if (pointData == nullptr) return QStringList();
        const int nArray = pointData->GetNumberOfArrays();
        QStringList nameList;
        for (int i = 0; i < nArray; ++i)
        {
            const char* cName = pointData->GetArrayName(i);
            QString name = QString(cName);
            nameList.append(name);
        }
        return nameList;
    }

    QStringList FITKAbstractCFDPostData::getCellDataArray()
    {
        auto dataSet = this->getOutput();
        if (dataSet == nullptr) return QStringList();

        auto cellData = dataSet->GetCellData();
        if (cellData == nullptr) return QStringList();
        const int nArray = cellData->GetNumberOfArrays();
        QStringList nameList;
        for (int i = 0; i < nArray; ++i)
        {
            const char* cName = cellData->GetArrayName(i);
            QString name = QString(cName);
            nameList.append(name);
        }
        return nameList;
    }

    FITKPostVariableType FITKAbstractCFDPostData::getVariableType(FITKPostFieldType fieldType, QString fieldName)
    {
        FITKPostVariableType type = FITKPostVariableType::Post_Error;
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return type;
        auto bName = fieldName.toLocal8Bit();
        char* cName = bName.data();

        vtkDataArray* dataArray = nullptr;

        switch (fieldType){
        case Interface::FITKPostFieldType::Post_Point:dataArray = dataSet->GetPointData()->GetArray(cName);break;
        case Interface::FITKPostFieldType::Post_Cell:dataArray = dataSet->GetCellData()->GetArray(cName); break;
        }

        if (dataArray == nullptr)return type;
        const int cNum = dataArray->GetNumberOfComponents();

        if (cNum == 1)type = FITKPostVariableType::Post_Scalar;
        else if (cNum == 3)type = FITKPostVariableType::Post_Vector;
        else if (cNum >= 4)type = FITKPostVariableType::Post_Tensor;
        return type;
    }

    int FITKAbstractCFDPostData::getVariableComponentCount(FITKPostFieldType fieldType, QString fieldName)
    {
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return 0;
        auto bName = fieldName.toLocal8Bit();
        char* cName = bName.data();

        vtkDataArray* dataArray = nullptr;
        switch (fieldType) {
        case Interface::FITKPostFieldType::Post_Point:dataArray = dataSet->GetPointData()->GetArray(cName); break;
        case Interface::FITKPostFieldType::Post_Cell:dataArray = dataSet->GetCellData()->GetArray(cName); break;
        }
        if (dataArray == nullptr)return 0;
        return dataArray->GetNumberOfComponents();
    }

    vtkScalarBarWidget * FITKAbstractCFDPostData::getScalarBarWidget()
    {
        return _scalarBarWidget;
    }

    int FITKAbstractCFDPostData::getScalaraBarColorNum()
    {
        if (_scalarBarWidget == nullptr)return 0;
        vtkScalarBarActor* scalarActor = _scalarBarWidget->GetScalarBarActor();
        if (scalarActor == nullptr)return 0;
        return scalarActor->GetNumberOfLabels();
    }

    void FITKAbstractCFDPostData::setScalarBarColorNum(int count)
    {
        if (_scalarBarWidget == nullptr)return;
        vtkScalarBarActor* scalarActor = _scalarBarWidget->GetScalarBarActor();
        if (scalarActor == nullptr)return;
        if (count == scalarActor->GetNumberOfLabels())return;
        scalarActor->SetNumberOfLabels(count);
    }

    void FITKAbstractCFDPostData::setScalarColorRange(double minRange, double maxRange)
    {
        for (vtkMapper* mapper : _mappers) {
            if (mapper == nullptr)continue;
            mapper->SetScalarRange(minRange, maxRange);
        }
    }

    void FITKAbstractCFDPostData::getScalarColorRange(double & minRange, double & maxRange)
    {
        for (vtkMapper* mapper : _mappers) {
            if (mapper == nullptr)continue;
            double* range = mapper->GetScalarRange();
            if (range[0] < minRange) minRange = range[0];
            if (range[1] > maxRange) maxRange = range[1];
        }
    }

    double FITKAbstractCFDPostData::getFieldValue(int id, FITKPostFieldType type, QString fieldName)
    {
        vtkDataSet* data = getOutput();
        if (data == nullptr) return 0.0;
        QByteArray name = fieldName.toLocal8Bit();
        char* cName = name.data();

        vtkDataArray* dataArray = nullptr;
        switch (type) {
        case Interface::FITKPostFieldType::Post_None: return 0.0;
        case Interface::FITKPostFieldType::Post_Point:dataArray = data->GetPointData()->GetArray(cName); break;
        case Interface::FITKPostFieldType::Post_Cell:dataArray = data->GetCellData()->GetArray(cName); break;
        }
        if (dataArray == nullptr) return 0.0;
        
        return dataArray->GetTuple1(id);
    }

    void FITKAbstractCFDPostData::getRange(double* range, FITKPostFieldType type, QString fieldName, int componentIndex)
    {
        vtkDataSet* data = getOutput();
        if (data == nullptr) return;
        QByteArray name = fieldName.toLocal8Bit();
        char* cName = name.data();
        vtkDataArray* dataArray = nullptr;

        switch (type) {
        case Interface::FITKPostFieldType::Post_None: break;
        case Interface::FITKPostFieldType::Post_Point:dataArray = data->GetPointData()->GetArray(cName); break;
        case Interface::FITKPostFieldType::Post_Cell:dataArray = data->GetCellData()->GetArray(cName); break;
        }
        if (dataArray == nullptr) return;
        dataArray->GetRange(range, componentIndex);
    }

    void FITKAbstractCFDPostData::getExtremum(double * range, double * point, FITKPostFieldType type, QString fieldName, int componentIndex)
    {
        //获取数据极值
        this->getRange(range, type, fieldName);

        //获取对应数据的数据组
        vtkDataSet* data = getOutput();
        if (data == nullptr) return;
        QByteArray name = fieldName.toLocal8Bit();
        char* cName = name.data();
        vtkDataArray* dataArray = nullptr;

        switch (type) {
        case Interface::FITKPostFieldType::Post_Point: {
            dataArray = data->GetPointData()->GetArray(cName);
            break;
        }
        case Interface::FITKPostFieldType::Post_Cell: {
            dataArray = data->GetCellData()->GetArray(cName);
            break;
        }
        }
        if (dataArray == nullptr) return;

        //获取数据极值点
        for (int i = 0; i < dataArray->GetNumberOfTuples(); i++) {
            double filedValue = dataArray->GetComponent(i, componentIndex);

            if (filedValue == range[0]) {
                double* minPoint = dataArray->GetTuple(i);
                point[0] = minPoint[0];
                point[1] = minPoint[1];
                point[2] = minPoint[2];
            }

            if (filedValue == range[1]) {
                double* maxPoint = dataArray->GetTuple(i);
                point[3] = maxPoint[0];
                point[4] = maxPoint[1];
                point[5] = maxPoint[2];
            }
        }
    }

    void FITKAbstractCFDPostData::setHueRange(double maxColor, double minColor)
    {
        if (_lookUpTable == nullptr)return;
        double* hueRange = _lookUpTable->GetHueRange();
        if ((hueRange[0] == maxColor) && (hueRange[1] == minColor))return;
        _lookUpTable->SetHueRange(maxColor, minColor);
    }

    void FITKAbstractCFDPostData::getHueRange(double* range)
    {
        if (_lookUpTable == nullptr)return;
        double* hueRange = _lookUpTable->GetHueRange();
        range = hueRange;
    }

    int FITKAbstractCFDPostData::getPointNumber()
    {
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return 0;
        int num = dataSet->GetNumberOfPoints();
        return num;
    }

    int FITKAbstractCFDPostData::getCellNumber()
    {
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return 0;
        int num = dataSet->GetNumberOfCells();
        return num;
    }

    FITKPostGridType FITKAbstractCFDPostData::getGridType()
    {
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return FITKPostGridType::Post_None;
        int vtkType = dataSet->GetDataObjectType();
        if (vtkType == 2)return FITKPostGridType::Post_Structured;
        else if (vtkType == 4)return FITKPostGridType::Post_UnStructured;
        return FITKPostGridType::Post_None;
    }

    void FITKAbstractCFDPostData::getBounds(double* bounds)
    {
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return;
        dataSet->GetBounds(bounds);
    }

    void FITKAbstractCFDPostData::getOrigin(double* origin)
    {
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return;
        dataSet->GetCenter(origin);
    }

    QHash<int, int> FITKAbstractCFDPostData::getGridCellType()
    {
        QHash<int, int> gridCellType = {};
        vtkDataSet* dataSet = getOutput();
        if (dataSet == nullptr)return gridCellType;
        int cellNum = dataSet->GetNumberOfCells();
        for (int i = 0; i < cellNum; i++) {
            QList<int> gridCellKeys = gridCellType.keys();
            int cellType = dataSet->GetCellType(i);
            if (gridCellType.contains(cellType)) {
                int num = gridCellType.value(cellType);
                num++;
                gridCellType.insert(cellType, num);
            }
            else {
                int num = 1;
                gridCellType.insert(cellType, num);
            }
        }
        return gridCellType;
    }

    void FITKAbstractCFDPostData::updateColor()
    {
        if (_colorLibData == nullptr)return;
        for (double i = 0; i < 255; i++) {
            double pos = i / 255;
            QColor color = _colorLibData->getColor(pos);
            _lookUpTable->SetTableValue(i, color.redF(), color.greenF(), color.blueF());
        }
        _lookUpTable->Build();
    }

    void FITKAbstractCFDPostData::setColorData(Interface::FITKCFDPostColorLibaryData* colorData)
    {
        if (colorData == nullptr)return;
        _colorLibData->copy(colorData);
        updateColor();
    }

    Interface::FITKCFDPostColorLibaryData * FITKAbstractCFDPostData::getColorData()
    {
        return _colorLibData;
    }

    vtkDataSetMapper * FITKAbstractCFDPostData::getFeatureEdgeMapper()
    {
        return _featureEdgeMapper;
    }

    void FITKAbstractCFDPostData::initFeatureEdge()
    {
        if (_featureEdge == nullptr)
        {
            return;
        }

        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
        surfaceFilter->SetInputConnection(getOutputPort());
        
        _featureEdge->SetInputConnection(surfaceFilter->GetOutputPort());
    }
}