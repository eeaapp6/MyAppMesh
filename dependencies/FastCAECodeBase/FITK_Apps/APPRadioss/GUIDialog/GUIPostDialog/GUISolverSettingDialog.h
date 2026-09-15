/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUISolverSettingDialog.h
 * @brief  求解器设置窗口类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-05
 *********************************************************************/
#ifndef _GUISolverSettingDialog_H
#define _GUISolverSettingDialog_H

#include "GUIPostDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "GUIRadiossResultRequestDialog.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"

namespace Core
{
    class FITKActionOperator;
}

namespace Radioss
{
    class FITKRadiossSolverSettings;
}

namespace Ui {
class GUISolverSettingDialog;
}

class QTreeWidgetItem;

namespace GUI
{

    /**
     * @brief  求解器设置窗口类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-12-31
     */
    class GUIPostDialogAPI GUISolverSettingDialog : public GUIDialogBase
    {
        Q_OBJECT
    public:
        /**
         * @brief  create object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-12-31
         */
        GUISolverSettingDialog(Core::FITKActionOperator* oper, QWidget *parent = nullptr);
        /**
         * @brief    Destroy object
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        virtual ~GUISolverSettingDialog();

    protected slots:
        /**
         * @brief cencel button clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief OK button clicked slot
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        void on_pushButton_OK_clicked();

    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-31
         */
        void init();
        /**
         * @brief    初始化运行时时间参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initRunExeTimeParamTreeNode();
        /**
         * @brief    初始化全局时间参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initGlobalTimeParamTreeNode();
        /**
         * @brief    初始化节点时间步长控制参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initNodalTimeStepControlParamTreeNode();
        /**
         * @brief    初始化单元时间步长控制参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initElementalTimeStepControlParamTreeNode();
        /**
         * @brief    初始化自适应动态松弛树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initAdaptiveDynamicRelaxationTreeNode();
        /**
         * @brief    初始化AMS单元时间步长控制参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initAMSElementalTimeStepControlParamTreeNode();
        /**
         * @brief    初始化计算控制参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initComputationControlParamTreeNode();
        /**
         * @brief    初始化收敛标准参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        void initConvergenceCriteriaParamTreeNode();
        /**
         * @brief    初始化全局SPH参数树节点
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-31
         */
        void initSPHGlobalParamTreeNode();
        /**
         * @brief    添加根节点
         * @param[i] text 
         * @return   QTreeWidgetItem *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        QTreeWidgetItem* addTopLevelItem(const QString& text);
        /**
         * @brief    添加子节点
         * @param[i] p 
         * @param[i] childItemData 
         * @return   QTreeWidgetItem *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        QTreeWidgetItem* addChildItem(QTreeWidgetItem* p, const ChildTreeNodeData& childItemData);
        /**
         * @brief    获取CheckBox状态
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        bool getCheckBoxState(const QString& objectName);
        /**
         * @brief    获取LineEdit内容
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        double getLineEditValue(const QString& objectName);
        /**
         * @brief    获取ComboBox内容
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        int getTimeStepControlType(const QString& objectName);
        /**
         * @brief    设置运行时时间参
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setRunExeTimeParam();
        /**
         * @brief    设置全局时间参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setGlobalTimeParam();
        /**
         * @brief    设置节点时间步长控制参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setNodalTimeStepControlParam();
        /**
         * @brief    设置单元时间步长控制参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setElementalTimeStepControlParam();
        /**
         * @brief    设置自适应动态松弛
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setAdaptiveDynamicRelaxation();
        /**
         * @brief    设置AMS单元时间步长控制参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setAMSElementalTimeStepControlParam();
        /**
         * @brief    设置计算控制参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setComputationControlParam();
        /**
         * @brief    设置收敛标准参数
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-09
         */
        void setConvergenceCriteriaParam();
        /**
         * @brief    设置全局SPH参数
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-31
         */
        void setSPHGlobalParam();
        /**
         * @brief    将求解器设置的命令写入Python脚本
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2026-05-15
         */
        void writePythonScript();
    protected:
        /**
         * @brief ui object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        Ui::GUISolverSettingDialog* _ui = nullptr;
        /**
         * @brief 操作器对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-23
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  求解设置数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-08
         */
        Radioss::FITKRadiossSolverSettings* _solverSettings{ nullptr };
    };
}


#endif // _GUISolverDialog_H
