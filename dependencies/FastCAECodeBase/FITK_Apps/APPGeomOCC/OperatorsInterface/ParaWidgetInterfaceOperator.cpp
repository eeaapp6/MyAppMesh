/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ParaWidgetInterfaceOperator.h"

namespace EventOper
{
    void ParaWidgetInterfaceOper::finalize(bool updateCommand)
    {
        // 根据不同子类重写。
        Q_UNUSED(updateCommand);
    }
}