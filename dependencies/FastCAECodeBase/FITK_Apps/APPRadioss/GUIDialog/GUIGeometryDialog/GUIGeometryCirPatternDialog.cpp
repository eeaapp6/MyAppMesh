/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGeometryCirPatternDialog.h"
#include "ui_GUIGeometryCirPatternDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoTransformation.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "PickDataProvider/PickedDataProvider.h"
#include "PickDataProvider/PickedData.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoPart.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossGeomPart.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "GUIFrame/MainWindow.h"
#include "GUIWidget/ControlPanel.h"
#include "GUIWidget/GUIEnumType.h"
#include "GUIWidget/TreeWidgetAssembly.h"
#include <QMessageBox>

#define _RECTANGULAR_IS_ 0
#define _RECTANGULAR_IL_ 1
#define _RECTANGULAR_SL_ 2

namespace GUI {
    GUIGeometryCirPatternDialog::GUIGeometryCirPatternDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryCirPatternDialog();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        //获取拾取器，绑定拾取结束信号
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        //清除拾取对象所有的连接
        if (pickProvider != nullptr)
        {
            connect(pickProvider, &GraphData::PickedDataProvider::sigDataPicked, this, &GUIGeometryCirPatternDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryCirPatternDialog::~GUIGeometryCirPatternDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
        if (_geometryData->getDataObjectName().isEmpty())
        {
            delete _geometryData;
            _geometryData = nullptr;
        }
        //所有的类在执行完毕后，将清空对3维的操作
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (!pickProvider) return;
        pickProvider->clearPickedData();
        ////拾取对象 关闭
        ////拾取方式 空
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBNone, GUI::GUIPickInfo::PickMethod::PMNone);
    }

