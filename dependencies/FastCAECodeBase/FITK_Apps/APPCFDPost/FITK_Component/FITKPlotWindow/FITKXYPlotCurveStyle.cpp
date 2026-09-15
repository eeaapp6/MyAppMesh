/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKXYPlotCurveStyle.h"
#include "FITKXYPlotWindow.h"
#include "ui_FITKXYPlotCurveStyle.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_text.h>

#include <QColorDialog>
#include <QDialog>

namespace Plot {
    FITKXYPlotCurveStyle::FITKXYPlotCurveStyle(FITKXYPlotWindow *parent)
    {
        _ui = new Ui::FITKXYPlotCurveStyle;
        _ui->setupUi(this);

    //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);

    //获得窗口所有曲线
        _plot = parent->getPlotWidget();
        _curveList = _plot->itemList(QwtPlotItem::Rtti_PlotCurve);

    //初始化
        connect(_ui->curveComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_currentCurveChanged(int)));
        init();
        exec();
    }

    FITKXYPlotCurveStyle::~FITKXYPlotCurveStyle()
    {
        if (_ui) delete _ui;
        _ui = nullptr;
    }

    void FITKXYPlotCurveStyle::init()
    {
        for (auto c : _curveList)
        {
            auto curve = dynamic_cast<QwtPlotCurve*>(c);
            if (curve == nullptr) continue;
            QwtText title = curve->title();
            _ui->curveComBox->addItem(title.text());
        }
    }

    void FITKXYPlotCurveStyle::fillCurve(QwtPlotCurve * curveitem)
    {
        //QwtText tit = curveitem->title();
        //_ui->namelineEdit->setText(tit.text());

        QPen pen = curveitem->pen();
        QColor col = pen.color();
        int width = pen.width();
        Qt::PenStyle style = pen.style();

        _ui->typecomBox->setCurrentIndex((int)style);
        _ui->widthSpinBox->setValue(width);
        QString strsheet = QString("background-color: rgb(%1,%2,%3)").arg(col.red()).arg(col.green()).arg(col.blue());
        _ui->colorButton->setStyleSheet(strsheet);

        const QwtSymbol* sym = curveitem->symbol();
        if (sym == nullptr)
            _ui->symbolcomBox->setCurrentIndex(0);
        else
        {
            const QwtSymbol::Style sty = sym->style();
            int index = (int)sty+1;
            _ui->symbolcomBox->setCurrentIndex(index);
        }
    }
   
    void FITKXYPlotCurveStyle::on_applyButton_clicked()
    {
        int index = _ui->curveComBox->currentIndex();
        if (index < 0 || index >= _curveList.size()) return;
        auto c = _curveList.at(index);
        auto curve = dynamic_cast<QwtPlotCurve*>(c);
        if (!curve)return;
        QColor color = _ui->colorButton->palette().color(QPalette::Background);
        int size = _ui->widthSpinBox->value();
        int type = _ui->typecomBox->currentIndex();

        QPen pen(color, size, (Qt::PenStyle)type);
        curve->setPen(pen);

        //QString title = _ui->namelineEdit->text();
       //curve->setTitle(title);
       //_ui->curveComBox->setItemText(index, title);

        int syIndex = _ui->symbolcomBox->currentIndex()-1;
        QwtSymbol* symbol = new QwtSymbol;
        QPen p(color, size+4);
        symbol->setPen(p);
        symbol->setStyle((QwtSymbol::Style)(syIndex));
        curve->setSymbol(symbol);

        _plot->replot();
    }

    void FITKXYPlotCurveStyle::on_colorButton_clicked()
    {
        QColor color = _ui->colorButton->palette().color(QPalette::Background);
        QColor col = QColorDialog::getColor(color);
        if (!col.isValid()) return;
        QString strsheet = QString("background-color: rgb(%1,%2,%3)").arg(col.red()).arg(col.green()).arg(col.blue());
        _ui->colorButton->setStyleSheet(strsheet);
    }

    void FITKXYPlotCurveStyle::on_cancelButton_clicked()
    {
        this->close();
    }

    void FITKXYPlotCurveStyle::on_currentCurveChanged(int index)
    {
        if (index < 0 || index >= _curveList.size()) return;
        auto c = _curveList.at(index);
        auto curve = dynamic_cast<QwtPlotCurve*>(c);
        if (curve == nullptr) return;
        fillCurve(curve);
    }
}
