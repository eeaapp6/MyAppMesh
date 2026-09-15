/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ToolBarColorMapper.h
 * @brief  颜色映射工具条
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef _ToolBarColorMapper_H
#define _ToolBarColorMapper_H

#include "GUIWidgetAPI.h"
#include "ToolBarBase.h"
#include <QComboBox>

namespace Interface {
    class FITKAbstractCFDPostData;
}

namespace GUI
{
    class GUIComboBox;
    class GUIWidgetAPI ToolBarColorMapper : public ToolBarBase
    {
        Q_OBJECT;
    public:
        ToolBarColorMapper(QWidget* parent = nullptr);
        ~ToolBarColorMapper();
        /**
         * @brief    根据当前渲染对象修改颜色映射
         * @param[i] objID    渲染对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual void updateCurrentGraphObj(int objID) override;
    private slots:
        ;
        void slotShowColorMapp();
        void slotEditColorMapp();
        /**
         * @brief    场变量修改事件
         * @param[i] index    索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        void slotVariableChange(int index);
        /**
         * @brief    
         * @param[i] index  索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotComponentChange(int index);
        /**
         * @brief    点、线、面显示修改事件
         * @param[i] index  索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void slotDisplayChange(int index);
    private:
        /**
         * @brief    更新是否显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void updateShowColorMapp();
        void updateDisplayComboBox();
        void updateVariable();
        void updateComponent(); 
    private:
        QAction* _actionShowColorMap = nullptr;
        QAction* _actionEditColorMap = nullptr;
        QAction* _actionColorLibEdit = nullptr;
        GUIComboBox* _variableCombox = nullptr;
        GUIComboBox* _componentCombox = nullptr;
        GUIComboBox* _disPlayCombox = nullptr;
    };
}

#endif