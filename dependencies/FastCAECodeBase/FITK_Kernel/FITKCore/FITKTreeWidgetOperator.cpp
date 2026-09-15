/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTreeWidgetOperator.h"

namespace Core
{
    FITKTreeWidgetOperator::~FITKTreeWidgetOperator()
    {
    }

    void FITKTreeWidgetOperator::on_itemClicked(QTreeWidget* w, QTreeWidgetItem* item, int col /*= 0*/)
    {
         //虚函数，不做任何操作
    }

    void FITKTreeWidgetOperator::on_itemDoubleClicked(QTreeWidget* w, QTreeWidgetItem* item, int col /*= 0*/)
    {
       //虚函数，不做任何操作
    }

}
