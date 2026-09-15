/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAlgGlyph.h"

#include <vtkInformation.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkMaskPoints.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkPointData.h>
#include <vtkCellCenters.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>

#include <QByteArray>

namespace Interface
{
    FITKAlgGlyph * FITKAlgGlyph::New()
    {
        FITKAlgGlyph* glyphAlg = new FITKAlgGlyph();
        glyphAlg->InitializeObjectBase();
        return glyphAlg;
    }

    void FITKAlgGlyph::PrintSelf(ostream & os, vtkIndent indent)
    {
        this->Superclass::PrintSelf(os, indent);
    }

    QString FITKAlgGlyph::getVectorName()
    {
        return _vectorName;
    }

    void FITKAlgGlyph::setVectorName(QString vectorName)
    {
        _vectorName = vectorName;
    }
    int FITKAlgGlyph::getTipResolution()
    {
        return _tipResolution;
    }
    void FITKAlgGlyph::setTipResolution(int tipResolution)
    {
        _tipResolution = tipResolution;
    }

    double FITKAlgGlyph::getTipRadius()
    {
        return _tipRadius;
    }

    void FITKAlgGlyph::setTipRadius(double tipRadius)
    {
        _tipRadius = tipRadius;
    }
    double FITKAlgGlyph::getTipLength()
    {
        return _tipLength;
    }

    void FITKAlgGlyph::setTipLength(double tipLength)
    {
        _tipLength = tipLength;
    }
    int FITKAlgGlyph::getShaftResolution()
    {
        return _shaftResolution;
    }
    void FITKAlgGlyph::setShaftResolution(int shaftResolution)
    {
        _shaftResolution = shaftResolution;
    }
    double FITKAlgGlyph::getShaftRadius()
    {
        return _shaftRadius;
    }

    void FITKAlgGlyph::setShaftRadius(double shaftRadius)
    {
        _shaftRadius = shaftRadius;
    }
    double FITKAlgGlyph::getScaleFactor()
    {
        return _scaleFactor;
    }
    void FITKAlgGlyph::setScaleFactor(double scaleFactor)
    {
        _scaleFactor = scaleFactor;
    }
    bool FITKAlgGlyph::getPointDataUsed()
    {
        return _pointDataUsed;
    }
    void FITKAlgGlyph::setPointDataUsed(bool pointDataUsed)
    {
        _pointDataUsed = pointDataUsed;
    }
    int FITKAlgGlyph::getPointNum()
    {
        return _pointNum;
    }
    void FITKAlgGlyph::setPointNum(int pointNum)
    {
        _pointNum = pointNum;
    }
    int FITKAlgGlyph::getMaxNumber()
    {
        return _maxNumber;
    }
    void FITKAlgGlyph::setMaxNumber(int maxNumber)
    {
        _maxNumber = maxNumber;
    }
    int FITKAlgGlyph::getScaleMode()
    {
        return _scaleMode;
    }
    void FITKAlgGlyph::setScaleMode(int scaleMode)
    {
        _scaleMode = scaleMode;
    }

    FITKAlgGlyph::FITKAlgGlyph()
    {
        _polyData = vtkPolyData::New();
    }

    FITKAlgGlyph::~FITKAlgGlyph()
    {
        if (_polyData) {
            _polyData->Delete();
            _polyData = nullptr;
        }
    }

    int FITKAlgGlyph::FillOutputPortInformation(int port, vtkInformation * info)
    {
        Q_UNUSED(port);
        info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
        return 1;
    }

    int FITKAlgGlyph::FillInputPortInformation(int port, vtkInformation * info)
    {
        Q_UNUSED(port);
        info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
        return 1;
    }

