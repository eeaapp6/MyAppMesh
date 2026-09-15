/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIRegionMeshSizeSphereDialog.h"
#include "ui_GUIRegionMeshSizeSphereDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshSizeInfoGenerator.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeSphere.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
namespace GUI {
    GUIRegionMeshSizeSphereDialog::GUIRegionMeshSizeSphereDialog(QWidget* parent)
        :Core::FITKDialog(parent)
    {
        _ui = new Ui::GUIRegionMeshSizeSphereDialog();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        connect(FITKAPP->getSignalTransfer(), &AppFrame::FITKSignalTransfer::graphObjectPickedSig, this, &GUIRegionMeshSizeSphereDialog::pickFinishedOper);
        this->init();
    }

    GUIRegionMeshSizeSphereDialog::~GUIRegionMeshSizeSphereDialog()
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

    void GUIRegionMeshSizeSphereDialog::init()
    { 
        setWindowTitle(tr("Region Sphere"));
    }

    void GUIRegionMeshSizeSphereDialog::pickFinishedOper(int pickType, int objectId, QHash<QString, void*> objInf)
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

    void GUIRegionMeshSizeSphereDialog::on_pushButton_OK_clicked()
    {
        //获取网格尺寸生成器
        Interface::FITKAbstractMeshSizeInfoGenerator* meshSizeInfoGenerator = Interface::FITKMeshGenInterface::getInstance()->getMeshSizeGenerator("GmshExec");
        if (meshSizeInfoGenerator == nullptr) return;
        //生成局部网格
        Interface::FITKRegionMeshSizeSphere* regionMeshSize = dynamic_cast<Interface::FITKRegionMeshSizeSphere*>(meshSizeInfoGenerator->createRegionMeshSize(Interface::FITKAbstractRegionMeshSize::RegionType::RegionSphere));
        if (regionMeshSize == nullptr) return;
        double point[3] = { _ui->lineEdit_X->text().toDouble(), _ui->lineEdit_Y->text().toDouble(), _ui->lineEdit_Z->text().toDouble() };
        regionMeshSize->setLocation(point);
        regionMeshSize->setRadius(_ui->lineEdit_Radius->text().toDouble());
        regionMeshSize->setInternalSize(_ui->lineEdit_InternalSize->text().toDouble());
        regionMeshSize->setExternalSize(_ui->lineEdit_ExternalSize->text().toDouble());
        regionMeshSize->setThickness(_ui->lineEdit_Thickness->text().toDouble());

        Interface::FITKRegionMeshSizeManager* regionMeshSizeManager = Interface::FITKMeshGenInterface::getInstance()->getRegionMeshSizeMgr("GmshExec");
        if (regionMeshSizeManager == nullptr) return;
        regionMeshSizeManager->appendDataObj(regionMeshSize);
        QString name = regionMeshSizeManager->checkName("Sphere Region-1");
        regionMeshSizeManager->appendDataObj(regionMeshSize);
        this->accept();
    }

    void GUIRegionMeshSizeSphereDialog::on_pushButton_Cancel_clicked()
    {
        this->reject();
    }

    void GUIRegionMeshSizeSphereDialog::on_point1_pick_clicked()
    {
        //设置拾取方式
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        emit signalTransfer->setPickableObjTypeSig(3003);
    }
}