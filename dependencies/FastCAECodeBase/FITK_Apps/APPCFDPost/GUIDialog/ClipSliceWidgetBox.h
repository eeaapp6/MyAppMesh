/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ClipSliceWidgetBox.h
 * @brief  包围盒切割切片界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-08
 *********************************************************************/
#ifndef _ClipSliceWidgetBox_H
#define _ClipSliceWidgetBox_H

#include "ClipSliceWidgetBase.h"

namespace Ui{
    class ClipSliceWidgetBox;
}

namespace Interface {
    class FITKAbstractCFDPostData;
    class PostGraphWidgetBox;
}

namespace GUI
{
    /**
     * @brief  包围盒切割切片界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-08
     */
    class ClipSliceWidgetBox :public ClipSliceWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] parent 父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        ClipSliceWidgetBox(ClipSlice widgetType, int currentDataID, int parentDataID, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        virtual ~ClipSliceWidgetBox();
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
         * @brief    VTK界面数据修改事件
         * @param[i] bounds     边界[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void slotVTKWidgetValueChange(double* bounds);
        /**
         * @brief    界面数据修改槽函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-26
         */
        void slotDataChange();
    private:
        /**
         * @brief  ui界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-08
         */
        Ui::ClipSliceWidgetBox* _ui = nullptr;
        /**
         * @brief  父对象ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-08
         */
        int _parentDataID = -1;
        /**
         * @brief  包围盒裁切界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-09
         */
        Interface::PostGraphWidgetBox* _boxWidget = nullptr;
    };
}

#endif
