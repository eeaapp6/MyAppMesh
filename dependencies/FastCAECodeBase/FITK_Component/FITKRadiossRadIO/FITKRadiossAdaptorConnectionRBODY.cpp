/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorConnectionRBODY.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKConnectionRBODY.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"
#include "RadWriteInformationMapper.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorConnectionRBODY::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorConnectionRBODY";
    }

    bool FITKRadiossAdaptorConnectionRBODY::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;

        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        //读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter || !mapper) return false;

        // 创建对象
        FITKConnectionRBODY* rbody = new FITKConnectionRBODY();

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
            ok = this->readRRBODY(rbody);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Connection Error : %1 %2").arg(keyLine).arg(rbody->getDataObjectName()));
            return false;
        }
        mapper->insertConnectionIDMap(id, rbody->getDataObjectID());
        _reader->consoleMessage(1, QString("Read Connection : %1 %2").arg(keyLine).arg(rbody->getDataObjectName()));
        _caseData->getCurrentSolution()->getConnectionManager()->appendDataObj(rbody);
        return ok;
    }

    bool FITKRadiossAdaptorConnectionRBODY::adaptW()
    {
        //写出数据
        FITKConnectionRBODY* rbody = dynamic_cast<FITKConnectionRBODY*>(_dataObj);
        if (rbody == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        RadWriteInformationMapper* infoMapper = _writer->getWriteInpInformationMapper();
        if (stream == nullptr || infoMapper == nullptr) return false;
        //写出关键字行  ID 从1开始递增
        int radID = infoMapper->getMaxRadConnectionID() + 1;
        //建立映射关系
        infoMapper->insertConnectionIDMap(rbody->getDataObjectID(), radID);
        *stream << rbody->GetFITKConnectionRBODYRadiossKeyWord() << "/" << radID << endl;
        //写出名称
        *stream << rbody->getDataObjectName() << endl;
        //节点组ID
        int grnd = infoMapper->getRadIDByNodeSetID(rbody->getSecondaryNodesGroupID());
        //面ID
        int surfd = infoMapper->getRadIDBySurfaceID(rbody->getEnvelopeSurface());
        //力矩
        double j[6] = { 0.0 };
        rbody->getInertiaValue(j);

        *stream << QString("#%1%2%3%4%5%6%7%8%9").arg("node_ID", 9).arg("sens_ID", 10).arg("Skew_ID", 10)
            .arg("Ispher", 10).arg("Mass", 20).arg("grnd_ID", 10).arg("Ikrem", 10)
            .arg("ICoG", 10).arg("surf_ID", 10) << endl;
        *stream << gapStr(rbody->getMainNodeID()) << gapStr(rbody->getSensorID()) << gapStr(rbody->getSystemID())
            << gapStr(rbody->getInertiaType()) << gapStr(rbody->getMass()) << gapStr(grnd)
            << gapStr(rbody->getDeactivationRigidWall()) << gapStr(rbody->getGravityCenterComputation())
            << gapStr(surfd) << endl;

        *stream << QString("#%1%2%3").arg("Jxx", 19).arg("Jyy", 20).arg("Jzz", 20) << endl;
        *stream << gapStr(j[0]) << gapStr(j[1]) << gapStr(j[2]) << endl;

        *stream << QString("#%1%2%3").arg("Jxy", 19).arg("Jyz", 20).arg("Jxz", 20) << endl;
        *stream << gapStr(j[3]) << gapStr(j[4]) << gapStr(j[5]) << endl;

        *stream << QString("#%1%2").arg("Ioptoff", 9).arg("Ifail", 20)<< endl;
        *stream << gapStr(rbody->getDomainDecomposition()) << gapStr(rbody->getIfail(),20) <<endl;

        if (rbody->getIfail() != 1)return true;
        RBODY_OptionData& option = rbody->getOptionData();
        *stream << QString("#%1%2%3%4").arg("FN", 19).arg("FT", 20).arg("expN", 10).arg("expT", 10) << endl;
        *stream << gapStr(option.FN) << gapStr(option.FT) << gapStr(option.expN) << gapStr(option.expT) << endl;

        return true;
    }

    bool FITKRadiossAdaptorConnectionRBODY::readRRBODY(FITKConnectionRBODY * rbody)
    {
        if (rbody == nullptr || _reader == nullptr) return false;

        QString line = _reader->previousLine().trimmed();
        //名称
        rbody->setDataObjectName(line);
        //读取信息
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
            _reader->consoleMessage(3, QString("Invalid Connection RBODY: %1.").arg(rbody->getDataObjectName()));
            return false;
        }
        //读取属性参数
        this->readParam(lines);
        //属性对象赋值
        this->setData(rbody);
        return true;
    }

    void FITKRadiossAdaptorConnectionRBODY::readParam(const QStringList & lines)
    {
        if (lines.size() < 4) return;
        //第一行参数
        QString par1 = lines[0];
        bool ok = false;
        for (int i = 0; i < 10; ++i)
        {
            if (i == 4) {
                double par = par1.mid(10 * i, 20).trimmed().toInt(&ok);
                if (ok) _param.insert(i, par);
                ++i;
                continue;
            }
            int par = par1.mid(10 * i, 10).trimmed().toInt(&ok);
            if (ok) _param.insert(i, par);
        }
        //第二行参数
        QStringList par2 = lines[1].simplified().split(" ");
        for (int i = 0; i < par2.size(); ++i)
        {
            double par = par2[i].toDouble(&ok);
            if (ok) _param.insert(i + 10, par);
        }
        //第三行参数
        QStringList par3 = lines[2].simplified().split(" ");
        for (int i = 0; i < par3.size(); ++i)
        {
            double par = par3[i].toDouble(&ok);
            if (ok) _param.insert(i + 20, par);
        }
        //第四行参数
        QStringList par4= lines[3].simplified().split(' ');
        for (int i = 0; i < par4.size(); ++i)
        {
            int par = par4[i].toInt(&ok);
            if (ok) _param.insert(i + 30, par);
        }
        //第五行参数（可选）
        if ((int)_param.value(31) != 1)return;
        QStringList par5 = lines.value(4).simplified().split(' ');
        for (int i = 0; i < par5.size(); ++i)
        {
            if (i < 2) {
                double par = par5[i].toDouble(&ok);
                if (ok) _param.insert(i + 40, par);
                continue;
            }
            int par = par5[i].toInt(&ok);
            if (ok) _param.insert(i + 40, par);
        }
    }

    void FITKRadiossAdaptorConnectionRBODY::setData(FITKConnectionRBODY * rbody)
    {
        if (rbody == nullptr) return;
        rbody->setMainNodeID(_param.value(0, rbody->getMainNodeID()));
        rbody->setSensorID(_param.value(1, rbody->getSensorID()));
        rbody->setSystemID(_param.value(2, rbody->getSystemID()));
        rbody->setInertiaType((FITKConnectionRBODY::RbodyIspherType)(int)_param.value(3, rbody->getInertiaType()));
        rbody->setMass(_param.value(4, rbody->getMass()));
        rbody->setSecondaryNodesGroupID(_param.value(6, rbody->getSecondaryNodesGroupID()));
        rbody->setDeactivationRigidWall(_param.value(7, rbody->getDeactivationRigidWall()));
        rbody->setGravityCenterComputation((FITKConnectionRBODY::GravityCenterComputationType)(int)_param.value(8, rbody->getGravityCenterComputation()));
        rbody->setEnvelopeSurface(_param.value(9, rbody->getEnvelopeSurface()));

        double j[6] = { 0.0 };
        j[0] = _param.value(10, 0);
        j[1] = _param.value(11, 0);
        j[2] = _param.value(12, 0);
        j[3] = _param.value(20, 0);
        j[4] = _param.value(21, 0);
        j[5] = _param.value(22, 0);
    
        rbody->setDomainDecomposition(_param.value(30, rbody->getDomainDecomposition()));
        rbody->setIfail(_param.value(31, rbody->getIfail()));

        if (rbody->getIfail() != 1)return;
        RBODY_OptionData& option = rbody->getOptionData();
        option.FN = _param.value(40, option.FN);
        option.FT = _param.value(41, option.FT);
        option.expN = _param.value(42, option.expN);
        option.expT = _param.value(43, option.expT);
    }
}