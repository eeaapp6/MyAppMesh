/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorRWallCYL.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorRWallCYL::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorRWallCYL";
    }

    bool FITKRadiossAdaptorRWallCYL::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        // 创建边界条件数据对象
        FITKRWallCylinder* cyl = new FITKRWallCylinder();
        //读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper *infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        //获取id和关键字
        int id = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();

        bool ok = false;
        while (_reader && !_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //读取刚性壁数据
            ok = this->readRWallCYL(cyl);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read RWall Error : %1 %2").arg(keyLine).arg(cyl->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read RWall : %1 %2").arg(keyLine).arg(cyl->getDataObjectName()));
        _caseData->getRWallManager()->appendDataObj(cyl);
        infoMapper->insertRWallIDMap(id, cyl->getDataObjectID());
        return ok;
    }

    bool FITKRadiossAdaptorRWallCYL::adaptW()
    {
        //写出刚性壁数据
        FITKRWallCylinder* wall = dynamic_cast<FITKRWallCylinder*>(_dataObj);
        if (wall == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadRWallID() + 1;
        //建立映射关系
        infoMapper->insertRWallIDMap(wall->getDataObjectID(), radID);
        *stream << wall->GetFITKRWallCylinderRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << wall->getDataObjectName() << endl;
        //节点组ID
        int grnd1 = infoMapper->getRadIDByNodeSetID(wall->getNodeGroupId1());
        int grnd2 = infoMapper->getRadIDByNodeSetID(wall->getNodeGroupId2());

        *stream << QString("#%1%2%3%4").arg("node_ID", 9).arg("Slide", 10).arg("grnd_ID1", 10).arg("grnd_ID2", 10) << endl;
        *stream << gapStr(wall->getNodeId()) << gapStr(wall->getSlidingType(), 10, false) <<
            gapStr(grnd1) << gapStr(grnd2) << endl;

        *stream << QString("#%1%2%3%4%5").arg("Dsearch", 19).arg("fric", 20).arg("Dia", 20).arg("ffac", 20).arg("ifq", 10) << endl;
        *stream << gapStr(wall->getDSearch()) << gapStr(wall->getFrictionCoef()) << gapStr(wall->getDiameter())
            << gapStr(wall->getFilteringFactor()) << gapStr(wall->getFilteringFlag(), 10, false) << endl;

        double xm, ym, zm;
        wall->getBasicPoint(xm, ym, zm);

        *stream << QString("#%1%2%3").arg("Xm1", 19).arg("Ym1", 20).arg("Zm1", 20) << endl;
        *stream << gapStr(xm) << gapStr(ym) << gapStr(zm) << endl;

        double xm1, ym1, zm1;
        wall->getAxisVector(xm1, ym1, zm1);
        *stream << QString("#%1%2%3").arg("Xm2", 19).arg("Ym2", 20).arg("Zm2", 20) << endl;
        *stream << gapStr(xm1 + xm) << gapStr(ym1 + ym) << gapStr(zm1 + zm) << endl;

        return true;
    }

    bool FITKRadiossAdaptorRWallCYL::readRWallCYL(FITKRWallCylinder * cyl)
    {
        if (cyl == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        cyl->setDataObjectName(line);
        //读取刚性壁信息
        QStringList lines;
        while (!_reader->atEnd())
        {
            line = _reader->readLine();
            if (line.startsWith("#")) continue;
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            lines.append(line);
        }
        //说明与手册不符
        if (lines.size() != 4)
        {
            _reader->consoleMessage(3, QString("Invalid RWall Cylinder : %1.").arg(cyl->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines);
        //属性对象赋值
        this->setData(cyl);
        return true;
    }

    void FITKRadiossAdaptorRWallCYL::readParam(const QStringList & lines)
    {
        if (lines.size() < 4) return;
        //第一行参数
        QString par0 = lines[0];
        bool ok = false;
        for (int i = 0; i < 4; ++i)
        {
            int par = par0.mid(10 * i, 10).trimmed().toInt(&ok);
            if (ok) _param.insert(i, par);
        }
        //第二行参数
        QString par1 = lines[1];
        for (int i = 0; i < 5; ++i)
        {
            double par = par1.mid(20 * i, 20).trimmed().toDouble(&ok);
            if (ok) _param.insert(i + 10, par);
        }
        //第三行参数
        QStringList par2 = lines[2].simplified().split(" ");
        for (int i = 0; i < par2.size(); ++i)
        {
            double par = par2[i].toDouble(&ok);
            if (ok) _param.insert(i + 20, par);
        }
        //第四行参数
        QStringList par3 = lines[3].simplified().split(' ');
        for (int i = 0; i < par3.size(); ++i)
        {
            double par = par3[i].toDouble(&ok);
            if (ok) _param.insert(i + 30, par);
        }
    }

    void FITKRadiossAdaptorRWallCYL::setData(FITKRWallCylinder * plane)
    {
        if (plane == nullptr) return;
        int nodeId = _param.value(0, plane->getNodeId());
        plane->setNodeId(nodeId);
        int slideType = _param.value(1, plane->getSlidingType());
        if (slideType == 0) plane->setSlidingType(FITKAbstractRWall::RWallSliding::Sliding);
        else if (slideType == 1) plane->setSlidingType(FITKAbstractRWall::RWallSliding::Tied);
        else if (slideType == 2) plane->setSlidingType(FITKAbstractRWall::RWallSliding::SlidingWithFriction);
        else plane->setSlidingType(plane->getSlidingType());
        plane->setNodeGroupId1(_param.value(2, plane->getNodeGroupId1()));
        plane->setNodeGroupId2(_param.value(3, plane->getNodeGroupId2()));
        plane->setDSearch(_param.value(10, plane->getDSearch()));
        plane->setFrictionCoef(_param.value(11, plane->getFrictionCoef()));
        plane->setDiameter(_param.value(12, plane->getDiameter()));
        plane->setFilteringFactor(_param.value(13, plane->getFilteringFactor()));
        plane->setFilteringFlag(_param.value(14, plane->getFilteringFlag()));

        //坐标
        double Xm1 = _param.value(20, 0);
        double Ym1 = _param.value(21, 0);
        double Zm1 = _param.value(22, 0);
        plane->setBasicPoint(Xm1, Ym1, Zm1);

        double Xm2 = _param.value(30, 0);
        double Ym2 = _param.value(31, 0);
        double Zm2 = _param.value(32, 0);
        plane->setAxisVector(Xm2- Xm1, Ym2- Ym1, Zm2- Zm1);
    }
}