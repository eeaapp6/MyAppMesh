/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectModelBase.h
 * @brief       Abstract graph object for TopoDS_Shape and OCC graph widget.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2025-03-31
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTMODELBASE_H__
#define __FITKOCCGRAPHOBJECTMODELBASE_H__

#include "FITKOCCGraphObject3D.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class TopoDS_Shape;
class TopTools_ShapeMapHasher;

template <typename TopoDS_Shape, typename TopTools_ShapeMapHasher>
class NCollection_IndexedMap;

typedef NCollection_IndexedMap<TopoDS_Shape, TopTools_ShapeMapHasher> TopTools_IndexedMapOfShape;

class FITKOCCAIS_ColoredShape;

namespace Interface
{
    class FITKAbsGeoCommand;
}

namespace Exchange
{
    /**
     * @brief       Abstract graph object for TopoDS_Shape and OCC graph widget.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2025-03-31
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectModelBase : public FITKOCCGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectModelBase, FITKOCCGraphObject3D);
        FITKCLASS(Exchange, FITKOCCGraphObjectModelBase);

        /**
         * @brief       Internal struct for getting the index of child data from root shape.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        struct _ShapeIndiceMap
        {
            TopTools_IndexedMapOfShape* iVert = nullptr;
            TopTools_IndexedMapOfShape* iEdge = nullptr;
            TopTools_IndexedMapOfShape* iWire = nullptr;
            TopTools_IndexedMapOfShape* iFace = nullptr;
            TopTools_IndexedMapOfShape* iShell = nullptr;
            TopTools_IndexedMapOfShape* iSolid = nullptr;
            TopTools_IndexedMapOfShape* iCompSolid = nullptr;
            TopTools_IndexedMapOfShape* iComPound = nullptr;
        };

    public:
        /**
         * @brief       Constructor.
         * @param[in]   dataObj: The model data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelBase(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        virtual ~FITKOCCGraphObjectModelBase();

        /**
         * @brief       Get the geometry command for the model.[pure virtual]
         * @return      The geometry command for the model
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        virtual Interface::FITKAbsGeoCommand* getGeoCommand() = 0;

        /**
         * @brief       Get the part id of the model.[virtual]
         * @return      The part id of the model
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-06-23
         */
        virtual int getPartId();

        /**
         * @brief       Set the shape transparency.[virtual]
         * @param[in]   trans: The transparency
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        virtual void setShapeTransparency(double trans);

        /**
         * @brief       Clear custom transparency.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-18
         */
        virtual void clearShapeTransparency();

        /**
         * @brief       Set the sub-shape color by manual.[virtual]
         * @param[in]   shape: The shape[quote]
         * @param[in]   color: The shape color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual void setSubShapeColor(const TopoDS_Shape & shape, QColor color);

        /**
         * @brief       Set the sub-shape transparency.[virtual]
         * @param[in]   shape: The shape[quote]
         * @param[in]   trans: The transparency
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual void setSubShapeTransparency(const TopoDS_Shape & shape, double trans);

        /**
         * @brief       Hide the sub-shape.[virtual]
         * @param[in]   shape: The shape[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual void setSubShapeHidden(const TopoDS_Shape & shape);

        /**
         * @brief       Clear custom transparency.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual void clearSubShapeTransparency();

        /**
         * @brief       Clear custom color and transparency.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-07-07
         */
        virtual void clearSubShapeAspect();

        /**
         * @brief       Update the interactive objects in graph object.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void updateInteractiveObjs() override;

        /**
         * @brief       Get the index with the shape data and shape type.[override]
         * @param[in]   type: The shape type
         * @param[in]   shape: The shape data[quote]
         * @return      The index of the shape
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        int getShapeIndex(FITKOCCCommons::ShapeMeshType type, TopoDS_Shape & shape) override;

        /**
         * @brief       Get the shape with the index and shape type.[override]
         * @param[in]   type: The shape type
         * @param[in]   index: The index of the shape
         * @return      The shape belonged to the given index
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        TopoDS_Shape getIndexedShape(FITKOCCCommons::ShapeMeshType type, int index) override;

        /**
         * @brief       Show or hide the shape by the given type and flag.[override]
         * @param[in]   type: The shape type
         * @param[in]   visible: The visibility
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setViewMode(FITKOCCCommons::ShapeMeshViewMode type, bool visible) override;

        /**
         * @brief       Set the color for single edge, face or solid.[override]
         * @param[in]   color: The color
         * @param[in]   type: The shape mesh type
         * @param[in]   index: The index of solid or edge or face or solid
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setColor(QColor color, FITKOCCCommons::ShapeMeshType type, int index) override;

        /**
         * @brief       Set the color for edges, faces or solids.[override]
         * @param[in]   color: The color
         * @param[in]   type: The shape mesh type
         * @param[in]   indice: The indice of edges or faces or solids
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setColor(QColor color, FITKOCCCommons::ShapeMeshType type, QList<int> indice) override;

        /**
         * @brief       Set the color to all vertice, edges and faces.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setColor(QColor color) override;

        /**
         * @brief       Enable or disable transparency.[override]
         * @param[in]   isOn: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setEnableTransparency(bool isOn) override;

        /**
         * @brief       Set the shape of point mark.[virtual]
         * @param[in]   type: The type of the point mark
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setPointMark(FITKOCCCommons::PointMarkShape type) override;

        /**
         * @brief       Set the point size of vertice' marks.[override]
         * @param[in]   factor: The size factor( default 1.0 )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setPointSizeScale(double factor) override;

        /**
         * @brief       Set the line width of the edges( wire ).[override]
         * @param[in]   size: The line width
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setLineWidth(double size) override;

        /**
         * @brief       Highlight the whole object.[override]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void highlight(bool update = true) override;

        /**
         * @brief       Dis-highlight the object.[override]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void disHighlight(bool update = true) override;

        /**
         * @brief       Hide or show the graph object's interactive objects.[override]
         * @param[in]   visibility: The visibility.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void setVisible(bool visibility) override;

        /**
         * @brief       Set the select mode for this graph object's interactive objects.[virtual][override]
         * @param[in]   mode: The select mode
         * @param[in]   force: Force add the select mode[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        virtual void addSelectMode(CommonOCC::SelectMode mode, bool force = false) override;

        // Test function
        //@{
        /**
         * @brief       Test function.[override]
         * @param[i/o]  details: The input information[quote]
         * @return      Is success
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        bool test(QVariant& details) override;
        //@}

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCGraphObjectModelBase() = default;

    protected:
        /**
         * @brief       Initialize the indice maps with the given shape.
         * @param[in]   shape: The shape[quote]
         * @param[in]   indiceMap: The indice maps[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void initShapeIndiceMap(TopoDS_Shape& shape, _ShapeIndiceMap* indiceMap);

        /**
         * @brief       Generate the AIS_Shape with the TopoDS_Shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        void generateShape();

    protected:
        /**
         * @brief       Whether the graph object need to use the index map.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-02
         */
        bool m_useIndexMap = false;

        /**
         * @brief       The indice map for shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        _ShapeIndiceMap* m_indiceMap{ nullptr };

        /**
         * @brief       The shape data of the model.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        TopoDS_Shape* m_tpShape{ nullptr };

        /**
         * @brief       The interactive object for colored shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        FITKOCCAIS_ColoredShape* m_fAisShape{ nullptr };

        /**
         * @brief       The hash code of the shape.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-31
         */
        int m_hashCode = -1;

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTMODELBASE_H__
