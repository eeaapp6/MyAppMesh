/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ProbeCurveWidget.h"
#include "ui_ProbeCurveWidget.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostProbe.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostUnSteady.h"
#include "FITK_Component/FITKPlotWindow/FITKPlotWindowsInterface.h"
#include "FITK_Component/FITKPlotWindow/FITKXYPlotWindow.h"
#include "FITK_Component/FITKPlotWindow/FITKXYPlotProperty.h"
#include "FITK_Component/FITKPlotWindow/FITKXYCurveDrawManager.h"

#include <vtkDataSet.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QDebug>

namespace GUI
{
    int getColor() {
        // 初始化随机数种子
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // 生成0到255之间的随机整数
        int randomNumber = std::rand() % 256;  // rand() % 256 生成0到255之间的随机数

        return randomNumber;
    }

    ProbeCurveWidget::ProbeCurveWidget(int currentObjID, QWidget * parent) :
        GUIWidgetBase(parent)
    {
        _ui = new Ui::ProbeCurveWidget();
        _ui->setupUi(this);
        _currentDataID = currentObjID;
        _plotWinInter = FITKAPP->getComponents()->getComponentTByName<Plot::FITKPlotWindowsInterface>("FITKPlotWindowsInterface");
        if (_plotWinInter == nullptr)return;
        _plotWin = dynamic_cast<Plot::FITKXYPlotWindow*>(_plotWinInter->getWidget(0));
        init();
    }

    ProbeCurveWidget::~ProbeCurveWidget()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }

        if (_plotWin) {
            delete _plotWin;
            _plotWin = nullptr;
        }
    }

    void ProbeCurveWidget::init()
    {
        if (_plotWin == nullptr)return;
        _ui->gridLayout->addWidget(_plotWin);
        Plot::FITKXYPlotProperty* plotPro = _plotWin->getPlotProperty();
        if (plotPro == nullptr)return;
        plotPro->setTitle("");
        plotPro->setAxisName("Time", "Value");
        plotPro->setLegendPlace(1);

        Interface::FITKCFDPostProbe* probe = dynamic_cast<Interface::FITKCFDPostProbe*>(_postDataManager->getDataByID(_currentDataID));
        if (probe == nullptr)return;
        Interface::FITKCFDPostUnSteady* parent = dynamic_cast<Interface::FITKCFDPostUnSteady*>(_postDataManager->getDataByID(probe->getCFDPostParentID()));
        if (parent == nullptr)return;

        for (int i = 0; i < probe->getOutput()->GetNumberOfPoints(); i++) {
            QStringList pointNames = probe->getPointDataArray();
            for (QString pointName : pointNames) {
                CurveStr curve;
                curve._type = Interface::FITKPostFieldType::Post_Point;
                curve._ID = i;
                curve._fileName = pointName;
                curve._curveData = new Plot::FITKXYCurveDrawProp();
                //设置随机色
                curve._curveData->setColor(QColor::fromHsl(rand() % 360, rand() % 256, rand() % 200));
                _value.append(curve);
            }
            QStringList cellsNames = probe->getCellDataArray();
            for (QString cellName : cellsNames) {
                CurveStr curve;
                curve._type = Interface::FITKPostFieldType::Post_Cell;
                curve._ID = i;
                curve._fileName = cellName;
                curve._curveData = new Plot::FITKXYCurveDrawProp();
                //设置随机色
                curve._curveData->setColor(QColor::fromHsl(rand() % 360, rand() % 256, rand() % 200));
                _value.append(curve);
            }
        }
        update();
        connect(probe, SIGNAL(sigCFDPostDataUpdate()), this, SLOT(slotCFDPostDataUpdate()));
    }

    void ProbeCurveWidget::update()
    {
        updateValue();
        updateCurve();
    }

    void ProbeCurveWidget::clear()
    {
        _value.clear();
        updateCurve();
    }

    void ProbeCurveWidget::slotCFDPostDataUpdate()
    {
        update();
    }

    void ProbeCurveWidget::updateValue()
    {
        Interface::FITKCFDPostProbe* probe = dynamic_cast<Interface::FITKCFDPostProbe*>(_postDataManager->getDataByID(_currentDataID));
        if (probe == nullptr)return;
        Interface::FITKCFDPostUnSteady* parent = dynamic_cast<Interface::FITKCFDPostUnSteady*>(_postDataManager->getDataByID(probe->getCFDPostParentID()));
        if (parent == nullptr)return;

        double currentTime = parent->getCurrentTime();
        for (int i = 0; i < _value.size();i++) {
            double value = probe->getFieldValue(_value[i]._ID, _value[i]._type, _value[i]._fileName);
            _value[i]._value.insert(currentTime, value);
            QList<double> times = _value[i]._value.keys();
            for (double t : times) {
                if (t > currentTime)_value[i]._value.remove(t);
            }
        }
    }

    void ProbeCurveWidget::updateCurve()
    {
        if (_plotWin == nullptr)return;        
        for (CurveStr curve : _value) {
            QVector<double> X, Y;
            for (double t : curve._value.keys()) {
                X.push_back(t);
            }
            for (double v : curve._value.values()) {
                Y.push_back(v);
            }
            bool isOk = curve._curveData->setData(curve._fileName, X, Y);
            if (isOk) _plotWin->getCurveManager()->appendGraphObj(curve._curveData);;
        }
        _plotWin->updataCanvas();
    }
}
