/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKGraph3DWindowOCC.h
 * @brief   The graph 3D render window for OCC.
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-11
 *********************************************************************/

#ifndef __FITKGRAPH3DWINDOWOCC_H__
#define __FITKGRAPH3DWINDOWOCC_H__

#include "FITK_Kernel/FITKCore/FITKAbstractGraphWidget.h"

#include "FITKRenderWindowOCCAPI.h"

#include "TypeDefOCCWindow.h"

#include <QColor>

class Graphic3d_AspectMarker3d;

 // Forward declaration.
 // Qt
class QToolBar;

// OCC
//@{
// Handle
namespace opencascade
{
    template <class T>
    class handle;
}

// Data
class MeshVS_Mesh;
class TopoDS_Shape;

// Interactive
class AIS_ViewController;
class AIS_InteractiveContext;
class V3d_Viewer;
class V3d_View;
class Graphic3d_GraphicDriver;
class AIS_Manipulator;
class AIS_Shape;
class AIS_TextLabel;

// Tools
class AIS_ViewCube;
class Quantity_Color;
class gp_Pnt;
class gp_Pln;

// Window
class Aspect_Window;

namespace GUI
{
    class WidgetAddSketchPos;
}

class FITKV3d_Viewer;
class FITKV3d_View;
class FITKAIS_SketchShape;
class FITKAIS_ViewController;
//@}

namespace Render
{
    class FITKGraphObjectOCC;
}

namespace Core
{
    class FITKAbstractGraphObject;
}

namespace Render
{
    class FITKGraph3DWindowOCC;

    /**
     * @brief   The internal widget of the occ background.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-08-08
     */
    class FITKGraph3DWindowOCCInternal : public Core::FITKWidget
    {
        Q_OBJECT
    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        explicit FITKGraph3DWindowOCCInternal(FITKGraph3DWindowOCC* occWidget);

        /**
         * @brief   Set the parent occ controller.
         * @param   controller: The interactive controller
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void setController(AIS_ViewController* controller);

        /**
         * @brief   Set the parent occ controller's context.
         * @param   context: The interactive context
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void setContext(AIS_InteractiveContext* context);

        /**
         * @brief   Set the parent occ context's viewer.
         * @param   context: The viewer
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        void setViewer(V3d_Viewer* viewer);

        /**
         * @brief   Set the parent occ context's view.
         * @param   context: The view
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void setView(V3d_View* view);

        /**
         * @brief   Set the text label.
         * @param   label: The position label
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-11-18
         */
        void setTextLabel(AIS_TextLabel* label);

        /**
         * @brief   Coordinate conversion, the screen point is converted to world coordinates and then projected onto a plane
         * @param   Xp: pointX
         * @param   Yp: pointY
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-13
         */
        gp_Pnt convertScreenPointOnPlane(const int & Xp, const int & Yp, V3d_View* view);

    protected:
        /**
         * @brief   Override the paint engine function.( Deal with the crash problem. )
         * @return  The paint engine
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        QPaintEngine* paintEngine() const override;

        /**
         * @brief   Override the paint event.( For updating parent widget. )
         * @param   event: The paint event
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void paintEvent(QPaintEvent* event) override;

        /**
         * @brief   Override the resize event.( For resizing the OCC widget. )
         * @param   event: The resize event
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void resizeEvent(QResizeEvent* event) override;

        /**
         * @brief   For changing view and picking.
         * @param   event: The mouse event
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void mousePressEvent(QMouseEvent* event) override;

        /**
        * @brief   used to complete the operation
        * @param   event: The mouse event
        * @author  fulipeng (fulipengqd@yeah.net)
        * @date    2024-08-26
        */
        void mouseDoubleClickEvent(QMouseEvent *event) override;

        /**
         * @brief   For changing view and picking.
         * @param   event: The mouse event
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void mouseReleaseEvent(QMouseEvent* event) override;

        /**
         * @brief   For changing view.
         * @param   event: The mouse event
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void mouseMoveEvent(QMouseEvent* event) override;

        /**
         * @brief   For changing view.
         * @param   event: The wheel event
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void wheelEvent(QWheelEvent* event) override;

        /**
         * @brief   For reseting the view flags.
         * @param   event: The foucs out event.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-29
         */
        void focusOutEvent(QFocusEvent* event) override;

