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
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentFactory.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCCInterface.h"
#include "FITK_Component/FITKRenderWindowOCC/FITKGraph3DWindowOCC.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace GUI
{
    RenderWidget::RenderWidget(QWidget* parent) : QWidget(parent)
    {
        // 如果当前没有FITK应用程序实例，则不执行后续操作
        if (!FITKAPP)
            return;

        // 尝试获取名为"Graph3DWindow"的三维渲染组件
        AppFrame::FITKComponentInterface *graph3DCompBase =
            FITKAPP->getComponents()->getComponentByName("Graph3DWindowOCC");
        auto graph3DComp = dynamic_cast<Render::FITKGraph3DWindowOCCInterface *>(graph3DCompBase);
        // 如果获取失败，则不执行后续操作
        if (nullptr == graph3DComp)
            return;

        // 获取三维渲染组件的界面，并将其作为一个子窗口添加到MDI区域
        m_Graph3DWidget = dynamic_cast<Core::FITKAbstractGraph3DWidget*>(graph3DComp->getWidget(1));
        QGridLayout* layout = new QGridLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_Graph3DWidget);
        this->setLayout(layout);
    }

    Core::FITKAbstractGraph3DWidget * RenderWidget::getGraph3DWidget() const
    {
        return m_Graph3DWidget;
    }

}
