/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKOCCGraphObject3D.h
 * @brief       The OCC graph object base for interactive objects,
 *              such as MeshVS and AIS_Shape.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-12
 *********************************************************************/

#ifndef __FITKOCCGRAPHOBJECT3D_H__
#define __FITKOCCGRAPHOBJECT3D_H__

#include "FITK_Component/FITKRenderWindowOCC/FITKGraphObjectOCC.h"

#include "FITKOCCGraphAdaptorAPI.h"

#include "FITKOCCCommons.h"

// Forward declaration
class QVariant;

class TopoDS_Shape;

namespace Core
{
    class FITKAbstractNDataObject;
}

namespace Exchange
{
    /**
     * @brief       The OCC graph object base for interactive objects,
     *              such as MeshVS and AIS_Shape.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-12
     */
    class FITKOCCGRAPHADAPTORAPI FITKOCCGraphObject3D : public Render::FITKGraphObjectOCC
    {
        // Regist
        FITKGraphObjectRegist(FITKOCCGraphObject3D, Render::FITKGraphObjectOCC);
        FITKCLASS(Exchange, FITKOCCGraphObject3D);

    public:
        /**
         * @brief       Constructor.
         * @param[in]   dataObj: The shape data object need to be exchanged
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        FITKOCCGraphObject3D(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        virtual ~FITKOCCGraphObject3D();

        /**
         * @brief       Enable or disable top most z-layer.[virtual]
         * @param[in]   flag: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        virtual void enableTopMost(bool flag);

        /**
         * @brief       Enable or disable top z-layer.[virtual]
         * @param[in]   flag: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-10
         */
        virtual void enableTop(bool flag);

        /**
         * @brief       Get the index with the shape data and shape type.[virtual]
         * @param[in]   type: The shape type
         * @param[in]   shape: The shape data[quote]
         * @return      The index of the shape
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-05
         */
        virtual int getShapeIndex(FITKOCCCommons::ShapeMeshType type, TopoDS_Shape & shape);

        /**
         * @brief       Get the shape with the index and shape type.[virtual]
         * @param[in]   type: The shape type
         * @param[in]   index: The index of the shape
         * @return      The shape belonged to the given index
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-05
         */
        virtual TopoDS_Shape getIndexedShape(FITKOCCCommons::ShapeMeshType type, int index);

        /**
         * @brief       Get the graph object's type and other information.
         * @return      The graph information struct
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-29
         */
        FITKOCCCommons::GraphInfo getGraphInfo();

        /**
         * @brief       Get the data object id.
         * @return      The data object id
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-01
         */
        int getDataId();

        /**
         * @brief       Get the data object.
         * @return      The data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-06
         */
        Core::FITKAbstractDataObject* getDataObj();

        /**
         * @brief       Get the data object as the given type.[template]
         * @return      The data object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-06
         */
        template <class T>
        T* getDataObjAs()
        {
            return dynamic_cast<T*>(_dataObj);
        }

        /**
         * @brief       Hide or show the graph object's interactive objects.[virtual]
         * @param[in]   visibility: The visibility.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        virtual void setVisible(bool visibility);

        /**
         * @brief       Show or hide the shape by the given type and flag.[virtual]
         * @param[in]   type: The shape type
         * @param[in]   visible: The visibility
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        virtual void setViewMode(FITKOCCCommons::ShapeMeshViewMode type, bool visible);

        /**
         * @brief       Clear the cache data.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        virtual void clearCache();

        /**
         * @brief       Update the graph object.[virtual][override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        virtual void update(bool forceUpdate = false) override;

        /**
         * @brief       Update the visibility with data object's visibility.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-08
         */
        virtual void updateVisibility();

        /**
         * @brief       Update the interactive objects in graph object.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-09
         */
        virtual void updateInteractiveObjs();

        /**
         * @brief       Set the color for single edge, face or solid or even mesh element.[virtual]
         * @param[in]   color: The color
         * @param[in]   type: The shape mesh type
         * @param[in]   index: The index of edge or face or solid or element
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        virtual void setColor(QColor color, FITKOCCCommons::ShapeMeshType type, int index);

        /**
         * @brief       Set the color for edges, faces or solids or even mesh elements.[virtual]
         * @param[in]   color: The color
         * @param[in]   type: The shape mesh type
         * @param[in]   indice: The indice of edges or faces or solids or elements
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        virtual void setColor(QColor color, FITKOCCCommons::ShapeMeshType type, QList<int> indice);

        /**
         * @brief       Set the color to all vertice, edges and faces.[virtual]
         * @param[in]   color: The color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-13
         */
        virtual void setColor(QColor color);

        /**
         * @brief       Enable or disable transparency.[virtual]
         * @param[in]   isOn: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-05
         */
        virtual void setEnableTransparency(bool isOn);

        /**
         * @brief       Set the shape of point mark.[virtual]
         * @param[in]   type: The type of the point mark
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        virtual void setPointMark(FITKOCCCommons::PointMarkShape type);

        /**
         * @brief       Set the point size of vertice' marks.[virtual]
         * @param[in]   factor: The size factor( default 1.0 )
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        virtual void setPointSizeScale(double factor);

        /**
         * @brief       Set the line width of the edges( wire ).[virtual]
         * @param[in]   size: The line width
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-20
         */
        virtual void setLineWidth(double size);

