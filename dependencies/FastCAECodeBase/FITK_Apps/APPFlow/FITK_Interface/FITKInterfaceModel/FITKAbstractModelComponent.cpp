/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractModelComponent.h"
#include "FITKAbstractModel.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKCore/FITKColorMap.hpp"

namespace Interface
{
    int Interface::FITKAbstractModelComponent::getAbsModelID() const
    {
        return _absModelID;
    }

    FITKAbstractModelComponent::FITKAbstractModelComponent()
    {
        static unsigned int instanceCount = 0;
        _color = Core::GetcAColorFromColorMap(instanceCount);
        instanceCount++;
    }

    FITKAbstractModelComponent::~FITKAbstractModelComponent()
    {
    }

    void FITKAbstractModelComponent::setModel(const int & parentID)
    {
        //判断parentID的有效性
        //数据块被释放得到空指针
        //数据块不是FITKAbstractModel派生类对象返回空指针
        auto model = FITKDATAREPO->getTDataByID<FITKAbstractModel>(parentID);
        if (model == nullptr) return;

        //通过ID记录model
        _absModelID = parentID;
    }

    void FITKAbstractModelComponent::setModel(FITKAbstractModel* model)
    {
        if (model == nullptr) return;
        _absModelID = model->getDataObjectID();
    }

    Interface::FITKAbstractModel* FITKAbstractModelComponent::getAbstractModel()
    {
        return FITKDATAREPO->getTDataByID<FITKAbstractModel>(_absModelID);
    }

    void FITKAbstractModelComponent::setColor(const QColor &color)
    {
        _color = color;
    }
    QColor FITKAbstractModelComponent::getColor() const
    {
        return _color;
    }
}



