/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "ResultRequestWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "SolutionWapper.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossResultRequest.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSolution.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

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

//创建ResultRequest实例
ResultRequest* ResultRequestWapper::new_ResultRequest()
{
    return new ResultRequest();
}

//释放ResultRequest实例
void ResultRequestWapper::delete_ResultRequest(ResultRequest* obj)
{
    if (obj != nullptr)
    {
        delete obj;
    }
}

//设置动画开始时间
QString ResultRequestWapper::setStartTime(ResultRequest* obj, double time)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Set Start Time failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Set Start Time failed!");
        return sendMessageToServer("Set Start Time failed!");
    }
    resultRequest->setStartTime(time);
    return sendMessageToServer("Set Start Time succeeded!");
}

//设置动画频率
QString ResultRequestWapper::setFrequency(ResultRequest* obj, double f)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Set Frequency failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Set Frequency failed!");
        return sendMessageToServer("Set Frequency failed!");
    }
    resultRequest->setFrequency(f);
    return sendMessageToServer("Set Frequency succeeded!");
}

//设置时间历程频率
QString ResultRequestWapper::setHisFrequency(ResultRequest* obj, double f)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Set History Frequency failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Set History Frequency failed!");
        return sendMessageToServer("Set History Frequency failed!");
    }
    resultRequest->setHisFrequency(f);
    return sendMessageToServer("Set History Frequency succeeded!");
}

//设置节点数据动画类型
QString ResultRequestWapper::setNodalOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Set Nodal Output Anim failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Set Nodal Output Anim failed!");
        return sendMessageToServer("Set Nodal Output Anim failed!");
    }

    QList<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> animList;
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        int type = var.toInt(&ok);
        if (ok)
        {
            animList.append(static_cast<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType>(type));
        }
    }
    resultRequest->setNodalOutputAnim(animList);
    return sendMessageToServer("Set Nodal Output Anim succeeded!");
}

QString ResultRequestWapper::appendNodalOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Append Nodal Output Anim failed!");
    }
    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Append Nodal Output Anim failed!");
        return sendMessageToServer("Append Nodal Output Anim failed!");
    }
    QList<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType> animList = resultRequest->getNodalOutputAnim();
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType type = static_cast<Radioss::FITKRadiossResultRequest::NodalOutputAnimationsType>(var.toInt(&ok));
        if (ok && !animList.contains(type))
        {
            animList.append(type);
        }
    }
    resultRequest->setNodalOutputAnim(animList);
    return sendMessageToServer("Append Nodal Output Anim succeeded!");
}

//设置单元数据动画类型
QString ResultRequestWapper::setElementalOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Set Elemental Output Anim failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Set Elemental Output Anim failed!");
        return sendMessageToServer("Set Elemental Output Anim failed!");
    }

    QList<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> animList;
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        int type = var.toInt(&ok);
        if (ok)
        {
            animList.append(static_cast<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType>(type));
        }
    }
    resultRequest->setElementalOutputAnim(animList);
    return sendMessageToServer("Set Elemental Output Anim succeeded!");
}

QString ResultRequestWapper::appendElementalOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Append Elemental Output Anim failed!");
    }
    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Append Elemental Output Anim failed!");
        return sendMessageToServer("Append Elemental Output Anim failed!");
    }
    QList<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType> animList = resultRequest->getElementalOutputAnim();
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType type = static_cast<Radioss::FITKRadiossResultRequest::ElementalOutputAnimationsType>(var.toInt(&ok));
        if (ok && !animList.contains(type))
        {
            animList.append(type);
        }
    }
    resultRequest->setElementalOutputAnim(animList);
    return sendMessageToServer("Append Elemental Output Anim succeeded!");
}

//设置Brick数据动画类型
QString ResultRequestWapper::setBrickOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Set Brick Output Anim failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Set Brick Output Anim failed!");
        return sendMessageToServer("Set Brick Output Anim failed!");
    }

    QList<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> animList;
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        int type = var.toInt(&ok);
        if (ok)
        {
            animList.append(static_cast<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType>(type));
        }
    }
    resultRequest->setBrickOutputAnim(animList);
    return sendMessageToServer("Set Brick Output Anim succeeded!");
}

QString ResultRequestWapper::appendBrickOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Append Elemental Output Anim failed!");
    }
    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Append Brick Output Anim failed!");
        return sendMessageToServer("Append Brick Output Anim failed!");
    }
    QList<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType> animList = resultRequest->getBrickOutputAnim();
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType type = static_cast<Radioss::FITKRadiossResultRequest::BrickOutputAnimationsType>(var.toInt(&ok));
        if (ok && !animList.contains(type))
        {
            animList.append(type);
        }
    }
    resultRequest->setBrickOutputAnim(animList);
    return sendMessageToServer("Append Brick Output Anim succeeded!");
}

//设置Shell数据动画类型
QString ResultRequestWapper::setShellOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Set Shell Output Anim failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Set Shell Output Anim failed!");
        return sendMessageToServer("Set Shell Output Anim failed!");
    }

    QList<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> animList;
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        int type = var.toInt(&ok);
        if (ok)
        {
            animList.append(static_cast<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType>(type));
        }
    }
    resultRequest->setShellOutputAnim(animList);
    return sendMessageToServer("Set Shell Output Anim succeeded!");
}

QString ResultRequestWapper::appendShellOutputAnim(ResultRequest* obj, const QVariantList& typeList)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Append Shell Output Anim failed!");
    }
    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Append Shell Output Anim failed!");
        return sendMessageToServer("Append Shell Output Anim failed!");
    }
    QList<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType> animList = resultRequest->getShellOutputAnim();
    for (const QVariant& var : typeList)
    {
        bool ok = false;
        Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType type = static_cast<Radioss::FITKRadiossResultRequest::ShellOutputAnimationsType>(var.toInt(&ok));
        if (ok && !animList.contains(type))
        {
            animList.append(type);
        }
    }
    resultRequest->setShellOutputAnim(animList);
    return sendMessageToServer("Append Shell Output Anim succeeded!");
}

//清理参数
QString ResultRequestWapper::clearResultRequestParam(ResultRequest* obj)
{
    if (obj == nullptr)
    {
        AppFrame::FITKMessageError("ResultRequest object is null!");
        return sendMessageToServer("Clear ResultRequest Param failed!");
    }

    Radioss::FITKRadiossResultRequest* resultRequest = obj->getResultRequest();
    if (resultRequest == nullptr)
    {
        AppFrame::FITKMessageError("Clear Param failed!");
        return sendMessageToServer("Clear ResultRequest Param failed!");
    }
    resultRequest->clearParam();
    return sendMessageToServer("Clear ResultRequest Param succeeded!");
}

//更新图形和树形事件
void ResultRequestWapper::updateGraphAndTreeEvent(int dataObjId)
{
    //更新树形菜单事件
    QStringList listOpertor = { "TreeSolutionEvent" };
    for (int i = 0; i < listOpertor.size(); ++i)
    {
        EventOper::TreeEventOperator* oper =
            dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr)
        {
            oper->updateTree();
        }
    }

    //更新图形显示
    EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph)
    {
        operGraph->updateGraph(dataObjId, true);
    }
}

//获取结果请求对象
Radioss::FITKRadiossResultRequest* ResultRequest::getResultRequest()
{
    //获取求解器设置
    Radioss::FITKRadiossCase* radiossCase =
        FITKAPP->getGlobalData()->getPhysicsData<Radioss::FITKRadiossCase>();
    if (radiossCase == nullptr)
    {
        return nullptr;
    }

    return radiossCase->getResultRequest();
}
