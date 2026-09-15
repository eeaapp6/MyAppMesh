/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperFieldOutput.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "GUIFrame/MainWindow.h"
#include "OperatorsInterface/GraphEventOperator.h"
#include <QComboBox>
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKAbstractStruPostVTK.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStruPost3DManager.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKDynamicStruPostVTK.h"
#include "FITK_Interface/FITKInterfaceStructuralPost/FITKStaticStruPostVTK.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossPostData.h"

namespace GUIOper
{
    bool OperFieldOutput::comboboxChanged()
    {
        QComboBox* combox = dynamic_cast<QComboBox*>(_emitter);
        if (combox == nullptr) return false;

        auto comboxName = combox->objectName();
        // 第一个下拉框
        if (comboxName == "comboBox_PostVariableLocation") {
            updatePostFieldVariable_Combox(combox->currentText());
            execPostVariableLocation_Combox();
        }
        // 第二个下拉框
        else if (comboxName == "comboBox_PostFieldVariable") {
            updatePostFieldWeight_Combox(combox->currentText());
            execPostFieldVariable_Combox();
        }
        // 第三个下拉框
        else if (comboxName == "comboBox_PostFieldWeight") {
            execPostFieldWeight_Combox();
        }
        else {
            return false;
        }

        return true;
    }


    void OperFieldOutput::setParentWidget(QWidget* w)
    {
        //auto comboBoxs = w->findChildren<QComboBox*>();
        //for (auto comboBox : comboBoxs) {
        //    //关联信号槽
        //    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboboxChanged(int)));
        //}
        auto combox = w->findChild<QComboBox*>("comboBox_PostVariableLocation");
        if (combox)connect(combox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboboxChanged(int)));
        combox = w->findChild<QComboBox*>("comboBox_PostFieldVariable");
        if (combox)connect(combox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboboxChanged(int)));
        combox = w->findChild<QComboBox*>("comboBox_PostFieldWeight");
        if (combox)connect(combox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboboxChanged(int)));

    }

    void OperFieldOutput::execPostVariableLocation_Combox()
    {
        // 避免第二个下拉框选中项不变，强制刷新第三个下拉框
        if (_mainWindow == nullptr) return;
        QComboBox* fieldComboBox = _mainWindow->findChild<QComboBox*>("comboBox_PostFieldVariable");
        if (!fieldComboBox) return;
        updatePostFieldWeight_Combox(fieldComboBox->currentText());
        execPostFieldVariable_Combox();
    }

    void OperFieldOutput::execPostFieldVariable_Combox()
    {
        execPostFieldWeight_Combox();
    }

    void OperFieldOutput::execPostFieldWeight_Combox()
    {
        resetViewport();
    }

    void OperFieldOutput::updatePostFieldVariable_Combox(QString firstComboxVal)
    {
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return;
        auto postData = radiossPostData->getPost3DManager()->getDataByID(radiossPostData->getCurrentPostDataID());
        if (postData == nullptr) return;
        if (_mainWindow == nullptr) return;
        QStringList variables{};
        if (firstComboxVal == "Node") {
            variables = postData->getVariables(Interface::VariableLocation::VALOCNode);
        }
        else if (firstComboxVal == "Element") {
            variables = postData->getVariables(Interface::VariableLocation::VALOCEle);
        }
        QStringList displayVariables = {};
        for (auto var : variables) {
            displayVariables.append(var);
        }
        QComboBox* fieldComboBox = _mainWindow->findChild<QComboBox*>("comboBox_PostFieldVariable");
        if (!fieldComboBox) return;
        fieldComboBox->blockSignals(true);
        fieldComboBox->clear();
        fieldComboBox->addItems(displayVariables);
        fieldComboBox->setCurrentText(displayVariables.size() > 0 ? displayVariables.at(0) : "");
        fieldComboBox->blockSignals(false);
    }

    void OperFieldOutput::updatePostFieldWeight_Combox(QString secondComboxVal)
    {
        Radioss::FITKRadiossPostData* radiossPostData = Radioss::FITKRadiossPostData::GetDataFromAPPFrame();
        if (radiossPostData == nullptr) return;
        Interface::FITKAbstractStructuralPostVTK* postData = radiossPostData->getPost3DManager()->getDataByID(radiossPostData->getCurrentPostDataID());
        if (postData == nullptr) return;
        if (_mainWindow == nullptr) return;
        QComboBox* variableLocationComboBox = _mainWindow->findChild<QComboBox*>("comboBox_PostVariableLocation");
        if (!variableLocationComboBox) return;

        //获取变量位置
        Interface::VariableLocation firstVal = variableLocationComboBox->currentText() == "Element" ? Interface::VariableLocation::VALOCEle : Interface::VariableLocation::VALOCNode;
        QStringList variables = postData->getVariables(firstVal);;
        QStringList variableComponents = postData->getVariableComponents(firstVal, secondComboxVal);

        if (variableComponents.size() >= 3) {
            variableComponents.insert(0, "Magnitude");
        }
        QComboBox* fieldComboBox = _mainWindow->findChild<QComboBox*>("comboBox_PostFieldWeight");
        if (!fieldComboBox) return;
        fieldComboBox->blockSignals(true);
        fieldComboBox->clear();

        for (int i = 0; i < variableComponents.size(); ++i)
        {
            fieldComboBox->addItem(variableComponents.at(i), i - 1);
        }
        fieldComboBox->setCurrentText(variableComponents.size() > 0 ? variableComponents.at(0) : "");
        fieldComboBox->blockSignals(false);
    }

    void OperFieldOutput::resetViewport()
    {
        Core::FITKActionOperator* graphOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<Core::FITKActionOperator>("actionUpdatePostGraph");
        if (graphOper) graphOper->execProfession();
    }

    void OperFieldOutput::onComboboxChanged(int)
    {
        _emitter = sender();
        comboboxChanged();
    }

}