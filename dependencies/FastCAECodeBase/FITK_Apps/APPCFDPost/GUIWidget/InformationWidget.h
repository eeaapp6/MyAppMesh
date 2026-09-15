/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   InformationWidget.h
 * @brief  信息界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-11
 *********************************************************************/
#ifndef _InformationWidget_H_
#define _InformationWidget_H_

#include "GUIWidgetAPI.h"
#include "GUIWidgetBase.h"

namespace Ui {
    class InformationWidget;
}

namespace Interface {
    class FITKCFDPost3DManager;
    class FITKAbstractCFDPostData;
}

namespace GUI
{
    class GUIWidgetAPI InformationWidget : public GUIWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent     父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        explicit InformationWidget(QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        virtual ~InformationWidget();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void init();
        /**
         * @brief    更新界面
         * @param[i] postDataID      后处理数据id 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void updateCurrentGraphObj(int postDataID);
    private:
        /**
         * @brief    更新单元表格
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void updateCellTable();
        /**
         * @brief    更新场量数据表格
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void updateDataTable();
    private:
        /**
         * @brief  ui
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        Ui::InformationWidget* _ui = nullptr;
    };
}

#endif
