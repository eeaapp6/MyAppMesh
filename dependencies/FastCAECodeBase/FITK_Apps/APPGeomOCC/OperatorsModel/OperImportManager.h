/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef OPERATORSIMPORTMANAGER_H
#define OPERATORSIMPORTMANAGER_H

#include "OperatorsModelAPI.h"
#include "OperManagerBase.h"

namespace ModelOper
{
    class OperatorsModelAPI OperImportManager : public OperManagerBase
    {
        Q_OBJECT
    public:
        /**
         * @brief 构造函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-19
         */
        explicit OperImportManager() = default;
        /**
         * @brief 析构函数
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date 2024-04-19
         */
        ~OperImportManager() override = default;

    private:
        /**
         * @brief 界面逻辑，生成UI交互
         * @return 执行成功返回true，否则返回false
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-15
         */
        bool execGUI() override;
        /**
         * @brief 业务处理逻辑，在execGUI后执行
         * @return 执行成功返回true，否则返回false
         * @author yanzhihui (chanyuantiandao@126.com)
         * @date 2023-08-15
         */
        bool execProfession() override;
    };

    // 按钮注册相关操作
    Register2FITKOperatorRepo(OperImportManager, OperImportManager);
    Register2FITKOperatorRepo(actionImportCreate, OperImportManager);
    Register2FITKOperatorRepo(actionImportEdit, OperImportManager);
    Register2FITKOperatorRepo(actionImportDelete, OperImportManager);
    Register2FITKOperatorRepo(actionImportShow, OperImportManager);
    Register2FITKOperatorRepo(actionImportHide, OperImportManager);
} // namespace ModelOper
#endif // OPERATORSIMPORTMANAGER_H
