/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   NodeGroupWidget.h
 * @brief  节点集合Widget
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-08
 *********************************************************************/
#ifndef _NodeGroupWidget_H
#define _NodeGroupWidget_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include <QComboBox>

namespace Ui
{
    class NodeGroupWidget;
}

namespace Interface
{
    class FITKComponentManager;
}

namespace GUI
{
    enum WidgetGroupType
    {
        Set_Node,
        Set_Element,
        Surface_Node,
        Surface_Element,
    };
    /**
     * @brief  节点集合Widget
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-08
     */
    class GUIAnalysisDialogAPI NodeGroupWidget : public Core::FITKWidget
    {
        Q_OBJECT
    public:
        explicit NodeGroupWidget(QWidget* parent = nullptr);
        ~NodeGroupWidget();
        /**
         * @brief    设置类型
         * @param[i] type
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-26
         */
        void setWidgetGroupType(WidgetGroupType type);
        /**
         * @brief    获取节点集合ID
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        int getNodeGroupId();
        /**
         * @brief    在ComboBox中找节点集合
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        void findNodeGroup(int nodeGroupId);

        void setFirstNodeGroup(QString name, int id = 0);

        //获取当前下拉框显示的条目
        QString getCurrentName();

    private:
        /**
         * @brief    初始化
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        void init();
    private slots:
        
        /**
         * @brief    创建按钮
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        void on_pushButton_create_clicked();
        void on_comboBox_nodeGroup_currentIndexChanged(int index);
        void onComboBoxAboutToShow(Core::FITKAbstractOperator * oper);
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-08
         */
        Ui::NodeGroupWidget* _ui{};
        /**
         * @brief  组类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-25
         */
        WidgetGroupType _type{ WidgetGroupType::Set_Node };
        /**
         * @brief  集合管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-25
         */
        Interface::FITKComponentManager* _componentManager{ nullptr };
        QString _firstNodeGroupName = "None";
        int _firstNodeGroupId = 0;
    };

    /**
     * @brief  重写ComboBox：展开前先更新内容，再执行原展开逻辑
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-05
     */
    class GUIAnalysisDialogAPI NodeGroupComboBox : public QComboBox
    {
        Q_OBJECT

    public:
        explicit NodeGroupComboBox(QWidget *parent = nullptr);
        /**
         * @brief    设置类型
         * @param[i] type
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-25
         */
        void setWidgetGroupType(WidgetGroupType type);

        /**
         * @brief    更新Item
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-26
         */
        void updateItem();
        /**
         * @brief  重写showPopup：展开前先更新内容，再执行原展开逻辑
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-05
         */
        void showPopup() override;
        /**
         * @brief    调整宽度
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-08
         */
        void adjustComboBoxWidth();
        /**
         * @brief    设置ComboBox第一项的内容
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-05
         */
        void setFirstItem(QString name, int id = 0);

        QString getFirstItemName() const { return _name; }
        int getFirstItemId() const { return _id; }
    private:
        /**
         * @brief  集合管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-25
         */
        Interface::FITKComponentManager* _componentManager{ nullptr };
        /**
         * @brief  组类型
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-25
         */
        WidgetGroupType _type{ WidgetGroupType::Set_Node };
        QString _name = "None";
        int _id = 0;
    };

}

#endif
