/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKParameter.h"
#include "FITKParamBool.h"
#include "FITKParamInt.h"
#include "FITKParamDouble.h"
#include "FITKParamDoubleList.h"
#include "FITKParamCombox.h"
#include "FITKParamComboxVector.h"
#include "FITKParamString.h"
#include "FITKParamLabel.h"
#include "FITKParamRadioGroup.h"
#include "FITKParamBoolGroup.h"

namespace Core
{
    FITKParameter::~FITKParameter()
    {
    }

    void FITKParameter::copyPara(FITKParameter* data)
    {
        if (!data)return;
        this->clear();
        this->appendPara(data);
    }

    void FITKParameter::appendPara(FITKParameter* data)
    {
        if (!data)return;
        for (int i = 0; i < data->getDataCount(); i++) {
            FITKAbstractEasyParam* oldData = data->getDataByIndex(i);
            if(oldData == nullptr)continue;
            this->copyAppendPara(oldData);
        }
    }
    QList<FITKAbstractEasyParam*> FITKParameter::getParameter()
    {
        QList<FITKAbstractEasyParam*> list;
        for (int i = 0; i < this->getDataCount(); ++i)
        {
            list.append(getDataByIndex(i));
        }
        return list;
    }

    FITKParamInt* FITKParameter::createIntParameter(QString name, int value, int upperLimit, int lowerLimit)
    {
        FITKParamInt* propInt = new FITKParamInt;
        int range[2]{ lowerLimit, upperLimit };
        propInt->setRange(range);
        propInt->setDataObjectName(name);
        propInt->setValue(value);
        appendDataObj(propInt);
        return propInt;
    }
    FITKParamDouble* FITKParameter::createDoubleParameter(QString name, double value, double upperLimit, double lowerLimit)
    {
        FITKParamDouble* propDouble = new FITKParamDouble;
        double range[2]{ lowerLimit, upperLimit };
        propDouble->setRange(range);
        propDouble->setDataObjectName(name);
        propDouble->setValue(value);
        appendDataObj(propDouble);
        return propDouble;
    }
    FITKParamDoubleList* FITKParameter::createDoubleListParameter(QString name, QList<double> value, double upperLimit, double lowerLimit)
    {
        FITKParamDoubleList* propDoubleList = new FITKParamDoubleList;
        double range[2]{ lowerLimit, upperLimit };
        propDoubleList->setRange(range);
        propDoubleList->setDataObjectName(name);
        propDoubleList->setValue(value);
        appendDataObj(propDoubleList);
        return propDoubleList;
    }

    FITKParamComboxVector * FITKParameter::createComBoxVectorParameter(QString objName, QList<QStringList> values)
    {
        FITKParamComboxVector* paraComVec = new FITKParamComboxVector();
        paraComVec->setDataObjectName(objName);
        for (QStringList value : values) {
            if (value.size() <= 0)continue;

            FITKParamCombox* paraCom = new FITKParamCombox();
            paraCom->setValue(value);
            paraCom->setIndex(0);
            paraComVec->pushData(paraCom);
        }
        appendDataObj(paraComVec);
        return paraComVec;
    }

    FITKParamString* FITKParameter::createStringParameter(QString name, QString value)
    {
        FITKParamString* propString = new FITKParamString;
        propString->setDataObjectName(name);
        propString->setValue(value);
        appendDataObj(propString);
        return propString;
    }
    FITKParamBool* FITKParameter::createBoolParameter(QString name, bool value)
    {
        FITKParamBool* propBool = new FITKParamBool;
        propBool->setDataObjectName(name);
        propBool->setValue(value);
        appendDataObj(propBool);
        return propBool;
    }
    FITKParamBoolGroup* FITKParameter::createBoolGroupParameter(QString name, bool value)
    {
        FITKParamBoolGroup* propBool = new FITKParamBoolGroup;
        propBool->setDataObjectName(name);
        propBool->setValue(value);
        appendDataObj(propBool);
        return propBool;
    }
    FITKParamCombox* FITKParameter::createComboxParameter(QString name, QStringList value, int index)
    {
        FITKParamCombox* propCombox = new FITKParamCombox;
        propCombox->setDataObjectName(name);
        propCombox->setValue(value);
        propCombox->setIndex(index);
        appendDataObj(propCombox);
        return propCombox;
    }
    FITKParamRadioGroup* FITKParameter::createRadioGroupParameter(QString name)
    {
        FITKParamRadioGroup* propRadioGroup = new FITKParamRadioGroup;
        propRadioGroup->setDataObjectName(name);
        appendDataObj(propRadioGroup);
        return propRadioGroup;
    }

    void FITKParameter::onlyKeepPara(FITKParameter* data)
    {
        if (!data) return;
        QString nameList;
        int count = data->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            FITKAbstractEasyParam* d = data->getDataByIndex(i);
            if (d) nameList.append(d->getDataObjectName());
        }
        for (int i = this->getDataCount() - 1; i >= 0; --i)
        {
            FITKAbstractEasyParam* d = this->getDataByIndex(i);
            if (d && nameList.contains(d->getDataObjectName())) continue;
            if (d) delete d;
            this->removeDataByIndex(i);
        }
    }

    void FITKParameter::copyAppendPara(FITKAbstractEasyParam* oldData)
    {
        if (!oldData)return;
        FITKAbstractEasyParam* newData = nullptr;
        if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPBool)
            newData = new FITKParamBool;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPCombox)
            newData = new FITKParamCombox;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            newData = new FITKParamDouble;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPInt)
            newData = new FITKParamInt;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPLabel)
            newData = new FITKParamLabel;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPString)
            newData = new FITKParamString;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPRadioGroup)
            newData = new FITKParamRadioGroup;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
            newData = new FITKParamDoubleList;
        else if (oldData->getParamType() == FITKAbstractEasyParam::FITKEasyParamType::FEPBoolGroup)
            newData = new FITKParamBoolGroup;
        if (!newData)return;
        newData->copyParaData(oldData);
        this->appendDataObj(newData);
    }

    double FITKParameter::getDoubleParaByName(QString name, bool& ok)
    {
        ok = false;
        FITKParamDouble* data = dynamic_cast<FITKParamDouble*>(this->getDataByName(name));
        if (!data)return 0.0;
        ok = true;
        return data->getValue();
    }
    int FITKParameter::getIntParaByName(QString name, bool& ok)
    {
        ok = false;
        FITKParamInt* data = dynamic_cast<FITKParamInt*>(this->getDataByName(name));
        if (!data)return 0;
        ok = true;
        return data->getValue();
    }
    bool FITKParameter::getBoolParaByName(QString name, bool& ok)
    {
        ok = false;
        FITKParamBool* data = dynamic_cast<FITKParamBool*>(this->getDataByName(name));
        if (!data)return false;
        ok = true;
        return data->getValue();
    }
    QString FITKParameter::getStringParaByName(QString name, bool& ok)
    {
        ok = false;
        FITKParamString* data = dynamic_cast<FITKParamString*>(this->getDataByName(name));
        if (!data)return QString();
        ok = true;
        return data->getValue();
    }
    QString FITKParameter::getComboxCurrentParaByName(QString name, bool& ok)
    {
        ok = false;
        FITKParamCombox* data = dynamic_cast<FITKParamCombox*>(this->getDataByName(name));
        if (!data)return QString();
        ok = true;
        return data->getCurrentValue();
    }
}
