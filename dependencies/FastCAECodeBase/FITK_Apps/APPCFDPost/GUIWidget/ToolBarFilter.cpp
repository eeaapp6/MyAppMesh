/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ToolBarFilter.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "PostGraphAdaptor/PostGraphObjectBase.h"
#include "PostGraphAdaptor/PostGraphProperty.h"
#include "PostGraphAdaptor/PostGraphObjectManager.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPost3DManager.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKCFDPostData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUI
{
    ToolBarFilter::ToolBarFilter(QWidget* parent) :
        ToolBarBase(parent)
    {
        _isoSurfAction = createAction("actionIsosurfCreate", "://icons/isosurf.png");
        this->addAction(_isoSurfAction);
        _vectorfAction = createAction("actionGlyphCreate", "://icons/vector.png");
        this->addAction(_vectorfAction);
        _clipAction = createAction("actionClipCreate", "://icons/clip.png");
        this->addAction(_clipAction); 
        _sliceAction = createAction("actionSliceCreate", "://icons/slice.png");
        this->addAction(_sliceAction);
        _streamlineSurfAction = createAction("actionStreamlineCreate", "://icons/streamline.png");
        this->addAction(_streamlineSurfAction);
        _thresholdAction = createAction("actionThresholdCreate", "");
        _thresholdAction->setText(tr("Threshold"));
        this->addAction(_thresholdAction);
        _deformAction = createAction("actionDeformCreate", "://icons/deform.svg");
        this->addAction(_deformAction);
        QAction* action = createAction("actionProbeCreate", "");
        action->setText(tr("Probe"));
        this->addAction(action);

        this->addSeparator();
        updateCurrentGraphObj(-1);
    }

    ToolBarFilter::~ToolBarFilter()
    {

    }

    void ToolBarFilter::updateCurrentGraphObj(int objID)
    {
        _currentDataID = objID;

        updateIsoSurf();
        updateVector();
        updateClip();
        updateSlice();
        updateStreamLine();
        updateThreshold();
        updateDeform();
    }

    void ToolBarFilter::updateIsoSurf()
    {
        _isoSurfAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        //判断当前数据是否可见
        if (currentData->isEnable() == false)return;

        QStringList pointList = currentData->getPointDataArray();
        QStringList contourList = {};
        //点标量数据
        for (QString point : pointList) {
            Interface::FITKPostVariableType type = currentData->getVariableType(Interface::FITKPostFieldType::Post_Point, point);
            if (type == Interface::FITKPostVariableType::Post_Scalar) contourList.append(point);
        }

        //判断当前数据是否有可用标量
        if (contourList.isEmpty() == false) {
            _isoSurfAction->setEnabled(true);
        }
    }

    void ToolBarFilter::updateVector()
    {
        _vectorfAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        //判断当前数据是否可见
        if (currentData->isEnable() == false)return;
        _vectorfAction->setEnabled(true);
    }

    void ToolBarFilter::updateClip()
    {
        _clipAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        //判断当前数据是否可见
        if (currentData->isEnable() == false)return;
        _clipAction->setEnabled(true);
    }

    void ToolBarFilter::updateSlice()
    {
        _sliceAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        //判断当前数据是否可见
        if (currentData->isEnable() == false)return;
        _sliceAction->setEnabled(true);
    }

    void ToolBarFilter::updateStreamLine()
    {
        _streamlineSurfAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        //判断当前数据是否可见
        if (currentData->isEnable() == false)return;
        
        //获取矢量
        QStringList vectorList = {};
        QStringList pointDataArray = currentData->getPointDataArray();
        for (QString point : pointDataArray) {
            Interface::FITKPostVariableType type = currentData->getVariableType(Interface::FITKPostFieldType::Post_Point, point);
            if (type == Interface::FITKPostVariableType::Post_Vector) {
                vectorList.append(point);
            }
        }
        QStringList cellDataArray = currentData->getCellDataArray();
        for (QString cell : cellDataArray) {
            Interface::FITKPostVariableType type = currentData->getVariableType(Interface::FITKPostFieldType::Post_Cell, cell);
            if (type == Interface::FITKPostVariableType::Post_Vector) {
                vectorList.append(cell);
            }
        }

        //判断当前数据是否有可用矢量
        if (vectorList.isEmpty() == false) {
            _streamlineSurfAction->setEnabled(true);
        }
    }

    void ToolBarFilter::updateThreshold()
    {
        _thresholdAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        //判断当前数据是否可见
        if (currentData->isEnable() == false)return;
        _thresholdAction->setEnabled(true);
    }

    void ToolBarFilter::updateDeform()
    {
        _deformAction->setEnabled(false);
        Interface::FITKAbstractCFDPostData* currentData = _postDataManager->getDataByID(_currentDataID);
        if (currentData == nullptr)return;
        //判断当前数据是否可见
        if (currentData->isEnable() == false)return;

        //添加变量
        int vectorNum = 0;
        QStringList pointDataArray = currentData->getPointDataArray();
        for (QString pointData : pointDataArray) {
            Interface::FITKPostVariableType type = currentData->getVariableType(Interface::FITKPostFieldType::Post_Point, pointData);
            if (type == Interface::FITKPostVariableType::Post_Error || type == Interface::FITKPostVariableType::Post_Scalar)continue;
            vectorNum++;
        }
        if (vectorNum <= 0)return;
        _deformAction->setEnabled(true);
    }
}