/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataSurfaceAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"

namespace Interface
{
    bool FITKRadiossDataSurfaceAdaptor::adaptR()
    {
        _surfaceData = dynamic_cast<Radioss::FITKRadiossSurfaceSeg*>(_dataObj);
        if (_reader == nullptr || _surfaceData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_surfaceData, *_h5Group)) return false;
        if (!readDataObject(_surfaceData, *_h5Group)) return false;
        int modelID = -1;
        QColor color{};
        int colorArray[3] = {};
        bool internal =false;

        if (!h5tools->readGroupint(*_h5Group, "ModelID", &modelID)) return false;
        if (!h5tools->readGroupAttrIntArray(*_h5Group, "Color", colorArray, 1, 3)) return false;
        if (!h5tools->readGroupBool(*_h5Group, "isInternal", &internal)) return false;
        color.setRed(colorArray[0]);
        color.setGreen(colorArray[1]);
        color.setBlue(colorArray[2]);
        _surfaceData->setModel(modelID);
        _surfaceData->setColor(color);
        _surfaceData->isInternal(internal);

        // 读取单元表面数据
        H5::Group eleSurfaceGroup;
        try {
            eleSurfaceGroup = _h5Group->openGroup("ElementSurface");
        }
        catch (H5::Exception&) {
            // 如果没有ElementSurface组，则跳过
            return true;
        }
        // 获取数据集维度
        int n =0, m=0;
        if (!getDataSetDim(eleSurfaceGroup, "ElementSurfaceData", n, m)) return false;
        hsize_t dimEleSurfaceData[2]{ n, m };
        H5::DataSpace eleSurfaceDataSpace(2, dimEleSurfaceData);
        H5::DataSet eleSurfaceDataSet = eleSurfaceGroup.openDataSet("ElementSurfaceData");
        // 循环读取单元表面数据
        for (int i = 0; i < n; ++i)
        {
            hsize_t start[2] = { i, 0 };
            hsize_t count[2] = { 1, m }; 
            eleSurfaceDataSpace.selectHyperslab(H5S_SELECT_SET, count, start);
            hsize_t dim_mem[] = { m };  
            H5::DataSpace mspace2(1, dim_mem);
            int* surfaceDataArray = new int[m];
            eleSurfaceDataSet.read(surfaceDataArray, H5::PredType::NATIVE_INT, mspace2, eleSurfaceDataSpace);
            if (m >= 2) {
                int elementID = surfaceDataArray[0];
                int surfaceIndex = surfaceDataArray[1];
                _surfaceData->addElementSurface(elementID, surfaceIndex);
            }
            delete[] surfaceDataArray;
        }
        eleSurfaceDataSet.close();
        eleSurfaceGroup.close();
        return true;
    }

    bool FITKRadiossDataSurfaceAdaptor::adaptW()
    {
        _surfaceData = dynamic_cast<Radioss::FITKRadiossSurfaceSeg*>(_dataObj);
        if (_writer == nullptr || _surfaceData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;

        if (!writeRadiossData(_surfaceData, *_h5Group)) return false;
        if (!writeDataObject(_surfaceData, *_h5Group)) return false;

        // 获取所有元素ID
        QList<int> elementIDs = _surfaceData->getAllElement();
        int elementCount = elementIDs.size();

        // 写出基本属性
        int modelID = _surfaceData->getAbsModelID();
        QColor color = _surfaceData->getColor();
        int colorArray[3] = {};
        colorArray[0] = color.red();
        colorArray[1] = color.green();
        colorArray[2] = color.blue();
        bool internal = _surfaceData->isInternal();

        h5tools->writeGroupAttrInt(*_h5Group, "ElementCount", elementCount);
        h5tools->writeGroupAttrInt(*_h5Group, "ModelID", modelID);
        h5tools->writeGroupAttrIntArray(*_h5Group, "Color", colorArray, 1, 3);
        h5tools->writeGroupAttrBool(*_h5Group, "isInternal", internal);

        // 写出单元表面数据
        H5::Group eleSurfaceGroup = _h5Group->createGroup("ElementSurface");

        // 获取_elementSurface中的所有数据
        QMultiHash<int, int> elementSurface = {};
        if (elementSurface.isEmpty()) 
        {
            QList<int> allElements = _surfaceData->getAllElement();
            for (int elementID : allElements) {
                QList<int> surfaceIDs = _surfaceData->getElementSurfaceIDs(elementID);
                for (int surfaceID : surfaceIDs) {
                    elementSurface.insert(elementID, surfaceID);
                }
            }
        }
        int surfacePairCount = elementSurface.size();
        h5tools->writeGroupAttrInt(eleSurfaceGroup, "ElementSurfaceCount", surfacePairCount);
        // 创建数据集，每行包含[elementID, surfaceIndex]
        int lineSize = 2; // 每行2个数据：元素ID和表面索引
        hsize_t dimEleSurfaceData[2]{ surfacePairCount, lineSize };
        H5::DataSpace eleSurfaceDataSpace(2, dimEleSurfaceData);
        H5::DataSet eleSurfaceDataSet = eleSurfaceGroup.createDataSet("ElementSurfaceData",
            H5::PredType::NATIVE_INT,
            eleSurfaceDataSpace);
        // 写入单元表面数据
        int rowIndex = 0;
        for (auto it = elementSurface.constBegin(); it != elementSurface.constEnd(); ++it)
        {
            hsize_t start[2] = { rowIndex, 0 };
            hsize_t count[2] = { 1, lineSize };
            eleSurfaceDataSpace.selectHyperslab(H5S_SELECT_SET, count, start);

            hsize_t dim_mem[] = { lineSize };
            H5::DataSpace mspace2(1, dim_mem);

            // 使用固定大小数组
            int surfaceDataArray[2];  // 直接使用字面值2
            surfaceDataArray[0] = it.key();
            surfaceDataArray[1] = it.value();
            eleSurfaceDataSet.write(surfaceDataArray, H5::PredType::NATIVE_INT, mspace2, eleSurfaceDataSpace);
            rowIndex++;
        }
        eleSurfaceDataSet.close();
        eleSurfaceGroup.close();
        return true;
    }

    bool FITKRadiossDataSurfaceAdaptor::getDataSetDim(H5::Group& h5group, std::string name, int& n, int& m)
    {
        if (!h5group.nameExists(name)) return false;
        H5::DataSet dataSet = h5group.openDataSet(name);
        H5::DataSpace space = dataSet.getSpace();
        hsize_t dim[2];
        space.getSimpleExtentDims(dim);
        n = dim[0];
        m = dim[1];
        return true;
    }
}
