/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   DeformDialog.h
 * @brief  形变对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-18
 *********************************************************************/
#ifndef _DeformDialog_H
#define _DeformDialog_H

#include "GUIDialogBase.h"
#include "GUIDialogAPI.h"

namespace Ui {
    class DeformDialog;
}

namespace Interface {
    class FITKCFDPostDeform;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    /**
     * @brief  形变对话框
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-18
     */
    class GUIDialogAPI DeformDialog :public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] oper              操作器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        DeformDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        ~DeformDialog();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void init();
    private slots:
        ;
        /**
         * @brief    OK按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    Cancel按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_pushButton_Cancel_clicked();
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
        void getDataFromWidget(Interface::FITKCFDPostDeform* data);
        /**
         * @brief    将数据对象中的数据填充至界面
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setDataToWidget(Interface::FITKCFDPostDeform* data);
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        Ui::DeformDialog* _ui = nullptr;
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
