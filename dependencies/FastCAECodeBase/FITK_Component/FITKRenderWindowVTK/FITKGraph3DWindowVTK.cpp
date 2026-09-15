/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraph3DWindowVTK.h"
#include "ui_FITKGraph3DWindowVTK.h"
#include "FITKGraphRender.h"
#include "FITKBoundaryActor.h"
#include "FITKGraphObjectVTK.h"
#include "FITKGraphOperator.h"
#include "FITKGraphInteractionStyle.h"
#include "FITKLegendScaleActor.h"
#include "FITKGraph3DClipDialog.h"
#include "FITKGraphAnnotation.h"
#include "FITKGraphObjectAnnotation.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKSystemInfo.h"

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkTIFFWriter.h>
#include <vtkBMPWriter.h>
#include <vtkLightCollection.h>
#include <vtkLight.h>
#include <vtkVersionMacros.h>
#include <vtkMapper.h>
#include <vtkExtractGeometry.h>
#include <vtkPlane.h>
#include <vtkDataSetMapper.h>
#include <vtkProperty.h>
#include <vtkAppendFilter.h>
#include <vtkRendererCollection.h>
#include <vtkInteractorObserver.h>

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

#if VTK_MAJOR_VERSION < 8
#include <QVTKWidget2.h>
#include <vtkGenericOpenGLRenderWindow.h>
#else
#include <QVTKOpenGLNativeWidget.h>
#endif

#include <QVBoxLayout>
#include <QDebug>
#include <QToolBar>
#include <QToolButton>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QMutexLocker>
#include <QFileDialog>
#include <QToolButton>
#include <QDebug>
#include <QFileInfo>
#include <QtSvg/QSvgGenerator>
#include <QPainter>

namespace Comp
{
    QMutex FITKGraph3DWindowVTK::_mutex;

    vtkAnnotationCallback::vtkAnnotationCallback(FITKGraph3DWindowVTK* graphWindow)
        : m_graphWindow(graphWindow)
    {

    }

    void vtkAnnotationCallback::Execute(vtkObject* caller, unsigned long eventId, void* callData)
    {
        if (!m_graphWindow)
        {
            return;
        }

        int nUpdatedAnnos = 0;

        FITKGraphObjectAnnotationManager* annoGraphMgr = m_graphWindow->getAnnotationGraphManager();
        if (annoGraphMgr)
        {
            int nAnnos = annoGraphMgr->getNumberOfObjects();

            // Update the graph object by event.
            if (eventId == vtkCommand::MouseMoveEvent)
            {
                for (int i = 0; i < nAnnos; i++)
                {
                    FITKGraphObjectAnnotation* gObj = annoGraphMgr->getObjectAt(i);
                    if (gObj && gObj->isModifying())
                    {
                        gObj->updatePositions();
                        nUpdatedAnnos++;
                    }
                }
            }
            else
            {
                for (int i = 0; i < nAnnos; i++)
                {
                    FITKGraphObjectAnnotation* gObj = annoGraphMgr->getObjectAt(i);
                    if (gObj && gObj->getVisibility())
                    {
                        gObj->updatePositions();
                        nUpdatedAnnos++;
                    }
                }
            }
        }
        
        // Update the temp graph object.
        FITKGraphObjectAnnotation* gObj = m_graphWindow->getTempAnnotationGraph();
        if (eventId == vtkCommand::MouseMoveEvent)
        {
            FITKGraphObjectAnnotation* gObj = m_graphWindow->getTempAnnotationGraph();
            if (gObj && gObj->isModifying())
            {
                gObj->updatePositions();
                nUpdatedAnnos++;
            }
        }
        else
        {
            if (gObj && gObj->getVisibility())
            {
                gObj->updatePositions();
                nUpdatedAnnos++;
            }
        }

        // Render.
        if (nUpdatedAnnos != 0)
        {
            m_graphWindow->reRender();
        } 
    }


    FITKGraph3DWindowVTK::FITKGraph3DWindowVTK(Graph3DWindowInitializer* ini)
        :_initializer(ini)
    {
        if (_initializer == nullptr) return;

        m_ui = new Ui::FITKGraph3DWindowVTK();
        m_ui->setupUi(this);

#if VTK_MAJOR_VERSION >= 9
        QVTKOpenGLNativeWidget* vtkWidget = new QVTKOpenGLNativeWidget;
        m_renderWindow = vtkWidget->renderWindow();
        m_interactor = (QVTKInteractor*)m_renderWindow->GetInteractor();
        m_vtkWidget = vtkWidget;
#elif VTK_MAJOR_VERSION == 8
        QVTKOpenGLNativeWidget* vtkWidget = new QVTKOpenGLNativeWidget;
        m_renderWindow = vtkWidget->GetRenderWindow();
        m_interactor = (QVTKInteractor*)m_renderWindow->GetInteractor();
        m_vtkWidget = vtkWidget;
#else
        QVTKWidget2* vtkWidget = new QVTKWidget2;
        m_renderWindow = m_vtkWidget->GetRenderWindow();
        m_interactor = m_renderWindow->GetInteractor();
        m_vtkWidget = vtkWidget;
#endif

        m_renderWindow->SetAlphaBitPlanes(1);

        //多采样设置
#if VTK_MAJOR_VERSION > 9
        m_renderWindow->SetMultiSamples(10);
#elif VTK_MAJOR_VERSION == 9
#if VTK_MINOR_VERSION >0
        m_renderWindow->SetMultiSamples(10);
#else
        m_renderWindow->SetMultiSamples(0);
#endif
#else
        m_renderWindow->SetMultiSamples(0);
#endif

        QLayout* lay = layout();
        if (!lay)
        {
            setLayout(new QVBoxLayout);
            lay = layout();
        }

        lay->addWidget(m_vtkWidget);

        //相机创建
        m_camera = vtkCamera::New();

        // 初始化三维窗口渲染频率。（2025/01/21体渲染支持）
        m_interactor->SetDesiredUpdateRate(15);
        m_interactor->SetStillUpdateRate(0.5);

        m_actorBound = new FITKBoundaryActor;

        int renderNum = _initializer->getLayerCount();

        //设置渲染图层数量
        m_renderWindow->SetNumberOfLayers(renderNum);

        //动态创建图层
        for (int i = 0; i < renderNum; i++)
        {
            FITKGraphRender* renderer = new FITKGraphRender(this);
            vtkRenderer* rendervtk = renderer->getRenderer();
            if (rendervtk) rendervtk->SetLayer(i);
            m_renders.insert(i, renderer);
        }

        //多Render协同同步，统一设置camera
        for (auto render : m_renders.values())
        {
            if (render == nullptr)continue;
            render->getRenderer()->SetActiveCamera(m_camera);
        }

        this->init();
        setFocusPolicy(Qt::ClickFocus);

        //设置名称
        this->setWindowTitle(QString("View Port-%1").arg(this->getGraphWidgetID()));

        //渲染窗口工具栏初始化
        _renderToolBarLayout = new QGridLayout(m_vtkWidget);
        _renderToolBarLayout->setContentsMargins(0, 0, 0, 0);
        _renderToolBar = new QToolBar(m_vtkWidget);
        _renderToolBar->setContentsMargins(0, 0, 0, 0);
        _renderToolBar->setIconSize(QSize(40, 40));
        _renderIconToolBar = new QToolBar(m_vtkWidget);
        _renderIconToolBar->setContentsMargins(0, 0, 0, 0);
        //初始化action
        initActions();
        //默认不显示action
        setIsShowActions(false);

        // 初始化定时器。
        m_timerView.setInterval(m_interval);
        m_timerView.setSingleShot(true);
        connect(&m_timerView, &QTimer::timeout, this, &FITKGraph3DWindowVTK::slot_changingView);

        // 创建标识管理器。
        m_annoMgr = new FITKGraphAnnotationManager;
        m_annoGraphMgr = new FITKGraphObjectAnnotationManager;

        // 初始化临时标识对象。
        createTempAnnotation();

        // 创建并绑定标识回调。
        m_annoCb = new vtkAnnotationCallback(this);
        if (m_interactor)
        {
            unsigned long tag = 0;
            tag = m_interactor->AddObserver(vtkCommand::InteractionEvent, m_annoCb, &vtkAnnotationCallback::Execute);
            if (!m_cbTags.contains(tag))
            {
                m_cbTags.push_back(tag);
            }
            
            tag = m_interactor->AddObserver(vtkCommand::WindowResizeEvent, m_annoCb, &vtkAnnotationCallback::Execute);
            if (!m_cbTags.contains(tag))
            {
                m_cbTags.push_back(tag);
            }

            tag = m_interactor->AddObserver(vtkCommand::MouseMoveEvent, m_annoCb, &vtkAnnotationCallback::Execute);
            if (!m_cbTags.contains(tag))
            {
                m_cbTags.push_back(tag);
            }

            tag = m_interactor->AddObserver(vtkCommand::MouseWheelBackwardEvent, m_annoCb, &vtkAnnotationCallback::Execute);
            if (!m_cbTags.contains(tag))
            {
                m_cbTags.push_back(tag);
            }

            tag = m_interactor->AddObserver(vtkCommand::MouseWheelForwardEvent, m_annoCb, &vtkAnnotationCallback::Execute);
            if (!m_cbTags.contains(tag))
            {
                m_cbTags.push_back(tag);
            }
        }
    }

