/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKStruThresholdPostData.h"

#include <vtkThreshold.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>

namespace Interface {
    // 初始化静态成员
    QString FITKStruThresholdPostData::_currentField = "";
    QHash<QString, QPair<double, double>> FITKStruThresholdPostData::_nameRange = {};

    FITKStruThresholdPostData::FITKStruThresholdPostData()
    {
        _thresholdFilter = vtkThreshold::New();
        _thresholdFilter->SetAllScalars(0);// 只要有一个点符合就保留该Cell，更符合常规物理切分效果
        _thresholdFilter->SetLowerThreshold(-1e308);
        _thresholdFilter->SetUpperThreshold(1e308);
        _thresholdFilter->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
    }

    FITKStruThresholdPostData::~FITKStruThresholdPostData()
    {
        if (_thresholdFilter) {
            _thresholdFilter->Delete();
            _thresholdFilter = nullptr;
        }
    }
    void FITKStruThresholdPostData::setFileName(const QString fileName)
    {
        _fileName = fileName;
    }
    QString FITKStruThresholdPostData::getFileName()
    {
        return _fileName;
    }
    void FITKStruThresholdPostData::setType(const VariableLocation type)
    {
        _fileType = type;
    }
    VariableLocation FITKStruThresholdPostData::getType()
    {
        return _fileType;
    }
    void FITKStruThresholdPostData::setThresholdRange(const double minValue, const double maxValue)
    {
        _thresholdFilter->SetLowerThreshold(minValue);
        _thresholdFilter->SetUpperThreshold(maxValue);
    }
    QPair<double, double> FITKStruThresholdPostData::getThresholdRange()
    {
        double range[2];
        range[0] = _thresholdFilter->GetLowerThreshold();
        range[1] = _thresholdFilter->GetUpperThreshold();
        return QPair<double, double>(range[0], range[1]);
    }
    void FITKStruThresholdPostData::setThresholdFunction(int func)
    {
        _thresholdFilter->SetThresholdFunction(func);
    }
    int FITKStruThresholdPostData::getThresholdFunction()
    {
        return _thresholdFilter->GetThresholdFunction();
    }
    vtkDataSet* FITKStruThresholdPostData::getOutput()
    {
        update();
        return vtkDataSet::SafeDownCast(_thresholdFilter->GetOutput());
    }
    vtkAlgorithmOutput* FITKStruThresholdPostData::getOutputAlgorithm()
    {
        if (_thresholdFilter == nullptr)
        {
            return nullptr;
        }
        return _thresholdFilter->GetOutputPort();
    }
    vtkAlgorithm* FITKStruThresholdPostData::getAlgorithm()
    {
        return _thresholdFilter;
    }
    void FITKStruThresholdPostData::setPostStructualType(const StruPostType type)
    {
        _postType = type;
    }
    StruPostType FITKStruThresholdPostData::getPostStructualType()
    {
        return _postType;
    }
    void FITKStruThresholdPostData::setInput(vtkDataSet* inp)
    {
        _thresholdFilter->SetInputData(inp);

        if(_fileName.isEmpty())
        {
            return;
        }
        std::string stringName = _fileName.toStdString();
        const char* scalarName = stringName.c_str();

        int association = vtkDataObject::FIELD_ASSOCIATION_POINTS;
        if (_fileType == VariableLocation::VALOCEle)
        {
            association = vtkDataObject::FIELD_ASSOCIATION_CELLS;
        }

        _thresholdFilter->SetInputArrayToProcess(0, 0, 0, association, scalarName);
    }
    void FITKStruThresholdPostData::setModelID(int modelId)
    {
        _modelID = modelId;
    }
    int FITKStruThresholdPostData::getModelID()
    {
        return _modelID;
    }
    void FITKStruThresholdPostData::update()
    {
        if (!_thresholdFilter->GetNumberOfInputConnections(0)
            && !_thresholdFilter->GetInputDataObject(0, 0))
        {
            return;
        }

        _thresholdFilter->Update();
    }
    vtkThreshold* FITKStruThresholdPostData::getThresholdFilter()
    {
        return _thresholdFilter;
    }

    // 静态成员函数实现
    FITKInterfaceStructuralPostAPI QString FITKStruThresholdPostData::getCurrentField()
    {
        return _currentField;
    }

    FITKInterfaceStructuralPostAPI void FITKStruThresholdPostData::setCurrentField(QString field)
    {
        _currentField = field;
    }

    FITKInterfaceStructuralPostAPI void FITKStruThresholdPostData::addNameRange(QString name, QPair<double, double> range)
    {
        _nameRange[name] = range;
    }

    QPair<double, double> FITKStruThresholdPostData::getRangeByName(QString name)
    {
        return _nameRange.value(name, QPair<double, double>(0.0, 0.0));
    }

    //===================== FITKStruThresholdPostDataManager =====================
    FITKStruThresholdPostDataManager::FITKStruThresholdPostDataManager(FITKAbstractStructuralPostVTK* absPost)
    {
        _absPost = absPost;
    }
    void FITKStruThresholdPostDataManager::appendDataObj(Core::FITKAbstractDataObject* item)
    {
        FITKStruThresholdPostData* v = dynamic_cast<FITKStruThresholdPostData*>(item);
        if (v == nullptr || _absPost == nullptr) {
            return;
        }
        auto def = _absPost->getOutput();
        if (def == nullptr) {
            return;
        }
        v->setModelID(_absPost->getDataObjectID());
        v->setInput(def);
        Core::FITKAbstractDataManager<FITKStruThresholdPostData>::appendDataObj(item);
    }
    void FITKStruThresholdPostDataManager::insertDataObj(int index, Core::FITKAbstractDataObject* item)
    {
        FITKStruThresholdPostData* v = dynamic_cast<FITKStruThresholdPostData*>(item);
        if (v == nullptr || _absPost == nullptr) {
            return;
        }
        auto def = _absPost->getOutput();
        if (def == nullptr) {
            return;
        }
        v->setModelID(_absPost->getDataObjectID());
        v->setInput(def);
        Core::FITKAbstractDataManager<FITKStruThresholdPostData>::insertDataObj(index, item);
    }
}
