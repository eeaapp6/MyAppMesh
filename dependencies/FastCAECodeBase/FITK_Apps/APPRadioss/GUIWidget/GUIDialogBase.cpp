/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIDialogBase.h"

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "OperatorsInterface/GraphEventOperator.h"

namespace GUI
{
    GUIDialogBase::GUIDialogBase(Core::FITKAbstractDataObject* data,QWidget* parent) :
        Core::FITKDialog(parent),_data(data)
    {
        //设置窗口关闭自动销毁
        setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
        //去掉问号
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::WindowContextHelpButtonHint;
        setWindowFlags(flags);
        //根据数据对象是否存在，设置创建标志
        if (_data) {
            _isCreate = false;
        }
        else {
            _isCreate = true;
        }
    }

    GUIDialogBase::~GUIDialogBase()
    {

    }

    void GUIDialogBase::hightGraphObj(const int& objID)
    {
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) {
            //operGraph->highlightGraph(objID);
        }
    }

    void GUIDialogBase::disHightGraphObj(const int & objID)
    {
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) {
            //operGraph->clearPreHighlight(objID);
        }
    }

    void GUIDialogBase::highlightPartGeomTopo(const int& objID, const int& topoID)
    {
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) {
            //operGraph->highlightGeomTopos(QList<int>() << topoID, objID);
        }
    }

    void GUIDialogBase::highlightMeshData(const int& objID, const int& elemID, const int& type)
    {
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) {
            //operGraph->highlightMeshData(QList<int>() << elemID, objID, type);
        }
    }

    void GUIDialogBase::clearAllHightGraph()
    {
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) {
            //operGraph->clearHighlight();
        }
    }

    void GUIDialogBase::reject()
    {
        //如果是创建模式且数据对象存在，则删除数据对象
        if (_isCreate && _data)
        {
            delete _data;
            _data = nullptr;
        }
        Core::FITKDialog::reject();
    }

    void GUIDialogBase::accept()
    {
        Core::FITKDialog::accept();
    }
}