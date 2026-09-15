/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataPartAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include <QSet>

namespace Interface
{
    bool FITKRadiossDataPartAdaptor::adaptR()
    {
        _partData = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (_reader == nullptr || _partData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _reader->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!readRadiossData(_partData, *_h5Group)) return false;
        if (!readDataObject(_partData, *_h5Group)) return false;
        int propId = -1;
        int matId = -1;
        int subsetId = -1;
        double thick = -1;
        if (!h5tools->readGroupint(*_h5Group, "PropId", &propId))return false;
        if (!h5tools->readGroupint(*_h5Group, "MatId", &matId))return false;
        if (!h5tools->readGroupint(*_h5Group, "SubSetId", &subsetId))return false;
        if (!h5tools->readGroupDouble(*_h5Group, "Thick", &thick))return false;
        int colorArray[3] = {};
        if (!h5tools->readGroupAttrIntArray(*_h5Group, "Color", colorArray, 1, 3)) return false;
        QColor color = {};
        color.setRed(colorArray[0]);
        color.setGreen(colorArray[1]);
        color.setBlue(colorArray[2]);
        _partData->setColor(color);
        // 设置零件属性
        _partData->setPropertyID(propId);
        _partData->setMaterialID(matId);
        _partData->setSubsetID(subsetId);
        _partData->setThickness(thick);
        H5::Group eleGroup = _h5Group->openGroup("Ele");
        int n = 0, m = 0;
        std::string name = _partData->getDataObjectName().toStdString();
        if (!getDataSetDim(eleGroup, "Element", n, m)) return false;
        hsize_t dimEleData[2]{ n, m };
        H5::DataSpace eleDataSpace(2, dimEleData);
        H5::DataSet eleDataSet = eleGroup.openDataSet("Element");
        for (int i = 0; i < n; ++i)
        {
            hsize_t start[2] = { i, 0 };
            hsize_t count[2] = { 1, m };
            eleDataSpace.selectHyperslab(H5S_SELECT_SET, count, start);
            hsize_t dim_mem[] = { m };
            H5::DataSpace mspace2(1, dim_mem);
            std::vector<int> ptIDArray(m);
            eleDataSet.read(ptIDArray.data(), H5::PredType::NATIVE_INT, mspace2, eleDataSpace);
            QList<int> nodes{};
            int eleNodeCount = ptIDArray[2];
            if (eleNodeCount + 3 + 2 > m)
            {
                continue;  // 跳过无效元素
            }
            // 读取节点ID
            for (int k = 0; k < eleNodeCount; ++k)
            {
                nodes.append(ptIDArray[k + 3]);
            }
            // 创建单元并设置基础属性
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType(ptIDArray[1]));
            if (!element)
            {
                continue;
            }
            element->setNodeID(nodes);
            element->setEleID(ptIDArray[0]);
            element->setTag(static_cast<unsigned int>(ptIDArray[3 + eleNodeCount]));
            element->setNativeFlag(ptIDArray[3 + eleNodeCount + 1] == 1);
            _partData->addElement(ptIDArray[0],element);
        }
        eleDataSet.close();
        return true;
    }

    bool FITKRadiossDataPartAdaptor::adaptW()
    {
        _partData = dynamic_cast<Radioss::FITKRadiossPart*>(_dataObj);
        if (_writer == nullptr || _partData == nullptr || _h5Group == nullptr) return false;
        IO::FITKHDF5FileTool* h5tools = _writer->getHDF5FileTool();
        if (h5tools == nullptr) return false;
        if (!writeRadiossData(_partData, *_h5Group))return false;
        if (!writeDataObject(_partData, *_h5Group))return false;
        //写出零件数据
        int propId = _partData->getPropertyID();
        int matId = _partData->getMaterialID();
        int subsetId = _partData->getSubsetID();
        double thick = _partData->getThickness();
        h5tools->writeGroupAttrInt(*_h5Group, "PropId", propId);
        h5tools->writeGroupAttrInt(*_h5Group, "MatId", matId);
        h5tools->writeGroupAttrInt(*_h5Group, "SubSetId", subsetId);
        h5tools->writeGroupAttrDouble(*_h5Group, "Thick", thick);
        int colorArray[3] = {};
        colorArray[0] = _partData->getColor().red();
        colorArray[1] = _partData->getColor().green();
        colorArray[2] = _partData->getColor().blue();
        h5tools->writeGroupAttrIntArray(*_h5Group, "Color", colorArray, 1, 3);

        H5::Group eleGroup = _h5Group->createGroup("Ele");
        int elemCount = _partData->getElementCount();
        h5tools->writeGroupAttrInt(eleGroup, "ElementCount", elemCount);

        int maxLength = 0;
        for (int i = 0; i < elemCount; ++i)
        {
            auto ele = _partData->getElementAt(i);
            if (!ele) continue;
            int num = ele->getNodeCount();
            if (maxLength < num)
                maxLength = num;
        }
        int lineSize = maxLength + 3 + 2;
        hsize_t dimEleData[2]{ elemCount, lineSize };
        H5::DataSpace eleDataSpace(2, dimEleData);
        H5::DataSet eleDataSet = eleGroup.createDataSet("Element",H5::PredType::NATIVE_INT, eleDataSpace);
        for (int i = 0; i < elemCount; ++i)
        {
            hsize_t start[2] = { i, 0 };
            hsize_t count[2] = { 1, lineSize };
            eleDataSpace.selectHyperslab(H5S_SELECT_SET, count, start);

            hsize_t dim_mem[] = { lineSize };
            H5::DataSpace mspace2(1, dim_mem);

            FITKAbstractElement* ele = _partData->getElementAt(i);
            if (!ele) continue;

            int eleNodeCount = ele->getNodeCount();
            std::vector<int> ptIDArray(lineSize, 0);  // 初始化为0
            // 节点ID（索引3开始）
            for (int k = 0; k < eleNodeCount; ++k)
                ptIDArray[k + 3] = ele->getNodeID(k);

            ptIDArray[0] = ele->getEleID();
            ptIDArray[1] = ele->getEleType();
            ptIDArray[2] = eleNodeCount;
            ptIDArray[3 + eleNodeCount] = ele->getTag();
            ptIDArray[3 + eleNodeCount + 1] = ele->getNativeFlag() ? 1 : 0;
            eleDataSet.write(ptIDArray.data(), H5::PredType::NATIVE_INT, mspace2, eleDataSpace);
        }
        eleDataSet.close();
        return true;
    }
    bool FITKRadiossDataPartAdaptor::getDataSetDim(H5::Group& h5group, std::string name, int& n, int& m)
    {
        if (!h5group.nameExists(name)) return false;
        H5::DataSet dataSet = h5group.openDataSet(name);
        H5::DataSpace space = dataSet.getSpace();
        hsize_t       dim[2];
        space.getSimpleExtentDims(dim);
        n = dim[0];
        m = dim[1];
        return true;
    }
}
