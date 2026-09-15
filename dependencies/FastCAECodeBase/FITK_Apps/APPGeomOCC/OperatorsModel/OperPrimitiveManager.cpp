/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperPrimitiveManager.h"

#include "GUIDialog/PrimitivesBoxInfoDialog.h"
#include "GUIDialog/PrimitivesCylinderInfoDialog.h"
#include "GUIDialog/PrimitivesSphereInfoDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    /** 方盒子 */
    /*@{*/
    void OperPrimitiveBoxManager::showDialog()
    {
        initDialog<GUI::PrimitivesBoxInfoDialog>();
    }

    bool OperPrimitiveBoxManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double locX, locY, locZ, width, height, length;
        bool flag = argValue("LocX", locX);
        flag &= argValue("LocY", locY);
        flag &= argValue("LocZ", locZ);
        flag &= argValue("Width", width);
        flag &= argValue("Height", height);
        flag &= argValue("Length", length);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelBox>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBox);
        // 更新命令数据
        double point[3] = { locX, locY, locZ };
        saveCmd->setPoint1(point);
        double lengths[3] = { width, height, length };
        saveCmd->setLength(lengths);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelBox* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelBox*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Box-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            double xyz[3];
            saveCmd->getPoint1(xyz);
            originCmd->setPoint1(xyz);
            saveCmd->getLength(xyz);
            originCmd->setLength(xyz);
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


    /** 球 */
    /*@{*/
    void OperPrimitiveSphereManager::showDialog()
    {
        initDialog<GUI::PrimitivesSphereInfoDialog>();
    }

    bool OperPrimitiveSphereManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double locX, locY, locZ, radius;
        bool flag = argValue("LocX", locX);
        flag &= argValue("LocY", locY);
        flag &= argValue("LocZ", locZ);
        flag &= argValue("Radius", radius);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelSphere>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSphere);
        // 更新命令数据
        double loc[3] = { locX, locY, locZ };
        saveCmd->setLocation(loc);
        saveCmd->setRadius(radius);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelSphere* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Sphere-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            double loc[3];
            saveCmd->getLocation(loc);
            originCmd->setLocation(loc);
            originCmd->setRadius(saveCmd->getRadius());
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


    /** 圆柱 */
    /*@{*/
    void OperPrimitiveCylinderManager::showDialog()
    {
        initDialog<GUI::PrimitivesCylinderInfoDialog>();
    }

    bool OperPrimitiveCylinderManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double locX, locY, locZ, axisX, axisY, axisZ, radius, height;
        bool flag = argValue("LocX", locX);
        flag &= argValue("LocY", locY);
        flag &= argValue("LocZ", locZ);
        flag &= argValue("AxisX", axisX);
        flag &= argValue("AxisY", axisY);
        flag &= argValue("AxisZ", axisZ);
        flag &= argValue("Radius", radius);
        flag &= argValue("Height", height);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelCylinder>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCylinder);
        // 更新命令数据
        double loc[3] = { locX, locY, locZ };
        saveCmd->setLocation(loc);

        double axis[3] = { axisX, axisY, axisZ };
        saveCmd->setDirection(axis);

        saveCmd->setRadius(radius);
        saveCmd->setLength(height);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelCylinder* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelCylinder*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Cylinder-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            double xyz[3];
            saveCmd->getLocation(xyz);
            originCmd->setLocation(xyz);

            saveCmd->getDirection(xyz);
            originCmd->setDirection(xyz);

            originCmd->setRadius(saveCmd->getRadius());
            originCmd->setLength(saveCmd->getLength());

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
