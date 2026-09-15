/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   StreamLineDialog.h
 * @brief  流线对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-19
 *********************************************************************/
#ifndef _StreamLineDialog_H
#define _StreamLineDialog_H

#include "GUIDialogAPI.h"
#include "GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

namespace Ui {
    class StreamLineDialog;
}

namespace Interface {
    class FITKCFDPostStreamLine;
    class PostGraphWidgetLine;
}

namespace GUI
{
    class GUIDialogAPI StreamLineDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        StreamLineDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate);
        ~StreamLineDialog();

        void init();
    protected:
        /**
         * @brief    显示事件
         * @param[i] event      事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void showEvent(QShowEvent* event);
        /**
         * @brief    关闭事件
         * @param[i] event      事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void closeEvent(QCloseEvent* event);
    private slots:
        ;
        void on_pushButton_OK_clicked();
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    数据修改槽函数
         * @param[i] startPoint        始点
         * @param[i] endPoint          终点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void slotValueChange(double* startPoint, double* endPoint);
        /**
         * @brief    界面数据修改槽函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void slotDataChange();
    private:
        /**
         * @brief    创建初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-24
         */
        void InitCreate();
        /**
         * @brief    编辑初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-24
         */
        void InitEdit();
        /**
         * @brief    创建确定
         * @return   bool         是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-24
         */
        bool OKCreate();
        /**
         * @brief    编辑确定
         * @return   bool         是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-24
         */
        bool OKEdit();
        /**
         * @brief    将界面数据填充至数据对象中
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void getDataFromWidget(Interface::FITKCFDPostStreamLine* streamLine);
        /**
         * @brief    将数据对象中的数据填充至界面
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setDataToWidget(Interface::FITKCFDPostStreamLine* streamLine);
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-27
         */
        Ui::StreamLineDialog* _ui = nullptr;
        /**
         * @brief  vtk线界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-19
         */
        Interface::PostGraphWidgetLine* _lineWidget = nullptr;
        /**
         * @brief  当前数据对象id
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-24
         */
        int _currentDataID = -1;
        /**
         * @brief  父对象数据ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-26
         */
        int _parentDataID = -1;
        /**
         * @brief  是否是创建模式
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-24
         */
        bool _isCreate = true;
    };
}

#endif
