/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperRegionMeshSize.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "OperatorsInterface/GraphEventOperator.h"

#include "FITK_Component/FITKGmshExeDriver/GUIRegionMeshSizeBoxDialog.h"
#include "FITK_Component/FITKGmshExeDriver/GUIRegionMeshSizeCylinderDialog.h"
#include "FITK_Component/FITKGmshExeDriver/GUIRegionMeshSizeSphereDialog.h"
#include <QWidget>

bool ModelOper::OperRegionMeshSize::execGUI(){
    //获取动作名称
    QString actionName = _emitter->objectName();
    if (actionName == "actionRegionMeshSizeBox")
    {
        this->updateMeshSizeBox();
    }
    else if (actionName == "actionRegionMeshSizeCylinder")
    {
        this->updateMeshSizeCylinder();
    }
    else if (actionName == "actionRegionMeshSizeSphere")
    {
        this->updateMeshSizeSphere();
    }
    return false;
}

bool ModelOper::OperRegionMeshSize::execProfession()
{
    return true;
}
bool ModelOper::OperRegionMeshSize::updateMeshSizeBox()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIRegionMeshSizeBoxDialog* dialog = new GUI::GUIRegionMeshSizeBoxDialog(mw);
    dialog->show();
    return true;
}
bool ModelOper::OperRegionMeshSize::updateMeshSizeCylinder()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIRegionMeshSizeCylinderDialog* dialog = new GUI::GUIRegionMeshSizeCylinderDialog(mw);
    dialog->show();
    return true;
}
bool ModelOper::OperRegionMeshSize::updateMeshSizeSphere()
{
    //主界面
    QWidget* mw = FITKAPP->getGlobalData()->getMainWindow();
    if (!mw) return false;
    //创建界面
    GUI::GUIRegionMeshSizeSphereDialog* dialog = new GUI::GUIRegionMeshSizeSphereDialog(mw);
    dialog->show();
    return true;
}
