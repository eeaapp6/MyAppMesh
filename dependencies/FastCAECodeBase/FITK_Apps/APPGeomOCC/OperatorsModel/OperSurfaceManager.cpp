/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperSurfaceManager.h"

#include "GUIDialog/SurfaceClosedWireInfoDialog.h"
#include "GUIDialog/SurfaceOffsetInfoDialog.h"
#include "GUIDialog/SurfaceExtrudeInfoDialog.h"
#include "GUIDialog/SurfaceRevolInfoDialog.h"
#include "GUIDialog/SurfaceSweepInfoDialog.h"
#include "GUIDialog/SurfaceBridgeSurfaceInfoDialog.h"
#include "GUIDialog/SurfaceSurfaceFromSolidInfoDialog.h"
#include "GUIDialog/SurfaceMultiSectionInfoDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    /** 闭合线段形成曲面 */
    /*@{*/
    void OperSurfaceClosedWireManager::showDialog()
    {
        initDialog<GUI::SurfaceClosedWireInfoDialog>();
    }

    bool OperSurfaceClosedWireManager::execProfession()
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
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelClosedWireSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTClosedWireSurface);
        // 更新命令数据
        saveCmd->setEdges(sourceShapes);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelClosedWireSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelClosedWireSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ClosedWireSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setEdges(saveCmd->edges());
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


    /** 偏移形成曲面 */
    /*@{*/
    void OperSurfaceOffsetManager::showDialog()
    {
        initDialog<GUI::SurfaceOffsetInfoDialog>();
    }

    bool OperSurfaceOffsetManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape;
        double offset;
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("Offset", offset);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelOffsetSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTOffsetSurface);
        // 更新命令数据
        saveCmd->setSourceSurface(sourceShape);
        saveCmd->setOffset(offset);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelOffsetSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelOffsetSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("OffsetSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceSurface(saveCmd->sourceSurface());
            originCmd->setOffset(saveCmd->offset());
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


    /** 拉伸形成曲面 */
    /*@{*/
    void OperSurfaceExtrudeManager::showDialog()
    {
        initDialog<GUI::SurfaceExtrudeInfoDialog>();
    }

    bool OperSurfaceExtrudeManager::execProfession()
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
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelExtrudeSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTExtrudeSurface);
        // 更新命令数据
        saveCmd->setSourceCurve(sourceShape);
        saveCmd->setDirection(dx, dy, dz);
        saveCmd->setLength(length);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelExtrudeSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelExtrudeSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ExtrudeSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve(saveCmd->sourceCurve());
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


    /** 旋转形成曲面 */
    /*@{*/
    void OperSurfaceRevolManager::showDialog()
    {
        initDialog<GUI::SurfaceRevolInfoDialog>();
    }

    bool OperSurfaceRevolManager::execProfession()
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
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelRevolSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRevolSurface);
        // 更新命令数据
        saveCmd->setSourveCurve(sourceShape);
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
        Interface::FITKAbsGeoModelRevolSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelRevolSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("RevolSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourveCurve(saveCmd->sourceCurve());
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


    /** 扫略形成曲面 */
    /*@{*/
    void OperSurfaceSweepManager::showDialog()
    {
        initDialog<GUI::SurfaceSweepInfoDialog>();
    }

    bool OperSurfaceSweepManager::execProfession()
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
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelSweepSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSweepSurface);
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
        Interface::FITKAbsGeoModelSweepSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelSweepSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("SweepSurface-%1").arg(geoCmdList->getDataCount() + 1)));
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
    void OperSurfaceMultiSectionManager::showDialog()
    {
        initDialog<GUI::SurfaceMultiSectionInfoDialog>();
    }

    bool OperSurfaceMultiSectionManager::execProfession()
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
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelMultiSectionSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTMultiSectionSurface);
        // 更新命令数据
        saveCmd->setSections(sections);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelMultiSectionSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelMultiSectionSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("MultiSectionSurface-%1").arg(geoCmdList->getDataCount() + 1)));
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


    /** 从体上提取面 */
    /*@{*/
    void OperSurfaceFromSolidManager::showDialog()
    {
        initDialog<GUI::SurfaceSurfaceFromSolidInfoDialog>();
    }

    bool OperSurfaceFromSolidManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceSolid;
        int faceId;
        bool flag = argValue("SourceSolid", sourceSolid);
        flag &= argValue("FaceId", faceId);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelSolidSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSurfaceFromSolid);
        // 更新命令数据
        saveCmd->setSourceSolid(sourceSolid);
        saveCmd->setFaceVirtualTopoId(faceId);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelSolidSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelSolidSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("SolidSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceSolid(saveCmd->sourceSolid());
            originCmd->setFaceVirtualTopoId(saveCmd->faceVirtualTopoId());
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


    /** 桥接面 */
    /*@{*/
    void OperBridgeSurfaceManager::showDialog()
    {
        initDialog<GUI::SurfaceBridgeSurfaceInfoDialog>();
    }

    bool OperBridgeSurfaceManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceSurface1, sourceSurface2;
        int edgeId1, edgeId2;
        bool flag = argValue("SourceSurface1", sourceSurface1);
        flag &= argValue("SourceSurface2", sourceSurface2);
        flag &= argValue("EdgeId1", edgeId1);
        flag &= argValue("EdgeId2", edgeId2);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelBridgeSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBridgeSurface);
        // 更新命令数据
        saveCmd->setSourceSurface1(sourceSurface1);
        saveCmd->setEdgeVirtualTopoId1(edgeId1);
        saveCmd->setSourceSurface2(sourceSurface2);
        saveCmd->setEdgeVirtualTopoId2(edgeId2);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelBridgeSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelBridgeSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("BridgeSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceSurface1(saveCmd->sourceSurface1());
            originCmd->setEdgeVirtualTopoId1(saveCmd->edgeVirtualTopoId1());
            originCmd->setSourceSurface2(saveCmd->sourceSurface2());
            originCmd->setEdgeVirtualTopoId2(saveCmd->edgeVirtualTopoId2());
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