    void FITKGraph3DWindowVTK::showEvent(QShowEvent* event)
    {
        if (!m_initFlag)
        {
            m_initFlag = true;

            // 设置相机视角 Added by ChengHaotian
//             double position[3]{ 1,1,1 };
//             double focalPoint[3]{ 0,0,0 };
//             double viewUp[3]{ 0,1,0 };
            this->setView(_defFp, _defVp, _defPos);

            // 初始化坐标轴 Added by ChengHaotian
            if (m_axesWidget)
            {
                m_axesWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
                m_axesWidget->SetViewport(0, 0, 0.2, 0.2);
                m_axesWidget->SetEnabled(true);
                m_axesWidget->InteractiveOff();
            }
        }

        QWidget::showEvent(event);

        /*vtkRendererCollection* renderers = m_renderWindow->GetRenderers();
        int nRenderers = renderers->GetNumberOfItems();
        for (int i = nRenderers - 1; i >= 0; i--)
        {
            vtkRenderer* ren = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(i));
            if (!ren)
            {
                continue;
            }

            bool hasRender = false;
            for (FITKGraphRender* render : m_renders.values())
            {
                if (!render)
                {
                    continue;
                }

                if (ren == render->getRenderer())
                {
                    hasRender = true;
                    break;
                }
            }

            if (!hasRender)
            {
                m_renderWindow->RemoveRenderer(ren);
                m_renderWindow->SetNumberOfLayers(m_renderWindow->GetNumberOfLayers() - 1);
            }
        }*/
    }

    void FITKGraph3DWindowVTK::updateAnnotations()
    {
        if (m_annoGraphMgr)
        {
            int nAnnos = m_annoGraphMgr->getNumberOfObjects();
            for (int i = 0; i < nAnnos; i++)
            {
                FITKGraphObjectAnnotation* gObj = m_annoGraphMgr->getObjectAt(i);
                if (gObj && gObj->getVisibility())
                {
                    gObj->updatePositions();
                }
            }

            // Update the temp graph object.
            if (m_tempAnnoGraph && m_tempAnnoGraph->getVisibility())
            {
                m_tempAnnoGraph->updatePositions();
            }
        }
    }

    FITKGraph3DWindowVTK::~FITKGraph3DWindowVTK()
    {
        // 析构 Added by ChengHaotian
        if (m_legendScaleActor)
        {
            m_legendScaleActor->Delete();
            m_legendScaleActor = nullptr;
        }

        if (m_ui != nullptr) delete m_ui; m_ui = nullptr;
        if (m_camera != nullptr)m_camera->Delete(); m_camera = nullptr;
        if (m_axesWidget != nullptr)m_axesWidget->Delete(); m_axesWidget = nullptr;
        if (m_interactionStyle != nullptr)m_interactionStyle->Delete(); m_interactionStyle = nullptr;
        if (m_actorBound != nullptr) delete m_actorBound;

        // 析构回调。
        if (m_annoCb)
        {
            if (m_renderWindow)
            {
                vtkRenderWindowInteractor* interactor = m_renderWindow->GetInteractor();
                if (interactor)
                {
                    for (const unsigned long& tag : m_cbTags)
                    {
                        interactor->RemoveObserver(tag);
                    }
                }
            }

            delete m_annoCb;
            m_annoCb = nullptr;
        }

        // 析构临时标识对象。
        if (m_tempAnno)
        {
            delete m_tempAnno;
            m_tempAnno = nullptr;
        }

        // 析构标识管理器。
        if (m_annoMgr)
        {
            delete m_annoMgr;
            m_annoMgr = nullptr;
        }

        if (m_annoGraphMgr)
        {
            delete m_annoGraphMgr;
            m_annoGraphMgr = nullptr;
        }

        //渲染图层链表释放
        for (auto re : m_renders.values())
        {
            if (re == nullptr)continue;
            delete re;
            re = nullptr;
        }
        m_renders.clear();

#if VTK_MAJOR_VERSION <= 7
        if (m_vtkWidget)
        {
            // For VTK 7 Added by ChengHaotian
            m_vtkWidget->setParent(nullptr);
            m_vtkWidget->deleteLater();
            m_vtkWidget = nullptr;
        }
#endif
    }

    FITKGraphAnnotationManager* FITKGraph3DWindowVTK::getAnnotationManager()
    {
        return m_annoMgr;
    }

