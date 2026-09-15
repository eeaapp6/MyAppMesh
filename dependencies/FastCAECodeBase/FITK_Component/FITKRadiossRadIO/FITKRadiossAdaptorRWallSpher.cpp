/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorRWallSpher.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorRWallSpher::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorRWallSpher";
    }

    bool FITKRadiossAdaptorRWallSpher::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        FITKRWallSpherical* spher = new FITKRWallSpherical();
        RadKeyLineSpliter* keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        int id = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();

        bool ok = this->readRWallSpher(spher);
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read RWall Error : %1 %2").arg(keyLine).arg(spher->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read RWall : %1 %2").arg(keyLine).arg(spher->getDataObjectName()));
        _caseData->getRWallManager()->appendDataObj(spher);
        infoMapper->insertRWallIDMap(id, spher->getDataObjectID());
        return ok;
    }

    bool FITKRadiossAdaptorRWallSpher::adaptW()
    {
        FITKRWallSpherical* wall = dynamic_cast<FITKRWallSpherical*>(_dataObj);
        if (wall == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;

        int radID = infoMapper->getMaxRadRWallID() + 1;
        infoMapper->insertRWallIDMap(wall->getDataObjectID(), radID);
        *stream << wall->GetFITKRWallSphericalRadiossKeyWord() << "/" << radID << endl;
        *stream << wall->getDataObjectName() << endl;

        int grnd1 = infoMapper->getRadIDByNodeSetID(wall->getNodeGroupId1());
        int grnd2 = infoMapper->getRadIDByNodeSetID(wall->getNodeGroupId2());

        *stream << QString("#%1%2%3%4").arg("node_ID", 9).arg("Slide", 10).arg("grnd_ID1", 10).arg("grnd_ID2", 10) << endl;
        *stream << gapStr(wall->getNodeId()) << gapStr(wall->getSlidingType(), 10, false) <<
            gapStr(grnd1) << gapStr(grnd2) << endl;

        *stream << QString("#%1%2%3%4%5").arg("D_search", 19).arg("fric", 20).arg("Diameter", 20).arg("ffac", 20).arg("ifq", 10) << endl;
        *stream << gapStr(wall->getDSearch()) << gapStr(wall->getFrictionCoef()) << gapStr(wall->getDiameter())
            << gapStr(wall->getFilteringFactor()) << gapStr(wall->getFilteringFlag(), 10, false) << endl;

        double xm, ym, zm;
        wall->getCenter(xm, ym, zm);
        *stream << QString("#%1%2%3").arg("XM", 19).arg("YM", 20).arg("ZM", 20) << endl;
        *stream << gapStr(xm) << gapStr(ym) << gapStr(zm) << endl;

        return true;
    }

    bool FITKRadiossAdaptorRWallSpher::readRWallSpher(FITKRWallSpherical* spher)
    {
        if (spher == nullptr || _reader == nullptr) return false;
        //名称
        QString line = _reader->readLine().trimmed();
        spher->setDataObjectName(line);

        bool ok = false;
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
            if (line.trimmed().isEmpty()) continue;
            lines.append(line);
        }
        //说明与手册不符
        if (lines.size() < 3)
        {
            _reader->consoleMessage(3, QString("Invalid RWall Spherical : %1.").arg(spher->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines);
        //属性对象赋值
        this->setData(spher);
        return true;
    }

    void FITKRadiossAdaptorRWallSpher::readParam(const QStringList& lines)
    {
        _param.clear();
        if (lines.size() < 3) return;
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
    }

    void FITKRadiossAdaptorRWallSpher::setData(FITKRWallSpherical* spher)
    {
        if (spher == nullptr) return;

        spher->setNodeId(_param.value(0, spher->getNodeId()));
        int slideType = _param.value(1, spher->getSlidingType());
        if (slideType == 0) spher->setSlidingType(FITKAbstractRWall::RWallSliding::Sliding);
        else if (slideType == 1) spher->setSlidingType(FITKAbstractRWall::RWallSliding::Tied);
        else if (slideType == 2) spher->setSlidingType(FITKAbstractRWall::RWallSliding::SlidingWithFriction);
        else spher->setSlidingType(spher->getSlidingType());
        spher->setNodeGroupId1(_param.value(2, spher->getNodeGroupId1()));
        spher->setNodeGroupId2(_param.value(3, spher->getNodeGroupId2()));
        spher->setDSearch(_param.value(10, spher->getDSearch()));
        spher->setFrictionCoef(_param.value(11, spher->getFrictionCoef()));
        spher->setDiameter(_param.value(12, spher->getDiameter()));
        spher->setFilteringFactor(_param.value(13, spher->getFilteringFactor()));
        spher->setFilteringFlag(_param.value(14, spher->getFilteringFlag()));

        double Xc = _param.value(20, 0);
        double Yc = _param.value(21, 0);
        double Zc = _param.value(22, 0);
        spher->setCenter(Xc, Yc, Zc);
    }
}