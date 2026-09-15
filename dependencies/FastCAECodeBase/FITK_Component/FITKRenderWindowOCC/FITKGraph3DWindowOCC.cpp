/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraph3DWindowOCC.h"
#include "FITKAIS_SketchShape.h"

// Qt
#include <QApplication>
#include <QMouseEvent>
#include <QGridLayout>
#include <QToolBar>
#include <QAction>
#include <QToolButton>
#include <QPaintDevice>
#include <QFileDialog>

// OCC Commons
#include <Standard_Version.hxx>
#include <Standard_Handle.hxx>

// OCC
#include <AIS_ViewController.hxx>
#include <Quantity_NameOfColor.hxx>
#include <Prs3d_PointAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <Prs3d_PlaneAspect.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_TextAspect.hxx>
#include <Graphic3d_RenderingParams.hxx>
#include <MeshVS_Drawer.hxx>
#include <MeshVS_NodalColorPrsBuilder.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_SelectionModeFlags.hxx>

#include <OpenGl_GraphicDriver.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <Graphic3d_Camera.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_Shape.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Manipulator.hxx>
#include <MeshVS_DataSource.hxx>
#include <MeshVS_Mesh.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Aspect_Grid.hxx>
#include <AIS_RubberBand.hxx>
#include <AIS_ViewController.hxx>
#include <AIS_Circle.hxx>
#include <gp_Circ.hxx>
#include <Geom_Circle.hxx>
#include <AIS.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include <Image_AlienPixMap.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRepBndLib.hxx>
#include <AIS_TextLabel.hxx>

// Graph
#include "FITKGraphObjectOCC.h"
#include "FITKV3d_View.h"
#include "FITKV3d_Viewer.h"
#include "FITKAIS_ViewController.h"
#include "WidgetAddSketchPos.h"

// Window frame
#ifdef Q_OS_WIN
#include <WNT_Window.hxx>
#else
#include <Xw_Window.hxx>
#endif

// Qt mouse buttons to OCC mouse buttons.
//@{
static Aspect_VKeyMouse qtMouseButtons2VKeys(Qt::MouseButtons qtBtns)
{
    Aspect_VKeyMouse aButtons = Aspect_VKeyMouse_NONE;
    if ((qtBtns & Qt::LeftButton) != 0)
    {
        aButtons |= Aspect_VKeyMouse_LeftButton;
    }
    if ((qtBtns & Qt::MiddleButton) != 0)
    {
        aButtons |= Aspect_VKeyMouse_MiddleButton;
    }
    if ((qtBtns & Qt::RightButton) != 0)
    {
        aButtons |= Aspect_VKeyMouse_RightButton;
    }

    return aButtons;
}
//@}

// Qt modifiers to OCC VKeys.
//@{
static Aspect_VKeyFlags qtModifiers2VKeys(Qt::KeyboardModifiers qtMods)
{
    Aspect_VKeyFlags aMods = Aspect_VKeyMouse_NONE;
    if ((qtMods & Qt::ShiftModifier) != 0)
    {
        aMods |= Aspect_VKeyFlags_SHIFT;
    }
    if ((qtMods & Qt::ControlModifier) != 0)
    {
        aMods |= Aspect_VKeyFlags_CTRL;
    }
    if ((qtMods & Qt::AltModifier) != 0)
    {
        aMods |= Aspect_VKeyFlags_ALT;
    }
    if ((qtMods & Qt::MetaModifier) != 0)
    {
        aMods |= Aspect_VKeyFlags_META;
    }

    return aMods;
}
//@}

