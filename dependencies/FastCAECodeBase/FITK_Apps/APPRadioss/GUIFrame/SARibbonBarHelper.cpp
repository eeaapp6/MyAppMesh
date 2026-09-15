/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "SARibbonBarHelper.h"
#include "MainWindow.h"
#include "FITK_Component/FITKWidget/FITKComboBox.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppSettings.h"
#include "OperatorsInterface/ComboboxEventOperator.h"

#include <SARibbonBar.h>
#include <SARibbonApplicationButton.h>
#include <SARibbonQuickAccessBar.h>
#include <SARibbonButtonGroupWidget.h>
#include <SARibbonTabBar.h>

#include <QObject>

namespace GUI
{
    SARibbonBarHelper::SARibbonBarHelper(MainWindow* mw)
        :_mainWindow(mw)
    {
        if (_mainWindow == nullptr) return;

        _ribbonBar = _mainWindow->ribbonBar();
        if (_ribbonBar == nullptr) return;
        //使用Office2013风格
        sa_set_ribbon_theme(_ribbonBar, SARibbonTheme::RibbonThemeOffice2013);

        //设置顶部线条颜色
        _ribbonBar->setTabBarBaseLineColor(QColor(186, 201, 219));
        _ribbonBar->setRibbonStyle(SARibbonBar::RibbonStyleLooseThreeRow);
        _ribbonBar->setWindowTitleTextColor(QColor(0, 0, 0));
    }

    QAction* SARibbonBarHelper::CreateAction(const QString& text, const QString & name, const QString& icon, QWidget* p)
    {
        QAction* ac = new QAction(p);
        ac->setText(text);
        ac->setObjectName(name);
        ac->setIcon(QIcon(icon));
        return ac;
    }

    void SARibbonBarHelper::initRibbonBar()
    {
        if (_ribbonBar == nullptr) return;
        //开始按钮
        this->initRibbonBarStartButton();
        //不同的Panel页面
        this->initRibbonBarFile();
        this->initRibbonBarGeometry();
        this->initRibbonBarMesh();
        this->initRibbonBarProp();
        this->initRibbonBarAnalysis();
        this->initRibbonBarSolver();
        this->initRibbonBarPost();
        this->initRibbonBarTools();
        this->initRibbonBarHelp();

       // SARibbonButtonGroupWidget* buttonGroup = _ribbonBar->rightButtonGroup();
       SARibbonQuickAccessBar* quickAccessBar = _ribbonBar->quickAccessBar();
        if (quickAccessBar)
        {
            QAction* action = _mainWindow->findChild<QAction*>("actionAIAgent");
            quickAccessBar->addAction(action, Qt::ToolButtonIconOnly, QToolButton::DelayedPopup);
        }

        //设置当前页为File页面
        SARibbonCategory* c = _ribbonBar->categoryByIndex(0);
        if (c) _ribbonBar->showCategory(c);

        this->comboBoxConnectOper();
    }

    void SARibbonBarHelper::initRibbonBarStartButton()
    {
        if (_ribbonBar == nullptr) return;
        //开始按钮
        QAbstractButton* startButton = _ribbonBar->applicationButton();
        startButton->setText(QObject::tr("Start"));
        startButton->setFixedWidth(60);
        //工作路径
        QAction* act = CreateAction(QObject::tr("Working Dir"), "actionWorkingDir", "", _mainWindow);
        startButton->addAction(act);

    }

