/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ToolBarBase.h
 * @brief  工具条基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef _ToolBarBase_H
#define _ToolBarBase_H

#include "GUIWidgetAPI.h"
#include <QToolBar>
#include <QComboBox>

namespace Interface {
    class FITKCFDPost3DManager;
    class PostGraphObjectManager;
    class FITKAbstractCFDPostData;
    class PostGraphObjectBase;
}

namespace GUI
{
    class GUIWidgetAPI ToolBarBase : public QToolBar
    {
        Q_OBJECT;
    public:
        ToolBarBase(QWidget* parent = nullptr);
        ~ToolBarBase();
        virtual void updateCurrentGraphObj(int objID) = 0;
    protected:
        QAction* createAction(QString actionName, QString iconPath = "", QString actionTitle = "");
        void reRender();
    protected:
        /**
         * @brief  后处理数据管理器
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-06
         */
        Interface::FITKCFDPost3DManager* _postDataManager = nullptr;

        Interface::PostGraphObjectManager* _postGraphManager = nullptr;
        /**
         * @brief  后处理数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-13
         */
        int _currentDataID = -1;
    };
}
#endif