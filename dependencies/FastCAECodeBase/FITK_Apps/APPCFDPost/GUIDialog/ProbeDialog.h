/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ProbeDialog.h
 * @brief  探针对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-18
 *********************************************************************/
#ifndef _ProbeDialog_H
#define _ProbeDialog_H

#include "GUIDialogBase.h"
#include "GUIDialogAPI.h"

namespace Ui {
    class ProbeDialog;
}

namespace Interface {
    class FITKAbstractCFDPostData;
}

namespace EventOper {
    class ParaWidgetInterfaceOperator;
}

namespace GUI
{
    class ClipSliceWidgetBase;
    /**
     * @brief  探针对话框
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-18
     */
    class GUIDialogAPI ProbeDialog :public GUIDialogBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] oper              操作器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        ProbeDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        ~ProbeDialog();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void init();
        /**
         * @brief    插入点
         * @param[i] point     点
         * @param[i] pointID   点ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-22
         */
        void insertPoint(double* point, int pointID);
        /**
         * @brief    插入单元
         * @param[i] cellID    单元
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void insertCell(int cellID);
    protected:
        /**
         * @brief    关闭事件重写
         * @param[i] event        事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void closeEvent(QCloseEvent* event) override;
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
        /**
         * @brief    点添加
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        void on_pushButton_PointAdd_clicked();
        /**
         * @brief    点选择
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        void on_pushButton_PointSelect_clicked();
        /**
         * @brief    点选择取消事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-04
         */
        void on_pushButton_PointSelectCancel_clicked();
        /**
         * @brief    单元选择
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void on_pushButton_CellSelect_clicked();
        /**
         * @brief    单元选择取消事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-11-04
         */
        void on_pushButton_CellSelectCancel_clicked();
        /**
         * @brief    移除事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void on_pushButton_Remove_clicked();
        /**
         * @brief    清空事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void on_pushButton_Clear_clicked();
        /**
         * @brief    类型切换
         * @param[i] index      类型索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void on_comboBox_Type_currentIndexChanged(int index);
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
        void getDataFromWidget(Interface::FITKAbstractCFDPostData* glyphData);
        /**
         * @brief    将数据对象中的数据填充至界面
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setDataToWidget(Interface::FITKAbstractCFDPostData* glyphData);
        /**
         * @brief    表格添加点
         * @param[i] point      点
         * @param[i] pointID    点id(id == -1时默认为创建点)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-18
         */
        void tableAddPoint(double* point, int pointID);
        /**
         * @brief    表格添加单元
         * @param[i] cellID     单元ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        void tableAddCell(int cellID);
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        Ui::ProbeDialog* _ui = nullptr;
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
