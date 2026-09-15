/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

//#define OCCRENDERMODE
#include <QVariant>
#include <QStackedLayout>
#include <QPushButton>
#include "RenderWidget.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QMdiSubWindow>
#include "FITK_Component/FITKWidget/FITKMdiArea.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUI
{
    RenderWidget::RenderWidget(QWidget* parent) :
        Core::FITKWidget(parent)
    {
        init();
        addGraphWidget("Render_1", 1);
    }

    void RenderWidget::init()
    {
        _gridLayout = new QVBoxLayout(this);
        _mdiArea = new Comp::FITKMdiArea(this);
        _gridLayout->addWidget(_mdiArea);
    }

    int RenderWidget::addGraphWidget(QString title, int index)
    {
        // 如果当前没有FITK应用程序实例，则不执行后续操作
        if (!FITKAPP)return -1;

        // 尝试获取名为"Graph3DWindow"的三维渲染组件
        AppFrame::FITKComponentInterface *graph3DCompBase = FITKAPP->getComponents()->getComponentByName("Graph3DWindowVTK");
        auto graph3DComp = dynamic_cast<Comp::FITKGraph3DWindowInterface *>(graph3DCompBase);
        // 如果获取失败，则不执行后续操作
        if (nullptr == graph3DComp)return -1;

        // 获取三维渲染组件的界面，并将其作为一个子窗口添加到MDI区域
        QWidget *graph3DWidget = graph3DComp->getWidget(index);
        Comp::FITKGraph3DWindowVTK* graph3DVTKWidget = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(graph3DWidget);
        if (graph3DVTKWidget != nullptr) {
            graph3DVTKWidget->setIsShowActions(true);
            graph3DVTKWidget->SetGlobalWarningDisplay(false);
            _widgetList.append(graph3DVTKWidget);
        }
        int id = _mdiArea->addSubWidget(graph3DWidget, title);
        _mdiArea->setLayoutType(_vportsType);
        return id;
    }

    int RenderWidget::addWidget(QWidget * widget, QString title)
    {
        if (_mdiArea == nullptr) return -1;
        int id = _mdiArea->addSubWidget(widget, title);
        _mdiArea->setLayoutType(_vportsType);
        return id;
    }

    Comp::FITKMdiArea * RenderWidget::getMdiArea() const
    {
        return _mdiArea;
    }

    void RenderWidget::setVportsType(Comp::FITKVportsLayoutType type)
    {
        if (_mdiArea == nullptr)return;
        _vportsType = type;
        _mdiArea->setLayoutType(_vportsType);
    }

    Comp::FITKVportsLayoutType RenderWidget::getVportsType()
    {
        return _vportsType;
    }

    QWidget* RenderWidget::getRenderWidget(int index)
    {
        if (index < 0 || index > _widgetList.size())return nullptr;
        return _widgetList[index];
    }

    //QWidget * RenderWidget::getCurrentWidget()
    //{
    //    if (_mdiArea == nullptr)return nullptr;
    //    return _mdiArea->getCurrentWidget();
    //}
}
