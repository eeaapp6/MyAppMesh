/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FaceHiddenDialog.h"
#include "ui_FaceHiddenDialog.h"

// APP
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

// Data
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoEnum.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopoMapper.h"

// Operator
#include "OperatorsInterface/GraphEventOperator.h"

// Pick
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/GraphPickedDataProvider.h"
#include "PickDataProvider/GraphPickedData.h"

// Qt
#include <QMessageBox>

namespace GUI {

    FaceHiddenDialog::FaceHiddenDialog(QWidget *parent) :
        GUIDialogBase(parent),
        ui(new Ui::FaceHiddenDialog)
    {
        ui->setupUi(this);
        
        // 初始化界面。
        init();
    }

    FaceHiddenDialog::~FaceHiddenDialog()
    {
        finalize();
        delete ui;
    }

    FaceHiddenDialog* FaceHiddenDialog::New(QWidget * parent, EventOper::ParaWidgetInterfaceOper* oper)
    {
        // 创建单例窗口。
        static FaceHiddenDialog* dlg;
        if (dlg == nullptr)
        {
            dlg = new FaceHiddenDialog(parent);
        }

        if (parent != dlg->parent())
        { 
            dlg->setParent(parent);
        }

        // 保存操作器。
        dlg->m_Operator = oper;

        // 刷新界面。
        dlg->initPickMode();
        dlg->refreshFaceGroupComboBox();

        return dlg;
    }

    void FaceHiddenDialog::init()
    {
        // 界面信号绑定。
        connect(ui->rdBtnPick, &QRadioButton::clicked, this, &FaceHiddenDialog::slot_rdBtnClicked);
        connect(ui->rdBtnFaceGroup, &QRadioButton::clicked, this, &FaceHiddenDialog::slot_rdBtnClicked);
        connect(ui->cmbShapeType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FaceHiddenDialog::slot_pickShapeTypeCmbChanged);
        connect(ui->btnHide, &QPushButton::clicked, this, &FaceHiddenDialog::slot_btnHide);
        connect(ui->btnShowAll, &QPushButton::clicked, this, &FaceHiddenDialog::slot_btnShowAll);
        connect(ui->btnOK, &QPushButton::clicked, this, &FaceHiddenDialog::slot_btnOK);

        // 界面状态初始化。
        ui->rdBtnPick->setChecked(true);
        slot_rdBtnClicked();

        // 初始化面组列表下拉框。
        refreshFaceGroupComboBox();
    }

    void FaceHiddenDialog::initPickMode()
    {
        // 界面状态初始化。
        ui->rdBtnPick->setChecked(true);
        slot_rdBtnClicked();

        // 切换拾取模式。
        ui->cmbShapeType->setCurrentIndex(0);
        slot_pickShapeTypeCmbChanged(0);

        // 清除历史拾取。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        pickDataProvider->clearPickedData();
    }

    void FaceHiddenDialog::hidePickedShapes()
    {
        // 面查询。
        Interface::FITKVirtualTopoMapper vMapperF;

        // 获取拾取数据。
        GraphData::GraphPickedDataProvider* pickDataProvider = GraphData::GraphPickedDataProvider::getInstance();
        QList<GraphData::GraphPickedData*> pickedList = pickDataProvider->getPickedList();
        for (GraphData::GraphPickedData* pickedData : pickedList)
        {
            Interface::FITKAbsGeoShapeAgent* shapeAgent = pickedData->getGeoShapeAgent();
            if (!shapeAgent)
            {
                continue;
            }

            Interface::FITKAbsGeoCommand* cmd = shapeAgent->getGeoCommand();
            if (!cmd)
            {
                continue;
            }

            // 查询该拓扑对应的面。
            Interface::FITKVirtualTopoManager* vTopoMgr = shapeAgent->getVirtualTopoManager();
            if (!vTopoMgr)
            {
                continue;
            }

            // 获取虚拓扑对象管理器。
            Interface::FITKShapeVirtualTopoManager* vFaceMgr = vTopoMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSFace);
            Interface::FITKShapeVirtualTopoManager* vSolidMgr = vTopoMgr->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSSolid);
            if (!vFaceMgr || !vSolidMgr)
            {
                continue;
            }

