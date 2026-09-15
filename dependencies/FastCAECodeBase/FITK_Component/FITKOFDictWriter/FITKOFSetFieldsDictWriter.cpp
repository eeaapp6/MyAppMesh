/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSetFieldsDictWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKAbstractOFSolver.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFRunControl.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBoolGroup.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFInitialConditions.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCylinder.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSphere.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"

#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFSetFieldsDictWriter::FITKOFSetFieldsDictWriter()
    {
    }
    FITKOFSetFieldsDictWriter::~FITKOFSetFieldsDictWriter()
    {
    }

    void FITKOFSetFieldsDictWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/setFieldsDict").arg(filePath);
    }

    bool FITKOFSetFieldsDictWriter::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取字典工程
        this->openDictObject();
        if (!m_objectDict)return false;
        //写出FoamFile
        this->dictObjectFoamFile();
        //设置写出的网格区域数据
        bool Ok = this->setFieldsDictW();
        //写出字典文件
        if (Ok)
            Ok = this->writeDictFile();

        //关闭字典工程
        this->closeDictObject();
        return Ok;
    }

    QString FITKOFSetFieldsDictWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFSetFieldsDictWriter::dictObjectFoamFile()
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"system\"");
        group->append("object", "setFieldsDict");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFSetFieldsDictWriter::setFieldsDictW()
    {
        if (!m_objectDict)return false;
        m_objectDict->append("defaultFieldValues", new DICT::FITKDictArray());
        DICT::FITKDictArray* regions = new DICT::FITKDictArray;
        m_objectDict->append("regions", regions);
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return false;
        //获取initial
        Interface::FITKOFInitialConditions* initialCond = physicsData->getInitialConditions();
        if (!initialCond)return false;
        int count = initialCond->getPatchCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKOFGeometryPatch* patch = initialCond->getPatch(i);
            if (!patch)continue;
            Interface::FITKAbsGeoCommand* geoCommand = patch->getGeometryModel();
            if (!geoCommand)continue;
            //获取应用类型
            Interface::FITKOFSolverInitialEnum::FITKPatchApplyObjectType applyType = patch->getAppltTo();
            DICT::FITKDictGroup* cellGroup = nullptr;
            DICT::FITKDictGroup* faceGroup = nullptr;
            //获取类型
            Interface::FITKGeoEnum::FITKGeometryComType type = geoCommand->getGeometryCommandType();
            if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTBox)
            {
                Interface::FITKAbsGeoModelBox* box = dynamic_cast<Interface::FITKAbsGeoModelBox*>(geoCommand);
                if (!box)return false;
                //第一个点
                double minP[3]{ 0.0, 0.0, 0.0 };
                box->getPoint1(minP);
                //长度
                double length[3]{ 0.0, 0.0, 0.0 };
                box->getLength(length);
                //最高点
                double maxP[3]{ minP[0], minP[1], minP[2] };
                length[0] < 0 ? minP[0] = minP[0] + length[0] : maxP[0] = minP[0] + length[0];
                length[1] < 0 ? minP[1] = minP[1] + length[1] : maxP[1] = minP[1] + length[1];
                length[2] < 0 ? minP[2] = minP[2] + length[2] : maxP[2] = minP[2] + length[2];
                QString value = QString("(%1 %2 %3) (%4 %5 %6)").arg(minP[0]).arg(minP[1]).arg(minP[2]).arg(maxP[0]).arg(maxP[1]).arg(maxP[2]);
                if (applyType == Interface::FITKOFSolverInitialEnum::FITKPatchApplyObjectType::Cells || applyType == Interface::FITKOFSolverInitialEnum::FITKPatchApplyObjectType::Both)
                {
                    cellGroup = new DICT::FITKDictGroup;
                    regions->append(cellGroup, "boxToCell");
                    cellGroup->append("box", value);
                }
                if (applyType == Interface::FITKOFSolverInitialEnum::FITKPatchApplyObjectType::Faces || applyType == Interface::FITKOFSolverInitialEnum::FITKPatchApplyObjectType::Both)
                {
                    faceGroup = new DICT::FITKDictGroup;
                    regions->append(faceGroup, "boxToFace");
                    faceGroup->append("box", value);
                }
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTCylinder)
            {
                Interface::FITKAbsGeoModelCylinder* cylinder = dynamic_cast<Interface::FITKAbsGeoModelCylinder*>(geoCommand);
                if (!cylinder)return false;
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTSphere)
            {
                Interface::FITKAbsGeoModelSphere* sphere = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(geoCommand);
                if (!sphere)return false;
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTBool)
            {
                Interface::FITKAbsGeoOperBool* boolOper = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoCommand);
                if (!boolOper)return false;
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTImport)
            {
                Interface::FITKAbsGeoModelImport* importModel = dynamic_cast<Interface::FITKAbsGeoModelImport*>(geoCommand);
                if (!importModel)return false;
            }
            if (cellGroup) this->writeFieldValue(patch, cellGroup);
            if (faceGroup) this->writeFieldValue(patch, faceGroup);
        }
        return true;
    }

    bool FITKOFSetFieldsDictWriter::writeFieldValue(Interface::FITKOFGeometryPatch* patch, DICT::FITKDictGroup* group)
    {
        if (!patch || !group)return false;
        Core::FITKParameter* para = patch->getFieldPara();
        if (!para)return false;
        DICT::FITKDictArray* fieldValues = new DICT::FITKDictArray;
        group->append("fieldValues", fieldValues);
        int count = para->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Core::FITKAbstractEasyParam* data = para->getDataByIndex(i);
            if (!data)continue;
            QString name = data->getDataObjectName();
            QString value;
            Core::FITKAbstractEasyParam::FITKEasyParamType type = data->getParamType();
            if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDouble)
            {
                Core::FITKParamDouble* p = dynamic_cast<Core::FITKParamDouble*>(data);
                if (!p)continue;
                value = QString("%1 %2").arg(name).arg(p->getValue());
            }
            else if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPInt)
            {
                Core::FITKParamInt* p = dynamic_cast<Core::FITKParamInt*>(data);
                if (!p)continue;
                value = QString("%1 %2").arg(name).arg(p->getValue());
            }
            else if (type == Core::FITKAbstractEasyParam::FITKEasyParamType::FEPDoubleList)
            {
                Core::FITKParamDoubleList* p = dynamic_cast<Core::FITKParamDoubleList*>(data);
                if (!p)continue;
                int num = p->getCount();
                QStringList valueList;
                for (int i = 0; i < num; ++i)
                    valueList.append(QString::number(p->getValue(i)));
                value = QString("%1 (%2)").arg(name).arg(valueList.join(' '));
            }
            DICT::FITKDictVector* scalar = new DICT::FITKDictVector;
            scalar->setParenthesesType(DICT::FITKDictVector::ParenthesesType::NoneParentheses);
            scalar->append(QString("volScalarFieldValue\t%2").arg(value));
            fieldValues->append(scalar);
        }

        return true;
    }
}



