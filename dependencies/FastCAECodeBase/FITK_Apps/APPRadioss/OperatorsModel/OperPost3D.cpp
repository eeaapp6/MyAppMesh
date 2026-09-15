/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperPost3D.h"
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QComboBox>
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "GUIFrame/MainWindow.h"

#include "GUIWidget/GraphArea.h"
#include "GUIFrame/CentralWidget.h"
#include "GUIWidget/GraphMdiArea.h"
#include "GUIFrame/SARibbonBarHelper.h"

#include "OperatorsInterface/TreeEventOperator.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKAbstractStruPostVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKDynamicStruPostVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStaticStruPostVTK.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"

#include "vtkAlgorithm.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkInformation.h"
#include "vtkSmartPointer.h"
#include "vtkInformationDoubleVectorKey.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkDataSetReader.h"
#include <vtkFieldData.h>

#include <QTimer>
#include <QTime>

namespace OperModel
{
    OperPost3D::OperPost3D()
    {
        _timer = new QTimer(this);
        // 设置为单次触发模式
        _timer->setSingleShot(true);
        // 连接信号和槽
        connect(_timer, &QTimer::timeout, this, &OperPost3D::startPlaying);
    }

    OperPost3D::~OperPost3D()
    {
        if (_timer) delete _timer;
    }

    bool OperPost3D::execGUI()
    {
        QAction* action = dynamic_cast<QAction*>(_emitter);
        if (!action) return false;
        QString name = _emitter->objectName();
        if (name == "actionFirstFrame")
        {
            this->playFirstFrame();
        }
        else if (name == "actionPreviousFrame")
        {
            this->playPreviousFrame();
        }
        else if (name == "actionNextFrame")
        {
            this->playNextFrame();
        }
        else if (name == "actionLastFrame")
        {
            this->playLastFrame();
        }
        else if (name == "actionStartPlaying")
        {
            int state = action->data().toInt();
            if (state == 0)
            {
                action->setText(QObject::tr("Stop Playing"));
                action->setData(1);
                action->setIcon(QIcon(":/toolBar/stopPlaying.png"));
                this->startPlaying();
            }
            else if (state == 1)
            {
                action->setText(QObject::tr("Start Playing"));
                action->setData(0);
                action->setIcon(QIcon(":/toolBar/startPlaying.png"));
                this->stopPlaying();
            }
        }
        else if (name == "actionUpdateToolBarField")
        {
            this->updateToolBarFieldComboBox();
        }
        return true;
    }
    bool OperPost3D::execProfession()
    {

        this->resetViewport();
        return true;
    }
    void OperPost3D::playFirstFrame()
    {
        //获取数据对象
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return;
        int id = radiossPostData->getCurrentPostDataID();
        Interface::FITKDynamicStruPostVTK* dataObj = dynamic_cast<Interface::FITKDynamicStruPostVTK*>(radiossPostData->getPost3DManager()->getDataByID(id));
        if (!dataObj) return;
        dataObj->firstFrame();
    }
    void OperPost3D::playPreviousFrame()
    {
        //获取数据对象
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return;
        int id = radiossPostData->getCurrentPostDataID();
        Interface::FITKDynamicStruPostVTK* dataObj = dynamic_cast<Interface::FITKDynamicStruPostVTK*>(radiossPostData->getPost3DManager()->getDataByID(id));
        if (!dataObj) return;
        dataObj->previousFrame();
    }
    void OperPost3D::playNextFrame(bool* isLastFrame)
    {
        if (isLastFrame) *isLastFrame = true;
        //获取数据对象
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return;
        int id = radiossPostData->getCurrentPostDataID();
        Interface::FITKDynamicStruPostVTK* dataObj = dynamic_cast<Interface::FITKDynamicStruPostVTK*>(radiossPostData->getPost3DManager()->getDataByID(id));
        if (!dataObj) return;
        dataObj->nextFrame();
        //判断是否是最后一帧
        if (isLastFrame && dataObj->getCurFrame() + 1 < dataObj->frameNum())
            *isLastFrame = false;

    }
    void OperPost3D::playLastFrame()
    {
        //获取数据对象
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return;
        int id = radiossPostData->getCurrentPostDataID();
        Interface::FITKDynamicStruPostVTK* dataObj = dynamic_cast<Interface::FITKDynamicStruPostVTK*>(radiossPostData->getPost3DManager()->getDataByID(id));
        if (!dataObj) return;
        dataObj->lastFrame();
    }
    void OperPost3D::startPlaying()
    {
        bool ok = false;
        this->playNextFrame(&ok);
        
        //启动定时功能
        if (!ok)
        {
            Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
            if (radiossPostData == nullptr) return;
            // 设置间隔
            _timer->setInterval(radiossPostData->getAnimTimeInterval());
            this->resetViewport();
            _timer->start();
        }
        else
        {
            GUI::MainWindow* mainWindow = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
            if (!mainWindow) return;
            QAction* action = mainWindow->findChild<QAction*>("actionStartPlaying");
            if (!action) return;
            action->setText(QObject::tr("Start Playing"));
            action->setData(0);
            action->setIcon(QIcon(":/toolBar/startPlaying.png"));
        }
    }

    void OperPost3D::stopPlaying()
    {
        //停止定时功能
        _timer->stop();
    }

