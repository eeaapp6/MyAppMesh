/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFTransportPropertiesWrite.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportProp.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFTransportModel.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFTransportPropertiesWrite::FITKOFTransportPropertiesWrite()
    {
    }
    FITKOFTransportPropertiesWrite::~FITKOFTransportPropertiesWrite()
    {
    }

    void FITKOFTransportPropertiesWrite::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/constant/transportProperties").arg(filePath);
    }

    bool FITKOFTransportPropertiesWrite::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        //获取物理材料数据
        Interface::FITKOFTransportProp* transportData = physicsData->getTransportProp();
        if (!transportData) return true;
        //获取字典工程
        this->openDictObject();
        if (!m_objectDict)return false;
        //写出FoamFile
        this->dictObjectFoamFile();
        //设置写出的网格区域数据
        bool Ok = this->transportPropW(transportData);
        //写出字典文件
        if (Ok)
            Ok = this->writeDictFile();
        //关闭字典工程
        this->closeDictObject();

        this->writePhasesFile(transportData);

        return Ok;
    }

    QString FITKOFTransportPropertiesWrite::getFileName()
    {
        return m_fileName;
    }

    void FITKOFTransportPropertiesWrite::dictObjectFoamFile(QString name)
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"constant\"");
        if (name.isEmpty())
            group->append("object", "transportProperties");
        else
            group->append("object", QString("physicalProperties.%1").arg(name));
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFTransportPropertiesWrite::writePhasesFile(Interface::FITKOFTransportProp* transportData)
    {
        int count = transportData->getPhasesCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFTransportPhase* phase = transportData->getPhase(i);
            if (!phase) continue;
            QString name = phase->getPhaseName();
            m_fileName = QString("%1/constant/physicalProperties.%2").arg(m_filePath).arg(name);
            //打开字典工程
            this->openDictObject();
            if (!m_objectDict)return false;
            //写出FoamFile
            this->dictObjectFoamFile(name);
            bool ok = this->phaseOneW(phase);
            //写出字典文件
            if (ok)
                ok = this->writeDictFile();
            //关闭字典工程
            this->closeDictObject();
        }
        return true;
    }

    bool FITKOFTransportPropertiesWrite::transportPropW(Interface::FITKOFTransportProp* transportData)
    {
        if (!m_objectDict) return false;
        int count = transportData->getPhasesCount();
        bool ok = true;
        if (count == 1)
            ok = this->phaseOneW(transportData->getPhase(0));
        else
        {
            DICT::FITKDictArray* phases = new DICT::FITKDictArray;
            m_objectDict->append("phases", phases);
            for (int i = 0; i < count; ++i)
            {
                Interface::FITKOFTransportPhase* phase = transportData->getPhase(i);
                if (!phase) continue;
                QString name = phase->getPhaseName();
                phases->append(name);
                DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
                m_objectDict->append(name, group);
                ok &= this->phaseOneW(phase, group);
            }
        }
        Core::FITKParameter* additionalPara = transportData->getTransportAdditionalData();
        if (additionalPara)
        {
            int count = additionalPara->getDataCount();
            for (int i = 0; i < count; ++i)
            {
                QString key, value;
                bool ok = this->getPara(additionalPara->getDataByIndex(i), key, value);
                if (!ok)continue;
                m_objectDict->append(key, value);
            }
        }
        return ok;
    }

    bool FITKOFTransportPropertiesWrite::phaseOneW(Interface::FITKOFTransportPhase* phase, DICT::FITKDictGroup* group)
    {
        if (!phase || !m_objectDict)return false;
        //设置参数
        Core::FITKParameter* additionalPara = phase->getPhaseAdditionalData();
        if (additionalPara)
        {
            int count = additionalPara->getDataCount();
            for (int i = 0; i < count; ++i)
            {
                QString key, value;
                bool ok = this->getPara(additionalPara->getDataByIndex(i), key, value, false);
                if (!ok)continue;
                if (group) group->append(key, value);
                else m_objectDict->append(key, value);
            }
        }
        Interface::FITKAbsOFTransportModel* model = phase->getTransportModel();
        if (!model)return false;
        Core::FITKParameter* para = model->getTransportModelPara();
        if (!para)return false;
        QString namePara = model->getDataObjectName();
        if (group) group->append("transportModel", namePara);
        else m_objectDict->append("transportModel", namePara);
        int count = para->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            QString key, value;
            bool ok = this->getPara(para->getDataByIndex(i), key, value);
            if (!ok)continue;
            if (group) group->append(key, value);
            else m_objectDict->append(key, value);
        }
        return true;
    }

    bool FITKOFTransportPropertiesWrite::getPara(Core::FITKAbstractEasyParam* para, QString& key, QString& value, bool ok)
    {
        if (!para)return false;
        //获取名称
        QString name = para->getDataObjectName();
        bool isUnit = ok && name.contains('[') && name.contains(']');
        QString unit;
        if (isUnit)
        {
            int index = name.lastIndexOf(' ');
            key = this->getVName(name.mid(0, index));
            unit = this->getUnit(name.mid(index + 1));
        }
        else
            key = name;
        //获取参数类型
        Core::FITKAbstractEasyParam::FITKEasyParamType type = para->getParamType();
        if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
        {
            Core::FITKParamDouble* pD = dynamic_cast<Core::FITKParamDouble*>(para);
            if (!pD)return false;
            if (isUnit)
                value = QString("%1 %2 %3").arg(key).arg(unit).arg(QString::number(pD->getValue(), 'E', 1));
            else
                value = QString::number(pD->getValue());
        }
        return true;
    }

    QString FITKOFTransportPropertiesWrite::getVName(QString vName)
    {
        if (vName == "v")
            return "nu";
        return vName;
    }

    QString FITKOFTransportPropertiesWrite::getUnit(QString unit)
    {
        if (unit == "[m2/s]")
            return "[0 2 -1 0 0 0 0]";
        else if (unit == "[N/m]")
            return "[1 0 -2 0 0 0 0]";
        return "[0 0 0 0 0 0 0]";

        //QString unitLower = unit.toLower();
        //bool isNegative = false;
        //int num[7] = { 0 };
        //QStringList unitStrList; unitStrList << "n" << "m" << "s" << "0" << "0" << "0" << "0";
        //for (int i = 0; i < unitLower.size(); ++i)
        //{
        //    QChar ch = unitLower.at(i);
        //    QString unitStr;
        //    QString unitNum;
        //    if (ch == '/')
        //        isNegative = true;
        //    if (ch >= 'a' && ch <= 'z')
        //    {
        //        for (; i < unitLower.size(); ++i)
        //        {
        //            ch = unitLower.at(i);
        //            if (!(ch >= 'a' && ch <= 'z'))break;
        //            unitStr += ch;
        //        }
        //    }
        //    if (ch == '-' || (ch >= '0' && ch <= '9'))
        //    {
        //        for (; i < unitLower.size(); ++i)
        //        {
        //            ch = unitLower.at(i);
        //            if (!(ch == '-' || (ch >= '0' && ch <= '9')))break;
        //            unitNum += ch;
        //        }
        //        --i;
        //    }
        //    if (unitStr.isEmpty())continue;
        //    int index = unitStrList.indexOf(unitStr);
        //    if (index < 0 || index > 6)continue;
        //    int n = 1;
        //    if (!unitNum.isEmpty())
        //        n = unitNum.toInt();
        //    if (isNegative)
        //        n = -n;
        //    num[index] = n;
        //}
        //return QString("[%1 %2 %3 %4 %5 %6 %7]").arg(num[0]).arg(num[1]).arg(num[2]).arg(num[3]).arg(num[4]).arg(num[5]).arg(num[6]);
    }
}



