/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIMaterialPropDialog.h
 * @brief  材料窗口基类声明
 * @author wangning (2185896382@qq.com)
 * @date   2025-05-23
 *********************************************************************/
#ifndef _GUIMaterialPropDialog_H
#define _GUIMaterialPropDialog_H

#include "GUIAnalysisDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include <QString>

class QComboBox;
class QTreeWidgetItem;

namespace Ui
{
    class GUIMaterialPropDialog;
}
namespace Interface
{
    class FITKAbstractMaterial;
    class FITKMaterialManager;
}
namespace GUI
{
    /**
     * @brief  材料窗口基类声明
     * @author wangning (2185896382@qq.com)
     * @date   2025-05-23
     */
    class GUIAnalysisDialogAPI GUIMaterialPropDialog : public Core::FITKDialog
    {
        Q_OBJECT;
    public:
        /**
         * @brief load object dialog
         * @param[i]  obj            My Param doc
         * @param[i]  oper           operator
         * @param[i]  parent
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        explicit GUIMaterialPropDialog(Interface::FITKAbstractMaterial* obj, Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief Destroy object
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual ~GUIMaterialPropDialog();
        /**
         * @brief    数据初始化
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual void init();

    protected:
        /**
         * @brief    创建数据对象
         * @return   bool
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool createObjData();
        /**
         * @brief    编辑数据对象
         * @return   boiol
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool editObjData();
        /**
         * @brief    检查
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual bool ckeckData();
        /**
        * @brief   初始化失效下拉框
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        void initFailComboBox();
        /**
        * @brief   添加需要QLineEdit的子Item
        * @param[i]  p     父节点
        * @param[i]  objName    QLineEdit的objName
        * @param[i]  label   新Item的名字
        * @param[i]  unit   数据的单位
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        QTreeWidgetItem* addChiledItem(QTreeWidgetItem* p, const QString& objName, const QString& label, const QString& unit="");
        /**
         * @brief    从界面获取数据
         * @return   void
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        virtual void getDataFormWidget() = 0;
        /**
         * @brief    写入Python脚本
         * @return   void
         * @author   wangning (2185896382@qq.com)
         * @date     2026-05-15
         */
        virtual void writePythonScript();

    protected slots:
        /**
         * @brief    取消事件重写
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        void reject() override;
        /**
        * @brief OK按钮信号
        * @return
        */
        void on_pushButtonOK_clicked();
        /**
        * @brief cancle按钮信号
        * @return
          */
        void on_pushButtonCancel_clicked();

    protected:
        /**
        * @brief 材料管理器
        * @author wangning (2185896382@qq.com)
        * @date   2025-05-23
        */
        Interface::FITKMaterialManager* _matMgr = nullptr;
        /**
         * @brief 材料数据对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Interface::FITKAbstractMaterial* _obj = nullptr;
        /**
         * @brief 操作器对象
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  是否是创建
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        bool _isCreate = false;
        /**
         * @brief  写脚本时使用的编辑前名称
         * @author wangning (2185896382@qq.com)
         * @date   2026-05-22
         */
        QString _oldNameForScript{};
    protected:
        /**
         * @brief ui 
         * @author wangning (2185896382@qq.com)
         * @date   2025-05-23
         */
        Ui::GUIMaterialPropDialog* _ui = nullptr;
    };
}

#endif
