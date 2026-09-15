/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGeometryBoolDialog.h"
#include "ui_GUIGeometryBoolDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"
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

namespace GUI {
    GUIGeometryBoolDialog::GUIGeometryBoolDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryBoolDialog();
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
            connect(pickProvider, &GraphData::PickedDataProvider::sigDataPicked, this, &GUIGeometryBoolDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryBoolDialog::~GUIGeometryBoolDialog()
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

    void GUIGeometryBoolDialog::init()
    {
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
        _ui->comboBox_type->addItem(tr("Unite"), Interface::FITKAbsGeoOperBool::GBTAdd);
        _ui->comboBox_type->addItem(tr("Intersect"), Interface::FITKAbsGeoOperBool::GBTCommon);
        _ui->comboBox_type->addItem(tr("Difference"), Interface::FITKAbsGeoOperBool::GBTCut);
        
        QString name = _geometryData->checkName(tr("Unite-1"));
        _ui->lineEdit_Name->setText(name);
        setWindowTitle(tr("Create Bool"));
    }

    void GUIGeometryBoolDialog::pickFinishedOper()
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
                _ui->label_Num->setText(tr("Picked (0)"));
                _virtualTarget.reset();
            }
            else
            {
                _virtualTarget = virtualShape.at(0);
                _virtualTarget.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
                _ui->label_Num->setText(tr("Picked (1)"));
            }
        }
        else if (_pickFlag == PickFlag::Tool)
        {
            if (virtualShape.size() == 0)
            {
                _ui->label_Num_2->setText(tr("Picked (0)"));
                _virtualTool.reset();
            }
            else
            {
                _virtualTool = virtualShape.at(0);
                _virtualTool.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
                _ui->label_Num_2->setText(tr("Picked (1)"));
            }
        }
    }

    void GUIGeometryBoolDialog::on_pushButton_OK_clicked()
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
        if (_virtualTool.isNull())
        {
            QMessageBox::warning(this, "", tr("Pick up the tool model first.").arg(name), QMessageBox::Ok);
            return;
        }
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        Interface::FITKAbsGeoOperBool* boolOper = dynamic_cast<Interface::FITKAbsGeoOperBool *>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool));
        if (boolOper == nullptr) return;
        boolOper->setTarget(_virtualTarget);
        boolOper->setTool(_virtualTool);
        boolOper->setBoolOperType(Interface::FITKAbsGeoOperBool::GeoBoolOperType(_ui->comboBox_type->currentData().toInt()));
        boolOper->clearReference();
        boolOper->addReferenceCmdID(_virtualTarget.CmdId);
        if (_virtualTarget.CmdId != _virtualTool.CmdId)
            boolOper->addReferenceCmdID(_virtualTool.CmdId);
        int objID = _geometryData->getDataObjectID();
        boolOper->setDataObjectName(name);
        if (_geometryData->getDataObjectName().isEmpty())
        {
            Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
            if (cmdList == nullptr) return;
            _geometryData->setDataObjectName(cmdList->checkName(name));
        }
        if (_geometryData->addAndUpdate(boolOper) == false)
        {
            //创建失败不将该数据append
            QMessageBox::warning(this, "", tr("The geometry could not be created properly").arg(_ui->lineEdit_Name->text()), QMessageBox::Ok);
            return;
        }
        _oper->setArgs("objID", objID);
        _oper->execProfession();
        this->accept();
    }

    void GUIGeometryBoolDialog::on_pushButton_Cancel_clicked()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        this->reject();
    }
    void GUIGeometryBoolDialog::on_Target_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Target;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoSolid, GUI::GUIPickInfo::PickMethod::PMSingle);
    }

    void GUIGeometryBoolDialog::on_Tool_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Tool;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoSolid, GUI::GUIPickInfo::PickMethod::PMSingle);
    }
    void GUIGeometryBoolDialog::on_comboBox_type_currentIndexChanged(int index)
    {
        QString name = {};
        _ui->lineEdit_Name->setText(name);

        if (index == 0)
            name = _geometryData->checkName(tr("Unite-1"));
        else if (index == 1)
            name = _geometryData->checkName(tr("Intersect-1"));
        else if (index == 2)
            name = _geometryData->checkName(tr("Difference-1"));

        _ui->lineEdit_Name->setText(name);

    }
    void GUIGeometryBoolDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }
}