            QList<int> indice, solidIndice;
            QList<Interface::VirtualShape> vShapes = pickedData->getVirtualShapes();
            for (const Interface::VirtualShape & vShape : vShapes)
            {
                // 面直接插入列表。
                if (vShape.Type == Interface::FITKGeoEnum::VSFace)
                {
                    indice.push_back(vShape.VirtualTopoIndex);
                }
                // 体获取对应的面列表。
                else if (vShape.Type == Interface::FITKGeoEnum::VSSolid)
                {
                    Interface::FITKAbsVirtualTopo* vSolid = vSolidMgr->getDataByIndex(vShape.VirtualTopoIndex);
                    if (!vSolid)
                    {
                        continue;
                    }

                    vMapperF.mapTopo(vSolid, Interface::FITKGeoEnum::VSFace);
                    int nSolidFaces = vMapperF.length();
                    for (int i = 0; i < nSolidFaces; i++)
                    {
                        Interface::FITKAbsVirtualTopo* vFace = vMapperF.virtualTopo(i);
                        int faceIndex = vFaceMgr->getDataIndex(vFace);
                        if (faceIndex != -1)
                        {
                            indice.push_back(faceIndex);
                        }
                    }

                    solidIndice.push_back(vShape.VirtualTopoIndex);
                }
            }

