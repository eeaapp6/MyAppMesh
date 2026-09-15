/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperSplitterManager.h"
#include "GUIDialog/CurveSplitterInfoDialog.h"
#include "GUIDialog/SurfaceSplitterInfoDialog.h"
#include "GUIDialog/SolidSplitterInfoDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    /** 线分割 */
    /*@{*/
    void OperCurveSplitterManager::showDialog()
    {
        initDialog<GUI::CurveSplitterInfoDialog>();
    }

    bool OperCurveSplitterManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape, toolShape;
        QList<double> interactionPoint{};
        bool isInteraction{};
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("IsInteraction", isInteraction);
        if (!flag) return false;
        if (isInteraction) {
            flag &= argValue("Point", interactionPoint);
        }
        else {
            flag &= argValue("ToolShape", toolShape);
        }
        if (!flag) return false;


        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoCurveSplitter>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveSplitter);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        if (isInteraction) {
            if (interactionPoint.size() != 3) return false;
            saveCmd->setToolPoint({ interactionPoint.at(0),interactionPoint.at(1),interactionPoint.at(2) });
        }
        else {
            saveCmd->setToolShape(toolShape);
        }

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoCurveSplitter* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoCurveSplitter*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("CurveSplitter-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setToolShape(saveCmd->toolShape());
            originCmd->update();
            delete saveCmd;
            saveCmd = originCmd;
        }

        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/


    /** 面分割 */
    /*@{*/
    void OperSurfaceSplitterManager::showDialog()
    {
        initDialog<GUI::SurfaceSplitterInfoDialog>();
    }

    bool OperSurfaceSplitterManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape, toolShape;
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("ToolShape", toolShape);
        if (!flag) return false;
        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoSurfaceSplitter>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSurfaceSplitter);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setToolShape(toolShape);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoSurfaceSplitter* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoSurfaceSplitter*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("SurfaceSplitter-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setToolShape(saveCmd->toolShape());
            originCmd->update();
            delete saveCmd;
            saveCmd = originCmd;
        }

        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/


    /** 体分割 */
    /*@{*/
    void OperSolidSplitterManager::showDialog()
    {
        initDialog<GUI::SolidSplitterInfoDialog>();
    }

    bool OperSolidSplitterManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape, toolShape;
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("ToolShape", toolShape);
        if (!flag) return false;
        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoSolidSplitter>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSolidSplitter);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setToolShape(toolShape);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoSolidSplitter* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoSolidSplitter*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("SolidSplitter-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setToolShape(saveCmd->toolShape());
            originCmd->update();
            delete saveCmd;
            saveCmd = originCmd;
        }

        // 显示对象
        auto graphPreprocessOper = Core::FITKOperatorRepo::getInstance()->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (graphPreprocessOper != nullptr) {
            graphPreprocessOper->updateGraph(saveCmd->getDataObjectID());
        }
        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/
} // namespace ModelOper
