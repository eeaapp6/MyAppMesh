/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   GUIDialogBase.h
 * @brief  对话框基类.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-08-26
 */
#ifndef _GUIDialogBase_H
#define _GUIDialogBase_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QWidget>

#include "GUIWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/GraphToolOperator.h"
#include "OperatorsInterface/GraphInteractionOperator.h"

namespace GUI
{
    class MainWindow;
    /**
     * @brief  对话框基类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-08-26
     */
    class GUIWIDGETAPI GUIDialogBase : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief  构造函数.
         * @param  parent 父对象
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        GUIDialogBase(QWidget* parent = nullptr);
        /**
         * @brief  析构函数.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        virtual ~GUIDialogBase();
        /**
         * @brief  设置操作器，用于回调.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        void setOperator(EventOper::ParaWidgetInterfaceOper* oper) { m_Operator = oper; };
        /**
         * @brief  调用操作器的execProfession方法.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        bool execProfession();

    protected:
        /**
         * @brief   重写窗口显示事件。[虚函数][重写]
         * @param   event：显示事件
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        virtual void showEvent(QShowEvent *event) override;

        /**
         * @brief  重写窗口关闭事件.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        virtual void closeEvent(QCloseEvent *e) override;
        /**
         * @brief  重写隐藏.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-27
         */
        virtual void hideEvent(QHideEvent *event) override;
        /**
         * @brief  清理窗口资源（包括信号和数据）.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-23
         */
        virtual void finalize();

        /**
         * @brief   重写窗口确定事件。[虚函数][重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        virtual void accept() override;

        /**
         * @brief   重写窗口取消事件。[虚函数][重写]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-23
         */
        virtual void reject() override;

        /**
         * @brief   输出日志到控制台。
         * @param   type：消息类型 1-normal 2-warning 3-error 4-info
         * @param   msg：消息文本
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        void printLog(int type, QString msg);

        /**
         * @brief   刷新可视化对象。
         * @param   id：命令ID
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        void updateGraph(int id);

        /**
         * @brief   预览可视化对象。
         * @param   id：命令ID
         * @param   type：预览类型（默认显示整个模型）[缺省]
         * @param   color：预览对象颜色[缺省]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        void previewGraph(int id, EventOper::PreviewType type = EventOper::PreviewType::PT_Default, QColor color = QColor());

        /**
         * @brief   清除所有预览对象对象。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-09
         */
        void clearPreview();

    protected:
        /**
         * @brief  主窗口.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        MainWindow* m_MainWindow{};

        /**
         * @brief   几何命令列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        Interface::FITKGeoCommandList* m_CmdList{ nullptr };

        /**
         * @brief   基准元素命令列表。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        Interface::FITKDatumList* m_DatumList{ nullptr };

        /**
         * @brief  操作器.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-08-26
         */
        EventOper::ParaWidgetInterfaceOper* m_Operator{};

        /**
         * @brief   前处理可视化操作器。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        EventOper::GraphEventOperator* m_GraphOper{};

        /**
         * @brief   前处理可视化工具操作器。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        EventOper::GraphToolOperator* m_GraphToolOper{};

        /**
         * @brief   前处理拾取功能操作器。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-18
         */
        EventOper::GraphInteractionOperator* m_GraphPickOper{};

        /**
         * @brief   被操作的数据ID。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        int m_dataId = -1;

    };

}

#endif