            m_GraphOper->setShapeHidden(cmd->getDataObjectID(), Interface::FITKGeoEnum::VSFace, indice);
            m_GraphOper->setShapeHidden(cmd->getDataObjectID(), Interface::FITKGeoEnum::VSSolid, solidIndice);
        }

        // 清除拾取高亮。
        pickDataProvider->clearPickedData();
    }

    void FaceHiddenDialog::hideCurrentFaceGroup()
    {
        int geoGroupId = ui->cmbFaceGroup->currentData().toInt();

        // 获取全局几何分组管理器。
        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return;
        }

        Interface::FITKGlobalGeoComponentManager* globalGeoCompMgr = cmdList->getGlobalGeoCompManager();
        if (!globalGeoCompMgr)
        {
            return;
        }

        Interface::FITKGlobalGeoComponent* geoComp = globalGeoCompMgr->getDataByID(geoGroupId);
        if (!geoComp)
        {
            return;
        }

        // 面查询。
        Interface::FITKVirtualTopoMapper vMapperF;

        // 处理子分组。
        int nSubComps = geoComp->getDataCount();
        for (int i = 0; i < nSubComps; i++)
        {
            Interface::FITKGeoComponent* subComp = geoComp->getDataByIndex(i);
            if (!subComp)
            {
                continue;
            }

            // 获取分组所属模型。
            Interface::FITKAbsGeoShapeAgent* shapeAgent = subComp->getAbstractModelAs<Interface::FITKAbsGeoShapeAgent>();
            if (!shapeAgent)
            {
                continue;
            }

            Interface::FITKAbsGeoCommand* cmd = shapeAgent->getGeoCommand();
            if (!cmd)
            {
                continue;
            }

            int cmdId = cmd->getDataObjectID();
            Interface::FITKGeoEnum::VTopoShapeType vShapeType = Interface::FITKGeoEnum::VSNone;
            switch (subComp->getGeoType())
            {
            case Interface::FITKModelEnum::FMSPoint:    vShapeType = Interface::FITKGeoEnum::VSPoint;       break;
            case Interface::FITKModelEnum::FMSEdge:     vShapeType = Interface::FITKGeoEnum::VSEdge;        break;
            case Interface::FITKModelEnum::FMSSurface:  vShapeType = Interface::FITKGeoEnum::VSFace;        break;
            case Interface::FITKModelEnum::FMSSolid:    vShapeType = Interface::FITKGeoEnum::VSSolid;       break;
            case Interface::FITKModelEnum::FMSComb:     vShapeType = Interface::FITKGeoEnum::VSAssembly;    break;
            default:
                return;
            }

            // 获取虚拓扑管理器。
            Interface::FITKShapeVirtualTopoManager* vTopoMgr = cmd->getShapeVirtualTopoManager(vShapeType);
            Interface::FITKShapeVirtualTopoManager* vFaceMgr = cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VSFace);
            if (!vTopoMgr || !vFaceMgr)
            {
                return;
            }

            QList<int> indice;
            QList<int> menbers = subComp->getMember();
            for (const int & menber : menbers)
            {
                Interface::FITKAbsVirtualTopo* vTopo = vTopoMgr->getDataByIndex(menber);
                if (!vTopo)
                {
                    continue;
                }

                vMapperF.mapTopo(vTopo, Interface::FITKGeoEnum::VSFace);
                int nTopoFaces = vMapperF.length();
                for (int j = 0; j < nTopoFaces; j++)
                {
                    Interface::FITKAbsVirtualTopo* vFace = vMapperF.virtualTopo(j);
                    int faceIndex = vFaceMgr->getDataIndex(vFace);
                    if (faceIndex != -1)
                    {
                        indice.push_back(faceIndex);
                    }
                }
            }

            m_GraphOper->setShapeHidden(cmdId, Interface::FITKGeoEnum::VSFace, indice);
        }
    }

    void FaceHiddenDialog::finalize()
    {
        // 清除拾取状态
        if (m_GraphPickOper)
        {
            m_GraphPickOper->setCustomPickMode();
        }

        // 初始化面组列表下拉框。
        ui->cmbFaceGroup->clear();
    }

    void FaceHiddenDialog::refreshFaceGroupComboBox()
    {
        ui->cmbFaceGroup->clear();

        // 获取全局几何分组管理器。
        Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
        if (!cmdList)
        {
            return;
        }

        Interface::FITKGlobalGeoComponentManager* globalGeoCompMgr = cmdList->getGlobalGeoCompManager();
        if (!globalGeoCompMgr)
        {
            return;
        }

        int nGroups = globalGeoCompMgr->getDataCount();
        for (int i = 0; i < nGroups; i++)
        {
            Interface::FITKGlobalGeoComponent* geoComp = globalGeoCompMgr->getDataByIndex(i);
            if (!geoComp)
            {
                continue;
            }

            ui->cmbFaceGroup->addItem(geoComp->getDataObjectName(), geoComp->getDataObjectID());
        }
    }

    void FaceHiddenDialog::slot_rdBtnClicked()
    {
        bool byPick = ui->rdBtnPick->isChecked();
        ui->labelPick->setEnabled(byPick);
        ui->cmbShapeType->setEnabled(byPick);
        ui->labelFaceGroup->setEnabled(!byPick);
        ui->cmbFaceGroup->setEnabled(!byPick);
    }

    void FaceHiddenDialog::slot_pickShapeTypeCmbChanged(int index)
    {
        // 设置拾取模式。
        GUIPickInfoStru pickInfo;
        pickInfo._pickMethod = GUI::GUIPickInfo::PickMethod::PMIndividually;
        pickInfo._enableDatumPick = false;
        pickInfo._enableModelPick = true;
        pickInfo._enableDatumPick = false;
        pickInfo._enablePickCenter = false;

        switch (index)
        {
        case 0:
            pickInfo._pickObjType = GUIPickInfo::POBJFace;
            break;
        case 1:
            pickInfo._pickObjType = GUIPickInfo::POBJSolid;
            break;
        default:
            return;
        }

        GUIPickInfo::SetPickInfo(pickInfo);
    }

    void FaceHiddenDialog::slot_btnHide()
    {
        if (!m_GraphOper)
        {
            return;
        }

        bool byPick = ui->rdBtnPick->isChecked();

        // 拾取隐藏。
        if (byPick)
        {
            hidePickedShapes();
        }
        // 面组隐藏。
        else
        {
            hideCurrentFaceGroup();
        }
    }

    void FaceHiddenDialog::slot_btnShowAll()
    {
        if (!m_GraphOper)
        {
            return;
        }

        m_GraphOper->clearShapeFaceTransparency();
    }

    void FaceHiddenDialog::slot_btnOK()
    {
        // 关闭对话框。
        finalize();
        accept();
    }
}

