/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKXYPlotWindow.h"
#include "ui_FITKXYPlotWindow.h"
#include "FITKXYPlotCurveStyle.h"
#include "FITKXYPlotProperty.h"
#include "FITKXYCurveDrawManager.h"

#include <qwt_plot.h>
#include <qwt_scale_engine.h>
#include <qwt_legend.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_renderer.h>

#include <QPen>
#include <QToolBar>
#include <cmath>

namespace Plot
{
    FITKXYPlotWindow::FITKXYPlotWindow(QWidget* parent/*= nullptr*/)
        :Core::FITKAbstractGraph2DWidget(parent)
    {
        _ui = new Ui::FITKXYPlotWindow;
        _ui->setupUi(this);

        _plotWidget = new QwtPlot;
        _ui->gridLayout->addWidget(_plotWidget);
        _plotProperty = new FITKXYPlotProperty(this);
        _curveDrawManager = new FITKXYCurveDrawManager(this);

    //画布边距留白
        //_plotWidget->canvas()->setContentsMargins(18, 0, 18, 10);

    //滚轮放大缩小
        _plotMagnifier = new QwtPlotMagnifier(_plotWidget->canvas());

    //使用鼠标左键平移 
        _plotPanner = new QwtPlotPanner(_plotWidget->canvas());

    //背景色
        _plotWidget->canvas()->setStyleSheet(
            "border: 1px solid Black;"
            "border-radius: 1px;"
            "background-color: rgba(250,250,255,190)"
        );
    //刻度值字体
        QFont font;
        font.setPointSize(13); 
        font.setBold(false);  //是否加粗
        _plotWidget->setAxisFont(QwtPlot::Axis::yLeft, font);
        _plotWidget->setAxisFont(QwtPlot::Axis::xBottom, font);

    //坐标轴底部曲线图例
        QwtLegend*_legend = new QwtLegend(_plotWidget);
        _legend->setDefaultItemMode(QwtLegendData::Mode::Clickable);
        font.setPointSize(12);
        _legend->setFont(font);
        _plotWidget->insertLegend(_legend, QwtPlot::BottomLegend);
        connect(_legend, SIGNAL(clicked(const QVariant&, int)), this, SLOT(legendClicked(const QVariant&, int)));

    //自动更新画布设置-不自动
        //const bool doAutoReplot = _plotWidget->autoReplot();
        _plotWidget->setAutoReplot(false);
        //_plotWidget->setAutoReplot(doAutoReplot);
        _plotWidget->replot();

    //添加工具栏
        _renderToolBar = new QToolBar();
        _renderToolBar->setContentsMargins(0, 0, 0, 0);
        _renderToolBar->setIconSize(QSize(40, 40));
        initActions();

    //大刻度个数
         _plotWidget->setAxisMaxMajor(QwtPlot::yLeft, 20);
         _plotWidget->setAxisMaxMajor(QwtPlot::xBottom, 20);

    //添加到QwtPlot上的所有图元项将在QwtPlot回收时，将自动析构
        _plotWidget->setAutoDelete(true);

    //重置尺寸
        this->setMinimumSize(QSize(1, 1));
    }

