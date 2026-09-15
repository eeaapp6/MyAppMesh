/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKIBEStrDataSetReader.h"

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <vtkUnstructuredGrid.h>
#include <vtkInformation.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformationVector.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>

#include <H5Cpp.h>

vtkStandardNewMacro(FITKIBEStrDataSetReader);

void FITKIBEStrDataSetReader::PrintSelf(ostream & os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
}

void FITKIBEStrDataSetReader::setMeshFile(const QString & meshFile)
{
    _meshFile = meshFile;
}

void FITKIBEStrDataSetReader::setResultFile(const QString & resultFile)
{
    _resultFile = resultFile;
}

FITKIBEStrDataSetReader::FITKIBEStrDataSetReader()
{
    this->SetNumberOfInputPorts(0);
}

FITKIBEStrDataSetReader::~FITKIBEStrDataSetReader()
{

}

int FITKIBEStrDataSetReader::FillOutputPortInformation(int port, vtkInformation * info)
{
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
    return 1;
}

int FITKIBEStrDataSetReader::ProcessRequest(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector)
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
    return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

int FITKIBEStrDataSetReader::RequestData(vtkInformation * request, vtkInformationVector ** inputVector, vtkInformationVector * outputVector)
{
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::GetData(outputVector);
    if (output == nullptr) {
        return 0;
    }
    if (_lastFilePath != _meshFile) {
        // 读取 .H5 File
        H5::H5File file;
        std::string fileName = _meshFile.toStdString();
        try {
            file.openFile(fileName.c_str(), H5F_ACC_RDONLY);
        }
        catch (H5::FileIException& error) {
            vtkErrorMacro(<< "Cannot open HDF5 file: " << fileName.c_str());
            AppFrame::FITKMessageError("Cannot open HDF5 file: " + QString::fromStdString(fileName));
            return 0;
        }

        H5::DataSet coordDataSet = file.openDataSet("/Mesh/Node/Coord");
        H5::DataSpace coordSpace = coordDataSet.getSpace();

        // 读到的是一维数组
        int ndims = coordSpace.getSimpleExtentNdims();
        if (ndims != 1) {
            vtkErrorMacro(<< "Coordinate dataset is not 1-dimensional. Dims: " << ndims);
            AppFrame::FITKMessageError("Coordinate dataset is not 1-dimensional. Dims: " + QString::number(ndims));
            return 0;
        }

        // 总行数，必须是3的倍数
        hsize_t totalRows;
        coordSpace.getSimpleExtentDims(&totalRows);

        // 点的数量
        hsize_t totalNodes = totalRows / 3;
        if (totalRows % 3 != 0) {
            vtkErrorMacro(<< "Coordinate dataset size (" << totalRows << ") is not a multiple of 3.");
            AppFrame::FITKMessageError("Coordinate dataset size (" + QString::number(totalRows) + ") is not a multiple of 3.");
            return 0;
        }
        // 读入数组中
        std::vector<double> coordData(totalRows);
        coordDataSet.read(coordData.data(), H5::PredType::NATIVE_DOUBLE);
        coordDataSet.close();

        // 创建点集
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        points->SetNumberOfPoints(totalNodes);
        _nodesNum = totalNodes;
        double pointCoord[3];
        for (int i = 0; i < totalNodes; ++i) {
            int index = i * 3;
            pointCoord[0] = coordData[index];
            pointCoord[1] = coordData[index + 1];
            pointCoord[2] = coordData[index + 2];
            points->SetPoint(i, pointCoord);
        }
        output->SetPoints(points);

        // 读取索引
        H5::DataSet idxDataSet = file.openDataSet("/Mesh/Cell/Nodes.Index");
        H5::DataSpace idxSpace = idxDataSet.getSpace();
        hsize_t idxSize;
        idxSpace.getSimpleExtentDims(&idxSize);
        std::vector<int> indexArray(idxSize);
        idxDataSet.read(indexArray.data(), H5::PredType::NATIVE_INT);
        idxDataSet.close();

        // 读取索引对应的节点id
        H5::DataSet valueDataSet = file.openDataSet("/Mesh/Cell/Nodes.Value");
        H5::DataSpace valueSpace = valueDataSet.getSpace();
        hsize_t valueSize;
        valueSpace.getSimpleExtentDims(&valueSize);
        // 索引与节点id连接起来，其中索引为单元数目的四倍(目前为四面体)
        std::vector<int> ids(valueSize);
        valueDataSet.read(ids.data(), H5::PredType::NATIVE_INT);
        valueDataSet.close();

        // 读取每个单元的类型，每个单元根据单元类型插入数据
        H5::DataSet typeDataSet = file.openDataSet("/Mesh/Cell/Type");
        H5::DataSpace typeSpace = typeDataSet.getSpace();
        hsize_t typeSize;
        typeSpace.getSimpleExtentDims(&typeSize);
        std::vector<int> typeArray(typeSize);
        typeDataSet.read(typeArray.data(), H5::PredType::NATIVE_INT);
        typeDataSet.close();

        // 设置单元数
        hsize_t numCells = typeSize;
        _cellNum = numCells;

        // 检测单元类型数目与单元数目是否匹配
        if (typeSize != idxSize) {
            vtkWarningMacro(<< "The number of Cells types: " << typeSize << "  does not match the number of Cells: " << numCells);
            AppFrame::FITKMessageWarning("The number of Cells types: " + QString::number(typeSize) + "  does not match the number of Cells: " + QString::number(idxSize));
        }
        output->Allocate(numCells);

        for (vtkIdType cellId = 0; cellId < numCells; ++cellId) {
            vtkIdType pointsNum = 0;
            if (cellId < typeArray.size()) {
                // 通过枚举单元类型获取每个单元的点的数量
                QPair<int, int> pair = _hashMap[typeArray[cellId]];
                pointsNum = pair.first;
            }
            if (pointsNum == 0) {
                vtkWarningMacro(<< "Unknown Cell Type: " << typeArray[cellId]);
                continue;
            }
            vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
            idList->SetNumberOfIds(pointsNum);
            bool isValidCell = true;
            for (vtkIdType i = 0; i < pointsNum; ++i) {
                if (indexArray[cellId] + i < ids.size()) {
                    vtkIdType id = ids[indexArray[cellId] + i];
                    if (id < 0 || id >= totalNodes) {
                        vtkWarningMacro(<< "Cell: " << cellId << " Include Invalid Node ID: " << id);
                        AppFrame::FITKMessageWarning("Cell: " + QString::number(cellId) + " Include Invalid Node ID: " + QString::number(id));
                        isValidCell = false;
                        break;
                    }
                    idList->SetId(i, id);
                }
            }
            int cellType = -1;
            if (cellId < typeSize) {
                cellType = typeArray[cellId];
            }
            QPair<int, int> pair = _hashMap[cellType];
            if (isValidCell) {
                output->InsertNextCell(pair.second, idList);
            }
        }
        file.close();
        _lastFilePath = _meshFile;
        // 深拷贝网格数据
        _cachedMesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
        _cachedMesh->DeepCopy(output);
    }
    else {
        if (_cachedMesh) {
            output->DeepCopy(_cachedMesh);
        }
        else {
            _lastFilePath = "";
            return this->RequestData(request, inputVector, outputVector);
        }
    }

    QString currentName = getCurrentGroupName();
    if (currentName.isEmpty()) {
        return 0;
    }

    H5::H5File fileResult;
    fileResult.openFile(_resultFile.toStdString().c_str(), H5F_ACC_RDONLY);
    QString filePath = "/Case/1-General/" + currentName + "/Field";
    H5::Group parent = fileResult.openGroup(filePath.toStdString());
    int numGroups = parent.getNumObjs();
    // 遍历每个场量
    for (int i = 0; i < numGroups; ++i) {
        std::string subName = (filePath + "/" + QString::fromStdString(parent.getObjnameByIdx(i))).toStdString();
        H5::Group subGroup = fileResult.openGroup(subName);
        // 判断是不是单元类型
        bool flag = false;
        for (int j = 0; j < subGroup.getNumObjs(); ++j) {
            std::string leafName = subGroup.getObjnameByIdx(j);
            flag = (leafName == "IntgrPoints");
            if (flag) {
                // 执行单元的添加
                addCellByPath(fileResult, filePath, output, QString::fromStdString(subName));
                break;
            }
        }
        if (!flag && subGroup.getNumObjs() > 0) {
            // 执行点的添加
            addPointByPath(fileResult, filePath, output, QString::fromStdString(subName));
        }
    }
    fileResult.close();
    return 1;
}


