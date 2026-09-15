/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorRWallPlane.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRWall.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorRWallPlane::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorRWallPlane";
    }

    bool FITKRadiossAdaptorRWallPlane::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 创建边界条件数据对象
        FITKRWallPlane* plane = new FITKRWallPlane();
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
            //读取无限平面刚性壁数据
            ok = this->readRWallPlane(plane);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read RWall Error : %1 %2").arg(keyLine).arg(plane->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read RWall : %1 %2").arg(keyLine).arg(plane->getDataObjectName()));
        _caseData->getRWallManager()->appendDataObj(plane);
        infoMapper->insertRWallIDMap(id, plane->getDataObjectID());
        return ok;
    }

    bool FITKRadiossAdaptorRWallPlane::adaptW()
    {
        //写出刚性壁数据
        FITKRWallPlane* wall = dynamic_cast<FITKRWallPlane*>(_dataObj);
        if (wall == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadRWallID() + 1;
        //建立映射关系
        infoMapper->insertRWallIDMap(wall->getDataObjectID(), radID);
        *stream << wall->GetFITKRWallPlaneRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << wall->getDataObjectName() << endl;
        //节点组ID
        int grnd1 = infoMapper->getRadIDByNodeSetID(wall->getNodeGroupId1());
        int grnd2 = infoMapper->getRadIDByNodeSetID(wall->getNodeGroupId2());

        *stream << QString("#%1%2%3%4").arg("node_ID", 9).arg("Slide", 10).arg("grnd_ID1", 10).arg("grnd_ID2", 10) << endl;
        *stream << gapStr(wall->getNodeId()) << gapStr(wall->getSlidingType(), 10, false) <<
            gapStr(grnd1) << gapStr(grnd2) << endl;

        *stream << QString("#%1%2%3%4%5").arg("Dsearch", 19).arg("fric", 20).arg("Dia", 20).arg("ffac", 20).arg("ifq", 10) << endl;
        *stream << gapStr(wall->getDSearch()) << gapStr(wall->getFrictionCoef()) <<gapStr(wall->getDiameter())
            << gapStr(wall->getFilteringFactor()) << gapStr(wall->getFilteringFlag(), 10, false) << endl;

        double xm, ym, zm;
        wall->getBasicPoint(xm, ym, zm);
        if(wall->getNodeId()==0){
            *stream << QString("#%1%2%3").arg("Xm", 19).arg("Ym", 20).arg("Zm", 20) << endl;
            *stream << gapStr(xm) << gapStr(ym) << gapStr(zm) << endl;
        }
        else {
            *stream << QString("#%1%2%3%4").arg("Mass", 19).arg("VX0", 20).arg("VY0", 20).arg("VZ0", 20) << endl;
            *stream << endl;/*无接口*/
        }
        double xm1, ym1, zm1;
        wall->getNormalVector(xm1, ym1, zm1);
        *stream << QString("#%1%2%3").arg("Xm1", 19).arg("Ym1", 20).arg("Zm1", 20) << endl;
        *stream << gapStr(xm1+ xm) << gapStr(ym1+ ym) << gapStr(zm1+ zm) << endl;

        return true;
    }

    bool FITKRadiossAdaptorRWallPlane::readRWallPlane(FITKRWallPlane * plane)
    {
        if (plane == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        plane->setDataObjectName(line);
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
        if (lines.size() < 4)
        {
            _reader->consoleMessage(3, QString("Invalid RWall Plane : %1.").arg(plane->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines);
        //属性对象赋值
        this->setData(plane);
        return true;
    }

    void FITKRadiossAdaptorRWallPlane::readParam(const QStringList & lines)
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

    void FITKRadiossAdaptorRWallPlane::setData(FITKRWallPlane * plane)
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
        if (nodeId == 0)
        {
            double xm = _param.value(20, 0.0);
            double ym = _param.value(21, 0.0);
            double zm = _param.value(22, 0.0);
            plane->setBasicPoint(xm, ym, zm);
        }
        else
        {
            double mass = _param.value(20);
            double vx0 = _param.value(21);
            double vy0 = _param.value(22);
            double vz0 = _param.value(23);
        }
        double xm1 = _param.value(30, 0.0);
        double ym1 = _param.value(31, 0.0);
        double zm1 = _param.value(32, 0.0);

        double bx{0.0}, by{0.0}, bz{0.0};
        plane->getBasicPoint(bx, by, bz);
        plane->setNormalVector(xm1-bx, ym1-by, zm1-bz);
    }
}


