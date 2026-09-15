/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGmshSettings.h"
#include "ui_GUIGmshSettings.h"
#include "FITKMesherDriverGmshExec.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"
#include <QMessageBox>

#define LOW_ORDER 1
#define HIGH_ORDER 2
#define Curve 1
#define Surface 2
#define Solid 3

namespace GUI {

    GUIGmshSettings::GUIGmshSettings(QWidget* parent/* = nullptr*/) : Core::FITKDialog(parent)
    {
        _ui = new Ui::GUIGmshSettings();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        //绑定模型拾取信号
        connect(FITKAPP->getSignalTransfer(), &AppFrame::FITKSignalTransfer::graphObjectPickedSig, this, &GUIGmshSettings::pickFinishedOper);
        //初始化界面数据
        this->init();
    }

    GUIGmshSettings::GUIGmshSettings(Gmsh::FITKMesherDriverGmshExec* driver, QWidget* parent/* = nullptr*/)
        :Core::FITKDialog(parent), _driver(driver)
    {
        _ui = new Ui::GUIGmshSettings();
        _ui->setupUi(this);
        // 设置对话框关闭时自动删除
        setAttribute(Qt::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        //绑定模型拾取信号
        connect(FITKAPP->getSignalTransfer(), &AppFrame::FITKSignalTransfer::graphObjectPickedSig, this, &GUIGmshSettings::pickFinishedOper);
        //初始化界面数据
        this->init();
    }

    GUIGmshSettings::~GUIGmshSettings()
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

    void GUIGmshSettings::isEnablePickModel(bool isEnable)
    {
        _isEnablePickModel = isEnable;
        _ui->groupBox_PickModel->setVisible(isEnable);
        this->adjustSize();
    }

    QList<Interface::VirtualShape> GUIGmshSettings::getPickModelVirtualShape()
    {
        return _virtualTopos;
    }

    void GUIGmshSettings::init()
    {
        _ui->groupBox_PickModel->setVisible(_isEnablePickModel);
        _ui->checkBox_GeometryCleanup->hide();
        setWindowTitle(tr("Gmsh Settings"));
        //隐藏表头
        _ui->tabWidget->tabBar()->hide();
        _ui->comboBox_Type->addItem(tr("Surface"), Surface);
        _ui->comboBox_Type->addItem(tr("Solid"), Solid);
 
        _ui->comboBox_Algorithm2D->addItem(tr("Automatic"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::Automatic));
        _ui->comboBox_Algorithm2D->addItem(tr("Mesh Adapt"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::MeshAdapt));
        _ui->comboBox_Algorithm2D->addItem(tr("Delaunay2D"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::Delaunay2D));
        _ui->comboBox_Algorithm2D->addItem(tr("Frontal Delaunay"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::FrontalDelaunay));
        _ui->comboBox_Algorithm2D->addItem(tr("BAMG"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::BAMG));
        _ui->comboBox_Algorithm2D->addItem(tr("Frontal Delaunay For Quads"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::FrontalDelaunayForQuads));
        _ui->comboBox_Algorithm2D->addItem(tr("Packing Of Parallelograms"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::PackingOfParallelograms));
        
        _ui->comboBox_Algorithm3D->addItem(tr("Delaunay3D"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::Delaunay3D));
        _ui->comboBox_Algorithm3D->addItem(tr("Frontal"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::Frontal));
        _ui->comboBox_Algorithm3D->addItem(tr("HXT"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::HXT));
        _ui->comboBox_Algorithm3D->addItem(tr("MMG3D"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::MMG3D));
        
        _ui->comboBox_ReAlgorithm2D->addItem(tr("Simple"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::Simple));
        _ui->comboBox_ReAlgorithm2D->addItem(tr("Blossom"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::Blossom));
        _ui->comboBox_ReAlgorithm2D->addItem(tr("Simple Full Quad"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::SimpleFullQuad));
        _ui->comboBox_ReAlgorithm2D->addItem(tr("Blossom Full Quad"), int(Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::BlossomFullQuad));
        
        _ui->comboBox_ElementOrder->addItem(tr("Low Order"), LOW_ORDER);
        _ui->comboBox_ElementOrder->addItem(tr("High Order"), HIGH_ORDER);

        //填充网格划分信息
        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo("GmshExec");
        if (meshSizeInfo == nullptr) return;

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (meshGenerateAlgorithmInfo == nullptr) return;
        //获取最大尺寸
        _ui->lineEdit_MaxSize->setText(QString::number(meshSizeInfo->getMaxSize()));
        //获取最小尺寸
        _ui->lineEdit_MinSize->setText(QString::number(meshSizeInfo->getMinSize()));
        //获取尺寸因子
        _ui->lineEdit_SizeFactor->setText(QString::number(meshSizeInfo->getSizeFactor()));
        //获取Smoothing Steps
        _ui->spinBox_SmoothingSteps->setValue(meshGenerateAlgorithmInfo->getSmoothStep());
        //获取单元阶数
        if (meshGenerateAlgorithmInfo->getElementOrder() == 1)
            _ui->comboBox_ElementOrder->setCurrentIndex(0);
        else
            _ui->comboBox_ElementOrder->setCurrentIndex(1);
        //获取重新组合所有三角形网格
        _ui->checkBox_RecombineAllTriMeshes->setChecked(meshGenerateAlgorithmInfo->getRecombineAllTriMeshes());
        //获取几何缝合
        _ui->checkBox_GeometryStitch->setChecked(meshGenerateAlgorithmInfo->getGeometryStitch());
        //获取不完整的单元
        _ui->checkBox_UseIncElem->setChecked(meshGenerateAlgorithmInfo->getUseIncElem());
        //获取2D算法
        _ui->comboBox_Algorithm2D->setCurrentIndex(meshGenerateAlgorithmInfo->get2DAlgorithm());
        //获取3D算法
        _ui->comboBox_Algorithm3D->setCurrentIndex(meshGenerateAlgorithmInfo->get3DAlgorithm());
        //获取2D重组算法
        _ui->comboBox_ReAlgorithm2D->setCurrentIndex(meshGenerateAlgorithmInfo->get2DRecombinationAlgorithm());
        //获取细化算法
        if (meshGenerateAlgorithmInfo->getSubdivisionAlgorithm() == Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::None)
        {
            _ui->radioButton_Triangle->setChecked(true);
            _ui->radioButton_Tetrahedron->setChecked(true);
            _ui->radioButton_Quads->setChecked(false);
            _ui->radioButton_Hexahedron->setChecked(false);
        }
        else if (meshGenerateAlgorithmInfo->getSubdivisionAlgorithm() == Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllQuads)
        {
            _ui->radioButton_Triangle->setChecked(false);
            _ui->radioButton_Quads->setChecked(true);
            _ui->radioButton_Tetrahedron->setChecked(true);
        }
        else if (meshGenerateAlgorithmInfo->getSubdivisionAlgorithm() == Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllHexas)
        {
            _ui->radioButton_Tetrahedron->setChecked(false);
            _ui->radioButton_Hexahedron->setChecked(true);
            _ui->radioButton_Triangle->setChecked(true);
        }
        //_ui->comboBox_SubdivisionAlgorithm->setCurrentIndex(meshGenerateAlgorithmInfo->getSubdivisionAlgorithm());
    }
    void GUIGmshSettings::pickFinishedOper(int pickType, int objectId, QHash<QString, void*> objInfo)
    {
        if (objInfo.contains("PickError"))
        {
            _virtualTopos.clear();
            _ui->label_10->setText("Picked (0)");
            return;
        }        bool pickStart = *static_cast<bool*>(objInfo["PickStart"]);
        if (pickStart)
        {
            _virtualTopos.clear();
            _ui->label_10->setText("Picked (0)");
        }
        int cmdID = *static_cast<int*>(objInfo["CmdId"]);
        Interface::FITKAbsGeoCommand* cmd = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(cmdID);
        if (!cmd) return;
        Interface::FITKShapeVirtualTopoManager* virtualTopoManager = nullptr;
        if(_ui->comboBox_Type->currentData().toInt() == Surface)
            virtualTopoManager = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        else
            virtualTopoManager = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
        int index = virtualTopoManager->getIndexByID(objectId);
        Interface::VirtualShape temp(cmdID, objectId, index);
        if (_ui->comboBox_Type->currentData().toInt() == Surface)
            temp.Type = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
        else
            temp.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
        _virtualTopos.push_back(temp);
        _ui->label_10->setText(QString("Picked (%1)").arg(QString::number(_virtualTopos.size())));
    }
    void GUIGmshSettings::on_pushButton_OK_clicked()
    {
        if (_isEnablePickModel && _virtualTopos.size() == 0)
        {
            QMessageBox::warning(this, "warning", tr("Pick up the geometric model."), QMessageBox::Ok);
            return;
        }
        if (_ui->lineEdit_MaxSize->text().toDouble() < _ui->lineEdit_MinSize->text().toDouble())
        {
            QMessageBox::warning(this, "warning", tr("Set the correct size."), QMessageBox::Ok);
            return;
        }

        Interface::FITKGlobalMeshSizeInfo* meshSizeInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshSizeInfo("GmshExec");
        if (meshSizeInfo == nullptr) return;

        Interface::FITKGlobalMeshGenerateAlgorithmInfo* meshGenerateAlgorithmInfo = Interface::FITKMeshGenInterface::getInstance()->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (meshGenerateAlgorithmInfo == nullptr) return;
        //设置最大尺寸
        meshSizeInfo->setMaxSize(_ui->lineEdit_MaxSize->text().toDouble());
        //设置最小尺寸
        meshSizeInfo->setMinSize(_ui->lineEdit_MinSize->text().toDouble());
        //设置尺寸因子
        meshSizeInfo->setSizeFactor(_ui->lineEdit_SizeFactor->text().toDouble());
        //设置SmoothStep
        meshGenerateAlgorithmInfo->setSmoothStep(_ui->spinBox_SmoothingSteps->value());
        //设置单元阶数
        meshGenerateAlgorithmInfo->setElementOrder(_ui->comboBox_ElementOrder->currentData().toInt());
        //设置重新组合所有三角形网格
        meshGenerateAlgorithmInfo->setUseIncElem(_ui->checkBox_UseIncElem->isChecked());
        //设置不完整的单元
        meshGenerateAlgorithmInfo->setRecombineAllTriMeshes(_ui->checkBox_RecombineAllTriMeshes->isChecked());
        //设置几何缝合
        meshGenerateAlgorithmInfo->setGeometryStitch(_ui->checkBox_GeometryStitch->isChecked());
        //设置网格划分最高维度
        meshGenerateAlgorithmInfo->setMeshGenerateDimension(_ui->comboBox_Type->currentData().toInt());
        bool ok {};
        //设置2D算法
        int algorithm2D = _ui->comboBox_Algorithm2D->currentData().toInt(&ok);
        if (ok)
            meshGenerateAlgorithmInfo->set2DAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D(algorithm2D));
        //设置3D算法
        int algorithm3D = _ui->comboBox_Algorithm3D->currentData().toInt(&ok);
        if (ok)
            meshGenerateAlgorithmInfo->set3DAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D(algorithm3D));
        // 设置2D重组算法设置
        int reAlgorithm2D = _ui->comboBox_ReAlgorithm2D->currentData().toInt(&ok);
        if (ok)
            meshGenerateAlgorithmInfo->set2DRecombinationAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D(reAlgorithm2D));
        // 设置细化算法设置
        if (_ui->tabWidget->currentIndex() == 0)
        {
            if (_ui->radioButton_Triangle->isChecked())
            {
                meshGenerateAlgorithmInfo->setSubdivisionAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::None);
            }
            else
            {
                meshGenerateAlgorithmInfo->setSubdivisionAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllQuads);
            }
        }
        else if (_ui->tabWidget->currentIndex() == 1)
        {
            if (_ui->radioButton_Tetrahedron->isChecked())
            {
                meshGenerateAlgorithmInfo->setSubdivisionAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::None);
            }
            else
            {
                meshGenerateAlgorithmInfo->setSubdivisionAlgorithm(Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllHexas);
            }
        }
        
        QVariant v = QVariant::fromValue(_virtualTopos);
        if (_driver)
            _driver->setValue("virtualTopos", v);

        this->accept();
    }

    void GUIGmshSettings::on_pushButton_Cancel_clicked()
    {
        QList<Interface::VirtualShape> virtualTopos{};
        QVariant v = QVariant::fromValue(virtualTopos);
        if (_driver)
            _driver->setValue("virtualTopos", v);
        this->reject();
    }

    void GUIGmshSettings::on_pushButton_Pick_clicked()
    {
        //设置拾取方式
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        if(_ui->comboBox_Type->currentData().toInt() == Surface)
            emit signalTransfer->setPickableObjTypeSig(3005);
        else
            emit signalTransfer->setPickableObjTypeSig(3006);

    }
    void GUIGmshSettings::on_comboBox_Type_currentIndexChanged(int index)
    {
        _ui->tabWidget->setCurrentIndex(index);
        _virtualTopos.clear();
        _ui->label_10->setText("Picked (0)");
        //关闭拾取
        AppFrame::FITKSignalTransfer* signalTransfer = FITKAPP->getSignalTransfer();
        if (!signalTransfer) return;
        emit signalTransfer->setPickableObjTypeSig(-1);
    }
}