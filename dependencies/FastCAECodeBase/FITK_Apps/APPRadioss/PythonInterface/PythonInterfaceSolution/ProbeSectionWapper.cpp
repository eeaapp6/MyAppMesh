/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ProbeSectionWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"

#include "FITK_Component/FITKRadiossData/FITKProbeSection.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossElementGroup.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

namespace
{
    Interface::FITKModelSet* getModelSetByName(const QString& name)
    {
        if (name.isEmpty())
        {
            return nullptr;
        }

        Radioss::FITKRadiossCase* radiossCase =
            FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
        if (radiossCase == nullptr)
        {
            return nullptr;
        }

        Radioss::FITKRadiossMeshModel* meshModel = radiossCase->getMeshModel();
        if (meshModel == nullptr)
        {
            return nullptr;
        }

        Interface::FITKComponentManager* componentManager = meshModel->getComponentManager();
        if (componentManager == nullptr)
        {
            return nullptr;
        }

        return dynamic_cast<Interface::FITKModelSet*>(componentManager->getDataByName(name));
    }
}

ProbeSection::ProbeSection(const QString& name) :
    PyInterface::PyProbeBase(name)
{
}

ProbeSection::ProbeSection() :
    PyInterface::PyProbeBase()
{
}

Radioss::FITKProbeSection* ProbeSection::getProbeSection()
{
    return dynamic_cast<Radioss::FITKProbeSection*>(PyInterface::PyProbeBase::getProbe());
}

bool ProbeSection::setSingleNodeGroup1(const QString& name)
{
    Radioss::FITKProbeSection* probe = getProbeSection();
    Interface::FITKModelSet* modelSet = getModelSetByName(name);
    if (probe == nullptr || modelSet == nullptr || modelSet->getMemberCount() != 1)
    {
        return false;
    }

    probe->setSetID1(modelSet->getDataObjectID());
    updateRender();
    return true;
}

bool ProbeSection::setSingleNodeGroup2(const QString& name)
{
    Radioss::FITKProbeSection* probe = getProbeSection();
    Interface::FITKModelSet* modelSet = getModelSetByName(name);
    if (probe == nullptr || modelSet == nullptr || modelSet->getMemberCount() != 1)
    {
        return false;
    }

    probe->setSetID2(modelSet->getDataObjectID());
    updateRender();
    return true;
}

bool ProbeSection::setSingleNodeGroup3(const QString& name)
{
    Radioss::FITKProbeSection* probe = getProbeSection();
    Interface::FITKModelSet* modelSet = getModelSetByName(name);
    if (probe == nullptr || modelSet == nullptr || modelSet->getMemberCount() != 1)
    {
        return false;
    }

    probe->setSetID3(modelSet->getDataObjectID());
    updateRender();
    return true;
}

bool ProbeSection::setNodeGroup(const QString& name)
{
    Radioss::FITKProbeSection* probe = getProbeSection();
    Interface::FITKModelSet* modelSet = getModelSetByName(name);
    if (probe == nullptr || modelSet == nullptr)
    {
        return false;
    }

    probe->setGrndID(modelSet->getDataObjectID());
    updateRender();
    return true;
}

bool ProbeSection::setShellElementGroup(const QString& name)
{
    Radioss::FITKProbeSection* probe = getProbeSection();
    Interface::FITKModelSet* modelSet = getModelSetByName(name);
    if (probe == nullptr || modelSet == nullptr ||
        modelSet->getModelSetType() != Interface::FITKModelEnum::FITKModelSetType::FMSElem)
    {
        return false;
    }

    probe->setElementGroupType(Radioss::FITKRadiossElementGroup::ElementGroupType::GRSHEL);
    probe->setGrshelID(modelSet->getDataObjectID());
    updateRender();
    return true;
}

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

ProbeSection* ProbeSectionWapper::new_ProbeSection()
{
    return new ProbeSection();
}

