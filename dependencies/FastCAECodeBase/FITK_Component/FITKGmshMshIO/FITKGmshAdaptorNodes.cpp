/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshAdaptorNodes.h"
#include "FITKGmshMshDataProcessor.h"
#include "FITKGmshMshIOReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextWriter.h"

namespace Gmsh
{
    QString FITKGmshAdaptorNodes::getAdaptorClass()
    {
        return "FITKGmshAdaptorNodes";
    }

    bool FITKGmshAdaptorNodes::adaptR()
    {
        //获取网格数据对象
        FITKGmshMshDataReaderProcessor* mshData = dynamic_cast<FITKGmshMshDataReaderProcessor*>(_dataObj);
        if (!mshData || !_reader) return false;
        //上一行
        QString line = _reader->previousLine();
        //获取关键字
        QString keywords = line.trimmed().toLower();
        keywords.remove("$");
        if (keywords != "nodes") return false;
        this->readNodesInfo();

        //开始读取节点
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine().trimmed();
            if (line.contains("$"))break;
            //维度 维度一致的第几个 是否有参数坐标 节点数
            QStringList info = line.split(' ', QString::SkipEmptyParts);
            if (info.size() != 4)continue;
            bool readOK = this->readNode(info);
            if (readOK)
                _reader->backLine();
            else
                return false;
        }

        return true;
    }

    bool FITKGmshAdaptorNodes::adaptW()
    {
        return true;
    }

    bool FITKGmshAdaptorNodes::readNodesInfo()
    {
        QString line = _reader->nextLine().trimmed();
        if (line.split(' ', QString::SkipEmptyParts).size() != 4) return false;
        //实体块 节点数 节点最小标号 节点最大标号
        QString nodesInfo = _reader->readLine().trimmed();
        QStringList info = nodesInfo.split(' ', QString::SkipEmptyParts);
        if (info.size() != 4) return false;
        //bool ok = false;
        ////实体块
        //int entityNum = info.at(0).toInt(&ok);
        //if (!ok) return false;
        ////节点数
        //int nodesNum = info.at(1).toInt(&ok);
        //if (!ok) return false;
        ////节点最小标号
        //int nodeMinID = info.at(2).toInt(&ok);
        //if (!ok) return false;
        ////节点最大标号
        //int nodeMaxID = info.at(3).toInt(&ok);
        //if (!ok) return false;

        return true;
    }

    bool FITKGmshAdaptorNodes::readNode(QStringList info)
    {
        FITKGmshMshDataReaderProcessor* mshData = dynamic_cast<FITKGmshMshDataReaderProcessor*>(_dataObj);
        if (!mshData) return false;
        //获取节点数
        bool ok = false;
        int nodeNum = info.at(3).toInt(&ok);
        if (!ok) return false;
        //读取节点数据
        QList<int> nodesID;
        QList<double> nodesX;
        QList<double> nodesY;
        QList<double> nodesZ;
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine().trimmed();
            if (line.contains("$"))break;
            QStringList info = line.split(' ', QString::SkipEmptyParts);
            if (info.size() == 4)break;
            if (info.isEmpty())continue;
            //节点编号
            if (info.size() == 1)
                nodesID.append(info.at(0).toInt(&ok));
            //节点坐标x轴 节点坐标y轴 节点坐标z轴
            else if (info.size() == 3)
            {
                nodesX.append(info.at(0).toDouble(&ok));
                nodesY.append(info.at(1).toDouble(&ok));
                nodesZ.append(info.at(2).toDouble(&ok));
            }
            if (!ok) return false;

        }
        if (nodesID.size() != nodeNum || nodesX.size() != nodeNum || nodesY.size() != nodeNum || nodesZ.size() != nodeNum)
            return false;
        //添加节点数据
        for (int i = 0; i < nodeNum; ++i)
        {
            int id = nodesID.at(i);
            double x = nodesX.at(i);
            double y = nodesY.at(i);
            double z = nodesZ.at(i);
            mshData->addNode(id, x, y, z);
        }
        return true;
    }
}


