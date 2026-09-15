/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GlyphDialog.h"
#include "ui_GlyphDialog.h"

#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostGlyph.h"

#include <QMessageBox>

Q_DECLARE_METATYPE(Interface::FITKPostFieldType);

namespace GUI
{
    GlyphDialog::GlyphDialog(int dataID, EventOper::ParaWidgetInterfaceOperator* oper, bool isCreate) :
        GUIDialogBase(oper,"GlyphDialog", FITKAPP->getGlobalData()->getMainWindow()),
        _isCreate(isCreate)
    {
        _ui = new Ui::GlyphDialog();
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

    GlyphDialog::~GlyphDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
    }

    void GlyphDialog::init()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;
        
        //添加矢量
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

        if (_isCreate) {
            InitCreate();
            setWindowTitle(tr("Create Glyph"));
        }
        else {
            InitEdit();
            setWindowTitle(tr("Edit Glyph"));
        }
    }

    void GlyphDialog::showEvent(QShowEvent * event)
    {
        GUIDialogBase::showEvent(event);
    }

    void GlyphDialog::closeEvent(QCloseEvent * event)
    {
        GUIDialogBase::closeEvent(event);
    }

    void GlyphDialog::on_pushButton_OK_clicked()
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

    void GlyphDialog::on_pushButton_Cancel_clicked()
    {
        this->close();
    }

    void GlyphDialog::InitCreate()
    {
        Interface::FITKAbstractCFDPostData* parentData = _postDataManager->getDataByID(_parentDataID);
        if (parentData == nullptr)return;

        QString name = _postDataManager->checkName("Glyph-0");
        _ui->lineEdit_Name->setText(name);

        int pointNum = parentData->getPointNumber();
        _ui->spinBox_PointNum->setValue(pointNum);
    }

    void GlyphDialog::InitEdit()
    {
        if (_currentDataID <= 0)return;
        Interface::FITKAbstractCFDPostData* absCurData = _postDataManager->getDataByID(_currentDataID);
        Interface::FITKCFDPostGlyph* currentData = dynamic_cast<Interface::FITKCFDPostGlyph*>(absCurData);
        if (currentData == nullptr)return;
        QString name = currentData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);

        setDataToWidget(currentData);
        _ui->lineEdit_Name->setEnabled(false);
    }

    bool GlyphDialog::OKCreate()
    {
        QString name = _ui->lineEdit_Name->text();
        if (_postDataManager->getDataByName(name)) {
            QMessageBox box(QMessageBox::Warning, "", QString(tr("\"%1\" Name exists!")).arg(name), QMessageBox::Ok);
            box.exec();
            return false;
        }

        Interface::FITKCFDPostGlyph* glyphData = new Interface::FITKCFDPostGlyph(_parentDataID);
        getDataFromWidget(glyphData);
        _currentDataID = glyphData->getDataObjectID();
        glyphData->update();
        _postDataManager->appendDataObj(glyphData);
        return true;
    }
    bool GlyphDialog::OKEdit()
    {
        Interface::FITKCFDPostGlyph* glyphData = dynamic_cast<Interface::FITKCFDPostGlyph*>(_postDataManager->getDataByID(_currentDataID));
        if (glyphData == nullptr)return false;
        getDataFromWidget(glyphData);
        glyphData->update();
        return true;
    }

    void GlyphDialog::getDataFromWidget(Interface::FITKCFDPostGlyph * glyphData)
    {
        if (glyphData == nullptr)return;
        QString name = _ui->lineEdit_Name->text();
        glyphData->setDataObjectName(name);

        QString vector = _ui->comboBox_Vector->currentText();
        glyphData->setVectorName(vector);

        Interface::FITKPostFieldType vectorType = _ui->comboBox_Vector->currentData().value<Interface::FITKPostFieldType>();
        switch (vectorType){
        case Interface::FITKPostFieldType::Post_Point:glyphData->setPointDataUsed(true); break;
        case Interface::FITKPostFieldType::Post_Cell:glyphData->setPointDataUsed(false); break;
        }

        //箭头精度
        int tipResolution = _ui->spinBox_TipRes->value();
        glyphData->setTipResolution(tipResolution);

        //箭头半径
        double tipRadius = _ui->lineEdit_TipRadius->text().toDouble();
        glyphData->setTipRadius(tipRadius);
       
        //箭头长度
        double tipLength = _ui->lineEdit_TipLength->text().toDouble();
        glyphData->setTipLength(tipLength);

        //箭柄维度
        int shaftResolution = _ui->spinBox_ShaftRes->value();
        glyphData->setShaftResolution(shaftResolution);

        //箭柄半径
        double shaftRadius = _ui->lineEdit_ShatfRadius->text().toDouble();
        glyphData->setShaftRadius(shaftRadius);

        //缩放比例
        double scaleFactor = _ui->lineEdit_ScaleFactor->text().toDouble();
        glyphData->setScaleFactor(scaleFactor);

        //点的数量
        int pointNum = _ui->spinBox_PointNum->value();
        glyphData->setPointNum(pointNum);

        //最大采样点
        int maxNumber = _ui->spinBox_MaxPoint->value();
        glyphData->setMaxNumber(maxNumber);

        //比例模式
        int scaleMode = _ui->comboBox_ScaleMode->currentIndex();
        glyphData->setScaleMode(scaleMode);
    }

    void GlyphDialog::setDataToWidget(Interface::FITKCFDPostGlyph * glyphData)
    {
        if (glyphData == nullptr)return;
        QString name = glyphData->getDataObjectName();
        _ui->lineEdit_Name->setText(name);

        //设置当前矢量
        QString vector = glyphData->getVectorName();
        Interface::FITKPostFieldType type;
        bool isPointDataUsed = glyphData->getPointDataUsed();
        if (isPointDataUsed) type = Interface::FITKPostFieldType::Post_Point;
        else type = Interface::FITKPostFieldType::Post_Cell;
        int vectorIndex = -1;
        for (int i = 0; i < _ui->comboBox_Vector->count(); i++) {
            QString currentVector = _ui->comboBox_Vector->itemText(i);
            Interface::FITKPostFieldType currentType = _ui->comboBox_Vector->itemData(i).value<Interface::FITKPostFieldType>();
            if (currentVector == vector && currentType == type) {
                vectorIndex = i;
                break;
            }
        }
        _ui->comboBox_Vector->setCurrentIndex(vectorIndex);

        //箭头精度
        int tipResolution = glyphData->getTipResolution();
        _ui->spinBox_TipRes->setValue(tipResolution);

        //箭头半径
        double tipRadius = glyphData->getTipRadius();
        _ui->lineEdit_TipRadius->setValue(tipRadius);

        //箭头长度
        double tipLength = glyphData->getTipLength();
        _ui->lineEdit_TipLength->setValue(tipLength);

        //箭柄维度
        int shaftResolution = glyphData->getShaftResolution();
        _ui->spinBox_ShaftRes->setValue(shaftResolution);

        //箭柄半径
        double shaftRadius = glyphData->getShaftRadius();
        _ui->lineEdit_ShatfRadius->setValue(shaftRadius); 

        //缩放比例
        double scaleFactor = glyphData->getScaleFactor();
        _ui->lineEdit_ScaleFactor->setValue(scaleFactor);

        //点的数量
        int pointNum = glyphData->getPointNum(); 
        _ui->spinBox_PointNum->setValue(pointNum);

        //最大采样点
        int maxNumber = glyphData->getMaxNumber(); 
        _ui->spinBox_MaxPoint->setValue(maxNumber);

        //比例模式
        int scaleMode = glyphData->getScaleMode(); 
        _ui->comboBox_ScaleMode->setCurrentIndex(scaleMode);
    }
}

