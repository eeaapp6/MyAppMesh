/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    GUIPropertySolidDialog.h
 * @brief   属性弹窗
 * @author  lilongyuan (lilongyuan@diso.cn)
 * @date    2025-07-17
 *********************************************************************/
#ifndef __GUI_PropertySolid_DIALOG_H__
#define __GUI_PropertySolid_DIALOG_H__

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include <QString>

class QComboBox;
class QLineEdit;

namespace Radioss
{
    class FITKPropAbstract;
}

namespace Ui
{
    class GUIPropertyDialog;
}

namespace Core
{
    class FITKActionOperator;
}

namespace Comp
{
    class FITKSciNotationLineEdit;
}

namespace GUI
{
    /**
     * @brief   属性弹窗类（当前仅支持跌落实体模型）
     * @author  lilongyuan (lilongyuan@diso.cn)
     * @date    2025-07-17
     */
    class GUIAnalysisDialogAPI GUIPropertySolidDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief     构造函数
         * @param[i]  oper 操作器
         * @param[i]  property 要编辑的属性对象
         * @param[i]  parent 父窗口
         * @author  ZhouZhouYi (zhouzhaoyi@diso.cn)
         * @date    2025-08-01
         */
        explicit GUIPropertySolidDialog(Core::FITKActionOperator* oper, Radioss::FITKPropAbstract* property, QWidget* parent = nullptr);

        /**
         * @brief   析构函数
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        virtual ~GUIPropertySolidDialog();

    private slots:
        /**
         * @brief   确定按钮点击事件
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-21
         */
        void on_pushButtonOK_clicked();

    private:
        /**
         * @brief   初始化GUI
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        void initGUI();

        /**
         * @brief     初始化表格
         * @param[i]  rowCount 行数
         * @param[i]  columnCount 列数
         * @param[i]  listFirst 第一列内容
         * @author    lilongyuan (lilongyuan@diso.cn)
         * @date      2025-07-17
         */
        void initTableWidget(int rowCount, int columnCount, QStringList listFirst);

        /**
         * @brief   更新Solid表格
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-21
         */
        void updateDropSolid();

        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-15
         */
        void writePythonScript();

    private:
        /**
         * @brief     改变下拉框样式
         * @param[i]  comboBox
         * @author    lilongyuan (lilongyuan@diso.cn)
         * @date      2025-07-17
         */
        void changeComboBoxStyle(QComboBox* comboBox);

        /**
         * @brief     创建下拉框
         * @param[i]  list 下拉选项
         * @param[i]  currentIndex 当前选中项
         * @param[i]  isEnable 是否可用
         * @return    QComboBox* 下拉框
         * @author    lilongyuan (lilongyuan@diso.cn)
         * @date      2025-07-21
         */
        QComboBox* createComboBox(QStringList list, int currentIndex = 0, bool isEnable = true);

        /**
         * @brief     创建编辑框
         * @param[i]  text 编辑框内容
         * @param[i]  isEnable 是否可用
         * @return    QLineEdit* 编辑框
         * @author    lilongyuan (lilongyuan@diso.cn)
         * @date      2025-09-18
         */
        Comp::FITKSciNotationLineEdit* createLineEdit(QString text, int model = 0, bool isEnable = true);

    private:
        /**
         * @brief   界面指针
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        Ui::GUIPropertyDialog* _ui{};

        /**
         * @brief   操作器指针
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        Core::FITKActionOperator* _oper{};

        /**
         * @brief   编辑的属性对象（编辑模式时使用）
         * @author  ZhouZhouYi (zhouzhaoyi@diso.cn)
         * @date    2025-08-01
         */
        Radioss::FITKPropAbstract* _editProperty{};

        /**
         * @brief   是否为编辑模式
         * @author  ZhouZhouYi (zhouzhaoyi@diso.cn)
         * @date    2025-08-01
         */
        bool _isEditMode{false};
        /**
         * @brief   写脚本时使用的编辑前名称
         * @author  wangning (2185896382@qq.com)
         * @date    2026-05-22
         */
        QString _oldNameForScript{};
    };
}
#endif  // __GUI_PROPERTY_DIALOG_H__