    FITKXYPlotWindow::~FITKXYPlotWindow()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
        if (_curveDrawManager) delete _curveDrawManager;
        _curveDrawManager = nullptr;
        if (_plotProperty) delete _plotProperty;
        _plotProperty = nullptr; 
        if (_renderToolBar) delete _renderToolBar;
        _renderToolBar = nullptr;
    //最后析构Plot
        if (_plotWidget) delete _plotWidget;
        _plotWidget = nullptr;
    }

    QwtPlot* FITKXYPlotWindow::getPlotWidget()
    {
        return _plotWidget;
    }

    QToolBar * FITKXYPlotWindow::getToolBar()
    {
        return _renderToolBar;
    }

    FITKXYPlotProperty * FITKXYPlotWindow::getPlotProperty()
    {
        return _plotProperty;
    }

    FITKXYCurveDrawManager * FITKXYPlotWindow::getCurveManager()
    {
        return _curveDrawManager;
    }

    void FITKXYPlotWindow::updataCanvas(bool updataByUser, int axis)
    {
        if (!_plotWidget)return;

        double xmin{}, xmax{}, ymin{}, ymax{};
        QList<QwtPlotItem*> items = _plotWidget->itemList();

        auto xAxis = [&]()
        {
            _Xmin =  1e64;
            _Xmax =  -1e64;
            for (int i = 0; i < items.size(); ++i)
            {
                QwtPlotCurve* cur = dynamic_cast<QwtPlotCurve*>(items[i]);
                if (!cur|| !cur->isVisible())continue;
                QPen pen = cur->pen();
                if(pen.style()== Qt::PenStyle::NoPen)continue;
                xmin = cur->minXValue();
                xmax = cur->maxXValue();
                if (xmin < _Xmin) _Xmin = xmin;
                if (xmax > _Xmax) _Xmax = xmax;
            }
        };
        auto yAxis = [&]()
        {
            _Ymin = 1e64;
            _Ymax = -1e64;
            for (int i = 0; i < items.size(); ++i)
            {
                QwtPlotCurve* cur = dynamic_cast<QwtPlotCurve*>(items[i]);
                if (!cur || !cur->isVisible())continue;
                QPen pen = cur->pen();
                if (pen.style() == Qt::PenStyle::NoPen)continue;
                ymin = cur->minYValue();
                ymax = cur->maxYValue();
                if (ymin < _Ymin) _Ymin = ymin;
                if (ymax > _Ymax) _Ymax = ymax;
            }
        };

        //全不自动
        if(updataByUser && !(axis==0||axis==2))
        {
            _Xmax = _userXmax;
            _Xmin = _userXmin;
            _Ymin = _userYmin;
            _Ymax = _userYmax;
            if (fabs(_Xmin- _Xmax) < 1e-7) xAxis();
            if (fabs(_Ymin- _Ymax) < 1e-7) yAxis();
        }
        //全自动
        else if(!updataByUser)
        {
            xAxis();
            yAxis();
        }
        //Y不自动，X自动
        else if (updataByUser && axis==0)
        {
            _Ymin = _userYmin;
            _Ymax = _userYmax;
            if (fabs(_Ymin-_Ymax)< 1e-7) yAxis();
            xAxis();
        }
        //Y自动，X不自动
        else if (updataByUser && axis == 2)
        {
            _Xmax = _userXmax;
            _Xmin = _userXmin;
            if (fabs(_Xmin -_Xmax) < 1e-7) xAxis();
            yAxis();
        }

    //如果Floating这项属性被设置为真，代表该轴变为Log指数轴
        bool yLeft = _plotWidget->axisScaleEngine(QwtPlot::yLeft)->testAttribute(QwtScaleEngine::Floating);
        yLeft &= (fabs(_Ymin- _Ymax)> 1e-7);
        bool xBottom = _plotWidget->axisScaleEngine(QwtPlot::xBottom)->testAttribute(QwtScaleEngine::Floating);
        xBottom &= (fabs(_Xmin- _Xmax) > 1e-7);
        if (yLeft) {
            if(_plotProperty->getLogAxisOrderOfMagnitudeEndpoint(0))
                calculatorEndpoint(_Ymin, _Ymax);
            _plotWidget->setAxisScale(QwtPlot::yLeft, _Ymin, _Ymax, 0);
        } else {
            double offset_y = (_Ymax - _Ymin) / 16;
            _plotWidget->setAxisScale(QwtPlot::yLeft, _Ymin - offset_y, _Ymax + offset_y, 0);
        }
        if (xBottom) {
            if (_plotProperty->getLogAxisOrderOfMagnitudeEndpoint(2))
                calculatorEndpoint(_Xmin, _Xmax);
            _plotWidget->setAxisScale(QwtPlot::xBottom, _Xmin, _Xmax, 0);
        } else {
            double offset_x = (_Xmax - _Xmin) / 16;
            _plotWidget->setAxisScale(QwtPlot::xBottom, _Xmin - offset_x, _Xmax + offset_x, 0);
        }  
        _plotWidget->replot();
    }

    QwtPlotMagnifier * FITKXYPlotWindow::getPlotMagnifier()
    {
        return _plotMagnifier;
    }

    QwtPlotPanner * FITKXYPlotWindow::getPlotPanner()
    {
        return _plotPanner;
    }

    void FITKXYPlotWindow::saveImage(const QString &image, int w, int h)
    {
        if(!_plotWidget) return;

        //获取 image 后缀
        QString suffix = image.section('.', -1, -1).toUpper();
        if (suffix != "PNG" && suffix != "JPG" && suffix != "JPEG" && suffix != "BMP")
            return;
        
        // 设置图像大小
        QSizeF size;
        if (w > 0 && h > 0) {
            size = QSizeF(w, h);
        } else {
            size = _plotWidget->size();// 默认大小
        }

        // 创建渲染器
        QwtPlotRenderer renderer;
        // 设置渲染选项
        //renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, true);
        //renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground, true);

        // 保存为不同格式
        renderer.renderDocument(_plotWidget, image, size); // 指定尺寸300x200
    }

    void FITKXYPlotWindow::initActions()
    {
        QList<QAction*> actions = {};
        QAction* action = nullptr;

    //复位界面按钮
        action = new QAction();
        action->setObjectName("actionRefreshPlot");
        action->setIcon(QIcon(":FITKIcons/icoR_synchLayers.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionRefreshPlotEvent()));
        actions.append(action);

    //改样式按钮
        action = new QAction();
        action->setObjectName("actionStylePlot");
        action->setIcon(QIcon(":FITKIcons/icoR_options.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionStyleEvent()));
        actions.append(action);

    //追加action
        _renderToolBar->insertActions(nullptr, actions);
        _renderToolBar->setOrientation(Qt::Vertical);
        _ui->gridLayout_2->addWidget(_renderToolBar, 0, 1, Qt::AlignRight | Qt::AlignTop);
    }

    void FITKXYPlotWindow::calculatorEndpoint(double& min, double& max)
    {
        if (std::abs(min) > std::numeric_limits<double>::epsilon()) {
            double sign = min > 0 ? 1.0 : -10;
            double absNum = std::abs(min);
            double magnitude = std::pow(10, std::floor(std::log10(absNum)));
            min = sign * magnitude;
        }
        if (std::abs(max) > std::numeric_limits<double>::epsilon()) {
            double sign = max > 0 ? 1.0 : -0.1;
            double absNum = std::abs(max);
            double magnitude = std::pow(10, std::ceil(std::log10(absNum)));
            max = sign * magnitude;
        }
    }

    void FITKXYPlotWindow::legendClicked(const QVariant& va, int index)
    {
        QwtPlotItem* plotItem = _plotWidget->infoToItem(va);
        if (plotItem == nullptr) return;
        auto  curve = dynamic_cast<QwtPlotCurve*>(plotItem);

    //曲线是否可见
        if (curve->isVisible()) 
            curve->setVisible(false);
        else 
            curve->setVisible(true);
        _plotWidget->replot();
    }

    void FITKXYPlotWindow::slotActionRefreshPlotEvent()
    {
        QwtPlotItemList curves = _plotWidget->itemList(QwtPlotItem::Rtti_PlotCurve);
        //显示所有被隐藏的曲线
        //for (QwtPlotItem* item : curves) {
        //    item->setVisible(true);
        //}
        updataCanvas();
    }

    void FITKXYPlotWindow::slotActionStyleEvent()
    {
        FITKXYPlotCurveStyle dlg(this);
    }
}

