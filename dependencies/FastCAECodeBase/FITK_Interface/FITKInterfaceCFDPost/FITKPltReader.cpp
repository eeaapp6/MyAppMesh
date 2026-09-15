/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPltReader.h"

/**
 * @file     FITKPltReader.cpp
 * @brief    Tecplot 二进制 plt 文件读取器实现。
 * @author   BaGuijun (baguijun@163.com)
 * @date     2026-07-03
 */

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
#include <vtkCellType.h>
#include <vtkCellArray.h>
#include <vtkIdList.h>

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#define MAXLENGTH 524280
#define INT32SIZE 4
#define FLOAT32SSIZE 4
#define DOUBLESIZE 8
#define INT64SIZE 8
#define FEPOLYGON 6
#define FEPOLYHEDRON 7

namespace Interface
{
    /**
     * @brief    创建读取器实例。
     * @return   FITKPltReader* 读取器实例
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    FITKPltReader* FITKPltReader::New()
    {
        FITKPltReader* reader = new FITKPltReader;
        reader->InitializeObjectBase();
        return reader;
    }

    /**
     * @brief    输出对象调试信息。
     * @param[i] os 输出流
     * @param[i] indent 缩进信息
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    void FITKPltReader::PrintSelf(ostream& os, vtkIndent indent)
    {
        Q_UNUSED(os)
        Q_UNUSED(indent)
    }

    /**
     * @brief    构造 plt 读取器。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    FITKPltReader::FITKPltReader()
    {
        this->SetNumberOfInputPorts(0);
        this->SetNumberOfOutputPorts(1);
        this->FileName = nullptr;
        this->_readErrorCode = NoError;
        this->TopologyDim = 0;
        this->GeometryDim = 0;
    }

    /**
     * @brief    析构 plt 读取器。
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    FITKPltReader::~FITKPltReader()
    {
    }

    /**
     * @brief    分发 VTK 管线请求。
     * @param[i] request 请求对象
     * @param[i] inputVector 输入信息
     * @param[i] outputVector 输出信息
     * @return   int 是否处理成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    int FITKPltReader::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
    {
        if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
        {
            return this->RequestInformation(request, inputVector, outputVector);
        }
        if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
        {
            return this->RequestUpdateExtent(request, inputVector, outputVector);
        }
        if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
        {
            return this->RequestData(request, inputVector, outputVector);
        }
        return this->Superclass::ProcessRequest(request, inputVector, outputVector);
    }

    /**
     * @brief    读取 plt 文件并构造多块输出数据。
     * @param[i] vtkInformation VTK 请求信息
     * @param[i] vtkInformationVector 输入信息
     * @param[i] vtkInformationVector 输出信息
     * @return   int 是否读取成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    int FITKPltReader::RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector* outputVector)
    {
        vtkMultiBlockDataSet* output = vtkMultiBlockDataSet::GetData(outputVector);

        zoneInfos.clear();
        dataInfos.clear();
        handerInfo = HanderInfo();
        NumberOfVariables = 0;
        DirectionHead = 0;
        readZoneIndex = 0;
        readDataIndex = 0;
        _readErrorCode = NoError;
        TopologyDim = 0;
        GeometryDim = 0;

        QFileInfo info(FileName);
        Q_UNUSED(info)
        QFile file(FileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QByteArray m_data = file.readAll();
        Reader(m_data);
        if (_readErrorCode != NoError)
        {
            return 0;
        }

        // 每个分区先构造点坐标，再根据分区类型选择对应的 VTK 数据对象。
        output->SetNumberOfBlocks(zoneInfos.size());
        for (int i = 0; i < zoneInfos.size(); i++)
        {
            vtkSmartPointer<vtkPoints> pntCords = vtkSmartPointer<vtkPoints>::New();
            pntCords->SetDataTypeToDouble();
            int numberPoints = zoneInfos[i].numberPoints;
            for (int j = 0; j < numberPoints; j++)
            {
                pntCords->InsertNextPoint(dataInfos[i].Points[j].x, dataInfos[i].Points[j].y, dataInfos[i].Points[j].z);
            }

            // ORDERED 分区直接映射为 StructuredGrid，有限元分区继续走 UnstructuredGrid。
            if (zoneInfos[i].zoneType == ORDERED)
            {
                vtkSmartPointer<vtkStructuredGrid> structured = vtkSmartPointer<vtkStructuredGrid>::New();
                structured->SetDimensions(zoneInfos[i].orderedIMax, zoneInfos[i].orderedJMax, zoneInfos[i].orderedKMax);
                structured->SetPoints(pntCords);
                GetArraysFromBlockPackingZone(i, zoneInfos[i].numberPoints, zoneInfos[i].numberCells, structured->GetPoints(), structured->GetPointData(), structured->GetCellData(), output);
                output->SetBlock(i, structured);
            }
            else
            {
                vtkSmartPointer<vtkUnstructuredGrid> unstruct = vtkSmartPointer<vtkUnstructuredGrid>::New();
                unstruct->SetPoints(pntCords);

                GetUnstructuredGridCells(i, zoneInfos[i].numberCells, zoneInfos[i].zoneType, unstruct, output);
                GetArraysFromBlockPackingZone(i, zoneInfos[i].numberPoints, zoneInfos[i].numberCells, unstruct->GetPoints(), unstruct->GetPointData(), unstruct->GetCellData(), output);

                output->SetBlock(i, unstruct);
            }
            output->GetMetaData(i)->Set(vtkCompositeDataSet::NAME(), zoneInfos[i].ZoneName.toLocal8Bit().constData());
        }

        return 1;
    }

    /**
     * @brief    读取 INT32 编码字符串或指定长度字节流。
     * @param[i] data 源数据
     * @param[i] typeSize 单个元素字节数
     * @param[i] readLengh 读取长度，0 表示读取到字符串结束标记
     * @return   QByteArray 读取结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    QByteArray FITKPltReader::ReadBytes(QByteArray & data, size_t typeSize, size_t readLengh)
    {
        QByteArray buffer{};
        QByteArray buffers{};
        if (readLengh == 0)
        {
            int end = 1;
            while (end != 0)
            {
                buffer = data.mid(DirectionHead, typeSize);
                memcpy(&end, buffer.data(), typeSize);
                DirectionHead += typeSize;
                if (end == 0)
                {
                    break;
                }
                buffers += static_cast<char>(end);
            }
        }
        return buffers;
    }

    /**
     * @brief    读取固定长度字节流。
     * @param[i] data 源数据
     * @param[i] readLengh 读取长度
     * @return   QByteArray 读取结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    QByteArray FITKPltReader::ReadBytes(QByteArray & data, size_t readLengh)
    {
        QByteArray buffer = data.mid(DirectionHead, readLengh);
        DirectionHead += readLengh;
        return buffer;
    }

    /**
     * @brief    读取固定长度字节流并写入指定变量。
     * @param[i] data 源数据
     * @param[o] backData 输出值
     * @param[i] readLengh 读取长度
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    template<typename T2>
    void FITKPltReader::ReadBytes(QByteArray & data, T2 & backData, size_t readLengh)
    {
        QByteArray buffer = data.mid(DirectionHead, readLengh);
        memcpy(&backData, buffer.data(), readLengh);
        DirectionHead += readLengh;
    }

    /**
     * @brief    读取 Tecplot INT32 字符串。
     * @param[i] data 源数据
     * @return   QString 字符串结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    QString FITKPltReader::ReadInt32String(QByteArray & data)
    {
        return QString::fromLocal8Bit(ReadBytes(data, INT32SIZE, 0));
    }

    /**
     * @brief    判断读取指针是否超出数据末尾。
     * @param[i] data 源数据
     * @return   bool 是否已经越界
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::IsReadEnd(QByteArray & data)
    {
        return DirectionHead > data.size();
    }

    /**
     * @brief    将 FE 分区拓扑写入非结构网格。
     * @param[i] zoneIndex 分区索引
     * @param[i] numberCells 单元数量
     * @param[i] cellType 单元类型
     * @param[o] unstrctGrid 非结构网格对象
     * @param[i] multZone 多块数据对象
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    void FITKPltReader::GetUnstructuredGridCells(int zoneIndex, int numberCells, int cellType, vtkUnstructuredGrid * unstrctGrid, vtkMultiBlockDataSet* multZone)
    {
        Q_UNUSED(multZone)
        if (unstrctGrid == nullptr)
        {
            return;
        }

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
            TopologyDim = vtkMath::Max(TopologyDim, 1);
        }
        else
        {
            qDebug() << "Unknown cell type for a zone:" << cellType;
            return;
        }

        vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
        int* cellTypes = new int[numberCells];
        for (int c = 0; c < numberCells; c++)
        {
            cellTypes[c] = theCellType;
            vtkSmartPointer<vtkIdList> cellIDs = vtkSmartPointer<vtkIdList>::New();
            for (int j = 0; j < numCellPnts; j++)
            {
                vtkIdType id = dataInfos[zoneIndex].Cells[c * numCellPnts + j];
                cellIDs->InsertNextId(id);
            }
            cells->InsertNextCell(cellIDs);
        }

        unstrctGrid->SetCells(cellTypes, cells);
        delete[] cellTypes;
        cellTypes = nullptr;
    }

    /**
     * @brief    把读取出的变量数据写入点属性或单元属性。
     * @param[i] zoneIndex 分区索引
     * @param[i] numNodes 点数量
     * @param[i] numCells 单元数量
     * @param[i] theNodes 点对象
     * @param[o] nodeData 点属性
     * @param[o] cellData 单元属性
     * @param[i] multZone 多块数据对象
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    void FITKPltReader::GetArraysFromBlockPackingZone(int zoneIndex, int numNodes, int numCells, vtkPoints * theNodes, vtkPointData * nodeData, vtkCellData * cellData, vtkMultiBlockDataSet * multZone)
    {
        Q_UNUSED(numNodes)
        Q_UNUSED(numCells)
        Q_UNUSED(multZone)
        if (!theNodes || !nodeData || !cellData || zoneIndex >= dataInfos.size())
        {
            qDebug() << "File not open, errors with reading, or nullptr vtkPoints / vtkPointData / vtkCellData /ReadDataError.";
            _readErrorCode = PointError;
            return;
        }

        int variableSizes = dataInfos[zoneIndex].variablesData.size();
        for (int i = 0; i < variableSizes; i++)
        {
            vtkSmartPointer<vtkDoubleArray> array = vtkSmartPointer<vtkDoubleArray>::New();
            int variableSize = dataInfos[zoneIndex].variablesData[i].size();
            array->SetName(handerInfo.VariableNames[i].toLocal8Bit().constData());
            array->SetNumberOfValues(variableSize);
            for (int j = 0; j < variableSize; j++)
            {
                array->SetValue(j, dataInfos[zoneIndex].variablesData[i][j]);
            }

            // 根据变量位置定义决定写入 PointData 还是 CellData。
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

    /**
     * @brief    读取 plt 主体内容。
     * @param[i] data 文件二进制内容
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    void FITKPltReader::Reader(QByteArray &data)
    {
        // 文件头固定以魔数、字节序标记、标题和变量名列表开始。
        handerInfo.Version = ReadBytes(data, 8);

        int integer = 0;
        ReadBytes(data, integer, INT32SIZE);
        if (integer != 1)
        {
            qDebug() << "Unsupported Tecplot byte order marker:" << integer;
            _readErrorCode = PointError;
            return;
        }

        ReadBytes(data, fileType, INT32SIZE);
        handerInfo.DataTitle = ReadInt32String(data);
        ReadBytes(data, NumberOfVariables, INT32SIZE);
        for (int i = 0; i < NumberOfVariables; i++)
        {
            handerInfo.VariableNames.append(ReadInt32String(data));
        }

        // Header 段由不同 marker 串联，直到遇到 EOH marker=357.0 为止。
        while (!IsReadEnd(data))
        {
            float marker = -1.0f;
            ReadBytes(data, marker, FLOAT32SSIZE);
            if (marker == 299.0f || marker == 298.0f)
            {
                if (!ReadZoneHeader(data, marker))
                {
                    _readErrorCode = PointError;
                    return;
                }
                continue;
            }
            if (marker == 399.0f)
            {
                if (!SkipGeometry(data))
                {
                    _readErrorCode = PointError;
                    return;
                }
                continue;
            }
            if (marker == 499.0f)
            {
                if (!SkipText(data))
                {
                    _readErrorCode = PointError;
                    return;
                }
                continue;
            }
            if (marker == 599.0f)
            {
                if (!SkipCustomLabel(data))
                {
                    _readErrorCode = PointError;
                    return;
                }
                continue;
            }
            if (marker == 699.0f)
            {
                if (!SkipUserRecord(data))
                {
                    _readErrorCode = PointError;
                    return;
                }
                continue;
            }
            if (marker == 799.0f)
            {
                if (!SkipDataSetAuxData(data))
                {
                    _readErrorCode = PointError;
                    return;
                }
                continue;
            }
            if (marker == 899.0f)
            {
                if (!SkipVariableAuxData(data))
                {
                    _readErrorCode = PointError;
                    return;
                }
                continue;
            }
            if (marker == 357.0f)
            {
                break;
            }

            qDebug() << "Unsupported Tecplot header marker:" << marker;
            _readErrorCode = PointError;
            return;
        }

        // Data 段的 zone 顺序必须与头段中的 zone 定义顺序一致。
        for (int zoneIndex = 0; zoneIndex < zoneInfos.size(); zoneIndex++)
        {
            float zoneMarker = -1.0f;
            ReadBytes(data, zoneMarker, FLOAT32SSIZE);
            if (zoneMarker != 299.0f && zoneMarker != 298.0f)
            {
                qDebug() << "Invalid Tecplot zone data marker:" << zoneMarker << "zone:" << zoneIndex;
                _readErrorCode = PointError;
                return;
            }
            if (!ReadZoneData(data, zoneIndex, zoneMarker))
            {
                _readErrorCode = PointError;
                return;
            }
        }
    }

    /**
     * @brief    读取单个 zone 头信息。
     * @param[i] data 源数据
     * @param[i] zoneMarker 分区头标记
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::ReadZoneHeader(QByteArray & data, float zoneMarker)
    {
        ZoneInfo info{};
        info.zoneMarker = static_cast<int>(zoneMarker);
        info.ZoneName = ReadInt32String(data);

        int parentZone = -1;
        ReadBytes(data, parentZone, INT32SIZE);

        int strandID = -1;
        ReadBytes(data, strandID, INT32SIZE);

        double solutionTime = -1.0;
        ReadBytes(data, solutionTime, DOUBLESIZE);

        ReadBytes(data, info.zoneColor, INT32SIZE);
        ReadBytes(data, info.zoneType, INT32SIZE);

        int specifyVarLocation = -1;
        ReadBytes(data, specifyVarLocation, INT32SIZE);
        if (specifyVarLocation == 1)
        {
            for (int i = 0; i < NumberOfVariables; i++)
            {
                int varLocation = -1;
                ReadBytes(data, varLocation, INT32SIZE);
                info.VariableTypes.append(varLocation);
            }
        }
        else if (specifyVarLocation == 0)
        {
            for (int i = 0; i < NumberOfVariables; i++)
            {
                info.VariableTypes.append(0);
            }
        }
        else
        {
            qDebug() << "Unsupported specifyVarLocation value:" << specifyVarLocation;
            return false;
        }

        ReadBytes(data, info.hasRawLocalFaceNeighbors, INT32SIZE);
        ReadBytes(data, info.numMiscFaceConnections, INT32SIZE);
        if (info.numMiscFaceConnections != 0)
        {
            ReadBytes(data, info.faceNeighborMode, INT32SIZE);
        }

        // ORDERED 分区使用 IJK 点尺寸，FE 分区使用点数/单元数描述。
        if (info.zoneType == ORDERED)
        {
            ReadBytes(data, info.orderedIMax, INT32SIZE);
            ReadBytes(data, info.orderedJMax, INT32SIZE);
            ReadBytes(data, info.orderedKMax, INT32SIZE);
            info.numberPoints = GetOrderedPointCount(info);
            info.numberCells = GetOrderedCellCount(info);
        }
        else
        {
            ReadBytes(data, info.numberPoints, INT32SIZE);
            // v191 对 polygon/polyhedron 的面数和面节点总数使用 64 位整数。
            if (info.zoneType == FEPOLYGON || info.zoneType == FEPOLYHEDRON)
            {
                bool isInt64 = (zoneMarker == 298.0f);
                info.numFaces = ReadIntegerValue(data, isInt64);
                info.totalFaceNodes = ReadIntegerValue(data, isInt64);
                ReadBytes(data, info.totalBoundaryFaces, INT32SIZE);
                ReadBytes(data, info.totalBoundaryConnections, INT32SIZE);
            }

            ReadBytes(data, info.numberCells, INT32SIZE);

            int iCellDim = 0;
            int jCellDim = 0;
            int kCellDim = 0;
            ReadBytes(data, iCellDim, INT32SIZE);
            ReadBytes(data, jCellDim, INT32SIZE);
            ReadBytes(data, kCellDim, INT32SIZE);

            if (info.numMiscFaceConnections != 0)
            {
                ReadBytes(data, info.faceNeighborsCompletelySpecified, INT32SIZE);
            }
        }

        if (!SkipZoneAuxData(data))
        {
            return false;
        }

        handerInfo.dim = vtkMath::Max(handerInfo.dim, GetGeometryDimension(info));
        zoneInfos.append(info);
        return true;
    }

    /**
     * @brief    读取单个 zone 的变量数据与共享信息。
     * @param[i] data 源数据
     * @param[i] zoneIndex 分区索引
     * @param[i] zoneMarker 分区标记
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::ReadZoneData(QByteArray & data, int zoneIndex, float zoneMarker)
    {
        Q_UNUSED(zoneMarker)
        DataInfo dataInfo{};

        QList<int> variableMemoryTypes{};
        variableMemoryTypes.reserve(NumberOfVariables);
        for (int i = 0; i < NumberOfVariables; i++)
        {
            int variableMemoryType = -1;
            ReadBytes(data, variableMemoryType, INT32SIZE);
            variableMemoryTypes.append(variableMemoryType);
        }

        QVector<int> passiveVariables(NumberOfVariables, 0);
        int passiveVariable = -1;
        ReadBytes(data, passiveVariable, INT32SIZE);
        if (passiveVariable != 0)
        {
            for (int i = 0; i < NumberOfVariables; i++)
            {
                ReadBytes(data, passiveVariables[i], INT32SIZE);
            }
        }

        QVector<int> sharingVariables(NumberOfVariables, -1);
        int sharingVariable = -1;
        ReadBytes(data, sharingVariable, INT32SIZE);
        if (sharingVariable != 0)
        {
            for (int i = 0; i < NumberOfVariables; i++)
            {
                ReadBytes(data, sharingVariables[i], INT32SIZE);
            }
        }

        int shareConnectivityZone = -1;
        ReadBytes(data, shareConnectivityZone, INT32SIZE);

        QList<QList<double>> zoneVariables{};
        zoneVariables.reserve(NumberOfVariables);
        for (int i = 0; i < NumberOfVariables; i++)
        {
            zoneVariables.append(QList<double>());
        }

        // 先读取变量的最小最大值描述，只有非 passive 且非 shared 的变量才真实存储。
        for (int i = 0; i < NumberOfVariables; i++)
        {
            bool isPassive = (passiveVariables[i] != 0);
            bool isShared = (sharingVariables[i] >= 0);
            if (!isPassive && !isShared)
            {
                double varMin = -1.0;
                double varMax = -1.0;
                ReadBytes(data, varMin, DOUBLESIZE);
                ReadBytes(data, varMax, DOUBLESIZE);
                dataInfo.VariablesMinMax.append(Vector2(varMin, varMax));
            }
            else
            {
                dataInfo.VariablesMinMax.append(Vector2());
            }
        }

        // 再按变量位置和共享状态读取真实数据或复用前序分区数据。
        for (int i = 0; i < NumberOfVariables; i++)
        {
            int valueCount = GetVariableValueCount(zoneInfos[zoneIndex], i);
            if (passiveVariables[i] != 0)
            {
                QList<double> values{};
                values.reserve(valueCount);
                for (int valueIndex = 0; valueIndex < valueCount; valueIndex++)
                {
                    values.append(0.0);
                }
                zoneVariables[i] = values;
                continue;
            }
            if (sharingVariables[i] >= 0)
            {
                int shareZone = sharingVariables[i];
                if (shareZone < 0 || shareZone >= dataInfos.size() || i >= dataInfos[shareZone].variablesData.size())
                {
                    qDebug() << "Invalid shared variable zone:" << shareZone << "variable:" << i;
                    return false;
                }
                zoneVariables[i] = dataInfos[shareZone].variablesData[i];
                continue;
            }

            QList<double> values{};
            values.reserve(valueCount);
            for (int j = 0; j < valueCount; j++)
            {
                values.append(ReadNumericValueAsDouble(data, variableMemoryTypes[i]));
            }
            zoneVariables[i] = values;
        }

        BuildZoneData(zoneIndex, zoneVariables, dataInfo);

        if (!ReadConnectivity(data, zoneIndex, shareConnectivityZone, dataInfo))
        {
            return false;
        }

        dataInfos.append(dataInfo);
        readDataIndex++;
        return true;
    }

    /**
     * @brief    获取有限元单元的节点数量。
     * @param[i] cellType 单元类型
     * @return   int 节点数量
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
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

    /**
     * @brief    获取分区几何维度。
     * @param[i] zoneInfo 分区信息
     * @return   int 几何维度
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    int FITKPltReader::GetGeometryDimension(const ZoneInfo &zoneInfo)
    {
        if (zoneInfo.zoneType == ORDERED)
        {
            if (zoneInfo.orderedKMax > 1)
            {
                return 3;
            }
            if (zoneInfo.orderedJMax > 1)
            {
                return 2;
            }
            return 1;
        }
        if (zoneInfo.zoneType == FE_LINE_SEG)
        {
            return 1;
        }
        if (zoneInfo.zoneType == FE_TRIANGLE || zoneInfo.zoneType == FE_QUAD || zoneInfo.zoneType == FEPOLYGON)
        {
            return 2;
        }
        return 3;
    }

    /**
     * @brief    计算 ORDERED 分区点数量。
     * @param[i] zoneInfo 分区信息
     * @return   int 点数量
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    int FITKPltReader::GetOrderedPointCount(const ZoneInfo &zoneInfo)
    {
        return zoneInfo.orderedIMax * zoneInfo.orderedJMax * zoneInfo.orderedKMax;
    }

    /**
     * @brief    计算 ORDERED 分区单元数量。
     * @param[i] zoneInfo 分区信息
     * @return   int 单元数量
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    int FITKPltReader::GetOrderedCellCount(const ZoneInfo &zoneInfo)
    {
        if (zoneInfo.orderedJMax <= 1 && zoneInfo.orderedKMax <= 1)
        {
            return vtkMath::Max(zoneInfo.orderedIMax - 1, 0);
        }
        if (zoneInfo.orderedKMax <= 1)
        {
            return vtkMath::Max(zoneInfo.orderedIMax - 1, 0) * vtkMath::Max(zoneInfo.orderedJMax - 1, 0);
        }
        return vtkMath::Max(zoneInfo.orderedIMax - 1, 0) * vtkMath::Max(zoneInfo.orderedJMax - 1, 0) * vtkMath::Max(zoneInfo.orderedKMax - 1, 0);
    }

    /**
     * @brief    获取指定变量在当前分区应读取的数据数量。
     * @param[i] zoneInfo 分区信息
     * @param[i] variableIndex 变量索引
     * @return   int 数据数量
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    int FITKPltReader::GetVariableValueCount(const ZoneInfo &zoneInfo, int variableIndex)
    {
        bool isCellCentered = (variableIndex < zoneInfo.VariableTypes.size() && zoneInfo.VariableTypes[variableIndex] == 1);
        if (!isCellCentered)
        {
            return zoneInfo.numberPoints;
        }
        if (zoneInfo.zoneType != ORDERED)
        {
            return zoneInfo.numberCells;
        }
        return zoneInfo.orderedIMax * zoneInfo.orderedJMax * vtkMath::Max(zoneInfo.orderedKMax - 1, 1);
    }

    /**
     * @brief    按 Tecplot 类型读取一个数值并转换为 double。
     * @param[i] data 源数据
     * @param[i] variableType Tecplot 变量类型
     * @return   double 读取结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    double FITKPltReader::ReadNumericValueAsDouble(QByteArray &data, int variableType)
    {
        if (variableType == 1)
        {
            float value = 0.0f;
            ReadBytes(data, value, FLOAT32SSIZE);
            return value;
        }
        if (variableType == 2)
        {
            double value = 0.0;
            ReadBytes(data, value, DOUBLESIZE);
            return value;
        }
        if (variableType == 3)
        {
            int value = 0;
            ReadBytes(data, value, INT32SIZE);
            return value;
        }
        if (variableType == 4)
        {
            short value = 0;
            ReadBytes(data, value, sizeof(short));
            return value;
        }
        if (variableType == 5)
        {
            unsigned char value = 0;
            ReadBytes(data, value, sizeof(unsigned char));
            return value;
        }
        if (variableType == 6)
        {
            unsigned char value = 0;
            ReadBytes(data, value, sizeof(unsigned char));
            return value == 0 ? 0.0 : 1.0;
        }

        qDebug() << "Unsupported Tecplot variable data type:" << variableType;
        _readErrorCode = PointError;
        return 0.0;
    }

    /**
     * @brief    按 zone header 版本读取 32 位或 64 位整数。
     * @param[i] data 源数据
     * @param[i] isInt64 是否按 64 位读取
     * @return   qint64 读取结果
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    qint64 FITKPltReader::ReadIntegerValue(QByteArray &data, bool isInt64)
    {
        if (isInt64)
        {
            qint64 value = 0;
            ReadBytes(data, value, INT64SIZE);
            return value;
        }

        int value = 0;
        ReadBytes(data, value, INT32SIZE);
        return value;
    }

    /**
     * @brief    把变量缓存转换为点坐标和最值统计。
     * @param[i] zoneIndex 分区索引
     * @param[i] zoneVariables 原始变量缓存
     * @param[o] dataInfo 输出数据缓存
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    void FITKPltReader::BuildZoneData(int zoneIndex, const QList<QList<double>> &zoneVariables, DataInfo &dataInfo)
    {
        const ZoneInfo &zoneInfo = zoneInfos[zoneIndex];
        int pointCount = zoneInfo.numberPoints;
        dataInfo.variablesData = zoneVariables;
        dataInfo.Points.reserve(pointCount);

        // Tecplot 读入后默认将前三个节点变量解释为几何坐标 XYZ。
        // 这里不能用拓扑维度限制 Z 坐标写入：FEQUAD/FETRIANGLE 等二维面分区
        // 仍然可能嵌入三维空间，如果把 Z 强制置零，就会表现成投影到 Z=0 平面。
        for (int pointIndex = 0; pointIndex < pointCount; pointIndex++)
        {
            Vector3 point{};
            if (NumberOfVariables > 0 && pointIndex < zoneVariables[0].size())
            {
                point.x = zoneVariables[0][pointIndex];
            }
            if (NumberOfVariables > 1 && pointIndex < zoneVariables[1].size())
            {
                point.y = zoneVariables[1][pointIndex];
            }
            if (NumberOfVariables > 2 && pointIndex < zoneVariables[2].size())
            {
                point.z = zoneVariables[2][pointIndex];
            }
            dataInfo.Points.append(point);
        }

        // 统计坐标轴最值，便于后续范围计算和显示使用。
        if (!zoneVariables.isEmpty() && !zoneVariables[0].isEmpty())
        {
            dataInfo.nodeXMinMax.min = zoneVariables[0][0];
            dataInfo.nodeXMinMax.max = zoneVariables[0][0];
            for (int i = 0; i < zoneVariables[0].size(); i++)
            {
                dataInfo.nodeXMinMax.min = vtkMath::Min(dataInfo.nodeXMinMax.min, zoneVariables[0][i]);
                dataInfo.nodeXMinMax.max = vtkMath::Max(dataInfo.nodeXMinMax.max, zoneVariables[0][i]);
            }
        }
        if (zoneVariables.size() > 1 && !zoneVariables[1].isEmpty())
        {
            dataInfo.nodeYMinMax.min = zoneVariables[1][0];
            dataInfo.nodeYMinMax.max = zoneVariables[1][0];
            for (int i = 0; i < zoneVariables[1].size(); i++)
            {
                dataInfo.nodeYMinMax.min = vtkMath::Min(dataInfo.nodeYMinMax.min, zoneVariables[1][i]);
                dataInfo.nodeYMinMax.max = vtkMath::Max(dataInfo.nodeYMinMax.max, zoneVariables[1][i]);
            }
        }
        if (zoneVariables.size() > 2 && !zoneVariables[2].isEmpty())
        {
            dataInfo.nodeZMinMax.min = zoneVariables[2][0];
            dataInfo.nodeZMinMax.max = zoneVariables[2][0];
            for (int i = 0; i < zoneVariables[2].size(); i++)
            {
                dataInfo.nodeZMinMax.min = vtkMath::Min(dataInfo.nodeZMinMax.min, zoneVariables[2][i]);
                dataInfo.nodeZMinMax.max = vtkMath::Max(dataInfo.nodeZMinMax.max, zoneVariables[2][i]);
            }
        }
    }

    /**
     * @brief    读取或跳过当前分区的拓扑和邻接数据。
     * @param[i] data 源数据
     * @param[i] zoneIndex 分区索引
     * @param[i] shareZone 共享连接分区索引
     * @param[o] dataInfo 输出数据缓存
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::ReadConnectivity(QByteArray &data, int zoneIndex, int shareZone, DataInfo &dataInfo)
    {
        const ZoneInfo &zoneInfo = zoneInfos[zoneIndex];
        if (zoneInfo.zoneType == ORDERED)
        {
            if (zoneInfo.numMiscFaceConnections != 0 && shareZone == -1)
            {
                return SkipFaceNeighborConnections(data, zoneInfo.numMiscFaceConnections, zoneInfo.faceNeighborMode);
            }
            return true;
        }

        if (shareZone >= 0)
        {
            if (shareZone >= dataInfos.size())
            {
                qDebug() << "Invalid shared connectivity zone:" << shareZone;
                return false;
            }
            dataInfo.Cells = dataInfos[shareZone].Cells;
            return true;
        }

        // 当前读取器暂不展开 polygon/polyhedron 面表，只保证按文档正确跳过相关数据段。
        if (zoneInfo.zoneType == FEPOLYGON || zoneInfo.zoneType == FEPOLYHEDRON)
        {
            if (zoneInfo.zoneType == FEPOLYHEDRON)
            {
                qint64 faceHeaderCount = (zoneInfo.zoneMarker == 298 ? zoneInfo.numFaces : zoneInfo.numFaces + 1);
                if (!SkipNumericValues(data, 3, faceHeaderCount))
                {
                    return false;
                }
            }
            if (!SkipNumericValues(data, 3, zoneInfo.totalFaceNodes))
            {
                return false;
            }
            if (!SkipNumericValues(data, 3, zoneInfo.numFaces))
            {
                return false;
            }
            if (!SkipNumericValues(data, 3, zoneInfo.numFaces))
            {
                return false;
            }
            if (zoneInfo.totalBoundaryFaces != 0)
            {
                if (!SkipNumericValues(data, 3, zoneInfo.totalBoundaryFaces + 1))
                {
                    return false;
                }
                if (!SkipNumericValues(data, 3, zoneInfo.totalBoundaryConnections))
                {
                    return false;
                }
                if (!SkipNumericValues(data, 3, zoneInfo.totalBoundaryConnections))
                {
                    return false;
                }
            }
            return true;
        }

        int numPoints = GetNumberOfPointSize(zoneInfo.zoneType);
        if (numPoints <= 0)
        {
            return false;
        }

        // FE 基本单元直接按 Tecplot connectivity 顺序缓存节点索引。
        for (int i = 0; i < zoneInfo.numberCells * numPoints; i++)
        {
            int value = 0;
            ReadBytes(data, value, INT32SIZE);
            dataInfo.Cells.append(value);
        }

        if (zoneInfo.hasRawLocalFaceNeighbors != 0)
        {
            int facesPerElement = 0;
            if (zoneInfo.zoneType == FE_LINE_SEG)
            {
                facesPerElement = 2;
            }
            else if (zoneInfo.zoneType == FE_TRIANGLE)
            {
                facesPerElement = 3;
            }
            else if (zoneInfo.zoneType == FE_QUAD || zoneInfo.zoneType == FE_TET)
            {
                facesPerElement = 4;
            }
            else if (zoneInfo.zoneType == FE_BRICK)
            {
                facesPerElement = 6;
            }
            if (facesPerElement > 0 && !SkipNumericValues(data, 3, zoneInfo.numberCells * facesPerElement))
            {
                return false;
            }
        }

        if (zoneInfo.numMiscFaceConnections != 0)
        {
            return SkipFaceNeighborConnections(data, zoneInfo.numMiscFaceConnections, zoneInfo.faceNeighborMode);
        }
        return true;
    }

    /**
     * @brief    跳过几何对象定义段。
     * @param[i] data 源数据
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipGeometry(QByteArray &data)
    {
        int positionCoordSys = 0;
        int scope = 0;
        int drawOrder = 0;
        double coordinate = 0.0;
        int zone = 0;
        int color = 0;
        int fillColor = 0;
        int isFilled = 0;
        int geomType = 0;
        int linePattern = 0;
        double patternLength = 0.0;
        double lineThickness = 0.0;
        int numEllipsePts = 0;
        int arrowheadStyle = 0;
        int arrowheadAttachment = 0;
        double arrowheadSize = 0.0;
        double arrowheadAngle = 0.0;
        int polylineFieldDataType = 0;
        int clipping = 0;

        ReadBytes(data, positionCoordSys, INT32SIZE);
        ReadBytes(data, scope, INT32SIZE);
        ReadBytes(data, drawOrder, INT32SIZE);
        ReadBytes(data, coordinate, DOUBLESIZE);
        ReadBytes(data, coordinate, DOUBLESIZE);
        ReadBytes(data, coordinate, DOUBLESIZE);
        ReadBytes(data, zone, INT32SIZE);
        ReadBytes(data, color, INT32SIZE);
        ReadBytes(data, fillColor, INT32SIZE);
        ReadBytes(data, isFilled, INT32SIZE);
        ReadBytes(data, geomType, INT32SIZE);
        ReadBytes(data, linePattern, INT32SIZE);
        ReadBytes(data, patternLength, DOUBLESIZE);
        ReadBytes(data, lineThickness, DOUBLESIZE);
        ReadBytes(data, numEllipsePts, INT32SIZE);
        ReadBytes(data, arrowheadStyle, INT32SIZE);
        ReadBytes(data, arrowheadAttachment, INT32SIZE);
        ReadBytes(data, arrowheadSize, DOUBLESIZE);
        ReadBytes(data, arrowheadAngle, DOUBLESIZE);
        ReadInt32String(data);
        ReadBytes(data, polylineFieldDataType, INT32SIZE);
        ReadBytes(data, clipping, INT32SIZE);

        // line 类型按折线点序列展开，其余类型只需跳过固定数量参数。
        int valueType = (polylineFieldDataType == 2 ? 2 : 1);
        if (geomType == 0)
        {
            int numPolylines = 0;
            ReadBytes(data, numPolylines, INT32SIZE);
            bool hasZ = (positionCoordSys == 4);
            for (int polylineIndex = 0; polylineIndex < numPolylines; polylineIndex++)
            {
                int numPoints = 0;
                ReadBytes(data, numPoints, INT32SIZE);
                if (!SkipNumericValues(data, valueType, numPoints) || !SkipNumericValues(data, valueType, numPoints))
                {
                    return false;
                }
                if (hasZ && !SkipNumericValues(data, valueType, numPoints))
                {
                    return false;
                }
            }
            return true;
        }
        if (geomType == 1 || geomType == 4)
        {
            return SkipNumericValues(data, valueType, 2);
        }
        if (geomType == 2 || geomType == 3)
        {
            return SkipNumericValues(data, valueType, 1);
        }
        return true;
    }

    /**
     * @brief    跳过文本对象定义段。
     * @param[i] data 源数据
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipText(QByteArray &data)
    {
        int intValue = 0;
        double doubleValue = 0.0;
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, doubleValue, DOUBLESIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadBytes(data, intValue, INT32SIZE);
        ReadInt32String(data);
        ReadBytes(data, intValue, INT32SIZE);
        ReadInt32String(data);
        return true;
    }

    /**
     * @brief    跳过自定义标签定义段。
     * @param[i] data 源数据
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipCustomLabel(QByteArray &data)
    {
        int labelCount = 0;
        ReadBytes(data, labelCount, INT32SIZE);
        for (int i = 0; i < labelCount; i++)
        {
            ReadInt32String(data);
        }
        return true;
    }

    /**
     * @brief    跳过用户记录定义段。
     * @param[i] data 源数据
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipUserRecord(QByteArray &data)
    {
        ReadInt32String(data);
        return true;
    }

    /**
     * @brief    跳过数据集辅助数据段。
     * @param[i] data 源数据
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipDataSetAuxData(QByteArray &data)
    {
        ReadInt32String(data);
        int auxFormat = 0;
        ReadBytes(data, auxFormat, INT32SIZE);
        ReadInt32String(data);
        return true;
    }

    /**
     * @brief    跳过变量辅助数据段。
     * @param[i] data 源数据
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipVariableAuxData(QByteArray &data)
    {
        int variableNumber = 0;
        ReadBytes(data, variableNumber, INT32SIZE);
        ReadInt32String(data);
        int auxFormat = 0;
        ReadBytes(data, auxFormat, INT32SIZE);
        ReadInt32String(data);
        return true;
    }

    /**
     * @brief    跳过分区辅助数据段。
     * @param[i] data 源数据
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipZoneAuxData(QByteArray &data)
    {
        while (true)
        {
            int hasAuxData = 0;
            ReadBytes(data, hasAuxData, INT32SIZE);
            if (hasAuxData == 0)
            {
                return true;
            }
            ReadInt32String(data);
            int auxFormat = 0;
            ReadBytes(data, auxFormat, INT32SIZE);
            ReadInt32String(data);
        }
    }

    /**
     * @brief    跳过面邻接连接记录。
     * @param[i] data 源数据
     * @param[i] connectionCount 连接数量
     * @param[i] faceNeighborMode 邻接模式
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipFaceNeighborConnections(QByteArray &data, int connectionCount, int faceNeighborMode)
    {
        for (int connectionIndex = 0; connectionIndex < connectionCount; connectionIndex++)
        {
            int currentCell = 0;
            int currentFace = 0;
            ReadBytes(data, currentCell, INT32SIZE);
            ReadBytes(data, currentFace, INT32SIZE);
            // 不同 face neighbor mode 的记录长度不同，必须严格按文档逐字段跳过。
            if (faceNeighborMode == 0)
            {
                int neighborCell = 0;
                ReadBytes(data, neighborCell, INT32SIZE);
                continue;
            }
            if (faceNeighborMode == 1)
            {
                int obscuration = 0;
                int neighborCount = 0;
                ReadBytes(data, obscuration, INT32SIZE);
                ReadBytes(data, neighborCount, INT32SIZE);
                if (!SkipNumericValues(data, 3, neighborCount))
                {
                    return false;
                }
                continue;
            }
            if (faceNeighborMode == 2)
            {
                int remoteZone = 0;
                int remoteCell = 0;
                ReadBytes(data, remoteZone, INT32SIZE);
                ReadBytes(data, remoteCell, INT32SIZE);
                continue;
            }
            if (faceNeighborMode == 3)
            {
                int obscuration = 0;
                int neighborCount = 0;
                ReadBytes(data, obscuration, INT32SIZE);
                ReadBytes(data, neighborCount, INT32SIZE);
                for (int neighborIndex = 0; neighborIndex < neighborCount; neighborIndex++)
                {
                    int remoteZone = 0;
                    int remoteCell = 0;
                    ReadBytes(data, remoteZone, INT32SIZE);
                    ReadBytes(data, remoteCell, INT32SIZE);
                }
                continue;
            }
            qDebug() << "Unsupported face neighbor mode:" << faceNeighborMode;
            return false;
        }
        return true;
    }

    /**
     * @brief    按 Tecplot 数值类型批量跳过指定数量的值。
     * @param[i] data 源数据
     * @param[i] variableType Tecplot 变量类型
     * @param[i] valueCount 跳过的数量
     * @return   bool 是否成功
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-07-03
     */
    bool FITKPltReader::SkipNumericValues(QByteArray &data, int variableType, qint64 valueCount)
    {
        for (qint64 valueIndex = 0; valueIndex < valueCount; valueIndex++)
        {
            ReadNumericValueAsDouble(data, variableType);
            if (_readErrorCode != NoError)
            {
                return false;
            }
        }
        return true;
    }
}
