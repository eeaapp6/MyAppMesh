/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorGravity.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"

#include "FITKRadiossRadReader.h"
#include "RadKeyLineSpliter.h"
#include "FITKRadiossRadWriter.h"
#include "RadWriteInformationMapper.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorGravity::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorRWallPlane";
    }

    bool FITKRadiossAdaptorGravity::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 创建边界条件数据对象
        FITKGravity* gravity = new FITKGravity();
        //读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        RadReadInformationMapper* infoMapper = _reader->getReadRadInformationMapper();
        if (!keyLineSpliter || !infoMapper) return false;

        //获取id和关键字
        int gid = keyLineSpliter->getId(0);
        int uid = keyLineSpliter->getId(1);
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
            //读取重力数据
            ok = this->readGravity(gravity);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Gravity Error : %1 %2").arg(keyLine).arg(gravity->getDataObjectName()));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Gravity : %1 %2").arg(keyLine).arg(gravity->getDataObjectName()));
        _caseData->getGravityManager()->appendDataObj(gravity);
        infoMapper->insertGravIDMap(gid, gravity->getDataObjectID());
        return ok;
    }

    bool FITKRadiossAdaptorGravity::adaptW()
    {
        //写出重力数据
        FITKGravity* gravity = dynamic_cast<FITKGravity*>(_dataObj);
        if (gravity == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadGravityID() + 1;
        //建立映射关系
        infoMapper->insertGravityIDMap(gravity->getDataObjectID(), radID);
        *stream << gravity->GetFITKGravityRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << gravity->getDataObjectName() << endl;
        //节点组ID
        int grnd = infoMapper->getRadIDByNodeSetID(gravity->getNodeGroupID());
        //方向
        QString dir{};
        switch (gravity->getGravityDirection()) {
        case FITKGravity::GravityDirection::GRAVITY_X:dir = "X"; break;
        case FITKGravity::GravityDirection::GRAVITY_Y:dir = "Y"; break;
        case FITKGravity::GravityDirection::GRAVITY_Z:dir = "Z"; break;
        }

        *stream << QString("#%1%2%3%4%5%6%7").arg("fct_IDT", 9).arg("Dir", 10).arg("skew_ID", 10)
            .arg("sens_ID", 10).arg("grnd_ID", 10).arg("Ascalex", 30).arg("FscaleY", 20) << endl;
        *stream << gapStr(gravity->getTimeFunctionID()) << QString("%1").arg(dir, 10)
            << gapStr(gravity->getSystemID()) << gapStr(gravity->getSensorID())
            << gapStr(grnd) << gapStr(gravity->getAScaleX(),30)
            << gapStr(gravity->getFScaleY()) << endl;

        return true;
    }

    bool FITKRadiossAdaptorGravity::readGravity(FITKGravity * gravity)
    {
        if (gravity == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        gravity->setDataObjectName(line);
        //读取重力信息
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
        if (lines.empty())
        {
            _reader->consoleMessage(3, QString("Invalid Gravity : %1.").arg(gravity->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines, gravity);
        return true;
    }

    void FITKRadiossAdaptorGravity::readParam(const QStringList & lines, FITKGravity * gravity)
    {
        if (lines.empty() || gravity == nullptr) return;
        QString par0 = lines[0];
        bool ok = false;
        int fctIDT = par0.mid(0, 10).trimmed().toInt(&ok);
        if (ok) gravity->setTimeFunctionID(fctIDT);
        //重力方向
        QString dir = par0.mid(10, 10).trimmed();
        if (dir == "X") 
            gravity->setGravityDirection(FITKGravity::GravityDirection::GRAVITY_X);
        else if (dir == "Y")
            gravity->setGravityDirection(FITKGravity::GravityDirection::GRAVITY_Y);
        else if (dir == "Z")
            gravity->setGravityDirection(FITKGravity::GravityDirection::GRAVITY_Z);
        else
            gravity->setGravityDirection(gravity->getGravityDirection());
        //局部坐标系Id
        int skewId = par0.mid(20, 10).trimmed().toInt(&ok);
        if (ok) gravity->setSystemID(skewId);
        //传感器Id
        int sensId = par0.mid(30, 10).trimmed().toInt(&ok);
        if (ok) gravity->setSensorID(sensId);
        //节点Id
        int grndId = par0.mid(40, 10).trimmed().toInt(&ok);
        if (ok) gravity->setNodeGroupID(grndId);
        //Ascalex
        double aScaleX = par0.mid(60, 20).trimmed().toDouble(&ok);
        if (ok) gravity->setAScaleX(aScaleX);
        //FscaleY
        double fScaleY = par0.mid(80, 20).trimmed().toDouble(&ok);
        if (ok) gravity->setFScaleY(fScaleY);
    }

}


