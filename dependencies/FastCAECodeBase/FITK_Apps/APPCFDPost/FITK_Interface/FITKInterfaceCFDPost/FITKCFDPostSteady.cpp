/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostSteady.h"
#include "FITKAlgDatReader.h"
#include "FITKAlgPltReader.h"
#include "FITKAlgCGNSReader.h"

#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

#include <vtkDataSetReader.h>
#include <vtkAlgorithm.h>
#include <vtkAlgorithm.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPolyData.h>
#include <vtkXMLUnstructuredGridReader.h>

#include <QFile>

namespace Interface
{
    FITKCFDPostSteady::FITKCFDPostSteady(QString fileType) :
        _fileType(fileType)
    {
        if (fileType == "vtk") {
            _dataReader = vtkDataSetReader::New();
        }
        else if (fileType == "cgns") {
            _dataReader = FITKAlgCGNSReader::New();
        }
        else if (fileType == "dat") {
            _dataReader = FITKAlgDatReader::New();
        }
        else if (fileType == "plt") {
            _dataReader = FITKAlgPltReader::New();
        }
        else if (fileType == "vtp") {
            _dataReader = vtkXMLPolyDataReader::New();
        }
        else if (fileType == "vtu") {
            _dataReader = vtkXMLUnstructuredGridReader::New();
        }

        if (_dataReader == nullptr)return;
        vtkDataSetMapper* mapper = vtkDataSetMapper::New();
        mapper->SetScalarVisibility(false);
        mapper->SetLookupTable(_lookUpTable);
        mapper->SetInterpolateScalarsBeforeMapping(true);  
        mapper->SetInputConnection(_dataReader->GetOutputPort());
        _mappers.append(mapper);

        //创建特征
        initFeatureEdge();
    }

    FITKCFDPostSteady::~FITKCFDPostSteady()
    {
        if (_dataReader) {
            _dataReader->Delete();
            _dataReader = nullptr;
        }
    }

    FITKPostDataType FITKCFDPostSteady::getPostDataType()
    {
        return FITKPostDataType::Post_Steady;
    }

    void FITKCFDPostSteady::setFile(QString & fileName)
    {
        if (_mappers.isEmpty())return;

        _fileName = fileName;
        if (_fileName.isEmpty())return;
        QByteArray ba = fileName.toLocal8Bit();
        char* cstr = ba.data();

        if (_fileType == "vtk") {
            vtkDataSetReader* dataSetReader = vtkDataSetReader::SafeDownCast(_dataReader);
            if (dataSetReader == nullptr)return;
            dataSetReader->SetFileName(cstr);
            dataSetReader->ReadAllScalarsOn();
            dataSetReader->ReadAllVectorsOn();
            dataSetReader->ReadAllTensorsOn();
            dataSetReader->ReadAllFieldsOn();
            dataSetReader->ReadAllColorScalarsOn();
            dataSetReader->ReadAllNormalsOn();
            dataSetReader->ReadAllTCoordsOn();
        }
        else if (_fileType == "cgns") {
            FITKAlgCGNSReader* datReader = dynamic_cast<FITKAlgCGNSReader*>(_dataReader);
            if (datReader == nullptr)return;
            datReader->setFileName(_fileName);
        }
        else if (_fileType == "dat") {
            FITKAlgDatReader* datReader = dynamic_cast<FITKAlgDatReader*>(_dataReader);
            if (datReader == nullptr)return;
            datReader->setFileName(_fileName);
        }
        else if (_fileType == "plt") {
            FITKAlgPltReader* datReader = dynamic_cast<FITKAlgPltReader*>(_dataReader);
            if (datReader == nullptr)return;
            datReader->setFileName(_fileName);
        }
        else if (_fileType == "vtp") {
            vtkXMLPolyDataReader* datReader = dynamic_cast<vtkXMLPolyDataReader*>(_dataReader);
            if (datReader == nullptr)return;
            datReader->SetFileName(cstr);
        }
        else if (_fileType == "vtu") {
            vtkXMLUnstructuredGridReader* dataReader = dynamic_cast<vtkXMLUnstructuredGridReader*>(_dataReader);
            if (dataReader == nullptr)return;
            dataReader->SetFileName(cstr);
        }
    }

    vtkDataSet* FITKCFDPostSteady::getOutput()
    {
        if (_dataReader == nullptr)return nullptr;
        if (_fileType == "vtk") {
            vtkDataSetReader* dataSetReader = vtkDataSetReader::SafeDownCast(_dataReader);
            if (dataSetReader == nullptr)return nullptr;
            return dataSetReader->GetOutput();
        }
        else if (_fileType == "cgns") {
            FITKAlgCGNSReader* dataSetReader = FITKAlgCGNSReader::SafeDownCast(_dataReader);
            if (dataSetReader == nullptr)return nullptr;
            return dataSetReader->GetOutput();
        }
        else if (_fileType == "dat") {
            FITKAlgDatReader* datReader = dynamic_cast<FITKAlgDatReader*>(_dataReader);
            if (datReader == nullptr)return nullptr;
            return datReader->GetOutput();
        }
        else if (_fileType == "plt") {
            FITKAlgPltReader* datReader = dynamic_cast<FITKAlgPltReader*>(_dataReader);
            if (datReader == nullptr)return nullptr;
            return datReader->GetOutput();
        }
        else if (_fileType == "vtp") {
            vtkXMLPolyDataReader* datReader = dynamic_cast<vtkXMLPolyDataReader*>(_dataReader);
            if (datReader == nullptr)return nullptr;
            return datReader->GetOutput();
        }
        else if (_fileType == "vtu") {
            vtkXMLUnstructuredGridReader* dataReader = dynamic_cast<vtkXMLUnstructuredGridReader*>(_dataReader);
            if (dataReader == nullptr)return nullptr;
            return dataReader->GetOutput();
        }
        return nullptr;
    }
    vtkAlgorithmOutput * FITKCFDPostSteady::getOutputPort()
    {
        if (_dataReader == nullptr)return nullptr;
        return _dataReader->GetOutputPort();
    }
}