    FITKGraphObjectAnnotationManager* FITKGraph3DWindowVTK::getAnnotationGraphManager()
    {
        return m_annoGraphMgr;
    }

    FITKGraphAnnotation* FITKGraph3DWindowVTK::getTempAnnotation()
    {
        if (!m_tempAnno)
        {
            createTempAnnotation();
        }

        return m_tempAnno;
    }

    FITKGraphObjectAnnotation* FITKGraph3DWindowVTK::getTempAnnotationGraph()
    {
        return m_tempAnnoGraph;
    }

    FITKGraphAnnotation* FITKGraph3DWindowVTK::createAnnotation(QString name)
    {
        int iRender = getRenderCount() - 1;
        FITKGraphRender* render = getRenderer(iRender);
        if (!render)
        {
            return nullptr;
        }

        // Create the annotation data.
        FITKGraphAnnotation* anno = new FITKGraphAnnotation(this);

        if (name.isEmpty())
        {
            name = "Annotation-1";
        }

        anno->setDataObjectName(m_annoMgr->checkName(name));
        m_annoMgr->appendDataObj(anno);

        // Create the annotation graph object at the same time.
        FITKGraphObjectAnnotation* annoGObj = FITKGraphObjectAnnotation::New(anno);
        annoGObj->initializeObject();
        m_annoGraphMgr->appendObject(annoGObj);

        // Connect the signals.
        connect(anno, &FITKGraphAnnotation::sig_dataUpdated, annoGObj, &FITKGraphObjectAnnotation::update);
        connect(anno, &FITKGraphAnnotation::sig_modifyStart, annoGObj, &FITKGraphObjectAnnotation::modifyStartAnchor);
        connect(anno, &FITKGraphAnnotation::sig_modifyEnd, annoGObj, &FITKGraphObjectAnnotation::modifyEndAnchor);
        connect(anno, &FITKGraphAnnotation::sig_stopModifying, annoGObj, &FITKGraphObjectAnnotation::endModifyingAnchor);
        connect(anno, &FITKGraphAnnotation::dataObjectDestoried, this, [&](Core::FITKAbstractDataObject* dataDel)
        {
            if (!dataDel)
            {
                return;
            }

            int dataId = dataDel->getDataObjectID();
            m_annoGraphMgr->removeObject(m_annoGraphMgr->getObjectByAnnoID(dataId), true);
        });

        // Add to the render window.
        fastAddObject(iRender, annoGObj);

        return anno;
    }

    bool FITKGraph3DWindowVTK::getDisplayPosByWorldPos(double* display2, double* world3)
    {
        // Get the default renderer.
        vtkRenderer* renderer = getVTKRenderer(0);
        if (!renderer)
        {
            return false;
        }

        double p3[3]{ 0., 0., 0. };
        vtkInteractorObserver::ComputeWorldToDisplay(
            renderer, world3[0], world3[1], world3[2], p3);

        display2[0] = p3[0];
        display2[1] = p3[1];

        return true;
    }

    bool FITKGraph3DWindowVTK::getWorldPosByDisplayPos(double* world3, double* display2)
    {
        // Get the default renderer.
        vtkRenderer* renderer = getVTKRenderer(0);
        if (!renderer)
        {
            return false;
        }

        double p4[4]{ 0., 0., 0., 0. };
        vtkInteractorObserver::ComputeDisplayToWorld(
            renderer, display2[0], display2[1], 0, p4);

        world3[0] = p4[0];
        world3[1] = p4[1];
        world3[2] = p4[2];

        return true;
    }

