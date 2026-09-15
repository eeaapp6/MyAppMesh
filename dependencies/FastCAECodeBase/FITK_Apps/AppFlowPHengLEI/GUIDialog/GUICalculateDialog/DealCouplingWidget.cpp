/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "DealCouplingWidget.h"
#include "ui_DealCouplingWidget.h"

#include "GUICalculateWidgetBase.h"
#include "GUIFrame/MainWindow.h"
#include "GUIFrame/PropertyWidget.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKWorkBenchHandler.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Component/FITKOFDriver/FITKOFInputInfo.h"
//#include "FITK_Component/FITKOFDictWriter/FITKOFDictWriterIO.h"


#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEICouplingControl.h"


#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <vector>
#include <QDir>
#include <QFile>



#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>




namespace GUI
{
    DealCouplingWidget::DealCouplingWidget(EventOper::ParaWidgetInterfaceOperator* oper, QWidget* parent) :
		GUICalculateWidgetBase(oper, parent)
    {
        _ui = new Ui::DealCouplingWidget;
        _ui->setupUi(this);
		if (_data_PHengLEI == nullptr) return;
		_couplingControl = _data_PHengLEI->getCouplingData();
		if (_couplingControl == nullptr) return;

        initWidget();
    }

    DealCouplingWidget::~DealCouplingWidget()
    {
        if (_ui)delete _ui;
    }

    void DealCouplingWidget::initWidget()
    {
		

        _ui->FSI_radioButton->setChecked(_couplingControl->isCouplingEnabled());
        //_ui->FSI_radioButton->setEnabled(false);

        _ui->lineEdit_participant->setText(_couplingControl->getParticipantName());
        _ui->lineEdit_Interface->setText(_couplingControl->getCouplingInterfaceName());


		_ui->lineEdit_Mesh_Center->setText(_couplingControl->getCouplingMeshName_1());
		_ui->lineEdit_Mesh_Type1->setText(_couplingControl->getCouplingMeshType_1());
		_ui->lineEdit_WriteData->setText(_couplingControl->getWriteDataName());

		_ui->lineEdit_Mesh_Node->setText(_couplingControl->getCouplingMeshName_2());
		_ui->lineEdit_Mesh_Type2->setText(_couplingControl->getCouplingMeshType_2());
        _ui->lineEdit_ReadData->setText(_couplingControl->getReadDataName());



        _ui->lineEdit_participant->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Interface->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Center->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Type1->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_WriteData->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Node->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Type2->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_ReadData->setEnabled(_couplingControl->isCouplingEnabled());


        // 获取当前界面中所有 QRadioButton 控件
        QList<QRadioButton *> radioButtons = findChildren<QRadioButton *>();
        // 为每个 QRadioButton 设置文本变动的槽函数
        for (QRadioButton *radioButton : radioButtons) {
            connect(radioButton, SIGNAL(clicked()), this, SLOT(onRadioButtonChanged()));
        }
		
		// 获取当前界面中所有 QPushButton 控件
		QList<QPushButton *> saveButton = findChildren<QPushButton *>();
		// 为每个 QPushButton 设置保存的槽函数
		for (QPushButton *saveButton : saveButton) {
			connect(saveButton, SIGNAL(clicked()), this, SLOT(onSaveButton()));
		}

		connect(_ui->lineEdit_participant, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);
		connect(_ui->lineEdit_Interface, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);
		connect(_ui->lineEdit_Mesh_Center, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);
		connect(_ui->lineEdit_Mesh_Type1, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);
		connect(_ui->lineEdit_WriteData, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);
		connect(_ui->lineEdit_Mesh_Node, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);
		connect(_ui->lineEdit_Mesh_Type2, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);
		connect(_ui->lineEdit_ReadData, &QLineEdit::editingFinished, this, &DealCouplingWidget::onLineEditChanged);

		updateWidget();

    }

    void DealCouplingWidget::onLineEditChanged()
    {
		// 检查核心数据结构指针是否有效
		if (_couplingControl == nullptr) return;

		// 获取发送信号的对象（即被修改的那个 QLineEdit）
		QObject* obj = sender();
		if (obj == nullptr) return; // 如果获取不到发送者，则直接返回

		// 将 QObject* 转换为 QLineEdit*
		QLineEdit* lineEdit = qobject_cast<QLineEdit*>(obj);
		if (lineEdit == nullptr) return; // 如果转换失败，则直接返回

		// 获取输入框的当前文本
		QString text = lineEdit->text();

		// 判断是哪个输入框，并调用对应的 setter 方法更新数据结构
		if (lineEdit == _ui->lineEdit_participant) {
			_couplingControl->setParticipantName(text);
		}
		else if (lineEdit == _ui->lineEdit_Interface) {
			_couplingControl->setCouplingInterfaceName(text);
		}
		else if (lineEdit == _ui->lineEdit_Mesh_Center) {
			_couplingControl->setCouplingMeshName_1(text);
		}
		else if (lineEdit == _ui->lineEdit_Mesh_Type1) {
			_couplingControl->setCouplingMeshType_1(text);
		}
		else if (lineEdit == _ui->lineEdit_WriteData) {
			_couplingControl->setWriteDataName(text);
		}
		else if (lineEdit == _ui->lineEdit_Mesh_Node) {
			_couplingControl->setCouplingMeshName_2(text);
		}
		else if (lineEdit == _ui->lineEdit_Mesh_Type2) {
			_couplingControl->setCouplingMeshType_2(text);
		}
		else if (lineEdit == _ui->lineEdit_ReadData) {
			_couplingControl->setReadDataName(text);
		}


    }


    void DealCouplingWidget::onRadioButtonChanged()
    {
     
		_couplingControl->setCouplingEnabled(_ui->FSI_radioButton->isChecked());
		

        updateWidget();
    }

