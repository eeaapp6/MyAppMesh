/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ClipSliceWidgetCylinder.h
 * @brief  圆柱面切割切片界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-08
 *********************************************************************/
#ifndef _ClipSliceWidgetCylinder_H
#define _ClipSliceWidgetCylinder_H

#include "ClipSliceWidgetBase.h"

namespace Ui{
    class ClipSliceWidgetCylinder;
}

namespace Interface {
    class FITKAbstractCFDPostData;
    class PostGraphWidgetCylinder;
}

namespace GUI
{
    /**
     * @brief  圆柱面切割切片界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-08
     */
    class ClipSliceWidgetCylinder :public ClipSliceWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent 父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        ClipSliceWidgetCylinder(ClipSlice widgetType, int currentDataID, int parentDataID, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        virtual ~ClipSliceWidgetCylinder();
        /**
         * @brief    将界面数据填充至数据对象中
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        virtual void getDataFromWidget(Interface::FITKAbstractCFDPostData* postData) override;
        /**
         * @brief    将数据对象中的数据填充至界面
         * @param[i] glyphData     数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        virtual void setDataToWidget(Interface::FITKAbstractCFDPostData* postParentData, Interface::FITKAbstractCFDPostData* postData) override;
    protected:
        /**
         * @brief    显示事件
         * @param[i] event      事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void showEvent(QShowEvent* event);
        /**
         * @brief    关闭事件
         * @param[i] event      事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-19
         */
        void closeEvent(QCloseEvent* event);
        /**
         * @brief    隐藏事件
         * @param[i] event     事件
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        void hideEvent(QHideEvent *event);
    private slots:
        ;
        /**
         * @brief    vtk圆柱界面数据修改事件
         * @param[i] center         中心点[3]
         * @param[i] axis           方向[3]
         * @param[i] radius         半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-11
         */
        void slotVTKWidgetValueChange(double* center, double* axis, double radius);
        /**
         * @brief    界面数据修改槽函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void slotDataChange();
    private:
        /**
         * @brief  vtk裁切界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-18
         */
        Interface::PostGraphWidgetCylinder* _cylinderWidget = nullptr;
        /**
         * @brief  ui界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-08
         */
        Ui::ClipSliceWidgetCylinder* _ui = nullptr;
        /**
         * @brief  父对象ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-08
         */
        int _parentDataID = -1;
    };
}

#endif
