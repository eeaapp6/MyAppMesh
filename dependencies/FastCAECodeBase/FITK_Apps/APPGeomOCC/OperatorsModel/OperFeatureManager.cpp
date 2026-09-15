/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "OperFeatureManager.h"

#include "GUIDialog/FeatureStiffenerDialog.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"

namespace ModelOper
{
    /** 肋特征 */
    /*@{*/
    void OperFeatureStiffenerManager::showDialog()
    {
        initDialog<GUI::FeatureStiffenerDialog>();
    }

    bool OperFeatureStiffenerManager::execProfession()
    {
        // 获取历史数据ID
        int originCmdId{ -1 };
        argValue("Id", originCmdId);

        // 获取输入参数
        Interface::VirtualShape sourceShape, profileShape;
        double thickness1, thickness2;
        bool flag = argValue("SourceShape", sourceShape);
        flag &= argValue("ProfileShape", profileShape);
        flag &= argValue("Thickness1", thickness1);
        flag &= argValue("Thickness2", thickness2);
        if (!flag) return false;

        // 获取全局变量
        auto geoCmdList = FITKGLODATA->getGeometryData<Interface::FITKGeoCommandList>();

        // 创建一个临时对象，避免更新失败
        auto fac = Interface::FITKInterfaceGeometryFactory::getInstance();
        auto saveCmd = fac->createCommandT<Interface::FITKAbsGeoStiffener>(Interface::FITKGeoEnum::FITKGeometryComType::FGTStiffener);
        // 更新命令数据
        saveCmd->setSourceShape(sourceShape);
        saveCmd->setProfileShape(profileShape);
        saveCmd->setThickness1(thickness1);
        saveCmd->setThickness2(thickness2);
        if (!saveCmd->update()) {
            // 更新失败直接删除临时对象
            delete saveCmd;
            saveCmd = nullptr;
            return false;
        }
        // 更新成功的话进行数据保存操作
        Interface::FITKAbsGeoStiffener* originCmd{};
        if (originCmdId <= 0 || (originCmd = dynamic_cast<Interface::FITKAbsGeoStiffener*>(geoCmdList->getDataByID(originCmdId))) == nullptr) {
            // 新建的对象直接保存
            saveCmd->setDataObjectName(geoCmdList->checkName(QString("Stiffener-%1").arg(geoCmdList->getDataCount() + 1)));
            geoCmdList->appendDataObj(saveCmd);
        }
        else {
            // 原有的对象进行替换操作
            originCmd->setSourceShape(saveCmd->sourceShape());
            originCmd->setProfileShape(saveCmd->profileShape());
            originCmd->setThickness1(saveCmd->thickness1());
            originCmd->setThickness2(saveCmd->thickness2());
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