        /**
         * @brief       Highlight the whole object.[virtual]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        virtual void highlight(bool update = true);

        /**
         * @brief       Dis-highlight the object.[virtual]
         * @param[in]   update: Need to update the view[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        virtual void disHighlight(bool update = true);

        /**
         * @brief       Get the highlight states of the graph.
         * @return      Is highlighting
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        bool isHighlighting();

        /**
         * @brief       Set the variant input data.( The input role must be an integer between 1000 and 1999. )[virtual]
         * @param[in]   value: The input value
         * @param[in]   inputRole: The input value[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        virtual void setInputVariantData(QVariant value, int inputRole = -1);

        /**
         * @brief       Get the variant output data.( The output role must be an integer between 2000 and 2999. )
         * @param[in]   outputRole: The output role[default]
         * @return      The output value
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        QVariant getOutputVariantData(int outputRole = -1);

        /**
         * @brief       Set the user data to the given port( role key ).
         * @param[in]   role: The data role key( Must larger than -1 )
         * @param[in]   value: The user data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        void setUserData(int role, QVariant value);

        /**
         * @brief       Get the user data by the given port( role key ).
         * @param[in]   role: The data role key
         * @return      The user data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        QVariant getUserData(int role);

        /**
         * @brief       Get the user data by the given port( role key ).[template]
         * @param[in]   role: The data role key
         * @return      The user data
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        template<class T>
        T getUserDataAs(int role)
        {
            if (!m_userInfo.contains(role))
            {
                return QVariant().value<T>();
            }

            return m_userInfo[role].value<T>();
        }

        /**
         * @brief       Remove the OCC object's reference count and delete the OCC object.[template]
         * @param[in]   obj: The object.[quote]
         * @param[in]   deleteObj: Need to delete the object[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        template <class T>
        void deleteOccObj(T* & obj, bool deleteObj = true)
        {
            if (obj)
            {
                obj->DecrementRefCounter();

                if (deleteObj)
                {
                    obj->Delete();
                }
            }

            obj = nullptr;
        }

        /**
         * @brief       Remove the OCC object's reference count and delete the OCC object list.[template]
         * @param[in]   objs: The object list.[quote]
         * @param[in]   deleteObj: Need to delete the object[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        template <class T>
        void deleteOccObjs(QList<T*> & objs, bool deleteObj = true)
        {
            for (T* obj : objs)
            {
                if (obj)
                {
                    obj->DecrementRefCounter();

                    if (deleteObj)
                    {
                        obj->Delete();
                    }
                }
            }

            objs.clear();
        }

        /**
         * @brief       Delete the c++ object.[template]
         * @param[in]   obj: The object.[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        template <class T>
        void deleteObj(T* & obj)
        {
            if (obj)
            {
                delete obj;
            }

            obj = nullptr;
        }

        /**
         * @brief       Delete the c++ object list.[template]
         * @param[in]   objs: The object list.[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        template <class T>
        void deleteObjs(QList<T*> & objs)
        {
            for (T* obj : objs)
            {
                if (obj)
                {
                    delete obj;
                }
            }

            objs.clear();
        }

        // Test function
        //@{
        /**
         * @brief       Test function.[virtual]
         * @param[in]   details: The input information
         * @return      Is success
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        virtual bool test(QVariant& details);
        //@}

    protected:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-12
         */
        FITKOCCGraphObject3D() = default;

        /**
         * @brief       Enable or disable top most z-layer.
         * @param[in]   obj: The interactive object
         * @param[in]   flag: Enable or not
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-10-08
         */
        void enableTopMost(AIS_InteractiveObject* obj, bool flag);

        /**
         * @brief       Internal function: Intialize the output variant value.
         *              ( The output role must be an integer between 2000 and 2999. )
         * @param[in]   value: The output variant value
         * @param[in]   outputRole: The output role[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        void setOutputVariantData(QVariant value, int outputRole = 2000);

        /**
         * @brief       Internal function: Saving the variant temp data.
         *              ( The output role must be an integer bigger than 2999. )
         * @param[in]   value: The temp data value
         * @param[in]   role: The temp data role[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        void setTempVariantData(QVariant value, int role = 3000);

        /**
         * @brief       Get the variant temp data.( The output role must be an integer bigger than 2999. )
         * @param[in]   role: The temp data role[default]
         * @return      The temp data value
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        QVariant getTempVariantData(int role = 3000);

    protected:
        /**
         * @brief       The default input data role for setting the variant input.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        int m_defaultInputRole = 1000;

        /**
         * @brief       The default output data role for setting the variant output.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-12
         */
        int m_defaultOutputRole = 2000;

        /**
         * @brief       The default temp data role for setting the variant data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-13
         */
        int m_defaultTempRole = 3000;

        /**
         * @brief       The view mode hash for visibility.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        QHash<FITKOCCCommons::ShapeMeshViewMode, bool> m_viewModes;

        /**
         * @brief       The graph object's model information.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-14
         */
        FITKOCCCommons::GraphInfo m_graphInfo;

        /**
         * @brief       If the model is highlighting.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-08-07
         */
        bool m_isHighlighting = false;

        /**
         * @brief       The user's information dict.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-09-09
         */
        QHash<int, QVariant> m_userInfo;

    };
}   // namespace Exchange

#endif // __FITKOCCGRAPHOBJECT3D_H__
