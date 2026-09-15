/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "GUIPickInfo.h"
#include "PickedData.h"
#include "PickedDataCalculator.h"
#include "PickedDataProvider.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "GraphDataVTKAdaptor/GraphVTKCommons.h"
#include "GraphDataVTKAdaptor/GraphVTKObject3D.h"
#include "GraphDataVTKAdaptor/GraphDataVTKPartSetManager.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKKeyMouseStates.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKSignalTransfer.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKVTKAlgorithm/FITKGraphActor.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"

#include <vtkPlanes.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkPoints.h>
#include <vtkSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkAppendFilter.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelection.h>

namespace GraphData
{
    // 静态变量初始化。
    PickedDataProvider* PickedDataProvider::s_instance{ nullptr };

    PickedDataProvider* PickedDataProvider::getInstance()
    {
        // 获取实例。
        if (!s_instance)
        {
            s_instance = new PickedDataProvider;
        }

        return s_instance;
    }

    void PickedDataProvider::Delete()
    {
        if (s_instance)
        {
            delete s_instance;
        }

        s_instance = nullptr;
    }

    PickedDataProvider::PickedDataProvider()
    {
        // 初始化键盘监视器。
        m_settings = FITKAPP->getGlobalData()->getKeyMouseStates();

        // 创建拾取可视化数据数据。
        m_dataSetPicked = vtkUnstructuredGrid::New();

        // 创建预选可视化数据。
        m_dataSetPreview = vtkUnstructuredGrid::New();
    }

    PickedDataProvider::~PickedDataProvider()
    {
        clearPickedData();

        // 清除VTK数据。
        if (m_dataSetPicked)
        {
            m_dataSetPicked->Delete();
            m_dataSetPicked = nullptr;
        }

        if (m_dataSetPreview)
        {
            m_dataSetPreview->Delete();
            m_dataSetPreview = nullptr;
        }
    }

    void PickedDataProvider::clearPickedData()
    {
        // 清除并析构。
        for (PickedData* data : m_pickedDataList)
        {
            if (data)
            {
                data->clearHighlight();
                delete data;
            }
        }

        m_pickedDataList.clear();

        // 同时清空可视化数据。
        if (m_dataSetPicked)
        {
            m_dataSetPicked->Reset();
            m_dataSetPicked->Modified();
        }

        // 同时清空预选数据。
        clearPickedDataPreview();

        //刷新
        EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
        if (operGraph) {
            operGraph->render();
        }
    }

    void PickedDataProvider::clearPickedDataPreview()
    {
        if (m_dataSetPreview)
        {
            m_dataSetPreview->Reset();
            m_dataSetPreview->Modified();
        }

        if (m_pickedPreviewData)
        {
            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }
    }

    vtkDataSet* PickedDataProvider::getPickedPreviewDataSet()
    {
        return m_dataSetPreview;
    }

    vtkDataSet* PickedDataProvider::getPickedDataSet()
    {
        return m_dataSetPicked;
    }

    PickedData* PickedDataProvider::getPickedData(vtkActor* actor, int index, double* pickedWorldPos, bool isPreview)
    {
        // 获取拾取方式及附加信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

        // 创建拾取信息。
        PickedData* pickedData = new PickedData(pickInfo, actor, index, pickedWorldPos, isPreview);

        // 判断拾取是否有效。
        if (!pickedData->isValid())
        {
            delete pickedData;
            return nullptr;
        }

        // 创建临时计算器。
        PickedDataCalculator calculator(pickedData);
        calculator.calculate();

        return pickedData;
    }

    PickedData* PickedDataProvider::getPickedData(vtkActor2D* actor, bool isPreview)
    {
        // 获取拾取方式及附加信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

        // 创建拾取信息。
        PickedData* pickedData = new PickedData(pickInfo, actor, isPreview);

        // 判断拾取是否有效。
        if (!pickedData->isValid())
        {
            delete pickedData;
            return nullptr;
        }

        // 创建临时计算器。
        PickedDataCalculator calculator(pickedData);
        calculator.calculate();

        return pickedData;
    }