    private:
        /**
         * @brief   The full occ widget pointer.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        FITKGraph3DWindowOCC* m_occWidget = nullptr;

        /**
         * @brief   The controller of the occ window.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        AIS_ViewController* m_controller{ nullptr };

        /**
         * @brief   The interactive context from parent widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        AIS_InteractiveContext* m_context{ nullptr };

        /**
         * @brief   The viewer from parent widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        V3d_Viewer* m_viewer{ nullptr };

        /**
         * @brief   The view for viewer from parent widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        V3d_View* m_view{ nullptr };

        /**
         * @brief   The text label for mouse position in sketching mode.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-11-18
         */
        AIS_TextLabel* m_coordLabel{ nullptr };

        /**
         * @brief   The context menu flag for mouse release event.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-18
         */
        bool m_contextMenuFlag = false;

    };

    /**
     * @brief   The graph 3D render window for OCC.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-11
     */
    class FITKRENDERWINDOWOCCAPI FITKGraph3DWindowOCC : public Core::FITKAbstractGraph3DWidget // , protected AIS_ViewController
    {
        Q_OBJECT
    public:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        explicit FITKGraph3DWindowOCC(QWidget *parent = nullptr);

        /**
         * @brief   Destructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        ~FITKGraph3DWindowOCC();

        /**
         * @brief   Enable or disbale sketch mode.
         * @param   isOn: Enable or disable
         * @param   shape: Datum plane
         * @param   circle: is circle
         * @return  true 成功 false 失败
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-23
         */
        bool enableSketchMode(bool isOn, double* pos = nullptr, double* dir = nullptr, double* up = nullptr, bool circle = false);

        /**
         * @brief   Enable or disbale viewer's grid lines.
         * @param   isOn: Enable or disable
         * @param   circle: Whether to draw the grid as a circle( default is rect )[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void enableGridLines(bool isOn, bool circle = false);

        /**
         * @brief   Enable or disbale sketch.
         * @param   isOn: Enable or disable
         * @param   circle: Whether to draw the grid as a circle( default is rect )[default]
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-13
         */
        void enableSketch(bool isOn, CommonOCC::SketchShapeType t = CommonOCC::SketchShapeType::None, bool circle = false);

        /**
         * @brief   get sketch point to list
         * @return  Point selected by mouse interaction
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        const QList<double*> getSketchPoint();

        /**
         * @brief   get sketch Pln [0] location [1] dir [2] dir
         * @return  Point selected by mouse interaction
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        void getSketchPln(double location[3], double direction[3], double yDirection[3]);

        /**
         * @brief       Add a new relative position in plane csys.
         * @param[in]   x: The u position
         * @param[in]   y: The v position
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        void addSketchPosition(double x, double y);

        /**
         * @brief       Get the sketch status.
         * @return      Is sketch mode on
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        bool getSketchStatus();

        /**
         * @brief   Enable or disbale parallel projection.[override]
         * @param   isOn: Enable or disbale
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void setParallelProjection(bool isOn = true) override;

        /**
         * @brief   Set the background color.[override]
         * @param   topColor: The top color
         * @param   btmColor: The bottom color[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void setBackgroundColor(float* topColor, float* btmColor = nullptr) override;

        /**
         * @brief   Get the background color.[override]
         * @param   topColor: The top color
         * @param   btmColor: The bottom color[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void getBackgroundColor(float* topColor, float* btmColor = nullptr) override;

        /**
         * @brief   Set the view direction.[override]
         * @param   viewDirec: The view direction
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-13
         */
        void setView(Core::FITKGraph3DStandardView viewDirec) override;

        /**
         * @brief   Set the view direction.[override]
         * @param   viewDirec: The view direction
         * @param   useAnime: useAnime
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-13
         */
        void setView(Core::FITKGraph3DStandardView viewDirec, bool useAnime);

        /**
         * @brief   Add a new graph object to the graph widget.
         * @param   obj: The OCC graph object
         * @param   fitView: Update the graph widget's camera position[default]
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        bool addObject(FITKGraphObjectOCC* obj, bool fitView = true);

        /**
         * @brief   Remove graph object.[override]
         * @param   obj: Graph object
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-13
         */
        void removeGraphObj(Core::FITKAbstractGraphObject* obj) override;

        /**
         * @brief   Update all graph objects by the mouse move position.( if necessary )
         * @param   mx: The mouse x
         * @param   my: The mouse y
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        void updateObjsByMouseMove(int mx, int my);

        /**
         * @brief   Update all graph objects by the mouse move picked position.( if necessary )
         * @param   mx: The mouse x
         * @param   my: The mouse y
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        void updateObjsByMousePick(int mx, int my);

        ///**
        // * @brief   Set the select mode.
        // * @param   mode: The select mode
        // * @param   shiftSelect: Shift select[default]
        // * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date    2024-06-12
        // */
        //void setSelectMode(SelectMode mode, bool shiftSelect = false);