namespace Render
{
    FITKGraph3DWindowOCCInternal::FITKGraph3DWindowOCCInternal(FITKGraph3DWindowOCC* occWidget)
        : Core::FITKWidget(occWidget), m_occWidget(occWidget)
    {
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_NoSystemBackground);
        // setBackgroundRole(QPalette::NoRole);
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
    }

    void FITKGraph3DWindowOCCInternal::setController(AIS_ViewController* controller)
    {
        m_controller = controller;
    }

    void FITKGraph3DWindowOCCInternal::setContext(AIS_InteractiveContext* context)
    {
        m_context = context;
    }

    void FITKGraph3DWindowOCCInternal::setViewer(V3d_Viewer* viewer)
    {
        m_viewer = viewer;
    }

    void FITKGraph3DWindowOCCInternal::setView(V3d_View* view)
    {
        m_view = view;
    }

    void FITKGraph3DWindowOCCInternal::setTextLabel(AIS_TextLabel* label)
    {
        m_coordLabel = label;
    }

    QPaintEngine* FITKGraph3DWindowOCCInternal::paintEngine() const
    {
        return nullptr;
    }

    gp_Pnt FITKGraph3DWindowOCCInternal::convertScreenPointOnPlane(const int & Xp, const int & Yp, V3d_View* view)
    {
        gp_Pnt point;

        // Check if Ctrl has been pressed.
        double x, y, z;
        Qt::KeyboardModifiers mods = QApplication::keyboardModifiers();
        if (mods.testFlag(Qt::KeyboardModifier::ControlModifier))
        {
            // Snap to the grid point.
            view->ConvertToGrid(Xp, Yp, x, y, z);
            point.SetX(x);
            point.SetY(y);
            point.SetZ(z);
        }
        else
        {   
            view->Convert(Xp, Yp, x, y, z); //, vx, vy, vz

            gp_Dir VPN = view->Camera()->Direction().Reversed(); // RefPlane
            gp_Pln aPlane(m_viewer->PrivilegedPlane());
            gp_Dir GPN = aPlane.Axis().Direction();
            // GPN.Reverse();

            Standard_Real XPp = 0.0, YPp = 0.0;
            view->Project(x, y, z, XPp, YPp);

            // Casw when the plane of the grid and the plane of the view
            // are perpendicular to MYEPSILON2 close radians
            if (Abs(VPN.Angle(GPN) - M_PI / 2.) < (M_PI / 180))
            {
                return gp_Pnt(x, y, z);
            }

            TColStd_Array2OfReal MyTrsf(1, 4, 1, 4);
            Standard_Real xl, yl, zl;
            Standard_Real xdx, xdy, xdz;
            Standard_Real ydx, ydy, ydz;
            Standard_Real dx, dy, dz;
            aPlane.Location().Coord(xl, yl, zl);
            aPlane.XAxis().Direction().Coord(xdx, xdy, xdz);
            aPlane.YAxis().Direction().Coord(ydx, ydy, ydz);
            GPN.Coord(dx, dy, dz);

            Standard_Real CosAlpha = Cos(view->Viewer()->Grid()->RotationAngle());
            Standard_Real SinAlpha = Sin(view->Viewer()->Grid()->RotationAngle());

            TColStd_Array2OfReal Trsf1(1, 4, 1, 4);
            Trsf1(4, 4) = 1.0;
            Trsf1(4, 1) = Trsf1(4, 2) = Trsf1(4, 3) = 0.0;
            // Translation
            Trsf1(1, 4) = xl,
                Trsf1(2, 4) = yl,
                Trsf1(3, 4) = zl;
            // Transformation change of marker
            Trsf1(1, 1) = xdx,
                Trsf1(2, 1) = xdy,
                Trsf1(3, 1) = xdz,
                Trsf1(1, 2) = ydx,
                Trsf1(2, 2) = ydy,
                Trsf1(3, 2) = ydz,
                Trsf1(1, 3) = dx,
                Trsf1(2, 3) = dy,
                Trsf1(3, 3) = dz;

            TColStd_Array2OfReal Trsf2(1, 4, 1, 4);
            Trsf2(4, 4) = 1.0;
            Trsf2(4, 1) = Trsf2(4, 2) = Trsf2(4, 3) = 0.0;
            // Translation of the origin
            Trsf2(1, 4) = -view->Viewer()->Grid()->XOrigin(),
                Trsf2(2, 4) = -view->Viewer()->Grid()->YOrigin(),
                Trsf2(3, 4) = 0.0;
            // Rotation Alpha around axis -Z
            Trsf2(1, 1) = CosAlpha,
                Trsf2(2, 1) = -SinAlpha,
                Trsf2(3, 1) = 0.0,
                Trsf2(1, 2) = SinAlpha,
                Trsf2(2, 2) = CosAlpha,
                Trsf2(3, 2) = 0.0,
                Trsf2(1, 3) = 0.0,
                Trsf2(2, 3) = 0.0,
                Trsf2(3, 3) = 1.0;

            Standard_Real valuetrsf;
            Standard_Real valueoldtrsf;
            Standard_Real valuenewtrsf;
            Standard_Integer i, j, k;
            // Calculation of the product of matrices
            for (i = 1; i <= 4; i++)
                for (j = 1; j <= 4; j++) {
                    MyTrsf(i, j) = 0.0;
                    for (k = 1; k <= 4; k++) {
                        valueoldtrsf = Trsf1(i, k);
                        valuetrsf = Trsf2(k, j);
                        valuenewtrsf = MyTrsf(i, j) + valueoldtrsf * valuetrsf;
                        MyTrsf(i, j) = valuenewtrsf;
                    }
                }

            Graphic3d_Vertex thePnt(0.0, 0.0, 0.0);
            gp_XYZ aPnt0;

            // CAL. S3892
            Standard_Integer lr = MyTrsf.LowerRow();
            Standard_Integer ur = MyTrsf.UpperRow();
            Standard_Integer lc = MyTrsf.LowerCol();
            Standard_Integer uc = MyTrsf.UpperCol();
            if ((ur - lr + 1 != 4) || (uc - lc + 1 != 4))
            {
                aPnt0 = gp_XYZ(thePnt.X(), thePnt.Y(), thePnt.Z());
            }
            else
            {
                Standard_Real X, Y, Z;
                thePnt.Coord(X, Y, Z);
                Standard_Real XX = (MyTrsf(lr, lc + 3) + X * MyTrsf(lr, lc) + Y * MyTrsf(lr, lc + 1) + Z * MyTrsf(lr, lc + 2)) / MyTrsf(lr + 3, lc + 3);
                Standard_Real YY = (MyTrsf(lr + 1, lc + 3) + X * MyTrsf(lr + 1, lc) + Y * MyTrsf(lr + 1, lc + 1) + Z * MyTrsf(lr + 1, lc + 2)) / MyTrsf(lr + 3, lc + 3);
                Standard_Real ZZ = (MyTrsf(lr + 2, lc + 3) + X * MyTrsf(lr + 2, lc) + Y * MyTrsf(lr + 2, lc + 1) + Z * MyTrsf(lr + 2, lc + 2)) / MyTrsf(lr + 3, lc + 3);
                aPnt0 = gp_XYZ(XX, YY, ZZ);
            }

            // project ray from camera onto grid plane
            const gp_Vec aProjection = view->Camera()->IsOrthographic()
                ? gp_Vec(view->Camera()->Direction())
                : gp_Vec(view->Camera()->Eye(), gp_Pnt(x, y, z)).Normalized();
            const gp_Vec aPointOrigin = gp_Vec(gp_Pnt(x, y, z), aPnt0);
            const Standard_Real aT = aPointOrigin.Dot(GPN) / aProjection.Dot(GPN);
            const gp_XYZ aPointOnPlane = gp_XYZ(x, y, z) + aProjection.XYZ() * aT;
            point = aPointOnPlane;
        } 

        return point;
    }

    void FITKGraph3DWindowOCCInternal::paintEvent(QPaintEvent* event)
    {
        if (m_view && m_context && m_controller)
        {
            m_view->InvalidateImmediate();
            m_controller->FlushViewEvents(m_context, m_view, true);
            m_view->MustBeResized();
        }

        Core::FITKWidget::paintEvent(event);
    }

    void FITKGraph3DWindowOCCInternal::resizeEvent(QResizeEvent* event)
    {
        if (m_view)
        {
            m_view->MustBeResized();
        }

        Core::FITKWidget::resizeEvent(event);
    }

    void FITKGraph3DWindowOCCInternal::mousePressEvent(QMouseEvent* event)
    {
        // Update mouse event.
        //@{
        if (m_occWidget->m_enableMouse)
        {
            const double dpRatio = devicePixelRatioF();
            Qt::MouseButtons aMouseButtons = event->buttons();
            Qt::KeyboardModifiers aKeyMods = event->modifiers();

            const Graphic3d_Vec2i aPnt(event->pos().x() * dpRatio, event->pos().y() * dpRatio);
            if (m_view && m_controller->UpdateMouseButtons(aPnt, qtMouseButtons2VKeys(aMouseButtons), qtModifiers2VKeys(aKeyMods), false))
            {
                update();
            }

            // Update sketch draw point
            if (event->button() == Qt::LeftButton && m_occWidget->m_sketchShape != nullptr && m_occWidget->m_sketchShape->drawStatus())
            {
                const gp_Ax3& ax3 = m_occWidget->m_viewer->PrivilegedPlane();

                // None-snap.
                //@{
                gp_Pnt p = convertScreenPointOnPlane(event->pos().x() * dpRatio, event->pos().y() * dpRatio, m_view);
                m_occWidget->m_sketchShape->setInitParameter(p, ax3.Direction(), ax3.XDirection());
                m_occWidget->m_sketchShape->addPoint(p);
                //@}

                // Snap.
                //@{
                //bool hasSnap = false;
                //gp_Pnt pnt;
                //for (m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected())
                //{
                //    TopoDS_Shape s = m_context->SelectedShape();
                //    const TopoDS_Vertex & vertex = TopoDS::Vertex(s);
                //    if (!vertex.IsNull())
                //    {
                //        pnt = BRep_Tool::Pnt(vertex);
                //        hasSnap = true;
                //        break;
                //    }            
                //}

                //if (!hasSnap)
                //{
                //    pnt = convertScreenPointOnPlane(event->pos().x() * dpRatio, event->pos().y() * dpRatio, m_view);
                //}

                //m_occWidget->m_sketchShape->setInitParameter(pnt, ax3.Direction(), ax3.XDirection());
                //m_occWidget->m_sketchShape->addPoint(pnt);
                //@}
            }

            // Right button pressed.
            if (event->button() == Qt::RightButton)
            {
                // Quit the sketch mode and remove the sketch shape.
                if (m_occWidget->m_sketchShape->drawStatus())
                {
                    if (m_occWidget->m_widgetAddPos)
                    {
                        m_occWidget->m_widgetAddPos->setAttribute(Qt::WA_DeleteOnClose, false);
                        m_occWidget->m_widgetAddPos->close();
                        delete m_occWidget->m_widgetAddPos;
                        m_occWidget->m_widgetAddPos = nullptr;
                    }

                    m_occWidget->m_sketchShape->setDrawStatus(false);
                    m_context->Remove(m_occWidget->m_sketchShape, true);
                }
                else
                {
                    // The context menu flag;
                    m_contextMenuFlag = true;
                }
            }
        }
        //@}
    }

    void FITKGraph3DWindowOCCInternal::mouseDoubleClickEvent(QMouseEvent * event)
    {
        //如果参数不满足
        if (event->button() == Qt::LeftButton && m_occWidget->m_sketchShape != nullptr && m_occWidget->m_sketchShape->isParametersSatisfy())
        {
            emit m_occWidget->sig_parametersSatisfy();
        }
    }

    void FITKGraph3DWindowOCCInternal::mouseReleaseEvent(QMouseEvent *event)
    {
        // End mouse event.
        if (m_occWidget->m_enableMouse)
        {
            const double dpRatio = devicePixelRatioF();
            Qt::MouseButtons aMouseButtons = event->buttons();
            Qt::KeyboardModifiers aKeyMods = event->modifiers();

            // Deal with the shit modifier.
            if (!m_occWidget->m_enableMultiSelect)
            {
                aKeyMods.setFlag(Qt::KeyboardModifier::ShiftModifier, false);
            }

            const Graphic3d_Vec2i aPnt(event->pos().x() * dpRatio, event->pos().y() * dpRatio);
            if (m_view && m_controller->UpdateMouseButtons(aPnt, qtMouseButtons2VKeys(aMouseButtons), qtModifiers2VKeys(aKeyMods), true))
            {
                update();
            }
        }

        // Show the context menu.
        if (m_contextMenuFlag)
        {
            // Try to perform select if the mouse button is right button.
            if (event->button() == Qt::RightButton)
            {
                // Return if the mouse hovered shape is selected.
                m_context->InitDetected();

                bool needPerformSelect = true;
                Handle(AIS_InteractiveObject) obj = m_context->DetectedCurrentObject();
                if (obj.get())
                {
                    for (m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected())
                    {
                        Handle(AIS_InteractiveObject) interObj = m_context->SelectedInteractive();
                        if (interObj.get() == obj.get())
                        {
                            needPerformSelect = false;
                            break;
                        }
                    }
                }
                //else
                //{
                //    needPerformSelect = false;
                //}

                if (needPerformSelect)
                {
                    m_context->Select(true);
                    emit m_occWidget->sig_picked();
                }
            }

            emit m_occWidget->sig_contextMenu(mapToGlobal(event->pos()));
        }
    }

    void FITKGraph3DWindowOCCInternal::mouseMoveEvent(QMouseEvent* event)
    {
        // Reset the flag;
        m_contextMenuFlag = false;

        // Mouse move.
        if (m_occWidget->m_enableMouse)
        {
            const double dpRatio = devicePixelRatioF();
            Qt::MouseButtons aMouseButtons = event->buttons();
            Qt::KeyboardModifiers aKeyMods = event->modifiers();
            const Graphic3d_Vec2i aNewPos(event->pos().x() * dpRatio, event->pos().y() * dpRatio);
            if (m_view && m_controller->UpdateMousePosition(aNewPos, qtMouseButtons2VKeys(aMouseButtons), qtModifiers2VKeys(aKeyMods), false))
            {
                update();
            }

            gp_Pnt p;
            if (m_occWidget->m_sketchShape->drawStatus())
            {
                p = convertScreenPointOnPlane(event->pos().x() * dpRatio, event->pos().y() * dpRatio, m_view);

                // Update position text.
                //@{
                if (m_coordLabel)
                {
                    const gp_Ax3 & pln = m_viewer->PrivilegedPlane();
                    const gp_Pnt & planeOrigin = pln.Location();
                    const gp_Dir & planeXDir = pln.XDirection();
                    const gp_Dir & planeYDir = pln.YDirection();

                    gp_Vec vec(planeOrigin, p);
                    double ux = vec * gp_Vec(planeXDir);
                    double vy = vec * gp_Vec(planeYDir);

                    Standard_Real XPp = 0.0, YPp = 0.0;
                    m_view->Project(p.X(), p.Y(), p.Z(), XPp, YPp);
                    m_coordLabel->SetText(TCollection_ExtendedString(
                        QString("X: %1, Y: %2").arg(ux, 0, 'f', 3).arg(vy, 0, 'f', 3).toUtf8().data()));
                    m_coordLabel->SetPosition(p);
                    m_context->Redisplay(m_coordLabel, Standard_False, Standard_True);
                }
                //@}
            }
            else
            {
                // Clear position text.
                //@{
                if (m_coordLabel)
                {
                    m_coordLabel->SetText("");
                    m_context->Redisplay(m_coordLabel, Standard_False, Standard_True);
                }
                //@}
            }

            // Update sketch draw point
            if (m_occWidget->m_sketchShape != nullptr && m_occWidget->m_sketchShape->drawStatus())
            {
                m_occWidget->m_sketchShape->replaceLast(p);
                m_occWidget->m_sketchShape->update();
                m_occWidget->m_sketchShape->SetToUpdate();
                auto s = m_occWidget->m_sketchShape->Shape();
                if (s.IsNull())return;
                if (m_context->IsDisplayed(m_occWidget->m_sketchShape))
                {
                    m_context->Redisplay(m_occWidget->m_sketchShape, true);//更新对象显示
                }
                else
                {
                    //m_context->Display(m_occWidget->m_sketchShape, true);
                    m_context->Display(m_occWidget->m_sketchShape, true, CommonOCC::SelectMode::S_ShapeVertex, false);//显示对象,并且可以拾取该对象的点
                }
            }

            // Update objects.
            m_occWidget->updateObjsByMouseMove(aNewPos.x(), aNewPos.y());
        }
    }

    void FITKGraph3DWindowOCCInternal::wheelEvent(QWheelEvent* event)
    {
        // Zoom.
        if (m_occWidget->m_enableMouse)
        {
            const double dpRatio = devicePixelRatioF();
            m_view->StartZoomAtPoint(width() / 2, height() / 2);
            m_view->ZoomAtPoint(0, 0, event->angleDelta().y() * dpRatio / 10, 0);
        }
    }

    void FITKGraph3DWindowOCCInternal::focusOutEvent(QFocusEvent* event)
    {
        // Reset the window flags.
        const double dpRatio = devicePixelRatioF();
        QPoint cPos = QCursor::pos();

        const Graphic3d_Vec2i aPnt(cPos.x() * dpRatio, cPos.y() * dpRatio);
        if (m_view && m_controller->UpdateMouseButtons(aPnt, Aspect_VKeyMouse_NONE, Aspect_VKeyMouse_NONE, true))
        {
            update();
        }

        Core::FITKWidget::focusOutEvent(event);
    }

    FITKGraph3DWindowOCC::FITKGraph3DWindowOCC(QWidget *parent) : Core::FITKAbstractGraph3DWidget(parent)
    {
        // Initialize OCC widget and Qt attributes.
        init();

        // Initialize the view direction.
        // setView(Core::FITKGraph3DStandardView::F3DVISO);
        if (m_view)
        {
            m_view->SetProj(0.4, 0.35, 0.3);
        }
    }

    FITKGraph3DWindowOCC::~FITKGraph3DWindowOCC()
    {
        // Clear all interactive objects first.
        clear();

        // Clear the GUI pointer.
        m_occBkgWidget = nullptr;
        // m_actionGridLines = nullptr;
        m_toolbar = nullptr;
        m_viewDireActs.clear();

        // Clear the pointer.
        m_context->DecrementRefCounter();
        m_context = nullptr;
        m_controller = nullptr;
        m_viewCube = nullptr;
        m_driver = nullptr;
        m_viewer = nullptr;
        m_view = nullptr;
        m_coordLabel = nullptr;

        // Clear the window pointer.
        m_occWindow = nullptr;
    }

    void FITKGraph3DWindowOCC::init()
    {
        // Create the view controller for mosue, wheel and paint event.
        m_controller = new FITKAIS_ViewController;
        connect(m_controller, &FITKAIS_ViewController::sig_pickPerformed, this, &FITKGraph3DWindowOCC::slot_mouseSelected, Qt::UniqueConnection);

        // The occ window's container.
        QGridLayout* lay = new QGridLayout(this);
        m_occBkgWidget = new FITKGraph3DWindowOCCInternal(this);

        lay->setContentsMargins(0, 0, 0, 0);
        lay->addWidget(m_occBkgWidget);

        // The background of occ window.
        QWidget* occBkg = m_occBkgWidget;

        // Initialize driver.
        Handle(Aspect_DisplayConnection) conn = new Aspect_DisplayConnection;
        Handle(OpenGl_GraphicDriver) driverHandle = new OpenGl_GraphicDriver(conn);
        m_driver = driverHandle.get();

        // Initialize viewer.
        Handle(FITKV3d_Viewer) viewerHandle = new FITKV3d_Viewer(m_driver);
        m_viewer = viewerHandle.get();
        m_viewer->SetDefaultShadingModel(V3d_GOURAUD);
        m_viewer->SetDefaultVisualization(V3d_ZBUFFER); // V3d_WIREFRAME V3d_ZBUFFER
        m_viewer->SetDefaultLights();
        m_viewer->SetLightOn();

        // Set default projection type. 
        // V3d_ORTHOGRAPHIC V3d_PERSPECTIVE
        m_viewer->SetDefaultTypeOfView(V3d_TypeOfView::V3d_ORTHOGRAPHIC);

        // Initialize context.
        Handle(AIS_InteractiveContext) contextHandle = new AIS_InteractiveContext(m_viewer);
        m_context = contextHandle.get();
        m_context->IncrementRefCounter();
        m_context->SetDisplayMode(AIS_Shaded, Standard_True);
        m_context->SetDeviationCoefficient(0.001);

        // Z-Layer test.
        //@{
        //        Handle(Prs3d_Drawer) selDrawer = new Prs3d_Drawer;
        //        //
        //        selDrawer->SetLink(m_context->DefaultDrawer());
        //        selDrawer->SetFaceBoundaryDraw(true);
        //        selDrawer->SetDisplayMode(1); // Shaded
        //        selDrawer->SetTransparency(0.5f);
        //        selDrawer->SetZLayer(Graphic3d_ZLayerId_Topmost);
        //        selDrawer->SetColor(Quantity_NOC_GOLD);
        //        selDrawer->SetBasicFillAreaAspect(new Graphic3d_AspectFillArea3d());

        //        Handle(AIS_InteractiveObject) theInterObj;
        //        Standard_Integer anId = 3;
        //        m_viewer->AddZLayer (anId);
        //        m_context->SetZLayer (theInterObj, anId);

        //        Graphic3d_ZLayerSettings aSettings = m_viewer->ZLayerSettings(anId);
        //        aSettings.SetEnableDepthTest (true);
        //        aSettings.SetEnableDepthWrite(true);
        //        aSettings.SetClearDepth (true);
        //        aSettings.SetPolygonOffset (Graphic3d_PolygonOffset());
        //        m_viewer->SetZLayerSettings (anId, aSettings);
        //@}

        // Create v3d view.
        // m_viewer->CreateView().get();
        Handle(FITKV3d_View) fitkViewHandle = new FITKV3d_View(m_viewer, m_viewer->DefaultTypeOfView());
        FITKV3d_View* fitkView = fitkViewHandle.get();
        connect(fitkView, &FITKV3d_View::sig_viewRedrawed, this, &Render::FITKGraph3DWindowOCC::slot_viewRedraw);
        m_view = fitkView;

        // Create position text label for sketch.
        m_coordLabel = new AIS_TextLabel;
        m_coordLabel->SetText("");
        m_coordLabel->SetFontAspect(Font_FontAspect::Font_FontAspect_Regular);
        m_coordLabel->SetZoomable(false);
        m_coordLabel->SetHeight(20.);
        m_coordLabel->SetZLayer(Graphic3d_ZLayerId_Topmost);
        m_coordLabel->SetColor(Quantity_Color(0., 0.3, 1., Quantity_TOC_RGB));
        m_context->Display(m_coordLabel, Standard_True);

        // Set projection type. 
        // Projection_Orthographic Projection_Perspective Projection_Stereo
        // m_view->Camera()->SetProjectionType(Graphic3d_Camera::Projection::Projection_Perspective);

#if OCC_VERSION_HEX >= 0x070200
        // Enable depth sorting, and the back face culling.
        //@{
        // Graphic3d_RTM_BLEND_UNORDERED Graphic3d_RTM_BLEND_OIT
        m_view->ChangeRenderingParams().TransparencyMethod = Graphic3d_RenderTransparentMethod::Graphic3d_RTM_BLEND_OIT;
        // m_view->SetBackFacingModel(V3d_TypeOfBackfacingModel::V3d_TOBM_NEVER_DISPLAYED);
        m_view->SetVisualization(V3d_TypeOfVisualization::V3d_ZBUFFER);      
        //@}
        //  m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, QColorToQuan(m_shapeColor), 0.08, V3d_ZBUFFER);
#endif

    // m_view->ChangeRenderingParams().Method = Graphic3d_RenderingMode::Graphic3d_RM_RAYTRACING;

        // Optimization.
        //@{
        if (m_optimization)
        {
            m_view->ChangeRenderingParams().IsAntialiasingEnabled = true;
            // m_view->ChangeRenderingParams().NbMsaaSamples = true;
            m_view->ChangeRenderingParams().RenderResolutionScale = 2;
            // m_view->ChangeRenderingParams().SamplesPerPixel = 3;
        }
        //@}

        // Color.
        //QColor bkgTopColor = QColor(250, 250, 255);
        //QColor bkgBtmColor = QColor(220, 230, 255);
        QColor bkgTopColor = QColor(50, 50, 100);
        QColor bkgBtmColor = QColor(170, 170, 190);
        m_view->SetBgGradientColors(QColorToQuan(bkgTopColor), QColorToQuan(bkgBtmColor), Aspect_GFM_VER);
        m_view->MustBeResized();

        // The container widget.
        //@{
#ifdef Q_OS_WIN
        Handle(Aspect_Window) windowHandle = new WNT_Window((Aspect_Handle)occBkg->winId());
#else
        Handle(Aspect_Window) windowHandle = new Xw_Window(conn, occBkg->winId());
#endif
        //@}

        m_occWindow = windowHandle.get();
        m_view->SetWindow(windowHandle);

        // Add view cube.
        //@{
        initViewCube();
        //@}

        // BETA
        // Create interactive object for editting model.
        //@{
        // initEditor();
        //@}

        // Initialize selection style.
        //@{
        initSelectionStyle();
        //@}

        // Initialize the tool bar.
        //@{
        initPopToolbar();
        //@}
        initSketch();
        // Initialize Qt widget.
        // MUST CALL THESE FUNCTIONS !!!
        //@{
        // setAttribute(Qt::WA_PaintOnScreen);
        // setAttribute(Qt::WA_NoSystemBackground);
        // setBackgroundRole(QPalette::NoRole);
        // setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
        //@}

        m_occBkgWidget->setViewer(m_viewer);
        m_occBkgWidget->setView(m_view);
        m_occBkgWidget->setContext(m_context);
        m_occBkgWidget->setController(m_controller);
        m_occBkgWidget->setTextLabel(m_coordLabel);

        // Initialize the selection mode flag.
        m_context->SetAutoActivateSelection(false);
        m_context->SetPixelTolerance(5 * devicePixelRatioF());

        // Initialize view z near.
        // m_view->StartZoomAtPoint(width() / 2, height() / 2);
        m_view->ZoomAtPoint(0, 0, -1, -1);

        // Initialize window foucs.    
        m_controller->UpdateMousePosition(Graphic3d_Vec2i(1., 1.), Aspect_VKeyMouse_NONE, Aspect_VKeyMouse_NONE, false);

        // Initialize select modes hash.
        clearSelectModes();

        // Disable multi select.
        enableShiftSelect(false);
        enableAreaSelect(false);
    }

    void FITKGraph3DWindowOCC::initViewCube()
    {
        // Text and color.
        //@{
        m_viewCube = new AIS_ViewCube;
        // m_viewCube->SetTransparency(0.1);
        m_viewCube->SetSize(60.);
        m_viewCube->SetBoxSideLabel(V3d_TypeOfOrientation::V3d_Xpos, "Left");
        m_viewCube->SetBoxSideLabel(V3d_TypeOfOrientation::V3d_Xneg, "Right");
        m_viewCube->SetBoxSideLabel(V3d_TypeOfOrientation::V3d_Ypos, "Top");
        m_viewCube->SetBoxSideLabel(V3d_TypeOfOrientation::V3d_Yneg, "Bottom");
        m_viewCube->SetBoxSideLabel(V3d_TypeOfOrientation::V3d_Zpos, "Front");
        m_viewCube->SetBoxSideLabel(V3d_TypeOfOrientation::V3d_Zneg, "Back");

        m_viewCube->SetMaterial(Graphic3d_NameOfMaterial::Graphic3d_NOM_STONE);
        m_viewCube->SetBoxColor(Quantity_NameOfColor::Quantity_NOC_WHITESMOKE);
        m_viewCube->SetTextColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
        m_viewCube->SetFontHeight(15);
        //@}

        // Edge color.
        //@{
        const Handle(Prs3d_Drawer)& cubeProperty = m_viewCube->Attributes();
        cubeProperty->SetupOwnFaceBoundaryAspect();
        cubeProperty->SetFaceBoundaryDraw(Standard_True);
        cubeProperty->FaceBoundaryAspect()->SetColor(QColorToQuan(QColor(50, 50, 50)));
        //@}

        // Axis.
        //@{
        Handle(Prs3d_DatumAspect) datumAspect = new Prs3d_DatumAspect;
        cubeProperty->SetDatumAspect(datumAspect);
        // m_viewCube->Attributes()->DatumAspect();

        datumAspect->ShadingAspect(Prs3d_DatumParts::Prs3d_DP_XAxis)->SetColor(Quantity_NameOfColor::Quantity_NOC_RED);
        datumAspect->ShadingAspect(Prs3d_DatumParts::Prs3d_DP_YAxis)->SetColor(Quantity_NameOfColor::Quantity_NOC_GREEN);
        datumAspect->ShadingAspect(Prs3d_DatumParts::Prs3d_DP_ZAxis)->SetColor(Quantity_NameOfColor::Quantity_NOC_BLUE1);

#if OCC_VERSION_HEX < 0x070600
        datumAspect->TextAspect()->SetColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
#else
        datumAspect->TextAspect(Prs3d_DatumParts::Prs3d_DP_XAxis)->SetColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
        datumAspect->TextAspect(Prs3d_DatumParts::Prs3d_DP_YAxis)->SetColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
        datumAspect->TextAspect(Prs3d_DatumParts::Prs3d_DP_ZAxis)->SetColor(Quantity_NameOfColor::Quantity_NOC_BLACK);
#endif
        //@}

        // Animation.
        m_viewCube->SetDuration(0.2); // second
        m_viewCube->SetResetCamera(false);
        m_viewCube->SetFitSelected(false);
        m_viewCube->SetViewAnimation(m_controller->ViewAnimation());
        m_viewCube->SetFixedAnimationLoop(false);
        m_viewCube->SetAutoStartAnimation(true);

        // Size and position.
        // m_viewCube->SetTransparency(0.1);
        Handle(Graphic3d_TransformPers) trans =
            new Graphic3d_TransformPers(
                Graphic3d_TransModeFlags::Graphic3d_TMF_TriedronPers,
                Aspect_TypeOfTriedronPosition::Aspect_TOTP_RIGHT_UPPER,
                Graphic3d_Vec2i(100, 100));
        m_viewCube->SetTransformPersistence(trans);

        m_context->Display(m_viewCube, Standard_True);
    }

    //void FITKGraph3DWindowOCC::initEditor()
    //{
    //    // Create editor.
    //    m_editor = new AIS_Manipulator;
    //    //    m_editor->SetPart(0, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);
    //    //    m_editor->SetPart(1, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
    //    m_editor->EnableMode(AIS_ManipulatorMode::AIS_MM_Translation);
    //    m_editor->EnableMode(AIS_ManipulatorMode::AIS_MM_Rotation);
    //    m_editor->EnableMode(AIS_ManipulatorMode::AIS_MM_Scaling);
    //    m_editor->SetModeActivationOnDetection(Standard_True);
    //    // m_context->Display(m_editor, Standard_False);
    //}

    void FITKGraph3DWindowOCC::initSelectionStyle()
    {
        // Set the preview selection style.
        //@{
        Handle(Prs3d_Drawer) highStyle = m_context->HighlightStyle();
        highStyle->SetColor(QColorToQuan(m_highlightColor));
        highStyle->SetTransparency(0.5f);
        highStyle->SetMethod(Aspect_TypeOfHighlightMethod::Aspect_TOHM_COLOR);
        highStyle->SetDisplayMode(1);
        highStyle->LineAspect()->SetWidth(3.);
        highStyle->WireAspect()->SetWidth(3.);
        highStyle->FaceBoundaryAspect()->SetWidth(3.);
        highStyle->SetZLayer(Graphic3d_ZLayerId_Topmost);
        m_context->SetHighlightStyle(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalDynamic, highStyle);
        //@}

        // Set the selection style.
        //@{
        Handle(Prs3d_Drawer) selectStyle = m_context->SelectionStyle();
        selectStyle->SetColor(QColorToQuan(m_selectionColor));
        selectStyle->SetTransparency(0.5f);
        selectStyle->SetMethod(Aspect_TypeOfHighlightMethod::Aspect_TOHM_COLOR);
        selectStyle->SetDisplayMode(1);
        selectStyle->LineAspect()->SetWidth(4.);
        selectStyle->WireAspect()->SetWidth(4.);
        selectStyle->FaceBoundaryAspect()->SetWidth(4.);
        selectStyle->SetZLayer(Graphic3d_ZLayerId_Topmost);
        m_context->SetHighlightStyle(Prs3d_TypeOfHighlight::Prs3d_TypeOfHighlight_LocalSelected, selectStyle);
        //@}
    }

    void FITKGraph3DWindowOCC::initPopToolbar()
    {
        // The toolbar's parent widget.
        QWidget* toolParent = this;
        m_viewDireActs.clear();

        // The grid layout for the toolbar. ( and the occ window )
        QGridLayout* gridLayout = dynamic_cast<QGridLayout*>(toolParent->layout());
        if (!gridLayout)
        {
            gridLayout = new QGridLayout(toolParent);
            gridLayout->setContentsMargins(0, 0, 0, 0);
        }

        // The actions'toolbar.
        QToolBar* toolbar = new QToolBar(toolParent);
        toolbar->setContentsMargins(0, 0, 0, 0);
        toolbar->setOrientation(Qt::Vertical);
        toolbar->setIconSize(QSize(40, 40));

        // Create actions.
        //@{
        // Front View.
        QAction* viewAct = toolbar->addAction(QIcon(":res/icons_toolbar/viewFront.svg"), tr("Front View"), [&]
        {
            setView(Core::FITKGraph3DStandardView::F3DVZPos);
        });

        m_viewDireActs.push_back(viewAct);

        // Back View.
        viewAct = toolbar->addAction(QIcon(":res/icons_toolbar/viewBack.svg"), tr("Back View"), [&]
        {
            setView(Core::FITKGraph3DStandardView::F3DVZNeg);
        });

        m_viewDireActs.push_back(viewAct);

        // Top View.
        viewAct = toolbar->addAction(QIcon(":res/icons_toolbar/viewTop.svg"), tr("Top View"), [&]
        {
            setView(Core::FITKGraph3DStandardView::F3DVYPos);
        });

        m_viewDireActs.push_back(viewAct);

        // Bottom View.
        viewAct = toolbar->addAction(QIcon(":res/icons_toolbar/viewBottom.svg"), tr("Bottom View"), [&]
        {
            setView(Core::FITKGraph3DStandardView::F3DVYNeg);
        });

        m_viewDireActs.push_back(viewAct);

        // Left View.
        viewAct = toolbar->addAction(QIcon(":res/icons_toolbar/viewLeft.svg"), tr("Left View"), [&]
        {
            setView(Core::FITKGraph3DStandardView::F3DVXPos);
        });

        m_viewDireActs.push_back(viewAct);

        // Right View.
        viewAct = toolbar->addAction(QIcon(":res/icons_toolbar/viewRight.svg"), tr("Right View"), [&]
        {
            setView(Core::FITKGraph3DStandardView::F3DVXNeg);
        });

        m_viewDireActs.push_back(viewAct);

        // Iso View.
        viewAct = toolbar->addAction(QIcon(":res/icons_toolbar/viewIso.svg"), tr("Iso View"), [&]
        {
            setView(Core::FITKGraph3DStandardView::F3DVISO);
        });

        m_viewDireActs.push_back(viewAct);

        // Fit All.
        toolbar->addAction(QIcon(":res/icons_toolbar/viewFit.svg"), tr("Fit All"), [&]
        {
            fitView();
        });

        // Save Image.
        toolbar->addAction(QIcon(":/res/icons_toolbar/saveImage.png"), tr("Save Image"), [&]
        {
            // GIF(Graphical Interchange Format)
            QString filePath = QFileDialog::getSaveFileName(nullptr, tr("Save Image"), "",
                "All(*.png;*.bmp;*.ppm;*.jpg;*.jpe;*.jpeg;*.tif;*.tiff;*.exr;*.tga);;"
                "PNG(Portable Network Graphics)(*.png);;"
                "BMP(Bitmap)(*.bmp);;"
                "PPM(Portable Pixmap Format)(*.ppm);;"
                "JPEG/JIFF(Joint Photographic Experts Group)(*.jpg;*.jpe;*.jpeg);;"
                "TIFF(Tagged Image File Format)(*.tif;*.tiff);;"
                "OpenEXR high dynamic-range(*.exr);;"
                "TGA(Truevision Targa Graphic)(*.tga)", nullptr,
                QFileDialog::DontUseNativeDialog);
            if (filePath.isEmpty())
            {
                return;
            }

            saveImage(filePath);
        });

        // Parallel View.
        toolbar->addAction(QIcon(":res/icons_toolbar/viewParallel.svg"), tr("Parallel View"), [&]
        {
            setParallelProjection(true);
        });

        // Perspective View.
        toolbar->addAction(QIcon(":res/icons_toolbar/viewPerspective.svg"), tr("Perspective View"), [&]
        {
            setParallelProjection(false);
        });

        //// Grid Lines.
        //m_actionGridLines = toolbar->addAction(QIcon(":res/icons_toolbar/gridLines.svg"), tr("Show/Hide Grid"), [=]
        //{
        //    if (m_actionGridLines)
        //    {
        //        enableGridLines(m_actionGridLines->isChecked());
        //    }
        //});

        // m_actionGridLines->setCheckable(true);

        // Gray background color, dark gray broder.
        toolbar->setStyleSheet("background-color: rgb(230, 230, 230); border: 1px solid rgb(120, 120, 120);");

        // Change the style sheet of the actions.
        for (QToolButton* tBtn : toolbar->findChildren<QToolButton*>())
        {
            if (tBtn)
            {
                tBtn->setStyleSheet("border: none;");
            }
        }

        // Left center.
        gridLayout->addWidget(toolbar, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

        toolbar->setMouseTracking(true);
        toolbar->setAttribute(Qt::WA_TransparentForMouseEvents);

        m_toolbar = toolbar;
    }

    void FITKGraph3DWindowOCC::initSketch()
    {
        if (m_context == nullptr)  throw "m_context is empty !";
        Handle(FITKAIS_SketchShape) sk = new FITKAIS_SketchShape(m_context);
        m_sketchShapeContainer.append(sk);
        m_sketchShape = sk.get();
        connect(this, &FITKGraph3DWindowOCC::sig_pickUpdateSketch, m_sketchShape, &FITKAIS_SketchShape::slot_updateSelectdObject, Qt::UniqueConnection);
    }

    bool FITKGraph3DWindowOCC::enableSketchMode(bool isOn, double* pos, double* dir, double* up, bool circle)
    {
        if (!m_viewer) return false;
        if (!m_controller) return false;
        if (m_sketchShape == nullptr) return false;
        if (isOn && (pos == nullptr || dir == nullptr || up == nullptr)) return false;
        // Enable or disable changing view direction by GUI interactions.
        setViewChangeEnabled(!isOn);

        if (isOn)
        {
            // Show position label.
            m_context->Display(m_coordLabel, Standard_True);

            // Show grid lines.
            m_viewer->ActivateGrid(circle ? Aspect_GridType::Aspect_GT_Circular : Aspect_GridType::Aspect_GT_Rectangular
                , Aspect_GridDrawMode::Aspect_GDM_Lines);
            m_controller->ChangeMouseGestureMap().UnBind(Aspect_VKeyMouse_LeftButton);
            m_controller->ChangeMouseGestureMap().UnBind(Aspect_VKeyMouse_RightButton | Aspect_VKeyFlags_CTRL);
        }
        else
        {
            // Hide position label.
            m_context->Erase(m_coordLabel, Standard_True);

            // Hide grid lines.
            m_viewer->DeactivateGrid();
            m_controller->ChangeMouseGestureMap().Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_RotateOrbit);
            m_controller->ChangeMouseGestureMap().Bind(Aspect_VKeyMouse_RightButton | Aspect_VKeyFlags_CTRL, AIS_MouseGesture_RotateOrbit);
            enableSketch(false);
            return true;
        }

        gp_Dir pDir(dir[0], dir[1], dir[2]);
        // pDir.Reverse();
        gp_Ax3 ax3(gp_Pnt(pos[0], pos[1], pos[2]), pDir, gp_Dir(up[0], up[1], up[2]));
        gp_Pln pln(ax3);
        m_viewer->SetPrivilegedPlane(ax3);
        m_view->Camera()->SetDirection(pln.Position().Direction());
        m_view->Camera()->SetUp(pln.Position().YDirection());

        if (m_view)
        {
            m_view->Update();
        }
        return true;
    }

    void FITKGraph3DWindowOCC::enableGridLines(bool isOn, bool circle)
    {
        if (!m_viewer)
        {
            return;
        }

        // Old version.
        //if (isOn)
        //{
        //    // Show grid lines.
        //    m_viewer->ActivateGrid(circle ? Aspect_GridType::Aspect_GT_Circular : Aspect_GridType::Aspect_GT_Rectangular
        //        , Aspect_GridDrawMode::Aspect_GDM_Lines);

        //    // Hit point aspects.
        //    Handle(Graphic3d_AspectMarker3d) hitAsp = new Graphic3d_AspectMarker3d(Aspect_TypeOfMarker::Aspect_TOM_STAR, 
        //        Quantity_NameOfColor::Quantity_NOC_DODGERBLUE1, 3.);
        //    m_viewer->SetGridEcho(hitAsp);
        //}
        //else
        //{
        //    // Hide grid lines.
        //    m_viewer->DeactivateGrid();
        //}

        // New version.
        if (isOn)
        {
            // Show grid lines.
            m_viewer->EnableCustomGrid();
        }
        else
        {
            // Hide grid lines.
            m_viewer->DisbaleCustomGrid();
        }

        auto dir = m_view->Camera()->Direction();
        auto xdir = m_view->Camera()->Up();
        gp_Ax3 ax3(gp_Pnt(), dir, xdir);
        m_viewer->SetPrivilegedPlane(ax3);

        // Update the view.
        if (m_view)
        {
            m_view->Update();
        }
    }

    void FITKGraph3DWindowOCC::enableSketch(bool isOn, CommonOCC::SketchShapeType t, bool circle)
    {
        if (m_sketchShape == nullptr || !m_viewer->IsActive()) return;
        //该信号始终连接
        connect(m_sketchShape, &FITKAIS_SketchShape::sig_parametersSatisfy, this, &FITKGraph3DWindowOCC::sig_parametersSatisfy, Qt::ConnectionType::UniqueConnection);
        m_sketchShape->setDrawStatus(isOn);
        m_sketchShape->setShapeType(t);
        //结束草绘后移除草绘对象
        if (!isOn) m_context->Remove(m_sketchShape, true);//移除显示对象
        m_controller->EnableSelectInView(isOn);

        // 显示添加坐标对话框。
        if (isOn)
        {
            if (!m_widgetAddPos)
            {
                m_widgetAddPos = new GUI::WidgetAddSketchPos(this);
                m_widgetAddPos->setRenderWindow(this);
                m_widgetAddPos->setAttribute(Qt::WA_DeleteOnClose);
                m_widgetAddPos->setWindowFlags(m_widgetAddPos->windowFlags() & ~Qt::WindowContextHelpButtonHint
                    & ~Qt::WindowCloseButtonHint & ~Qt::WindowMinMaxButtonsHint);
                connect(m_widgetAddPos, &QObject::destroyed, this, [&] { m_widgetAddPos = nullptr; });
            }
            
            if (!m_widgetAddPos->isVisible())
            {
                m_widgetAddPos->show();
                m_widgetAddPos->move(mapToGlobal(this->pos()));
            }
        }
        else
        {
            if (m_widgetAddPos)
            {
                m_widgetAddPos->setAttribute(Qt::WA_DeleteOnClose, false);
                m_widgetAddPos->close();
                delete m_widgetAddPos;
                m_widgetAddPos = nullptr;
            }
        }
    }

    const QList<double*> FITKGraph3DWindowOCC::getSketchPoint()
    {
        if (m_sketchShape == nullptr) return QList<double*>();
        return m_sketchShape->getPoint();
    }

    void FITKGraph3DWindowOCC::getSketchPln(double location[3], double direction[3], double yDirection[3])
    {
        auto pln = m_viewer->PrivilegedPlane();
        location[0] = pln.Location().X();
        location[1] = pln.Location().Y();
        location[2] = pln.Location().Z();
        direction[0] = pln.Direction().X();
        direction[1] = pln.Direction().Y();
        direction[2] = pln.Direction().Z();
        yDirection[0] = pln.YDirection().X();
        yDirection[1] = pln.YDirection().Y();
        yDirection[2] = pln.YDirection().Z();
    }

    void FITKGraph3DWindowOCC::addSketchPosition(double x, double y)
    {
        const gp_Ax3 & pln = m_viewer->PrivilegedPlane();
        const gp_Pnt & org = pln.Location();

        const gp_Dir & xDir = pln.XDirection();
        const gp_Dir & yDir = pln.YDirection();

        gp_Vec xVec = x * gp_Vec(xDir);
        gp_Vec yVec = y * gp_Vec(yDir);

        gp_Pnt pt3d = org.Translated(xVec + yVec);

        if (m_sketchShape->drawStatus())
        {
            m_sketchShape->replaceLast(pt3d);
            m_sketchShape->update();
            m_sketchShape->setInitParameter(pt3d, pln.Direction(), xDir);
            m_sketchShape->addPoint(pt3d);
        }
    }

    bool FITKGraph3DWindowOCC::getSketchStatus()
    {
        return m_sketchShape->drawStatus();
    }

    void FITKGraph3DWindowOCC::setParallelProjection(bool isOn)
    {
        if (m_view)
        {
            // Set the projection of the camera.
            Handle(Graphic3d_Camera) camera = m_view->Camera();
            if (!camera.IsNull())
            {
                camera->SetProjectionType(isOn ? Graphic3d_Camera::Projection_Orthographic : Graphic3d_Camera::Projection_Perspective);
                camera->InvalidateProjection();
            }

            // Update the view.
            m_view->Update();
        }
    }

    void FITKGraph3DWindowOCC::setBackgroundColor(float* topColor, float* btmColor)
    {
        if (!m_view)
        {
            return;
        }

        QColor top, bottom;
        if (topColor)
        {
            top = QColor::fromRgbF(topColor[0], topColor[1], topColor[2]);
        }

        if (btmColor)
        {
            bottom = QColor::fromRgbF(btmColor[0], btmColor[1], btmColor[2]);
        }
        else
        {
            bottom = top;
        }

        m_view->SetBgGradientColors(QColorToQuan(top), QColorToQuan(bottom), Aspect_GFM_VER);
    }

    void FITKGraph3DWindowOCC::getBackgroundColor(float* topColor, float* btmColor)
    {
        if (!m_view)
        {
            return;
        }

        Quantity_Color top, bottom;
        m_view->GradientBackgroundColors(top, bottom);

        if (topColor)
        {
            topColor[0] = top.Red();
            topColor[1] = top.Green();
            topColor[2] = top.Blue();
        }

        if (btmColor)
        {
            btmColor[0] = bottom.Red();
            btmColor[1] = bottom.Green();
            btmColor[2] = bottom.Blue();
        }
    }

    void FITKGraph3DWindowOCC::setView(Core::FITKGraph3DStandardView viewDirec)
    {
        // Check if the view direction has been disabled.
        if (!m_enableViewChange)
        {
            return;
        }

        setView(viewDirec, true);
    }

    void FITKGraph3DWindowOCC::setView(Core::FITKGraph3DStandardView viewDirec, bool useAnime)
    {
        // Check the view( camera ).
        if (!m_view)
        {
            return;
        }

        V3d_TypeOfOrientation orientType;

        switch (viewDirec)
        {
        case Core::F3DVXPos:
            orientType = V3d_TypeOfOrientation::V3d_Xpos;
            break;
        case Core::F3DVXNeg:
            orientType = V3d_TypeOfOrientation::V3d_Xneg;
            break;
        case Core::F3DVYPos:
            orientType = V3d_TypeOfOrientation::V3d_Ypos;
            break;
        case Core::F3DVYNeg:
            orientType = V3d_TypeOfOrientation::V3d_Yneg;
            break;
        case Core::F3DVZPos:
            orientType = V3d_TypeOfOrientation::V3d_Zpos;
            break;
        case Core::F3DVZNeg:
            orientType = V3d_TypeOfOrientation::V3d_Zneg;
            break;
        case Core::F3DVISO:
            orientType = V3d_TypeOfOrientation::V3d_XposYposZpos;
            break;
        default:
            return;
        }

        // Set the view type.
        if (m_viewCube && useAnime)
        {
            Handle(AIS_ViewCubeOwner) cubeOwner = new AIS_ViewCubeOwner(m_viewCube, orientType);
            m_viewCube->StartAnimation(cubeOwner);
            m_viewCube->UpdateAnimation(true);
        }
        else
        {
            m_view->SetProj(orientType);
        }

        // Reset camera.
        // fitView();
    }

    bool FITKGraph3DWindowOCC::addObject(FITKGraphObjectOCC* obj, bool fitView)
    {
        if (!obj || !m_context)
        {
            return false;
        }

        // If the graph object has been added to the graph widget already,
        // return true.
        if (m_objects.contains(obj))
        {
            return true;
        }

        // Check if the window has model or mesh already.
        bool hasObjBefore = false;
        bool hasObjAfter = false;
        for (FITKGraphObjectOCC* iterObj : m_objects)
        {
            if (!iterObj)
            {
                continue;
            }

            if (iterObj->hasFixedSize())
            {
                hasObjBefore = true;
                break;
            }
        }

        // Add to object list.
        m_objects.push_back(obj);

        // Save the interactive context and view.
        obj->setInteractiveContext(m_context);
        obj->setView(m_view);   

        // Get the interactive objects from graph object.
        int nInter = obj->getNumberOfInteractiveObjs();

        // Get the select modes.
        QList<int> selectShapeModes = getActiveShapeSelectModes();
        // QList<int> selectMeshModes = getActiveMeshSelectModes();

        // Add to display widget.
        for (int i = 0; i < nInter; i++)
        {
            Handle(AIS_InteractiveObject) interactiveObj = obj->getInteractiveObjAt(i);
            if (!interactiveObj)
            {
                continue;
            }

            // Deactive the select mode first.
            m_context->Display(interactiveObj, Standard_False);
            m_context->Deactivate(interactiveObj);

            // Update MeshVS_Mesh selectablity.
            Handle(MeshVS_Mesh) meshVS = Handle(MeshVS_Mesh)::DownCast(interactiveObj);
            if (meshVS && !meshVS.IsNull())
            {
                // Try to active node select.
                if (m_selectModeHash[CommonOCC::S_MeshNode])
                {
                    m_context->Display(interactiveObj, interactiveObj->DisplayMode(), MeshVS_SMF_Node, Standard_False);
                }

                // Try to active element select.
                if (m_selectModeHash[CommonOCC::S_MeshElement])
                {
                    m_context->Display(interactiveObj, interactiveObj->DisplayMode(), MeshVS_SMF_Element, Standard_False);
                }
            }

            // Update AIS_Shape selectablity.
            Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(interactiveObj);
            if (aisShape)
            {
                for (const int & selectMode : selectShapeModes)
                {
                    // Set selectability for shape.
                    m_context->Display(interactiveObj, interactiveObj->DisplayMode(), selectMode, Standard_False);
                }
            }

            // Datum.
            if (interactiveObj->Type() == AIS_KindOfInteractive::AIS_KOI_Datum)
            {
                for (const int & selectMode : selectShapeModes)
                {
                    // Set selectability for shape.
                    m_context->Display(interactiveObj, interactiveObj->DisplayMode(), selectMode, Standard_False);
                }
            }

            // Tools.
            if (interactiveObj->Type() == AIS_KindOfInteractive::AIS_KOI_Dimension)
            {
                m_context->Display(interactiveObj, Standard_False);
            }  
        }

        // Erase or keep the objects.
        bool visible = obj->getDataVisibility();
        if (!visible)
        {
            for (int i = 0; i < nInter; i++)
            {
                Handle(AIS_InteractiveObject) interactiveObj = obj->getInteractiveObjAt(i);
                if (!interactiveObj)
                {
                    continue;
                }

                m_context->Erase(interactiveObj, Standard_False);
            }
        }

        for (FITKGraphObjectOCC* iterObj : m_objects)
        {
            if (!iterObj)
            {
                continue;
            }

            if (iterObj->hasFixedSize())
            {
                hasObjAfter = true;
                break;
            }
        }

        if (fitView)
        {
            FITKGraph3DWindowOCC::fitView();
        }
        else
        {
            if (!hasObjBefore && hasObjAfter)
            {
                FITKGraph3DWindowOCC::fitView();
            }
            else
            {
                FITKGraph3DWindowOCC::reRender();
            }
        }

        return true;
    }

    void FITKGraph3DWindowOCC::removeGraphObj(Core::FITKAbstractGraphObject* obj)
    {
        FITKGraphObjectOCC* occObj = FITKGraphObjectOCC::SafeDownCast(obj);
        if (!occObj || !m_context)
        {
            return;
        }

        // Get the interactive objects from graph object.
        int nInter = occObj->getNumberOfInteractiveObjs();

        // Erase all interactive objects.
        for (int i = 0; i < nInter; i++)
        {
            opencascade::handle<AIS_InteractiveObject> interactiveObj = occObj->getInteractiveObjAt(i);
            if (!interactiveObj)
            {
                continue;
            }

            m_context->Remove(interactiveObj, Standard_True);
        }

        // Reset the interactive context.
        occObj->setInteractiveContext(nullptr);

        // Remove from the object list.
        m_objects.removeOne(occObj);
    }

    void FITKGraph3DWindowOCC::updateObjsByMouseMove(int mx, int my)
    {
        // Update all objects.
        for (FITKGraphObjectOCC* obj : m_objects)
        {
            obj->updateByMouseMove(mx, my);
        }
    }

    void FITKGraph3DWindowOCC::updateObjsByMousePick(int mx, int my)
    {
        // Update all objects.
        for (FITKGraphObjectOCC* obj : m_objects)
        {
            obj->updateByMousePick(mx, my);
        }
    }

    void FITKGraph3DWindowOCC::updateAllShape(int selectMode, bool update)
    {
        if (!m_context)
        {
            return;
        }

        // Get all interactive objects.
        AIS_ListOfInteractive objs;
        m_context->DisplayedObjects(objs);

        // Update interactive objects selectablity.
        AIS_ListIteratorOfListOfInteractive iter;
        for (iter.Initialize(objs); iter.More(); iter.Next())
        {
            Handle(AIS_InteractiveObject) aisObj = iter.Value();

            // The view cube.
            if (aisObj == m_viewCube)
            {
                continue;
            }

            // Update AIS_Shape selectablity.
            Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(aisObj);
            if (aisShape || aisObj->Type() == AIS_KindOfInteractive::AIS_KOI_Datum)
            {
                m_context->Deactivate(aisShape, -1);

                switch (selectMode)
                {
                case CommonOCC::S_ShapeVertex:
                case CommonOCC::S_ShapeEdge:
                case CommonOCC::S_ShapeFace:
                case CommonOCC::S_ShapeWire:
                case CommonOCC::S_ShapeShell:
                case CommonOCC::S_ShapeSolid:
                case CommonOCC::S_ShapeCompound:
                case CommonOCC::S_ShapeShape:
                {
                    // Set selectability for shape.
                    m_context->Display(aisObj, aisObj->DisplayMode(), selectMode, update);

                    // Save select modes.
                    m_selectModeHash[selectMode] = true;

                    break;
                }
                //case S_ShapeEdgeAndWire:
                //{
                //    // Set selectability for shape.
                //    m_context->Display(aisObj, aisObj->DisplayMode(), S_ShapeEdge, update);
                //    m_context->Display(aisObj, aisObj->DisplayMode(), S_ShapeWire, update);

                //    // Save select modes.
                //    m_selectModeHash[S_ShapeEdge] = true;
                //    m_selectModeHash[S_ShapeWire] = true;

                //    break;
                //}
                default:
                {
                    // Deactive the select mode.
                    m_context->Display(aisObj, aisObj->DisplayMode(), -1, update);
                    break;
                }
                }
            }
        }
    }

    void FITKGraph3DWindowOCC::updateAllMesh(int selectMode, bool update)
    {
        if (!m_context)
        {
            return;
        }

        // Get all interactive objects.
        AIS_ListOfInteractive objs;
        m_context->DisplayedObjects(objs);

        // Update interactive objects selectablity.
        AIS_ListIteratorOfListOfInteractive iter;
        for (iter.Initialize(objs); iter.More(); iter.Next())
        {
            // The view cube.
            if (iter.Value() == m_viewCube)
            {
                continue;
            }

            // Update MeshVS_Mesh selectablity.
            Handle(MeshVS_Mesh) meshVS = Handle(MeshVS_Mesh)::DownCast(iter.Value());
            if (meshVS && !meshVS.IsNull())
            {
                m_context->Deactivate(meshVS, -1);

                switch (selectMode)
                {
                case CommonOCC::S_MeshNode:
                {
                    // Update selectability for mesh node.
                    m_context->Display(meshVS, meshVS->DisplayMode(), MeshVS_SMF_Node, update);

                    // Save select modes.
                    m_selectModeHash[selectMode] = true;

                    break;
                }
                case CommonOCC::S_MeshElement:
                {
                    // Update selectability for mesh element.
                    m_context->Display(meshVS, meshVS->DisplayMode(), MeshVS_SMF_Element, update);

                    // Save select modes.
                    m_selectModeHash[selectMode] = true;

                    break;
                }
                default:
                {
                    // Deactive the select mode.
                    m_context->Display(meshVS, meshVS->DisplayMode(), -1, update);
                    break;
                }
                }
            }
        }
    }

    void FITKGraph3DWindowOCC::deactiveAllInteractive(bool update)
    {
        Q_UNUSED(update);

        if (!m_context)
        {
            return;
        }

        // Get all interactive objects.
        AIS_ListOfInteractive objs;
        m_context->DisplayedObjects(objs);

        // Update interactive objects selectablity.
        AIS_ListIteratorOfListOfInteractive iter;
        for (iter.Initialize(objs); iter.More(); iter.Next())
        {
            // The view cube.
            if (iter.Value() == m_viewCube)
            {
                continue;
            }

            m_context->Deactivate(iter.Value());
        }
    }

    //void FITKGraph3DWindowOCC::setSelectMode(SelectMode mode, bool shiftSelect)
    //{
    //    if (!m_context || !m_controller)
    //    {
    //        return;
    //    }

    //    // Deactive last view mode.
    //    // m_context->Deactivate(m_selectMode);

    //    m_shitSelect = shiftSelect;
    //    clearSelectModes();
    //    enableAreaSelect(shiftSelect);

    //    // Clear last selection.
    //    clearSelection();

    //    switch (mode)
    //    {
    //    case S_ShapeVertex:
    //    case S_ShapeEdge:
    //    case S_ShapeFace:
    //    case S_ShapeWire:
    //    case S_ShapeShell:
    //    case S_ShapeSolid:
    //    // case S_ShapeEdgeAndWire:
    //    {
    //        // Update selectability for shape.
    //        updateAllShape(mode);
    //        m_controller->EnableSelectInView(true);
    //        break;
    //    }
    //    case S_MeshNode:
    //    {
    //        // Update selectability for mesh node.
    //        updateAllMesh(MeshVS_SMF_Node);
    //        m_controller->EnableSelectInView(true);
    //        break;
    //    }
    //    case S_MeshElement:
    //    {
    //        // Update selectability for mesh element.
    //        updateAllMesh(MeshVS_SMF_Element);
    //        m_controller->EnableSelectInView(true);
    //        break;
    //    }
    //    case S_None:
    //    default:
    //    {
    //        // Disable all select mode.( except the view cube )
    //        deactiveAllInteractive();
    //        m_controller->EnableSelectInView(false);
    //        return;
    //    }
    //    }

    //    // Reset selected shapes.
    //    m_context->InitSelected();
    //}

    void FITKGraph3DWindowOCC::enableSelect(bool isOn)
    {
        if (m_controller)
        {
            m_controller->EnableSelectInView(isOn);
        }
    }

    void FITKGraph3DWindowOCC::enableShiftSelect(bool isOn)
    {
        m_enableMultiSelect = isOn;

        //if (!m_controller)
        //{
        //    return;
        //}

        //if (isOn)
        //{
        //    m_controller->ChangeMouseGestureMap().Bind(Aspect_VKeyMouse_LeftButton | Aspect_VKeyFlags_SHIFT, AIS_MouseGesture_Pan);
        //}
        //else
        //{
        //    m_controller->ChangeMouseGestureMap().UnBind(Aspect_VKeyMouse_LeftButton | Aspect_VKeyFlags_SHIFT);
        //}
    }

    void FITKGraph3DWindowOCC::enableAreaSelect(bool isOn)
    {
        if (!m_controller)
        {
            return;
        }

        if (isOn)
        {
            m_controller->ChangeMouseGestureMap().Bind(Aspect_VKeyMouse_LeftButton | Aspect_VKeyFlags_ALT, AIS_MouseGesture_SelectRectangle);
        }
        else
        {
            m_controller->ChangeMouseGestureMap().UnBind(Aspect_VKeyMouse_LeftButton | Aspect_VKeyFlags_ALT);
        }
    }

    bool FITKGraph3DWindowOCC::getSelectedShapes(QVector<TopoDS_Shape> &shapes)
    {
        shapes.clear();
        if (!m_context)
        {
            return false;
        }

        m_context->InitSelected();
        while (m_context->MoreSelected())
        {
            TopoDS_Shape shape = m_context->SelectedShape();
            if (!shape.IsNull())
            {
                shapes.push_back(m_context->SelectedShape());
            }
            
            m_context->NextSelected();
        }

        return true;
    }

    bool FITKGraph3DWindowOCC::getFirstSelectedShape(TopoDS_Shape &shape)
    {
        shape.Nullify();

        if (!m_context)
        {
            return false;
        }

        m_context->InitSelected();
        shape = m_context->SelectedShape();
        return !shape.IsNull();
    }

    void FITKGraph3DWindowOCC::clearSelection()
    {
        if (!m_context)
        {
            return;
        }

        m_context->ClearSelected(true);
        m_context->InitSelected();
    }

    bool FITKGraph3DWindowOCC::hasSelection()
    {
        if (!m_context)
        {
            return false;
        }

        m_context->InitSelected();
        return m_context->HasSelectedShape();
    }

    void FITKGraph3DWindowOCC::removeSelectedShape(const TopoDS_Shape & shape)
    {
        for (m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected())
        {
            // Find the selected shape's owner.
            const TopoDS_Shape & s = m_context->SelectedShape();
            if (shape == s)
            {
                Handle(SelectMgr_EntityOwner) own = m_context->SelectedOwner();
                m_context->AddOrRemoveSelected(own, true);

                // Re-emit the pick data signal.
                emit sig_picked();

                break;
            }       
        }
    }

    void FITKGraph3DWindowOCC::setViewChangeEnabled(bool isOn)
    {
        // Enable or disable the tool buttons.
        for (QAction* act : m_viewDireActs)
        {
            if (act)
            {
                act->setEnabled(isOn);
            }
        }

        // Enable or disable the view cube.
        if (m_viewCube && m_context)
        {
            if (isOn)
            {
                // Enable shape select mode by default.
                m_context->Activate(m_viewCube, AIS_Shape::SelectionMode(TopAbs_ShapeEnum::TopAbs_SHAPE));
            }
            else
            {
                m_context->Deactivate(m_viewCube);
            }
        }

        // Save the flag.
        m_enableViewChange = isOn;
    }

    //void FITKGraph3DWindowOCC::setEditMode(bool isOn)
    //{
    //    if (m_isEditing == isOn)
    //    {
    //        return;
    //    }

    //    m_isEditing = isOn;

    //    //    if(!m_editor.IsNull())
    //    //    {
    //    //        m_context->Erase(m_editor, true);
    //    //    }

    //    if (m_isEditing)
    //    {
    //        m_context->InitSelected();
    //        Handle(AIS_InteractiveObject) aisShape = m_context->SelectedInteractive();
    //        if (aisShape.IsNull())
    //        {
    //            if (m_editor)
    //            {
    //                if (m_editor->IsAttached())
    //                {
    //                    m_editor->Detach();
    //                }

    //                m_context->Erase(m_editor, true);
    //            }

    //            m_isEditing = false;
    //            return;
    //        }

    //        initEditor();
    //        m_editor->Attach(aisShape);

    //        // Load the transform from the object attached
    //        if (!aisShape.IsNull())
    //        {
    //            gp_Trsf trans = aisShape->Transformation();
    //            gp_Ax2 axis = m_editor->Position();
    //            axis.SetXDirection(trans.VectorialPart().Column(1));
    //            axis.SetYDirection(trans.VectorialPart().Column(2));
    //            axis.SetDirection(trans.VectorialPart().Column(3));
    //            m_editor->SetPosition(axis);
    //        }

    //        m_context->Display(m_editor, true);
    //    }
    //    else
    //    {
    //        Handle(AIS_InteractiveObject) aisObj = m_editor->Object();
    //        if (!aisObj.IsNull())
    //        {
    //            Handle(Geom_Transformation) trans = m_editor->TransformationGeom();

    //            //            gp_Trsf trans = m_editor->Transformation();
    //            Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(aisObj);
    //            TopoDS_Shape shape = aisShape->Shape();
    //            TopLoc_Location loc = shape.Location();

    //            shape = shape.Located(TopLoc_Location(trans->Trsf()) * loc);
    //            aisShape->SetShape(shape);
    //            //            m_context->Display(aisShape, aisShape->DisplayMode(), m_selectMode, true);
    //        }

    //        m_editor->Detach();
    //        m_context->Erase(m_editor, true);
    //    }
    //}

    void FITKGraph3DWindowOCC::fitView()
    {
        if (m_view)
        {
            m_view->FitAll();
        }
    }

    void FITKGraph3DWindowOCC::fitSelected()
    {
        if (!m_view || !m_context)
        {
            return;
        }

        // Get the bound box of the selection.
        Bnd_Box box;
        for (m_context->InitSelected(); m_context->MoreSelected(); m_context->NextSelected())
        {
            const TopoDS_Shape & s = m_context->SelectedShape();  
            BRepBndLib::Add(s, box, false);
        }

        m_view->FitAll(box);
    }

    void FITKGraph3DWindowOCC::reRender()
    {
        if (m_context)
        {
            m_context->UpdateCurrentViewer();
        }
    }

    void FITKGraph3DWindowOCC::clear()
    {
        if (m_context)
        {
            m_context->RemoveAll(true);
        }

        for (FITKGraphObjectOCC* obj : m_objects)
        {
            obj->removeFromGraphWidget();
        }

        m_objects.clear();
    }

    void FITKGraph3DWindowOCC::saveImage(const QString& image, int w, int h)
    {
        if (!m_view || image.isEmpty())
        {
            return;
        }

        // Get the window size.
        int winW = 0;
        int winH = 0;
        m_view->Window()->Size(winW, winH);

        // Get the pixmap of the view.
        Image_AlienPixMap pixmap;
        V3d_ImageDumpOptions params;
        params.Width = w <= 0 ? winW : w;
        params.Height = h <= 0 ? winH : h;

        bool flag = m_view->ToPixMap(pixmap, params);

        if (flag)
        {
            TCollection_AsciiString tStr((const char*)image.toStdString().c_str());
            pixmap.Save(tStr);
        }
    }

    AIS_InteractiveContext* FITKGraph3DWindowOCC::getContext()
    {
        return m_context;
    }

    QPaintEngine* FITKGraph3DWindowOCC::paintEngine() const
    {
        return nullptr;
    }

    void FITKGraph3DWindowOCC::paintEvent(QPaintEvent* event)
    {
        if (m_occBkgWidget)
        {
            m_occBkgWidget->update();
        }

        Core::FITKAbstractGraph3DWidget::paintEvent(event);
    }

    Quantity_Color FITKGraph3DWindowOCC::QColorToQuan(QColor color)
    {
        Quantity_Color quanColor;
        quanColor.SetValues(color.redF(), color.greenF(), color.blueF(), Quantity_TOC_RGB);
        return quanColor;
    }

    void FITKGraph3DWindowOCC::slot_viewRedraw()
    {
        // Update the internal widget of aspect window.
        if (m_occBkgWidget)
        {
            m_occBkgWidget->update();
        }
    }

    void FITKGraph3DWindowOCC::slot_mouseSelected()
    {
        // Initialize the selected.
        m_context->InitSelected();

        // Update objects.
        const Graphic3d_Vec2i & pos2d = m_controller->LastMousePosition();
        updateObjsByMousePick(pos2d.x(), pos2d.y());

        // For sketch.
        emit sig_pickUpdateSketch();

        // For pick data.
        emit sig_picked();

        //// Check the selection.
        //if (!getActiveSelectModes().isEmpty())
        //{
        //    m_context->InitSelected();

        //    if (m_context->HasSelectedShape())
        //    {
        //        emit sig_picked();
        //    }
        //}
    }

    QList<int> FITKGraph3DWindowOCC::getActiveShapeSelectModes()
    {
        // Get the select modes.
        QList<int> modes;
        for (int mode = CommonOCC::S_ShapeBaseModeStart; mode <= CommonOCC::S_ShapeBaseModeEnd; mode++)
        {
            if (m_selectModeHash[mode])
            {
                modes.push_back(mode);
            }
        }

        return modes;
    }

    QList<int> FITKGraph3DWindowOCC::getActiveMeshSelectModes()
    {
        // Get the select modes.
        QList<int> modes;
        if (m_selectModeHash[CommonOCC::S_MeshNode])
        {
            modes.push_back(CommonOCC::S_MeshNode);
        }

        if (m_selectModeHash[CommonOCC::S_MeshElement])
        {
            modes.push_back(CommonOCC::S_MeshElement);
        }

        return modes;
    }

    QList<int> FITKGraph3DWindowOCC::getActiveSelectModes()
    {
        // Get the select modes.
        QList<int> modes;
        QHashIterator<int, bool> iter(m_selectModeHash);
        while (iter.hasNext())
        {
            iter.next();

            if (iter.value())
            {
                modes.push_back(iter.key());
            }
        }

        return modes;
    }

    void FITKGraph3DWindowOCC::clearSelectModes()
    {
        // Shape.
        m_selectModeHash[CommonOCC::S_ShapeVertex] = false;
        m_selectModeHash[CommonOCC::S_ShapeEdge] = false;
        m_selectModeHash[CommonOCC::S_ShapeWire] = false;
        m_selectModeHash[CommonOCC::S_ShapeFace] = false;
        m_selectModeHash[CommonOCC::S_ShapeShell] = false;
        m_selectModeHash[CommonOCC::S_ShapeSolid] = false;
        m_selectModeHash[CommonOCC::S_ShapeCompSolid] = false;
        m_selectModeHash[CommonOCC::S_ShapeCompound] = false;
        m_selectModeHash[CommonOCC::S_ShapeShape] = false;

        // Mesh.
        m_selectModeHash[CommonOCC::S_MeshNode] = false;
        m_selectModeHash[CommonOCC::S_MeshElement] = false;
    }
}
