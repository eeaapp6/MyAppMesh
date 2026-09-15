/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperBoolManager.h"
#include "GUIDialog/BoolOperatorInfoDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    /** 布尔求交 */
    /*@{*/
    void OperBoolIntersectManager::showDialog()
    {
        auto dlg = initDialog<GUI::BoolOperatorInfoDialog>();
        dlg->setBoolType(Interface::FITKAbsGeoOperBool::GeoBoolOperType::GBTCommon);
    }

    bool OperBoolIntersectManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape targetShape, toolShape;
        bool flag = argValue("TargetShape", targetShape);
        flag &= argValue("ToolShape", toolShape);
        if (!flag) return false;
        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoOperBool>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool);
        // 更新命令数据
        saveCmd->setTarget(targetShape);
        saveCmd->setTool(toolShape);
        saveCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GBTCommon);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoOperBool* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Intersect-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setTarget(saveCmd->target());
            originCmd->setTool(saveCmd->tool());
            originCmd->setBoolOperType(saveCmd->getBoolOperType());
            originCmd->update();
            delete saveCmd;
            saveCmd = originCmd;
        }

        // 移除命令引用并添加新命令引用。
        saveCmd->clearReference();
        saveCmd->addReferenceCmdID(targetShape.CmdId);
        if (targetShape.CmdId != toolShape.CmdId) {
            saveCmd->addReferenceCmdID(toolShape.CmdId);
        }
        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/


    /** 布尔求并 */
    /*@{*/
    void OperBoolUniteManager::showDialog()
    {
        auto dlg = initDialog<GUI::BoolOperatorInfoDialog>();
        dlg->setBoolType(Interface::FITKAbsGeoOperBool::GeoBoolOperType::GBTAdd);
    }

    bool OperBoolUniteManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape targetShape, toolShape;
        bool flag = argValue("TargetShape", targetShape);
        flag &= argValue("ToolShape", toolShape);
        if (!flag) return false;
        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoOperBool>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool);
        // 更新命令数据
        saveCmd->setTarget(targetShape);
        saveCmd->setTool(toolShape);
        saveCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GBTAdd);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoOperBool* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Unite-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setTarget(saveCmd->target());
            originCmd->setTool(saveCmd->tool());
            originCmd->setBoolOperType(saveCmd->getBoolOperType());
            originCmd->update();
            delete saveCmd;
            saveCmd = originCmd;
        }

        // 移除命令引用并添加新命令引用。
        saveCmd->clearReference();
        saveCmd->addReferenceCmdID(targetShape.CmdId);
        if (targetShape.CmdId != toolShape.CmdId) {
            saveCmd->addReferenceCmdID(toolShape.CmdId);
        }
        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/


    /** 布尔求差 */
    /*@{*/
    void OperBoolSubtractManager::showDialog()
    {
        auto dlg = initDialog<GUI::BoolOperatorInfoDialog>();
        dlg->setBoolType(Interface::FITKAbsGeoOperBool::GeoBoolOperType::GBTCut);
    }

    bool OperBoolSubtractManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape targetShape, toolShape;
        bool flag = argValue("TargetShape", targetShape);
        flag &= argValue("ToolShape", toolShape);
        if (!flag) return false;
        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoOperBool>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBool);
        // 更新命令数据
        saveCmd->setTarget(targetShape);
        saveCmd->setTool(toolShape);
        saveCmd->setBoolOperType(Interface::FITKAbsGeoOperBool::GBTCut);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoOperBool* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Subtract-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setTarget(saveCmd->target());
            originCmd->setTool(saveCmd->tool());
            originCmd->setBoolOperType(saveCmd->getBoolOperType());
            originCmd->update();
            delete saveCmd;
            saveCmd = originCmd;
        }

        // 移除命令引用并添加新命令引用。
        saveCmd->clearReference();
        saveCmd->addReferenceCmdID(targetShape.CmdId);
        if (targetShape.CmdId != toolShape.CmdId) {
            saveCmd->addReferenceCmdID(toolShape.CmdId);
        }
        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/
} // namespace ModelOper