    void OperPost3D::updateToolBarFieldComboBox()
    {
        // 获取工具栏
        GUI::MainWindow* mw = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mw == nullptr) return;
        QComboBox* variableLocationComboBox = mw->findChild<QComboBox*>("comboBox_PostVariableLocation");
        QComboBox* fieldVariableComboBox = mw->findChild<QComboBox*>("comboBox_PostFieldVariable");
        QComboBox* fieldWeightComboBox = mw->findChild<QComboBox*>("comboBox_PostFieldWeight");
        if (!variableLocationComboBox || !fieldVariableComboBox || !fieldWeightComboBox) return;
        //获取数据对象
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return;
        int id = radiossPostData->getCurrentPostDataID();
        Interface::FITKDynamicStruPostVTK* dataObj = dynamic_cast<Interface::FITKDynamicStruPostVTK*>(radiossPostData->getPost3DManager()->getDataByID(id));
        if (!dataObj) return;
        //获取场数据
        QStringList elemVariables = dataObj->getVariables(Interface::VariableLocation::VALOCNode);
        if (elemVariables.isEmpty()) return;
        QStringList variableComponents = dataObj->getVariableComponents(Interface::VariableLocation::VALOCEle, elemVariables.at(0));

        variableLocationComboBox->blockSignals(true);
        variableLocationComboBox->clear();
        variableLocationComboBox->addItems({ "Node", "Element" });
        variableLocationComboBox->setCurrentIndex(0);
        variableLocationComboBox->blockSignals(false);

        fieldVariableComboBox->blockSignals(true);
        fieldVariableComboBox->clear();
        fieldVariableComboBox->addItems(elemVariables);
        int index = fieldVariableComboBox->findText("Displacement");
        fieldVariableComboBox->setCurrentIndex(index < 0 ? 0 : index);
        fieldVariableComboBox->blockSignals(false);

        fieldWeightComboBox->blockSignals(true);
        fieldWeightComboBox->clear();
        for (int i = 0; i < variableComponents.size(); ++i)
        {
            fieldWeightComboBox->addItem(variableComponents.at(i), i - 1);
        }
        
        fieldWeightComboBox->setCurrentIndex(0);
        fieldWeightComboBox->blockSignals(false);

        dataObj->setCurFrame(0);
    }

    void OperPost3D::resetViewport()
    {
        Core::FITKActionOperator* graphOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionUpdatePostGraph");
        if (graphOper) graphOper->execProfession();
    }


    bool OperPost3DResultUpdate::execProfession()
    {
        GUI::MainWindow* mw = dynamic_cast<GUI::MainWindow*>(FITKAPP->getGlobalData()->getMainWindow());
        if (mw == nullptr) return false;
        //错误判断
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return false;
        Interface::FITKAbstractStructuralPostVTK* postData = radiossPostData->getPost3DManager()->getDataByID(radiossPostData->getCurrentPostDataID());
        if (postData == nullptr) return false;
        //获取时间信息
        bool isTimeRead = false;
        double time = 0.0;
        vtkDataSet* dataSet = postData->getOutput();
        if (dataSet && dataSet->GetFieldData())
        {
            vtkFieldData* fieldData = dataSet->GetFieldData();
            for (int i = 0; i < fieldData->GetNumberOfArrays(); ++i)
            {
                vtkDataArray* arrayData = fieldData->GetArray(i);
                if (!arrayData || QString(arrayData->GetName()) != "TIME") continue;
                // 检查数据类型和维度
                if (arrayData->GetDataType() != VTK_DOUBLE || arrayData->GetNumberOfTuples() <= 0) continue;
                isTimeRead = true;
                time = arrayData->GetTuple1(0); // 返回第一个时间值
                break;
            }
        }
        //获取场数据
        QComboBox* variableLocationComboBox = mw->findChild<QComboBox*>("comboBox_PostVariableLocation");
        QComboBox* fieldVariableComboBox = mw->findChild<QComboBox*>("comboBox_PostFieldVariable");
        QComboBox* fieldWeightComboBox = mw->findChild<QComboBox*>("comboBox_PostFieldWeight");
        if (!variableLocationComboBox || !fieldVariableComboBox || !fieldWeightComboBox) return false;
        int firstVal = variableLocationComboBox->currentText() == "Element" ? 2 : 1;
        QString fieldName = fieldVariableComboBox->currentText();
        int compIndex = fieldWeightComboBox->currentData().toInt();
        auto operatorRepo = Core::FITKOperatorRepo::getInstance();
        //后处理结果刷新
        EventOper::GraphEventOperator* graphOper = operatorRepo->getOperatorT<EventOper::GraphEventOperator>("GraphPostProcess");
        if (graphOper)
        {
            if (isTimeRead)
                graphOper->updateTextInfo(radiossPostData->getCurrentPostDataID(), QString("Time:  %1").arg(time));
            else
                graphOper->updateTextInfo(radiossPostData->getCurrentPostDataID(), QString());
             graphOper->changeField(radiossPostData->getCurrentPostDataID(), firstVal, fieldName, compIndex);
            graphOper->updateGraph(radiossPostData->getCurrentPostDataID(), true);
        }
        return true;
    }
}



