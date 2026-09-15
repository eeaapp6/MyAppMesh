/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "StreamLineDialog.h"
#include "ui_StreamLineDialog.h"

#include "GUIFrame/MainWindow.h"
#include "GUIFrame/RenderWidget.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "PostGraphAdaptor/PostGraphWidgetLine.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKVec3D.h"
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostStreamLine.h"

#include <QMessageBox>

Q_DECLARE_METATYPE(Interface::FITKPostFieldType);
Q_DECLARE_METATYPE(Interface::FITKPostStreamDirection);

namespace GUI
{
    StreamLineDialog::StreamLineDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate) :
        GUIDialogBase(oper,"StreamLineDialog", FITKAPP->getGlobalData()->getMainWindow()), _isCreate(isCreate)
    {
        _ui = new Ui::StreamLineDialog();
        _ui->setupUi(this);
        if (isCreate) {
            _parentDataID = dataID;
        }
        else {
            _currentDataID = dataID;
            if (_postDataManager->getDataByID(_currentDataID)) {
                _parentDataID = _postDataManager->getDataByID(_currentDataID)->getCFDPostParentID();
            }
        }
        init();
    }

    StreamLineDialog::~StreamLineDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void StreamLineDialog::init()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        //设置vtk界面
        if (_mainWin == nullptr)return;
        QWidget* currentWidget = _mainWin->getRenderWidget()->getRenderWidget();
        _lineWidget = new Interface::PostGraphWidgetLine(currentWidget);

        //获取矢量
        int index = 0;
        QStringList pointDataArray = parentData->getPointDataArray();
        for (QString point : pointDataArray) {
            Interface::FITKPostVariableType type = parentData->getVariableType(Interface::FITKPostFieldType::Post_Point, point);
            if (type == Interface::FITKPostVariableType::Post_Vector) {
                _ui->comboBox_Vector->addItem(QIcon("://icons/nodeValue.png"), point);
                _ui->comboBox_Vector->setItemData(index, QVariant::fromValue(Interface::FITKPostFieldType::Post_Point));
                index++;
            }
        }
        QStringList cellDataArray = parentData->getCellDataArray();
        for (QString cell : cellDataArray) {
            Interface::FITKPostVariableType type = parentData->getVariableType(Interface::FITKPostFieldType::Post_Cell, cell);
            if (type == Interface::FITKPostVariableType::Post_Vector) {
                _ui->comboBox_Vector->addItem(QIcon("://icons/cellValue.png"), cell);
                _ui->comboBox_Vector->setItemData(index, QVariant::fromValue(Interface::FITKPostFieldType::Post_Cell));
                index++;
            }
        }

        //设置方向
        _ui->comboBox_Direction->addItem(QString(tr("Forward")), QVariant::fromValue(Interface::FITKPostStreamDirection::Post_Forward));
        _ui->comboBox_Direction->addItem(QString(tr("Backward")), QVariant::fromValue(Interface::FITKPostStreamDirection::Post_Backward));
        _ui->comboBox_Direction->addItem(QString(tr("Both")), QVariant::fromValue(Interface::FITKPostStreamDirection::Post_Both));

        if (_isCreate) {
            InitCreate();
            setWindowTitle(tr("StreamLine Create"));
        }
        else {
            InitEdit();
            setWindowTitle(tr("StreamLine Edit"));
        }

        connect(_lineWidget, SIGNAL(sigValueChange(double*, double*)), this, SLOT(slotValueChange(double*, double*)));
        connect(_ui->lineEdit_Point1X, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point1Y, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point1Z, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point2X, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point2Y, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
        connect(_ui->lineEdit_Point2Z, SIGNAL(dataChanged()), this, SLOT(slotDataChange()));
    }

    void StreamLineDialog::showEvent(QShowEvent * event)
    {
        if (_lineWidget) {
            _lineWidget->setIsShow(true);
            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
            if (graphOper) {
                graphOper->reRender();
            }
        }
        GUIDialogBase::showEvent(event);
    }

    void StreamLineDialog::closeEvent(QCloseEvent * event)
    {
        if (_lineWidget) {
            _lineWidget->setIsShow(false);
            delete _lineWidget;
            _lineWidget = nullptr;
            EventOper::GraphEventOperator* graphOper = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("OperGraphPreprocess");
            if (graphOper) {
                graphOper->reRender();
            }
        }
        GUIDialogBase::closeEvent(event);

    }

    void StreamLineDialog::on_pushButton_OK_clicked()
    {
        if (_isCreate) {
            if (!OKCreate())return;
        }
        else {
            if (!OKEdit())return;
        }

        if (_oper) {
            _oper->setArgs(objID_String, _currentDataID);
            _oper->execProfession();
        }

        this->close();
    }

    void StreamLineDialog::on_pushButton_Cancel_clicked()
    {
        this->close();
    }

    void StreamLineDialog::slotValueChange(double * startPoint, double * endPoint)
    {
        Core::FITKPoint point1(startPoint[0], startPoint[1], startPoint[2]);
        Core::FITKPoint point2(endPoint[0], endPoint[1], endPoint[2]);
        double length = Core::Distance(point1, point2);

        //设置长度
        _ui->lineEdit_Length->setText(QString::number(length));
        //设置初始点
        _ui->lineEdit_Point1X->setText(QString::number(startPoint[0]));
        _ui->lineEdit_Point1Y->setText(QString::number(startPoint[1]));
        _ui->lineEdit_Point1Z->setText(QString::number(startPoint[2]));
        _ui->lineEdit_Point2X->setText(QString::number(endPoint[0]));
        _ui->lineEdit_Point2Y->setText(QString::number(endPoint[1]));
        _ui->lineEdit_Point2Z->setText(QString::number(endPoint[2]));
    }

    void StreamLineDialog::slotDataChange()
    {
        if (_lineWidget == nullptr)return;
        double startPoint[3] = { 0,0,0 };
        double endPoint[3] = { 0,0,0 };
        startPoint[0] = _ui->lineEdit_Point1X->value();
        startPoint[1] = _ui->lineEdit_Point1Y->value();
        startPoint[2] = _ui->lineEdit_Point1Z->value();
        endPoint[0] = _ui->lineEdit_Point2X->value();
        endPoint[1] = _ui->lineEdit_Point2Y->value();
        endPoint[2] = _ui->lineEdit_Point2Z->value();
        _lineWidget->setLine(startPoint, endPoint);
        reRender();
    }

    void StreamLineDialog::InitCreate()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        //设置名称
        QString name = _postDataManager->checkName("StreamLine-1");
        _ui->lineEdit_Name->setText(name);

        double bounds[6] = { 0,0,0,0,0,0 };
        parentData->getBounds(bounds);
        double startPoint[3] = { 0,0,0 };
        startPoint[0] = bounds[0];
        startPoint[1] = bounds[2];
        startPoint[2] = bounds[4];
        double endPoint[3] = { 0,0,0 };
        endPoint[0] = bounds[1];
        endPoint[1] = bounds[3];
        endPoint[2] = bounds[5];

        Core::FITKPoint point1(startPoint[0], startPoint[1], startPoint[2]);
        Core::FITKPoint point2(endPoint[0], endPoint[1], endPoint[2]);
        double length = Core::Distance(point1, point2);

        //设置长度
        _ui->lineEdit_Length->setText(QString::number(length));
        //设置初始点
        _ui->lineEdit_Point1X->setText(QString::number(startPoint[0]));
        _ui->lineEdit_Point1Y->setText(QString::number(startPoint[1]));
        _ui->lineEdit_Point1Z->setText(QString::number(startPoint[2]));
        _ui->lineEdit_Point2X->setText(QString::number(endPoint[0]));
        _ui->lineEdit_Point2Y->setText(QString::number(endPoint[1]));
        _ui->lineEdit_Point2Z->setText(QString::number(endPoint[2]));

        //设置精度
        _ui->lineEdit_Resolution->setText(QString::number(500));

        _lineWidget->setLine(startPoint, endPoint);
    }

    void StreamLineDialog::InitEdit()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        Interface::FITKCFDPostStreamLine* currentData = dynamic_cast<Interface::FITKCFDPostStreamLine*>(_postDataManager->getDataByID(_currentDataID));
        if (currentData == nullptr)return;

        //设置名称
        QString name = currentData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);
        _ui->lineEdit_Name->setEnabled(false);

        //设置矢量
        Interface::FITKPostFieldType vectorType = Interface::FITKPostFieldType::Post_None;
        QString vectorName = "";
        currentData->getVector(vectorType, vectorName);
        int vectorIndex = -1;
        for (int i = 0; i < _ui->comboBox_Vector->count(); i++) {
            QString currentVector = _ui->comboBox_Vector->itemText(i);
            Interface::FITKPostFieldType currentType = _ui->comboBox_Vector->itemData(i).value<Interface::FITKPostFieldType>();
            if (currentVector == vectorName && currentType == vectorType) {
                vectorIndex = i;
                break;
            }
        }
        _ui->comboBox_Vector->setCurrentIndex(vectorIndex);

        //设置长度、初始点、精度
        double startPoint[3] = { 0,0,0 };
        double endPoint[3] = { 0,0,0 };
        int resolution = 0;
        currentData->getLine(startPoint, endPoint, resolution);
        double length = currentData->getLength();
        _ui->lineEdit_Length->setText(QString::number(length));
        _ui->lineEdit_Point1X->setText(QString::number(startPoint[0]));
        _ui->lineEdit_Point1Y->setText(QString::number(startPoint[1]));
        _ui->lineEdit_Point1Z->setText(QString::number(startPoint[2]));
        _ui->lineEdit_Point2X->setText(QString::number(endPoint[0]));
        _ui->lineEdit_Point2Y->setText(QString::number(endPoint[1]));
        _ui->lineEdit_Point2Z->setText(QString::number(endPoint[2]));
        _ui->lineEdit_Resolution->setText(QString::number(resolution));

        //设置流线方向
        Interface::FITKPostStreamDirection dirType = currentData->getDirection();
        _ui->comboBox_Direction->setCurrentIndex(_ui->comboBox_Direction->findData(QVariant::fromValue(dirType)));

        //设置VTK界面
        _lineWidget->setLine(startPoint, endPoint);
    }

    bool StreamLineDialog::OKCreate()
    {
        QString name = _ui->lineEdit_Name->text();
        if (_postDataManager->getDataByName(name)) {
            QMessageBox box(QMessageBox::Warning, "", QString(tr("\"%1\" Name exists!")).arg(name), QMessageBox::Ok);
            box.exec();
            return false;
        }

        Interface::FITKCFDPostStreamLine* streamLine = new Interface::FITKCFDPostStreamLine(_parentDataID);
        getDataFromWidget(streamLine);
        _currentDataID = streamLine->getDataObjectID();
        _postDataManager->appendDataObj(streamLine);
        return true;
    }

    bool StreamLineDialog::OKEdit()
    {
        Interface::FITKCFDPostStreamLine* streamLine = dynamic_cast<Interface::FITKCFDPostStreamLine*>(_postDataManager->getDataByID(_currentDataID));
        if (streamLine == nullptr)return false;
        getDataFromWidget(streamLine);
        return true;
    }

    void StreamLineDialog::getDataFromWidget(Interface::FITKCFDPostStreamLine* streamLine)
    {
        if (streamLine == nullptr)return;
        //设置名称
        QString name = _ui->lineEdit_Name->text();
        streamLine->setDataObjectName(name);

        //设置长度
        double length = _ui->lineEdit_Length->text().toDouble();
        streamLine->setLength(length);

        //设置点和精度
        double startPoint[3] = { 0,0,0 };
        startPoint[0] = _ui->lineEdit_Point1X->text().toDouble();
        startPoint[1] = _ui->lineEdit_Point1Y->text().toDouble();
        startPoint[2] = _ui->lineEdit_Point1Z->text().toDouble();
        double endPoint[3] = { 0,0,0 };
        endPoint[0] = _ui->lineEdit_Point2X->text().toDouble();
        endPoint[1] = _ui->lineEdit_Point2Y->text().toDouble();
        endPoint[2] = _ui->lineEdit_Point2Z->text().toDouble();
        int resolution = 0;
        resolution = _ui->lineEdit_Resolution->text().toInt();
        streamLine->setLine(startPoint, endPoint, resolution);

        //设置矢量
        QString vectorName = _ui->comboBox_Vector->currentText();
        Interface::FITKPostFieldType type = _ui->comboBox_Vector->itemData(_ui->comboBox_Vector->currentIndex()).value<Interface::FITKPostFieldType>();
        streamLine->setVector(type, vectorName);

        //设置矢量方向
        Interface::FITKPostStreamDirection direction = _ui->comboBox_Direction->currentData().value<Interface::FITKPostStreamDirection>();
        streamLine->setDirection(direction);
    }

    void StreamLineDialog::setDataToWidget(Interface::FITKCFDPostStreamLine* streamLine)
    {

    }
}