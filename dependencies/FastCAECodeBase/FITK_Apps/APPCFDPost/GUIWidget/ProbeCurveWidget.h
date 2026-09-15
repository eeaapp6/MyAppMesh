/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   ProbeCurveWidget.h
 * @brief  探针曲线界面
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-24
 *********************************************************************/
#ifndef _ProbeCurveWidget_H
#define _ProbeCurveWidget_H

#include "GUIWidgetBase.h"
#include "GUIWidgetAPI.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostEnum.h"

namespace Ui {
    class ProbeCurveWidget;
}

namespace Plot {
    class FITKPlotWindowsInterface;
    class FITKXYCurveDrawProp;
    class FITKXYPlotWindow;
}

namespace GUI
{
    struct CurveStr
    {
        Plot::FITKXYCurveDrawProp* _curveData = nullptr;
        Interface::FITKPostFieldType _type = Interface::FITKPostFieldType::Post_None;
        //点或者单元id
        int _ID = -1;
        //场量名称
        QString _fileName = "";
        //场量数据(时间，数据)
        QMap<double, double> _value;
    };
    /**
     * @brief  探针曲线界面
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-24
     */
    class GUIWidgetAPI ProbeCurveWidget :public GUIWidgetBase
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] currentObjID      当前数据ID 
         * @param[i] parent            父对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        ProbeCurveWidget(int currentObjID, QWidget* parent = nullptr);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        ~ProbeCurveWidget();
        /**
         * @brief    初始化
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-28
         */
        void init();
        /**
         * @brief    更新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        void update();
        /**
         * @brief    清空
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        void clear();
    private slots:
        ;
        /**
         * @brief    后处理数据修改槽函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-28
         */
        void slotCFDPostDataUpdate();
    private:
        /**
         * @brief    更新数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        void updateValue();
        /**
         * @brief    更新曲线
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        void updateCurve();
    private:
        /**
         * @brief  ui
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-24
         */
        Ui::ProbeCurveWidget* _ui = nullptr;
        /**
         * @brief  探针数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-24
         */
        QList<CurveStr> _value = {};
        /**
         * @brief  曲线组件接口
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-28
         */
        Plot::FITKPlotWindowsInterface* _plotWinInter = nullptr;
        /**
         * @brief  曲线界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-28
         */
        Plot::FITKXYPlotWindow* _plotWin = nullptr;
    };
}

#endif
