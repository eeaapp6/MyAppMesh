/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUICSVCurveDialog.h"
#include "ui_GUICSVCurveDialog.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKAPPSettings.h"
#include "FITK_Component/FITKPlotWindow/FITKPlotWindowsInterface.h"
#include "FITK_Component/FITKPlotWindow/FITKXYPlotWindow.h"
#include "FITK_Component/FITKPlotWindow/FITKXYPlotProperty.h"
#include "FITK_Component/FITKPlotWindow/FITKXYCurveDrawManager.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QSplitter>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

#include <QMessageBox>
#include <QTimer>
namespace GUI
{
    GUICSVCurveDialog::GUICSVCurveDialog(Core::FITKActionOperator* oper, QWidget* parent) :
        FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUICSVCurveDialog;
        _ui->setupUi(this);
        // 设置窗口标题
        setWindowTitle("CSVC Curve");
        QTimer::singleShot(0, this, &GUICSVCurveDialog::init);
    }

    GUICSVCurveDialog::~GUICSVCurveDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GUICSVCurveDialog::init()
    {
        QString file;
        if (!_oper->argValue<QString>("ImportFile", file))
        {
            AppFrame::FITKMessageError(tr("File does not exist: %1").arg(file));
        }
        QFileInfo fi(file);
        if (!fi.exists())
        {
            AppFrame::FITKMessageError(tr("File does not exist: %1").arg(file));
        }
        // 读取CSV文件数据
        if (!this->readCSVFile(file))
        {
            AppFrame::FITKMessageError(tr("Failed to read CSV file: %1").arg(file));
        }
        // 创建xy直角坐标系界面
        Plot::FITKPlotWindowsInterface* plotWindows = FITKAPP->getComponents()->getComponentTByName<Plot::FITKPlotWindowsInterface>("FITKPlotWindowsInterface");
        if (!plotWindows)
        {
            AppFrame::FITKMessageError(tr("Failed to get plot windows interface"));
        }

        _xyPlotWindow = dynamic_cast<Plot::FITKXYPlotWindow*>(plotWindows->getWidget(0));
        if (!_xyPlotWindow)
        {
            AppFrame::FITKMessageError(tr("Failed to get XY plot window"));
        }
        // 设置绘图窗口的最小尺寸和尺寸策略
        _xyPlotWindow->setMinimumSize(300, 300);
        _xyPlotWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // 创建表格控件
        _tableWidget = new QTableWidget();
        // 设置表格的尺寸策略和最小尺寸
        _tableWidget->setMinimumSize(300, 300);
        _tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        initTableWidget();
        // 创建曲线选择控件
        QWidget* controlWidget = new QWidget();
        // 设置控制控件的尺寸策略
        controlWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        controlWidget->setMaximumHeight(40);
        QHBoxLayout* controlLayout = new QHBoxLayout(controlWidget);
        controlLayout->setContentsMargins(0, 0, 0, 0);  // 去掉控制布局的边距
        controlLayout->setSpacing(5);  // 设置控件之间的间距
        QLabel* curveLabel = new QLabel("Curve Name:");
        _curveComboBox = new QComboBox();
        // 添加曲线名称到下拉框
        for (const QString& curveName : _curveNames) {
            _curveComboBox->addItem(curveName);
        }
        controlLayout->addWidget(curveLabel);
        controlLayout->addWidget(_curveComboBox);
        controlLayout->addStretch(); // 添加弹性空间，使控件靠左对齐
        // 创建水平分割器
        QSplitter *splitterH = new QSplitter(Qt::Horizontal);
        splitterH->addWidget(_tableWidget);
        splitterH->addWidget(_xyPlotWindow);
        splitterH->setSizes({ 400, 600 });
        splitterH->setStretchFactor(0, 1);  // 表格的拉伸因子
        splitterH->setStretchFactor(1, 2);  // 绘图窗口的拉伸因子（占据更多空间）
        // 创建主布局
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->setSpacing(2);  // 统一组件之间的间距
        mainLayout->setContentsMargins(5, 5, 5, 5);  // 设置适中的边距
        mainLayout->addWidget(controlWidget);
        mainLayout->addWidget(splitterH);
        // 清除原有的布局内容
        QLayoutItem* child;
        while ((child = _ui->gridLayout->takeAt(0)) != nullptr) {
            delete child->widget(); // 删除widget
            delete child; // 删除layout item
        }
        // 将新的布局添加到布局中（第0行，第0列，占1行1列）
        _ui->gridLayout->addLayout(mainLayout, 0, 0);
        // 初始化曲线的设置
        Plot::FITKXYPlotProperty* propCurvePlot = _xyPlotWindow->getPlotProperty();
        if (!propCurvePlot)
        {
            AppFrame::FITKMessageError(tr("Failed to get plot property"));
        }
        propCurvePlot->setPlotMagnifier(false);
        propCurvePlot->setPlotPanner(false);
        propCurvePlot->setLegendClickable(false);
        // 连接信号槽
        connect(_curveComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GUICSVCurveDialog::onCurveSelected);
        // 强制布局更新
        this->layout()->update();
        this->layout()->activate();
        // 初始化显示第一条曲线
        if (!_curveNames.isEmpty()) {
            _curveComboBox->setCurrentIndex(0);
            this->updateDisplay(0);
        }
    }

    void GUICSVCurveDialog::initTableWidget()
    {
        if (!_tableWidget || _curveValues.isEmpty() || _timeData.isEmpty()) return;
        // 清理数据
        _tableWidget->clear();
        // 设置行数和列数时间和选中的曲线
        int rowCount = _timeData.size();
        int columnCount = 2; // 第一列为时间，第二列为选中的曲线数据
        _tableWidget->setRowCount(rowCount);
        _tableWidget->setColumnCount(columnCount);
        // 设置表头
        QStringList headers;
        headers << "Time" << "Value";
        _tableWidget->setHorizontalHeaderLabels(headers);

        // 表格数据将在updateDisplay中填充
        _tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        _tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }

    void GUICSVCurveDialog::initPlotWindow()
    {
        if (!_xyPlotWindow) return;
        // 清理数据
        Plot::FITKXYCurveDrawManager* curveDrawManager = _xyPlotWindow->getCurveManager();
        if (!curveDrawManager) return;
        curveDrawManager->clear();
        // 图表数据将在updateDisplay中创建
        _xyPlotWindow->updataCanvas();
    }

    void GUICSVCurveDialog::updateDisplay(int curveIndex)
    {
        if (curveIndex < 0 || curveIndex >= _curveNames.size()) return;
        // 更新表格显示
        updateTableWidget(curveIndex);
        // 更新图表显示
        updatePlotWindow(curveIndex);
    }

    void GUICSVCurveDialog::updateTableWidget(int curveIndex)
    {
        if (!_tableWidget || _curveValues.isEmpty() || _timeData.isEmpty()) return;
        if (curveIndex < 0 || curveIndex >= _curveValues.size()) return;
        // 设置第二列表头为当前曲线名称
        _tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(_curveNames[curveIndex]));
        // 填充数据
        QVector<double>& selectedCurveData = _curveValues[curveIndex];
        for (int row = 0; row < _timeData.size(); ++row)
        {
            // 第一列为时间数据
            if (_tableWidget->item(row, 0) == nullptr) {
                _tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(_timeData[row])));
            }
            else {
                _tableWidget->item(row, 0)->setText(QString::number(_timeData[row]));
            }
            // 第二列为选中的曲线数据
            double value = selectedCurveData[row];
            if (_tableWidget->item(row, 1) == nullptr) {
                _tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(value)));
            }
            else {
                _tableWidget->item(row, 1)->setText(QString::number(value));
            }
        }
    }

    void GUICSVCurveDialog::updatePlotWindow(int curveIndex)
    {
        if (!_xyPlotWindow || _curveValues.isEmpty() || _timeData.isEmpty()) return;
        if (curveIndex < 0 || curveIndex >= _curveValues.size()) return;
        // 清理数据
        Plot::FITKXYCurveDrawManager* curveDrawManager = _xyPlotWindow->getCurveManager();
        if (!curveDrawManager) return;
        curveDrawManager->clear();
        // 创建当前选中的曲线
        QVector<double>& yData = _curveValues[curveIndex];
        QString curveName = _curveNames[curveIndex];
        // 创建曲线
        Plot::FITKXYCurveDrawProp* curveData = new Plot::FITKXYCurveDrawProp;
        curveData->setData(curveName, _timeData, yData);
        curveData->setVisible(true);
        curveDrawManager->appendGraphObj(curveData);
        // 更新曲线图
        _xyPlotWindow->updataCanvas();
    }

    void GUICSVCurveDialog::onCurveSelected(int index)
    {
        this->updateDisplay(index);
    }

    bool GUICSVCurveDialog::readCSVFile(const QString& filePath)
    {
        // 清理现有数据
        _timeData.clear();
        _curveValues.clear();
        _curveNames.clear();

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
        QTextStream in(&file);
        // 读取第一行（标题行）
        QString firstLine = in.readLine().trimmed();
        if (firstLine.isEmpty())
        {
            file.close();
            return false;
        }
        // 解析标题行
        QStringList fields = parseCSVLine(firstLine);
        // 第一列是"time"，其余列是曲线名称（至少需要1个有效曲线列）
        if (fields.size() < 2)
        {
            file.close();
            return false;
        }
        int count = fields.size();
        for (int i = 1; i < count; ++i)
        {
            QString curveName = fields[i];
            // 1. 移除字段中的引号（处理CSV中带引号的标题）
            curveName = curveName.remove('"');
            curveName = curveName.simplified();
            if (curveName.isEmpty())
            {
                continue;
            }
            _curveNames.append(curveName);
            _curveValues.append(QVector<double>());
        }
        // 使用过滤后的曲线数量
        int validCurveCount = _curveNames.size(); // 有效曲线数量（已过滤空列）
        // 若没有有效曲线，直接返回失败
        if (validCurveCount == 0)
        {
            file.close();
            QMessageBox::warning(nullptr, QObject::tr("Error"), QObject::tr("No valid curve names found in CSV file"));
            return false;
        }
        // 读取所有数据行（使用有效曲线数量校验）
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList dataFields = parseCSVLine(line);
            // 校验：数据行字段数 ≥ 有效曲线数 + 1（时间列）
            if (dataFields.size() < validCurveCount + 1) continue;
            // 解析时间列
            bool ok;
            double time = dataFields[0].toDouble(&ok);
            if (!ok) continue;
            _timeData.append(time);
            // 解析有效曲线的数据（仅处理过滤后的曲线）
            for (int i = 0; i < validCurveCount; ++i)
            {
                double value = dataFields[i + 1].toDouble(&ok);
                if (!ok) value = 0.0;
                _curveValues[i].append(value);
            }
        }
        file.close();
        return !_timeData.isEmpty() && !_curveValues.isEmpty();
    }

    QStringList GUICSVCurveDialog::parseCSVLine(const QString& line)
    {
        QStringList fields;
        QString field;
        bool inQuotes = false;
        for (int i = 0; i < line.length(); ++i)
        {
            QChar currentChar = line[i];

            if (currentChar == '"')
            {
                inQuotes = !inQuotes;
            }
            else if (currentChar == ',' && !inQuotes)
            {
                fields.append(field);
                field.clear();
            }
            else
            {
                field.append(currentChar);
            }
        }
        // 添加最后一个字段
        fields.append(field);
        return fields;
    }
}