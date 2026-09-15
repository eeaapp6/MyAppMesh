/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAlgPltReader.h"
#include "FITKPltReader.h"

#include <vtkDataSet.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCompositeDataSet.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkAppendFilter.h>
#include <vtkCellDataToPointData.h>
#include <vtkSmartPointer.h>
#include <QElapsedTimer>
#include <vtkPolyData.h>
#include <QDebug>
#include <QThread>
#include <QDateTime>
#define  MAXBLOCKNUM  2000

namespace Interface
{
    FITKAlgPltReader* FITKAlgPltReader::New()
    {
        return new FITKAlgPltReader;
    }

    void FITKAlgPltReader::PrintSelf(ostream& os, vtkIndent indent)
    {
        QByteArray bFileName = _fileName.toLocal8Bit();
        char* FileName = bFileName.data();
        this->Superclass::PrintSelf(os, indent);
        os << indent << "FileName:" << (FileName ? FileName : "(none)") << "\n";
    }

    void FITKAlgPltReader::setFileName(QString fileName)
    {
        _fileName = fileName;
    }

    QString FITKAlgPltReader::getFileName()
    {
        return _fileName;
    }

    int FITKAlgPltReader::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
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

    int FITKAlgPltReader::RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector* outputVector)
    {
        if (_fileName == "") {
            vtkErrorMacro("A FileName must be specified.");
            return 0;
        }
        QByteArray bFileName = _fileName.toLocal8Bit();
        char* FileName = bFileName.data();

        _blockList.clear();
        _blockNames.clear();

        QList<vtkCellDataToPointData *> dataList{};
        QList<vtkAppendFilter*> appendFilterList{};

        vtkMultiBlockDataSet* mBlock = nullptr;

        //plt文件读取
        vtkSmartPointer<FITKPltReader> reader = vtkSmartPointer<FITKPltReader>::New();
        reader->SetFileName(FileName);
        reader->Update();
        qDebug() << "Read Over:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        mBlock = reader->GetOutput();

        vtkAppendFilter* appendFilter = vtkAppendFilter::New();
        appendFilterList.append(appendFilter);
        appendFilter->MergePointsOn();

        if (mBlock == nullptr) return 0;
        getBlocks(mBlock);

        for (int i = 0; i < _blockList.size(); ++i)
        {
            if (!_visibleStates[i]) continue;
            auto dataset = _blockList.at(i);
            appendFilter->AddInputData(dataset);
        }

        vtkSmartPointer<vtkCellDataToPointData> cellToPoint = vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(appendFilter->GetOutputPort());
        cellToPoint->Update();

        vtkUnstructuredGrid* output = vtkUnstructuredGrid::GetData(outputVector);
        output->CopyStructure(cellToPoint->GetOutput());
        output->GetPointData()->PassData(cellToPoint->GetOutput()->GetPointData());
        output->GetCellData()->PassData(cellToPoint->GetOutput()->GetCellData());


        //appFilter->Update();
        //QStringList names;
        //int cellArrayNumber = appendFilter->GetOutput()->GetCellData()->GetNumberOfArrays();


        //for (int i = 0; i < cellArrayNumber; i++)
        //{
        //    vtkCellDataToPointData* cellToPoint = vtkCellDataToPointData::New();
        //    dataList.append(cellToPoint);
        //    names << QString(appendFilter->GetOutput()->GetCellData()->GetArrayName(i));
        //    DataThreadProcessing *dataProc = new DataThreadProcessing(appendFilter->GetOutputPort(), names.at(i), cellToPoint);
        //    THREADPOOLMANAGER->threadPool->start(dataProc);
        //    QThread::msleep(100);
        //    //qDebug() << __FUNCTION__ << " u2 " << names.at(i) << tm.elapsed();
        //}
        //THREADPOOLMANAGER->waitForDone();

        //for (int i = 0; i < dataList.size(); i++)
        //{
        //    output->CopyStructure(dataList[i]->GetOutput());
        //    output->GetPointData()->PassData(dataList[i]->GetOutput()->GetPointData());
        //    output->GetCellData()->PassData(dataList[i]->GetOutput()->GetCellData());
        //    dataList[i]->Delete();
        //}
        //dataList.clear();

        //for (int i = 0; i < _blockList.size(); i++)
        //{
        //    if (i > MAXBLOCKNUM) break;
        //    if (!_visibleStates[i]) continue;
        //    auto data = _blockList.at(i);
        //    vtkAppendFilter* appFilter = vtkAppendFilter::New();
        //    vtkCellDataToPointData* ctp = vtkCellDataToPointData::New();
        //    appendFilterList.append(appFilter);
        //    dataList.append(ctp);

        //    appFilter->AddInputData(data);
        //    appFilter->Update();
        //    DataThreadProcessing *dataProc = new DataThreadProcessing(appFilter->GetOutputPort(), QString::Null(), ctp);
        //    THREADPOOLMANAGER->threadPool->start(dataProc);
        //    //qDebug() << __FUNCTION__ << " u3 " << i << tm.elapsed();
        //}
        //THREADPOOLMANAGER->waitForDone();

        //for (int i = 0; i < _blockList.size(); ++i)
        //{
        //    vtkUnstructuredGrid* outData = vtkUnstructuredGrid::GetData(outputVector, i + 1);
        //    if (dataList[i]->GetOutput() == nullptr) continue;
        //    outData->CopyStructure(dataList[i]->GetOutput());
        //    outData->GetPointData()->PassData(dataList[i]->GetOutput()->GetPointData());
        //    outData->GetCellData()->PassData(dataList[i]->GetOutput()->GetCellData());
        //    dataList[i]->Delete();
        //    appendFilterList[i]->Delete();
        //    //qDebug() << __FUNCTION__ << " u4 " << i << tm.elapsed();
        //}
        //dataList.clear();
        //appendFilterList.clear();
        ////qDebug() << __FUNCTION__ << " u3 " << tm.elapsed();

        for (int i = 1; i <= _blockList.size(); i++)
        {
            if (i > MAXBLOCKNUM) break;
            auto data = _blockList.at(i - 1);
            vtkUnstructuredGrid* outData = vtkUnstructuredGrid::GetData(outputVector, i);
            vtkSmartPointer<vtkAppendFilter> appFilter = vtkSmartPointer<vtkAppendFilter>::New();
            appFilter->AddInputData(data);
            vtkSmartPointer<vtkCellDataToPointData> ctp = vtkSmartPointer<vtkCellDataToPointData>::New();
            ctp->SetInputConnection(appFilter->GetOutputPort());
            ctp->Update();
            //appFilter->Update();
            outData->CopyStructure(ctp->GetOutput());
            outData->GetPointData()->PassData(ctp->GetOutput()->GetPointData());
            outData->GetCellData()->PassData(ctp->GetOutput()->GetCellData());;
        }
        qDebug() << "data Appent:" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        return 1;
    }

    FITKAlgPltReader::FITKAlgPltReader()
    {
        this->SetNumberOfInputPorts(0);
        this->SetNumberOfOutputPorts(MAXBLOCKNUM + 1);
        for (int i = 0; i < MAXBLOCKNUM; ++i)_visibleStates.append(true);

    }
    FITKAlgPltReader::~FITKAlgPltReader()
    {

    }

    void FITKAlgPltReader::getBlocks(vtkDataObject* blockData, const char* Name)
    {
        vtkMultiBlockDataSet* block = vtkMultiBlockDataSet::SafeDownCast(blockData);

        if (block == nullptr)
        {
            vtkDataSet* dataset = vtkDataSet::SafeDownCast(blockData);
            if (dataset == nullptr) return;
            dataset->Modified();
            QString totalName = QString::fromLocal8Bit(Name);
            //QStringList NameBC = totalName.split("!|||!");

            _blockNames.append(totalName);

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

    int FITKAlgPltReader::getNumberOfBlocks()
    {
        return _blockList.size();
    }
    void FITKAlgPltReader::setVisible(int blockIndex, bool vis)
    {
        _visibleStates[blockIndex] = vis;
    }
    QStringList FITKAlgPltReader::getBlockNames()
    {
        return _blockNames;
    }


    void FITKAlgPltReader::getPointArray(vtkDataSet* dataset)
    {
        if (dataset == nullptr) return;

        auto pointData = dataset->GetPointData();
        if (pointData == nullptr) return;

        const int nPointArray = pointData->GetNumberOfArrays();
        for (int i = 0; i < nPointArray; i++)
        {
            const char* aName = pointData->GetArrayName(i);
            auto dataArray = pointData->GetArray(aName);
            if (dataArray == nullptr) return; //continue;
            int aNum[2]{ 0 };
            aNum[0] = dataArray->GetNumberOfComponents();
            aNum[1] = dataArray->GetNumberOfTuples();

            if (_pointDataArray.contains(aName)) continue;
            _pointDataArray.insert(aName, aNum);
        }
    }

    void FITKAlgPltReader::getCellArray(vtkDataSet* dataset)
    {
        if (dataset == nullptr) return;

        auto cellData = dataset->GetCellData();
        if (cellData == nullptr) return;

        const int nCellArray = cellData->GetNumberOfArrays();
        for (int i = 0; i < nCellArray; i++)
        {
            const char* aName = cellData->GetArrayName(i);
            auto dataArray = cellData->GetArray(aName);
            if (dataArray == nullptr) return; //continue;
            int aNum[2]{ 0 };
            aNum[0] = dataArray->GetNumberOfComponents();
            aNum[1] = dataArray->GetNumberOfTuples();

            if (_cellDataArray.contains(aName)) continue;
            _cellDataArray.insert(aName, aNum);
        }
    }

    int FITKAlgPltReader::FillOutputPortInformation(int port, vtkInformation* info)
    {
        Q_UNUSED(port)
            info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
        return 1;
    }
}