void FITKIBEStrDataSetReader::setStep(int step)
{
    if (_step != step) {
        _step = step;
        this->Modified();
    }
}

int FITKIBEStrDataSetReader::getCurrentStep()
{
    return _step;
}

int FITKIBEStrDataSetReader::getTotalStep()
{
    H5::H5File file;
    std::string fileName = _resultFile.toStdString();
    try {
        file.openFile(fileName.c_str(), H5F_ACC_RDONLY);
    }
    catch (H5::FileIException& error) {
        vtkErrorMacro(<< "Cannot open ResultFile: " + fileName);
        AppFrame::FITKMessageError("Cannot open ResultFile: " + QString::fromStdString(fileName));
        return -1;
    }
    H5::Group parent = file.openGroup("/Case/1-General");

    hsize_t numObjs = parent.getNumObjs();
    if (numObjs >= 0) {
        _totalSteps = numObjs;
    }
    file.close();
    return _totalSteps;
}

QString FITKIBEStrDataSetReader::getCurrentGroupName()
{
    
    if (getTotalStep() == -1) {
        return "";
    }

    H5::H5File file;
    std::string fileName = _resultFile.toStdString();
    file.openFile(fileName.c_str(), H5F_ACC_RDONLY);

    H5::Group parent = file.openGroup("/Case/1-General");

    // 临界情况判断
    if (_step >= 0 && _step < _totalSteps) {
        std::string name = parent.getObjnameByIdx(_step);
        file.close();
        return QString::fromStdString(name);
    }
    file.close();
    return "";
}

