/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGeometryFilletDialog.h"
#include "ui_GUIGeometryFilletDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelOperSolid.h"
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
    GUIGeometryFilletDialog::GUIGeometryFilletDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryFilletDialog();
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
            connect(pickProvider, &GraphData::PickedDataProvider::sigDataPicked, this, &GUIGeometryFilletDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryFilletDialog::~GUIGeometryFilletDialog()
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

    void GUIGeometryFilletDialog::init()
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
        QString name = _geometryData->checkName(tr("Fillet-1"));
        _ui->lineEdit_Name->setText(name);
        setWindowTitle(tr("Create Fillet"));
    }

    void GUIGeometryFilletDialog::pickFinishedOper()
    {
        _edgeVirtualTopos.clear();
        //获取拾取数据
        GraphData::PickedDataProvider* pickProvider = GraphData::PickedDataProvider::getInstance();
        if (!pickProvider) return;

        QList<GraphData::PickedData*> pickList = pickProvider->getPickedList();
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
                    _edgeVirtualTopos.push_back(temp);
                }
            }
        }
        _ui->label_Num->setText(QString("Picked (%1)").arg(QString::number(_edgeVirtualTopos.size())));
    }

    void GUIGeometryFilletDialog::on_pushButton_OK_clicked()
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
        if (_edgeVirtualTopos.size() == 0)
        {
            QMessageBox::warning(this, "", tr("Pick up the edge first.").arg(name), QMessageBox::Ok);
            return;
        }
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        Interface::FITKAbsGeoModelFilletSolid* filletModel = dynamic_cast<Interface::FITKAbsGeoModelFilletSolid*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTFilletSolid));
        if (filletModel == nullptr) return;
        //设置数据
        double radius = _ui->lineEdit_Radius->text().toDouble();
        filletModel->setRadius(radius);
        filletModel->setEdgeTopos(_edgeVirtualTopos);
        filletModel->setInputCmdId(_edgeVirtualTopos.at(0).CmdId);
        filletModel->addReferenceCmdID(_edgeVirtualTopos.at(0).CmdId);
        int objID = _geometryData->getDataObjectID();
        filletModel->setDataObjectName(name);
        if (_geometryData->getDataObjectName().isEmpty())
        {
            Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
            if (cmdList == nullptr) return;
            _geometryData->setDataObjectName(cmdList->checkName(name));
        }
        if (_geometryData->addAndUpdate(filletModel) == false)
        {
            //创建失败不将该数据append
            QMessageBox::warning(this, "", tr("The geometry could not be created properly").arg(_ui->lineEdit_Name->text()), QMessageBox::Ok);
            return;
        }
        _oper->setArgs("objID", objID);
        _oper->execProfession();
        this->accept();
    }

    void GUIGeometryFilletDialog::on_pushButton_Cancel_clicked()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        this->reject();
    }

    void GUIGeometryFilletDialog::on_edge_pick_clicked()
    {
        //设置拾取方式
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoEdge, GUI::GUIPickInfo::PickMethod::PMIndividually);
    }

    void GUIGeometryFilletDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }
}