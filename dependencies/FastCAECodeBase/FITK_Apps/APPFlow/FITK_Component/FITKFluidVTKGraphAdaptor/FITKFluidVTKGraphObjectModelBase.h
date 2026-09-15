/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObjectModelBase.h
 * @brief   Abstract shape graph object 3D model for VTK graph widget.

 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2025-03-31
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECTMODELBASE_H__
#define __FITKFLUIDVTKGRAPHOBJECTMODELBASE_H__

#include "FITKFluidVTKGraphObject3D.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

#include <QVector>
#include <QVariant>

// Forward declaration
class vtkPolyData;
class vtkUnstructuredGrid;
class vtkDataSet;
class vtkDoubleArray;

class FITKGraphActor;

namespace Interface
{
    class FITKAbsGeoShapeAgent;
    class FITKGeometryMeshVS;
    class FITKVirtualTopoManager;
}

namespace Exchange
{
    class FITKFluidVTKShapeColorMap;
}

namespace Exchange
{
    class FITKFluidVTKGraphObjectSelect;
}

namespace Exchange
{
    /**
     * @brief   Abstract shape graph object 3D model for VTK graph widget.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2025-03-31
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObjectModelBase : public FITKFluidVTKGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObjectModelBase, FITKFluidVTKGraphObject3D);
        FITKCLASS(Exchange, FITKFluidVTKGraphObjectModelBase);

    public:
        /**
         * @brief   Constructor.
         * @param   dataObj: The shape model data object need to exchange
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKFluidVTKGraphObjectModelBase(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief   Destructor.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        virtual ~FITKFluidVTKGraphObjectModelBase();

        /**
         * @brief   Show or hide graph object.[override]
         * @param   visibility: The visibility of graph object
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void setVisible(bool visibility) override;

        /**
         * @brief   Show or hide the shape by the given type and flag.[override]
         * @param   type: The shape type
         * @param   visible: The visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible) override;

        /**
         * @brief   Enable or disable transparent.[override]
         * @param   isOn: Enable or disable
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void setTransparent(bool isOn) override;

        /**
         * @brief   Set the color for single edge, face or solid.[override]
         * @param   color: The color
         * @param   type: The shape mesh type
         * @param   index: The shape id
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void setColor(QColor color, FITKFluidVTKCommons::ShapeType type, int index) override;

        /**
         * @brief   Highlight the graph actors.[override]
         * @param   type: The shape type need to be highlighted[default]
         * @param   color: The given color for highlighting this time[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void highlight(FITKFluidVTKCommons::ShapeType type = FITKFluidVTKCommons::ShapeTypeNone, QColor color = QColor()) override;

        /**
         * @brief   Dis-highlight the graph actors.[override]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void disHighlight() override;

        /**
         * @brief   Advanced highlight mode for highlighting part of the graph actors.[override]
         * @param   type: The type of the model need to be highlighted
         * @param   indice: The given indice of the model component need to be highlight
         * @param   color: The given color for highlighting this time[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color = QColor()) override;

        /**
         * @brief   Dis-highlight the graph actors if the graph object is in advanced highlighting mode.[override]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void disAdvanceHighlight() override;

        /**
         * @brief   Get the number of components of the given type.[override]
         * @param   type: The shape type
         * @return  The count
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        int getNumberOf(FITKFluidVTKCommons::ShapeType type) override;

        // For picking.
        //@{
        /**
         * @brief   Set if the graph object pickable mode.[override]
         * @param   mode: The pickable mode
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void setPickMode(FITKFluidVTKCommons::ShapePickMode mode) override;

        /**
         * @brief   Get the shape id by the given shape type and VTK cell id.[override]
         * @param   vtkCellId: The VTK cell id in shape data
         * @param   topAbsShapeType: The shape type of the given ID( the same as TopAbs_ShapeEnum )
         * @return  The shape id
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        int getShapeIdByVTKCellId(int vtkCellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType) override;

        /**
         * @brief   Get the VTK cell indice by the given shape type and shape id.[override]
         * @param   shapeId: The shape id in shape data
         * @param   topAbsShapeType: The shape type of the given ID( the same as TopAbs_ShapeEnum )
         * @return  The cell indice[const]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        const QVector<int> getVTKCellIdsByShapeId(int shapeId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType) override;

        /**
         * @brief   Get the VTK cell indice by the given shape type and cell id in VTK.[override]
         * @param   cellId: The cell id in VTK
         * @param   topAbsShapeType: The shape type of the given ID( the same as TopAbs_ShapeEnum )
         * @return  The cell indice[const]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        const QVector<int> getVTKCellIdsByVTKCellId(int cellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType) override;
        //@}

        // For highlighting.
        //@{
        /**
         * @brief   Get the VTK mesh grid by the given mesh type.[override]
         * @param   type: The shape's VTK mesh type
         * @return  The vtkDataSet object of the given type of mesh
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        vtkDataSet* getMesh(FITKFluidVTKCommons::ShapeType type) override;
        //@}

        // Test function
        //@{
        /**
         * @brief   Test function.[override]
         * @param   details: The input information
         * @return  Is success
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        bool test(QVariant& details) override;
        //@}

    protected:
        /**
         * @brief   Initialize.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void init();

        /**
         * @brief   Convert the trianglation data to a vtkDataSet.
         * @return  Is valid
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        bool meshVSToVtkDataSet();

        // Split shape data and save the id map.
        //@{
        /**
         * @brief   Create the shape's vertice grid data.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void createVertexGrid();

        /**
         * @brief   Create the shape's edge grid data.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void createEdgeGrid();

        /**
         * @brief   Create the shape's face grid data.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void createFaceGrid();

        /**
         * @brief   Create the shape's solid and face ID map.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void initializeSolidFaceMap();
        //@}

        /**
         * @brief   Generate the data set by the shape data.
         * @param   shapeAgent: The shape agent
         * @return  Is OK
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        bool generateDataSet(Interface::FITKAbsGeoShapeAgent* shapeAgent);

        /**
         * @brief   Generate the shape with the vtkDataSet.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void generateGraph();

        /**
         * @brief   Clear all data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        void clearData();

    protected:
        /**
         * @brief   The shape data of the model.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        Interface::FITKGeometryMeshVS* m_triData{ nullptr };

        /**
         * @brief   The topo manager of the shape agent.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        Interface::FITKVirtualTopoManager* m_topoMgr{ nullptr };

        // For colors.
        //@{
        /**
         * @brief   The color map key words for showing default model color.[const]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        const QString c_defaultArrName = "Default";

        /**
         * @brief   The model extractor for normal or advanced highlight.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKFluidVTKGraphObjectSelect* m_highlightSelector{ nullptr };

        /**
         * @brief   The color map for vertex data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKFluidVTKShapeColorMap* m_colorMapVertex{ nullptr };

        /**
         * @brief   The color map for edge data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKFluidVTKShapeColorMap* m_colorMapEdge{ nullptr };

        /**
         * @brief   The color map for face data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKFluidVTKShapeColorMap* m_colorMapFace{ nullptr };
        //@}

        /**
         * @brief   The model vertice grid data for VTK.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        vtkUnstructuredGrid* m_gridVertex{ nullptr };

        /**
         * @brief   The model edges grid data for VTK.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        vtkUnstructuredGrid* m_gridEdge{ nullptr };

        /**
         * @brief   The model faces grid data for VTK.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        vtkPolyData* m_gridFace{ nullptr };

        /**
         * @brief   The normals for the face points.( For better visualization. )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        vtkDoubleArray* m_normalFacePoint{ nullptr };

        /**
         * @brief   The model actor for vertice.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKGraphActor* m_fActorVertex{ nullptr };

        /**
         * @brief   The model actor for edges.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKGraphActor* m_fActorEdge{ nullptr };

        /**
         * @brief   The model actor for faces.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        FITKGraphActor* m_fActorFace{ nullptr };

        // For picking.
        //@{
        // VTK -> Shape
        //@{
        /**
         * @brief   The id array for getting the vertex id of picked vertex cell.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QVector<int> m_cellIdVertexIdMap;

        /**
         * @brief   The id array for getting the edge id of picked cell.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QVector<int> m_cellIdEdgeIdMap;

        /**
         * @brief   The id array for getting the face id of picked cell.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QVector<int> m_cellIdFaceIdMap;
        //@}

        // Shape -> Shape ( Find children )
        //@{
        /**
         * @brief   The ID hash for getting the solid id of the picked face cell.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QHash<int, int> m_faceSolidIdMap;

        // Shape -> VTK
        //@{
        /**
         * @brief   The id array for getting the cell id with vertex id.
         *          The key is the shape id of vertex in indexed map,
         *          the values are the VTK cell id in vtkDataSet.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QHash<int, int> m_vertexIdCellIdMap;

        /**
         * @brief   The id array for getting the cell id with edge id.
         *          The key is the shape id of edge in indexed map,
         *          the values are the VTK cell id in vtkDataSet.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QHash<int, int> m_edgeIdCellIdMap;

        /**
         * @brief   The solid and face id map.
         *          The key is the shape id of solid in indexed map,
         *          the values are the shape face ids in indexed map.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QHash<int, QVector<int>> m_solidFaceIdsHash;
        //@}

        /**
         * @brief   The solid and face id map.
         *          The key is the shape id of face in indexed map,
         *          the values are the VTK cell ids in vtkDataSet.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QHash<int, QVector<int>> m_faceCellIdsHash;
        //@}
        //@}

        /**
         * @brief   The shape state tag.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2025-03-31
         */
        QVariant m_shapeState;

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECTMODELBASE_H__