    void SARibbonBarHelper::initRibbonBarFile()
    {
        if (_ribbonBar == nullptr) return;

        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("File"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;
        //项目
        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Project"));
        action = CreateAction(QObject::tr("New"), "actionNew", ":/icons/new.svg", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Open"), "actionOpen", ":/icons/open.svg", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);     
        action = CreateAction(QObject::tr("Save"), "actionSave", ":/icons/save.svg", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Save As"), "actionSaveAs", ":/icons/saveAs.svg", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Import Geometry"), "actionImportGeometry", ":/toolBar/importGeometry.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Import Mesh"), "actionImportMesh", ":/toolBar/ImportMesh.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
 //       action->setVisible(false);
        action = CreateAction(QObject::tr("Import rad"), "actionImportRad", ":/toolBar/importRadioss.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Export rad"), "actionExportRad", ":/toolBar/exportRadioss.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Import LS-Dyna"), "actionImportLSDyna", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        //action->setVisible(tr);
        action = CreateAction(QObject::tr("Export LS-Dyna"), "actionExportLSDyna", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        //action->setVisible(false);
        action = CreateAction(QObject::tr("Import fitkmesh"), "actionImportFITKMesh", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Export fitkmesh"), "actionExportFITKMesh", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Export INP"), "actionExportAsModel", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        //获取配置状态信息
        AppFrame::FITKAppSettings* appSettings = FITKAPP->getAppSettings();
        if (appSettings)
        {
            pannel = gategory->addPannel(QObject::tr("Visibility"));

            QVariant isEnable;
            if (appSettings) isEnable = appSettings->getVarient("GraphSetting/GeoPointVisible");
            if (!isEnable.isValid()) isEnable = true;
            action = CreateAction(QObject::tr("Point Visible"), "actionPointVisible", isEnable.toBool() ? ":/toolBar/visibleGeoPoint.png" : ":/toolBar/no_visibleGeoPoint.png", _mainWindow);
            pannel->addAction(action, SARibbonPannelItem::Small);

            isEnable.clear();
            if (appSettings) isEnable = appSettings->getVarient("GraphSetting/GeoLineVisible");
            if (!isEnable.isValid()) isEnable = true;
            action = CreateAction(QObject::tr("Curve Visible"), "actionLineVisible", isEnable.toBool() ? ":/toolBar/visibleGeoLine.png" : ":/toolBar/no_visibleGeoLine.png", _mainWindow);
            pannel->addAction(action, SARibbonPannelItem::Small);

            isEnable.clear();
            if (appSettings) isEnable = appSettings->getVarient("GraphSetting/GeoVisible");
            if (!isEnable.isValid()) isEnable = true;
            action = CreateAction(QObject::tr("Surface Visible"), "actionGeoVisible", isEnable.toBool() ? ":/toolBar/visibleGeo.png" : ":/toolBar/no_visibleGeo.png", _mainWindow);
            pannel->addAction(action, SARibbonPannelItem::Small);

            isEnable.clear();
            if (appSettings) isEnable = appSettings->getVarient("GraphSetting/MeshPointVisible");
            if (!isEnable.isValid()) isEnable = true;
            action = CreateAction(QObject::tr("Node Visible"), "actionMeshPointVisible", isEnable.toBool() ? ":/toolBar/visibleMeshPoint.png" : ":/toolBar/no_visibleMeshPoint.png", _mainWindow);
            pannel->addAction(action, SARibbonPannelItem::Small);

            isEnable.clear();
            if (appSettings) isEnable = appSettings->getVarient("GraphSetting/MeshLineVisible");
            if (!isEnable.isValid()) isEnable = true;
            action = CreateAction(QObject::tr("Edge Visible"), "actionMeshLineVisible", isEnable.toBool() ? ":/toolBar/visibleMeshLine.png" : ":/toolBar/no_visibleMeshLine.png", _mainWindow);
            pannel->addAction(action, SARibbonPannelItem::Small);

            isEnable.clear();
            if (appSettings) isEnable = appSettings->getVarient("GraphSetting/PartVisible");
            if (!isEnable.isValid()) isEnable = true;
            action = CreateAction(QObject::tr("Face Visible"), "actionPartVisible", isEnable.toBool() ? ":/toolBar/visiblePart.png" : ":/toolBar/no_visiblePart.png", _mainWindow);
            pannel->addAction(action, SARibbonPannelItem::Small);
        }

        pannel = gategory->addPannel(QObject::tr("Graph"));
        action = CreateAction(QObject::tr("Graph Setting"), "actionGraphSetting", ":/toolBar/Graph_Setting.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

    }

    void SARibbonBarHelper::initRibbonBarMesh()
    {
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Mesh"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;
        //分析
        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Gmsh"));
        action = CreateAction(QObject::tr("Mesh"), "actionGmshSettings", ":/toolBar/MeshGen.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        //action = CreateAction(QObject::tr("Region Box"), "actionRegionMeshSizeBox", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);
        //action = CreateAction(QObject::tr("Region Cylinder"), "actionRegionMeshSizeCylinder", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);
        //action = CreateAction(QObject::tr("Region Sphere"), "actionRegionMeshSizeSphere", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);
    }

    void SARibbonBarHelper::initRibbonBarTools()
    {
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Tools"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;
        //拾取
        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Pick Set"));
        action = CreateAction(QObject::tr("Create Set-Nodes"), "actionSetNodesCreate", ":/toolBar/Create_Set_Nodes.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Create Set-Elements"), "actionSetElementCreate", ":/toolBar/Create_Set_Elements.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        pannel = gategory->addPannel(QObject::tr("Pick Surface"));
        action = CreateAction(QObject::tr("Create Surface-Nodes"), "actionSurfNodeCreate", ":/toolBar/Create_Surface_Nodes.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Create Surface-Element"), "actionSurfElementCreate", ":/toolBar/Create_Surface_Element.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        //脚本
        pannel = gategory->addPannel(QObject::tr("Script"));
        action = CreateAction(QObject::tr("Run Script"), "actionRunScript", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Record Script"), "actionRecordScript", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("End Script"), "actionEndScript", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Clear Script"), "actionClearVariable", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        //AI
        pannel = gategory->addPannel(QObject::tr("AI"));
        action = CreateAction(QObject::tr("Show/Hide Agent"), "actionAIAgent", "", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
    }

    void SARibbonBarHelper::initRibbonBarSolver()
    {
        //求解工具栏
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Solve"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;

        //求解
        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Solve"));
        action = CreateAction(QObject::tr("Solver Setting"), "actionSolverSettings", ":/toolBar/solverSetting.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Result Request"), "actionResultRequest", ":/toolBar/resultRequestSetting.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Start Solver"), "actionStartSolver", ":/toolBar/startSolver.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        //pannel = gategory->addPannel(QObject::tr("Post"));
        //action = CreateAction(QObject::tr("Viewer"), "actionSolverViewer", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large); 

        //pannel = gategory->addPannel(QObject::tr("Animation"));
        //action = CreateAction(QObject::tr("First Frame"), "actionFirstFrame", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);
        //action = CreateAction(QObject::tr("Previous Frame"), "actionPreviousFrame", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);
        //action = CreateAction(QObject::tr("Start Playing"), "actionStartPlaying", "", _mainWindow);
        //action->setData(0);//1 运行 0 停止
        //pannel->addAction(action, SARibbonPannelItem::Large);
        //action = CreateAction(QObject::tr("Next Frame"), "actionNextFrame", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);
        //action = CreateAction(QObject::tr("Last Frame"), "actionLastFrame", "", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);

        //FITKComboBox* comboBoxField = new FITKComboBox(_mainWindow);//变量位置
        //comboBoxField->setObjectName("comboBox_PostVariableLocation");
        //comboBoxField->setMinimumWidth(90);
        //comboBoxField->setSizeAdjustPolicy(FITKComboBox::AdjustToContents);
        //pannel->addSmallWidget(comboBoxField);
        //comboBoxField = new FITKComboBox(_mainWindow);//场变量
        //comboBoxField->setObjectName("comboBox_PostFieldVariable");
        //comboBoxField->setMinimumWidth(90);
        //comboBoxField->setSizeAdjustPolicy(FITKComboBox::AdjustToContents);
        //pannel->addSmallWidget(comboBoxField);
        //comboBoxField = new FITKComboBox(_mainWindow);//场分量
        //comboBoxField->setObjectName("comboBox_PostFieldWeight");
        //comboBoxField->setMinimumWidth(90);
        //comboBoxField->setSizeAdjustPolicy(FITKComboBox::AdjustToContents);
        //pannel->addSmallWidget(comboBoxField);

    }
    void SARibbonBarHelper::initRibbonBarPost()
    {
        //求解工具栏
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Post"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;

        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Import"));
        action = CreateAction(QObject::tr("Import 3D Result"), "actionImportVTKResult", ":/toolBar/import3DResult.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Import 2D Result"), "actionImportCSVResult", ":/toolBar/Import_2D_Result.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        pannel = gategory->addPannel(QObject::tr("3D"));
        action = CreateAction(QObject::tr("Spectrogram Settings"), "actionColorBar", ":/toolBar/Spectrogram_Settings.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        pannel = gategory->addPannel(QObject::tr("Animation"));
        action = CreateAction(QObject::tr("First Frame"), "actionFirstFrame", ":/toolBar/firstFrame.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Previous Frame"), "actionPreviousFrame", ":/toolBar/previousFrams.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Start Playing"), "actionStartPlaying", ":/toolBar/startPlaying.png", _mainWindow);
        action->setData(0);//1 运行 0 停止
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Next Frame"), "actionNextFrame", ":/toolBar/nextFrame.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Last Frame"), "actionLastFrame", ":/toolBar/lastFrame.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);


        FITKComboBox* comboBoxField = new FITKComboBox(_mainWindow);//变量位置
        comboBoxField->setObjectName("comboBox_PostVariableLocation");
        comboBoxField->setMinimumWidth(90);
        comboBoxField->setSizeAdjustPolicy(FITKComboBox::AdjustToContents);
        pannel->addSmallWidget(comboBoxField);
        comboBoxField = new FITKComboBox(_mainWindow);//场变量
        comboBoxField->setObjectName("comboBox_PostFieldVariable");
        comboBoxField->setMinimumWidth(90);
        comboBoxField->setSizeAdjustPolicy(FITKComboBox::AdjustToContents);
        pannel->addSmallWidget(comboBoxField);
        comboBoxField = new FITKComboBox(_mainWindow);//场分量
        comboBoxField->setObjectName("comboBox_PostFieldWeight");
        comboBoxField->setMinimumWidth(90);
        comboBoxField->setSizeAdjustPolicy(FITKComboBox::AdjustToContents);
        pannel->addSmallWidget(comboBoxField);

    }
    void SARibbonBarHelper::initRibbonBarProp()
    {
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Property"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;

        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Property"));
        action = CreateAction(QObject::tr("Material"), "actionMaterialPropCreate", ":/toolBar/createMaterial.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Property"), "actionPropertyCreate", ":/toolBar/createProperty.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("EOS"), "actionEOSCreate", ":/toolBar/EOS.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Failure Model"), "actionFailureModelCreate", ":/toolBar/Failure_Model.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        pannel = gategory->addPannel(QObject::tr("Curve"));
        action = CreateAction(QObject::tr("Create Curve"), "actionCurveCreate", ":/toolBar/createCure.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
    }

    void SARibbonBarHelper::initRibbonBarHelp()
    {
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Help"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;

        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Help"));

        action = CreateAction(QObject::tr("License"), "actionLicense", ":/toolBar/License.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
    }

    void SARibbonBarHelper::initRibbonBarGeometry()
    {
        //求解工具栏
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Geometry"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;

        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Model"));
        action = CreateAction(QObject::tr("Box"), "actionCreateBox", ":/toolBar/createBox.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Cylinder"), "actionCreateCylinder", ":/toolBar/createCylinder.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Sphere"), "actionCreateSphere", ":/toolBar/createSphere.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        pannel = gategory->addPannel(QObject::tr("Bool"));
        action = CreateAction(QObject::tr("Bool"), "actionCreateBool", ":/toolBar/GeoBool.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        pannel = gategory->addPannel(QObject::tr("Angle Transformation"));
        action = CreateAction(QObject::tr("Chamfer"), "actionCreateChamfer", ":/toolBar/createGeoChamfer.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Fillet"), "actionCreateFillet", ":/toolBar/createGeoFillet.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Defeature"), "actionCreateDefeature", ":/toolBar/GeoDefeature.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);

        pannel = gategory->addPannel(QObject::tr("Transformation"));
        action = CreateAction(QObject::tr("Trans"), "actionCreateTrans", ":/toolBar/Translation.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Rotate"), "actionCreateRotate", ":/toolBar/Rotate.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Scale"), "actionCreateScale", ":/toolBar/Scale.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Mirror"), "actionCreateMirror", ":/toolBar/Mirror.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Rectangular Pattern"), "actionCreateRectangularPattern", ":/toolBar/RectangularPattern.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Circular Pattern"), "actionCreateCircularPattern", ":/toolBar/CircularPattern.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
    }

    void SARibbonBarHelper::comboBoxConnectOper()
    {
        //设置
        auto operFieldOutput = FITKOPERREPO->getOperatorT<EventOper::ComboboxEventOperator>("OperFieldOutput");
        if (operFieldOutput)
        {
            operFieldOutput->setParentWidget(_mainWindow);
            operFieldOutput->setMainWindow(_mainWindow);
        }
    }

    void SARibbonBarHelper::setPushButtonStyle(QAction* action)
    {
        QWidgetList qw = action->associatedWidgets();
        for (auto w : qw)
        {
            auto* btn = qobject_cast<SARibbonToolButton*>(w);
            if (btn == nullptr) continue;
            btn->setStyleSheet(
                "SARibbonToolButton {"
                "   background-color: #ADD8E6;"
                "}"
            );
        }
    }

    void SARibbonBarHelper::initRibbonBarAnalysis()
    {
        if (_ribbonBar == nullptr) return;
        SARibbonCategory* gategory = _ribbonBar->addCategoryPage(QObject::tr("Analysis"));
        _ribbonBar->raiseCategory(gategory);
        QAction* action = nullptr;
        //Solution
        SARibbonPannel* pannel = gategory->addPannel(QObject::tr("Solution"));
        action = CreateAction(QObject::tr("Create Solution"), "actionSolutionCreate", ":/toolBar/createSolution.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        //刚性壁
        pannel = gategory->addPannel(QObject::tr("Rigid Wall"));
        action = CreateAction(QObject::tr("Create Rigid Wall"), "actionRigidWallCreate", ":/toolBar/createRigidWall.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        //重力
        pannel = gategory->addPannel(QObject::tr("Gravity"));
        action = CreateAction(QObject::tr("Create Gravity"), "actionGravityCreate", ":/toolBar/createGravity.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        //连接
        pannel = gategory->addPannel(QObject::tr("Contact"));
        action = CreateAction(QObject::tr("Contact"), "actionContactCreate", ":/toolBar/Tie.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        //action = CreateAction(QObject::tr("Multi-Usage Impact"), "actionContactMultiUsageImpactCreate", ":/toolBar/Multi_Usage_Impact.png", _mainWindow);
        //pannel->addAction(action, SARibbonPannelItem::Large);
        //连接
        pannel = gategory->addPannel(QObject::tr("Connection"));
        action = CreateAction(QObject::tr("Rigid Body"), "actionRBodyCreate", ":/toolBar/Rigid_Body.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        //边界条件
        pannel = gategory->addPannel(QObject::tr("Boundary Conditions"));
        action = CreateAction(QObject::tr("BCS"), "actionBCSCreate", ":/toolBar/BCS.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        // 初始化场
        pannel = gategory->addPannel(QObject::tr("Initial Field"));
        action = CreateAction(QObject::tr("Create Initial Field"), "actionInitialFieldCreate", ":/toolBar/createInitialField.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        // 创建加速度计
        pannel = gategory->addPannel(QObject::tr("Probe"));
        action = CreateAction(QObject::tr("Create Accelerometer"), "actionCreateAcceleration", ":/toolBar/Create_Accelerometer.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);
        action = CreateAction(QObject::tr("Create Section"), "actionCreateSection", ":/toolBar/Create_Section.png", _mainWindow);
        pannel->addAction(action, SARibbonPannelItem::Large);


    }

    SARibbonCategory * SARibbonBarHelper::addPage(const QString & pageText)
    {
        if (_ribbonBar == nullptr) return nullptr;
        SARibbonCategory* category = _ribbonBar->addCategoryPage(pageText);
        _ribbonBar->raiseCategory(category);
        _ribbonBar->setCurrentIndex(0);
        return category;
    }

    SARibbonPannel * SARibbonBarHelper::addPanel(SARibbonCategory * page, const QString & text)
    {
        if (page == nullptr) return nullptr;

        SARibbonPannel* pannel = page->addPannel(text);
        return pannel;
    }

    void SARibbonBarHelper::panelAddAction(SARibbonPannel * pannel, QAction * action, SARibbonPannelItem::RowProportion actionType)
    {
        //pannel中添加action
        if (pannel == nullptr || action == nullptr)return;
        pannel->addAction(action, actionType);
    }

    void SARibbonBarHelper::removePage(SARibbonCategory * page)
    {
        if (_ribbonBar == nullptr || page == nullptr) return;
        _ribbonBar->removeCategory(page);
    }

}




