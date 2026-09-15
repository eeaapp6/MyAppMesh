/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file OperManagerBase.h
 * @brief 操作器基类
 */
#ifndef __OPERMANAGERBASE_H__
#define __OPERMANAGERBASE_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "GUIFrame/MainWindow.h"
#include "OperatorsModelAPI.h"
#include <QStringList>

namespace ModelOper
{
    /**
     * @brief 操作器基类
     */
    class OperatorsModelAPI OperManagerBase : public EventOper::ParaWidgetInterfaceOper
    {
        Q_OBJECT
    public:
        OperManagerBase();
        virtual ~OperManagerBase();

    protected:
        /**
         * @brief   输出日志到控制台。
         * @param   type：消息类型 1-normal 2-warning 3-error 4-info
         * @param   msg：消息文本
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-27
         */
        void printLog(int type, QString msg);

        /**
         * @brief   创建移除命令并移除指定建模命令。[虚函数]
         * @param   id：命令ID
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-07
         */
        virtual void deleteCmd(int id);

        /**
         * @brief   退出草图模式：取消草图按钮按下状态，解除拾取回调。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-25
         */
        void exitSketchMode();

    protected:
        /**
         * @brief   更新模型树。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-22
         */
        void updateTree();

        /**
         * @brief 参数预处理
         */
        void preArgs() override;
        /**
         * @brief  获取操作类型
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2024-03-29
         */
        void preEmitter();
        /**
         * @brief 按钮类型对象的参数获取
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2024-03-29
         */
        void preItemArgs();
        /**
         * @brief 操作类型
         */
        enum OperType
        {
            None,       ///< 未定义
            Create,     ///< 创建操作
            Edit,       ///< 编辑操作
            Copy,       ///< 拷贝操作
            Delete,     ///< 删除操作
            Rename,     ///< 重命名操作
            Select,     ///< 选择操作
            Hide,
            Show
        } m_OperType{ None };
        /**
         * @brief 主界面
         */
        GUI::MainWindow *m_MainWindow{};
        /**
         * @brief  操作对象的ID
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-03-28
         */
        int m_Id{ -1 };
        /**
         * @brief  附加信息
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-05-21
         */
        QStringList m_AddtionalInfo{};
    };
} // namespace ModelOper
#endif //!__OPERMANAGERBASE_H__
