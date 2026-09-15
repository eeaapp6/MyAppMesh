/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ToolBarColorMapper.h"
#include "TreeWidget.h"
#include "GUIComboBox.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/ParaWidgetInterfaceOperator.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphProperty.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

#include <QComboBox>
#include <QSignalBlocker>

Q_DECLARE_METATYPE(Interface::FITKPostDisplayType)
Q_DECLARE_METATYPE(Interface::FITKPostFieldType)

namespace GUI
{
    ToolBarColorMapper::ToolBarColorMapper(QWidget * parent) :
        ToolBarBase(parent)
    {
        _actionShowColorMap = createAction("actionShowColorMap", "://icons/counter.png", QString(tr("")));
        _actionShowColorMap->setCheckable(true);
        _actionShowColorMap->setEnabled(false);
        this->addAction(_actionShowColorMap);
        _actionEditColorMap = createAction("actionEditColorMap", "://icons/post.png", QString(tr("")));
        _actionEditColorMap->setEnabled(false);
        this->addAction(_actionEditColorMap);
        _actionColorLibEdit = createAction("actionColorLibEdit", "", QString(tr("")));
        _actionColorLibEdit->setEnabled(false);
        _actionColorLibEdit->setText("colorEdit");
        this->addAction(_actionColorLibEdit);
        _variableCombox = new GUIComboBox(this);
        _variableCombox->setFixedWidth(130);
        _variableCombox->setEnabled(false);
        this->addWidget(_variableCombox);
        _componentCombox = new GUIComboBox(this);
        _componentCombox->setEnabled(false);
        this->addWidget(_componentCombox);
        _disPlayCombox = new GUIComboBox(this);
        _disPlayCombox->setEnabled(false);
        _disPlayCombox->addItem(QIcon("://icons/point.png"), QString(tr("Point")), QVariant::fromValue(Interface::FITKPostDisplayType::Post_Point));
        _disPlayCombox->addItem(QIcon("://icons/wireFrame.png"), QString(tr("WireFrame")), QVariant::fromValue(Interface::FITKPostDisplayType::Post_Edge));
        _disPlayCombox->addItem(QIcon("://icons/face.png"), QString(tr("Surface")), QVariant::fromValue(Interface::FITKPostDisplayType::Post_Face));
        _disPlayCombox->addItem(QIcon("://icons/faceWithEdge.png"), QString(tr("Surface With Edge")), QVariant::fromValue(Interface::FITKPostDisplayType::Post_FaceWithEdge));
        _disPlayCombox->setCurrentIndex(2);
        this->addWidget(_disPlayCombox);

        connect(_actionShowColorMap, SIGNAL(triggered()), this, SLOT(slotShowColorMapp()));
        connect(_actionEditColorMap, SIGNAL(triggered()), this, SLOT(slotEditColorMapp()));
        connect(_variableCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotVariableChange(int)));
        connect(_componentCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotComponentChange(int)));
        connect(_disPlayCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDisplayChange(int)));
    }

    ToolBarColorMapper::~ToolBarColorMapper()
    {
        this->clear();
    }

    void ToolBarColorMapper::updateCurrentGraphObj(int objID)
    {
        _currentDataID = objID;
        updateShowColorMapp();
        updateVariable();
        updateComponent();
        updateDisplayComboBox();
    }

    void ToolBarColorMapper::slotShowColorMapp()
    {
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        if (currentGraphObj == nullptr)return;
        Interface::PostGraphProperty* property = currentGraphObj->getProperty();
        if (property == nullptr)return;

        //是否显示colormapper编辑按钮
        if (_actionShowColorMap->isChecked()) {
            _actionEditColorMap->setEnabled(true);
            property->setScalarBarWidgetIsShow(true);
        }
        else {
            _actionEditColorMap->setEnabled(false);
            property->setScalarBarWidgetIsShow(false);
        }

        reRender();
    }

    void ToolBarColorMapper::slotEditColorMapp()
    {
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        int postDataId = currentData->getDataObjectID();
        auto colorMapEditOper = FITKOPERREPO->getOperatorT<EventOper::ParaWidgetInterfaceOperator>("colorMapEdit");
        if (colorMapEditOper == nullptr)return;
        colorMapEditOper->setArgs("objID", postDataId);
        colorMapEditOper->actionTriggered();
    }

    void ToolBarColorMapper::slotVariableChange(int index)
    {
        Q_UNUSED(index);
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        if (currentGraphObj == nullptr)return;
        QString name = _variableCombox->currentText();
        Interface::FITKPostFieldType type = _variableCombox->currentData().value<Interface::FITKPostFieldType>();
        Interface::PostGraphProperty* property = currentGraphObj->getProperty();
        if (property) {
            property->setCurrentField(type, name);
        }

        //是否开启场变量数值显示
        if (name == SolidColor) {
            _actionShowColorMap->setEnabled(false);
        }
        else {
            _actionShowColorMap->setEnabled(true);
        }

        updateShowColorMapp();
        updateComponent();
        reRender();
    }

    void ToolBarColorMapper::slotComponentChange(int index)
    {
        Q_UNUSED(index);
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        if (currentGraphObj == nullptr)return;
        QString name = _variableCombox->currentText();
        Interface::FITKPostFieldType type = _variableCombox->currentData().value<Interface::FITKPostFieldType>();
        int compomentIndex = _componentCombox->currentData().toInt();
        Interface::PostGraphProperty* property = currentGraphObj->getProperty();
        if (property) {
            property->setCurrentField(type, name, compomentIndex);
        }
        reRender();
    }

    void ToolBarColorMapper::slotDisplayChange(int index)
    {
        Q_UNUSED(index);
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        if (currentGraphObj == nullptr)return;
        Interface::PostGraphProperty* property = currentGraphObj->getProperty();
        if (property == nullptr)return;
        Interface::FITKPostDisplayType type = _disPlayCombox->currentData().value<Interface::FITKPostDisplayType>();
        property->setCurrentDisplayType(type);

        reRender();
    }

    void ToolBarColorMapper::updateShowColorMapp()
    {
        //默认关闭scalarBarWidget相关按钮
        QSignalBlocker blocker1(_actionShowColorMap);
        QSignalBlocker blocker2(_actionEditColorMap);
        _actionShowColorMap->setEnabled(false);
        _actionShowColorMap->setChecked(false);
        _actionEditColorMap->setEnabled(false);
        _actionColorLibEdit->setEnabled(false);

        bool isShow = false;
        Interface::PostGraphProperty* currentProperty = nullptr;
        Interface::FITKPostFieldType currentFieldType;
        QString currentFieldName = "";
        int currentCompIndex = -1;
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentGraphObj) {
            currentProperty = currentGraphObj->getProperty();
            if (currentProperty) {
                isShow = currentProperty->getScalarBarWidgetIsShow();
                currentProperty->getCurrentField(currentFieldType, currentFieldName, currentCompIndex);
            }
        }

        //关闭所有scalarBarWidget
        for (int i = 0; i < _postGraphManager->getDataCount(); i++) {
            Interface::PostGraphObjectAgent* agent = _postGraphManager->getDataByIndex(i);
            if (agent == nullptr)continue;
            Interface::PostGraphObjectBase* graphObj = agent->getGraphObject();
            if (graphObj == nullptr)continue;
            Interface::PostGraphProperty* postProperty = graphObj->getProperty();
            if(postProperty == nullptr)continue;
            //只隐藏scalarBarWidget，不更改hideScalarBarWidget在property中的是否显示标签
            postProperty->hideScalarBarWidget();
        }

        //判断当前数据scalarBarWidget是否显示
        if (currentProperty && (currentFieldName != SolidColor) && currentData && currentData->isEnable()) {
            _actionShowColorMap->setEnabled(true);
            _actionColorLibEdit->setEnabled(true);
            if (isShow) {
                _actionEditColorMap->setEnabled(true);
                currentProperty->setScalarBarWidgetIsShow(true);
                _actionShowColorMap->setChecked(true);
            }
            else {
                currentProperty->setScalarBarWidgetIsShow(false);
                _actionShowColorMap->setChecked(false);
            }
        }

        //刷新
        reRender();
    }

    void ToolBarColorMapper::updateDisplayComboBox()
    {
        QSignalBlocker block(_disPlayCombox);
        _disPlayCombox->setEnabled(false);
        _disPlayCombox->setCurrentIndex(2);
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        if (currentGraphObj == nullptr)return;
        if (!currentData->isEnable())return;
        _disPlayCombox->setEnabled(true);
        Interface::PostGraphProperty* property = currentGraphObj->getProperty();
        Interface::FITKPostDisplayType type = property->getCurrentDisplayType();
        _disPlayCombox->setCurrentIndex(_disPlayCombox->findData(QVariant::fromValue(type)));
    }

    void ToolBarColorMapper::updateVariable()
    {
        QSignalBlocker block(_variableCombox);
        _variableCombox->setEnabled(false);
        _variableCombox->clear();
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        if (currentGraphObj == nullptr)return;
        //当前数据是否显示
        if (!currentData->isEnable())return;
        _variableCombox->setEnabled(true);

        //获取当前的场数据
        Interface::FITKPostFieldType currentFieldType;
        QString currentFieldName = "";
        int currentCompIndex = -1;
        int currentIndex = 0;
        if (currentGraphObj->getProperty()) {
            currentGraphObj->getProperty()->getCurrentField(currentFieldType, currentFieldName, currentCompIndex);
        }

        int index = 0;
        _variableCombox->addItem(QIcon("://icons/solidColor.png"), SolidColor,QVariant::fromValue(Interface::FITKPostFieldType::Post_None));
        if (currentFieldName == SolidColor && currentFieldType == Interface::FITKPostFieldType::Post_None)currentIndex = index;

        QStringList pointData = currentData->getPointDataArray();
        for (auto array : pointData) {
            index++;
            _variableCombox->addItem(QIcon("://icons/nodeValue.png"), array, QVariant::fromValue(Interface::FITKPostFieldType::Post_Point));
            if (currentFieldName == array && currentFieldType == Interface::FITKPostFieldType::Post_Point)currentIndex = index;
        }

        QStringList cellData = currentData->getCellDataArray();
        for (auto array : cellData) {
            index++;
            _variableCombox->addItem(QIcon("://icons/cellValue.png"), array, QVariant::fromValue(Interface::FITKPostFieldType::Post_Cell));
            if (currentFieldName == array && currentFieldType == Interface::FITKPostFieldType::Post_Cell)currentIndex = index;
        }
        // 找到所有项目中最长的文本宽度
        int maxWidth = 0;
        QFontMetrics fm = _variableCombox->fontMetrics();
        for (int i = 0; i < _variableCombox->count(); ++i) {
            int width = fm.horizontalAdvance(_variableCombox->itemText(i));
            if (width > maxWidth) {
                maxWidth = width;
            }
        }

        // 加上额外的边距和下拉箭头宽度
        _variableCombox->setMinimumWidth(maxWidth + 50);
        _variableCombox->setCurrentIndex(currentIndex);
    }

    void ToolBarColorMapper::updateComponent()
    {
        QSignalBlocker block(_componentCombox);
        _componentCombox->setEnabled(false);
        _componentCombox->clear();
        Interface::PostGraphObjectBase* currentGraphObj = _postGraphManager->getGraphObjectBase(_currentDataID);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        if (currentGraphObj == nullptr)return;
        //当前数据是否是显示状态
        if (!currentData->isEnable())return;
        Interface::PostGraphProperty* postProperty = currentGraphObj->getProperty();
        if (postProperty == nullptr)return;

        //获取当前的场数据
        Interface::FITKPostFieldType currentFieldType;
        QString currentFieldName = "";
        int currentCompIndex = -1;
        if (postProperty) {
            postProperty->getCurrentField(currentFieldType, currentFieldName, currentCompIndex);
        }

        Interface::FITKPostVariableType type = currentData->getVariableType(currentFieldType, currentFieldName);
        switch (type){
        case Interface::FITKPostVariableType::Post_Error:
        case Interface::FITKPostVariableType::Post_Scalar:
            break;
        case Interface::FITKPostVariableType::Post_Vector: {
            _componentCombox->addItem("magnitude", -1);
            _componentCombox->addItem("X", 0);
            _componentCombox->addItem("Y", 1);
            _componentCombox->addItem("Z", 2);
            _componentCombox->setEnabled(true);
            break;
        }
        case Interface::FITKPostVariableType::Post_Tensor: {
            int n = currentData->getVariableComponentCount(currentFieldType, currentFieldName);
            for (int i = 1; i <= n; ++i) {
                _componentCombox->addItem(QString("%1").arg(i), i - 1);
            }
            _componentCombox->setEnabled(true);
            break;
        }
        }

        if (_componentCombox->count() > 0) {
            _componentCombox->setCurrentIndex(currentCompIndex + 1);
        }
    }
}
