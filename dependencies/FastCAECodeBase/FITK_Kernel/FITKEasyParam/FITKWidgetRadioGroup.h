/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _GUIWidgetRadioGroup_H
#define _GUIWidgetRadioGroup_H

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParamWidget.h"
#include <QWidget>

class QButtonGroup;
class QHBoxLayout;

namespace Ui {
    class FITKWidgetRadioGroup;
}

namespace Core
{
    class FITKWidgetHBox;
    class FITKAbstractEasyParam;
    class FITKParamRadioGroup;

    class FITKEasyParamAPI FITKWidgetRadioGroup : public QWidget, public FITKAbstractEasyParamWidget
    {
        Q_OBJECT;
    public:
        FITKWidgetRadioGroup(FITKAbstractEasyParam* dataBase, QWidget* parent = nullptr);
        ~FITKWidgetRadioGroup();

        void init();
    private:
        void initRadioData();
        void initSubData();
        /**
         * @brief    禁用Radio界面
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-08-22
         */
        void disableAllRadioWidget();
    private slots:
        ;
        void slotRadioClicked(int index);
    private:
        Ui::FITKWidgetRadioGroup* _ui = nullptr;
        FITKParamRadioGroup* _data = nullptr;

        QList<QWidget*> _subWidget = {};

        QButtonGroup* _group = nullptr;

        QHash<int, FITKWidgetHBox*> _radioWidgets = {};
    };
}

#endif
