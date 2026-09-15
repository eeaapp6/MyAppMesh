/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossDataNodeAdaptor.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include <array>

namespace Interface
{
    bool FITKRadiossDataNodeAdaptor::adaptR()
    {
        _nodeData = dynamic_cast<Radioss::FITKRadiossNodes*>(_dataObj);
        if (_reader == nullptr || _nodeData == nullptr || _h5Group == nullptr) return false;

        //PointIDs
        if (!_h5Group->nameExists("PointIDs")) return false;
        auto PointIDSet = _h5Group->openDataSet("PointIDs");
        //Points
        if (!_h5Group->nameExists("Points")) return false;
        auto pointsSet = _h5Group->openDataSet("Points");
        H5::DataSpace space = pointsSet.getSpace();
        hsize_t       dim[2];
        space.getSimpleExtentDims(dim);
        int n = dim[0];
        int m = dim[1];
        //PointTags
        if (!_h5Group->nameExists("PointTags")) return false;
        auto PointTagSet = _h5Group->openDataSet("PointTags");
        //PointNativeFlags
        if (!_h5Group->nameExists("PointNativeFlags")) return false;
        auto PointNativeSet = _h5Group->openDataSet("PointNativeFlags");
        //点
        // 行数 // 列数
        hsize_t       dimPtData[2]{ n ,m };
        H5::DataSpace ptDataSpace(2, dimPtData);
        //点ID
        // 行数 // 列数
        hsize_t       dimPtIDData[2]{ n ,1 };
        H5::DataSpace ptIDDataSpace(2, dimPtIDData);

        hsize_t dimTagData[2]{ n, 1 };
        H5::DataSpace ptTagDataSpace(2, dimTagData);

        hsize_t dimNativeFlagData[2]{ n, 1 };
        H5::DataSpace ptNativeDataSpace(2, dimNativeFlagData);

        //点数据
        hsize_t       dim3[] = { 3 };
        H5::DataSpace mspace3(1, dim3);
        //点ID数据
        hsize_t       dim1[] = { 1 };
        H5::DataSpace mspace1(1, dim1);
        //表格
        hsize_t       coordPtData[3][2];
        coordPtData[0][1] = 0;
        coordPtData[1][1] = 1;
        coordPtData[2][1] = 2;
        for (int i = 0; i < n; ++i)
        {
            coordPtData[0][0] = i;
            coordPtData[1][0] = i;
            coordPtData[2][0] = i;
            //点
            std::array< double, 3 >  point{};
            ptDataSpace.selectElements(H5S_SELECT_SET, 3, (const hsize_t*)coordPtData);
            pointsSet.read(&point, H5::PredType::NATIVE_DOUBLE, mspace3, ptDataSpace);
            //点ID
            int pID = -1;
            ptIDDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            PointIDSet.read(&pID, H5::PredType::NATIVE_INT, mspace1, ptIDDataSpace);

            unsigned int tag = 0;
            ptTagDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            PointTagSet.read(&tag, H5::PredType::NATIVE_UINT, mspace1, ptTagDataSpace);

            int nativeFlag = 0;
            ptNativeDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            PointNativeSet.read(&nativeFlag, H5::PredType::NATIVE_INT, mspace1, ptNativeDataSpace);
            bool isNative = (nativeFlag != 0);

            _nodeData->addNode(pID, point[0], point[1], point[2], isNative, tag);
        }
        return true;
    }

    bool FITKRadiossDataNodeAdaptor::adaptW()
    {
        _nodeData = dynamic_cast<Radioss::FITKRadiossNodes*>(_dataObj);
        if (_writer == nullptr || _nodeData == nullptr || _h5Group == nullptr) return false;

        int nodeCount = _nodeData->getNodeCount();
        //点
        // 行数 // 列数
        hsize_t       dimPtData[2]{ nodeCount ,3 };
        H5::DataSpace ptDataSpace(2, dimPtData);
        H5::DataSet   ptData = _h5Group->createDataSet("Points", H5::PredType::NATIVE_DOUBLE, ptDataSpace);
        //点ID
        // 行数 // 列数
        hsize_t       dimPtIDData[2]{ nodeCount ,1 };
        H5::DataSpace ptIDDataSpace(2, dimPtIDData);
        H5::DataSet   ptIDData = _h5Group->createDataSet("PointIDs", H5::PredType::NATIVE_ULLONG, ptIDDataSpace);

        hsize_t dimTagData[2]{ nodeCount, 1 };
        H5::DataSpace tagDataSpace(2, dimTagData);
        H5::DataSet tagData = _h5Group->createDataSet("PointTags", H5::PredType::NATIVE_UINT, tagDataSpace);

        hsize_t dimNativeFlagData[2]{ nodeCount, 1 };
        H5::DataSpace nativeFlagDataSpace(2, dimNativeFlagData);
        H5::DataSet nativeFlagData = _h5Group->createDataSet("PointNativeFlags", H5::PredType::NATIVE_INT, nativeFlagDataSpace);

        // 写入点数据
        hsize_t       dim1[] = { 1 };
        H5::DataSpace mspace1(1, dim1);
        //写入点ID数据
        hsize_t       dim3[] = { 3 };
        H5::DataSpace mspace3(1, dim3);

        //表格
        hsize_t       coordPtData[3][2];
        coordPtData[0][1] = 0;
        coordPtData[1][1] = 1;
        coordPtData[2][1] = 2;
        for (int i = 0; i < dimPtData[0]; ++i)
        {
            coordPtData[0][0] = i;
            coordPtData[1][0] = i;
            coordPtData[2][0] = i;
            //点
            //auto meshNode = dynamic_cast<Interface::FITKMeshNode*>(mesh->getNodeAt(i));
            Core::FITKNode* node = _nodeData->getNodeAt(i);
            const std::array< double, 3 > pt0{ node->x() ,node->y() ,node->z() };
            ptDataSpace.selectElements(H5S_SELECT_SET, 3, (const hsize_t*)coordPtData);
            ptData.write(&pt0, H5::PredType::NATIVE_DOUBLE, mspace3, ptDataSpace);
            //点ID
            int pID = node->getNodeID();
            ptIDDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            ptIDData.write(&pID, H5::PredType::NATIVE_INT, mspace1, ptIDDataSpace);

            unsigned int tag = node->getTag();
            tagDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            tagData.write(&tag, H5::PredType::NATIVE_UINT, mspace1, tagDataSpace);

            int isNative = node->getNativeFlag() ? 1 : 0;
            nativeFlagDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            nativeFlagData.write(&isNative, H5::PredType::NATIVE_INT, mspace1, nativeFlagDataSpace);
        }
        return true;
    }
}
