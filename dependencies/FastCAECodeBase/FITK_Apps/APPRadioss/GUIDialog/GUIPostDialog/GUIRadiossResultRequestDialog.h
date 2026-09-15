/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRadiossResultRequestDialog.h
 * @brief  结果请求窗口类
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-09
 *********************************************************************/
#ifndef _GUIRadiossResultRequestDialog_H
#define _GUIRadiossResultRequestDialog_H

#include "GUIPostDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"

namespace Core
{
    class FITKActionOperator;
}

namespace Radioss
{
    class FITKRadiossResultRequest;
}

namespace Ui {
class GUIRadiossResultRequestDialog;
}

class QTreeWidgetItem;

namespace GUI
{
    /**
     * @brief  子节点数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-08
     */
    class ChildTreeNodeData
    {
    public:
        ChildTreeNodeData() {};
        /**
         * @brief    设置子节点名称
         * @param[i] text 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setText(QString text)
        {
            this->_text = text;
        }
        /**
         * @brief    启用子节点的CheckBox
         * @param[i] isEnable 
         * @param[i] objectName 
         * @param[i] data 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setEnableCheckBox(bool isEnable, QString objectName = "", bool data = false)
        {
            this->_isEnableCheckBox = isEnable;
            this->_checkBox_objectName = objectName;
            this->_checkBox_Data = data;
        }
        /**
         * @brief    启用子节点的LineEdit
         * @param[i] isEnable 
         * @param[i] objectName 
         * @param[i] data 
         * @param[i] unit 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setEnableLineEdit(bool isEnable, QString objectName = "", QString data = "", QString unit = "")
        {
            this->_isEnableLineEdit = isEnable;
            this->_lineEdit_objectName = objectName;
            this->_lineEdit_Data = data;
            this->_unit = unit;
        }
        /**
         * @brief    启用子节点的ComboBox
         * @param[i] isEnable 
         * @param[i] objectName 
         * @param[i] items 
         * @param[i] itemDatas 
         * @param[i] data 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void setEnableComboBox(bool isEnable, QString objectName = "", QStringList items = QStringList(), QList<int> itemDatas = QList<int>(), int data = 0)
        {
            _isEnableComboBox = isEnable;
            _comboBox_objectName = objectName;
            _comboBox_items = items;
            _comboBox_ItemDatas = itemDatas;
            _comboBox_Data = data;
        }
        /**
         * @brief    启用子节点的PushButton
         * @param[i] isEnable 
         * @param[i] objectName 
         * @param[i] text 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-24
         */
        void setEnablePushButton(bool isEnable, QString objectName = "", QString text = "")
        {
            _isEnablePushButton = isEnable;
            _pushButton_objectName = objectName;
            _pushButton_Text = text;
        }
        /**
         * @brief  子节点名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        QString _text{};
        /**
         * @brief  子节点是否启用checkBox
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        bool _isEnableCheckBox{ false };
        QString _checkBox_objectName{};
        bool _checkBox_Data{ false };
        /**
         * @brief  子节点是否启用LineEdit
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        bool _isEnableLineEdit{ false };
        QString _lineEdit_objectName{};
        QString _lineEdit_Data{};
        QString _unit{};
        /**
         * @brief  子节点是否启用ComboBox
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        bool _isEnableComboBox{ false };
        QString _comboBox_objectName{};
        QStringList _comboBox_items{};
        QList<int> _comboBox_ItemDatas{};
        int _comboBox_Data{ 0 };
        /**
         * @brief  子节点是否启用PushButton
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-24
         */
        bool _isEnablePushButton{ false };
        QString _pushButton_objectName{};
        QString _pushButton_Text{};
    };


    /**
     * @brief  结果请求窗口类
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-09
     */
    class GUIPostDialogAPI GUIRadiossResultRequestDialog : public GUIDialogBase
    {
        Q_OBJECT
    public:

        GUIRadiossResultRequestDialog(Core::FITKActionOperator* oper, QWidget *parent = nullptr);
        virtual ~GUIRadiossResultRequestDialog();

    protected slots:
        /**
         * @brief  取消按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-09
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    OK按钮
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void on_pushButton_OK_clicked();

        void pushButton_ACCEL_clicked();
        void pushButton_INTER_clicked();
        void pushButton_RWALL_clicked();
        void pushButton_RBODY_clicked();
        void pushButton_SECTIO_clicked();
        void pushButton_SPHCEL_clicked();

    private:
        /**
         * @brief    初始化
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void init();
        /**
         * @brief    初始化动画参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void initAnimationParamsTreeNode();
        /**
         * @brief    初始化节点动画输出参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void initNodalOutputAnimationsTreeNode();
        /**
         * @brief    初始化单元动画输出参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void initElementalOutputAnimationsTreeNode();
        /**
         * @brief    初始化体单元动画输出参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-31
         */
        void initBrickOutputAnimationsTreeNode();
        /**
         * @brief    初始化壳单元动画输出参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-31
         */
        void initShellOutputAnimationsTreeNode();
        /**
         * @brief    初始化时间历程输出参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-03
         */
        void initTimeHistoryTreeNode();
        /**
         * @brief    设置动画参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-03
         */
        void setAnimFileParam();
        /**
         * @brief    设置节点动画输出参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-03
         */
        void setNodalOutputAnim();
        /**
         * @brief    设置单元动画输出参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-03
         */
        void setElementalOutputAnim();
        /**
         * @brief    设置体单元动画输出参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-03
         */
        void setBrickOutputAnim();
        /**
         * @brief    设置壳单元动画输出参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-03
         */
        void setShellOutputAnim();
        /**
         * @brief    设置时间历程数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-07
         */
        void setTimeHistory();
        /**
         * @brief    添加根节点
         * @param[i] text 
         * @return   QTreeWidgetItem *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        QTreeWidgetItem* addTopLevelItem(const QString& text);
        /**
         * @brief    添加子节点
         * @param[i] p 
         * @param[i] childItemData 
         * @return   QTreeWidgetItem *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        QTreeWidgetItem* addChildItem(QTreeWidgetItem* p, const ChildTreeNodeData& childItemData);
        /**
         * @brief    获取CheckBox状态
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        bool getCheckBoxState(const QString& objectName);
        /**
         * @brief    获取LineEdit内容
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        double getLineEditValue(const QString& objectName);
        /**
         * @brief    将结果请求的命令写入Python脚本
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-05-15
         */
        void writePythonScript();
    protected:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-09
         */
        Ui::GUIRadiossResultRequestDialog* _ui = nullptr;
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-09
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  结果请求
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-09
         */
        Radioss::FITKRadiossResultRequest* _resultRequest{ nullptr };
    };
}


#endif // _GUISolverDialog_H
