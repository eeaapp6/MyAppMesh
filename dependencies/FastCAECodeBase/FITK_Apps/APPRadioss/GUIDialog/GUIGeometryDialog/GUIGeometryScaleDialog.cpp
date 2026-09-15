/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGeometryScaleDialog.h"
#include "ui_GUIGeometryScaleDialog.h"

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

namespace GUI {
    GUIGeometryScaleDialog::GUIGeometryScaleDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :Core::FITKDialog(parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryScaleDialog();
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
            connect(pickProvider, &GraphData::PickedDataProvider::sigDataPicked, this, &GUIGeometryScaleDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryScaleDialog::~GUIGeometryScaleDialog()
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

    void GUIGeometryScaleDialog::init()
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
        QString name = _geometryData->checkName(tr("Scale-1"));
        _ui->lineEdit_Name->setText(name);
        setWindowTitle(tr("Create Scale"));
    }

    void GUIGeometryScaleDialog::pickFinishedOper()
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
                _ui->Pick_num->setText(tr("Picked (0)"));
                _virtualOriginalModel.reset();
            }
            else
            {
                _virtualOriginalModel = virtualShape.at(0);
                _virtualOriginalModel.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
                _ui->Pick_num->setText(tr("Picked (1)"));
            }
        }
        else if (_pickFlag == PickFlag::Point)
        {
            if (virtualShape.size() == 0) return;
            std::array<double, 3> xyz = this->getPointXYZ(virtualShape[0].VirtualTopoId);
            _ui->lineEdit_X->setText(QString::number(xyz.at(0)));
            _ui->lineEdit_Y->setText(QString::number(xyz.at(1)));
            _ui->lineEdit_Z->setText(QString::number(xyz.at(2)));
        }
    }

    void GUIGeometryScaleDialog::on_pushButton_OK_clicked()
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
        if (_virtualOriginalModel.isNull())
        {
            QMessageBox::warning(this, "", tr("Pick up the target model first.").arg(name), QMessageBox::Ok);
            return;
        }
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return;
        Interface::FITKAbsGeoModelScale*scaleP = dynamic_cast<Interface::FITKAbsGeoModelScale*>(geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTScale));
        if (scaleP == nullptr) return;
        scaleP->setSourceShape(_virtualOriginalModel);
        //设置基准点
        double x = _ui->lineEdit_X->text().toDouble();
        double y = _ui->lineEdit_Y->text().toDouble();
        double z = _ui->lineEdit_Z->text().toDouble();
        scaleP->setBasePoint(x, y, z);
        //设置缩放因子
        x = _ui->lineEdit_X_2->text().toDouble();
        y = _ui->lineEdit_Y_2->text().toDouble();
        z = _ui->lineEdit_Z_2->text().toDouble();
        scaleP->setFactors(x, y, z);
        if (_ui->checkBox->isChecked())
            scaleP->setCopy(false);
        else
        {
            scaleP->setCopy(true);
            scaleP->addReferenceCmdID(_virtualOriginalModel.CmdId);
        }
        int objID = _geometryData->getDataObjectID();
        scaleP->setDataObjectName(name);
        if (_geometryData->getDataObjectName().isEmpty())
        {
            Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
            if (cmdList == nullptr) return;
            _geometryData->setDataObjectName(cmdList->checkName(name));
        }
        if (_geometryData->addAndUpdate(scaleP) == false)
        {
            //创建失败不将该数据append
            QMessageBox::warning(this, "", tr("The geometry could not be created properly").arg(_ui->lineEdit_Name->text()), QMessageBox::Ok);
            return;
        }
        _oper->setArgs("objID", objID);
        _oper->execProfession();
        this->accept();
    }

    void GUIGeometryScaleDialog::on_pushButton_Cancel_clicked()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        this->reject();
    }

    void GUIGeometryScaleDialog::on_pushButton_Pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Target;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoSolid, GUI::GUIPickInfo::PickMethod::PMSingle);
    }

    void GUIGeometryScaleDialog::on_point_1_pick_clicked()
    {
        //设置拾取方式
        _pickFlag = PickFlag::Point;
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoVert, GUI::GUIPickInfo::PickMethod::PMSingle);
    }

    void GUIGeometryScaleDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }
    std::array<double, 3> GUIGeometryScaleDialog::getPointXYZ(int id)
    {
        std::array<double, 3> xyz = { -1,-1,-1 };

        Interface::FITKInterfaceGeometryFactory*factory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (factory == nullptr)
        {
            return xyz;
        }
        Interface::FITKAbstractGeomToolsCreator*createor = factory->getGeomToolsCreator();
        if (!createor)return xyz;
        Interface::FITKAbstractGeomPointTool *pointTool = createor->createPointTool();
        double PointArray[3];
        pointTool->getXYZByID(id, PointArray);
        xyz.at(0) = PointArray[0];
        xyz.at(1) = PointArray[1];
        xyz.at(2) = PointArray[2];
        delete pointTool;
        return xyz;
    }
}