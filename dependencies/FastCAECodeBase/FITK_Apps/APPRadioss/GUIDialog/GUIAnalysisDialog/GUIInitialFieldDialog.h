/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    GUIInitialFieldDialog.h
 * @brief   初始化场对话框基类
 * @author  lilongyuan(lilongyuan@diso.cn)
 * @date    2025-09-03
 *********************************************************************/
#ifndef __GUI_INITIAL_FIELF_DIALOG_H__
#define __GUI_INITIAL_FIELF_DIALOG_H__

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"

namespace Ui
{
    class GUIInitialFieldDialog;
}

namespace GUI
{
    /**
     * @brief   初始化场对话框基类
     * @author  lilongyuan(lilongyuan@diso.cn)
     * @date    2025-09-03
     */
    class GUIAnalysisDialogAPI GUIInitialFieldDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief     构造函数
         * @param[i]  parent 父窗口
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        explicit GUIInitialFieldDialog(QWidget* parent = nullptr);
        /**
         * @brief   析构函数
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        ~GUIInitialFieldDialog();
    protected:
        /**
         * @brief     设置模式
         * @param[i]  model 模式，0-创建，1-修改
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setModel(int model);
        /**
         * @brief     设置名称
         * @param[i]  text 名称
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setInitialFieldName(const QString& text);
        /**
         * @brief   获取名称
         * @return  QString 名称
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        QString getInitialFieldName();
        /**
         * @brief     设置类型
         * @param[i]  text 类型
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setInitialFieldType(const QString& text);
        /**
         * @brief     设置关键字
         * @param[i]  text 关键字
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setInitialFieldKeyWord(const QString& text);
        /**
         * @brief     设置X轴初速度
         * @param[i]  text X轴初速度
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setInitialFieldSetID(int id);
        /**
         * @brief   获取X轴初速度
         * @return  QString X轴初速度
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        int getInitialFieldSetID();
        /**
         * @brief   获取集合名称
         * @return  QString 集合名称
         * @author  wangning (2185896382@qq.com)
         * @date    2026-05-14
         */
        QString getInitialFieldSetName();
        /**
         * @brief     设置X轴初速度
         * @param[i]  text X轴初速度
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setInitialFieldX(const QString& text);
        /**
         * @brief   获取X轴初速度
         * @return  QString X轴初速度
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        QString getInitialFieldX();
        /**
         * @brief     设置Y轴初速度
         * @param[i]  text Y轴初速度
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setInitialFieldY(const QString& text);
        /**
         * @brief   获取Y轴初速度
         * @return  QString Y轴初速度
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        QString getInitialFieldY();
        /**
         * @brief     设置Z轴初速度
         * @param[i]  text Z轴初速度
         * @author    lilongyuan(lilongyuan@diso.cn)
         * @date      2025-09-03
         */
        void setInitialFieldZ(const QString& text);
        /**
         * @brief   获取Z轴初速度
         * @return  QString Z轴初速度
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        QString getInitialFieldZ();
        /**
         * @brief   创建初始化场对象（纯虚函数，子类需要重写）
         * @return  bool 创建结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        virtual bool createInitialField() = 0;
        /**
         * @brief   修改初始化场对象（纯虚函数，子类需要重写）
         * @return  bool 修改结果
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        virtual bool modifyInitialField() = 0;
    private slots:
        /**
         * @brief   确认按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        void on_pushButtonOK_clicked();
        /**
         * @brief   取消按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        void on_pushButtonCancel_clicked();
    protected:
        /**
         * @brief   UI指针
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        Ui::GUIInitialFieldDialog* _ui = nullptr;
        /**
         * @brief   模式标记位
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-03
         */
        int _model = -1;
    };
}
#endif  // __GUI_INITIAL_FIELF_DIALOG_H__