/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _ColorMapperEditDialog_H
#define _ColorMapperEditDialog_H

#include "GUIDialogAPI.h"
#include "GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

namespace Ui {
    class ColorMapperEditDialog;
}

namespace Interface {
    class FITKAbstractCFDPostData;
}

namespace GUI
{
    class GUIDialogAPI ColorMapperEditDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        ColorMapperEditDialog(Interface::FITKAbstractCFDPostData* postData, EventOper::ParaWidgetInterfaceOperator* oper);
        ~ColorMapperEditDialog();

        void init();
    private:
        void setDataToWidget();
    private slots:
        ;
        void on_pushButton_OK_clicked();
        void on_pushButton_Cencel_clicked();
    private:
        Interface::FITKAbstractCFDPostData* _postData = nullptr;
        Ui::ColorMapperEditDialog* _ui = nullptr;

        Interface::FITKPostFieldType _type = Interface::FITKPostFieldType::Post_None;
        QString _filedName = "";
        int _componentIndex = -1;
    };
}

#endif
