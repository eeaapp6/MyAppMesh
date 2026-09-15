/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperTransformationManager.h"
#include "GUIDialog/TransformationInfoDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    /** 平移变换 */
    /*@{*/
    void OperTransformManager::showDialog()
    {
        auto dlg = initDialog<GUI::TransformationInfoDialog>();
        dlg->setTransType(Interface::FITKGeoEnum::FITKGeometryComType::FGTTransformByTwoPoints);
    }

    bool OperTransformManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::FITKGeoEnum::FITKGeometryComType type{};
        Interface::VirtualShape sourceShape;
        bool isCopy;
        bool flag = argValue("Type", type);
        flag &= argValue("SourceShape", sourceShape);
        flag &= argValue("IsCopy", isCopy);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoTransformation>(type);
        if (saveCmd == nullptr) return false;
        if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTTransformByTwoPoints) {
            double p1x, p1y, p1z, p2x, p2y, p2z;
            bool flag = argValue("P1x", p1x);
            flag &= argValue("P1y", p1y);
            flag &= argValue("P1z", p1z);
            flag &= argValue("P2x", p2x);
            flag &= argValue("P2y", p2y);
            flag &= argValue("P2z", p2z);
            if (!flag) return false;

            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByTwoPoints*>(saveCmd);
            if (tempCmd == nullptr) return false;
            tempCmd->setStartPoint(p1x, p1y, p1z);
            tempCmd->setEndPoint(p2x, p2y, p2z);
        }
        else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTTransformByDirAndDis) {
            double dx, dy, dz, distance;
            bool flag = argValue("Dx", dx);
            flag &= argValue("Dy", dy);
            flag &= argValue("Dz", dz);
            flag &= argValue("Distance", distance);
            if (!flag) return false;

            auto tempCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByDirAndDis*>(saveCmd);
            if (tempCmd == nullptr) return false;
            tempCmd->setDirection(dx, dy, dz);
            tempCmd->setDistance(distance);
        }
        else {
            delete saveCmd;
            return false;
        }
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setCopy(isCopy);
        // 更新命令数据
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTTransformByTwoPoints) {
            auto tempSaveCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByTwoPoints*>(saveCmd);
            if (tempSaveCmd == nullptr) { delete saveCmd; return false; }
            // 更新成功的话进行数据保存操作
            Interface::FITKAbsGeoModelTransformByTwoPoints* originCmd{};
            if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByTwoPoints*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
                // 新建的对象直接保存
                saveCmd->setDataObjectName(geoCmdList->checkName(QString("Trans-%1").arg(geoCmdList->getDataCount() + 1)));
                geoCmdList->appendDataObj(saveCmd);
            }
            else {
                // 原有的对象进行替换操作
                originCmd->setStartPoint(tempSaveCmd->startPoint());
                originCmd->setEndPoint(tempSaveCmd->endPoint());
                originCmd->setSourceShape(tempSaveCmd->sourceShape());
                originCmd->setCopy(tempSaveCmd->isCopy());
                originCmd->update();
                delete saveCmd;
                saveCmd = originCmd;
            }
        }
        else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTTransformByDirAndDis) {
            auto tempSaveCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByDirAndDis*>(saveCmd);
            if (tempSaveCmd == nullptr) { delete saveCmd; return false; }
            // 更新成功的话进行数据保存操作
            Interface::FITKAbsGeoModelTransformByDirAndDis* originCmd{};
            if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelTransformByDirAndDis*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
                // 新建的对象直接保存
                saveCmd->setDataObjectName(geoCmdList->checkName(QString("Trans-%1").arg(geoCmdList->getDataCount() + 1)));
                geoCmdList->appendDataObj(saveCmd);
            }
            else {
                // 原有的对象进行替换操作
                originCmd->setDirection(tempSaveCmd->direction());
                originCmd->setDistance(tempSaveCmd->distance());
                originCmd->setSourceShape(tempSaveCmd->sourceShape());
                originCmd->setCopy(tempSaveCmd->isCopy());
                originCmd->update();
                delete saveCmd;
                saveCmd = originCmd;
            }
        }

        // 如果不是拷贝，需要刷新源对象，保证隐藏
        if (!saveCmd->isCopy()) {
            // 移除命令引用并添加新命令引用。
            saveCmd->clearReference();
            saveCmd->addReferenceCmdID(sourceShape.CmdId);
        }
        else {
            saveCmd->clearReference();
        }

        // 暂存数据ID。
        setArgs("Id", saveCmd->getDataObjectID());

        // 释放资源
        finalize();

        return true;
    }
    /*@*/


    /** 旋转变换 */
    /*@{*/
    void OperRotateManager::showDialog()
    {
        auto dlg = initDialog<GUI::TransformationInfoDialog>();
        dlg->setTransType(Interface::FITKGeoEnum::FITKGeometryComType::FGTRotate);
    }

    bool OperRotateManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape;
        bool isCopy;
        double p1x, p1y, p1z, p2x, p2y, p2z, angle;
        bool flag = argValue("P1x", p1x);
        flag &= argValue("P1y", p1y);
        flag &= argValue("P1z", p1z);
        flag &= argValue("P2x", p2x);
        flag &= argValue("P2y", p2y);
        flag &= argValue("P2z", p2z);
        flag &= argValue("Angle", angle);
        flag &= argValue("SourceShape", sourceShape);
        flag &= argValue("IsCopy", isCopy);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelRotate>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRotate);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setCopy(isCopy);
        saveCmd->setAxisStartPoint(p1x, p1y, p1z);
        saveCmd->setAxisEndPoint(p2x, p2y, p2z);
        saveCmd->setDegree(angle);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelRotate* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelRotate*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Trans-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setCopy(saveCmd->isCopy());
            originCmd->setAxisStartPoint(saveCmd->axisStartPoint());
            originCmd->setAxisEndPoint(saveCmd->axisEndPoint());
            originCmd->setDegree(saveCmd->degree());
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


    /** 缩放变换 */
    /*@{*/
    void OperScaleManager::showDialog()
    {
        auto dlg = initDialog<GUI::TransformationInfoDialog>();
        dlg->setTransType(Interface::FITKGeoEnum::FITKGeometryComType::FGTScale);
    }

    bool OperScaleManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape;
        bool isCopy;
        double px, py, pz, fx, fy, fz;
        bool flag = argValue("Px", px);
        flag &= argValue("Py", py);
        flag &= argValue("Pz", pz);
        flag &= argValue("Fx", fx);
        flag &= argValue("Fy", fy);
        flag &= argValue("Fz", fz);
        flag &= argValue("SourceShape", sourceShape);
        flag &= argValue("IsCopy", isCopy);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelScale>(Interface::FITKGeoEnum::FITKGeometryComType::FGTScale);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setCopy(isCopy);
        saveCmd->setBasePoint(px, py, pz);
        saveCmd->setFactors(fx, fy, fz);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelScale* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelScale*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Trans-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setCopy(saveCmd->isCopy());
            originCmd->setBasePoint(saveCmd->basePoint());
            originCmd->setFactors(saveCmd->factors());
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


    /** 镜像变换 */
    /*@{*/
    void OperMirrorManager::showDialog()
    {
        auto dlg = initDialog<GUI::TransformationInfoDialog>();
        dlg->setTransType(Interface::FITKGeoEnum::FITKGeometryComType::FGTMirror);
    }

    bool OperMirrorManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape, reference;
        bool isCopy;
        bool flag = argValue("Reference", reference);
        flag &= argValue("SourceShape", sourceShape);
        flag &= argValue("IsCopy", isCopy);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelMirror>(Interface::FITKGeoEnum::FITKGeometryComType::FGTMirror);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setCopy(isCopy);
        saveCmd->setReference(reference);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelMirror* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelMirror*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Trans-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setCopy(saveCmd->isCopy());
            originCmd->setReference(saveCmd->reference());
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


    /** 线性（矩阵）阵列变换 */
    /*@{*/
    void OperRectangularPatternManager::showDialog()
    {
        auto dlg = initDialog<GUI::TransformationInfoDialog>();
        dlg->setTransType(Interface::FITKGeoEnum::FITKGeometryComType::FGTRectangularPattern);
    }

    bool OperRectangularPatternManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape;
        bool isCopy;
        int firstDefinition, firstInstances, secondDefinition, secondInstances;
        double firstSpacing, firstLength, firstDx, firstDy, firstDz, secondSpacing, secondLength, secondDx, secondDy, secondDz;
        bool flag = argValue("FirstDefinition", firstDefinition);
        flag &= argValue("FirstInstances", firstInstances);
        flag &= argValue("FirstSpacing", firstSpacing);
        flag &= argValue("FirstLength", firstLength);
        flag &= argValue("FirstDx", firstDx);
        flag &= argValue("FirstDy", firstDy);
        flag &= argValue("FirstDz", firstDz);

        flag &= argValue("SecondDefinition", secondDefinition);
        flag &= argValue("SecondInstances", secondInstances);
        flag &= argValue("SecondSpacing", secondSpacing);
        flag &= argValue("SecondLength", secondLength);
        flag &= argValue("SecondDx", secondDx);
        flag &= argValue("SecondDy", secondDy);
        flag &= argValue("SecondDz", secondDz);

        flag &= argValue("SourceShape", sourceShape);
        flag &= argValue("IsCopy", isCopy);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelRectangularPattern>(Interface::FITKGeoEnum::FITKGeometryComType::FGTRectangularPattern);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setCopy(isCopy);
        saveCmd->setFirstDefinition(static_cast<Interface::FITKAbsGeoModelRectangularPattern::Definition>(firstDefinition));
        saveCmd->setFirstCount(firstInstances);
        saveCmd->setFirstSpacing(firstSpacing);
        saveCmd->setFirstTotal(firstLength);
        saveCmd->setFirstDirection(firstDx, firstDy, firstDz);

        saveCmd->setSecondDefinition(static_cast<Interface::FITKAbsGeoModelRectangularPattern::Definition>(secondDefinition));
        saveCmd->setSecondCount(secondInstances);
        saveCmd->setSecondSpacing(secondSpacing);
        saveCmd->setSecondTotal(secondLength);
        saveCmd->setSecondDirection(secondDx, secondDy, secondDz);


        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelRectangularPattern* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelRectangularPattern*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Trans-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setCopy(saveCmd->isCopy());

            originCmd->setFirstDefinition(saveCmd->firstDefinition());
            originCmd->setFirstCount(saveCmd->firstCount());
            originCmd->setFirstSpacing(saveCmd->firstSpacing());
            originCmd->setFirstTotal(saveCmd->firstTotal());
            originCmd->setFirstDirection(saveCmd->firstDirection());

            originCmd->setSecondDefinition(saveCmd->secondDefinition());
            originCmd->setSecondCount(saveCmd->secondCount());
            originCmd->setSecondSpacing(saveCmd->secondSpacing());
            originCmd->setSecondTotal(saveCmd->secondTotal());
            originCmd->setSecondDirection(saveCmd->secondDirection());

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


    /** 环形阵列变换 */
    /*@{*/
    void OperCircularPatternManager::showDialog()
    {
        auto dlg = initDialog<GUI::TransformationInfoDialog>();
        dlg->setTransType(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircularPattern);
    }

    bool OperCircularPatternManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape, patternAxis;
        bool isCopy, isReverse;
        int axisDefinition, axisInstances, crownDefinition, crownInstances;
        double axisSpacing, axisLength, crownSpacing, crownLength;
        bool flag = argValue("AxisDefinition", axisDefinition);
        flag &= argValue("AxisInstances", axisInstances);
        flag &= argValue("AxisSpacing", axisSpacing);
        flag &= argValue("AxisLength", axisLength);
        flag &= argValue("PatternAxis", patternAxis);
        flag &= argValue("IsReverseAxis", isReverse);

        flag &= argValue("CrownDefinition", crownDefinition);
        flag &= argValue("CrownInstances", crownInstances);
        flag &= argValue("CrownSpacing", crownSpacing);
        flag &= argValue("CrownLength", crownLength);

        flag &= argValue("SourceShape", sourceShape);
        flag &= argValue("IsCopy", isCopy);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoModelCircularPattern>(Interface::FITKGeoEnum::FITKGeometryComType::FGTCircularPattern);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setCopy(isCopy);
        saveCmd->setFirstDefinition(static_cast<Interface::FITKAbsGeoModelCircularPattern::Definition>(axisDefinition));
        saveCmd->setAxisCount(axisInstances);
        saveCmd->setAxisSpacing(axisSpacing);
        saveCmd->setAxisTotal(axisLength);
        saveCmd->setAxis(patternAxis);
        saveCmd->reverse(isReverse);

        saveCmd->setSecondDefinition(static_cast<Interface::FITKAbsGeoModelCircularPattern::Definition>(crownDefinition));
        saveCmd->setCrownCount(crownInstances);
        saveCmd->setCrownSpacing(crownSpacing);
        saveCmd->setCrownTotal(crownLength);

        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoModelCircularPattern* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoModelCircularPattern*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Trans-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setCopy(saveCmd->isCopy());

            originCmd->setFirstDefinition(saveCmd->firstDefinition());
            originCmd->setAxisCount(saveCmd->axisCount());
            originCmd->setAxisSpacing(saveCmd->axisSpacing());
            originCmd->setAxisTotal(saveCmd->axisTotal());
            originCmd->setAxis(saveCmd->axis());
            originCmd->reverse(saveCmd->isReverse());

            originCmd->setSecondDefinition(saveCmd->secondDefinition());
            originCmd->setCrownCount(saveCmd->crownCount());
            originCmd->setCrownSpacing(saveCmd->crownSpacing());
            originCmd->setCrownTotal(saveCmd->crownTotal());
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