void ProbeSectionWapper::delete_ProbeSection(ProbeSection* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

ProbeSection* ProbeSectionWapper::static_ProbeSection_GetProbeSection(const QString& name)
{
    return new ProbeSection(name);
}

QString ProbeSectionWapper::getName(ProbeSection* obj)
{
    if (obj == nullptr)
    {
        return QString();
    }

    return obj->getName();
}

QString ProbeSectionWapper::setName(ProbeSection* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->setName(name))
    {
        return sendMessageToServer(tr("Failed, Probe section with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Set name to %1").arg(name));
}

QString ProbeSectionWapper::createProbe(ProbeSection* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    if (!obj->createProbe(name, static_cast<int>(Radioss::FITKAbstractProbe::Probe_Section)))
    {
        return sendMessageToServer(tr("Creation failed! Probe section with name %1 may already exist").arg(name));
    }

    return sendMessageToServer(tr("Success, Probe section created with name %1").arg(obj->getName()));
}

QString ProbeSectionWapper::setSingleNodeGroup1(ProbeSection* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        probe->setSetID1(-1);
        return sendMessageToServer(tr("Clear probe section single node group1 successfully"));
    }

    if (!obj->setSingleNodeGroup1(name))
    {
        return sendMessageToServer(tr("Failed !Single node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set probe section single node group1 successfully"));
}

QString ProbeSectionWapper::setSingleNodeGroup2(ProbeSection* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        probe->setSetID2(-1);
        return sendMessageToServer(tr("Clear probe section single node group2 successfully"));
    }

    if (!obj->setSingleNodeGroup2(name))
    {
        return sendMessageToServer(tr("Failed !Single node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set probe section single node group2 successfully"));
}

QString ProbeSectionWapper::setSingleNodeGroup3(ProbeSection* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        probe->setSetID3(-1);
        return sendMessageToServer(tr("Clear probe section single node group3 successfully"));
    }

    if (!obj->setSingleNodeGroup3(name))
    {
        return sendMessageToServer(tr("Failed !Single node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set probe section single node group3 successfully"));
}

QString ProbeSectionWapper::setNodeGroup(ProbeSection* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        probe->setGrndID(-1);
        return sendMessageToServer(tr("Clear probe section node group successfully"));
    }

    if (!obj->setNodeGroup(name))
    {
        return sendMessageToServer(tr("Failed !Node group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set probe section node group successfully"));
}

QString ProbeSectionWapper::setIsave(ProbeSection* obj, int flag)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    probe->setIsave(flag);
    return sendMessageToServer(tr("Success, Set probe section isave to %1").arg(flag));
}

// QString ProbeSectionWapper::setFrameID(ProbeSection* obj, int frameID)
// {
//     if (obj == nullptr)
//     {
//         return tr("Failed, object is null");
//     }

//     Radioss::FITKProbeSection* probe = obj->getProbeSection();
//     if (probe == nullptr)
//     {
//         return tr("Failed !No probe section found!");
//     }

//     probe->setFrameID(frameID);
//     return tr("Success, Set probe section frame ID to %1").arg(frameID);
// }

QString ProbeSectionWapper::setFileName(ProbeSection* obj, const QString& fileName)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    probe->setFileName(fileName);
    return sendMessageToServer(tr("Success, Set probe section file name to %1").arg(fileName));
}

QString ProbeSectionWapper::setDt(ProbeSection* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    probe->setDt(value);
    return sendMessageToServer(tr("Success, Set probe section dt to %1").arg(value));
}

QString ProbeSectionWapper::setAlpha(ProbeSection* obj, double value)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    probe->setAlpha(value);
    return sendMessageToServer(tr("Success, Set probe section alpha to %1").arg(value));
}

QString ProbeSectionWapper::setShellElementGroup(ProbeSection* obj, const QString& name)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    if (name.isEmpty() || name == "None")
    {
        probe->setElementGroupType(Radioss::FITKRadiossElementGroup::ElementGroupType::GRSHEL);
        probe->setGrshelID(-1);
        return sendMessageToServer(tr("Clear probe section shell element group successfully"));
    }

    if (!obj->setShellElementGroup(name))
    {
        return sendMessageToServer(tr("Failed !Shell element group with name %1 not found!").arg(name));
    }

    return sendMessageToServer(tr("Set probe section shell element group successfully"));
}

QString ProbeSectionWapper::setIframe(ProbeSection* obj, int flag)
{
    if (obj == nullptr)
    {
        return sendMessageToServer(tr("Failed, object is null"));
    }

    Radioss::FITKProbeSection* probe = obj->getProbeSection();
    if (probe == nullptr)
    {
        return sendMessageToServer(tr("Failed !No probe section found!"));
    }

    probe->setIframe(flag);
    return sendMessageToServer(tr("Success, Set probe section iframe to %1").arg(flag));
}