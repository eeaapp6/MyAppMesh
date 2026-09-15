/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GraphArea.h"
#include "ui_GraphArea.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"

namespace GUI
{
    GraphArea::GraphArea(QWidget* parent /*= nullptr*/)
        :Core::FITKWidget(parent)
    {
        _ui = new Ui::GraphArea;
        _ui->setupUi(this);

        //从组件获取3D窗口
        AppFrame::FITKComponentInterface* comp = FITKAPP->getComponents()
            ->getComponentByName("Graph3DWindowVTK");
        QWidget* w = comp->getWidget(1);

        _ui->gridLayout->addWidget(w);

        //强制类型转换
        _renderWindow = dynamic_cast<Comp::FITKGraph3DWindowVTK*>(w);

        if (_renderWindow)
        {
            double position[3]{ 1,1,1 };
            double focalPoint[3]{ 0,0,0 };
            double viewUp[3]{ 0,0,1 };
            //设置视角
            _renderWindow->setDefaultView(focalPoint, viewUp, position);
            _renderWindow->setISOView(focalPoint, viewUp, position);
            _renderWindow->setIsShowActions(true);
            _renderWindow->setIsShowClipAction(true);
            //设置关闭VTK警告
            _renderWindow->SetGlobalWarningDisplay(false);
        }
    }

     
    GraphArea::~GraphArea()
    {
        if (_renderWindow) delete _renderWindow;
        if (_ui) delete _ui;
    }

   

    GUI::GraphAreaType GraphArea::getGraphAreaType()
    {
        return GUI::GATNone;
    }

    Comp::FITKGraph3DWindowVTK* GraphArea::getRenderWindow()
    {
        return _renderWindow;
    }

    void GraphArea::reRender()
    {
        if (_renderWindow)
            _renderWindow->reRender();
    }


    void GraphArea::addObject(int index, Comp::FITKGraphObjectVTK* object, bool fitview /*= true*/)
    {
        if (_renderWindow)
            _renderWindow->addObject(index, object, fitview);
    }

    void GraphArea::clear()
    {
        if (_renderWindow)
            _renderWindow->clear();
    }

    void GraphArea::fitView()
    {
        if (_renderWindow)
            _renderWindow->fitView();
    }

    void GraphArea::flush()
    {
        if (_renderWindow)
            _renderWindow->flush();
    }

    void GraphArea::removeGraphObj(Core::FITKAbstractGraphObject* gobj)
    {
        if (_renderWindow)
            _renderWindow->removeGraphObj(gobj);
    }

    void GraphArea::saveImage(const QString& image, int w /*= -1*/, int h /*= -1*/)
    {
        if (_renderWindow)
            _renderWindow->saveImage(image, w, h);
    }

}


