/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAlgCGNSReader.h"

#include <vtkInformation.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkSmartPointer.h>
#include <vtkCGNSReader.h>
#include <vtkAppendFilter.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkCellData.h>
#include <vtkUnstructuredGrid.h>

#define  MAXBLOCKNUM  2000

namespace Interface
{
    FITKAlgCGNSReader* FITKAlgCGNSReader::New()
    {
        FITKAlgCGNSReader* alg = new FITKAlgCGNSReader();
        alg->InitializeObjectBase();
        return alg;
    }

    void FITKAlgCGNSReader::PrintSelf(ostream & os, vtkIndent indent)
    {

    }

    void FITKAlgCGNSReader::setFileName(const QString fileName)
    {
        _fileName = fileName;
    }

    FITKAlgCGNSReader::FITKAlgCGNSReader()
    {
        this->SetNumberOfInputPorts(0);
        this->SetNumberOfOutputPorts(MAXBLOCKNUM + 1);

        for (int i = 0; i < MAXBLOCKNUM; ++i)
            _visibleStates.append(true);
    }

    FITKAlgCGNSReader::~FITKAlgCGNSReader()
    {

    }

    int FITKAlgCGNSReader::FillOutputPortInformation(int port, vtkInformation * info)
    {
        info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
        return 1;
    }

    int FITKAlgCGNSReader::ProcessRequest(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
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

    int FITKAlgCGNSReader::RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
    {
        if (_fileName.isEmpty()){
            vtkErrorMacro("A FileName must be specified.");
            return 0;
        }
        QByteArray f = _fileName.toLocal8Bit();
        char* FileName = f.data();
        vtkMultiBlockDataSet* mBlock = nullptr;
        vtkSmartPointer<vtkCGNSReader> cgnsReader = vtkSmartPointer<vtkCGNSReader>::New();
        cgnsReader->SetFileName(FileName);
        cgnsReader->Update();
        mBlock = cgnsReader->GetOutput();

        if (mBlock == nullptr) return 0;
        getBlocks(mBlock);

        vtkSmartPointer<vtkAppendFilter> appFilter = vtkSmartPointer<vtkAppendFilter>::New();
        for (int i = 0; i < _blockList.size(); ++i)
        {
            if (!_visibleStates[i]) continue;
            auto dataset = _blockList.at(i);

            //fillPointArray(dataset);
            //fillCellArray(dataset);
            appFilter->AddInputData(dataset);
        }

        appFilter->Update();

        vtkUnstructuredGrid* output = vtkUnstructuredGrid::GetData(outputVector);
        output->CopyStructure(appFilter->GetOutput());
        output->GetPointData()->PassData(appFilter->GetOutput()->GetPointData());
        output->GetCellData()->PassData(appFilter->GetOutput()->GetCellData());

        for (int i = 1; i <= _blockList.size(); i++)
        {
            if (i > MAXBLOCKNUM) break;
            auto data = _blockList.at(i - 1);
            vtkUnstructuredGrid* outData = vtkUnstructuredGrid::GetData(outputVector, i);
            vtkSmartPointer<vtkAppendFilter> appFilter = vtkSmartPointer<vtkAppendFilter>::New();
            appFilter->AddInputData(data);
            appFilter->Update();
            outData->CopyStructure(appFilter->GetOutput());
            outData->GetPointData()->PassData(appFilter->GetOutput()->GetPointData());
            outData->GetCellData()->PassData(appFilter->GetOutput()->GetCellData());;
        }
    }

    void FITKAlgCGNSReader::getBlocks(vtkDataObject * blockData, const char * cuttentName)
    {
        auto block = vtkMultiBlockDataSet::SafeDownCast(blockData);

        if (block == nullptr)
        {
            auto dataset = vtkDataSet::SafeDownCast(blockData);
            if (dataset == nullptr) return;
            dataset->Modified();
            QString totalName = QString::fromLocal8Bit(cuttentName);
            QStringList NameBC = totalName.split("!|||!");

            if (NameBC.size() == 3)
            {
                _blockNames.append(NameBC.at(0));
                _bcTypes.append(NameBC.at(1));
                int index = _bcTypes.size() - 1;
                int id = NameBC.at(2).toInt();
                _reslationShip.insert(id, index);
            }
            else
            {
                _blockNames.append(totalName);
                _bcTypes.append("None");
            }

            _blockList.append(dataset);
            getPointArray(dataset);
            getCellArray(dataset);
            return;
        }

        block->Modified();
        const int nBlock = block->GetNumberOfBlocks();

        for (int i = 0; i < nBlock; i++)
        {
            vtkDataObject* obj = block->GetBlock(i);
            const char* currentName = block->GetMetaData(i)->Get(vtkCompositeDataSet::NAME());
            getBlocks(obj, currentName);
        }
    }

    void FITKAlgCGNSReader::getPointArray(vtkDataSet * dataset)
    {
        if (dataset == nullptr) return;

        vtkPointData* pointData = dataset->GetPointData();
        if (pointData == nullptr) return;

        const int nPointArray = pointData->GetNumberOfArrays();
        for (int i = 0; i < nPointArray; i++)
        {
            const char* aName = pointData->GetArrayName(i);
            vtkDataArray* dataArray = pointData->GetArray(aName);
            if (dataArray == nullptr) return; //continue;
            int aNum[2]{ 0 };
            aNum[0] = dataArray->GetNumberOfComponents();
            aNum[1] = dataArray->GetNumberOfTuples();

            if (_pointDataArray.contains(aName)) continue;
            _pointDataArray.insert(aName, aNum);
        }
    }

    void FITKAlgCGNSReader::getCellArray(vtkDataSet * dataset)
    {
        if (dataset == nullptr) return;

        vtkCellData* cellData = dataset->GetCellData();
        if (cellData == nullptr) return;

        const int nCellArray = cellData->GetNumberOfArrays();
        for (int i = 0; i < nCellArray; i++)
        {
            const char* aName = cellData->GetArrayName(i);
            vtkDataArray* dataArray = cellData->GetArray(aName);
            if (dataArray == nullptr) return; //continue;
            int aNum[2]{ 0 };
            aNum[0] = dataArray->GetNumberOfComponents();
            aNum[1] = dataArray->GetNumberOfTuples();

            if (_cellDataArray.contains(aName)) continue;
            _cellDataArray.insert(aName, aNum);
        }
    }
}