        /**
         * @brief   Enable or disbale the selected signal.
         * @param   isOn: Enable or disbale
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        void enableSelect(bool isOn);

        /**
         * @brief   Enable or disable shift select by shift + left button.
         * @param   isOn: Enable or disbale
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        void enableShiftSelect(bool isOn);

        /**
         * @brief   Enable or disable area select by alt + left button.
         * @param   isOn: Enable or disbale
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void enableAreaSelect(bool isOn);

        /**
         * @brief   Get the selected shape list.
         * @param   shapes: The shapes selected
         * @return  Has selected something
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        bool getSelectedShapes(QVector<TopoDS_Shape>& shapes);

        /**
         * @brief   Get the first selected shape.
         * @param   shape: The shape selected
         * @return  Has selected something
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        bool getFirstSelectedShape(TopoDS_Shape& shape);

        /**
         * @brief   Clear the selection.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void clearSelection();

        /**
         * @brief   Whether the context has selected shape.
         * @return  Has selected shape
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        bool hasSelection();

        /**
         * @brief   Remove the shape from the selection.
         * @param   shape: The shape[const][quote]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-30
         */
        void removeSelectedShape(const TopoDS_Shape & shape);

        /**
         * @brief   Enable or disable the view direction change.
         * @param   isOn: Enable or disable
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        void setViewChangeEnabled(bool isOn);

        // BETA
        ///**
        // * @brief   Enable or disable edit mode.
        // * @param   isOn: Enable or disable
        // * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date    2024-06-12
        // */
        //void setEditMode(bool isOn);

        /**
         * @brief   Reset the view distance.[override]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void fitView() override;

        /**
         * @brief   Reset the view distance with the selection.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-24
         */
        void fitSelected();

        /**
         * @brief   Update the viewer.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-01
         */
        void reRender();

        /**
         * @brief   Clear all interactive objects.[override]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void clear() override;

        /**
         * @brief   Save the graph window as a photo.[override]
         *
         *          Support formats:
         *          All(*.png;*.bmp;*.ppm;*.jpg;*.jpe;*.jpeg;*.tif;*.tiff;*.exr;*.tga)
         *          PNG(Portable Network Graphics)(*.png)
         *          BMP(Bitmap)(*.bmp)
         *          PPM(Portable Pixmap Format)(*.ppm)
         *          JPEG/JIFF(Joint Photographic Experts Group)(*.jpg;*.jpe;*.jpeg)
         *          TIFF(Tagged Image File Format)(*.tif;*.tiff)
         *          OpenEXR high dynamic-range(*.exr)
         *          TGA(Truevision Targa Graphic)(*.tga)
         *
         * @param   image: The file path[quote]
         * @param   w: The width of the picture[default]
         * @param   h: The hight of the picture[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void saveImage(const QString& image, int w = 0, int h = 0) override;

        /**
         * @brief   Get interactive Context.
         * @return  The interactive context
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        AIS_InteractiveContext* getContext();

    signals:
        /**
         * @brief   Signal - Interactive object picked.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void sig_picked();

        /**
         * @brief   Parameters meet the conditions
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-26
         */
        void sig_parametersSatisfy();

        /**
         * @brief   Select point
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-09-19
         */
        void sig_pickUpdateSketch();

        /**
         * @brief   Signal - Context menu.
         * @param   pos: The event position
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-18
         */
        void sig_contextMenu(QPoint pos);

    protected:
        /**
         * @brief   Override the paint engine function.( Deal with the crash problem. )
         * @return  The paint engine
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QPaintEngine* paintEngine() const override;

        /**
         * @brief   Override the paint event.( For updating parent widget. )
         * @param   event: The paint event
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        void paintEvent(QPaintEvent* event) override;

    private slots:
        /**
         * @brief   Slot - The view redrawed.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-16
         */
        void slot_viewRedraw();

        /**
         * @brief   Slot - The interactive controller performed a select.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void slot_mouseSelected();

    private:
        /**
         * @brief   Initialize the OCC window.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void init();

        /**
         * @brief   Initialize the view cube object.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void initViewCube();

        // BETA
        ///**
        // * @brief   Initialize the editor object.
        // * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date    2024-06-12
        // */
        //void initEditor();

        /**
         * @brief   Initialize the selection style.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void initSelectionStyle();

        /**
         * @brief   Initialize the pop toolbar.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        void initPopToolbar();

        /**
         * @brief   Initialize the Sketch.
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-13
         */
        void initSketch();

