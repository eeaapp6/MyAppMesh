/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   OperComponentManager.h
 * @brief  分组管理
 * @author YanZhiHui(chanyuantiandao@126.com)
 * @date   2024-10-29
 */
#ifndef OPERATORSPOINTMANAGER_H
#define OPERATORSPOINTMANAGER_H

#include "OperCreateGeoManagerBase.h"

#include "OperatorsModelAPI.h"

namespace ModelOper
{
    /**
     * @brief   几何创建相关操作器基类。
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-29
     */
    class OperatorsModelAPI OperComponentManager : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief   构造函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-29
         */
        OperComponentManager() = default;

        /**
         * @brief   析构函数。
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-29
         */
        virtual ~OperComponentManager() = default;

        /**
         * @brief   执行界面相关功能。[虚函数][重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        virtual bool execGUI() override;

        /**
         * @brief   执行业务逻辑。[虚函数][重写]
         * @return  是否执行成功
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-26
         */
        virtual bool execProfession() override;
        /**
         * @brief   显示模型参数窗口。[纯虚函数 TODO]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-29
         */
        virtual void showDialog();
    };

    Register2FITKOperatorRepo(actionComponent, OperComponentManager);
    Register2FITKOperatorRepo(actionComponentCreate, OperComponentManager);
    Register2FITKOperatorRepo(actionComponentEdit, OperComponentManager);
    Register2FITKOperatorRepo(actionComponentDelete, OperComponentManager);
    Register2FITKOperatorRepo(actionComponentHide, OperComponentManager);
    Register2FITKOperatorRepo(actionComponentShow, OperComponentManager);
    Register2FITKOperatorRepo(actionComponentRename, OperComponentManager);

} // namespace ModelOper
#endif // !OPERATORSPOINTMANAGER_H
