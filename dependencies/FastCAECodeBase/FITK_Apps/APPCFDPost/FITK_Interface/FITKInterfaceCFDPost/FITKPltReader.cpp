/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPltReader.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkStructuredGrid.h>
#include <vtkUnsignedCharArray.h>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkMath.h>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <vtkCellType.h>
#include <vtkCellArray.h>

#include <QDateTime>

#define MAXLENGTH 524280 //最大读取字节长度
#define INT32SIZE 4
#define FLOAT32SSIZE 4
#define DOUBLESIZE 8
#define DOUBLESIZE 8
#define FEPOLYGON 6
#define FEPOLYHEDRON  7

namespace Interface
{
    FITKPltReader* FITKPltReader::New()
    {
        auto reader = new FITKPltReader;
        reader->InitializeObjectBase();
        return reader;
    }

    void FITKPltReader::PrintSelf(ostream& os, vtkIndent indent)
    {
        Q_UNUSED(os)
            Q_UNUSED(indent)
    }

    FITKPltReader::FITKPltReader()
    {
        this->SetNumberOfInputPorts(0);
        this->SetNumberOfOutputPorts(1);
        this->FileName = nullptr;
    }

    FITKPltReader::~FITKPltReader()
    {
    }




    int FITKPltReader::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
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

    int FITKPltReader::RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector* outputVector)
    {
        auto output = vtkMultiBlockDataSet::GetData(outputVector);
        QFileInfo info(FileName);
        QFile file(FileName);
        if (!file.open(QIODevice::ReadOnly)) return false;
        QByteArray m_data = file.readAll();
        Reader(m_data);

        //设置分区数量
        output->SetNumberOfBlocks(zoneInfos.size());
        for (size_t i = 0; i < zoneInfos.size(); i++){
            //创建VTK的点数据对象
            vtkPoints* pntCords = vtkPoints::New();
            int numberPoints = zoneInfos[i].numberPoints;
            for (int j = 0; j < numberPoints; j++){
                pntCords->InsertNextPoint(dataInfos[i].Points[j].x, dataInfos[i].Points[j].y, dataInfos[i].Points[j].z);
            }
            //创建VTK的非机构化网格数据对象
            vtkUnstructuredGrid* unstruct = vtkUnstructuredGrid::New();
            unstruct->SetPoints(pntCords);

            GetUnstructuredGridCells(i, zoneInfos[i].numberCells, zoneInfos[i].zoneType, unstruct, output);
            GetArraysFromBlockPackingZone(i, zoneInfos[i].numberPoints, zoneInfos[i].numberCells, unstruct->GetPoints(), unstruct->GetPointData(), unstruct->GetCellData(), output);

            output->SetBlock(i, unstruct);
            output->GetMetaData(i)->Set(vtkCompositeDataSet::NAME(), zoneInfos[i].ZoneName.toLocal8Bit());
        }

        if (_readErrorCode == NoError){
            return 1;
        }
        else{
            return 0;
        }
    }

    QByteArray FITKPltReader::ReadBytes(QByteArray & data, size_t typeSize, size_t readLengh)
    {
        QByteArray buffer{};
        QByteArray buffers{};
        if (readLengh == 0)
        {
            int end = 1; //当end == 0 时，则结束循环
            while (end != 0)
            {
                //读取指定长度的数据
                buffer = data.mid(DirectionHead, DirectionHead + typeSize);
                memcpy(&end, buffer.data(), typeSize);
                //readLengh += typeSize;
                DirectionHead += typeSize;
                if (end == 0) break;
                buffers += (char)end;
            }
            //DirectionHead -= readLengh;
        }

        //buffer = data.mid(DirectionHead, readLengh);
        //DirectionHead += readLengh;
        return buffers;
    }

    QByteArray FITKPltReader::ReadBytes(QByteArray & data, size_t readLengh)
    {
        QByteArray buffer{};
        buffer = data.mid(DirectionHead, readLengh);
        DirectionHead += readLengh;
        return buffer;
    }

    template<typename T2>
    void FITKPltReader::ReadBytes(QByteArray & data, T2 & backData, size_t readLengh)
    {
        QByteArray buffer{};
        //读取指定数据结果
        buffer = data.mid(DirectionHead, readLengh);
        //将数据结果拷贝至数据中
        memcpy(&backData, buffer.data(), readLengh);
        DirectionHead += readLengh;
    }

    bool FITKPltReader::IsReadEnd(QByteArray & data)
    {
        if (DirectionHead > data.size())
            return true;
        return false;
    }

    void FITKPltReader::GetUnstructuredGridCells(int zoneIndex, int numberCells, int cellType, vtkUnstructuredGrid * unstrctGrid, vtkMultiBlockDataSet* multZone)
    {
        if (unstrctGrid == nullptr)return;
        int numCellPnts = -1;
        int theCellType = -1;

        if (cellType == FE_BRICK)
        {
            numCellPnts = 8;
            theCellType = VTK_HEXAHEDRON;
            TopologyDim = vtkMath::Max(TopologyDim, 3);
        }
        else if (cellType == FE_TRIANGLE)
        {
            numCellPnts = 3;
            theCellType = VTK_TRIANGLE;
            TopologyDim = vtkMath::Max(TopologyDim, 2);
        }
        else if (cellType == FE_QUAD)
        {
            numCellPnts = 4;
            theCellType = VTK_QUAD;
            TopologyDim = vtkMath::Max(TopologyDim, 2);
        }
        else if (cellType == FE_TET)
        {
            numCellPnts = 4;
            theCellType = VTK_TETRA;
            TopologyDim = vtkMath::Max(TopologyDim, 3);
        }
        else if (cellType == FE_LINE_SEG)
        {
            numCellPnts = 2;
            theCellType = VTK_LINE;
            TopologyDim = vtkMath::Max(TopologyDim, 3);
        }
        else
        {
            qDebug() << "Unknown cell type for a zone.";

            numCellPnts = 4;
            theCellType = VTK_TETRA;
            TopologyDim = vtkMath::Max(TopologyDim, 3);
            if (numberCells == 0) numberCells = unstrctGrid->GetNumberOfPoints() / 4;
            vtkCellArray *cells = vtkCellArray::New();
            int *cellTypes = new int[numberCells];

            // fill the three arrays
            for (int c = 0; c < numberCells; c++)
            {
                cellTypes[c] = theCellType;
                vtkIdList* cellIDs = vtkIdList::New();
                for (int j = 0; j < numCellPnts; j++)
                {
                    auto ids = c * 4 + j;
                    cellIDs->InsertNextId(ids);
                }
                cells->InsertNextCell(cellIDs);
                cellIDs->Delete();
                cellIDs = nullptr;
                cellIDs = vtkIdList::New();
            }
            unstrctGrid->SetCells(cellTypes, cells);
            delete[] cellTypes;
            cellTypes = nullptr;
            cells->Delete();
            cells = nullptr;
            return;
        }

        vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
        int *cellTypes = new int[numberCells];

        // fill the three arrays
        for (int c = 0; c < numberCells; c++)
        {
            cellTypes[c] = theCellType;
            vtkSmartPointer<vtkIdList> cellIDs = vtkSmartPointer<vtkIdList>::New();
            for (int j = 0; j < numCellPnts; j++)
            {
                auto ids = dataInfos[zoneIndex].Cells[c * numCellPnts + j];
                cellIDs->InsertNextId(ids);
            }
            cells->InsertNextCell(cellIDs);
        }

        unstrctGrid->SetCells(cellTypes, cells);
        delete[] cellTypes;
        cellTypes = nullptr;
    }

    void FITKPltReader::GetArraysFromBlockPackingZone(int zoneIndex, int numNodes, int numCells, vtkPoints * theNodes, vtkPointData * nodeData, vtkCellData * cellData, vtkMultiBlockDataSet * multZone)
    {
        if (!theNodes || !nodeData || !cellData || zoneIndex >= dataInfos.size())
        {
            qDebug() << "File not open, errors with reading, or nullptr vtkPoints /"
                << "vtkPointData / vtkCellData /ReadDataError.";

            _readErrorCode = PointError;
            return;
        }

        int variableSizes = dataInfos[zoneIndex].variablesData.size();
        for (int i = 0; i < variableSizes; i++)
        {
            vtkSmartPointer<vtkDoubleArray> array = vtkSmartPointer<vtkDoubleArray>::New();
            int variableSize = dataInfos[zoneIndex].variablesData[i].size();
            array->SetName(handerInfo.VariableNames[i].toLocal8Bit());
            array->SetNumberOfValues(variableSize);
            for (int j = 0; j < variableSize; j++)
            {
                array->SetValue(j, dataInfos[zoneIndex].variablesData[i][j]);
            }


            if (zoneInfos[zoneIndex].VariableTypes[i] == 0)
            {
                nodeData->AddArray(array);
            }
            else if (zoneInfos[zoneIndex].VariableTypes[i] == 1)
            {
                cellData->AddArray(array);
            }
        }
    }

    void FITKPltReader::Reader(QByteArray &data)
    {
        //读取文件基本信息
        {
            //获取plt版本
            handerInfo.Version = ReadBytes(data, 8);
            //获取整数（默认是1）
            int integer = 0;
            ReadBytes(data, integer, INT32SIZE);
            //读取文件类型（0：NULL  1：网格  2：后处理结果）
            int fileType = 0;
            ReadBytes(data, fileType, INT32SIZE);
            //The TITLE. int32 * N
            handerInfo.DataTitle = QString(ReadBytes(data, INT32SIZE, 0));
            //获取场数量
            ReadBytes(data, NumberOfVariables, INT32SIZE);
            for (int i = 0; i < NumberOfVariables; i++)
            {
                //获取每个场名称 Variable names. int32 * N  N == NumberOfVariables
                handerInfo.VariableNames.append(QString(ReadBytes(data, INT32SIZE, 0)));
            }
            handerInfo.print();
        }

        //读取分区标记（版本v112时：299.0  版本v191时：298.0）
        float startZone = -1;
        ReadBytes(data, startZone, FLOAT32SSIZE);

        //读取所有分区
        if (startZone == 299.0){
            ReadZones(data);
        }

        //获取标记
        float startData = -1;
        ReadBytes(data, startData, FLOAT32SSIZE);

        //读取数据
        if (startData == 299.0){
            ReadDataSection(data);
        }
        return;
    }


    float FITKPltReader::ReadZones(QByteArray & data)
    {
        ZoneInfo info{};
        //zones marker
        //int zonesMarker = -1;
        //ReadBytes(data, zonesMarker, INT32SIZE);

        //分区名称获取
        info.ZoneName = QString(ReadBytes(data, INT32SIZE, 0));

        //父分区获取（已经弃用）
        int parentZone = -1;
        ReadBytes(data, parentZone, INT32SIZE);

        //StrandID（-2 =等待Tecplot分配的链ID， - 1 =静态链，ID >= 0有效链ID）
        int strandID = -1;
        ReadBytes(data, strandID, INT32SIZE);

        //求解时间获取
        double solutionTime = -1;
        ReadBytes(data, solutionTime, DOUBLESIZE);
        
        //默认区域颜色（很少使用，设置为-1）
        ReadBytes(data, info.zoneColor, INT32SIZE);
        
        //获取分区类型（0 = 有序、1 = FELINESEG、2 = 三角形 、3 = 四边形 、4 = 四面体、5 = 框体、6 = 多边形、7 = 多面体）
        ReadBytes(data, info.zoneType, INT32SIZE);

        //指定变量位置0 =不指定，所有数据都位于节点上。1 =指定
        int specifyVarLocation = -1;
        ReadBytes(data, specifyVarLocation, INT32SIZE);

        //指定变量位置读取
        if (specifyVarLocation == 1){
            int dimNumber = 0;
            for (int i = 0; i < NumberOfVariables; i++)
            {
                int VarLocation = -1;
                ReadBytes(data, VarLocation, INT32SIZE);
                //获取变量位置（0 = 节点，1 = 单元中心）
                info.VariableTypes.append(VarLocation);
                if (VarLocation == 0)
                    dimNumber++;
            }
            handerInfo.dim = dimNumber;
        }
        //不指定var位置读取
        else if (specifyVarLocation == 0)
        {
            //暂定dimNumber == 0 则认为是粒子文件,并且云图位于节点上
            for (int i = 0; i < NumberOfVariables; i++)
            {
                info.VariableTypes.append(0);
            }
            handerInfo.dim = 3;
        }

        //no face neighbers（0=FALSE，1=TRUE）
        int AreRawLocal = -1;
        ReadBytes(data, AreRawLocal, INT32SIZE);

        //no miscellaneous（0 =当地一对一,1 =当地一对多的关系,2=全局1对1,3=全局1对多）
        int numberOfMiscellaneous = -1;
        ReadBytes(data, numberOfMiscellaneous, INT32SIZE);

        if (numberOfMiscellaneous != 0)
        {
            //跳过
        }

        //如果是有序分区
        if (info.zoneType == 0)
        {
            int IMax = -1, JMax = -1, KMax = -1;
            ReadBytes(data, IMax, INT32SIZE);
            ReadBytes(data, JMax, INT32SIZE);
            ReadBytes(data, KMax, INT32SIZE);
            //暂时作为粒子的字段
            info.numberPoints = IMax;
        }
        //如果是有限元分区
        else if (info.zoneType != 0)
        {
            //获取点的数量
            ReadBytes(data, info.numberPoints, INT32SIZE);

            //如果分区的类型是多边形或多面体
            if (info.zoneType == FEPOLYGON || info.zoneType == FEPOLYHEDRON){
                //未完成 暂时不用
            }

            //
            ReadBytes(data, info.numberCells, INT32SIZE);

            int ICellDim = -1, JCellDim = -1, KCellDim = -1;
            //for future use, set to zero.
            ReadBytes(data, ICellDim, INT32SIZE);
            ReadBytes(data, JCellDim, INT32SIZE);
            ReadBytes(data, KCellDim, INT32SIZE);
            //未完成 暂时不用
        }

        //获取分区是否有辅助值格式（0=不再有辅助名称/值对，1=后续的辅助名称/值对）
        int auxData = -1;
        ReadBytes(data, auxData, INT32SIZE); 

        //如果包含辅助值格式
        if (auxData != 0){
            //暂未遇到该格式数据，暂不处理
            qDebug() << "pointerIndex : " << DirectionHead;
            for (int i = 0; i < 12; i++)
            {
                QString nameStr = QString(ReadBytes(data, INT32SIZE, 0));
                qDebug() << nameStr;
            }
            int auxiliaryValue = -1;
            ReadBytes(data, auxiliaryValue, INT32SIZE);

            for (int i = 0; i < 12; i++)
            {
                QString ValueStr = QString(ReadBytes(data, INT32SIZE, 0));
                qDebug() << ValueStr;
            }
        }

        zoneInfos.append(info);

        while (true){
            //获取标记
            float endHeader = -1;
            ReadBytes(data, endHeader, FLOAT32SSIZE);

            //判断是否有其他分区
            if (endHeader == 299.0){
                return ReadZones(data);
            }
            //判断是否读取到标题与数据的分隔符: EOHMARKER，值=357.0
            else if (endHeader == 357.0) {
                return endHeader;
            }
            //判断文件是否读取结束
            else if (IsReadEnd(data)){
                return -1;
            }
        }
    }

    float FITKPltReader::ReadDataSection(QByteArray & data)
    {
        DataInfo dataInfo{};
        //获取所有场量类型（1= Float, 2=Double, 3=LongInt，4 = ShortInt, 5 = Byte, 6 = Bit）
        for (int i = 0; i < NumberOfVariables; i++){
            int variableMemoryType = -1;
            ReadBytes(data, variableMemoryType, INT32SIZE);
            qDebug() << "VarLocation Type:  " << variableMemoryType;
        }

        //获取是否有被动场量
        int passiveVariable = -1;
        ReadBytes(data, passiveVariable, INT32SIZE);

        //如果有被动场量
        if (passiveVariable != 0){
            //int32*NV
            //Is variable passive : 0 = no, 1 = yes
            //(Omit entirely if “Has passive variables” is 0)
        }

        //获取是否具有共享场量
        int sharingVariable = -1;
        ReadBytes(data, sharingVariable, INT32SIZE);

        //如果具有共享场量
        if (sharingVariable != 0){
            //int32*NV
            //Zero based zone number to share variable with
            //(relative to this datafile). (-1 = no sharing).
            //(Omit entirely if “Has variable sharing” is 0).
        }

        //获取，共享连接的基于零的区域号列表：4个字节（-1 =不共享，多边形与多面体使用这个区域编号共享映射面数据）
        int sharingInfo = -1; //-1 = no sharing
        ReadBytes(data, sharingInfo, INT32SIZE);

        //场量最大最小值获取
        //X、Y、Z方向上点的最大最小值获取
        double Min = -1, Max = -1;
        ReadBytes(data, Min, DOUBLESIZE);
        ReadBytes(data, Max, DOUBLESIZE);
        dataInfo.nodeXMinMax.min = Min;
        dataInfo.nodeXMinMax.max = Max;
        ReadBytes(data, Min, DOUBLESIZE);
        ReadBytes(data, Max, DOUBLESIZE);
        dataInfo.nodeYMinMax.min = Min;
        dataInfo.nodeYMinMax.max = Max;
        if (handerInfo.dim == 3){
            ReadBytes(data, Min, DOUBLESIZE);
            ReadBytes(data, Max, DOUBLESIZE);
            dataInfo.nodeZMinMax.min = Min;
            dataInfo.nodeZMinMax.max = Max;
        }
        //其余场量最大最小值获取
        for (int i = 0; i < NumberOfVariables - handerInfo.dim; i++)
        {
            double varMin = -1, varMax = -1;
            ReadBytes(data, varMin, DOUBLESIZE);
            ReadBytes(data, varMax, DOUBLESIZE);
            dataInfo.VariablesMinMax.append(Vector2(varMin, varMax));

        }

        ///开空间
        for (int i = 0; i < zoneInfos[readDataIndex].numberPoints; i++)
        {
            dataInfo.Points.append(Vector3());
        }

        float PointData = 0;
        QList<float> varXDatas{};
        QList<float> varYDatas{};
        QList<float> varZDatas{};
        //////////读取points
        for (int i = 0; i < zoneInfos[readDataIndex].numberPoints; i++)
        {
            ReadBytes(data, PointData, FLOAT32SSIZE);
            dataInfo.Points[i].x = PointData;
            varXDatas.append(PointData);
        }

        for (int i = 0; i < zoneInfos[readDataIndex].numberPoints; i++)
        {
            ReadBytes(data, PointData, FLOAT32SSIZE);
            dataInfo.Points[i].y = PointData;
            varYDatas.append(PointData);
        }

        //Append variables X Y Z
        dataInfo.variablesData.append(varXDatas);
        dataInfo.variablesData.append(varYDatas);
        if (handerInfo.dim == 3)
        {
            for (int i = 0; i < zoneInfos[readDataIndex].numberPoints; i++)
            {
                ReadBytes(data, PointData, FLOAT32SSIZE);
                dataInfo.Points[i].z = PointData;
                varZDatas.append(PointData);
            }
            dataInfo.variablesData.append(varZDatas);
        }

        //////////

        //////////Read Variables
        float varData = 0;
        for (int i = 0; i < NumberOfVariables - handerInfo.dim; i++)
        {
            float number = 0;
            //Save Variable List
            QList<float> varDatas{};


            if (zoneInfos[readDataIndex].VariableTypes[i + handerInfo.dim] == 1)
                number = zoneInfos[readDataIndex].numberCells;
            else
                number = zoneInfos[readDataIndex].numberPoints;

            for (int j = 0; j < number; j++)
            {
                ReadBytes(data, varData, FLOAT32SSIZE);
                varDatas.append(varData);
            }
            dataInfo.variablesData.append(varDatas);
        }
        //////////

        //////////Read Cells
        int numPoints = GetNumberOfPointSize(zoneInfos[readDataIndex].zoneType);

        int numberCells = zoneInfos[readDataIndex].numberCells;
        int value = 0;
        if (handerInfo.dim == 2)
        {
            for (int i = 0; i < numberCells * numPoints; i++)
            {
                ReadBytes(data, value, INT32SIZE);
                //qDebug() << DirectionHead << i << value;
                dataInfo.Cells.append(value);
            }
        }
        else if (handerInfo.dim == 3)
        {
            for (int i = 0; i < numberCells * numPoints; i++)
            {
                ReadBytes(data, value, INT32SIZE);
                dataInfo.Cells.append(value);
            }
        }
        dataInfos.append(dataInfo);
        readDataIndex++;

        while (readDataIndex < zoneInfos.size())
        {
            //end header
            float endHeader = -1;
            ReadBytes(data, endHeader, FLOAT32SSIZE);
            if (endHeader == 299.0)
            {
                return ReadDataSection(data);
            }
            else if (endHeader == 357.0) {
                return endHeader;
            }
            else if (IsReadEnd(data))
            {
                return -1;
            }
        }
        return 357.0f;
    }

    int FITKPltReader::GetNumberOfPointSize(int cellType)
    {
        int numCellPnts = 0;
        if (cellType == FE_BRICK)
        {
            numCellPnts = 8;
        }
        else if (cellType == FE_TRIANGLE)
        {
            numCellPnts = 3;
        }
        else if (cellType == FE_QUAD)
        {
            numCellPnts = 4;
        }
        else if (cellType == FE_TET)
        {
            numCellPnts = 4;
        }
        else if (cellType == FE_LINE_SEG)
        {
            numCellPnts = 2;
        }
        else
        {
            qDebug() << "Unknown cell type for a zone.";
            numCellPnts = 0;
        }
        return numCellPnts;
    }
}