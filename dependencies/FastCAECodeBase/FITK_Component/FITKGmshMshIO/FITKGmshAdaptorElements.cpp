/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshAdaptorElements.h"
#include "FITKGmshMshIOReader.h"
#include "FITKGmshMshDataProcessor.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"
//#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKElementFactory.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
//#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

namespace Gmsh
{
    QString FITKGmshAdaptorElements::getAdaptorClass()
    {
        return "FITKGmshAdaptorElements";
    }

    bool FITKGmshAdaptorElements::adaptR()
    {
        //获取msh数据对象
        FITKGmshMshDataReaderProcessor* mshData = dynamic_cast<FITKGmshMshDataReaderProcessor*>(_dataObj);
        if (!mshData || !_reader) return false;
        //上一行
        QString line = _reader->previousLine();
        //获取关键字
        QString keywords = line.trimmed().toLower();
        keywords.remove("$");
        if (keywords != "elements") return false;
        this->readElementsInfo();

        //开始读取节点
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine().trimmed();
            if (line.contains("$"))break;
            //维度 维度一致的第几个 单元类型 单元数
            QStringList info = line.split(' ', QString::SkipEmptyParts);
            if (info.size() != 4)continue;
            bool readOK = this->readElement(info);
            if (!readOK)
                return false;
        }

        return true;
    }

    bool FITKGmshAdaptorElements::adaptW()
    {
        return true;
    }

    bool FITKGmshAdaptorElements::readElementsInfo()
    {
        QString line = _reader->nextLine().trimmed();
        if (line.split(' ', QString::SkipEmptyParts).size() != 4) return false;
        //实体块 单元数 单元最小标号 单元最大标号
        QString elementsInfo = _reader->readLine().trimmed();
        QStringList info = elementsInfo.split(' ', QString::SkipEmptyParts);
        if (info.size() != 4) return false;
        //bool ok = false;
        ////实体块
        //int entityNum = info.at(0).toInt(&ok);
        //if (!ok) return false;
        ////节点数
        //int elementsNum = info.at(1).toInt(&ok);
        //if (!ok) return false;
        ////节点最小标号
        //int elementMinID = info.at(2).toInt(&ok);
        //if (!ok) return false;
        ////节点最大标号
        //int elementMaxID = info.at(3).toInt(&ok);
        //if (!ok) return false;
        return true;
    }

    bool FITKGmshAdaptorElements::readElement(QStringList info)
    {
        //获取msh数据对象
        FITKGmshMshDataReaderProcessor* mshData = dynamic_cast<FITKGmshMshDataReaderProcessor*>(_dataObj);
        if (!mshData || !_reader) return false;
        //获取节点数
        bool ok = false;
        int dim = info.at(0).toInt(&ok);
        if (!ok) return false;
        if (dim >= 4) return false;
        int elemType = info.at(2).toInt(&ok);
        if (!ok) return false;
        int elemNum = info.at(3).toInt(&ok);
        if (!ok) return false;
        //获取单元数据
        QList<int> elementsType;
        QList<int> elementsID;
        QList<QList<int>> elemsNodeID;
        for (int i = 0; i < elemNum; ++i)
        {
            if (_reader->atEnd())
                break;
            QString line = _reader->readLine().trimmed();
            if (line.contains("$"))break;
            //单元编号 节点1 ...
            QStringList info = line.split(' ', QString::SkipEmptyParts);
            if (info.isEmpty())continue;
            //获取单元编号
            int elemID = info.at(0).toInt(&ok);
            if (!ok) return false;
            //获取单元类型
            int nodeNum = info.size() - 1;
            QList<int> nodeIndexs;
            int type = this->getElementType(elemType, nodeNum, nodeIndexs);
            //获取单元节点id
            QList<int> nodeIDs;
            for (int index : nodeIndexs)
            {
                QString nodeIDStr = info.at(index + 1);
                int nodeID = nodeIDStr.toInt(&ok);
                if (!ok) return false;
                nodeIDs.append(nodeID);
            }
            elementsID.append(elemID);
            elementsType.append(type);
            elemsNodeID.append(nodeIDs);
        }
        if (elementsType.size() != elemNum || elemsNodeID.size() != elemNum || elementsID.size() != elemNum)
            return false;
        //添加单元
        for (int i = 0; i < elemNum; ++i)
        {
            int elemID = elementsID.at(i);
            Interface::FITKModelEnum::FITKEleType type = Interface::FITKModelEnum::FITKEleType(elementsType.at(i));
            QList<int> nodeIDs = elemsNodeID.at(i);
            mshData->addElement(elemID, type, dim, nodeIDs);
        }
        //创建集合
        if (elemNum > 0 && elementsType.at(0) != Interface::FITKModelEnum::FITKEleType::EleNone)
        {
            mshData->addElementSet(dim, elementsID);
        }
        return true;
    }

    int FITKGmshAdaptorElements::getElementType(int type, int & nodeNum, QList<int>& nodeIndexs)
    {
        if (type == 15 && nodeNum == 1)
        {
            nodeIndexs << 0;
            return Interface::FITKModelEnum::FITKEleType::Vertex1;
        }
        else if (type == 1 && nodeNum == 2)
        {
            nodeIndexs << 0 << 1;
            return Interface::FITKModelEnum::FITKEleType::Line2;
        }
        else if (type == 8 && nodeNum == 3)
        {
            nodeIndexs << 0 << 1 << 2;
            return Interface::FITKModelEnum::FITKEleType::Line3;
        }
        else if (type == 2 && nodeNum == 3)
        {
            nodeIndexs << 0 << 1 << 2;
            return Interface::FITKModelEnum::FITKEleType::Tri3;
        }
        else if (type == 9 && nodeNum == 6)
        {
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5;
            return Interface::FITKModelEnum::FITKEleType::Tri6;
        }
        else if (type == 3 && nodeNum == 4)
        {
            nodeIndexs << 0 << 1 << 2 << 3;
            return Interface::FITKModelEnum::FITKEleType::Quad4;
        }
        else if (type == 10 && nodeNum == 9)
        {
            nodeNum = 8;
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;
            return Interface::FITKModelEnum::FITKEleType::Quad8;
        }
        else if (type == 4 && nodeNum == 4)
        {
            nodeIndexs << 0 << 1 << 2 << 3;
            return Interface::FITKModelEnum::FITKEleType::Tet4;
        }
        else if (type == 11 && nodeNum == 10)
        {
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 9 << 8;
            return Interface::FITKModelEnum::FITKEleType::Tet10;
        }
        else if (type == 5 && nodeNum == 8)
        {
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;
            return Interface::FITKModelEnum::FITKEleType::Hex8;
        }
        else if (type == 12 && nodeNum == 27)
        {
            nodeNum = 20;
            nodeIndexs << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 11 
                << 13 << 9 << 16 << 18 << 19 << 17 << 10 << 12 << 14 << 15;
            return Interface::FITKModelEnum::FITKEleType::Hex20;
        }
        return Interface::FITKModelEnum::FITKEleType::EleNone;
    }
}


