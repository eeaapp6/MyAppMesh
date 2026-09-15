/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIPropertyBeamSPR_DOF_Dialog.h
 * @brief  BeamSPR不同维度值设置窗口
 * @author wangning (2185896382@qq.com)
 * @date   2025-11-04
 *********************************************************************/
#ifndef _GUIPropertyBeamSPR_DOF_Dialog_H
#define _GUIPropertyBeamSPR_DOF_Dialog_H

#include "GUIPropertyDialog.h"
#include "FITK_Component/FITKRadiossData/FITKPropBeamSPR.h"

namespace GUI
{
    /**
     * @brief  BeamSPR不同维度值设置窗口
     * @author wangning (2185896382@qq.com)
     * @date   2025-08-26
     */
    class GUIAnalysisDialogAPI GUIPropertyBeamSPR_DOF_Dialog : public GUIPropertyDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief 构造函数
         * @param[i]  obj          属性对象
         * @param[i]  oper           operator
         * @param[i]  parent
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        explicit GUIPropertyBeamSPR_DOF_Dialog(Radioss::ValueDOF& obj, QWidget* parent = nullptr);
        virtual ~GUIPropertyBeamSPR_DOF_Dialog() = default;

    protected:
        /**
        * @brief    UI初始化
        * @return   void
        * @author wangning (2185896382@qq.com)
        * @date   2025-08-26
        */
        void initUI();
        /**
        * @brief    数据初始化
        * @return   void
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        void init() override;
        /**
         * @brief    检查
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool ckeckData() override;
        /**
         * @brief    从界面获取数据
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        void getDataFormWidget() override;
        /**
         * @brief 添加ComboBox
         * @param[i]  label          行标题
         * @param[i]  objname        QComboBox对象名称
         * @param[i]  isCurve        是否是曲线选择框
         * @author wangning (2185896382@qq.com)
         * @date   2025-08-26
         */
        QComboBox* addComboBox(const QString& label, const QString& objname,bool isCurve);

    protected slots:
        /**
        * @brief OK按钮信号
        * @return
        */
        virtual void on_pushButtonOK_clicked()override;
        /**
        * @brief Cancel按钮信号
        * @return
        */
        virtual void on_pushButtonCancel_clicked()override;

    private:
        Radioss::ValueDOF& _objDof;
    };
}

#endif

