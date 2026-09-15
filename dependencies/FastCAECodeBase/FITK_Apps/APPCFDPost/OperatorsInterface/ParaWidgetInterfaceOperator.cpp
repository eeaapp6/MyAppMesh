/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ParaWidgetInterfaceOperator.h"


namespace EventOper
{
    ParaWidgetInterfaceOperator::ParaWidgetInterfaceOperator()
    {

    }

    ParaWidgetInterfaceOperator::~ParaWidgetInterfaceOperator()
    {

    }

    void ParaWidgetInterfaceOperator::eventProcess(int index, QHash<int, QVariant> value)
    {
        Q_UNUSED(index);
        Q_UNUSED(value);
    }

    void ParaWidgetInterfaceOperator::CurrentObjectChange(int objID)
    {
        Q_UNUSED(objID);
    }

    void ParaWidgetInterfaceOperator::updateTree()
    {

    }
}