    int FITKAlgGlyph::ProcessRequest(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
    {
        if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION())) {
            return this->RequestInformation(request, inputVector, outputVector);
        }
        if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT())) {
            return this->RequestUpdateExtent(request, inputVector, outputVector);
        }
        if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA())) {
            return this->RequestData(request, inputVector, outputVector);
        }
        return this->Superclass::ProcessRequest(request, inputVector, outputVector);
    }

    int FITKAlgGlyph::RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector * outputVector)
    {
        //获取输入输出数据
        vtkDataSet* inputData = vtkDataSet::GetData(inputVector[0]);
        vtkPolyData* outputData = vtkPolyData::GetData(outputVector);
        if (inputData == nullptr) {
            vtkErrorMacro("InputData is error");
            return 0;
        }

        if (!extractPoints(inputData))
        {
            vtkErrorMacro("Glyphing faild.");
            return 0;
        }

        vtkSmartPointer<vtkMaskPoints> maskPoint = vtkSmartPointer<vtkMaskPoints>::New();
        //计算跨行数
        int mRatio = 1;
        if (_pointNum > 20000) {
            mRatio = _pointNum / 20000;
        }
        //设置输入数据
        maskPoint->SetInputData(_polyData);
        //打开每n个点(跨行采样)
        maskPoint->SetOnRatio(mRatio);
        //设置是否开启特殊标志导致点选择的随机化
        maskPoint->RandomModeOn();
        //设置最大采样点数量
        maskPoint->SetMaximumNumberOfPoints(_maxNumber);
        maskPoint->Update();

        //箭头数据设置
        vtkSmartPointer<vtkArrowSource> arrowSource = vtkSmartPointer<vtkArrowSource>::New();
        //箭头维度
        arrowSource->SetTipResolution(_tipResolution);
        //箭头半径
        arrowSource->SetTipRadius(_tipRadius);
        //箭头长度
        arrowSource->SetTipLength(_tipLength);
        //箭柄维度
        arrowSource->SetShaftResolution(_shaftResolution);
        //箭柄半径
        arrowSource->SetShaftRadius(_shaftRadius);

        vtkSmartPointer<vtkGlyph3D> glyph3D = vtkSmartPointer<vtkGlyph3D>::New();
        glyph3D->SetSourceConnection(arrowSource->GetOutputPort());
        glyph3D->SetInputConnection(maskPoint->GetOutputPort());
        //设置模式为法线
        glyph3D->SetVectorMode(VTK_USE_NORMAL);
        //设置比例模式
        glyph3D->SetScaleMode(_scaleMode);
        //设置缩放比例
        glyph3D->SetScaleFactor(_scaleFactor);
        //打开缩放源几何
        glyph3D->ScalingOn();
        //打开输入几何体沿矢量/法线方向。
        glyph3D->OrientOn();
        glyph3D->Update();

        outputData->CopyStructure(glyph3D->GetOutput());
        outputData->GetPointData()->PassData(glyph3D->GetOutput()->GetPointData());

        return 1;
    }

    bool FITKAlgGlyph::extractPoints(vtkDataSet* inputData)
    {
        if (inputData == nullptr)return false;
        QByteArray a = _vectorName.toLocal8Bit();
        char* cName = a.data();

        if (_pointDataUsed) {
            vtkPointData* pointData = inputData->GetPointData();
            if (pointData == nullptr)return false;

            extractPointDataArray(inputData);
            _pointNum = inputData->GetNumberOfPoints();

            vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
            for (vtkIdType i = 0; i < _pointNum; i++) {
                double* p;
                p = inputData->GetPoint(i);
                points->InsertNextPoint(p);
            }
            _polyData->SetPoints(points);
            vtkDataArray* array = pointData->GetArray(cName);
            _polyData->GetPointData()->SetNormals(array);
        }
        else {
            vtkCellData* cellData = inputData->GetCellData();
            if (cellData == nullptr) return false;

            vtkSmartPointer<vtkCellCenters> cellCenters = vtkSmartPointer<vtkCellCenters>::New();
            cellCenters->SetInputData(inputData);
            //启用生成顶点单元
            cellCenters->VertexCellsOn();
            cellCenters->Update();

            extractPointDataArray(cellCenters->GetOutput());
            vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

            vtkPolyData* cPolyData = cellCenters->GetOutput();
            if (cPolyData == nullptr) return false;

            _pointNum = cPolyData->GetNumberOfCells();
            for (vtkIdType i = 0; i < _pointNum; i++) {
                double p[3];
                cPolyData->GetPoint(i, p);
                points->InsertNextPoint(p);
            }

            _polyData->SetPoints(points);
            vtkDataArray* array = cellData->GetArray(cName);
            _polyData->GetPointData()->SetNormals(array);
        }
        return true;
    }

    void FITKAlgGlyph::extractPointDataArray(vtkDataSet * inputData)
    {
        auto pointData = inputData->GetPointData();
        if (pointData == nullptr || _polyData == nullptr) return;

        const int nArr = _polyData->GetPointData()->GetNumberOfArrays();
        for (int i = 0; i < nArr; ++i) {
            _polyData->GetPointData()->RemoveArray(i);
        }

        const int nPointArray = pointData->GetNumberOfArrays();
        for (int i = 0; i < nPointArray; i++) {
            vtkDataArray* array = pointData->GetArray(i);
            _polyData->GetPointData()->AddArray(array);
        }
    }
}
