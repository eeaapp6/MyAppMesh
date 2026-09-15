/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorMesh.h"

#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

#include "FITK_Interface/FITKInterfaceModel/FITKElementHex.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementQuad.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementTet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKElementTri.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKUnstructuredMesh.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKMeshNode.h"

#include <array>
#include <QSet>
#include <QList>
#include <QMultiHash>

#include "H5Cpp.h"

namespace IO
{

    QString FITKInterfaceHDF5AdaptorMesh::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorMesh";
    }

    bool FITKInterfaceHDF5AdaptorMesh::adaptR()
    {
        //auto mesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObj);
        //if (!_reader || !mesh || !_h5Group) return false;
        //bool isR = true;
        ////写出节点
        //isR &= readNode(mesh, *_h5Group);
        ////写出单元
        //isR &= readElement(mesh, *_h5Group);
        //return isR;
        return false;
    }

    bool FITKInterfaceHDF5AdaptorMesh::adaptW()
    {
        //auto mesh = dynamic_cast<Interface::FITKUnstructuredMesh*>(_dataObj);
        //if (!_writer || !mesh || !_h5Group) return false;
        //bool isW = true;
        ////写出节点
        //isW &= writeNode(mesh, *_h5Group);
        ////写出单元
        //isW &= writeElement(mesh, *_h5Group);
        //return isW;
        return false;
    }

    bool FITKInterfaceHDF5AdaptorMesh::readNode(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group)
    {
        if (mesh == nullptr) return false;

        //PointIDs
        if (!h5Group.nameExists("PointIDs")) return false;
        auto PointIDSet = h5Group.openDataSet("PointIDs");
        int n, m;
        //Points
        if (!getDataSetDim(h5Group, "Points", n, m)) return false;
        auto pointsSet = h5Group.openDataSet("Points");

        if (!h5Group.nameExists("PointTags")) return false;
        auto PointTagSet = h5Group.openDataSet("PointTags");

        if (!h5Group.nameExists("PointNativeFlags")) return false;
        auto PointNativeSet = h5Group.openDataSet("PointNativeFlags");

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
            std::array< double, 3 >  point;
            ptDataSpace.selectElements(H5S_SELECT_SET, 3, (const hsize_t*)coordPtData);
            pointsSet.read(&point, H5::PredType::NATIVE_DOUBLE, mspace3, ptDataSpace);

            //点ID
            int pID;
            ptIDDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            PointIDSet.read(&pID, H5::PredType::NATIVE_INT, mspace1, ptIDDataSpace);
            
            unsigned int tag ;
            ptTagDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            PointTagSet.read(&tag, H5::PredType::NATIVE_UINT, mspace1, ptTagDataSpace);

            int nativeFlag;
            ptNativeDataSpace.selectElements(H5S_SELECT_SET, 1, (const hsize_t*)coordPtData);
            PointNativeSet.read(&nativeFlag, H5::PredType::NATIVE_INT, mspace1, ptNativeDataSpace);
            bool isNative = (nativeFlag != 0);

            mesh->addNode(pID, point[0], point[1], point[2], isNative,tag);
        }
        return true;
    }

    bool FITKInterfaceHDF5AdaptorMesh::readElement(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group)
    {
        if (mesh == nullptr) return false;
        //创建Grid->Cells子节点 用于存储单元数据
        if (!h5Group.nameExists("Cells")) return false;
        H5::Group eleGroup = h5Group.openGroup("Cells");
        //循环类型

        int EleTypeCount = eleGroup.getNumAttrs();
        for (int i = 0; i < EleTypeCount; ++i)
        {
            auto eleType = eleGroup.openAttribute(i);
            //name
            std::string elementTypeName;
            H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
            eleType.read(datatype, elementTypeName);
            int n, m;
            if (!getDataSetDim(eleGroup, elementTypeName, n, m)) return false;

            hsize_t dimEleData[2]{ n ,m };

            H5::DataSpace eleDataSpace(2, dimEleData);
            H5::DataSet eleDataSet = eleGroup.openDataSet(elementTypeName);
            int i_Ele = -1;

            //枚举转换字符 确定解析的单元类型
            Core::FITKEnumTransfer<Interface::FITKModelEnum::FITKEleType> fitkEleTypeTrafer;
            bool isValid = false;
            auto elementType = fitkEleTypeTrafer.fromString(QString::fromStdString(elementTypeName), isValid);
            if (!isValid) return false;
            auto element = Interface::FITKElementFactory::createElement(elementType);
            if (!element) return false;
            int elementTag = readIntAttribute(eleGroup, "EleTag");
            element->setTag(elementTag);
            // 对同类单元进行每行循环读取数据
            for (int j = 0; j < n; ++j)
            {
                // hyperslab的开始
                hsize_t start[2] = { ++i_Ele, 0 };
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
                for (int k = 1; k < m; k++)
                {
                    nodes.append(ptIDArray[k]);
                }
                //添加点与单元
                element->setEleID(ptIDArray[0]);
                element->setNodeID(nodes);
                mesh->appendElement(element);
                delete[] ptIDArray;
            }
        }
        return true;
    }


    bool FITKInterfaceHDF5AdaptorMesh::writeNode(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group)
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

        hsize_t dimTagData[2]{ nodeCount, 1 };
        H5::DataSpace tagDataSpace(2, dimTagData);
        H5::DataSet tagData = h5Group.createDataSet("PointTags", H5::PredType::NATIVE_UINT, tagDataSpace);

        hsize_t dimNativeFlagData[2]{ nodeCount, 1 };
        H5::DataSpace nativeFlagDataSpace(2, dimNativeFlagData);
        H5::DataSet nativeFlagData = h5Group.createDataSet("PointNativeFlags", H5::PredType::NATIVE_INT, nativeFlagDataSpace);

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
            auto node = mesh->getNodeAt(i);
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

    bool FITKInterfaceHDF5AdaptorMesh::writeElement(Interface::FITKUnstructuredMesh* mesh, H5::Group& h5Group)
    {
        if (mesh == nullptr) return false;
        //创建Grid->Cells子节点 用于存储单元数据
        H5::Group eleGroup = h5Group.createGroup("Cells");
        //key 单元类型名称 value单元编号
        QMultiHash<QString, int> elementType = extractElementInformation(mesh);
        QList<QString> typeNames = elementType.keys().toSet().toList();
        for (QString typeName : typeNames)
        {
            auto elIDList = elementType.values(typeName);
            auto ele = mesh->getElementByID(elIDList[0]);
            const int eleNodeCount = ele->getNodeCount();
            int elementTag = ele->getTag();
            int i_Ele = -1;
            //行长度+1 将CellID 放在0的位置
            int lineSize = eleNodeCount + 1;
            hsize_t dimEleData[2]{ elIDList.size() ,lineSize };

            H5::DataSpace eleDataSpace(2, dimEleData);
            H5::DataSet eleDataSet = eleGroup.createDataSet(typeName.toStdString(), H5::PredType::NATIVE_INT, eleDataSpace);
            writeStrAttribute(eleDataSet, "EleType", typeName.toStdString());
            writeIntAttribute(eleDataSet, "EleTag", &elementTag);
            // 对同类单元进行循环
            for (int j = 0; j < elIDList.size(); ++j)
            {
                // hyperslab的开始
                hsize_t start[2] = { ++i_Ele, 0 };
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

                int* ptIDArray = new int[lineSize];
                for (int k = 0; k < eleNodeCount; ++k)
                    ptIDArray[k + 1] = mesh->getElementByID(elIDList[j])->getNodeID(k);
                ptIDArray[0] = elIDList[j];
                eleDataSet.write(ptIDArray, H5::PredType::NATIVE_INT, mspace2, eleDataSpace);
                delete[] ptIDArray;
            }

            //存储集合名称的链表 用index作为属性名称添加
            int numAttrCount = eleGroup.getNumAttrs();
            writeStrAttribute(eleGroup, std::to_string(numAttrCount), typeName.toStdString());
        }

        return true;
    }

    //提取单元信息
    QMultiHash<QString, int> FITKInterfaceHDF5AdaptorMesh::extractElementInformation(Interface::FITKUnstructuredMesh* mesh)
    {
        //key 单元类型名称 value单元编号
        QMultiHash<QString, int> elementType;
        if (mesh == nullptr) return elementType;
        //枚举转换字符 确定解析的单元类型
        Core::FITKEnumTransfer<Interface::FITKModelEnum::FITKEleType> fitkEleTypeTrafer;
        bool isValid = false;
        //提取信息
        for (int i = 0; i < mesh->getElementCount(); ++i)
        {
            auto ele = mesh->getElementAt(i);
            auto eleType = ele->getEleType();
            //枚举转换字符
            QString eleTypeStr = fitkEleTypeTrafer.toString(eleType, isValid);
            elementType.insert(eleTypeStr, ele->getEleID());
        }
        return elementType;
    }
}
