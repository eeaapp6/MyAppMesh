/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTecplotDatReader.h"

#include <vtkMultiBlockDataSet.h>
#include <vtkAlgorithmOutput.h>
#include <vtkAppendFilter.h>
#include <vtkInformation.h>
#include <vtkIndent.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataObject.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkInformationVector.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkIdTypeArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellDataToPointData.h>
#include <vtkTecplotReader.h>

#include <QDebug>

#define MAXBLOCKNUM 2000

FITKTecplotDatReader::FITKTecplotDatReader()
{
    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(MAXBLOCKNUM + 1);

    for (int i = 0; i < MAXBLOCKNUM; ++i)
        _visibleStates.append(true);
}

FITKTecplotDatReader::~FITKTecplotDatReader()
{

}

FITKTecplotDatReader *FITKTecplotDatReader::New()
{
    return new FITKTecplotDatReader;
}

void FITKTecplotDatReader::PrintSelf(ostream &os, vtkIndent indent)
{
    QByteArray bFileName = _fileName.toLocal8Bit();
    char* FileName = bFileName.data();
    this->Superclass::PrintSelf(os, indent);
    os << indent << "FileName: " << (FileName ? FileName : "(none)") << "\n";
}

void FITKTecplotDatReader::setFileName(QString fileName)
{
    _fileName = fileName;
}

QString FITKTecplotDatReader::getFileName()
{
    return _fileName;
}

int FITKTecplotDatReader::RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *outputVector)
{
    // Make sure we have a file to read.
    if (_fileName == "") {
        vtkErrorMacro("A FileName must be specified.");
        return 0;
    }
    QByteArray bFileName = _fileName.toLocal8Bit();
    char* FileName = bFileName.data();
    _blockList.clear();
    //_blockNames.clear();

    vtkSmartPointer<vtkTecplotReader> reader = vtkSmartPointer<vtkTecplotReader>::New();
    reader->SetFileName(FileName);
    reader->Update();

    //拿到文件中所有的数据块
    auto multidataset = reader->GetOutput();
    const int nblock = reader->GetNumberOfBlocks();
    for (int i = 0; i < nblock; i++)
    {
        _blockNames.append(reader->GetBlockName(i));
    }
    getBlocks(multidataset);
    vtkSmartPointer<vtkAppendFilter> appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
    for (int i = 0; i < _blockList.size(); i++)
    {
        if (!_visibleStates[i])
            continue;
        auto dataset = _blockList.at(i);

        appendFilter->AddInputData(dataset);
    }
    //appendFilter->Update();

    vtkSmartPointer<vtkCellDataToPointData> cellToPoint = vtkSmartPointer<vtkCellDataToPointData>::New();
    cellToPoint->SetInputConnection(appendFilter->GetOutputPort());
    cellToPoint->Update();

    //拷贝数据
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::GetData(outputVector);
    output->CopyStructure(cellToPoint->GetOutput());
    output->GetPointData()->PassData(cellToPoint->GetOutput()->GetPointData());
    output->GetCellData()->PassData(cellToPoint->GetOutput()->GetCellData());

    for (int i = 1; i <= _blockList.size(); i++)
    {
        if (i > MAXBLOCKNUM)
            break;
        auto data = _blockList.at(i - 1);
        vtkUnstructuredGrid *outData = vtkUnstructuredGrid::GetData(outputVector, i);
        vtkSmartPointer<vtkAppendFilter> appFilter = vtkSmartPointer<vtkAppendFilter>::New();
        appFilter->AddInputData(data);
        vtkSmartPointer<vtkCellDataToPointData> ctp = vtkSmartPointer<vtkCellDataToPointData>::New();
        ctp->SetInputConnection(appFilter->GetOutputPort());
        ctp->Update();
        //appFilter->Update();
        outData->CopyStructure(ctp->GetOutput());
        outData->GetPointData()->PassData(ctp->GetOutput()->GetPointData());
        outData->GetCellData()->PassData(ctp->GetOutput()->GetCellData());
    }

    return 1;
}

int FITKTecplotDatReader::ProcessRequest(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector)
{
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
    {
        return this->RequestInformation(request, inputVector,
            outputVector);
    }
    if (request->Has(
        vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
    {
        return this->RequestUpdateExtent(request, inputVector,
            outputVector);
    }
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
    {
        return this->RequestData(request, inputVector, outputVector);
    }
    return this->Superclass::ProcessRequest(request, inputVector,
        outputVector);
}

void FITKTecplotDatReader::getBlocks(vtkDataObject *blockData)
{
    auto block = vtkMultiBlockDataSet::SafeDownCast(blockData);

    if (block == nullptr)
    {
        auto dataset = vtkDataSet::SafeDownCast(blockData);
        if (dataset == nullptr)
            return;
        dataset->Modified();
        _blockList.append(dataset);
        return;
    }

    block->Modified();
    const int nBlock = block->GetNumberOfBlocks();

    for (int i = 0; i < nBlock; i++)
    {
        vtkDataObject *obj = block->GetBlock(i);
        getBlocks(obj);
    }
}

int FITKTecplotDatReader::getNumberOfBlocks()
{
    return _blockList.size();
}

void FITKTecplotDatReader::setVisible(int blockIndex, bool vis)
{
    if (blockIndex >= MAXBLOCKNUM)
        return;
    _visibleStates[blockIndex] = vis;
}

QStringList FITKTecplotDatReader::getBlockNames()
{
    //     QStringList names{};
    //     const int n = getNumberOfBlocks();
    //     for (int i = 0; i < n; ++i)
    //     {
    //         names.append(QString("Zone_%1").arg(i + 1));
    //     }
    //     return names;
    return _blockNames;
}

QStringList FITKTecplotDatReader::getBlockBCTypes()
{
    QStringList names{};
    const int n = getNumberOfBlocks();
    for (int i = 0; i < n; ++i)
    {
        names.append("None");
    }
    return names;
}

int FITKTecplotDatReader::FillOutputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
    return 1;
}