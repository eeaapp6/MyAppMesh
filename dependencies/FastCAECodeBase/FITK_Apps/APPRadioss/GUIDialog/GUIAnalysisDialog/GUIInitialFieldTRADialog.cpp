/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIInitialFieldTRADialog.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKInitialField.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include <QMessageBox>

namespace GUI
{
    GUIInitialFieldTRADialog::GUIInitialFieldTRADialog(Core::FITKActionOperator* oper, QWidget* parent)
        : GUIInitialFieldDialog(parent), _oper(oper)
    {
        // 获取模式
        int model = -1;
        _oper->argValue("Model", model);
        setModel(model);

        // 根据模式初始化界面
        // 创建模式
        if (model == 0)
        {
            QString str;
            _oper->argValue("DataName", str);
            setInitialFieldName(str);
            setInitialFieldType("TRA");
            _oper->argValue("DataKeyWord", str);
            setInitialFieldKeyWord(str);
            setInitialFieldX("0.0");
            setInitialFieldY("0.0");
            setInitialFieldZ("0.0");
        }
        // 编辑模式
        else if (model == 1)
        {
            // 获取初始平移速度数据
            Radioss::FITKInitialFieldTra* data = _oper->argValuePtr<Radioss::FITKInitialFieldTra>("InitialFieldData");
            if (data == nullptr) return;

            _oldNameForScript = data->getDataObjectName();
            setInitialFieldName(data->getDataObjectName());
            setInitialFieldType("TRA");
            setInitialFieldKeyWord(Radioss::FITKInitialFieldTra::GetFITKInitialFieldTraRadiossKeyWord());
            setInitialFieldSetID(data->getSetID());
            setInitialFieldX(QString::number(data->getValue(0)));
            setInitialFieldY(QString::number(data->getValue(1)));
            setInitialFieldZ(QString::number(data->getValue(2)));
        }
    }

    bool GUIInitialFieldTRADialog::createInitialField()
    {
        // 创建初始平移速度数据对象
        Radioss::FITKInitialFieldTra* data = new Radioss::FITKInitialFieldTra();
        if (data == nullptr) return false;
        data->setDataObjectName(getInitialFieldName());

        // 集合ID
        int idSet = getInitialFieldSetID();
        if (idSet < 1)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please select a set"));
            return false;
        }
        data->setSetID(idSet);

        // 初始速度
        double value[3] = { 0.0, 0.0, 0.0 };
        bool isValue1Effect = false, isValue2Effect = false, isValue3Effect = false;
        value[0] = getInitialFieldX().toDouble(&isValue1Effect);
        value[1] = getInitialFieldY().toDouble(&isValue2Effect);
        value[2] = getInitialFieldZ().toDouble(&isValue3Effect);
        if (!isValue1Effect || !isValue2Effect || !isValue3Effect)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please set an effective initial value."));
            return false;
        }
        data->setValue(value);

        // 存储数据
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return false;
        Radioss::FITKInitialFieldManager* manager = dataCase->getInitialFieldManager();
        if (manager == nullptr) return false;
        manager->appendDataObj(data);

        _oper->setArgs("DataID", data->getDataObjectID());
        const bool result = _oper->execProfession();
        if (result)
        {
            writePythonScript();
        }

        return result;
    }

    bool GUIInitialFieldTRADialog::modifyInitialField()
    {
        // 创建初始平移速度数据对象
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr) return false;
        Radioss::FITKInitialFieldManager* manager = dataCase->getInitialFieldManager();
        if (manager == nullptr) return false;
        QString lookupName = _oldNameForScript.isEmpty() ? getInitialFieldName() : _oldNameForScript;
        Radioss::FITKAbstractInitialField* data = manager->getDataByName(lookupName);
        if (data == nullptr) return false;
        Radioss::FITKInitialFieldTra* dataTra = dynamic_cast<Radioss::FITKInitialFieldTra*>(data);
        if (dataTra == nullptr) return false;

        if (dataTra->getDataObjectName() != getInitialFieldName())
        {
            dataTra->setDataObjectName(getInitialFieldName());
        }

        // 集合ID
        int idSet = getInitialFieldSetID();
        if (idSet < 1)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please select a set"));
            return false;
        }
        dataTra->setSetID(idSet);

        // 初始速度
        double value[3] = { 0.0, 0.0, 0.0 };
        bool isValue1Effect = false, isValue2Effect = false, isValue3Effect = false;
        value[0] = getInitialFieldX().toDouble(&isValue1Effect);
        value[1] = getInitialFieldY().toDouble(&isValue2Effect);
        value[2] = getInitialFieldZ().toDouble(&isValue3Effect);
        if (!isValue1Effect || !isValue2Effect || !isValue3Effect)
        {
            QMessageBox::warning(this, tr("Warning"), tr("Please set an effective initial value."));
            return false;
        }
        dataTra->setValue(value);

        _oper->setArgs("DataID", dataTra->getDataObjectID());

        const bool result = _oper->execProfession();
        if (result)
        {
            writePythonScript();
        }

        return result;
    }

    void GUIInitialFieldTRADialog::writePythonScript()
    {
        Radioss::FITKRadiossCase* dataCase = FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (dataCase == nullptr)
        {
            return;
        }

        Radioss::FITKRadiossSolution* solutionData = dataCase->getCurrentSolution();
        if (solutionData == nullptr)
        {
            return;
        }

        QStringList script;
        script.append(QString("Solution.Solution.setCurrentSolution('%1')").arg(solutionData->getDataObjectName()));

        if (_model == 0)
        {
            script.append("p=InitialValue.InitialValueTra()");
            script.append(QString("p.createInitialValue('%1')").arg(getInitialFieldName()));
        }
        else if (_model == 1)
        {
            QString oldName = _oldNameForScript.isEmpty() ? getInitialFieldName() : _oldNameForScript;
            script.append(QString("p=InitialValue.InitialValueTra.GetInitialValueTra('%1')").arg(oldName));
            if (oldName != getInitialFieldName())
            {
                script.append(QString("p.setName('%1')").arg(getInitialFieldName()));
            }
        }
        else
        {
            return;
        }

        script.append(QString("p.setValue(%1, %2, %3)")
            .arg(getInitialFieldX())
            .arg(getInitialFieldY())
            .arg(getInitialFieldZ()));
        script.append(QString("p.setNodeGroup('%1')").arg(getInitialFieldSetName()));

        saveScript(script);
    }
}