/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file    OperatorProperty.h
 * @brief   属性操作器
 * @author  lilongyuan (lilongyuan@diso.cn)
 * @date    2025-07-17
 *********************************************************************/
#ifndef __OPERATOR_PROPERTY_H__
#define __OPERATOR_PROPERTY_H__

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

class QTableWidget;

namespace Radioss
{
    class FITKPropAbstract;
    class FITKPropSolid;
}

namespace Interface
{
    class FITKAbstractSectionManager;
}

namespace OperModel
{
    /**
     * @brief   属性操作器类
     * @author  lilongyuan (lilongyuan@diso.cn)
     * @date    2025-07-17
     */
    class OperatorProperty :public Core::FITKActionOperator
    {
    public:
        /**
         * @brief   构造函数
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        explicit OperatorProperty() = default;
        /**
         * @brief   析构函数
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        virtual ~OperatorProperty() = default;
        /**
         * @brief   GUI执行
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        virtual bool execGUI();
        /**
         * @brief   业务逻辑执行
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-17
         */
        virtual bool execProfession();
        /**
         * @brief   获取选中的属性
         * @return  Radioss::FITKPropAbstract* 选中的属性
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-08-22
         */
        Radioss::FITKPropAbstract* getSelectedProperty();
        /**
         * @brief   获取属性管理器
         * @return  Interface::FITKAbstractSectionManager* 属性管理器
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-21
         */
        Interface::FITKAbstractSectionManager* getPropertyManager();
        /**
         * @brief   删除属性
         * @return  bool 删除结果
         * @author  ZhouZhouYi (zhouzhaoyi@diso.cn)
         * @date    2025-08-01
         */
        bool deleteProperty();

    protected:
        /**
         * @brief   参数预处理
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-22
         */
        virtual void preArgs();

    private:
        /**
         * @brief   数据加载
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-22
         */
        void dataLoading();
        /*
         * @brief   数据处理
         * @return  bool 执行结果
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-21
         */
        bool dataProcessing();
        /**
         * @brief     获取当前下拉框列表索引值
         * @param[i]  row 行索引
         * @param[i]  column 列索引
         * @return    int 索引
         * @author    lilongyuan (lilongyuan@diso.cn)
         * @date      2025-07-22
         */
        int getCurrentIndex(int row, int column);
        /**
         * @brief    重命名
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        bool propertyRenameOper();
        /**
         * @brief     获取当前格双精度数据
         * @param[i]  row 行索引
         * @param[i]  column 列索引
         * @return    double 双精度数据
         * @author    lilongyuan (lilongyuan@diso.cn)
         * @date      2025-07-22
         */
        double getDoubleValue(int row, int column);
        /**
         * @brief     获取当前选择框勾选结果
         * @param[i]  row 行索引
         * @param[i]  column 列索引
         * @return    bool 勾选结果
         * @author    lilongyuan (lilongyuan@diso.cn)
         * @date      2025-07-22
         */
        bool getIsChecked(int row, int column);
        /**
         * @brief   更新界面树形菜单
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-08-22
         */
        void updateTree();
    private:
        /**
         * @brief   表格控件
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-22
         */
        QTableWidget* _tableWidget{};
        /**
         * @brief   存储待修改数据指针
         * @author  lilongyuan (lilongyuan@diso.cn)
         * @date    2025-07-22
         */
        Radioss::FITKPropAbstract* _data{};
    };
    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionPropertyCreate, OperatorProperty);
    Register2FITKOperatorRepo(actionPropertyRename, OperatorProperty);
    Register2FITKOperatorRepo(actionPropertyEdit, OperatorProperty);
    Register2FITKOperatorRepo(actionPropertyDelete, OperatorProperty);
}
#endif  // __OPERATOR_PROPERTY_H__