    bool PickedDataProvider::addPreviewPicked(vtkActor* actor, int index, double* pickedWorldPos)
    {
        if (m_pickedPreviewData)
        {
            // 获取拾取方式及附加信息。
            GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

            bool isTheSamePick = false;

            if (m_pickedPreviewData->getPickedInfo()._pickObjType != pickInfo._pickObjType ||
                m_pickedPreviewData->getPickedInfo()._pickMethod != pickInfo._pickMethod ||
                m_pickedPreviewData->getPickedInfo()._pickParam != pickInfo._pickParam)
            {
                // 判断是否与上次预选相同，相同则跳出，节约性能。
                if (m_pickedPreviewData->contains(actor, index))
                {
                    isTheSamePick = true;
                }
            }

            if (isTheSamePick)
            {
                return false;
            }

            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }

        // 计算生成拾取数据。
        PickedData* pickedData = getPickedData(actor, index, pickedWorldPos, true);
        if (!pickedData)
        {
            return false;
        }

        m_pickedPreviewData = pickedData;

        // 生成预选数据。
        generatePickedPreviewDataSet();

        return true;
    }

    bool PickedDataProvider::addPreviewPicked(vtkActor2D* actor)
    {
        if (m_pickedPreviewData)
        {
            bool isTheSamePick = false;

            // 判断是否与上次预选相同，相同则跳出，节约性能。
            if (m_pickedPreviewData->contains(actor))
            {
                isTheSamePick = true;
            }

            if (isTheSamePick)
            {
                return false;
            }

            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }

        // 计算生成拾取数据。
        PickedData* pickedData = getPickedData(actor, true);
        if (!pickedData)
        {
            return false;
        }

        m_pickedPreviewData = pickedData;

        // 生成预选数据。
        generatePickedPreviewDataSet();

        return true;
    }

    void PickedDataProvider::addPicked(vtkActor* actor, int index, double* pickedWorldPos)
    {
        //创建对象信息哈希表
        QHash<QString, void*> objInfo;
        bool pickStart = true;
        PickedData* pickedData{ nullptr };
        objInfo.insert("PickStart", &pickStart);
        // 如果有预选数据则使用预选数据。
        if (m_pickedPreviewData)
        {
            pickedData = m_pickedPreviewData->getCopy();

            // 拷贝后析构，防止快速进行多次拾取导致无法预选。
            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }
        else
        {
            // 计算生成拾取数据。
            pickedData = getPickedData(actor, index, pickedWorldPos);
        }

        if (!pickedData)
        {
            return;
        }

        // 处理拾取数据。
        dealPickedData(pickedData);

        // 生成拾取数据。
        generatePickedDataSet();
        emit sigDataPicked();
        // 如果有数据被拾取，则发送信号。
        if (m_pickedDataList.count() == 0)
        {
            bool pickError = true;
            objInfo.insert("PickError", &pickError);
            FITKAPP->getSignalTransfer()->graphObjectPickedSig(-1, -1, objInfo);
            return;
        }
        //有拾取

        QList<PickedData*> dataList = m_pickedDataList;
        for (PickedData* pData : dataList) {

            if (pData) {
                //获取拾取类型并转换
                int pickType = -1;
                switch (pData->getPickedInfo()._pickObjType)
                {
                case GUI::GUIPickInfo::PickObjType::POBGeoVert:
                    pickType = 3003; // 几何点
                    break;
                case GUI::GUIPickInfo::PickObjType::POBGeoEdge:
                    pickType = 3004; // 几何线
                    break;
                case GUI::GUIPickInfo::PickObjType::POBGeoFace:
                    pickType = 3005; // 几何面
                    break;
                case GUI::GUIPickInfo::PickObjType::POBGeoSolid:
                    pickType = 3006; // 几何体
                    break;
                default:
                    pickType = -1;
                    break;
                }
                int cmdID = pData->getPickedDataObjId();
                objInfo.insert("CmdId", &cmdID);
                //遍历拾取ID和虚拓扑ID，逐个发送
                QHash<int, int> pickedOtherIDs = pData->getPickedOtherIDs();
                QList<PickDataInfo> infos = pData->getPickOtherInfos();
                for (const PickDataInfo& info : infos) {

                    if (pickedOtherIDs.contains(info._id)) {

                        FITKAPP->getSignalTransfer()->graphObjectPickedSig(pickType, info._id, objInfo);
                        pickStart = false;
                        objInfo.insert("PickStart", &pickStart);
                    }
                }

                //拾取结束
                if (pData->isFinished()) {
                    emit sigPickOver();
                }
            }
        }

    }

