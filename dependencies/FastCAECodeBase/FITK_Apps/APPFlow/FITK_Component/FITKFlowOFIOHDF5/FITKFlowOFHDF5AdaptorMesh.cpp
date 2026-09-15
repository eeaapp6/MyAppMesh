/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorMesh.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"

#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementPolygon.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"

//数据层
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"

#include <array>
#include <QSet>
#include <QList>
#include <QMultiHash>

#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorMesh::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorMesh";
    }

    bool FITKFlowOFHDF5AdaptorMesh::adaptR()
    {
        _unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredFluidMeshVTK*>(_dataObj);
        if (!_reader || !_unstructuredMesh) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        if (!readNDataObject(_unstructuredMesh, *_h5Group)) return false;
        bool isR = true;
        int count = 0;
        tool->readGroupint(*_h5Group, "RegionsMeshCount", &count);

        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> regionsMeshH5Group = tool->openGp(*_h5Group, name.c_str());
            if (!std::get<0>(regionsMeshH5Group)) continue;
            Interface::FITKFluidRegionsMesh* regionMesh = _unstructuredMesh->getDataByIndex(i);
            if (regionMesh == nullptr)
            {
                regionMesh = new Interface::FITKFluidRegionsMesh;
                _unstructuredMesh->appendDataObj(regionMesh);
            }
            isR &= regionMeshR(std::get<1>(regionsMeshH5Group), regionMesh);
        }

        return isR;
    }

    bool FITKFlowOFHDF5AdaptorMesh::adaptW()
    {
        _unstructuredMesh = dynamic_cast<Interface::FITKUnstructuredFluidMeshVTK*>(_dataObj);
        if (!_writer || !_unstructuredMesh) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        if (!writeNDataObject(_unstructuredMesh, *_h5Group)) return false;
        bool isW = true;
        
        int count = _unstructuredMesh->getDataCount();
        tool->writeGroupAttrInt(*_h5Group, "RegionsMeshCount", count);

        for (int i = 0; i < count; ++i)
        {
            Interface::FITKFluidRegionsMesh* regionMesh = _unstructuredMesh->getDataByIndex(i);
            if (!regionMesh) continue;
            std::tuple<bool, H5::Group> regionsMeshH5Group = tool->createGp(*_h5Group, QString::number(i));
            if (!std::get<0>(regionsMeshH5Group)) continue;
            isW &= regionMeshW(std::get<1>(regionsMeshH5Group), regionMesh);
        }

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorMesh::regionMeshW(H5::Group& h5Group, Interface::FITKFluidRegionsMesh* regionMesh)
    {
        if (!_writer || !regionMesh) return false;
        writeNDataObject(regionMesh, h5Group);
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        Interface::FITKUnstructuredMeshVTK* fieldMesh = regionMesh->getFieldMesh();
        Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr = regionMesh->getBoundaryMeshManager();
        if (!fieldMesh || !boundaryMeshMgr) return false;
        bool isW = true;
        isW &= fieldMeshW(h5Group, fieldMesh);
        isW &= boundaryMeshW(h5Group, boundaryMeshMgr);
        return isW;
    }

    bool FITKFlowOFHDF5AdaptorMesh::regionMeshR(H5::Group& h5Group, Interface::FITKFluidRegionsMesh* regionMesh)
    {
        if (!_reader || !regionMesh) return false;
        readNDataObject(regionMesh, h5Group);
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        Interface::FITKUnstructuredMeshVTK* fieldMesh = regionMesh->getFieldMesh();
        Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr = regionMesh->getBoundaryMeshManager();
        if (!fieldMesh || !boundaryMeshMgr) return false;
        bool isR = true;
        isR &= fieldMeshR(h5Group, fieldMesh);
        isR &= boundaryMeshR(h5Group, boundaryMeshMgr, fieldMesh);
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorMesh::fieldMeshW(H5::Group& h5Group, Interface::FITKUnstructuredMeshVTK* fieldMesh)
    {
        if (!_writer || !_unstructuredMesh || !fieldMesh)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //创建网格组
        std::tuple<bool, H5::Group> subGroup = tool->createGp(h5Group, "FieldMesh");
        if (std::get<0>(subGroup) == false) return false;
        writeNDataObject(fieldMesh, std::get<1>(subGroup));
        //写出节点
        writeNode(fieldMesh, std::get<1>(subGroup));
        //写出单元
        writeElement(fieldMesh, std::get<1>(subGroup));
        return true;
    }

    bool FITKFlowOFHDF5AdaptorMesh::fieldMeshR(H5::Group& h5Group, Interface::FITKUnstructuredMeshVTK* fieldMesh)
    {
        if (!_reader || !fieldMesh || !_unstructuredMesh)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取网格组
        std::tuple<bool, H5::Group> subGroup = tool->openGp(h5Group, "FieldMesh");
        if (std::get<0>(subGroup) == false) return false;
        readNDataObject(fieldMesh, std::get<1>(subGroup));
        //读取节点
        readNode(fieldMesh, std::get<1>(subGroup));
        //读取单元
        readElement(fieldMesh, std::get<1>(subGroup));
        return true;
    }
    bool FITKFlowOFHDF5AdaptorMesh::boundaryMeshW(H5::Group& h5Group, Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr)
    {
        if (!_writer || !_unstructuredMesh || !boundaryMeshMgr)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //创建边界网格分组
        std::tuple<bool, H5::Group> subGroup = tool->createGp(h5Group, "BoundaryMesh");
        if (std::get<0>(subGroup) == false) return false;
        //写出边界网格数据
        int count = boundaryMeshMgr->getDataCount();
        tool->writeGroupAttrInt(std::get<1>(subGroup), "BoundaryMeshCount", count);
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKBoundaryMeshVTK* boundaryMesh = boundaryMeshMgr->getDataByIndex(i);
            if (!boundaryMesh)continue;
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> boundaryH5Group = tool->createGp(std::get<1>(subGroup), name.c_str());
            if (!std::get<0>(boundaryH5Group)) continue;
            writeNDataObject(boundaryMesh, std::get<1>(boundaryH5Group));
            //写出单元数据
            writeElement(boundaryMesh, std::get<1>(boundaryH5Group));
        }

        return true;
    }
    bool FITKFlowOFHDF5AdaptorMesh::boundaryMeshR(H5::Group& h5Group, Interface::FITKBoundaryMeshVTKManager* boundaryMeshMgr, Interface::FITKUnstructuredMeshVTK* fieldMesh)
    {
        if (!_reader || !_unstructuredMesh || !boundaryMeshMgr)return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取边界网格分组
        std::tuple<bool, H5::Group> subGroup = tool->openGp(h5Group, "BoundaryMesh");
        if (std::get<0>(subGroup) == false) return false;
        int count = 0;
        tool->readGroupint(std::get<1>(subGroup), "BoundaryMeshCount", &count);
        for (int i = 0; i < count; ++i)
        {
            std::string name = std::to_string(i);
            std::tuple<bool, H5::Group> boundaryH5Group = tool->openGp(std::get<1>(subGroup), name.c_str());
            if (!std::get<0>(boundaryH5Group)) continue;
            Interface::FITKBoundaryMeshVTK* boundaryMesh = new Interface::FITKBoundaryMeshVTK(fieldMesh);
            readNDataObject(boundaryMesh, std::get<1>(boundaryH5Group));
            readElement(boundaryMesh, std::get<1>(boundaryH5Group));
            boundaryMeshMgr->appendDataObj(boundaryMesh);
        }

        return true;
    }

    bool FITKFlowOFHDF5AdaptorMesh::readNode(Interface::FITKNodeList* mesh, H5::Group& h5Group)
    {
        if (mesh == nullptr) return false;

        //PointIDs
        if (!h5Group.nameExists("PointIDs")) return false;
        auto PointIDSet = h5Group.openDataSet("PointIDs");
        int n, m;
        //Points
        if (!getDataSetDim(h5Group, "Points", n, m)) return false;
        auto pointsSet = h5Group.openDataSet("Points");

        //点
        // 行数 // 列数
        hsize_t       dimPtData[2]{ n ,m };
        H5::DataSpace ptDataSpace(2, dimPtData);
        //点ID
        // 行数 // 列数
        hsize_t       dimPtIDData[2]{ n ,1 };
        H5::DataSpace ptIDDataSpace(2, dimPtIDData);

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
            std::array< double, 3 >  point;
            ptDataSpace.selectElements(H5S_SELECT_SET, 3, (const hsize_t*)coordPtData);
            pointsSet.read(&point, H5::PredType::NATIVE_DOUBLE, mspace3, ptDataSpace);

            //点ID
            int pID;
            ptIDDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            PointIDSet.read(&pID, H5::PredType::NATIVE_INT, mspace1, ptIDDataSpace);
            mesh->addNode(pID, point[0], point[1], point[2]);
        }
        PointIDSet.close();
        pointsSet.close();
        return true;
    }

    bool FITKFlowOFHDF5AdaptorMesh::readElement(Interface::FITKElementList* mesh, H5::Group& h5Group)
    {
        if (mesh == nullptr) return false;
        //获取行列
        int n, m;
        if (!getDataSetDim(h5Group, "Element", n, m)) return false;
        hsize_t dimEleData[2]{ n ,m };
        //获取单元数据
        H5::DataSpace eleDataSpace(2, dimEleData);
        H5::DataSet eleDataSet = h5Group.openDataSet("Element");
        //循环类型
        // 对同类单元进行每行循环读取数据
        for (int i = 0; i < n; ++i)
        {
            // hyperslab的开始
            hsize_t start[2] = { i, 0 };
            // hyperslab的步长
            hsize_t stride[2] = { 1, m };
            hsize_t count[2] = { 1, 1 };
            // Block sizes
            hsize_t block[2] = { 1, m };
            eleDataSpace.selectHyperslab(H5S_SELECT_SET, count, start, stride, block);
            hsize_t dim[] = { m * 2 };

            H5::DataSpace mspace2(1, dim);
            hsize_t start2[1] = { 0 };
            hsize_t stride2[1] = { 1 };
            hsize_t count2[1] = { m };
            hsize_t block2[1] = { 1 };
            mspace2.selectHyperslab(H5S_SELECT_SET, count2, start2, stride2, block2);

            int* ptIDArray = new int[m];
            eleDataSet.read(ptIDArray, H5::PredType::NATIVE_INT, mspace2, eleDataSpace);
            QList<int> nodes;
            int eleNodeCount = ptIDArray[2];
            if (eleNodeCount + 3 > m)return false;
            for (int k = 0; k < eleNodeCount; ++k)
            {
                nodes.append(ptIDArray[k + 3]);
            }
            Interface::FITKAbstractElement* element = Interface::FITKElementFactory::createElement(Interface::FITKModelEnum::FITKEleType(ptIDArray[1]));
            if (!element) return false;
            //添加点与单元
            auto ele = dynamic_cast<Interface::FITKElementPolygon*>(element);
            if (ele)
                ele->setNodeCount(nodes.size());
            element->setEleID(ptIDArray[0]);
            element->setNodeID(nodes);
            mesh->appendElement(element);
            delete[] ptIDArray;
        }
        eleDataSet.close();
        return true;
    }


    bool FITKFlowOFHDF5AdaptorMesh::writeNode(Interface::FITKNodeList* mesh, H5::Group& h5Group)
    {
        if (mesh == nullptr) return false;
        // 写出节点函数主体
        int nodeCount = mesh->getNodeCount();
        //点
        // 行数 // 列数
        hsize_t       dimPtData[2]{ nodeCount ,3 };
        H5::DataSpace ptDataSpace(2, dimPtData);
        H5::DataSet   ptData = h5Group.createDataSet("Points", H5::PredType::NATIVE_DOUBLE, ptDataSpace);
        //点ID
        // 行数 // 列数
        hsize_t       dimPtIDData[2]{ nodeCount ,1 };
        H5::DataSpace ptIDDataSpace(2, dimPtIDData);
        H5::DataSet   ptIDData = h5Group.createDataSet("PointIDs", H5::PredType::NATIVE_ULLONG, ptIDDataSpace);

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
            auto node = mesh->getNodeAt(i);
            if (!node)continue;
            const std::array< double, 3 > pt0{ node->x() ,node->y() ,node->z() };
            ptDataSpace.selectElements(H5S_SELECT_SET, 3, (const hsize_t*)coordPtData);
            ptData.write(&pt0, H5::PredType::NATIVE_DOUBLE, mspace3, ptDataSpace);
            //点ID
            int pID = node->getNodeID();
            ptIDDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            ptIDData.write(&pID, H5::PredType::NATIVE_INT, mspace1, ptIDDataSpace);
        }
        ptData.close();
        ptIDData.close();
        return true;
    }

    bool FITKFlowOFHDF5AdaptorMesh::writeElement(Interface::FITKElementList* mesh, H5::Group& h5Group)
    {
        if (mesh == nullptr) return false;
        // 写出单元函数主体
        int elemCount = mesh->getElementCount();
        int maxLength = 0;
        for (int i = 0; i < elemCount; ++i)
        {
            //单元
            auto ele = mesh->getElementAt(i);
            if (!ele)continue;
            int num = ele->getNodeCount();
            if (maxLength < num)
                maxLength = num;
        }
        int lineSize = maxLength + 3;
        hsize_t dimEleData[2]{ elemCount ,lineSize };

        H5::DataSpace eleDataSpace(2, dimEleData);
        H5::DataSet eleDataSet = h5Group.createDataSet("Element", H5::PredType::NATIVE_INT, eleDataSpace);
        // 对同类单元进行循环
        for (int i = 0; i < elemCount; ++i)
        {
            // hyperslab的开始
            hsize_t start[2] = { i, 0 };
            // hyperslab的步长
            hsize_t stride[2] = { 1, lineSize };
            hsize_t count[2] = { 1, 1 };
            // Block sizes
            hsize_t block[2] = { 1, lineSize };
            eleDataSpace.selectHyperslab(H5S_SELECT_SET, count, start, stride, block);
            hsize_t dim[] = { lineSize * 2 };

            H5::DataSpace mspace2(1, dim);
            hsize_t start2[1] = { 0 };
            hsize_t stride2[1] = { 1 };
            hsize_t count2[1] = { lineSize };
            hsize_t block2[1] = { 1 };
            mspace2.selectHyperslab(H5S_SELECT_SET, count2, start2, stride2, block2);
            //写入单元数据
            auto ele = mesh->getElementAt(i);
            if (!ele)continue;
            int eleNodeCount = ele->getNodeCount();
            int* ptIDArray = new int[lineSize];
            for (int k = 0; k < eleNodeCount; ++k)
                ptIDArray[k + 3] = ele->getNodeID(k);
            ptIDArray[0] = ele->getEleID();
            ptIDArray[1] = ele->getEleType();
            ptIDArray[2] = eleNodeCount;
            eleDataSet.write(ptIDArray, H5::PredType::NATIVE_INT, mspace2, eleDataSpace);
            delete[] ptIDArray;
        }
        eleDataSet.close();
        return true;
    }

    bool FITKFlowOFHDF5AdaptorMesh::getDataSetDim(H5::Group& h5group, std::string name, int& n, int& m)
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
