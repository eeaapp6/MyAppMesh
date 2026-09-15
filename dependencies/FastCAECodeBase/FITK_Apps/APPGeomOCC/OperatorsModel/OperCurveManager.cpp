/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperCurveManager.h"

// GUI
#include "GUIDialog/CurveCurveProjSurfaceInfoDialog.h"
#include "GUIDialog/CurveThreePointsEllipseInfoDialog.h"
#include "GUIDialog/CurveOffsetCurveInfoDialog.h"
#include "GUIDialog/CurveThreePointsHyperbolaInfoDialog.h"
#include "GUIDialog/CurveSurfaceInterSurfaceInfoDialog.h"  
#include "GUIDialog/CurveThreePointsParabolaInfoDialog.h"
#include "GUIDialog/CurveThreePointsCircleInfoDialog.h"    
#include "GUIDialog/CurveTwoPointsSegmentInfoDialog.h"
#include "GUIDialog/CurveBridgeCurveInfoDialog.h"
#include "GUIDialog/CurveCurveFromSurfaceInfoDialog.h"
#include "GUIDialog/Curve3DBSplineInfoDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeomTools.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{

    // 直线段。
    //@{
    void OperTwoPointsSegmentManager::showDialog()
    {
        initDialog<GUI::CurveTwoPointsSegmentInfoDialog>();
    }

    bool OperTwoPointsSegmentManager::execProfession()
    {

        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double p1x, p1y, p1z, p2x, p2y, p2z;
        bool flag = argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelSegment>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSegment);
        // 更新命令数据
        saveCmd->setStartPoint(p1x, p1y, p1z);
        saveCmd->setEndPoint(p2x, p2y, p2z);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelSegment* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelSegment*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Segment-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setStartPoint(saveCmd->startPoint());
            originCmd->setEndPoint(saveCmd->endPoint());
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


    // 三点确定圆。
    //@{
    void OperThreePointsCircleManager::showDialog()
    {
        initDialog<GUI::CurveThreePointsCircleInfoDialog>();
    }

    bool OperThreePointsCircleManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;
        bool flag = argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        flag &= argValue("P3x", p3x);
        flag &= argValue("P3y", p3y);
        flag &= argValue("P3z", p3z);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelThreePointsCircle>(Interface::FITKGeoEnum::FITKGeometryComType::FGTThreePointsCircle);
        // 更新命令数据
        saveCmd->setPoint(0, p1x, p1y, p1z);
        saveCmd->setPoint(1, p2x, p2y, p2z);
        saveCmd->setPoint(2, p3x, p3y, p3z);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelThreePointsCircle* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelThreePointsCircle*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ThreePointsCircle-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setPoint(0, saveCmd->point(0));
            originCmd->setPoint(1, saveCmd->point(1));
            originCmd->setPoint(2, saveCmd->point(2));
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


    // 三点确定椭圆。
    //@{
    void OperThreePointsEllipseManager::showDialog()
    {
        initDialog<GUI::CurveThreePointsEllipseInfoDialog>();
    }

    bool OperThreePointsEllipseManager::execProfession()
    {

        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;
        bool flag = argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        flag &= argValue("P3x", p3x);
        flag &= argValue("P3y", p3y);
        flag &= argValue("P3z", p3z);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelThreePointsEllipse>(Interface::FITKGeoEnum::FITKGeometryComType::FGTThreePointsEllipse);
        // 更新命令数据
        saveCmd->setOrigin(p1x, p1y, p1z);
        saveCmd->setMajorPoint(p2x, p2y, p2z);
        saveCmd->setMinorPoint(p3x, p3y, p3z);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelThreePointsEllipse* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelThreePointsEllipse*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ThreePointsEllipse-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setOrigin(saveCmd->origin());
            originCmd->setMajorPoint(saveCmd->majorPoint());
            originCmd->setMinorPoint(saveCmd->minorPoint());
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


    // 三点确定双曲线。
    //@{
    void OperThreePointsHyperbolaManager::showDialog()
    {
        initDialog<GUI::CurveThreePointsHyperbolaInfoDialog>();
    }

    bool OperThreePointsHyperbolaManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;
        bool flag = argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        flag &= argValue("P3x", p3x);
        flag &= argValue("P3y", p3y);
        flag &= argValue("P3z", p3z);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelThreePointsHyperbola>(Interface::FITKGeoEnum::FITKGeometryComType::FGTThreePointsHyperbola);

        // 更新命令数据
        saveCmd->setOrigin(p1x, p1y, p1z);
        saveCmd->setMajorPoint(p2x, p2y, p2z);
        saveCmd->setMinorPoint(p3x, p3y, p3z);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelThreePointsHyperbola* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelThreePointsHyperbola*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ThreePointsHyperbola-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setOrigin(saveCmd->origin());
            originCmd->setMajorPoint(saveCmd->majorPoint());
            originCmd->setMinorPoint(saveCmd->minorPoint());
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


    // 三点确定抛物线。
    //@{
    void OperThreePointsParabolaManager::showDialog()
    {
        initDialog<GUI::CurveThreePointsParabolaInfoDialog>();
    }

    bool OperThreePointsParabolaManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        double p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z;
        bool flag = argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        flag &= argValue("P3x", p3x);
        flag &= argValue("P3y", p3y);
        flag &= argValue("P3z", p3z);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelThreePointsParabola>(Interface::FITKGeoEnum::FITKGeometryComType::FGTThreePointsParabola);
        // 更新命令数据
        saveCmd->setPoint(0, p1x, p1y, p1z);
        saveCmd->setPoint(1, p2x, p2y, p2z);
        saveCmd->setPoint(2, p3x, p3y, p3z);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelThreePointsParabola* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelThreePointsParabola*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("ThreePointsParabola-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setPoint(0, saveCmd->point(0));
            originCmd->setPoint(1, saveCmd->point(1));
            originCmd->setPoint(2, saveCmd->point(2));
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

    // 样条曲线。
    //@{
    void OperCurveBSplineManager::showDialog()
    {
        initDialog<GUI::Curve3DBSplineInfoDialog>();
    }

    bool OperCurveBSplineManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        QList<Interface::VirtualShape> pointShapes;
        bool flag = argValue("PointShapes", pointShapes);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelBsplineByThroughPoints>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBSplineByThroughPoints);
        // 更新命令数据
        saveCmd->setThroughPointShapes(pointShapes);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelBsplineByThroughPoints* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelBsplineByThroughPoints*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("BSpline-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setThroughPoints(saveCmd->throughPoints());
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

    // 偏移曲线。
    //@{
    void OperOffsetCurveManager::showDialog()
    {
        initDialog<GUI::CurveOffsetCurveInfoDialog>();
    }

    bool OperOffsetCurveManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceCurve;
        double dx, dy, dz, offset;
        bool flag = argValue("SourceCurve", sourceCurve);
        flag &= argValue("Dx", dx);
        flag &= argValue("Dy", dy);
        flag &= argValue("Dz", dz);
        flag &= argValue("Offset", offset);

        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelOffsetCurve>(Interface::FITKGeoEnum::FITKGeometryComType::FGTOffsetCurve);
        // 更新命令数据
        saveCmd->setSourceCurve(sourceCurve);
        saveCmd->setDir(dx, dy, dz);
        saveCmd->setOffset(offset);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelOffsetCurve* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelOffsetCurve*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("OffsetCurve-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve(saveCmd->sourceCurve());
            originCmd->setDir(saveCmd->dir());
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
    //@}


    // 线在面上的投影。
    //@{
    void OperCurveProjSurfaceManager::showDialog()
    {
        initDialog<GUI::CurveCurveProjSurfaceInfoDialog>();
    }

    bool OperCurveProjSurfaceManager::execProfession()
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
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelCurveProjectionOnSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveProjectionOnSurface);
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
        Interface::FITKAbsGeoModelCurveProjectionOnSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelCurveProjectionOnSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("CurveProjectionOnSurface-%1").arg(geoCmdList->getDataCount() + 1)));
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


    // 面面交线。
    //@{
    void OperSurfaceInterSurfaceManager::showDialog()
    {
        initDialog<GUI::CurveSurfaceInterSurfaceInfoDialog>();
    }

    bool OperSurfaceInterSurfaceManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape surface1, surface2;
        bool flag = argValue("InputEdgeSurface1", surface1);
        flag &= argValue("InputEdgeSurface2", surface2);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelSurfaceIntersectionSurface>(Interface::FITKGeoEnum::FITKGeometryComType::FGTSurfaceIntersectSurface);
        // 更新命令数据
        saveCmd->setSourceSurface1(surface1);
        saveCmd->setSourceSurface2(surface2);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelSurfaceIntersectionSurface* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelSurfaceIntersectionSurface*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("SurfaceIntersectionSurface-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceSurface1(saveCmd->sourceSurface1());
            originCmd->setSourceSurface2(saveCmd->sourceSurface2());
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


    // 桥接曲线。
    //@{
    void OperBridgeCurveManager::showDialog()
    {
        initDialog<GUI::CurveBridgeCurveInfoDialog>();
    }

    bool OperBridgeCurveManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceCurve1, sourceCurve2;
        bool useStartEnd1, useStartEnd2;
        double parameter1, parameter2;

        bool flag = argValue("SourceCurve1", sourceCurve1);
        flag &= argValue("SourceCurve2", sourceCurve2);
        flag &= argValue("UseStartEnd1", useStartEnd1);
        flag &= argValue("UseStartEnd2", useStartEnd2);
        flag &= argValue("Parameter1", parameter1);
        flag &= argValue("Parameter2", parameter2);

        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelBridgeCurve>(Interface::FITKGeoEnum::FITKGeometryComType::FGTBridgeCurve);
        // 更新命令数据
        saveCmd->setSourceCurve1(sourceCurve1);
        saveCmd->setUseStartEnd1(useStartEnd1);
        saveCmd->setParameter1(parameter1);
        saveCmd->setSourceCurve2(sourceCurve2);
        saveCmd->setUseStartEnd2(useStartEnd2);
        saveCmd->setParameter2(parameter2);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelBridgeCurve* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelBridgeCurve*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("BridgeCurve-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceCurve1(saveCmd->sourceCurve1());
            originCmd->setUseStartEnd1(saveCmd->useStartEnd1());
            originCmd->setParameter1(saveCmd->parameter1());
            originCmd->setSourceCurve2(saveCmd->sourceCurve2());
            originCmd->setUseStartEnd2(saveCmd->useStartEnd2());
            originCmd->setParameter2(saveCmd->parameter2());
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


    // 从面提取边线。
    //@{
    void OperCurveFromSurfaceManager::showDialog()
    {
        initDialog<GUI::CurveCurveFromSurfaceInfoDialog>();
    }

    bool OperCurveFromSurfaceManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceSurface;
        int edgeId;
        bool flag = argValue("SourceSurface", sourceSurface);
        flag &= argValue("EdgeId", edgeId);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelSurfaceEdge>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCurveFromSurface);
        // 更新命令数据
        saveCmd->setSourceSurface(sourceSurface);
        saveCmd->setEdgeVirtualTopoId(edgeId);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelSurfaceEdge* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelSurfaceEdge*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("SurfaceEdge-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceSurface(saveCmd->sourceSurface());
            originCmd->setEdgeVirtualTopoId(saveCmd->edgeVirtualTopoId());
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