    void PickedDataProvider::addPicked(vtkActor2D* actor)
    {
        PickedData* pickedData{ nullptr };

        // 如果有预选数据则使用预选数据。
        if (m_pickedPreviewData)
        {
            pickedData = m_pickedPreviewData->getCopy();

            // 拷贝后析构，防止快速进行多次拾取导致无法预选。
            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
        }
        else
        {
            // 计算生成拾取数据。
            pickedData = getPickedData(actor);
        }

        if (!pickedData)
        {
            return;
        }

        // 处理拾取数据。
        dealPickedData(pickedData);

        // 生成拾取数据。
        generatePickedDataSet();

        // 如果有数据被拾取，发送信号。
        if (m_pickedDataList.count() != 0)
        {
            emit sigDataPicked();
        }
    }

    void PickedDataProvider::addPicked(QList<vtkActor*> actors, vtkPlanes* cutPlane)
    {
        //创建对象信息哈希表
        QHash<QString, void*> objInfo;
        bool pickStart = true;
        objInfo.insert("PickStart", &pickStart);
        // 获取拾取方式及附加信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

        // Single拾取模式禁止框选。
        if (pickInfo._pickMethod == GUI::GUIPickInfo::PickMethod::PMSingle)
        {
            return;
        }

        // 是否需要合并或移除数据。
        bool needAddOrSubData = (m_settings->keyPressed(Qt::Key_Shift) && !m_settings->keyPressed(Qt::Key_Control)) ||
            (!m_settings->keyPressed(Qt::Key_Shift) && m_settings->keyPressed(Qt::Key_Control));

        // 不需要操作数据说明是普通框选，需要清空历史拾取信息。
        if (!needAddOrSubData)
        {
            clearPickedData();
        }

        for (vtkActor* actor : actors)
        {
            // 创建拾取信息。
            PickedData* pickedData = new PickedData(pickInfo, actor, cutPlane);

            // 判断拾取是否有效。
            if (!pickedData->isValid())
            {
                delete pickedData;
                continue;
            }

            // 创建临时计算器。
            PickedDataCalculator calculator(pickedData);
            calculator.calculate();

            // 处理拾取数据。
            dealPickedData(pickedData, true);
        }

        // 生成拾取数据。
        generatePickedDataSet();
        emit sigDataPicked();
        // 如果有数据被拾取，发送信号。
        if (m_pickedDataList.count() == 0)
        {
            bool pickError = true;
            objInfo.insert("PickError", &pickError);
            int cmdID = -1;
            objInfo.insert("CmdId", &cmdID);
            FITKAPP->getSignalTransfer()->graphObjectPickedSig(-1, -1, objInfo);
            return;
        }

        QList<PickedData*> dataList = m_pickedDataList;
        for (PickedData* pData : dataList) {
            if (pData) {

                //获取拾取类型并转换
                int pickType = -1;
                switch (pData->getPickedInfo()._pickObjType)
                {
                case GUI::GUIPickInfo::PickObjType::POBGeoVert:
                    pickType = 3003; // 几何点
                    break;
                case GUI::GUIPickInfo::PickObjType::POBGeoEdge:
                    pickType = 3004; // 几何线
                    break;
                case GUI::GUIPickInfo::PickObjType::POBGeoFace:
                    pickType = 3005; // 几何面
                    break;
                case GUI::GUIPickInfo::PickObjType::POBGeoSolid:
                    pickType = 3006; // 几何体
                    break;
                default:
                    pickType = -1;
                    break;
                }

                int cmdID = pData->getPickedDataObjId();
                objInfo.insert("CmdId", &cmdID);

                //遍历拾取ID和虚拓扑ID，逐个发送
                QHash<int, int> pickedOtherIDs = pData->getPickedOtherIDs();
                QList<PickDataInfo> infos = pData->getPickOtherInfos();
                for (const PickDataInfo info : infos) {

                    if (pickedOtherIDs.contains(info._id)) {

                        FITKAPP->getSignalTransfer()->graphObjectPickedSig(pickType, info._id, objInfo);
                        pickStart = false;
                        objInfo.insert("PickStart", &pickStart);
                    }
                }

                //拾取结束
                if (pData->isFinished()) {
                    emit sigPickOver();
                }
            }
        }
    }

