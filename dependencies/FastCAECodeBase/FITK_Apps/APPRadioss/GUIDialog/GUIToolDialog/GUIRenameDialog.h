/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRenameDialog.h
 * @brief  重命名窗口类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-08-22
 *********************************************************************/
#ifndef _GUIRENAMEDIALOG_H__
#define _GUIRENAMEDIALOG_H__

#include "GUIToolDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"

namespace Core {
    class FITKActionOperator;
    class FITKAbstractNDataObject;
    class FITKAbstractDataManagerPrivate;
}
namespace Ui {
    class GUIRenameDialog;
}
namespace GUI
{
    /**
     * @brief  重命名窗口类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-08-22
     */
    class GUIToolDialogAPI GUIRenameDialog : public GUIDialogBase
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        GUIRenameDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        GUIRenameDialog(Core::FITKActionOperator* oper, Core::FITKAbstractNDataObject* obj, Core::FITKAbstractDataManagerPrivate* manager = nullptr, QWidget* parent = nullptr);
        /**
         * @brief  析构函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        ~GUIRenameDialog();
        /**
         * @brief    设置数据对象
         * @param[i] obj 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        void setObjData(Core::FITKAbstractNDataObject* obj);
        /**
         * @brief    设置数据管理器-用于判断是否设置了重复名称
         * @param[i] manager 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        void setObjManager(Core::FITKAbstractDataManagerPrivate* manager);
        /**
         * @brief    初始化
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        void init();

    private slots:
        /**
         * @brief  确定
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    取消
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-08-22
         */
        void on_pushButton_Cancel_clicked();

    private:
        /**
         * @brief  ui指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        Ui::GUIRenameDialog* _ui = nullptr;
        /**
         * @brief  操作器指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  需重命名的数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        Core::FITKAbstractNDataObject* _obj = nullptr;
        /**
         * @brief  数据管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-08-22
         */
        Core::FITKAbstractDataManagerPrivate* _objManager = nullptr;
    };
}
#endif // !_GUIPARTPICKDIALOG_H__
