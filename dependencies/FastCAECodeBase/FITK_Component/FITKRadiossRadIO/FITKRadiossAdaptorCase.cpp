/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorCase.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
//RadiossData
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKMaterialElastic.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Component/FITKRadiossData/FITKPropAbstract.hpp"
#include "FITK_Interface/FITKInterfacePhysics/FITKAbstractSection.h"
#include "FITK_Component/FITKRadiossData/FITKNonLinerCurve.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractRWall.h"
#include "FITK_Component/FITKRadiossData/FITKGravity.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInitialField.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBCS.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractInteraction.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractProbe.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractConnection.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractTimeHistory.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractEquationOfState.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolverSettings.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractFailureModel.h"
#include "FITK_Component/FITKRadiossData/FITKAbstractBEM.h"
//Adaptor
#include "FITKRadiossAdaptorMaterialElastic.h"
#include "FITKRadiossAdaptorNode.h"
#include "FITKRadiossAdaptorPart.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorCase::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorCase";
    }

    bool FITKRadiossAdaptorCase::adaptR()
    {
        _caseData = FITKRadiossCase::SafeDownCastFITKRadiossCase(_dataObj);
        if (_reader == nullptr || _caseData == nullptr) return false;
        bool ok = false;
        //回退一行，读取关键字
        _reader->backLine();
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine().trimmed();
            if (!line.startsWith("/")) continue;

            //单位制读取
            if(line == "/BEGIN")
            {
                ok = readBegin();
            }
            //读取标题
            else if (line == "/TITLE")
            {
                ok = readTitle();
            }
            //其他关键字if (line.startsWith("/")) 
            else
            {
                _reader->backLine();
                break;
            }
        }
        return ok;
    }

    bool FITKRadiossAdaptorCase::adaptW()
    {
        _caseData = Radioss::FITKRadiossCase::SafeDownCastFITKRadiossCase(_dataObj);
        QTextStream* stream = _writer->getStream();
        if (_writer == nullptr || _caseData == nullptr || stream == nullptr) return false;
        bool ok = true;
        //写出固定头信息,暂时默认
        ok &= writeHead();
        // 写出BEGIN、TITLE
        ok &= writeBegin();
        ok &= writeTitle();
        //写出粒子全局属性
        ok &= writeSPHGLO();
        //写出节点
        ok &= writeNode();
        //写出节点组(节点组要在初始场，重力，刚性墙，边界，相互作用前面)
        ok &= writeNodeSet();
        //写出曲线(曲线要在重力，材料(/MAT/PLAS_TAB)，属性(/PROP/SPR_BEAM)前面)
        ok &= writeCurve();
        //写出材料(材料要在Part 属性 状态方程前面)
        ok &= writeMaterials();
        //写出状态方程
        ok &= writeEOS();
        //写出失效
        ok &= writeFailure();
        //写出属性默认值
        ok &= writeDefaultPropValue();
        //写出属性(截面)(属性要在Part 前面)
        ok &= writeProperty();
        //写出全局坐标系
        // ok &=  （坐标系要在初始场和边界前面）
        //写出子集
        ok &= writeSubset();
        //写出PART（Part要在surf前面）
        ok &= writePart();
        //写出surface（surface要在相互作用前面）
        ok &= writeSurface();
        //写出默认相互作用（）
        ok &= writeDefaultInteraction();
        //写出相互作用（）
        ok &= writeInteraction();
        //写出初始场
        ok &= writeInitialField();
        //写出重力
        ok &= writeGravity();
        //写出刚性墙
        ok &= writeRigidWall();
        //写出边界
        ok &= writeBC();
        //写出BEM
        ok &= writeBEM();
        //写出连接（时间历程前面）
        ok &= writeConnection();
        //写出探针（时间历程前面）
        ok &= writeProbe();
        //写出时间历程
        ok &= writeTimerHistory();

        *stream << "/END" << endl;
        return ok;
    }

    bool FITKRadiossAdaptorCase::readBegin()
    {
        QString line = _reader->previousLine().trimmed();
        if (line != "/BEGIN") return false;

        QStringList beginText;

        while (!_reader->atEnd())
        {
            line = _reader->readLine();
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            line.simplified();
            beginText.append(line);
        }

        if (beginText.empty()) return false;
        //行数与手册说明不符
        if (beginText.size() != 4)
        {
            _reader->consoleMessage(3, "Invalie Begin.");
            return false;
        }

        //案例名称
        QString runName = beginText[0];
        //手册版本
        QStringList inverss = beginText[1].split(' ');
        int invers = inverss[0].toInt();
        //输入单位，如果为空则与工作单位相同
        if (!beginText[2].isEmpty())
            QStringList inputUnits = beginText[2].split(' ');
        //工作单位
        QStringList workUnits = beginText[3].split(' ');
        return true;
    }

    bool FITKRadiossAdaptorCase::readTitle()
    {
        QString line = _reader->previousLine().trimmed();
        if (line != "/TITLE") return false;

        QStringList titleText;

        while (!_reader->atEnd())
        {
            line = _reader->readLine();
            if (line.startsWith("#")) continue;
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            line.simplified();
            titleText.append(line);
        }
        //行数与手册说明不符
        if (titleText.size() != 1)
        {
            _reader->consoleMessage(3, "Invalid Title.");
            return false;
        }
        //案例标题
        QString title = titleText[0];
        return true;
    }
    bool FITKRadiossAdaptorCase::writeHead()
    {
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "#RADIOSS STARTER" << endl;
        *stream << "# Copyright (C) 2022 Altair Engineering Inc. (\"Holder\")" << endl;
        *stream << "# Model is licensed by Holder under CC BY-NC 4.0" << endl;
        *stream << "# (https://creativecommons.org/licenses/by-nc/4.0/legalcode)." << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }
    bool FITKRadiossAdaptorCase::writeBegin()
    {
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "/BEGIN" << endl;
        *stream << _caseData->getCurrentSolution()->getDataObjectName() << endl;
        *stream << "      2022         0" << endl;
        *stream << "            1.000000            1.000000            1.000000" << endl;
        *stream << "            1.000000            1.000000            1.000000" << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        return true;
    }

    bool FITKRadiossAdaptorCase::writeTitle()
    {
        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "#-  1. CONTROL CARDS:" << endl;
        *stream << "#---1----|----2----|----3----|----4----|----5----|----6----|----7----|----8----|----9----|---10----|" << endl;
        *stream << "/TITLE" << endl;
        *stream << "" << endl;
        return true;
    }

    bool FITKRadiossAdaptorCase::writeMaterials()
    {
        if (!_writer || !_caseData)return false;

        Interface::FITKMaterialManager* materialsMgr = _caseData->getMaterialManager();
        if (materialsMgr == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "##---------------------------------------------" << endl;
        *stream << "## Material" << endl;
        *stream << "##---------------------------------------------" << endl;
        //遍历材料
        const int n = materialsMgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            Interface::FITKAbstractMaterial* mat = materialsMgr->getDataByIndex(i);
            if (mat == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                             <FITKRadiossAbstractAdaptor>("rad", mat);
            if (adaptor == nullptr) 
            {
                _writer->consoleMessage(3, QString("Not Find Material Adaptor, Material:%1 Class:%2")
                    .arg(mat->getDataObjectName()).arg(mat->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(mat);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok) 
                _writer->consoleMessage(3, QString("Write Material Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeNode()
    {
        if (!_writer || !_caseData)return false;
        FITKRadiossAdaptorNode* adaptor = new FITKRadiossAdaptorNode;
        if (adaptor == nullptr) return false;
        //设置写出器
        adaptor->setFileWriter(_writer);
        adaptor->setCaseData(_caseData);
        bool ok = adaptor->adaptW();
        delete adaptor;
        if (!ok) 
            _writer->consoleMessage(3, QString("Write Node Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        return true;
    }

    bool FITKRadiossAdaptorCase::writePart()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossPartManager * partManager = _caseData->getMeshModel()->getPartsManager();
        if (partManager == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "##---------------------------------------------" << endl;
        *stream << "## Parts" << endl;
        *stream << "##---------------------------------------------" << endl;
        //遍历属性
        const int n = partManager->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKRadiossPart* part = partManager->getDataByIndex(i);
            if (part == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", part);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Part Adaptor, Part:%1 Class:%2")
                    .arg(part->getDataObjectName()).arg(part->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(part);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Part Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeDefaultPropValue()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossPropValueManager*  valueMgr = _caseData->getDefaultPropValueManager();
        if (valueMgr == nullptr) return false;

        //遍历属性默认值
        const int n = valueMgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKPropValueAbstract* value = valueMgr->getDataByIndex(i);
            if (value == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", value);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Default Property Value Adaptor, Class:%2")
                    .arg(value->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(value);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Default Property Value Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeProperty()
    {
        if (!_writer || !_caseData)return false;

        Interface::FITKAbstractSectionManager*  propMgr = _caseData->getSectionManager();
        if (propMgr == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (!stream) return false;
        *stream << "##---------------------------------------------" << endl;
        *stream << "## General Property" << endl;
        *stream << "##---------------------------------------------" << endl;
        //遍历属性
        const int n = propMgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            Interface::FITKAbstractSection* prop = propMgr->getDataByIndex(i);
            if (prop == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", prop);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Property Adaptor, Property:%1 Class:%2")
                    .arg(prop->getDataObjectName()).arg(prop->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(prop);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Property Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeCurve()
    {
        if (!_writer || !_caseData)return false;

        FITKNonLinerCurveManager*  curveMgr = _caseData->getNonLinerCurveManager();
        if (curveMgr == nullptr) return false;

        //遍历属性
        const int n = curveMgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKNonLinerCurve* data = curveMgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Curve Adaptor, Curve:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Curve Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeSurface()
    {
        if (!_writer || !_caseData)return false;

        Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
        if (!radiossMeshModel)return false;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return false;

        //遍历
        const int n = componentManager->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            Radioss::FITKAbstractRadiossSurface* data = dynamic_cast<Radioss::FITKAbstractRadiossSurface*>(componentManager->getDataByIndex(i));
            if (!data)continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Surface Set Adaptor,  Surface Set:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Surface Set Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeRigidWall()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossRWallManager*  wallMgr = _caseData->getRWallManager();
        if (wallMgr == nullptr) return false;

        //遍历属性
        const int n = wallMgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractRWall* data = wallMgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Rigid Wall Adaptor, Rigid Wall:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Rigid Wall Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeGravity()
    {
        if (!_writer || !_caseData)return false;

        FITKGravityManager*  gMgr = _caseData->getGravityManager();
        if (gMgr == nullptr) return false;

        //遍历属性
        const int n = gMgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKGravity* data = gMgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Gravity Adaptor, Gravity:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Gravity Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeInitialField()
    {
        if (!_writer || !_caseData)return false;

        FITKInitialFieldManager*  Mgr = _caseData->getInitialFieldManager();
        if (Mgr == nullptr) return false;

        //遍历属性
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractInitialField* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Initial Field Adaptor, Initial Field:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Initial Field Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeNodeSet()
    {
        if (!_writer || !_caseData)return false;

        Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
        if (!radiossMeshModel)return false;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return false;

        //遍历
        const int n = componentManager->getDataCount();
        for (int i = 0; i < n;++i) 
        {
            Interface::FITKModelSet* data = dynamic_cast<Interface::FITKModelSet*>(componentManager->getDataByIndex(i));
            if (!data)continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Node Group Adaptor, Node Group:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Node Group Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeBC()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossBCSManager* Mgr = _caseData->getBCSManager();
        if (Mgr == nullptr) return false;

        //遍历属性
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractBCS* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find BC Adaptor, BC:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write BC Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeSubset()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossSubSetManager* Mgr = _caseData->getMeshModel()->getSubSetManager();
        if (Mgr == nullptr) return false;

        auto writeSubset = [&](FITKRadiossSubSet* data) 
        {
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find SubSet Adaptor, SubSet:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                return;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write SubSet Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        };
        //遍历属性
        const int n = Mgr->getDataCount();
        //先写出普通子集，再写出嵌套子集
        for (int i = 0; i < n; ++i)
        {
            FITKRadiossSubSet* data = Mgr->getDataByIndex(i);
            if (data == nullptr|| data->isCombination()) continue;
            writeSubset(data);
        }
        for (int i = 0; i < n; ++i)
        {
            FITKRadiossSubSet* data = Mgr->getDataByIndex(i);
            if (data == nullptr || !data->isCombination()) continue;
            writeSubset(data);
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeDefaultInteraction()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossInteractionDefaultManager*  valueMgr = _caseData->getDefaultInteractionManager();
        if (valueMgr == nullptr) return false;

        //遍历属性默认值
        const int n = valueMgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKInteractionDefaultAbstract* value = valueMgr->getDataByIndex(i);
            if (value == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", value);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Default Interaction Adaptor, Class:%2")
                    .arg(value->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(value);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Default Interaction Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeInteraction()
    {
        if (!_writer || !_caseData)return false;

        FITKInteractionManager* Mgr = _caseData->getInteractionManager();
        if (Mgr == nullptr) return false;

        //遍历
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractInteraction* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Interaction Adaptor, Interaction:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Interaction Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeConnection()
    {
        if (!_writer || !_caseData)return false;

        FITKConnectionManager* Mgr = _caseData->getCurrentSolution()->getConnectionManager();
        if (Mgr == nullptr) return false;

        //遍历
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractConnection* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Connection Adaptor, Interaction:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Connection Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeProbe()
    {
        if (!_writer || !_caseData)return false;

        FITKProbeManager* Mgr = _caseData->getCurrentSolution()->getProbeManager();
        if (Mgr == nullptr) return false;

        //遍历
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractProbe* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Probe Adaptor, Interaction:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Probe Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeTimerHistory()
    {
        if (!_writer || !_caseData)return false;

        FITKTimeHistoryManager* Mgr = _caseData->getCurrentSolution()->getTimeHietoryManager();
        if (Mgr == nullptr) return false;

        //遍历
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractTimeHistory* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            if (!data->isEnable())continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Time History Adaptor, Interaction:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Time History Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeEOS()
    {
        if (!_writer || !_caseData)return false;

        FITKEquationOfStateManager* Mgr = _caseData->getEquationOfState();
        if (Mgr == nullptr) return false;

        //遍历
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractEquationOfState* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find EOS Adaptor, Interaction:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write EOS Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }
    bool FITKRadiossAdaptorCase::writeSPHGLO()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossSolverSettings* solverSetting = _caseData->getSolverSettings();
        if (!solverSetting) return false;
        FITKRadiossSolverSPHGlobal* sph = solverSetting->getGlobalSPHParam();
        if (!sph) return false;
        if (sph->isEnable() == false)
            return true;
        //获取适配器
        FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
            <FITKRadiossAbstractAdaptor>("rad", sph);
        if (adaptor == nullptr)
        {
            _writer->consoleMessage(3, QString("Not Find SPHGLO Adaptor"));
            return false;
        }
        //设置写出器
        adaptor->setFileWriter(_writer);
        adaptor->setDataObject(sph);
        bool ok = adaptor->adaptW();
        delete adaptor;
        if (!ok)
            _writer->consoleMessage(3, QString("Write SPHGLO Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));

        return true;
    }

    bool FITKRadiossAdaptorCase::writeFailure()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossFailureModelManager* Mgr = _caseData->getFailureModelManager();
        if (Mgr == nullptr) return false;

        //遍历
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractFailureModel* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find Failure Adaptor, Interaction:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write Failure Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }

    bool FITKRadiossAdaptorCase::writeBEM()
    {
        if (!_writer || !_caseData)return false;

        FITKRadiossBEMManager* Mgr = _caseData->getBEMManager();
        if (Mgr == nullptr) return false;

        //遍历BEM
        const int n = Mgr->getDataCount();
        for (int i = 0; i < n; ++i)
        {
            FITKAbstractBEM* data = Mgr->getDataByIndex(i);
            if (data == nullptr) continue;
            //获取适配器
            FITKRadiossAbstractAdaptor* adaptor = FITKIOADAPTORFACTORY->createT
                <FITKRadiossAbstractAdaptor>("rad", data);
            if (adaptor == nullptr)
            {
                _writer->consoleMessage(3, QString("Not Find BEM Adaptor, BEM:%1 Class:%2")
                    .arg(data->getDataObjectName()).arg(data->getClassName()));
                continue;
            }
            //设置写出器
            adaptor->setFileWriter(_writer);
            adaptor->setDataObject(data);
            bool ok = adaptor->adaptW();
            delete adaptor;
            if (!ok)
                _writer->consoleMessage(3, QString("Write BEM Error, File:%1 Line:%2").arg(__FILE__).arg(__LINE__));
        }
        return true;
    }
}