/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperFieldOutput.h
 * @brief  后处理场输出变量控制器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef _OPERFIELDOUTPUT_H__
#define _OPERFIELDOUTPUT_H__

#include <QToolBar>
#include "FITK_Kernel/FITKCore/FITKAbstractOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/ComboboxEventOperator.h"

namespace GUIOper
{
    /**
     * @brief  后处理场输出变量控制器类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-16
     */
    class OperFieldOutput : public EventOper::ComboboxEventOperator
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        explicit OperFieldOutput() = default;
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        ~OperFieldOutput() = default;
        /**
         * @brief  下拉列表发生变化事件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        virtual bool comboboxChanged() override;
        /**
         * @brief  设置父级部件，同时关联信号
         * @param  w 父级部件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void setParentWidget(QWidget* w) override;

    protected:
        /**
         * @brief  执行变量位置下拉框
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void execPostVariableLocation_Combox();
        /**
         * @brief  执行场变量下拉框
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void execPostFieldVariable_Combox();
        /**
         * @brief    执行场分量下拉框
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-17
         */
        void execPostFieldWeight_Combox();
        /**
         * @brief  根据 变量位置 下拉框的值刷新 场变量 下拉框
         * @param  firstComboxVal 第一个下拉框的值
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void updatePostFieldVariable_Combox(QString firstComboxVal);
        /**
         * @brief  根据 场变量 下拉框的值刷新 场分量 下拉框
         * @param  secondComboxVal 第二个下拉框的值
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void updatePostFieldWeight_Combox(QString secondComboxVal);
        /**
         * @brief   根据界面所选模块、模型、部件或装配更新可视化对象。
         * @author  liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void resetViewport();
    private slots:
        /**
         * @brief  下拉列表发生变化事件
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-16
         */
        void onComboboxChanged(int);
    };

    Register2FITKOperatorRepo(OperFieldOutput, OperFieldOutput);
}

#endif // _OPERFIELDOUTPUT_H__