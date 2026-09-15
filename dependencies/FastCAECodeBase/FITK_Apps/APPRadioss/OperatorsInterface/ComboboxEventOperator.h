/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ComboboxEventOperator.h
 * @brief  combobox事件处理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef __COMBOBOXEVENTOPERATOR_H__
#define __COMBOBOXEVENTOPERATOR_H__

#include "OperatorsInterfaceAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"

class QWidget;

namespace GUI
{
    class MainWindow;
}

namespace EventOper
{
    /**
     * @brief  ComboBox处理器基类，抽象类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-16
     */
    class OperatorsInterfaceAPI ComboboxEventOperator : public Core::FITKAbstractOperator
    {

    public:
        explicit ComboboxEventOperator();
        virtual ~ComboboxEventOperator() = 0;
        /**
         * @brief 当前项发生变化的执行函数
         * @return true  执行成功
         * @return false  执行失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        virtual bool comboboxChanged();
        /**
         * @brief 设置父窗口，可以基类形式使用，访问内部子控件
         * @param  w                父窗口指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        virtual void setParentWidget(QWidget* w);
        /**
         * @brief 设置当前索引
         * @param  index           当前索引
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void setCurrentIndex(int index);
        /**
         * @brief  设置主窗口
         * @param  mw 主窗口
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        virtual void setMainWindow(GUI::MainWindow* mw);

    protected:
        /**
         * @brief  父级部件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        QWidget* _parentWidget{ };
        /**
         * @brief  当前索引
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        int _currentIndex{ -1 };
        /**
         * @brief  主窗口
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        GUI::MainWindow* _mainWindow{};
    };

}  // namespace EventOper

#endif