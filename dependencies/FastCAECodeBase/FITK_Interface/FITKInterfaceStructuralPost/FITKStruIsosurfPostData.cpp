/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStruIsosurfPostData.h"
#include "FITKStruPost3DManager.h"
#include "FITKAbstractStruPostVTK.h"

#include <vtkContourFilter.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkLookupTable.h>
#include <vtkTrivialProducer.h>



// DATA
namespace Interface {
    QString FITKStruIsosurfPostData::_currentField = "";

    QHash<QString, QPair<double, double>> FITKStruIsosurfPostData::_nameRange = {};

    FITKStruIsosurfPostData::FITKStruIsosurfPostData()
    {
        // 初始化为不显示
        enable(false);
        _contourFilter = vtkContourFilter::New();
        _contourFilter->ComputeNormalsOn();      // 计算法向量（用于光照）
        _contourFilter->ComputeScalarsOn();      // 保留标量值
        _contourFilter->ComputeGradientsOn();   // 不计算梯度（需要时再开启）
        _contourFilter->SetGenerateTriangles(true); // 生成三角形
    }

    FITKStruIsosurfPostData::~FITKStruIsosurfPostData() {
        if (_contourFilter) {
            _contourFilter->Delete();
            _contourFilter = nullptr;
        }
    }

    void FITKStruIsosurfPostData::setFileName(const QString fileName) {
        _fileName = fileName;
    }

    QString FITKStruIsosurfPostData::getFileName() {
        return _fileName;
    }

    void FITKStruIsosurfPostData::setType(const VariableLocation type) {
        _fileType = type;
    }

    VariableLocation FITKStruIsosurfPostData::getType() {
        return _fileType;
    }

    void FITKStruIsosurfPostData::setInput(vtkDataSet* inp)
    {
        _contourFilter->SetInputData(inp);

        std::string stringName = _fileName.toStdString();
        const char* scalarName = stringName.c_str();
        vtkPointData* pointData = inp->GetPointData();
        if (pointData == nullptr) {
            return;
        }
        _contourFilter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, scalarName);
    }

    void FITKStruIsosurfPostData::setValues(QList<double> values) {
        _values = values;
        for (int i = 0; i < _values.size(); ++i) {
            _contourFilter->SetValue(i, _values[i]);
        }
    }

    vtkDataSet* FITKStruIsosurfPostData::getOutput() {
        update();
        return vtkDataSet::SafeDownCast(_contourFilter->GetOutput());
    }

    vtkAlgorithmOutput* FITKStruIsosurfPostData::getOutputAlgorithm() {
        if (_contourFilter == nullptr) {
            return nullptr;
        }
        return _contourFilter->GetOutputPort();
    }

    StruPostType FITKStruIsosurfPostData::getPostStructualType() {
        return StruPostType::SPTDynamicExplict;
    }

    void FITKStruIsosurfPostData::setModelID(int modelId) {
        _modelID = modelId;
    }

    int FITKStruIsosurfPostData::getModelID() {
        return _modelID;
    }

    void FITKStruIsosurfPostData::update() {
        _contourFilter->Update();
    }
    vtkContourFilter * FITKStruIsosurfPostData::getContourFilter()
    {
        return _contourFilter;
    }
    QList<double> FITKStruIsosurfPostData::getValues()
    {
        return _values;
    }
    // 静态成员函数实现
    FITKInterfaceStructuralPostAPI QString FITKStruIsosurfPostData::getCurrentField()
    {
        return _currentField;
    }
    // 静态成员函数实现
    FITKInterfaceStructuralPostAPI void FITKStruIsosurfPostData::setCurrentField(QString field)
    {
        _currentField = field;
    }
    // 静态成员函数实现
    FITKInterfaceStructuralPostAPI void FITKStruIsosurfPostData::addNameRange(QString name, QPair<double, double> range)
    {
        _nameRange[name] = range;
    }
    QPair<double, double> FITKStruIsosurfPostData::getRangeByName(QString name)
    {
        return _nameRange[name];
    }
    bool FITKStruIsosurfPostData::getIsRemoved()
    {
        return _isRemoved;
    }
    void FITKStruIsosurfPostData::setIsRemoved(bool isRemoved)
    {
        _isRemoved = isRemoved;
    }
}

// DATA MANAGER

namespace Interface {
    FITKStruIsoPostDataManager::FITKStruIsoPostDataManager(FITKAbstractStructuralPostVTK* absPost) : _postObj(absPost) {

    }

    void FITKStruIsoPostDataManager::appendDataObj(Core::FITKAbstractDataObject* item) {
        FITKStruIsosurfPostData* v = dynamic_cast<FITKStruIsosurfPostData*>(item);
        if (v == nullptr || _postObj == nullptr) {
            return;
        }
        auto def = _postObj->getOutput();
        if (def == nullptr) {
            return;
        }
        v->setModelID(_postObj->getDataObjectID());
        v->setInput(def);
        Core::FITKAbstractDataManager<FITKStruIsosurfPostData>::appendDataObj(item);
    }

    void FITKStruIsoPostDataManager::insertDataObj(int idx, Core::FITKAbstractDataObject* item) {
        FITKStruIsosurfPostData* v = dynamic_cast<FITKStruIsosurfPostData*>(item);
        if (v == nullptr || _postObj == nullptr) {
            return;
        }
        auto def = _postObj->getOutput();
        if (def == nullptr) {
            return;
        }
        v->setModelID(_postObj->getDataObjectID());
        v->setInput(def);
        Core::FITKAbstractDataManager<FITKStruIsosurfPostData>::insertDataObj(idx, item);
    }
}
