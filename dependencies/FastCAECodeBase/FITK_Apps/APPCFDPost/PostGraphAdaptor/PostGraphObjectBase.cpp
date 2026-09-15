/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PostGraphObjectBase.h"
#include "PostGraphProperty.h"

#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowVTK.h"
#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkProp.h>
#include <vtkScalarBarWidget.h>

#include <QDebug>
namespace Interface
{
    PostGraphObjectBase::PostGraphObjectBase(FITKAbstractCFDPostData* postData, Comp::FITKGraph3DWindowVTK* graph3DWidget) :
        Comp::FITKGraphObjectVTK(postData), _postData(postData), _currentWidget(graph3DWidget)
    {
        if (_postData == nullptr)return;
        _postData->setScalarBarWidgetInteractor(graph3DWidget->getVTKRenderWindow()->GetInteractor());
        _property = new PostGraphProperty(this, _postData);
    }

    PostGraphObjectBase::~PostGraphObjectBase()
    {
        if (_property) {
            delete _property;
            _property = nullptr;
        }
    }

    void PostGraphObjectBase::create()
    {
        QList<vtkMapper*> mappers = _postData->getMappers();
        for (vtkMapper* mapper : mappers) {
            if (mapper == nullptr)continue;
            vtkActor* actor = vtkActor::New();
            actor->SetMapper(mapper);
            this->addActor(actor);
        }

        vtkDataSetMapper* featureMapper = _postData->getFeatureEdgeMapper();
        if (featureMapper == nullptr) {
            return;
        }
        _featureActor = vtkActor::New();
        _featureActor->SetMapper(featureMapper);
        _featureActor->SetVisibility(false);
        _featureActor->SetPickable(false);
        this->addActor(_featureActor);
    }

    int PostGraphObjectBase::getPostID()
    {
        if (_postData == nullptr)return -1;
        return _postData->getDataObjectID();
    }

    PostGraphProperty * PostGraphObjectBase::getProperty()
    {
        return _property;
    }

    QList<vtkProp*> PostGraphObjectBase::getActors()
    {
        return m_actorList;
    }

    void PostGraphObjectBase::setIsShow(bool isShow)
    {
        if (_postData == nullptr)return;
        vtkScalarBarWidget* scalarBarWidget = _postData->getScalarBarWidget();
        if (scalarBarWidget == nullptr)return;
        if (isShow == _postData->isEnable())return;
        _postData->enable(isShow);

        //隐藏所有actor
        for (int i = 0; i < m_actorList.size(); i++) {
            vtkProp* actor = m_actorList[i];
            if (isShow) actor->SetVisibility(true);
            else actor->SetVisibility(false);
        }

        //隐藏标量显示界面
        if (isShow) {
            bool scalarBarWidgetShowType = _property->getScalarBarWidgetIsShow();
            if (scalarBarWidgetShowType)scalarBarWidget->On();
            else scalarBarWidget->Off();
        }
        else {
            scalarBarWidget->Off();
        }
    }

    int PostGraphObjectBase::getRenderLayer()
    {
        return 0;
    }

    void PostGraphObjectBase::updateRotateVis(bool isVis)
    {
        if (_postData == nullptr) {
            return;
        }

        for (auto actor : m_actorList) {
            //判断是否是特征演员
            if (actor == _featureActor) {
                continue;
            }

            if (_postData->isEnable() == true) {
                if (isVis == true) {
                    actor->SetVisibility(true);
                }
                else {
                    actor->SetVisibility(false);
                }
            }
            else {
                actor->SetVisibility(false);
            }
        }

        if (_featureActor) {
            if (_postData->isEnable() == true) {
                if (isVis == true) {
                    _featureActor->SetVisibility(false);
                }
                else {
                    _featureActor->SetVisibility(true);
                }
            }
            else {
                _featureActor->SetVisibility(false);
            }
        }
    }
}

