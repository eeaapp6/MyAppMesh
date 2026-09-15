/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperSolidManager.h"

#include "GUIDialog/SolidClosedSurfaceInfoDialog.h"
#include "GUIDialog/SolidExtrudeInfoDialog.h"
#include "GUIDialog/SolidRevolInfoDialog.h"
#include "GUIDialog/SolidSweepInfoDialog.h"
#include "GUIDialog/SolidMultiSectionInfoDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    /** 闭合面段形成体 */
    /*@{*/
    void OperSolidClosedSurfaceManager::showDialog()
    {
        initDialog<GUI::SolidClosedSurfaceInfoDialog>();
    }

    bool OperSolidClosedSurfaceManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        QList<Interface::VirtualShape> sourceShapes;
        bool flag = argValue("SourceShapes", sourceShapes);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelClosedSurfaceSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTClosedSurfaceSolid);
        // 更新命令数据
        saveCmd->setFaces(sourceShapes);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelClosedSurfaceSolid* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelClosedSurfaceSolid*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ClosedSurfaceSolid-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setFaces(saveCmd->faces());
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


    /** 拉伸形成体 */
    /*@{*/
    void OperSolidExtrudeManager::showDialog()
    {
        initDialog<GUI::SolidExtrudeInfoDialog>();
    }

    bool OperSolidExtrudeManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape;
        double dx, dy, dz, length;
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("Dx", dx);
        flag &= argValue("Dy", dy);
        flag &= argValue("Dz", dz);
        flag &= argValue("Length", length);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeSolid);
        // 更新命令数据
        saveCmd->setSourceSurface(sourceShape);
        saveCmd->setDirection(dx, dy, dz);
        saveCmd->setLength(length);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelExtrudeSolid* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelExtrudeSolid*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ExtrudeSolid-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceSurface(saveCmd->sourceSurface());
            originCmd->setDirection(saveCmd->direction());
            originCmd->setLength(saveCmd->length());
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


    /** 旋转形成体 */
    /*@{*/
    void OperSolidRevolManager::showDialog()
    {
        initDialog<GUI::SolidRevolInfoDialog>();
    }

    bool OperSolidRevolManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape;
        double p1x, p1y, p1z, p2x, p2y, p2z, angle;
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        flag &= argValue("Angle", angle);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelRevolSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRevolSolid);
        // 更新命令数据
        saveCmd->setSourceSurface(sourceShape);
        saveCmd->setRotateAxisPoint1(p1x, p1y, p1z);
        saveCmd->setRotateAxisPoint2(p2x, p2y, p2z);
        saveCmd->setAngle(angle);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelRevolSolid* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelRevolSolid*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("RevolSolid-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceSurface(saveCmd->sourceSurface());
            originCmd->setRotateAxisPoint1(saveCmd->rotateAxisPoint1());
            originCmd->setRotateAxisPoint2(saveCmd->rotateAxisPoint2());
            originCmd->setAngle(saveCmd->angle());
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


    /** 扫略形成体 */
    /*@{*/
    void OperSolidSweepManager::showDialog()
    {
        initDialog<GUI::SolidSweepInfoDialog>();
    }

    bool OperSolidSweepManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape profileShape, curveShape;
        bool flag = argValue("ProfileShape", profileShape);
        flag &= argValue("CurveShape", curveShape);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSolid);
        // 更新命令数据
        saveCmd->setProfile(profileShape);
        saveCmd->setCurve(curveShape);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelSweepSolid* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelSweepSolid*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("SweepSolid-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setProfile(saveCmd->profile());
            originCmd->setCurve(saveCmd->curve());
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


    /** 多截面扫略形成体 */
    /*@{*/
    void OperSolidMultiSectionManager::showDialog()
    {
        initDialog<GUI::SolidMultiSectionInfoDialog>();
    }

    bool OperSolidMultiSectionManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        QList<Interface::VirtualShape> sections;
        bool flag = argValue("Sections", sections);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelMultiSectionSolid>(Interface::FITKGeoEnum::FITKGeometryComType::FGTMultiSectionSolid);
        // 更新命令数据
        saveCmd->setSections(sections);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelMultiSectionSolid* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelMultiSectionSolid*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("MultiSectionSolid-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSections(saveCmd->sections());
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


} // namespace ModelOper