    void GUIGeometryCirPatternDialog::init()
    {
        _ui->tabWidget->setTabText(0, "Axial Reference");
        _ui->tabWidget->setTabText(1, "Crown Definition");
        _ui->comboBox_First_Parameter->addItem(tr("Instances & Spacing"), _RECTANGULAR_IS_);
        _ui->comboBox_First_Parameter->addItem(tr("Instances & Length"), _RECTANGULAR_IL_);
        _ui->comboBox_First_Parameter->addItem(tr("Spacing & Length"), _RECTANGULAR_SL_);

        _ui->comboBox_Second_Parameter->addItem(tr("Instances & Spacing"), _RECTANGULAR_IS_);
        _ui->comboBox_Second_Parameter->addItem(tr("Instances & Length"), _RECTANGULAR_IL_);
        _ui->comboBox_Second_Parameter->addItem(tr("Spacing & Length"), _RECTANGULAR_SL_);

        _ui->spinBox_First_Instance->setMinimum(2);
        _ui->spinBox_Second_Instance->setMinimum(1);
        _ui->checkBox->setChecked(true);
        //获取几何管理器
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) return;
        Radioss::FITKRadiossGeomPartManager* partMgr = radiossCase->getGeomPartManager();
        if (partMgr == nullptr) return;
        // 获取主窗口
        GUI::MainWindow* mainWindow = GUI::MainWindow::GetMainWindowFromFramework();
        if (mainWindow == nullptr) return;
        // 获取控件面板
        GUI::ControlPanel* controlPanel = mainWindow->getControlPanel();
        if (controlPanel == nullptr) return;
        // 获取装配树
        GUI::TreeWidgetAssembly* assemblyTreeWidget = controlPanel->getAssemblyTree();
        if (assemblyTreeWidget == nullptr) return;
        QTreeWidgetItem* item = assemblyTreeWidget->currentItem();
        if (item && item->type() == int(TreeWidgetAssemblyType::TWAssemblyGeometryChild))
        {
            int id = item->data(1, 0).toInt();
            Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
            if (cmdList == nullptr) return;
            _geometryData = dynamic_cast<Interface::FITKAbsGeoPart*>(cmdList->getDataByID(id));
        }
        else
        {
            Radioss::FITKRadiossGeomPart* part = new Radioss::FITKRadiossGeomPart();
            partMgr->appendDataObj(part);
            _geometryData = part->getGeoPart();
        }
        if (_geometryData == nullptr) return;
        QString name = _geometryData->checkName(tr("Pattern-1"));
        _ui->lineEdit_Name->setText(name);
        setWindowTitle(tr("Create Circular Pattern"));
    }

    void GUIGeometryCirPatternDialog::pickFinishedOper()
    {
        //获取拾取数据
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (!pickProvider) return;
        QList<GraphData::PickedData*> pickList = pickProvider->getPickedList();
        QList<Interface::VirtualShape> virtualShape;
        for (GraphData::PickedData* pickData : pickList) {
            if (pickData == nullptr)continue;
            int cmdID = pickData->getPickedDataObjId();
            QHash<int, int>OtherIDs = pickData->getPickedOtherIDs();//key PickedId  value index
            for (GraphData::PickDataInfo i : pickData->getPickOtherInfos())
            {
                if (OtherIDs.contains(i._id) == true)
                {
                    //添加拾取数据
                    Interface::VirtualShape temp(cmdID, i._id, OtherIDs.value(i._id));
                    virtualShape.push_back(temp);
                }
            }
        }
        if (_pickFlag == PickFlag::Target)
        {
            if (virtualShape.size() == 0)
            {
                _virtualTarget.reset();
                _ui->label_Num->setText(tr("Picked (0)"));
            }
            else
            {
                _virtualTarget = virtualShape.at(0);
                _virtualTarget.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
                _ui->label_Num->setText(tr("Picked (1)"));
            }
        }
        else if (_pickFlag == PickFlag::AxisDirection)
        {
            if (virtualShape.size() == 0)
            {
                _virtualAxis.reset();
                _ui->label_Num_2->setText(tr("Picked (0)"));
            }
            else
            {
                _virtualAxis = virtualShape.at(0);
                _virtualAxis.Type = Interface::FITKGeoEnum::VTopoShapeType::VSEdge;
                _ui->label_Num_2->setText(tr("Picked (1)"));
            }
        }
    }

    void GUIGeometryCirPatternDialog::getDataFormWidget()
    {
        int firstInstances = 0;
        double firstSpacing = 0;
        double firstLength = 0;
        int secondInstances = 0;
        double secondSpacing = 0;
        double secondLength = 0;
        Interface::FITKAbsGeoModelCircularPattern *cirPatternOper = dynamic_cast<Interface::FITKAbsGeoModelCircularPattern *>(_obj);
        if (cirPatternOper == nullptr) return;
        cirPatternOper->setSourceShape(_virtualTarget);
        cirPatternOper->setAxis(_virtualAxis);
        if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IS_)
        {
            cirPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndSpacing);
            firstInstances = _ui->spinBox_First_Instance->text().toInt();
            firstSpacing = _ui->lineEdit_First_Spacing->text().toDouble();
            firstLength = firstInstances * firstSpacing;
        }
        else if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_IL_)
        {
            cirPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndTotal);
            firstInstances = _ui->spinBox_First_Instance->text().toInt();
            firstLength = _ui->lineEdit_First_Length->text().toDouble();
            firstSpacing = firstLength / firstInstances;
        }
        else if (_ui->comboBox_First_Parameter->currentData().toInt() == _RECTANGULAR_SL_)
        {
            cirPatternOper->setFirstDefinition(Interface::FITKAbsGeoModelPattern::Definition::SpacingAndTotal);
            firstSpacing = _ui->lineEdit_First_Spacing->text().toDouble();
            firstLength = _ui->lineEdit_First_Length->text().toDouble();
            firstInstances = (firstLength / firstSpacing) / 1;
        }
        if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IS_)
        {
            cirPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndSpacing);
            secondInstances = _ui->spinBox_Second_Instance->text().toInt();
            secondSpacing = _ui->lineEdit_Second_Spacing->text().toDouble();
            secondLength = secondInstances * secondSpacing;
        }
        else if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_IL_)
        {
            cirPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::InstanceAndTotal);
            secondInstances = _ui->spinBox_Second_Instance->text().toInt();
            secondLength = _ui->lineEdit_Second_Length->text().toDouble();
            secondSpacing = secondLength / secondInstances;
        }
        else if (_ui->comboBox_Second_Parameter->currentData().toInt() == _RECTANGULAR_SL_)
        {
            cirPatternOper->setSecondDefinition(Interface::FITKAbsGeoModelPattern::Definition::SpacingAndTotal);
            secondLength = _ui->lineEdit_Second_Length->text().toDouble();
            secondSpacing = _ui->lineEdit_Second_Spacing->text().toDouble();
            secondInstances = (secondLength / secondSpacing) / 1;
        }
        cirPatternOper->setAxisCount(firstInstances);
        cirPatternOper->setAxisSpacing(firstSpacing);
        cirPatternOper->setAxisTotal(firstLength);
        cirPatternOper->setCrownCount(secondInstances);
        cirPatternOper->setCrownSpacing(secondSpacing);
        cirPatternOper->setCrownTotal(secondLength);
        if (_ui->checkBox->isChecked())
            cirPatternOper->setCopy(false);
        else
        {
            cirPatternOper->addReferenceCmdID(_virtualTarget.CmdId);
            cirPatternOper->setCopy(true);
        }
    }

    void GUIGeometryCirPatternDialog::on_pushButton_OK_clicked()
    {
        if (!_geometryData) return ;
        //获取数据名称
        QString name = _ui->lineEdit_Name->text();
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "", tr("Please set the model name.").arg(name), QMessageBox::Ok);
            return;
        }
        if (_geometryData->getDataByName(name)) {
            QMessageBox::warning(this, "", tr("\"%1\" already exists and cannot be overwritten.").arg(name), QMessageBox::Ok);
            return ;
        }
        if (_virtualTarget.isNull())
        {
            QMessageBox::warning(this, "", tr("Pick up the target model first.").arg(name), QMessageBox::Ok);
            return;
        }
        if (_virtualAxis.isNull())
        {
            QMessageBox::warning(this, "", tr("Pick up the axis first.").arg(name), QMessageBox::Ok);
            return;
        }
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
         _obj = geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircularPattern);
         if (!_obj) return;
        this->getDataFormWidget();
        int objID = _geometryData->getDataObjectID();
        _obj->setDataObjectName(name);
        if (_geometryData->getDataObjectName().isEmpty())
        {
            Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
            if (cmdList == nullptr) return;
            _geometryData->setDataObjectName(cmdList->checkName(name));
        }
        if (_geometryData->addAndUpdate(_obj) == false)
        {
            //创建失败不将该数据append
            QMessageBox::warning(this, "", tr("The geometry could not be created properly").arg(_ui->lineEdit_Name->text()), QMessageBox::Ok);
            return;
        }
        _oper->setArgs("objID", objID);
        _oper->execProfession();
        this->accept();
    }

    void GUIGeometryCirPatternDialog::on_pushButton_Cancel_clicked()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        this->reject();
    }

    void GUIGeometryCirPatternDialog::on_Target_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Target;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoSolid, GUI::GUIPickInfo::PickMethod::PMSingle);
    }

    void GUIGeometryCirPatternDialog::on_Direction_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::AxisDirection;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoEdge, GUI::GUIPickInfo::PickMethod::PMSingle);
    }
    void GUIGeometryCirPatternDialog::on_comboBox_First_Parameter_currentIndexChanged(int index)
    {
        if (index == _RECTANGULAR_IS_)
        {
            _ui->spinBox_First_Instance->setEnabled(true);
            _ui->lineEdit_First_Spacing->setEnabled(true);
            _ui->lineEdit_First_Length->setEnabled(false);
        }
        else if (index == _RECTANGULAR_IL_)
        {
            _ui->spinBox_First_Instance->setEnabled(true);
            _ui->lineEdit_First_Spacing->setEnabled(false);
            _ui->lineEdit_First_Length->setEnabled(true);
        }
        else if (index == _RECTANGULAR_SL_)
        {
            _ui->spinBox_First_Instance->setEnabled(false);
            _ui->lineEdit_First_Spacing->setEnabled(true);
            _ui->lineEdit_First_Length->setEnabled(true);
        }
    }
    void GUIGeometryCirPatternDialog::on_comboBox_Second_Parameter_currentIndexChanged(int index)
    {
        if (index == _RECTANGULAR_IS_)
        {
            _ui->spinBox_Second_Instance->setEnabled(true);
            _ui->lineEdit_Second_Spacing->setEnabled(true);
            _ui->lineEdit_Second_Length->setEnabled(false);
        }
        else if (index == _RECTANGULAR_IL_)
        {
            _ui->spinBox_Second_Instance->setEnabled(true);
            _ui->lineEdit_Second_Spacing->setEnabled(false);
            _ui->lineEdit_Second_Length->setEnabled(true);
        }
        else if (index == _RECTANGULAR_SL_)
        {
            _ui->spinBox_Second_Instance->setEnabled(false);
            _ui->lineEdit_Second_Spacing->setEnabled(true);
            _ui->lineEdit_Second_Length->setEnabled(true);
        }
    }
    void GUIGeometryCirPatternDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }
}