    void DealCouplingWidget::updateWidget()
    {

		

		_ui->FSI_radioButton->setChecked(_couplingControl->isCouplingEnabled());
		//_ui->FSI_radioButton->setEnabled(false);

		_ui->lineEdit_participant->setText(_couplingControl->getParticipantName());
		_ui->lineEdit_Interface->setText(_couplingControl->getCouplingInterfaceName());


		_ui->lineEdit_Mesh_Center->setText(_couplingControl->getCouplingMeshName_1());
		_ui->lineEdit_Mesh_Type1->setText(_couplingControl->getCouplingMeshType_1());
		_ui->lineEdit_WriteData->setText(_couplingControl->getWriteDataName());

		_ui->lineEdit_Mesh_Node->setText(_couplingControl->getCouplingMeshName_2());
		_ui->lineEdit_Mesh_Type2->setText(_couplingControl->getCouplingMeshType_2());
		_ui->lineEdit_ReadData->setText(_couplingControl->getReadDataName());



		_ui->lineEdit_participant->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Interface->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Center->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Type1->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_WriteData->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Node->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_Mesh_Type2->setEnabled(_couplingControl->isCouplingEnabled());
		_ui->lineEdit_ReadData->setEnabled(_couplingControl->isCouplingEnabled());


		QList<QPushButton *> pushButtons = this->findChildren<QPushButton *>();

		// 遍历所有找到的按钮，并设置它们的可用性
		for (QPushButton *button : pushButtons) {
			button->setEnabled(_couplingControl->isCouplingEnabled());
		}
    }

	void DealCouplingWidget::onSaveButton()
	{
		if (_couplingControl == nullptr) return;

		// 1. 从UI控件同步最新数据到 _couplingCon
		_couplingControl->setCouplingEnabled(_ui->FSI_radioButton->isChecked());
		_couplingControl->setParticipantName(_ui->lineEdit_participant->text());

	
		_couplingControl->setCouplingInterfaceName(_ui->lineEdit_Interface->text());
	

		_couplingControl->setCouplingMeshName_1(_ui->lineEdit_Mesh_Center->text());
		_couplingControl->setCouplingMeshType_1(_ui->lineEdit_Mesh_Type1->text());
		_couplingControl->setWriteDataName(_ui->lineEdit_WriteData->text());

		_couplingControl->setCouplingMeshName_2(_ui->lineEdit_Mesh_Node->text());
		_couplingControl->setCouplingMeshType_2(_ui->lineEdit_Mesh_Type2->text());
		_couplingControl->setReadDataName(_ui->lineEdit_ReadData->text());

		// --- 创建JSON对象结构 ---

		// 2. 创建根对象
		QJsonObject rootObject;
		rootObject["couplingOn"] = _couplingControl->isCouplingEnabled();
		rootObject["participant"] = _couplingControl->getParticipantName();
		rootObject["missionType"] = "incompressible";
		rootObject["dimension"] = 3;

		// 3. 创建 "interfaces" 数组
		QJsonArray interfacesArray;

		// 3.1 创建第一个接口对象
		QJsonObject interface1;
		interface1["mesh"] = _couplingControl->getCouplingMeshName_1();
		// 使用第一个接口的patch名称
		interface1["patch"] = _couplingControl->getCouplingInterfaceName();
		interface1["locations"] = _couplingControl->getCouplingMeshType_1();
		interface1["readData"] = QJsonArray(); // readData 为空数组
		QJsonArray writeDataArray1;
		writeDataArray1.append(_couplingControl->getWriteDataName());
		interface1["writeData"] = writeDataArray1;
		interfacesArray.append(interface1);

		// 3.2 创建第二个接口对象
		QJsonObject interface2;
		interface2["mesh"] = _couplingControl->getCouplingMeshName_2();		
		interface2["patch"] = _couplingControl->getCouplingInterfaceName(); 
		interface2["locations"] = _couplingControl->getCouplingMeshType_2();
		QJsonArray readDataArray2;
		readDataArray2.append(_couplingControl->getReadDataName());
		interface2["readData"] = readDataArray2;
		interface2["writeData"] = QJsonArray(); // writeData 为空数组
		interfacesArray.append(interface2);

		// 3.3 将 "interfaces" 数组设置到根对象中
		rootObject["interfaces"] = interfacesArray;

		// --- 文件写入操作 ---

		// 4. 获取工作路径并准备文件路径
		QString workDir = "";
		if (FITKAPP->getAppSettings()) {
			workDir = FITKAPP->getAppSettings()->getWorkingDir();
		}
		if (workDir.isEmpty()) {
			workDir = QApplication::applicationDirPath() + "/../WorkDir";
		}
		QString caseDir = workDir + "/case/PHengLEI";
		QString filePath = caseDir + "/PHengLEIparam.json"; // 更改文件名为 .json

		// 5. 创建目录（如果不存在）
		QDir dir(caseDir);
		if (!dir.exists()) {
			if (!dir.mkpath(".")) {
				qDebug() << "错误: 无法创建目录" << caseDir;
				QMessageBox::critical(this, "文件错误", "创建目录失败: " + caseDir);
				return;
			}
		}

		// 6. 打开文件
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			qDebug() << "错误: 无法打开文件进行写入:" << filePath;
			QMessageBox::critical(this, "文件错误", "无法打开文件进行写入: \n" + filePath);
			return;
		}

		// 7. 将JSON对象写入文件
		QJsonDocument jsonDoc(rootObject);
		file.write(jsonDoc.toJson(QJsonDocument::Indented)); 

		// 8. 关闭文件并给出成功提示
		file.close();
		QMessageBox::information(this, "successful", "outputfile: \n" + filePath);
	}

}


