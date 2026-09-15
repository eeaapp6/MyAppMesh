/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIGeometryCylinderDialog.h
 * @brief  圆柱体
 * @author guqingtao (15598887859@163.com)
 * @date   2025-09-16
 *********************************************************************/
#ifndef GUIGeometryCylinderDialog_H
#define GUIGeometryCylinderDialog_H

#include "GUIGeometryDialogAPI.h"
#include "PickDataProvider/GUIPickInfo.h"
#include "GUIWidget/GUIDialogBase.h"

namespace Ui { class GUIGeometryCylinderDialog; }
namespace Interface {
    class FITKAbsGeoPart;
}

namespace Radioss
{
    class FITKRadiossGeomPart;
}

namespace Core
{
    class FITKActionOperator;
}

namespace GUI {
    /**
     * @brief  创建圆柱
     * @author guqingtao (15598887859@163.com)
     * @date   2025-09-16
     */
    class GUIGeometryDialogAPI GUIGeometryCylinderDialog : public GUIDialogBase
    {
        Q_OBJECT

    public:

        GUIGeometryCylinderDialog(Core::FITKActionOperator* oper,QWidget* parent = nullptr);
        ~GUIGeometryCylinderDialog();
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void init();
        /**
         * @brief    处理拾取到的数据
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void pickFinishedOper();
        /**
         * @brief    设置拾取方式
         * @param[i] objType
         * @param[i] methodType
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType);

    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-16
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    取消按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取点的按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-16
         */
        void on_point1_pick_clicked();

    private:
        /**
         * @brief    将创建圆柱的命令写入Python脚本
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void writePythonScript();

    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-15
         */
        Ui::GUIGeometryCylinderDialog *_ui{};
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-15
         */
        Core::FITKActionOperator* _oper{};
        /**
         * @brief  几何数据管理器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-29
         */
        Interface::FITKAbsGeoPart* _geometryData{};
        Radioss::FITKRadiossGeomPart* _modelData{};
    };
}
#endif