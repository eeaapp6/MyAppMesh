/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGeometrySphereDialog.h"
#include "ui_GUIGeometrySphereDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSphere.h"
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
#include <QMessageBox>

namespace GUI {
    GUIGeometrySphereDialog::GUIGeometrySphereDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :GUIDialogBase(nullptr, parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometrySphereDialog();
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
            connect(pickProvider, &GraphData::PickedDataProvider::sigPickOver, this, &GUIGeometrySphereDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometrySphereDialog::~GUIGeometrySphereDialog()
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

    void GUIGeometrySphereDialog::init()
    {
        //获取几何管理器
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) return;
        Radioss::FITKRadiossGeomPartManager* partMgr = radiossCase->getGeomPartManager();
        if (partMgr == nullptr) return;

        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return;

        if (_isCreate)
        {
            setWindowTitle(tr("Create Sphere"));
            _modelData = new Radioss::FITKRadiossGeomPart();
            partMgr->appendDataObj(_modelData);
            _geometryData = _modelData->getGeoPart();
            QString name = partMgr->checkName(tr("Sphere-1"));
            _ui->lineEdit_Name->setText(name);
        }
        else
        {
            setWindowTitle(tr("Edit Sphere"));
            _modelData = dynamic_cast<Radioss::FITKRadiossGeomPart*>(_data);
            if (!_modelData) return;
            _geometryData = _modelData->getGeoPart();
            if (_geometryData == nullptr) return;
            _ui->lineEdit_Name->setText(_modelData->getDataObjectName());

            // 读取现有几何参数
            if (_geometryData->getDataCount() > 0)
            {
                Interface::FITKAbsGeoModelSphere* sphere = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(
                    _geometryData->getDataByIndex(0));
                if (sphere)
                {
                    double location[3];
                    double radius = 0.0;
                    sphere->getLocation(location);
                    radius = sphere->getRadius();

                    _ui->lineEdit_X->setText(QString::number(location[0]));
                    _ui->lineEdit_Y->setText(QString::number(location[1]));
                    _ui->lineEdit_Z->setText(QString::number(location[2]));
                    _ui->lineEdit_Radius->setText(QString::number(radius));
                }
            }
        }
    }

    void GUIGeometrySphereDialog::pickFinishedOper()
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
        if (virtualShape.isEmpty()) return;
        //获取坐标
        Interface::FITKInterfaceGeometryFactory*factory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (factory == nullptr) return;
        Interface::FITKAbstractGeomToolsCreator*createor = factory->getGeomToolsCreator();
        if (!createor) return;
        Interface::FITKAbstractGeomPointTool *pointTool = createor->createPointTool();
        if (!pointTool) return;
        double PointArray[3];
        pointTool->getXYZ(FITKDATAREPO->getTDataByID<Interface::FITKAbsVirtualTopo>(virtualShape[0].VirtualTopoId), PointArray);
        _ui->lineEdit_X->setText(QString::number(PointArray[0]));
        _ui->lineEdit_Y->setText(QString::number(PointArray[1]));
        _ui->lineEdit_Z->setText(QString::number(PointArray[2]));
    }

    void GUIGeometrySphereDialog::on_pushButton_OK_clicked()
    {
        if (!_geometryData) return;
        Radioss::FITKRadiossCase* radiossCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr) return;
        Radioss::FITKRadiossGeomPartManager* partMgr = radiossCase->getGeomPartManager();
        if (partMgr == nullptr) return;

        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (cmdList == nullptr) return;
        //获取数据名称
        QString name = _ui->lineEdit_Name->text();
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "", tr("Please set the model name.").arg(name), QMessageBox::Ok);
            return;
        }
        if (name != _modelData->getDataObjectName() && partMgr->getDataObjectByName(name))
        {
            QMessageBox::warning(this, "", tr("The model name already exists. Please change a name.").arg(name), QMessageBox::Ok);
            return;
        }

        int objID = _modelData->getGeoPartID();

        if (_isCreate)
        {
            // 创建模式：创建新的几何对象
            Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
            if (!geoFactory) return;
            Interface::FITKAbsGeoModelSphere* obj = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(
                geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTSphere));
            if (!obj) return;

            double location[3] = { _ui->lineEdit_X->text().toDouble(),
                                _ui->lineEdit_Y->text().toDouble(),
                                _ui->lineEdit_Z->text().toDouble() };
            double radius = _ui->lineEdit_Radius->text().toDouble();
            obj->setLocation(location);
            obj->setRadius(radius);

            if (_geometryData->addAndUpdate(obj) == false)
            {
                QMessageBox::warning(this, "", tr("The geometry could not be created properly"), QMessageBox::Ok);
                return;
            }
        }
        else
        {
            // 编辑模式：更新现有的几何对象
            if (_geometryData->getDataCount() > 0)
            {
                Interface::FITKAbsGeoModelSphere* sphere = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(
                    _geometryData->getDataByIndex(0));
                if (sphere)
                {
                    double location[3] = { _ui->lineEdit_X->text().toDouble(),
                                        _ui->lineEdit_Y->text().toDouble(),
                                        _ui->lineEdit_Z->text().toDouble() };
                    double radius = _ui->lineEdit_Radius->text().toDouble();
                    sphere->setLocation(location);
                    sphere->setRadius(radius);

                    if (sphere->update() == false)
                    {
                        QMessageBox::warning(this, "", tr("The geometry error"), QMessageBox::Ok);
                        return;
                    }
                    _geometryData->reGenerate();
                }
            }
        }
        _geometryData->setDataObjectName(cmdList->checkName(name));
        _modelData->setDataObjectName(partMgr->checkName(name));
        _oper->setArgs("objID", objID);
        _oper->execProfession();
        //将创建球体的命令写入Python脚本
        this->writePythonScript();

        this->accept();
    }

    void GUIGeometrySphereDialog::on_pushButton_Cancel_clicked()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        this->reject();
    }

    void GUIGeometrySphereDialog::on_point1_pick_clicked()
    {
        //设置拾取方式
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoVert, GUI::GUIPickInfo::PickMethod::PMSingle);
    }

    void GUIGeometrySphereDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }

    void GUIGeometrySphereDialog::writePythonScript()
    {
        //获取数据
        double location[3] = { _ui->lineEdit_X->text().toDouble(),
                            _ui->lineEdit_Y->text().toDouble(),
                            _ui->lineEdit_Z->text().toDouble() };
        double radius = _ui->lineEdit_Radius->text().toDouble();
        QString name = _ui->lineEdit_Name->text();

        //将创建球体的命令写入Python脚本
        QStringList pythonCmd;
        pythonCmd.append("sphere=Geometrys()");
        pythonCmd.append(QString("sphere.createSphere(%1, %2, %3, %4)")
            .arg(location[0]).arg(location[1]).arg(location[2]).arg(radius));
        pythonCmd.append(QString("sphere.setName(\"%1\")").arg(name));
        this->saveScript(pythonCmd);
    }
}