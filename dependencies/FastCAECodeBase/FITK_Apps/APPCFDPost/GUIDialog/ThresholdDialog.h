/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ThresholdDialog.h
 * @brief  阈值对话框
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-19
 *********************************************************************/
#ifndef _ThresholdDialog_H
#define _ThresholdDialog_H

#include "GUIDialogAPI.h"
#include "GUIDialogBase.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostEnum.h"

namespace Ui {
    class ThresholdDialog;
}

namespace Interface {
    class FITKCFDPostThreshold;
}

namespace GUI
{
    class GUIDialogAPI ThresholdDialog : public GUIDialogBase
    {
        Q_OBJECT;
    public:
        ThresholdDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate);
        ~ThresholdDialog();

        void init();
    private slots:
        ;
        void on_pushButton_OK_clicked();
        void on_pushButton_Cancel_clicked();
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
         * @param[i] data     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void getDataFromWidget(Interface::FITKCFDPostThreshold* data);
        /**
         * @brief    将数据对象中的数据填充至界面
         * @param[i] data     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void setDataToWidget(Interface::FITKCFDPostThreshold* data);
        /**
         * @brief    获取当前场量
         * @param[i] fieldType     场量类型 
         * @param[i] fieldName     场量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        void getCurrentField(Interface::FITKPostFieldType& fieldType, QString& fieldName);
        /**
         * @brief    设置当前场量
         * @param[i] fieldType     场量类型
         * @param[i] fieldName     场量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-14
         */
        void setCurrentField(Interface::FITKPostFieldType fieldType, QString fieldName);
    private:
        /**
         * @brief  ui对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-27
         */
        Ui::ThresholdDialog* _ui = nullptr;
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