void FITKIBEStrDataSetReader::addCellByPath(H5::H5File &file, QString path, vtkUnstructuredGrid* output, QString fieldName)
{
    path = fieldName + "/Value";
    H5::DataSet cellDataSet = file.openDataSet(path.toStdString());
    H5::DataSpace cellSpace = cellDataSet.getSpace();

    hsize_t cellSize;
    cellSpace.getSimpleExtentDims(&cellSize);

    if (cellSize % _cellNum != 0) {
        // 此处添加warning
    }
    int dim = cellSize / _cellNum;
    vtkSmartPointer<vtkDoubleArray> cellVectors = vtkSmartPointer<vtkDoubleArray>::New();
    QStringList strList = fieldName.split("/");
    int idx = strList.size();
    cellVectors->SetName(strList[idx - 1].toStdString().c_str());
    // 设置维度
    cellVectors->SetNumberOfComponents(dim);
    std::vector<double> values(cellSize);

    cellDataSet.read(values.data(), H5::PredType::NATIVE_DOUBLE);
    std::vector<double> nums(dim);
    for (int i = 0; i < cellSize; i += dim) {
        for (int j = 0; j < dim; ++j) {
            nums[j] = values[i + j];
        }
        cellVectors->InsertNextTuple(nums.data());
    }
    output->GetCellData()->AddArray(cellVectors);
}

void FITKIBEStrDataSetReader::addPointByPath(H5::H5File &file, QString path, vtkUnstructuredGrid* output, QString fieldName)
{
    path = fieldName + "/Value";
    H5::DataSet pointDataSet = file.openDataSet(path.toStdString());
    H5::DataSpace pointSpace = pointDataSet.getSpace();

    hsize_t pointSize;
    pointSpace.getSimpleExtentDims(&pointSize);

    if (pointSize % _nodesNum != 0) {
        // 此处添加warning
    }
    int dim = pointSize / _nodesNum;
    vtkSmartPointer<vtkDoubleArray> pointVectors = vtkSmartPointer<vtkDoubleArray>::New();
    QStringList strList = fieldName.split("/");
    int idx = strList.size();
    pointVectors->SetName(strList[idx - 1].toStdString().c_str());
    // 设置维度
    pointVectors->SetNumberOfComponents(dim);
    std::vector<double> values(pointSize);

    pointDataSet.read(values.data(), H5::PredType::NATIVE_DOUBLE);
    std::vector<double> nums(dim);
    for (int i = 0; i < pointSize; i += dim) {
        for (int j = 0; j < dim; ++j) {
            nums[j] = values[i + j];
        }
        pointVectors->InsertNextTuple(nums.data());
    }
    output->GetPointData()->AddArray(pointVectors);
}