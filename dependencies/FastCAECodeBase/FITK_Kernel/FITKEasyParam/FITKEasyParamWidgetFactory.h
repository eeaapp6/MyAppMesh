/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_EASY_PARAM_WIDGET_FACTORY_H___
#define _FITK_EASY_PARAM_WIDGET_FACTORY_H___

#include "FITKEasyParamAPI.h"
#include <QString>

class QWidget;

namespace Core
{
    class FITKAbstractEasyParamWidget;
    class FITKAbstractEasyParam;

    class FITKEasyParamAPI FITKEasyParamWidgetFactory
    {
    public:
        static QWidget* createWidget(FITKAbstractEasyParam* data, QWidget * parent = nullptr, QString name = "");

    private:
        FITKEasyParamWidgetFactory() = default;
        ~FITKEasyParamWidgetFactory() = default;
    };

 
}


#endif
