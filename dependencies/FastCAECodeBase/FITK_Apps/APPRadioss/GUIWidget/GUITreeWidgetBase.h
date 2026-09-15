/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
* @file    GUITreeWidgetBase.h
* @brief   树形菜单基类
* @author  lilongyuan(lilongyuan@diso.cn)
* @date    2025-09-18
*********************************************************************/
#ifndef __GUI_TREE_WIDGET_BASE_H__
#define __GUI_TREE_WIDGET_BASE_H__

#include "FITK_Component/FITKWidget/FITKTreeWidget.h"
#include "GUIWidgetAPI.h"

namespace Radioss { class FITKRadiossCase; }

namespace GUI
{
	/**
	 * @brief   树形菜单基类
	 * @author  lilongyuan(lilongyuan@diso.cn)
	 * @date    2025-09-18
	 */
	class GUIWIDGETAPI GUITreeWidgetBase : public Comp::FITKTreeWidget
	{
	public:
		/**
		 * @brief     构造函数
		 * @param[i]  parent 父窗口
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		explicit GUITreeWidgetBase(QWidget* parent = nullptr);
		/**
		 * @brief   析构函数
		 * @author  lilongyuan(lilongyuan@diso.cn)
		 * @date    2025-09-18
		 */
		virtual ~GUITreeWidgetBase() = default;
		/**
		 * @brief   更新树形菜单（纯虚函数）
		 * @author  lilongyuan(lilongyuan@diso.cn)
		 * @date    2025-09-18
		 */
		virtual void updateTreeWidget() = 0;
	protected:
		/**
		 * @brief     创建根节点
		 * @param[i]  parentWidget 树形菜单
		 * @param[i]  name 节点名称
		 * @param[i]  type 节点类型
		 * @return    QTreeWidgetItem*
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		QTreeWidgetItem* createRootItem(QTreeWidget* parentWidget, QString name, int type);
		/**
		 * @brief     创建子节点
		 * @param[i]  parentItem 父节点
		 * @param[i]  name 节点名称
		 * @param[i]  type 节点类型
		 * @return    QTreeWidgetItem*
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		QTreeWidgetItem* createChildItem(QTreeWidgetItem* parentItem, QString name, int type);
		/**
		 * @brief     在节点中存储数据对象ID
		 * @param[i]  item 节点
		 * @param[i]  id 数据对象ID
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		void setObjectID(QTreeWidgetItem* item, int id);
		/**
		 * @brief     获取节点存储的数据对象ID
		 * @param[i]  item 节点
		 * @return    int 数据对象ID
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		int getObjectID(QTreeWidgetItem* item);
		/**
		 * @brief     设置节点点击状态
		 * @param[i]  item 节点
		 * @param[i]  column 所在列号
		 * @param[i]  checked 点击状态
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		void setItemChecked(QTreeWidgetItem* item, int column, bool checked);
		/**
		 * @brief     创建菜单项
		 * @param[i]  menu 菜单
		 * @param[i]  itemName 菜单项名称
		 * @param[i]  itemObjectName 菜单项对象名称
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		void createMenuItem(QMenu* menu, QString itemName, QString itemObjectName);
		/**
		 * @brief     显示/隐藏模型
		 * @param[i]  id 数据ID
		 * @param[i]  isShow 是否显示
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-23
		 */
		void show(int id, bool isShow);
		/**
		 * @brief     独立高亮
		 * @param[i]  id 对象ID
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		void highlight(int id);
		/**
		 * @brief     批量高亮
		 * @param[i]  ids 对象ID列表
		 * @author    lilongyuan(lilongyuan@diso.cn)
		 * @date      2025-09-18
		 */
		void highlight(QList<int> ids);
		/**
		 * @brief   清除全部高亮
		 * @author  lilongyuan(lilongyuan@diso.cn)
		 * @date    2025-09-18
		 */
		void clearAllHighlight();
		/**
		 * @brief   获取Radioss算例数据
		 * @return  Radioss::FITKRadiossCase* Radioss算例数据
		 * @author  lilongyuan(lilongyuan@diso.cn)
		 * @date    2025-09-18
		 */
		Radioss::FITKRadiossCase* getDataCase();
	};
}
#endif  // __GUI_TREE_WIDGET_BASE_H__