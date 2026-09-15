/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperPointManager.h"

#include "GUIDialog/PointCoordinateInfoDialog.h"
#include "GUIDialog/PointCurveEndAndDistanceInfoDialog.h"
#include "GUIDialog/PointInterOfCurveAndSurfaceInfoDialog.h"
#include "GUIDialog/PointCurveEndInfoDialog.h"
#include "GUIDialog/PointInterTwoCurveInfoDialog.h"
#include "GUIDialog/PointPointProjOnCurveInfoDialog.h"
#include "GUIDialog/PointCurveAndRadioInfoDialog.h"
#include "GUIDialog/PointPointProjOnSurfaceInfoDialog.h"
#include "GUIDialog/PointInteractionInfoDialog.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    // 坐标点。
    //@{
    void OperCoordinatePointManager::showDialog()
    {
        initDialog<GUI::PointCoordinateInfoDialog>();
    }

    bool OperCoordinatePointManager::execProfession()
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

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelPoint>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPoint);
        // 更新命令数据
        saveCmd->setCoord(px, py, pz);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelPoint* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelPoint*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("CoordPoint-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setCoord(saveCmd->x(), saveCmd->y(), saveCmd->z());
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
    //@}


    // 曲线端点。
    //@{
    void OperCurveEndPointManager::showDialog()
    {
        initDialog<GUI::PointCurveEndInfoDialog>();
    }

    bool OperCurveEndPointManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceCurve;
        bool useStartEnd{};
        bool flag = argValue("SourceCurve", sourceCurve);
        flag &= argValue("UseStartEnd", useStartEnd);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelCurveEnd>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveEnd);
        // 更新命令数据
        saveCmd->setSourceCurve(sourceCurve);
        saveCmd->setUseStartEnd(useStartEnd);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelCurveEnd* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelCurveEnd*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("CurveEnd-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve(saveCmd->sourceCurve());
            originCmd->setUseStartEnd(saveCmd->useStartEnd());
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
    //@}  


    // 曲线和比例。
    //@{
    void OperCurveAndRatioPointManager::showDialog()
    {
        initDialog<GUI::PointCurveAndRadioInfoDialog>();
    }

    bool OperCurveAndRatioPointManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceCurve;
        double ratio;
        bool flag = argValue("SourceCurve", sourceCurve);
        flag &= argValue("Ratio", ratio);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelCurveRadio>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveRadio);
        // 更新命令数据
        saveCmd->setSourceCurve(sourceCurve);
        saveCmd->setRadio(ratio);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelCurveRadio* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelCurveRadio*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("CurveRadio-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve(saveCmd->sourceCurve());
            originCmd->setRadio(saveCmd->radio());
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
    //@}


    // 距端点距离。
    //@{
    void OperCurveEndAndDistancePointManager::showDialog()
    {
        initDialog<GUI::PointCurveEndAndDistanceInfoDialog>();
    }

    bool OperCurveEndAndDistancePointManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceCurve;
        bool useStartEnd{};
        double distance;
        bool flag = argValue("SourceCurve", sourceCurve);
        flag &= argValue("UseStartEnd", useStartEnd);
        flag &= argValue("Distance", distance);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelCurveEndDistance>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveEndDistance);
        // 更新命令数据
        saveCmd->setSourceCurve(sourceCurve);
        saveCmd->setUseStartEnd(useStartEnd);
        saveCmd->setDistance(distance);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelCurveEndDistance* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelCurveEndDistance*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("CurveEndDistance-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve(saveCmd->sourceCurve());
            originCmd->setUseStartEnd(saveCmd->useStartEnd());
            originCmd->setDistance(saveCmd->distance());
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
    //@}


    // 点在线上的投影。
    //@{
    void OperPointProjectionOnCurveManager::showDialog()
    {
        initDialog<GUI::PointPointProjOnCurveInfoDialog>();
    }

    bool OperPointProjectionOnCurveManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourcePoint, sourceCurve;
        bool flag = argValue("SourcePoint", sourcePoint);
        flag &= argValue("SourceCurve", sourceCurve);

        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelPointProjectionOnCurve>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPointProjectionOnCurve);
        // 更新命令数据
        saveCmd->setSourcePoint(sourcePoint);
        saveCmd->setSourceCurve(sourceCurve);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelPointProjectionOnCurve* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelPointProjectionOnCurve*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("PointProjectionOnCurve-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourcePoint(saveCmd->sourcePoint());
            originCmd->setSourceCurve(saveCmd->sourceCurve());
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
    //@}


    // 曲线交点。
    //@{
    void OperIntersectionOfTwoCurveManager::showDialog()
    {
        initDialog<GUI::PointInterTwoCurveInfoDialog>();
    }

    bool OperIntersectionOfTwoCurveManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceCurve1, sourceCurve2;
        bool isNearestPoint;
        bool flag = argValue("SourceCurve1", sourceCurve1);
        flag &= argValue("SourceCurve2", sourceCurve2);
        flag &= argValue("IsNearestPoint", isNearestPoint);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelTwoCurveIntersection>(Interface::FITKGeoEnum::FITKGeometryComType::FGTTwoCurveIntersectPoint);
        // 更新命令数据
        saveCmd->setSourceCurve(sourceCurve1, sourceCurve2);
        saveCmd->setNearestPoint(isNearestPoint);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelTwoCurveIntersection* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelTwoCurveIntersection*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString(saveCmd->isNearestPoint() ? "NearestPointOfTwoCurve-%1" : "TwoCurveIntersectPoint-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve(saveCmd->sourceCurve1(), saveCmd->sourceCurve2());
            originCmd->setNearestPoint(saveCmd->isNearestPoint());
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
    //@}


    // 点在面上的投影。
    //@{
    void OperPointProjectionOnSurfaceManager::showDialog()
    {
        initDialog<GUI::PointPointProjOnSurfaceInfoDialog>();
    }

    bool OperPointProjectionOnSurfaceManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourcePoint, sourceSurface;
        bool flag = argValue("SourcePoint", sourcePoint);
        flag &= argValue("SourceSurface", sourceSurface);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelPointProjectionOnSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTPointProjectionOnSurface);
        // 更新命令数据
        saveCmd->setSourcePoint(sourcePoint);
        saveCmd->setSourceSurface(sourceSurface);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelPointProjectionOnSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelPointProjectionOnSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("PointProjectionOnSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourcePoint(saveCmd->sourcePoint());
            originCmd->setSourceSurface(saveCmd->sourceSurface());
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
    //@}


    // 面面交点。
    //@{
    void OperIntersectionOfCurveAndSurfaceManager::showDialog()
    {
        initDialog<GUI::PointInterOfCurveAndSurfaceInfoDialog>();
    }

    bool OperIntersectionOfCurveAndSurfaceManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceCurve, sourceSurface;
        bool flag = argValue("SourceCurve", sourceCurve);
        flag &= argValue("SourceSurface", sourceSurface);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelCurveIntersectionSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveIntersectSurface);
        // 更新命令数据
        saveCmd->setSourceCurve(sourceCurve);
        saveCmd->setSourceSurface(sourceSurface);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelCurveIntersectionSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelCurveIntersectionSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("CurveIntersectSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve(saveCmd->sourceCurve());
            originCmd->setSourceSurface(saveCmd->sourceSurface());
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
    //@}  

    // 交互生成点。
    //@{
    void OperInteractionPointManager::showDialog()
    {
        initDialog<GUI::PointInteractionInfoDialog>();
    }

    bool OperInteractionPointManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape;
        double px, py, pz;
        int type;
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("Px", px);
        flag &= argValue("Py", py);
        flag &= argValue("Pz", pz);
        flag &= argValue("Pz", pz);
        flag &= argValue("Type", type);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelInteractionPoint>(Interface::FITKGeoEnum::FITKGeometryComType::FGTInteractionPoint);
        // 更新命令数据
        saveCmd->setCoord(px, py, pz);
        saveCmd->setSource(sourceShape);
        saveCmd->setType(type);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelInteractionPoint* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelInteractionPoint*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("InteractionPoint-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setCoord(saveCmd->x(), saveCmd->y(), saveCmd->z());
            originCmd->setSource(saveCmd->source());
            originCmd->setType(saveCmd->type());
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
    //@}  
} // namespace ModelOper