    void FITKGraph3DWindowVTK::setHiddenLineRemoval(bool flag)
    {
        vtkRendererCollection* renderers = m_renderWindow->GetRenderers();
        int nRenderers = renderers->GetNumberOfItems();
        for (int i = nRenderers - 1; i >= 0; i--)
        {
            vtkRenderer* ren = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(i));
            if (!ren)
            {
                continue;
            }

            ren->SetUseHiddenLineRemoval(flag);
        }
    }

    bool FITKGraph3DWindowVTK::getHiddenLineRemoval()
    {
        vtkRendererCollection* renderers = m_renderWindow->GetRenderers();
        int nRenderers = renderers->GetNumberOfItems();
        if (nRenderers == 0)
        {
            return false;
        }

        vtkRenderer* ren = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(0));
        if (!ren)
        {
            return false;
        }

        return ren->GetUseHiddenLineRemoval();
    }

    void FITKGraph3DWindowVTK::reRender()
    {
        m_renderWindow->Render();
    }

    void FITKGraph3DWindowVTK::setAxesEnable(bool state)
    {
        m_axesWidget->SetEnabled(state);
    }

    void FITKGraph3DWindowVTK::setView(double* focalPoint, double* viewUp, double* position)
    {
        // 异常处理。
        for (int i = 0; i < 3; i++)
        {
            if (qIsNaN(focalPoint[i]) || qIsNaN(viewUp[i]) || qIsNaN(position[i]))
            {
                return;
            }
        }

        //设置VTK中相机的视角
        if (m_camera == nullptr) return;
        m_camera->SetViewUp(viewUp);
        m_camera->SetPosition(position);
        m_camera->SetFocalPoint(focalPoint);

        //多个Render的大小不同，需要统一计算出合适相机视角
        fitView();

        //通知交互器窗口视角发生变化
        if (m_interactionStyle)
        {
            emit m_interactionStyle->sig_viewChangedManully();
        }
    }

    void FITKGraph3DWindowVTK::setDefaultView(double* focalPoint, double* viewUp, double* position)
    {
        for (int i = 0; i < 3; ++i)
        {
            _defFp[i] = focalPoint[i];
            _defVp[i] = viewUp[i];
            _defPos[i] = position[i];
        }
    }

    void FITKGraph3DWindowVTK::setISOView(double* focalPoint, double* viewUp, double* position)
    {
        for (int i = 0; i < 3; ++i)
        {
            _ISOFp[i] = focalPoint[i];
            _ISOVp[i] = viewUp[i];
            _ISOPos[i] = position[i];
        }
    }

    bool FITKGraph3DWindowVTK::getView(double* pos, double* focual, double* viewup)
    {
        if (m_camera == nullptr)return false;
        m_camera->GetPosition(pos);
        m_camera->GetFocalPoint(focual);
        m_camera->GetViewUp(viewup);
        return true;
    }

    void FITKGraph3DWindowVTK::setViewBounds(double* bds)
    {
        if (m_renders.isEmpty())
        {
            return;
        }

        // 获取渲染器设置包围盒。
        FITKGraphRender* fRender = m_renders[0];
        if (!fRender)
        {
            return;
        }

        vtkRenderer* renderer = fRender->getRenderer();
        if (!renderer)
        {
            return;
        }

        renderer->ResetCamera(bds);

        // 刷新渲染。
        reRender();
    }

    void FITKGraph3DWindowVTK::fastAddObject(int index, FITKGraphObjectVTK* object)
    {
        FITKGraphRender* fRender = m_renders.value(index);
        if (!fRender)
        {
            return;
        }

        fRender->addObject(object);

        // 添加控件。
        int nWidget = object->getWidgetCount();
        for (int i = 0; i < nWidget; i++)
        {
            vtkInteractorObserver* widget = object->getWidget(i);
            if (!widget)
            {
                continue;
            }

            // 设置交互器。
            if (m_renderWindow)
            {
                widget->SetInteractor(m_renderWindow->GetInteractor());
            }

            // 控件默认使用最下层渲染器，防止遮挡其他演员。
            int index = 0;// _initializer->getLayerCount() - 1;
            FITKGraphRender* render = m_renders[index];
            if (render)
            {
                widget->SetCurrentRenderer(render->getRenderer());
            }
        }
    }

    void FITKGraph3DWindowVTK::addObject(int index, FITKGraphObjectVTK* object, bool fitview)
    {
        FITKGraphRender* r = m_renders.value(index);
        if (r == nullptr)return;

        int graphCount = getGraphObjCount();

        r->addObject(object);

        // 添加控件。
        int nWidget = object->getWidgetCount();
        for (int i = 0; i < nWidget; i++)
        {
            vtkInteractorObserver* widget = object->getWidget(i);
            if (!widget)
            {
                continue;
            }

            // 设置交互器。
            if (m_renderWindow)
            {
                widget->SetInteractor(m_renderWindow->GetInteractor());
            }

            // 控件默认使用最下层渲染器，防止遮挡其他演员。
            int index = 0;// _initializer->getLayerCount() - 1;
            FITKGraphRender* render = m_renders[index];
            if (render)
            {
                widget->SetCurrentRenderer(render->getRenderer());
            }

            // widget->SetCurrentRenderer(m_renderer);
        }

        //添加actor后，不同Render的相机发生变化需要自适应
        if (fitview && graphCount == 0)
        {
            this->fitView();
        }
        else
        {
            updateBoundary();
        }
        //else
        //{ 
        //    this->reRender();
        //    /*if (graphCount == 0)
        //    {
        //        this->fitView();
        //    }*/
        //}
    }

    QList<FITKGraphObjectVTK*> FITKGraph3DWindowVTK::getAllGraphObj()
    {
        QList<FITKGraphObjectVTK*> objs = {};
        for (FITKGraphRender* render : m_renders.values()) {
            if (render == nullptr) {
                continue;
            }
            Core::FITKGraphObjManager* objManager = render->getGraphObjManager();
            if (objManager == nullptr) {
                continue;
            }

            int objCount = objManager->getGraphObjCount();
            for (int i = 0; i < objCount; i++) {
                objs.append(dynamic_cast<FITKGraphObjectVTK*>(objManager->getGraphObjAt(i)));
            }
        }
        return objs;
    }

    void FITKGraph3DWindowVTK::clear()
    {
        for (auto render : m_renders.values())
        {
            if (render == nullptr)continue;
            render->clear();
        }
    }

    void FITKGraph3DWindowVTK::fitView()
    {
        double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };

        bool emptyWindow = true;
        for (auto render : m_renders.values())
        {
            if (render == nullptr)continue;

            //获取渲染图层中的所有actor，并计算大小
            double length = render->getActorBounds(bound);
            if (length > 0) emptyWindow = false;
        }

        if (emptyWindow)
        {
            bound[0] = bound[2] = bound[4] = -1;
            bound[1] = bound[3] = bound[5] = 1;
        }
        //更新边界
        if (m_actorBound != nullptr)
            m_actorBound->updateBoundary(bound);

        for (auto render : m_renders.values())
        {
            render->getRenderer()->ResetCamera(bound);
        }

        // 刷新标签。
        updateAnnotations();

        reRender();
    }

    void FITKGraph3DWindowVTK::setIsShowClipAction(bool isShow)
    {
        if (_renderToolBar == nullptr || _renderToolBar->isHidden() == true) {
            return;
        }
        for (QAction* clipAction : _renderToolBar->actions()) {
            if (clipAction == nullptr || clipAction->objectName() != "actionRenderClip") {
                continue;
            }
            clipAction->setVisible(isShow);
            break;
        }
    }

    void FITKGraph3DWindowVTK::updateBoundary()
    {
        double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };

        bool emptyWindow = true;
        for (auto render : m_renders.values())
        {
            if (render == nullptr)continue;

            //获取渲染图层中的所有actor，并计算大小
            double length = render->getActorBounds(bound);
            if (length > 0) emptyWindow = false;
        }

        if (emptyWindow)
        {
            bound[0] = bound[2] = bound[4] = -1;
            bound[1] = bound[3] = bound[5] = 1;
        }
        //更新边界
        if (m_actorBound != nullptr)
            m_actorBound->updateBoundary(bound);

        // 重置相机视距。
        vtkRenderer* renderer = getVTKRenderer(getRenderCount() - 1);
        if (renderer)
        {
            // Added by ChengHaotian 2026/05/19 适当放大边界。
            bound[0] -= (bound[1] - bound[0]) * 0.25;
            bound[1] += (bound[1] - bound[0]) * 0.25;
            bound[2] -= (bound[3] - bound[2]) * 0.25;
            bound[3] += (bound[3] - bound[2]) * 0.25;
            bound[4] -= (bound[5] - bound[4]) * 0.25;
            bound[5] += (bound[5] - bound[4]) * 0.25;
            renderer->ResetCameraClippingRange(bound);
        }
        //for (int i = 0; i < getRenderCount(); i++)
        //{
        //    vtkRenderer* renderer = getVTKRenderer(i);
        //    if (renderer)
        //    {
        //        renderer->ResetCameraClippingRange(bound);
        //    }
        //}
    }

    bool FITKGraph3DWindowVTK::getBoundray(double* bds)
    {
        if (!m_actorBound)
        {
            return false;
        }

        vtkActor* bdActor = m_actorBound->getActor();
        if (!bdActor)
        {
            return false;
        }

        //if (!bdActor->GetUseBounds())
        //{
        //    return false;
        //}

        vtkDataSet* dataSet = bdActor->GetMapper()->GetInput();
        if (!dataSet)
        {
            return false;
        }

        dataSet->GetBounds(bds);
        return true;
    }

    void FITKGraph3DWindowVTK::SetGlobalWarningDisplay(bool isOpen)
    {
        vtkObject::SetGlobalWarningDisplay(isOpen);
    }

    void FITKGraph3DWindowVTK::SetUseDepthPeeling(bool isOpen, int peelMaxNum, int renderIndex)
    {
        Core::FITKSystemInfo* info = new Core::FITKSystemInfo();
        if (info->getGPUType() == Core::GPUType::FITK_GPU_AMD) {
            if (m_renderWindow) {
                m_renderWindow->SetMultiSamples(isOpen ? 0 : 2);
            }
        }
        if (renderIndex == -1) {
            for (int i = 0; i < m_renders.size(); i++) {
                FITKGraphRender* render = m_renders.value(i);
                if (render == nullptr) {
                    continue;
                }
                vtkRenderer* vtkRender = render->getRenderer();
                if (vtkRender == nullptr) {
                    continue;
                }

                vtkRender->SetUseDepthPeeling(isOpen);
                vtkRender->SetMaximumNumberOfPeels(peelMaxNum);
                vtkRender->SetOcclusionRatio(0.5);
            }
        }
        else {
            FITKGraphRender* render = m_renders.value(renderIndex);
            if (render == nullptr) {
                return;
            }
            vtkRenderer* vtkRender = render->getRenderer();
            if (vtkRender == nullptr) {
                return;
            }
            vtkRender->SetUseDepthPeeling(isOpen);
            vtkRender->SetMaximumNumberOfPeels(peelMaxNum);
            vtkRender->SetOcclusionRatio(0.5);
        }
    }

    FITKGraphInteractionStyle* FITKGraph3DWindowVTK::getGraphInteractionStyle()
    {
        return m_interactionStyle;
    }

    void FITKGraph3DWindowVTK::flush()
    {
        this->reRender();
    }

    void FITKGraph3DWindowVTK::setBackgroundColor(float* rgb1, float* rgb2 /*=nullptr*/)
    {
        const int n = this->getRenderCount();
        for (int i = 0; i < n; ++i)
        {
            FITKGraphRender* r = this->getRenderer(i);
            if (r) r->setBackgroundColor(rgb1, rgb2);
        }
    }

    void FITKGraph3DWindowVTK::getBackgroundColor(float* rgb1, float* rgb2)
    {
        const int n = this->getRenderCount();
        for (int i = 0; i < n; ++i)
        {
            FITKGraphRender* r = this->getRenderer(i);
            if (r == nullptr)continue;
            vtkRenderer* render = r->getRenderer();
            if (render == nullptr)continue;

            double* butColor = render->GetBackground2();
            rgb1[0] = butColor[0];
            rgb1[1] = butColor[1];
            rgb1[2] = butColor[2];

            if (rgb2 != nullptr) {
                double* topColor = render->GetBackground();
                rgb2[0] = topColor[0];
                rgb2[1] = topColor[1];
                rgb2[2] = topColor[2];
            }
            return;
        }
    }

    void FITKGraph3DWindowVTK::setInteractionStyle(FITKGraphInteractionStyle* style)
    {
        if (style == nullptr)return;
        m_interactionStyle = style;
        //添加交互器
        m_interactionStyle->setInteractor(m_interactor);
        //添加事件对应的窗口
        m_interactionStyle->setGraph3DWin(this);

        //设置框选器对应的渲染图层，默认选择最表面的渲染图层
        int lastRenderIndex = m_renders.size() - 1;
        if (lastRenderIndex >= 0 && m_renders.value(lastRenderIndex) != nullptr)
        {
            m_interactionStyle->setAreaPickRenderer(m_renders.value(lastRenderIndex)->getRenderer());
        }
    }

    FITKGraphRender* FITKGraph3DWindowVTK::getRenderer(int index)
    {
        //越界判断
        if (index < 0 || index >= m_renders.size())return nullptr;
        return m_renders.value(index);
    }

    int FITKGraph3DWindowVTK::getRenderCount()
    {
        if (m_renderWindow)
            return m_renderWindow->GetNumberOfLayers();
        return 0;
    }

    vtkRenderWindow* FITKGraph3DWindowVTK::getVTKRenderWindow()
    {
        return m_renderWindow;
    }

    vtkCamera* FITKGraph3DWindowVTK::getVTKCamera()
    {
        // 获取VTK相机。
        return m_camera;
    }

    vtkRenderer* FITKGraph3DWindowVTK::getVTKRenderer(int index)
    {
        // 获取VTK渲染器。
        if (index == -1)
        {
            int nRender = m_renders.count();
            if (nRender == 0)
            {
                return nullptr;
            }

            FITKGraphRender* render = getRenderer(nRender - 1);
            if (!render)
            {
                return nullptr;
            }

            return render->getRenderer();
        }
        else
        {
            FITKGraphRender* render = getRenderer(index);
            if (!render)
            {
                return nullptr;
            }

            return render->getRenderer();
        }
    }

    //     void FITKGraph3DWindow::setAredPickerState(bool state)
    //     {
    //         if (m_interactionStyle == nullptr)return;
    //         m_interactionStyle->setAredPickerState(state);
    //     }

    void FITKGraph3DWindowVTK::setParallelProjection(bool enable)
    {
        if (m_camera == nullptr) return;
        m_camera->SetParallelProjection(enable);
        this->reRender();
    }

    bool FITKGraph3DWindowVTK::getParallelProjection()
    {
        if (m_camera == nullptr) return false;
        return m_camera->GetParallelProjection();
    }

    void FITKGraph3DWindowVTK::removeGraphObj(Core::FITKAbstractGraphObject* gobj)
    {
        FITKGraphObjectVTK* objvtk = dynamic_cast<FITKGraphObjectVTK*>(gobj);
        if (objvtk == nullptr) return;

        for (auto render : m_renders)
        {
            if (render == nullptr) continue;
            render->removeObject(objvtk);
        }
    }

    int FITKGraph3DWindowVTK::getGraphObjCount()
    {
        int c = 0;
        for (auto render : m_renders)
        {
            if (render == nullptr) continue;
            c += render->getGraphObjectCount();
        }
        return c;
    }

    bool FITKGraph3DWindowVTK::getGraphObjCenter(double* center)
    {

        double bound[6] = { 9e64, -9e64, 9e64, -9e64, 9e64, -9e64 };

        bool emptyWindow = true;
        for (auto render : m_renders.values())
        {
            if (render == nullptr)continue;

            //获取渲染图层中的所有actor，并计算大小
            double length = render->getActorBounds(bound);
            if (length > 0) emptyWindow = false;
        }

        if (emptyWindow)
        {
            bound[0] = bound[2] = bound[4] = -1;
            bound[1] = bound[3] = bound[5] = 1;
        }
        center[0] = (bound[0] + bound[1]) / 2.0;
        center[1] = (bound[2] + bound[3]) / 2.0;
        center[2] = (bound[4] + bound[5]) / 2.0;
        return true;
    }

    void FITKGraph3DWindowVTK::setCurImageType(Comp::FITKGraphWinImageType imageType, bool shouldLogSaveImageInfo)
    {
        _imageType = imageType;
        _shouldLogSaveImageInfo = shouldLogSaveImageInfo;
    }

    void FITKGraph3DWindowVTK::saveImage(const QString& image, int w, int h)
    {
        Q_UNUSED(w);
        Q_UNUSED(h);
        if (image.isEmpty())return;
        vtkSmartPointer<vtkWindowToImageFilter> imageFilter
            = vtkSmartPointer<vtkWindowToImageFilter>::New();
        imageFilter->SetInput(m_renderWindow);
        //设置缩放比例，比例需要根据窗口大小自己计算
        //imageFilter->SetScale(2)

        vtkSmartPointer<vtkImageWriter> writer = vtkSmartPointer<vtkImageWriter>::New();

        switch (_imageType)
        {
        case Comp::FITKGraphWinImageType::FITK_PNG: {
            writer = vtkSmartPointer<vtkPNGWriter>::New();
            break;
        }
        case Comp::FITKGraphWinImageType::FITK_JPEG: {
            writer = vtkSmartPointer<vtkJPEGWriter>::New();
            break;
        }
        case Comp::FITKGraphWinImageType::FITK_TIFF: {
            writer = vtkSmartPointer<vtkTIFFWriter>::New();
            break;
        }
        case Comp::FITKGraphWinImageType::FITK_BMP: {
            writer = vtkSmartPointer<vtkBMPWriter>::New();
            break;
        }
        case  FITKGraphWinImageType::FITK_SVG: {
            QSvgGenerator svgGenerator;
            svgGenerator.setFileName(image);
            svgGenerator.setSize(size());
            svgGenerator.setViewBox(QRect(0, 0, size().width(), size().height()));

            QPainter painter;
            painter.begin(&svgGenerator);
            render(&painter);
            painter.end();

            QString messageText = QString(tr("The specified viewports were printed to file \"%1\"").arg(image));
            AppFrame::FITKMessageNormal(messageText);
            return;
        }
        default:
            return;
        }

        writer->SetFileName(image.toUtf8());
        writer->SetInputConnection(imageFilter->GetOutputPort());
        writer->Write();

        if (_shouldLogSaveImageInfo)
        {
            QString messageText = QString(tr("The specified viewports were printed to file \"%1\"").arg(image));
            AppFrame::FITKMessageNormal(messageText);
        }
    }

    void FITKGraph3DWindowVTK::showScaleLegend(bool visibility)
    {
        if (m_legendScaleActor)
        {
            m_legendScaleActor->SetVisibility(visibility);
        }
    }

    void FITKGraph3DWindowVTK::addActionsToolBar(QList<QAction*> actions, QAction* beforeAction)
    {
        //互斥锁
        QMutexLocker locer(&_mutex);

        if (_renderToolBarLayout == nullptr)return;
        //追加action
        _renderToolBar->insertActions(beforeAction, actions);
        setToolBarPos(_renderBarStyle);

        //修改toolbar中所有的action背景颜色为透明
        QList<QToolButton*> buttons = _renderToolBar->findChildren<QToolButton*>();
        for (auto b : buttons) {
            if (b == nullptr)continue;
            b->setStyleSheet("background: transparent;");
            b->setToolTipDuration(1);
        }
    }

    void FITKGraph3DWindowVTK::setToolBarPos(FITKGraphWinToolBarPos pos)
    {
        if (_renderToolBar == nullptr || _renderToolBarLayout == nullptr)return;

        _renderBarStyle = pos;

        switch (pos)
        {
            //1：左顶
        case FITKGraphWinToolBarPos::FITKLeftTop: {
            _renderToolBar->setOrientation(Qt::Vertical);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop);
            break;
        }
                                                //2：左中 
        case FITKGraphWinToolBarPos::FITKLeftCenter: {
            _renderToolBar->setOrientation(Qt::Vertical);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            break;
        }
                                                   //3：左底
        case FITKGraphWinToolBarPos::FITKLeftBottom: {
            _renderToolBar->setOrientation(Qt::Vertical);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
            break;
        }
                                                   //4：底部
        case FITKGraphWinToolBarPos::FITKBottomCenter: {
            _renderToolBar->setOrientation(Qt::Horizontal);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignHCenter | Qt::AlignBottom);
            break;
        }
                                                     //5：右底  
        case FITKGraphWinToolBarPos::FITKRightBottom: {
            _renderToolBar->setOrientation(Qt::Vertical);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignRight | Qt::AlignBottom);
            break;
        }
                                                    //6：右中
        case FITKGraphWinToolBarPos::FITKRightCenter: {
            _renderToolBar->setOrientation(Qt::Vertical);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
            break;
        }
                                                    //7：右上 
        case FITKGraphWinToolBarPos::FITKRightTop: {
            _renderToolBar->setOrientation(Qt::Vertical);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignRight | Qt::AlignTop);
            break;
        }
                                                 //8：顶部
        case FITKGraphWinToolBarPos::FITKTopCenter: {
            _renderToolBar->setOrientation(Qt::Horizontal);
            _renderToolBarLayout->addWidget(_renderToolBar, 0, 0, Qt::AlignHCenter | Qt::AlignTop);
            break;
        }
        }
    }

    void FITKGraph3DWindowVTK::initRenderIcon(const QPixmap& image, FITKGraphWinToolBarPos pos)
    {
        //互斥锁
        QMutexLocker locker(&_mutex);
        _renderIconToolBar->clear();

        QLabel* label = new QLabel(m_vtkWidget);
        label->setPixmap(image);

        //将 QLabel 添加到布局中的指定位置
        _renderIconToolBar->addWidget(label);

        switch (pos)
        {
            //左侧顶部
        case FITKGraphWinToolBarPos::FITKLeftTop: _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignLeft | Qt::AlignTop); break;
            //左侧底部
        case FITKGraphWinToolBarPos::FITKLeftBottom: _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignLeft | Qt::AlignBottom); break;
            //右侧底部
        case FITKGraphWinToolBarPos::FITKRightBottom: _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignRight | Qt::AlignBottom); break;
            //右侧顶部
        case FITKGraphWinToolBarPos::FITKRightTop: _renderToolBarLayout->addWidget(_renderIconToolBar, 0, 0, Qt::AlignRight | Qt::AlignTop); break;
        }
    }

    QList<QAction*> FITKGraph3DWindowVTK::getActions()
    {
        if (_renderToolBar == nullptr)return QList<QAction*>();
        return _renderToolBar->actions();
    }

    void FITKGraph3DWindowVTK::setIsShowActions(bool isShow)
    {
        if (_renderToolBar == nullptr) {
            return;
        }

        if (isShow == false) {
            _renderToolBar->hide();
        }
        else
        {
            _renderToolBar->show();
        }
    }

    void FITKGraph3DWindowVTK::slotActionViewFrontEvent()
    {
        //前视角参数初始化
        double position[3]{ 0,0,1 };
        double focalPoint[3]{ 0,0,0 };
        double viewUp[3]{ 0,1,0 };
        this->setView(focalPoint, viewUp, position);
    }

    void FITKGraph3DWindowVTK::slotActionViewBackEvent()
    {
        //后视角参数初始化
        double position[3]{ 0,0,-1 };
        double focalPoint[3]{ 0,0,0 };
        double viewUp[3]{ 0,1,0 };
        this->setView(focalPoint, viewUp, position);
    }

    void FITKGraph3DWindowVTK::slotActionViewTopEvent()
    {
        //顶部视角参数初始化
        double position[3]{ 0,1,0 };
        double focalPoint[3]{ 0,0,0 };
        double viewUp[3]{ 0,0,-1 };
        this->setView(focalPoint, viewUp, position);
    }
    void FITKGraph3DWindowVTK::slotActionViewBottomEvent()
    {
        //底部视角参数初始化
        double position[3]{ 0,-1,0 };
        double focalPoint[3]{ 0,0,0 };
        double viewUp[3]{ 0,0,1 };
        this->setView(focalPoint, viewUp, position);
    }
    void FITKGraph3DWindowVTK::slotActionViewLeftEvent()
    {
        //左视角参数初始化
        double position[3]{ -1,0,0 };
        double focalPoint[3]{ 0,0,0 };
        double viewUp[3]{ 0,1,0 };
        this->setView(focalPoint, viewUp, position);
    }
    void FITKGraph3DWindowVTK::slotActionViewRightEvent()
    {
        //右视角参数初始化
        double position[3]{ 1,0,0 };
        double focalPoint[3]{ 0,0,0 };
        double viewUp[3]{ 0,1,0 };
        this->setView(focalPoint, viewUp, position);
    }
    void FITKGraph3DWindowVTK::slotActionViewIsoEvent()
    {
        //正视角参数初始化
//         double position[3]{ 1,1,1 };
//         double focalPoint[3]{ 0,0,0 };
//         double viewUp[3]{ 0,0,1 };
        this->setView(_ISOFp, _ISOVp, _ISOPos);
    }

    void FITKGraph3DWindowVTK::slotActionViewPanEvent()
    {
        //自适应
        fitView();
    }

    void FITKGraph3DWindowVTK::slotActionSaveImageEvent()
    {
        //保存图片文件获取
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG (*.png) ;; JPEG(*.jpeg) ;; TIFF(*.tiff) ;; BMP(*.bmp) ;; SVG(*.svg)"));
        if (filePath.isEmpty())return;
        QFile file(filePath);
        QFileInfo fileInfo(file);
        QString fileSuffix = fileInfo.suffix();
        if (fileSuffix == "png") {
            _imageType = FITKGraphWinImageType::FITK_PNG;
        }
        else if (fileSuffix == "jpeg") {
            _imageType = FITKGraphWinImageType::FITK_JPEG;
        }
        else if (fileSuffix == "tiff") {
            _imageType = FITKGraphWinImageType::FITK_TIFF;
        }
        else if (fileSuffix == "bmp") {
            _imageType = FITKGraphWinImageType::FITK_BMP;
        }
        else if (fileSuffix == "svg") {
            _imageType = FITKGraphWinImageType::FITK_SVG;
        }
        else {
            _imageType = FITKGraphWinImageType::FITK_PNG;
        }
        saveImage(filePath);
    }

    void FITKGraph3DWindowVTK::slotActionViewParallelEvent()
    {
        //平行投影
        setParallelProjection(true);
    }

    void FITKGraph3DWindowVTK::slotActionViewPerspectiveEvent()
    {
        //正交投影
        setParallelProjection(false);
    }

    void FITKGraph3DWindowVTK::slotActionClipEvent(bool isCreateNewModel)
    {
        FITKGraph3DClipDialog* dialog = new FITKGraph3DClipDialog(this, isCreateNewModel);
        dialog->show();
    }

    void FITKGraph3DWindowVTK::init()
    {
        FITKGraphRender* lastRender = this->getRenderer(m_renders.count() - 1);

        //追加边界actor
        if (_initializer && _initializer->getLayerCount() > 0)
        {
            //int index = _initializer->getLayerCount() - 1;
            //if (this->getRenderer(index) && this->getRenderer(index)->getRenderer())
            //{
            //    vtkActor* actor = nullptr;
            //    if (m_actorBound) actor = m_actorBound->getActor();
            //    if (actor)
            //        this->getRenderer(index)->getRenderer()->AddActor(actor);
            //}  

            if (lastRender)
            {
                vtkActor* actor = nullptr;
                if (m_actorBound) actor = m_actorBound->getActor();
                if (actor)
                    lastRender->getRenderer()->AddActor(actor);
            }
        }

        // 创建比例尺（默认隐藏） Added by ChengHaotian
        //@{        
        if (lastRender)
        {
            vtkRenderer* renderer = lastRender->getRenderer();
            if (renderer)
            {
                m_legendScaleActor = FITKLegendScaleActor::New();
                m_legendScaleActor->SetVisibility(false);
                renderer->AddActor(m_legendScaleActor);
            }
        }
        //@}

        //坐标系创建
        //设置坐标系所在的图层
        vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
        axesActor->SetShaftTypeToCylinder();
        axesActor->SetConeRadius(0.5);
        axesActor->SetCylinderRadius(0.1);
        axesActor->SetNormalizedShaftLength(0.7, 0.7, 0.7);
        axesActor->SetNormalizedTipLength(0.3, 0.3, 0.3);
        axesActor->SetConeResolution(12);
        axesActor->SetCylinderResolution(12);
        axesActor->SetNormalizedLabelPosition(1.4, 1.4, 1.4);

        vtkOrientationMarkerWidget* axesWidget = vtkOrientationMarkerWidget::New();
        axesWidget->SetInteractor(m_interactor);
        axesWidget->SetOrientationMarker(axesActor);
        axesWidget->SetDefaultRenderer(getRenderer(0)->getRenderer());
        m_axesWidget = axesWidget;

        // 初始化光照 Added by ChengHaotian
        for (FITKGraphRender* fRender : m_renders)
        {
            if (!fRender)
            {
                continue;
            }

            vtkRenderer* renderer = fRender->getRenderer();
            if (!renderer)
            {
                continue;
            }

            // Initialize light.
            renderer->RemoveAllLights();

            vtkLight* light1 = renderer->MakeLight();
            light1->SetExponent(0.);
            light1->SetLightTypeToHeadlight();
            // light1->SetPositional(true);
            renderer->AddLight(light1);
        }

        //设置自定义交互器
        if (_initializer == nullptr) return;
        this->setInteractionStyle(_initializer->getStyle());
    }

    void FITKGraph3DWindowVTK::initActions()
    {
        QList<QAction*> actions = {};
        QAction* action = nullptr;

        //前视角按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewFront");
        action->setIcon(QIcon(":FITKIcons/icoR_viewFront.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewFrontEvent()));
        actions.append(action);
        //后视角按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewBack");
        action->setIcon(QIcon(":FITKIcons/icoR_viewBack.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewBackEvent()));
        actions.append(action);
        //顶部视角按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewTop");
        action->setIcon(QIcon(":FITKIcons/icoR_viewTop.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewTopEvent()));
        actions.append(action);
        //底部视角按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewBottom");
        action->setIcon(QIcon(":FITKIcons/icoR_viewBottom.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewBottomEvent()));
        actions.append(action);
        //左侧视角按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewLeft");
        action->setIcon(QIcon(":FITKIcons/icoR_viewLeft.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewLeftEvent()));
        actions.append(action);
        //右侧视角按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewRight");
        action->setIcon(QIcon(":FITKIcons/icoR_viewRight.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewRightEvent()));
        actions.append(action);
        //正视角按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewIso");
        action->setIcon(QIcon(":FITKIcons/icoR_viewIso.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewIsoEvent()));
        actions.append(action);
        //自适应按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewAuto");
        action->setIcon(QIcon(":FITKIcons/icoR_autofit.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewPanEvent()));
        actions.append(action);
        //保存图片按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionSaveImage");
        action->setIcon(QIcon(":FITKIcons/icoR_saveimage.png"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionSaveImageEvent()));
        actions.append(action);
        //平行投影按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewParallel");
        action->setIcon(QIcon(":FITKIcons/icoR_viewParallel.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewParallelEvent()));
        actions.append(action);
        //正交投影按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionViewPerspective");
        action->setIcon(QIcon(":FITKIcons/icoR_viewPerspective.svg"));
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionViewPerspectiveEvent()));
        actions.append(action);
        //裁切按钮添加
        action = new QAction(m_vtkWidget);
        action->setObjectName("actionRenderClip");
        action->setIcon(QIcon(":FITKIcons/icoR_clip.png"));
        //裁切按钮默认隐藏
        action->setVisible(false);
        connect(action, SIGNAL(triggered()), this, SLOT(slotActionClipEvent()));
        actions.append(action);

        addActionsToolBar(actions);
    }

    void FITKGraph3DWindowVTK::setUserInformation(QVariant info)
    {
        m_userInfo = info;
    }

    QVariant FITKGraph3DWindowVTK::getUserInformation()
    {
        return m_userInfo;
    }

    void FITKGraph3DWindowVTK::setViewAnime(double* focalPoint, double* viewUp, double* position)
    {
        // 异常处理。
        for (int i = 0; i < 3; i++)
        {
            if (qIsNaN(focalPoint[i]) || qIsNaN(viewUp[i]) || qIsNaN(position[i]))
            {
                return;
            }
        }

        startChangingView(focalPoint, viewUp, position);
    }

    void FITKGraph3DWindowVTK::startChangingView(double* foc, double* up, double* pos)
    {
        if (m_timerView.isActive())
        {
            m_timerView.stop();
        }

        m_currStep = 0;

        // 保存目标相机参数。
        for (int i = 0; i < 3; i++)
        {
            m_tarFoc3[i] = foc[i];
            m_tarUp3[i] = up[i];
            m_tarPos3[i] = pos[i];
        }

        // 启动定时器。
        m_timerView.start();
    }

    void FITKGraph3DWindowVTK::createTempAnnotation()
    {
        int iRender = getRenderCount() - 1;
        FITKGraphRender* render = getRenderer(iRender);
        if (!render)
        {
            return;
        }

        // Create the annotation data.
        m_tempAnno = new FITKGraphAnnotation(this);
        connect(m_tempAnno, &FITKGraphAnnotation::dataObjectDestoried, this, [&]
        {
            m_tempAnno = nullptr;
        });

        m_tempAnno->setDataObjectName("AnnoTemp");

        // Create the annotation graph object at the same time.
        m_tempAnnoGraph = FITKGraphObjectAnnotation::New(m_tempAnno);
        m_tempAnnoGraph->initializeObject();

        // Connect the signals.
        connect(m_tempAnno, &FITKGraphAnnotation::sig_dataUpdated, m_tempAnnoGraph, &FITKGraphObjectAnnotation::update);
        connect(m_tempAnno, &FITKGraphAnnotation::sig_modifyStart, m_tempAnnoGraph, &FITKGraphObjectAnnotation::modifyStartAnchor);
        connect(m_tempAnno, &FITKGraphAnnotation::sig_modifyEnd, m_tempAnnoGraph, &FITKGraphObjectAnnotation::modifyEndAnchor);
        connect(m_tempAnno, &FITKGraphAnnotation::sig_stopModifying, m_tempAnnoGraph, &FITKGraphObjectAnnotation::endModifyingAnchor);
        connect(m_tempAnno, &FITKGraphAnnotation::dataObjectDestoried, this, [&](Core::FITKAbstractDataObject* dataDel)
        {
            if (m_tempAnnoGraph)
            {
                delete m_tempAnnoGraph;
            }

            m_tempAnnoGraph = nullptr;
        });

        // Add to the render window.
        fastAddObject(iRender, m_tempAnnoGraph);
    }

    void FITKGraph3DWindowVTK::slot_changingView()
    {
        m_currStep++;

        if (m_currStep >= m_viewStep)
        {
            // 设置相机视角至目标属性。
            m_camera->SetViewUp(m_tarUp3);
            m_camera->SetPosition(m_tarPos3);
            m_camera->SetFocalPoint(m_tarFoc3);
            return;
        }

        double* currFoc = m_camera->GetFocalPoint();
        double* currUp = m_camera->GetViewUp();
        double* currPos = m_camera->GetPosition();

        double deltaFoc3[3]{ 0., 0., 0. };
        double deltaUp3[3]{ 0., 0., 0. };
        double deltaPos3[3]{ 0., 0., 0. };

        // 计算相机变量。
        int steps = m_viewStep - m_currStep;
        for (int i = 0; i < 3; i++)
        {
            deltaFoc3[i] = (m_tarFoc3[i] - currFoc[i]) / steps;
            deltaUp3[i] = (m_tarUp3[i] - currUp[i]) / steps;
            deltaPos3[i] = (m_tarPos3[i] - currPos[i]) / steps;
        }

        // 设置相机视角。
        m_camera->SetViewUp(currUp[0] + deltaUp3[0], currUp[1] + deltaUp3[1], currUp[2] + deltaUp3[2]);
        m_camera->SetPosition(currPos[0] + deltaPos3[0], currPos[1] + deltaPos3[1], currPos[2] + deltaPos3[2]);
        m_camera->SetFocalPoint(currFoc[0] + deltaFoc3[0], currFoc[1] + deltaFoc3[1], currFoc[2] + deltaFoc3[2]);

        // 刷新渲染。
        reRender();

        // 启动定时器执行下一步。
        m_timerView.start();
    }
}