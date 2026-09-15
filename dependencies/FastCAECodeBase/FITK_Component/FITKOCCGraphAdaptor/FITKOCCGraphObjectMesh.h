/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObjectMesh.h
 * @brief       Graph object for MeshVS_DataSource and OCC graph widget.
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-13
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECTMESH_H__
#define __FITKOCCGRAPHOBJECTMESH_H__

#include "FITKOCCGraphObject3D.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include <QColor>

// Forward declaration
class QVariant;

class MeshVS_DataSource;
class MeshVS_Mesh;
class MeshVS_ElementalColorPrsBuilder;

namespace Interface
{
    class FITKAbstractMesh;
}

class FITKOCCMeshVS_Mesh;

namespace Exchange
{
    /**
     * @brief       Graph object for MeshVS_DataSource and OCC graph widget.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-13
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObjectMesh : public FITKOCCGraphObject3D
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObjectMesh, FITKOCCGraphObject3D);
        FITKCLASS(Exchange, FITKOCCGraphObjectMesh);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   meshData: The shape mesh data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        FITKOCCGraphObjectMesh(Interface::FITKAbstractMesh* meshData);

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        ~FITKOCCGraphObjectMesh();

        /**
         * @brief       Update the interactive objects in graph object.[override]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-09
         */
        void updateInteractiveObjs() override;

        /**
         * @brief       Show or hide the shape by the given type and flag.[override]
         * @param[in]   type: The shape type
         * @param[in]   visible: The visibility
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        void setViewMode(FITKOCCCommons::ShapeMeshViewMode type, bool visible) override;

        /**
         * @brief       Set the color for single element.[override]
         * @param[in]   color: The color
         * @param[in]   type: The shape mesh type
         * @param[in]   index: The index of element
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        void setColor(QColor color, FITKOCCCommons::ShapeMeshType type, int index) override;

        /**
         * @brief       Set the color for elements.[override]
         * @param[in]   color: The color
         * @param[in]   type: The shape mesh type
         * @param[in]   indice: The indice of elements
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        void setColor(QColor color, FITKOCCCommons::ShapeMeshType type, QList<int> indice) override;

        /**
         * @brief       Set the color.[override]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        void setColor(QColor color) override;

        /**
         * @brief       Enable or disable transparency.[override]
         * @param[in]   isOn: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-05
         */
        void setEnableTransparency(bool isOn) override;

        /**
         * @brief       Highlight the whole object.[override]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        void highlight(bool update = true) override;

        /**
         * @brief       Dis-highlight the object.[override]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        void disHighlight(bool update = true) override;

        /**
         * @brief       Hide or show the graph object's interactive objects.[override]
         * @param[in]   visibility: The visibility.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-05
         */
        void setVisible(bool visibility) override;

        // Test function
        //@{
        /**
         * @brief       Test function.[override]
         * @param[i/o]  details: The input information
         * @return      Is success
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        bool test(QVariant& details) override;
        //@}

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        FITKOCCGraphObjectMesh() = default;

    private:
        /**
         * @brief       Generate the AIS_Shape with the TopoDS_Shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        void generateShape();
 
    private:
        /**
         * @brief       The mesh data source.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        MeshVS_DataSource* m_meshSource{ nullptr };

        /**
         * @brief       The interactive object for shape data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        FITKOCCMeshVS_Mesh* m_meshVS{ nullptr };

        /**
         * @brief       The color map for element.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        MeshVS_ElementalColorPrsBuilder* m_colorMap{ nullptr };

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECTMESH_H__