        /**
         * @brief   Update all shapes' selectability and view mode.
         * @param   selectMode: The select mode[default]
         * @param   update: Need to re-render the interactive obejct[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        void updateAllShape(int selectMode = -1, bool update = true);

        /**
         * @brief   Update all meshes' selectability and view mode.
         * @param   selectMode: The select mode[default]
         * @param   update: Need to re-render the interactive obejct[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-14
         */
        void updateAllMesh(int selectMode = -1, bool update = true);

        /**
         * @brief   Deactive all interactive objects' selection mode( except the view cube ).
         * @param   update: Need to re-render the interactive obejct[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-22
         */
        void deactiveAllInteractive(bool update = true);

        /**
         * @brief   Convert QColor to OCC Quantity_Color.
         * @param   color: The QColor
         * @return  The Quantity_Color
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        Quantity_Color QColorToQuan(QColor color);

        /**
         * @brief   Get all actived shape select modes.
         * @return  The select modes
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        QList<int> getActiveShapeSelectModes();

        /**
         * @brief   Get all actived mesh select modes.
         * @return  The select modes
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        QList<int> getActiveMeshSelectModes();

        /**
         * @brief   Get all actived select modes.
         * @return  The select modes
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        QList<int> getActiveSelectModes();

        /**
         * @brief   Clear all actived select modes.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        void clearSelectModes();

    private:
        // For OCC widget.
        //@{
        /**
         * @brief   The toolbar.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        QToolBar* m_toolbar{ nullptr };

        /**
         * @brief   The actions for changing view direction.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        QList<QAction*> m_viewDireActs;

        /**
         * @brief   The checkable action for showing grid lines.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        // QAction* m_actionGridLines = nullptr;

        /**
         * @brief   The background widget of the occ window.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        FITKGraph3DWindowOCCInternal* m_occBkgWidget{ nullptr };

        /**
         * @brief   The view controller.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        FITKAIS_ViewController* m_controller{ nullptr };

        /**
         * @brief   The occ window.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-08
         */
        Aspect_Window* m_occWindow{ nullptr };

        /**
         * @brief   The interactive context.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        AIS_InteractiveContext* m_context{ nullptr };

        /**
         * @brief   The driver for OCC widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        Graphic3d_GraphicDriver* m_driver{ nullptr };

        /**
         * @brief   The viewer for OCC widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        FITKV3d_Viewer* m_viewer{ nullptr };

        /**
         * @brief   The view for viewer.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        FITKV3d_View* m_view{ nullptr };

        /**
         * @brief   The view cube obejct.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        AIS_ViewCube* m_viewCube{ nullptr };

        /**
         * @brief   The text label for mouse position in sketching mode.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-11-18
         */
        AIS_TextLabel* m_coordLabel{ nullptr };

        // BETA
        ///**
        // * @brief   The interactive editor.
        // * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date    2024-06-12
        // */
        //AIS_Manipulator* m_editor{ nullptr };
        //@}

        /**
         * @brief   The OCC graph objects list.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QList<FITKGraphObjectOCC*> m_objects;

        /**
         * @brief   The preview selection color.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        QColor m_highlightColor = QColor(255, 0, 0);

        /**
         * @brief   The selection color.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QColor m_selectionColor = QColor(0, 0, 255);

        /**
         * @brief   The default transparency of the interactive object.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        double m_transparency = 0.0; // 0.0 0.1

        /**
         * @brief   Optimize the visualization.[const]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        const bool m_optimization = false;

        /**
         * @brief   The select modes hash.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        QHash<int, bool> m_selectModeHash;

        // BETA
        ///**
        // * @brief   Is edit mode on.
        // * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date    2024-06-12
        // */
        //bool m_isEditing = false;

        /**
         * @brief   Enable or disable camera changing with the mouse.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-09
         */
        bool m_enableMouse = true;

        /**
         * @brief   Enable or disable multi selection.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-11
         */
        bool m_enableMultiSelect = false;

        /**
         * @brief   Enable or disable changing view direction by GUI.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-28
         */
        bool m_enableViewChange = true;

        /**
         * @brief   The container for the FITKAIS_SketchShape.
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-13
         */
        QList<opencascade::handle<FITKAIS_SketchShape>> m_sketchShapeContainer;

        /**
         * @brief   sketch shape object
         * @author  fulipeng (fulipengqd@yeah.net)
         * @date    2024-08-13
         */
        FITKAIS_SketchShape* m_sketchShape{ nullptr };

        /**
         * @brief       Widget for adding sketch position.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        GUI::WidgetAddSketchPos* m_widgetAddPos{ nullptr };

        // For view control.
        friend class FITKGraph3DWindowOCCInternal;
    };
}

#endif // __FITKGRAPH3DWINDOWOCC_H__
