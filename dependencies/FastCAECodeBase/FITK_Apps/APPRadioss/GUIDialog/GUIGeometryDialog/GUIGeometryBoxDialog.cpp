/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIGeometryBoxDialog.h"
#include "ui_GUIGeometryBoxDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
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
    GUIGeometryBoxDialog::GUIGeometryBoxDialog(Core::FITKActionOperator* oper, QWidget* parent)
        :GUIDialogBase(nullptr, parent), _oper(oper)
    {
        _ui = new Ui::GUIGeometryBoxDialog();
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
            connect(pickProvider, &GraphData::PickedDataProvider::sigPickOver, this, &GUIGeometryBoxDialog::pickFinishedOper);
        }
        this->init();
    }

    GUIGeometryBoxDialog::~GUIGeometryBoxDialog()
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

    void GUIGeometryBoxDialog::init()
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
            setWindowTitle(tr("Create Box"));
            _modelData = new Radioss::FITKRadiossGeomPart();
            partMgr->appendDataObj(_modelData);
            _geometryData = _modelData->getGeoPart();
            QString name = partMgr->checkName(tr("Box-1"));
            _ui->lineEdit_Name->setText(name);
        }
        else
        {
            setWindowTitle(tr("Edit Box"));
            _modelData = dynamic_cast<Radioss::FITKRadiossGeomPart*>(_data);
            if (!_modelData) return;
            _geometryData = _modelData->getGeoPart();
            if (_geometryData == nullptr) return;
            _ui->lineEdit_Name->setText(_modelData->getDataObjectName());

            // 读取现有几何参数
            if (_geometryData->getDataCount() > 0)
            {
                Interface::FITKAbsGeoModelBox* box = dynamic_cast<Interface::FITKAbsGeoModelBox*>(
                    _geometryData->getDataByIndex(0));
                if (box)
                {
                    double point1[3];
                    double length[3];
                    double dir1[3];
                    double dir2[3];
                    box->getPoint1(point1);
                    box->getLength(length);
                    box->getDirection1(dir1);
                    box->getDirection2(dir2);

                    _ui->lineEdit_X->setText(QString::number(point1[0]));
                    _ui->lineEdit_Y->setText(QString::number(point1[1]));
                    _ui->lineEdit_Z->setText(QString::number(point1[2]));
                    _ui->lineEdit_X_2->setText(QString::number(length[0]));
                    _ui->lineEdit_Y_2->setText(QString::number(length[1]));
                    _ui->lineEdit_Z_2->setText(QString::number(length[2]));
                }
            }
        }
    }

    void GUIGeometryBoxDialog::pickFinishedOper()
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
 

    void GUIGeometryBoxDialog::on_pushButton_OK_clicked()
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
            Interface::FITKAbsGeoModelBox* obj = dynamic_cast<Interface::FITKAbsGeoModelBox*>(
                geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTBox));
            if (!obj) return;

            double point1[3] = { _ui->lineEdit_X->text().toDouble(),
                                _ui->lineEdit_Y->text().toDouble(),
                                _ui->lineEdit_Z->text().toDouble() };
            double length[3] = { _ui->lineEdit_X_2->text().toDouble(),
                                _ui->lineEdit_Y_2->text().toDouble(),
                                _ui->lineEdit_Z_2->text().toDouble() };
            obj->setPoint1(point1);
            obj->setLength(length);
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
                Interface::FITKAbsGeoModelBox* box = dynamic_cast<Interface::FITKAbsGeoModelBox*>(
                    _geometryData->getDataByIndex(0));
                if (box)
                {
                    double point1[3] = { _ui->lineEdit_X->text().toDouble(),
                                        _ui->lineEdit_Y->text().toDouble(),
                                        _ui->lineEdit_Z->text().toDouble() };
                    double length[3] = { _ui->lineEdit_X_2->text().toDouble(),
                                        _ui->lineEdit_Y_2->text().toDouble(),
                                        _ui->lineEdit_Z_2->text().toDouble() };
                    box->setPoint1(point1);
                    box->setLength(length);
                    if (box->update() == false)
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
        //将创建长方体的命令写入Python脚本
        this->writePythonScript();

        this->accept();
    }

    void GUIGeometryBoxDialog::on_pushButton_Cancel_clicked()
    {
        //关闭窗口
        _oper->setArgs("objID", -1);
        this->reject();
    }

    void GUIGeometryBoxDialog::on_point1_pick_clicked()
    {
        //设置拾取方式
        enablePickSig(GUI::GUIPickInfo::PickObjType::POBGeoVert, GUI::GUIPickInfo::PickMethod::PMSingle);
    }

    void GUIGeometryBoxDialog::writePythonScript()
    {
        //获取数据
        double point1[3] = { _ui->lineEdit_X->text().toDouble(),
                            _ui->lineEdit_Y->text().toDouble(),
                            _ui->lineEdit_Z->text().toDouble() };
        double length[3] = { _ui->lineEdit_X_2->text().toDouble(),
                            _ui->lineEdit_Y_2->text().toDouble(),
                            _ui->lineEdit_Z_2->text().toDouble() };
        QString name = _ui->lineEdit_Name->text();

        //将创建长方体的命令写入Python脚本
        QStringList pythonCmd;
        pythonCmd.append("box=Geometrys()");
        pythonCmd.append(QString("box.createBox(%1, %2, %3, %4, %5, %6)")
            .arg(point1[0]).arg(point1[1]).arg(point1[2])
            .arg(length[0]).arg(length[1]).arg(length[2]));
        pythonCmd.append(QString("box.setName(\"%1\")").arg(name));
        this->saveScript(pythonCmd);
    }

    void GUIGeometryBoxDialog::enablePickSig(GUI::GUIPickInfo::PickObjType objType, GUI::GUIPickInfo::PickMethod methodType)
    {
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();
        pickInfo._pickObjType = GUI::GUIPickInfo::PickObjType(objType);
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod(methodType);
        //设置拾取方式
        GUI::GUIPickInfo::SetPickInfo(pickInfo);
    }
}