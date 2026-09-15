/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperReferenceManager.h"

#include "GUIDialog/ReferencePointInfoDialog.h"
#include "GUIDialog/ReferenceLineInfoDialog.h"
#include "GUIDialog/ReferencePlaneInfoDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoDatum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{

    /** 参考点操作器 */
    /*@{*/
    void OperReferencePointManager::showDialog()
    {
        initDialog<GUI::ReferencePointInfoDialog, Interface::FITKAbsGeoDatumPoint>();
    }

    bool OperReferencePointManager::execProfession()
    {

        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double px, py, pz;
        bool flag = argValue("Px", px);
        flag &= argValue("Py", py);
        flag &= argValue("Pz", pz);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto datumManager = geoCmdList->getDatumManager();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createDatumT<Interface::FITKAbsGeoDatumPoint>(Interface::FITKGeoEnum::FDTPoint);
        // 更新命令数据
        double xyz1[3] = { px, py, pz };
        saveCmd->setPosition(xyz1);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoDatumPoint* originCmd{};
        if (originCmdId <= 0 || ((originCmd = dynamic_cast<Interface::FITKAbsGeoDatumPoint*>(datumManager->getDataByID(originCmdId))) == nullptr)) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("RefPoint-%1").arg(geoCmdList->getDataCount() + 1)));
            datumManager->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            double xyz[3];
            saveCmd->getPosition(xyz);
            originCmd->setPosition(xyz);
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


    /** 参考线操作器 */
    /*@{*/
    void OperReferenceLineManager::showDialog()
    {
        initDialog<GUI::ReferenceLineInfoDialog, Interface::FITKAbsGeoDatumLine>();
    }

    bool OperReferenceLineManager::execProfession()
    {

        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double p1x, p1y, p1z, p2x, p2y, p2z, dx, dy, dz;
        bool isInfinite;
        bool flag = argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        flag &= argValue("Dx", dx);
        flag &= argValue("Dy", dy);
        flag &= argValue("Dz", dz);
        flag &= argValue("IsInfinite", isInfinite);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto datumManager = geoCmdList->getDatumManager();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createDatumT<Interface::FITKAbsGeoDatumLine>(Interface::FITKGeoEnum::FDTLine);
        // 更新命令数据
        double xyz1[3] = { p1x, p1y, p1z };
        saveCmd->setPosition(xyz1);
        double xyz2[3] = { p2x, p2y, p2z };
        saveCmd->setPosition2(xyz2);
        //double xyz3[3] = { dx, dy, dz };
        //saveCmd->setDirection(xyz3);

        saveCmd->setIsInfinite(isInfinite);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoDatumLine* originCmd{};
        if (originCmdId <= 0 || ((originCmd = dynamic_cast<Interface::FITKAbsGeoDatumLine*>(datumManager->getDataByID(originCmdId))) == nullptr)) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("RefLine-%1").arg(geoCmdList->getDataCount() + 1)));
            datumManager->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            double xyz[3];
            saveCmd->getPosition(xyz);
            originCmd->setPosition(xyz);

            saveCmd->getPosition2(xyz);
            originCmd->setPosition2(xyz);

            //saveCmd->getDirection(xyz);
            //originCmd->setDirection(xyz);
            originCmd->setIsInfinite(saveCmd->getIsInfinite());
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


    /** 参考面操作器 */
    /*@{*/
    void OperReferencePlaneManager::showDialog()
    {
        initDialog<GUI::ReferencePlaneInfoDialog, Interface::FITKAbsGeoDatumPlane>();
    }

    bool OperReferencePlaneManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::FITKGeoEnum::FITKDatumType type;
        bool flag = argValue("Type", type);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();
        auto datumManager = geoCmdList->getDatumManager();
        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        Interface::FITKAbsGeoReferencePlane* saveCmd{};
        // 更新命令数据
        switch (type)
        {
            // 偏移
        case Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane:
        {
            Interface::VirtualShape sourceFace;
            double offset, dx, dy, dz;
            flag = argValue("SourceFace", sourceFace);
            flag &= argValue("Offset", offset);
            flag &= argValue("Dx", dx);
            flag &= argValue("Dy", dy);
            flag &= argValue("Dz", dz);
            if (!flag) return false;
            auto tempSaveCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceOffsetPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane);
            tempSaveCmd->setSourceSurface(sourceFace);
            tempSaveCmd->setOffset(offset);
            tempSaveCmd->setDirection(dx, dy, dz);
            saveCmd = tempSaveCmd;
            break;
        }
        // 三点
        case Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane:
        {
            double p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;
            flag = argValue("D1x", p1x);
            flag &= argValue("D1y", p1y);
            flag &= argValue("D1z", p1z);
            flag &= argValue("D2x", p2x);
            flag &= argValue("D2y", p2y);
            flag &= argValue("D2z", p2z);
            flag &= argValue("D3x", p3x);
            flag &= argValue("D3y", p3y);
            flag &= argValue("D3z", p3z);
            if (!flag) return false;
            auto tempSaveCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceThreePointsPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane);
            tempSaveCmd->setPoint(0, p1x, p1y, p1z);
            tempSaveCmd->setPoint(1, p2x, p2y, p2z);
            tempSaveCmd->setPoint(2, p3x, p3y, p3z);
            saveCmd = tempSaveCmd;
            break;
        }
        // 方程
        case Interface::FITKGeoEnum::FITKDatumType::FDTEquationPlane:
        {
            double a, b, c, d;
            flag = argValue("A", a);
            flag &= argValue("B", b);
            flag &= argValue("C", c);
            flag &= argValue("D", d);
            if (!flag) return false;
            auto tempSaveCmd = fac->createDatumT<Interface::FITKAbsGeoReferenceEquationPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTEquationPlane);
            tempSaveCmd->setA(a);
            tempSaveCmd->setB(b);
            tempSaveCmd->setC(c);
            tempSaveCmd->setD(d);
            saveCmd = tempSaveCmd;
            break;
        }
        // 点和法线
        case Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane:
        {
            double px, py, pz, dx, dy, dz;
            flag = argValue("Px", px);
            flag &= argValue("Py", py);
            flag &= argValue("Pz", pz);
            flag &= argValue("Dx", dx);
            flag &= argValue("Dy", dy);
            flag &= argValue("Dz", dz);
            if (!flag) return false;
            auto tempSaveCmd = fac->createDatumT<Interface::FITKAbsGeoReferencePointAndDirectionPlane>(Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane);
            tempSaveCmd->setPoint(px, py, pz);
            tempSaveCmd->setDirection(dx, dy, dz);
            saveCmd = tempSaveCmd;
            break;
        }
        default:
            break;
        }
        if (saveCmd == nullptr || !saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }

        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoReferencePlane* originCmd{};
        if (originCmdId <= 0 || ((originCmd = dynamic_cast<Interface::FITKAbsGeoReferencePlane*>(datumManager->getDataByID(originCmdId))) == nullptr)) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("RefPlane-%1").arg(datumManager->getDataCount() + 1)));
            datumManager->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            switch (saveCmd->getDatumType())
            {
            case Interface::FITKGeoEnum::FITKDatumType::FDTOffsetPlane:
            {
                auto tempOriginCmd = dynamic_cast<Interface::FITKAbsGeoReferenceOffsetPlane*>(originCmd);
                auto tempSaveCmd = dynamic_cast<Interface::FITKAbsGeoReferenceOffsetPlane*>(saveCmd);
                if (tempOriginCmd == nullptr || tempSaveCmd == nullptr) {
                    printLog(3, tr("System Error."));
                    delete saveCmd;
                    return false;
                }
                tempOriginCmd->setSourceSurface(tempSaveCmd->sourceSurface());
                tempOriginCmd->setOffset(tempSaveCmd->offset());
                tempOriginCmd->setDirection(tempSaveCmd->direction());
                delete tempSaveCmd;
                saveCmd = tempOriginCmd;
                break;
            }
            case Interface::FITKGeoEnum::FITKDatumType::FDTThreePointsPlane:
            {
                auto tempOriginCmd = dynamic_cast<Interface::FITKAbsGeoReferenceThreePointsPlane*>(originCmd);
                auto tempSaveCmd = dynamic_cast<Interface::FITKAbsGeoReferenceThreePointsPlane*>(saveCmd);
                if (tempOriginCmd == nullptr || tempSaveCmd == nullptr) {
                    printLog(3, tr("System Error."));
                    delete saveCmd;
                    return false;
                }
                for (int i = 0; i < 3; ++i) {
                    tempOriginCmd->setPoint(i, tempSaveCmd->point(i));
                }
                delete tempSaveCmd;
                saveCmd = tempOriginCmd;
                break;
            }
            case Interface::FITKGeoEnum::FITKDatumType::FDTEquationPlane:
            {
                auto tempOriginCmd = dynamic_cast<Interface::FITKAbsGeoReferenceEquationPlane*>(originCmd);
                auto tempSaveCmd = dynamic_cast<Interface::FITKAbsGeoReferenceEquationPlane*>(saveCmd);
                if (tempOriginCmd == nullptr || tempSaveCmd == nullptr) {
                    printLog(3, tr("System Error."));
                    delete saveCmd;
                    return false;
                }
                tempOriginCmd->setA(tempSaveCmd->a());
                tempOriginCmd->setB(tempSaveCmd->b());
                tempOriginCmd->setC(tempSaveCmd->c());
                tempOriginCmd->setD(tempSaveCmd->d());
                delete tempSaveCmd;
                saveCmd = tempOriginCmd;
                break;
            }
            case Interface::FITKGeoEnum::FITKDatumType::FDTPointAndDirectionPlane:
            {
                auto tempOriginCmd = dynamic_cast<Interface::FITKAbsGeoReferencePointAndDirectionPlane*>(originCmd);
                auto tempSaveCmd = dynamic_cast<Interface::FITKAbsGeoReferencePointAndDirectionPlane*>(saveCmd);
                if (tempOriginCmd == nullptr || tempSaveCmd == nullptr) {
                    printLog(3, tr("System Error."));
                    delete saveCmd;
                    return false;
                }
                tempOriginCmd->setPoint(tempSaveCmd->point());
                tempOriginCmd->setDirection(tempSaveCmd->direction());
                delete tempSaveCmd;
                saveCmd = tempOriginCmd;
                break;
            }
            default:
                return false;
                break;
            }
            if (!saveCmd->update()) return false;
        }

        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/

} // namespace ModelOper
