/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRegionMeshInfoDialog.h
 * @brief  区域网格信息
 * @author guqingtao (15598887859@163.com)
 * @date   2025-07-04
 *********************************************************************/
#ifndef GUIREGIONMESHINFODIALOG_H
#define GUIREGIONMESHINFODIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKGmshExeDriverAPI.h"

namespace Ui { class GUIRegionMeshInfoDialog; }

namespace GUI {
    /**
     * @brief  创建长方体区域网格
     * @author guqingtao (15598887859@163.com)
     * @date   2025-07-04
     */
    class FITKGmshExeDriverAPI GUIRegionMeshInfoDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-04
         */
        GUIRegionMeshInfoDialog(QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-04
         */
        ~GUIRegionMeshInfoDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-04
         */
        void init();

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-04
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    关闭按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-04
         */
        void on_pushButton_Cancel_clicked();

    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-07-04
         */
        Ui::GUIRegionMeshInfoDialog *_ui{};
    };
}
#endif 
