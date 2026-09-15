/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRegionMeshSizeCylinderDialog.h"
#include "ui_GUIRegionMeshSizeCylinderDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeCylinder.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
namespace GUI {
    GUIRegionMeshSizeCylinderDialog::GUIRegionMeshSizeCylinderDialog(QWidget* parent)
        :Core::FITKDialog(parent)
    {
        _ui = new Ui::GUIRegionMeshSizeCylinderDialog();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        connect(FITKAPP->getSignalTransfer(), &AppFrame::FITKSignalTransfer::graphObjectPickedSig, this, &GUIRegionMeshSizeCylinderDialog::pickFinishedOper);
        this->init();
    }

    GUIRegionMeshSizeCylinderDialog::~GUIRegionMeshSizeCylinderDialog()
    {
        if (_ui) {
            delete _ui;
            _ui = nullptr;
        }
        //关闭拾取
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        emit signalTransfer->setPickableObjTypeSig(-1);
    }

    void GUIRegionMeshSizeCylinderDialog::init()
    { 
        setWindowTitle(tr("Region Cylinder"));
    }

    void GUIRegionMeshSizeCylinderDialog::pickFinishedOper(int pickType, int objectId, QHash<QString, void*> objInfo)
    {
        //获取坐标
        Interface::FITKInterfaceGeometryFactory*factory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (factory == nullptr) return;
        Interface::FITKAbstractGeomToolsCreator*createor = factory->getGeomToolsCreator();
        if (!createor) return;
        Interface::FITKAbstractGeomPointTool *pointTool = createor->createPointTool();
        if (!pointTool) return;
        double PointArray[3] = {};
        pointTool->getXYZ(FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(objectId), PointArray);
        _ui->lineEdit_X->setText(QString::number(PointArray[0]));
        _ui->lineEdit_Y->setText(QString::number(PointArray[1]));
        _ui->lineEdit_Z->setText(QString::number(PointArray[2]));
    }

    void GUIRegionMeshSizeCylinderDialog::on_pushButton_OK_clicked()
    {
        //获取网格尺寸生成器
        Interface::FITKAbstractMeshSizeInfoGenerator* meshSizeInfoGenerator = Interface::FITKMeshGenInterface::getInstance()->getMeshSizeGenerator("GmshExec");
        if (meshSizeInfoGenerator == nullptr) return;
        //生成局部网格
        Interface::FITKRegionMeshSizeCylinder* regionMeshSize = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(meshSizeInfoGenerator->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType::RegionCylinder));
        if (regionMeshSize == nullptr) return;
        double point[3] = { _ui->lineEdit_X->text().toDouble(), _ui->lineEdit_Y->text().toDouble(), _ui->lineEdit_Z->text().toDouble() };
        double direction[3] = { _ui->lineEdit_X_2->text().toDouble() , _ui->lineEdit_Y_2->text().toDouble() , _ui->lineEdit_Z_2->text().toDouble() };
        regionMeshSize->setLocation(point);
        regionMeshSize->setDirection(direction);
        regionMeshSize->setRadius(_ui->lineEdit_Radius->text().toDouble());
        regionMeshSize->setLength(_ui->lineEdit_Length->text().toDouble());
        regionMeshSize->setRadialFraction(_ui->lineEdit_RadiusFaction->text().toDouble());
        regionMeshSize->setInternalSize(_ui->lineEdit_InternalSize->text().toDouble());
        regionMeshSize->setExternalSize(_ui->lineEdit_ExternalSize->text().toDouble());
        regionMeshSize->setThickness(_ui->lineEdit_Thickness->text().toDouble());

        Interface::FITKRegionMeshSizeManager* regionMeshSizeManager = Interface::FITKMeshGenInterface::getInstance()->getRegionMeshSizeMgr("GmshExec");
        if (regionMeshSizeManager == nullptr) return;
        QString name = regionMeshSizeManager->checkName("CylinderRegion-1");
        regionMeshSize->setDataObjectName(name);
        regionMeshSizeManager->appendDataObj(regionMeshSize);
        this->accept();
    }

    void GUIRegionMeshSizeCylinderDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIRegionMeshSizeCylinderDialog::on_point1_pick_clicked()
    {
        //设置拾取方式
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        emit signalTransfer->setPickableObjTypeSig(3003);
    }

}