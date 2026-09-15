/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   IsosurfDialog.h
 * @brief  等值面对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-18
 *********************************************************************/
#ifndef _IsosurfDialog_H
#define _IsosurfDialog_H

#include "GUIDialogAPI.h"
#include "GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

namespace Ui {
    class IsosurfDialog;
}

namespace Interface {
    class FITKCFDPostIsosurface;
}

namespace GUI
{
    /**
     * @brief  等值面对话框
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-18
     */
    class GUIDialogAPI IsosurfDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] postDataID          后处理数据id
         * @param[i] oper                操作器
         * @param[i] isCreate            是否创建
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        IsosurfDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate);
        /**
         * @brief    析构函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        ~IsosurfDialog();
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
         * @brief    取消按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_pushButton_Cancel_clicked();
        void on_comboBox_Contour_currentIndexChanged(int index);
        /**
         * @brief    添加按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_pushButton_Add_clicked();
        /**
         * @brief    移除按钮点击事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_pushButton_Remove_clicked();
        /**
         * @brief    tableWidget点击事件
         * @param[i] row                行
         * @param[i] column             列
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void on_tableWidget_cellClicked(int row, int column);
    private:
        /**
         * @brief    创建初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void createInit();
        /**
         * @brief    编辑初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void editInit();
        /**
         * @brief    创建确认
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        bool createOK();
        /**
         * @brief    编辑确认
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        bool editOK();
        /**
         * @brief    将界面数据填充至数据对象中
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void getDataFromWidget(Interface::FITKCFDPostIsosurface* isoSurData);
        /**
         * @brief    将数据对象中的数据填充至界面
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setDataToWidget(Interface::FITKCFDPostIsosurface* isoSurData);
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        Ui::IsosurfDialog* _ui = nullptr;
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
         * @date   2024-09-26
         */
        bool _isCreate = false;
    };
}
#endif