    void PickedDataProvider::generatePickedPreviewDataSet()
    {
        if (!m_dataSetPreview)
        {
            return;
        }

        m_dataSetPreview->Reset();

        if (!m_pickedPreviewData)
        {
            m_dataSetPreview->Modified();
            return;
        }

        // 为空则不进行计算。
        if (m_pickedPreviewData->isEmpty())
        {
            m_dataSetPreview->Modified();

            delete m_pickedPreviewData;
            m_pickedPreviewData = nullptr;
            return;
        }

        // 获取预选三维数据。
        vtkUnstructuredGrid* ugrid = vtkUnstructuredGrid::New();
        m_pickedPreviewData->getDataSet(ugrid);
        m_dataSetPreview->DeepCopy(ugrid);
        ugrid->Delete();
        m_dataSetPreview->Modified();
    }

    void PickedDataProvider::generatePickedDataSet()
    {
        // 合并拾取可视化数据。
        vtkAppendFilter* appendFilter = vtkAppendFilter::New();

        // 移除已经被清空的数据。
        for (int i = m_pickedDataList.count() - 1; i >= 0; i--)
        {
            PickedData* data = m_pickedDataList[i];
            if (data->isEmpty())
            {
                m_pickedDataList.removeAt(i);
                delete data;
            }
        }

        for (PickedData* data : m_pickedDataList)
        {
            if (!data)
            {
                continue;
            }

            // 外部创建网格数据，内部进行数据填充。
            vtkUnstructuredGrid* ugrid = vtkUnstructuredGrid::New();
            data->getDataSet(ugrid);
            appendFilter->AddInputData(ugrid);

            ugrid->Delete();
        }

        // 合并数据并拷贝。
        appendFilter->Update();

        m_dataSetPicked->DeepCopy(appendFilter->GetOutput());
        m_dataSetPicked->Modified();
        appendFilter->Delete();
    }

    void PickedDataProvider::dealPickedData(PickedData* data, bool isAreaPick)
    {
        // 获取拾取方式及附加信息。
        GUI::GUIPickInfoStru pickInfo = GUI::GUIPickInfo::GetPickInfo();

        // 保存拾取时按键状态。
        bool ctrlPressed = m_settings->keyPressed(Qt::Key::Key_Control);
        bool shiftPressed = m_settings->keyPressed(Qt::Key::Key_Shift);
        data->saveKeyFlags(Qt::Key::Key_Control, ctrlPressed);
        data->saveKeyFlags(Qt::Key::Key_Shift, shiftPressed);

        //追加拾取(仅按下shift)
        if (m_settings->keyPressed(Qt::Key_Shift) && !m_settings->keyPressed(Qt::Key_Control) && pickInfo._pickMethod != GUI::GUIPickInfo::PickMethod::PMSingle) {
            // 获取相同模型拾取信息进行合并，否则直接添加。
            PickedData* brotherData = getSameModelPickData(data);
            if (brotherData) {
                brotherData->add(data);
            }
            else {
                m_pickedDataList.push_back(data);
            }
        }
        //拾取擦除(仅按下ctrl)
        else if (!m_settings->keyPressed(Qt::Key_Shift) && m_settings->keyPressed(Qt::Key_Control)) {
            // 获取相同模型拾取信息进行做差，否则直接跳出。
            PickedData* brotherData = getSameModelPickData(data);
            if (brotherData) {
                brotherData->subtract(data);
            }
            else {
                return;
            }
        }
        //结束拾取(按下ctrl且按下shift)
        else if (m_settings->keyPressed(Qt::Key_Shift) && m_settings->keyPressed(Qt::Key_Control)) {
            // 获取相同模型拾取信息进行合并，否则直接添加。
            PickedData* brotherData = getSameModelPickData(data);
            if (brotherData) {
                brotherData->add(data);
            }
            else {
                m_pickedDataList.push_back(data);
            }
        }
        //取消重新拾取
        else {
            if (!isAreaPick) {
                clearPickedData();
            }
            m_pickedDataList.push_back(data);
        }

        // 移除已经被清空的数据。
        for (int i = m_pickedDataList.count() - 1; i >= 0; i--)
        {
            PickedData* data = m_pickedDataList[i];
            if (data->isEmpty())
            {
                m_pickedDataList.removeAt(i);
                delete data;
            }
        }
    }

    PickedData* PickedDataProvider::getSameModelPickData(PickedData* data)
    {
        if (!data || m_pickedDataList.isEmpty())
        {
            return nullptr;
        }

        // 寻找拾取到的同模型数据。
        for (PickedData* pData : m_pickedDataList)
        {
            if (data->isSameAs(pData))
            {
                return pData;
            }
        }

        return nullptr;
    }

    QList<PickedData*> PickedDataProvider::getPickedList()
    {
        return m_pickedDataList;
    }
}