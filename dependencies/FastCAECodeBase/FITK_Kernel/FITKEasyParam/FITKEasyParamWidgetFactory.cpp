/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKEasyParamWidgetFactory.h"
#include "FITKAbstractEasyParam.h"
#include "FITKParamCombox.h"
#include "FITKParamComboxVector.h"
#include "FITKParamDouble.h"
#include "FITKParamDoubleList.h"
#include "FITKParamBoolGroup.h"
#include "FITKWidgetBool.h"
#include "FITKWidgetBoolGroup.h"
#include "FITKWidgetComBox.h"
#include "FITKWidgetDouble.h"
#include "FITKWidgetInt.h"
#include "FITKWidgetRadioGroup.h"
#include "FITKWidgetString.h"
#include "FITKWidgetHBox.h"

#include <QString>

namespace Core
{
    QWidget* FITKEasyParamWidgetFactory::createWidget(FITKAbstractEasyParam * data, QWidget * parent, QString name)
    {
        QWidget* widget = nullptr;
        if (data == nullptr)return widget;
        FITKAbstractEasyParam::FITKEasyParamType type = data->getParamType();
        switch (type) {
        case FITKAbstractEasyParam::FITKEasyParamType::FEPInt: {
            auto w = new FITKWidgetInt(data, parent);
            widget = w;
            break;
        }
        case FITKAbstractEasyParam::FITKEasyParamType::FEPDouble: {
            auto w = new FITKWidgetDouble(data, parent);
            widget = w;
            break;
        }
        case FITKAbstractEasyParam::FITKEasyParamType::FEPString: {
            auto w = new FITKWidgetString(data, parent);
            widget = w;
            break;
        }
        case FITKAbstractEasyParam::FITKEasyParamType::FEPCombox: {
            auto w = new FITKWidgetComBox(data, parent);
            widget = w;
            break;
        }
        case FITKAbstractEasyParam::FITKEasyParamType::FEPComboxVector: {
            FITKParamComboxVector* comboxsData = dynamic_cast<FITKParamComboxVector*>(data);
            if (comboxsData == nullptr)break;
            QList<QWidget*> widgets = {};
            for (FITKParamCombox* d : comboxsData->getDatas()) {
                if (!d)continue;
                widgets.append(new FITKWidgetComBox(d, parent));
            }
            FITKWidgetHBox* w = new FITKWidgetHBox(widgets, parent);
            widget = w;
            break;
        }
        case FITKAbstractEasyParam::FITKEasyParamType::FEPBool: {
            auto w = new FITKWidgetBool(data, parent);
            w->setText(name);
            widget = w;
            break;
        }
        case FITKAbstractEasyParam::FITKEasyParamType::FEPRadioGroup: {
            auto w = new FITKWidgetRadioGroup(data, parent);
            widget = w;
            break;
        }case FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList: {
            auto doubleListData = dynamic_cast<FITKParamDoubleList*>(data);
            if (doubleListData == nullptr)break;
            QList<QWidget*> widgets = {};
            for (FITKParamDouble* d : doubleListData->getDoubleDatas()) {
                if (!d)continue;
                widgets.append(new FITKWidgetDouble(d, parent));
            }
            FITKWidgetHBox* w = new FITKWidgetHBox(widgets, parent);
            widget = w;
            break;
        }case FITKAbstractEasyParam::FITKEasyParamType::FEPBoolGroup: {
            auto boolGroupData = dynamic_cast<FITKParamBoolGroup*>(data);
            if (boolGroupData == nullptr)break;
            FITKWidgetBoolGroup* w = new FITKWidgetBoolGroup(boolGroupData, parent);
            widget = w;
            break;
        }
        }
        return widget;
    }
}