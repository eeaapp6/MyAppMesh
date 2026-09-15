/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUICSVCurveDialog.h
 * @brief  CSV曲线窗口
 * @author guqingtao (15598887859@163.com)
 * @date   2025-10-20
 *********************************************************************/

#ifndef _GUICSVCurveDialog_H__
#define _GUICSVCurveDialog_H__

#include "GUIPostDialogAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"
#include <QTableWidget>
#include <QVector>
#include <QStringList>
#include <QComboBox>

namespace Ui
{
    class GUICSVCurveDialog;
}
namespace Core
{
    class FITKActionOperator;
}

namespace Plot {
    class FITKXYPlotWindow;
    class FITKXYPlotProperty;
}

namespace GUI
{
    /**
     * @brief  CSV曲线窗口
     * @author guqingtao (15598887859@163.com)
     * @date   2025-10-20
     */
    class GUIPostDialogAPI GUICSVCurveDialog :public Core::FITKDialog
    {
        Q_OBJECT

    public:

        explicit GUICSVCurveDialog(Core::FITKActionOperator* oper, QWidget* parent = nullptr);
        virtual ~GUICSVCurveDialog();
 
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-20
         */
        void init();
        /**
         * @brief    初始化表格
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-20
         */
        void initTableWidget();
        /**
         * @brief    初始化曲线
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-20
         */
        void initPlotWindow();
        /**
         * @brief    读CSV文件
         * @param[i] filePath 
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-20
         */
        bool readCSVFile(const QString& filePath);
        /**
         * @brief    处理一行数据
         * @param[i] line 
         * @return   QStringList
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-10-20
         */
        QStringList parseCSVLine(const QString& line);
        /**
         * @brief    更新显示
         * @param[i] curveIndex 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-19
         */
        void updateDisplay(int curveIndex);
        /**
         * @brief    更新表格
         * @param[i] curveIndex 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-19
         */
        void updateTableWidget(int curveIndex);
        /**
         * @brief    更新曲线
         * @param[i] curveIndex 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-11-19
         */
        void updatePlotWindow(int curveIndex);
    private slots:
        /**
         * @brief  选择曲线
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-19
         */
        void onCurveSelected(int index);
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-20
         */
        Ui::GUICSVCurveDialog* _ui = nullptr;
        /**
         * @brief  参数表格
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-20
         */
        QTableWidget* _tableWidget = nullptr;
        /**
         * @brief  直接坐标系图表
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-20
         */
        Plot::FITKXYPlotWindow* _xyPlotWindow = nullptr;
        /**
         * @brief  时间数据（x轴）
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-20
         */
        QVector<double> _timeData;    
        /**
         * @brief  曲线数据
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-20
         */
        QList<QVector<double>> _curveValues; 
        /**
         * @brief  曲线名称
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-20
         */
        QStringList _curveNames;
        /**
         * @brief  操作器
         * @author guqingtao (15598887859@163.com)
         * @date   2025-10-20
         */
        Core::FITKActionOperator* _oper = nullptr;
        /**
         * @brief  曲线下拉框
         * @author guqingtao (15598887859@163.com)
         * @date   2025-11-19
         */
        QComboBox* _curveComboBox{};
    };
}

#endif // 