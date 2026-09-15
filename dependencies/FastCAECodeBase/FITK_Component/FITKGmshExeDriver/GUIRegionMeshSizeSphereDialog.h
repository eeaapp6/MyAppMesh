/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIRegionMeshSizeSphereDialog.h
 * @brief  球体区域网格
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-17
 *********************************************************************/
#ifndef GUIREGIONMESHSIZESPHEREDIALOG_H
#define GUIREGIONMESHSIZESPHEREDIALOG_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKGmshExeDriverAPI.h"

namespace Ui { class GUIRegionMeshSizeSphereDialog; }

namespace GUI {
    /**
     * @brief  创建球体区域网格
     * @author guqingtao (15598887859@163.com)
     * @date   2025-06-07
     */
    class FITKGmshExeDriverAPI GUIRegionMeshSizeSphereDialog : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-16
         */
        GUIRegionMeshSizeSphereDialog(QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        ~GUIRegionMeshSizeSphereDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void init();
        /**
         * @brief    处理拾取到的数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void pickFinishedOper(int pickType, int objectId, QHash<QString, void*> objInfo);

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-16
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    关闭按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取点
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-16
         */
        void on_point1_pick_clicked();
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-16
         */
        Ui::GUIRegionMeshSizeSphereDialog *_ui{};
    };
}
#endif 
