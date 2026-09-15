/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRBodyDialog.h
 * @brief  刚体连接界面
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-06
 *********************************************************************/
#ifndef _GUIRBODYDIALOG_H
#define _GUIRBODYDIALOG_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include <QTableWidgetItem>

namespace Ui
{
    class GUIRBodyDialog;
}
namespace Radioss
{
    class FITKConnectionRBODY;
    class FITKConnectionManager;
}
namespace GUI
{
    /**
     * @brief  状态方程基类声明
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-10-30
     */
    class GUIAnalysisDialogAPI GUIRBodyDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief     create connect object dialog
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-05-23
         */
        explicit GUIRBodyDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief     edit connect object dialog
         * @author    liuzhonghua (liuzhonghuaszch@163.com)
         * @date      2025-05-23
         */
        explicit GUIRBodyDialog(Radioss::FITKConnectionRBODY* obj, Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-23
         */
        virtual ~GUIRBodyDialog();
        /**
         * @brief    数据初始化
         * @return   void
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-23
         */
        virtual void init();

    private slots:
        /**
         * @brief   OK按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        void on_pushButtonOK_clicked();
        /**
         * @brief   取消按钮点击事件
         * @author  lilongyuan(lilongyuan@diso.cn)
         * @date    2025-09-02
         */
        void on_pushButtonCancel_clicked();
        /**
         * @brief    拾取主节点触发按键
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-23
         */
        void on_pushButton_MainNodeID_clicked();
        /**
         * @brief    自动按键状态变化
         * @param[i] state 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void on_checkBox_Auto_stateChanged(int state);

    private slots:
        /**
         * @brief  拾取数据槽
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-12-23
         */
        void pickData_MainNode_Finished();

    private:
        /**
         * @brief    初始化界面
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void initWidget();
        /**
         * @brief    初始化传感器下拉框
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void initSensorPropComboBox();
        /**
         * @brief    初始化局部坐标系下拉框
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void initSkewComboBox();
        /**
         * @brief    创建数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        bool createObjData();
        /**
         * @brief    边界数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        bool editObjData();
        /**
         * @brief    从界面获取数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        void getDataFormWidget();
        /**
         * @brief    写入Python脚本
         * @param[in] oldName 修改前名称
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-25
         */
        void writePythonScript(const QString& oldName = QString());
        /**
         * @brief    获取节点集合的中心点ID,没有就会创建一个中心点
         * @param[i] setNodeID 
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        int getCenterNodeIDBySetNodeID(int setNodeID);

    protected:
        /**
         * @brief  ui
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-23
         */
        Ui::GUIRBodyDialog* _ui = nullptr;
        /**
        * @brief  连接管理器
        * @author liuzhonghua (liuzhonghuaszch@163.com)
        * @date   2025-05-23
        */
        Radioss::FITKConnectionManager* _mgr = nullptr;
        /**
         * @brief  数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-23
         */
        Radioss::FITKConnectionRBODY* _obj = nullptr;
        /**
         * @brief  操作器对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-23
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否是创建
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-05-23
         */
        bool _isCreate = false;
    };
}

#endif
