/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKGraphObject3D.h
 * @brief   Fluid graph object 3D base for VTK graph widget.
 *          All of the fluid graph objects need to inherit this class.
 *          Not all of the interface need to be override, because most of
 *          the graph objects only need to be shown. 
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-05
 *********************************************************************/

#ifndef __FITKFLUIDVTKGRAPHOBJECT3D_H__
#define __FITKFLUIDVTKGRAPHOBJECT3D_H__

#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

#include "FITKFluidVTKGraphAdaptorAPI.h"

#include "FITKFluidVTKCommons.h"

// Forward declaration
class QVariant;

class vtkActor;
class vtkObject;
class vtkDataSet;

class FITKGraphActor;

namespace Comp
{
    class FITKGraphRender;
}

namespace Core
{
    class FITKAbstractNDataObject;
    class FITKAbstractGraphWidget;
}

namespace Exchange
{
    /**
     * @brief   Fluid graph object for 3D base for VTK graph widget.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-05
     */
    class FITKFLUIDGRAPHADAPTORAPI FITKFluidVTKGraphObject3D : public Comp::FITKGraphObjectVTK
    {
        Q_OBJECT

        // Regist
        FITKGraphObjectRegist(FITKFluidVTKGraphObject3D, Comp::FITKGraphObjectVTK);
        FITKCLASS(Exchange, FITKFluidVTKGraphObject3D);

    public:
        /**
         * @brief   Constructor.
         * @param   dataObj: The data object need to exchange
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        FITKFluidVTKGraphObject3D(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief   Destructor.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual ~FITKFluidVTKGraphObject3D();

        /**
         * @brief   Judge and save the graph widget.[BETA]
         * @param   widget: The graph widget.
         * @return  If the graph object has set a graph widget already, then return it, and
         *          do not save the given widget.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        Core::FITKAbstractGraphWidget* setConstGraphWidget(Core::FITKAbstractGraphWidget* widget);

        /**
         * @brief   Show or hide the shape by the given type and flag.[virtual]
         * @param   type: The shape type
         * @param   visible: The visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-14
         */
        virtual void setViewMode(FITKFluidVTKCommons::ShapeMeshViewMode type, bool visible);

        /**
         * @brief   Enable or disable transparent.[virtual]
         * @param   isOn: Enable or disable
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-30
         */
        virtual void setTransparent(bool isOn);

        /**
         * @brief   Set the color for single edge, face or solid.[virtual]
         * @param   color: The color
         * @param   type: The shape mesh type
         * @param   index: The index of face or edge or vertex
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        virtual void setColor(QColor color, FITKFluidVTKCommons::ShapeType type, int index);

        /**
         * @brief   Set the color.[virtual]
         * @param   color: The color
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        virtual void setColor(QColor color);

        /**
         * @brief   Get the number of components of the given type.[virtual]
         * @param   type: The shape type
         * @return  The count
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-22
         */
        virtual int getNumberOf(FITKFluidVTKCommons::ShapeType type);

        // For picking.
        //@{
        /**
         * @brief   Set if the graph object pickable mode.[virtual]
         * @param   mode: The pickable mode
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        virtual void setPickMode(FITKFluidVTKCommons::ShapePickMode mode);

        /**
         * @brief   Get the shape id by the given shape type and VTK cell id.[virtual]
         * @param   vtkCellId: The VTK cell id in shape data
         * @param   topAbsShapeType: The shape type of the given ID( the same as TopAbs_ShapeEnum )
         * @return  The shape id
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-19
         */
        virtual int getShapeIdByVTKCellId(int vtkCellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType);

        /**
         * @brief   Get the VTK cell indice by the given shape type and shape id in shape data.[virtual]
         * @param   shapeId: The shape id in shape data
         * @param   topAbsShapeType: The shape type of the given ID( the same as TopAbs_ShapeEnum )
         * @return  The cell indice[const]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        virtual const QVector<int> getVTKCellIdsByShapeId(int shapeId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType);

        /**
         * @brief   Get the VTK cell indice by the given shape type and cell id in VTK.[virtual]
         * @param   cellId: The cell id in VTK
         * @param   topAbsShapeType: The shape type of the given ID( the same as TopAbs_ShapeEnum )
         * @return  The cell indice[const]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        virtual const QVector<int> getVTKCellIdsByVTKCellId(int cellId, FITKFluidVTKCommons::ShapeAbsEnum topAbsShapeType);
        //@}

        // For highlighting.
        //@{
        /**
         * @brief   Get the VTK mesh grid by the given mesh type.[virtual]
         * @param   type: The shape's VTK mesh type
         * @return  The vtkDataSet object of the given type of mesh
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        virtual vtkDataSet* getMesh(FITKFluidVTKCommons::ShapeType type);
        //@}

        /**
         * @brief   Get the highlight states of the graph.
         * @return  Is highlighting
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        bool isHighlighting();

        /**
         * @brief   Get the pre-highlight states of the graph.
         * @return  Is highlighting
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        bool isPreHighlighting();

        /**
         * @brief   Get the advanced highlight states of the graph.
         * @return  Is advanced highlighting
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        bool isAdvancedHighlighting();

        /**
         * @brief   Set the render, which will add this graph object to itself.( for calculating size )
         * @param   render: The FITKGrapgRender
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void setFITKRender(Comp::FITKGraphRender* render);

        /**
         * @brief   Get the layer need to be render.[virtual]
         * @return  The layer index
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual int getRenderLayer();

        /**
         * @brief   Get the layer for highlighting need to be render.[virtual]
         * @return  The layer index
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual int getRenderLayerHighlight();

        /**
         * @brief   Pre-highlight the graph actors.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void preHighlight();

        /**
         * @brief   Highlight the graph actors.[virtual]
         * @param   type: The shape type need to be highlighted[default]
         * @param   color: The given color for highlighting this time[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void highlight(FITKFluidVTKCommons::ShapeType type = FITKFluidVTKCommons::ShapeTypeNone, QColor color = QColor());

        /**
         * @brief   Dis-highlight the graph actors.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void disHighlight();

        /**
         * @brief   Advanced highlight mode for highlighting part of the graph actors.[virtual]
         * @param   type: The type of the model need to be highlighted
         * @param   indice: The given indice of the model component need to be highlight
         * @param   color: The given color for highlighting this time[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        virtual void advanceHighlight(FITKFluidVTKCommons::ShapeType type, QVector<int> indice, QColor color = QColor());

        /**
         * @brief   Dis-highlight the graph actors if the graph object is in advanced highlighting mode.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        virtual void disAdvanceHighlight();

        /**
         * @brief   Show or hide graph object.[virtual]
         * @param   visibility: The visibility of graph object
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void setVisible(bool visibility);

        /**
         * @brief   Update the visibility with data object's visibility.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void updateVisibility();

        /**
         * @brief   Update all graph actors.[virtual][override]
         * @param   forceUpdate: Force update[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void update(bool forceUpdate = false) override;

        /**
         * @brief   Get the data id.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        int getDataId();

        /**
         * @brief   Get the shape graph object's information.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        FITKFluidVTKCommons::ShapeInfo getShapeInfo();

        /**
         * @brief   Update the actor with the camera normal if necessary.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void updateCameraNormal();

        /**
         * @brief   Simplified the graph object while the camera is changing.
         * @param   flag: Is on
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        void setGraphSimplified(bool flag);

        /**
         * @brief   Simplified the graph object if necessary.[virtual]
         * @param   enabled: Enabled simplified flag
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void simplified(bool enabled);

        /**
         * @brief   Get the additional graph objects of this graph.( Not all the graph objects have additional objects. )[virtual]
         * @return  The graph object list;
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual QList<FITKFluidVTKGraphObject3D*> getAddinGraphObjs();

        /**
         * @brief   Set the information.[virtual]
         * @param   info: The variant information
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void setAddinInfo(QVariant info);

        /**
         * @brief   Whether the graph object has relation with the given data object id.[virtual]
         * @param   dataObjId: The data object id
         * @return  Has relation
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual bool isRelatedTo(int dataObjId);

        /**
         * @brief   Set the variant input data.
         * @param   value: The input value
         * @param   inputRole: The input role[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        void setInputVariantData(QVariant value, int inputRole = 1000);

        /**
         * @brief   Get the variant output data.
         * @param   outputRole: The output role[default]
         * @return  The output value
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        QVariant getOutputVariantData(int outputRole = 2000);

        /**
         * @brief   Set the user data to the given port( role key ).
         * @param   role: The data role key( Must larger than -1 )
         * @param   value: The user data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        void setUserData(int role, QVariant value);

        /**
         * @brief   Get the user data by the given port( role key ).
         * @param   role: The data role key
         * @return  The user data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        QVariant getUserData(int role);

        /**
         * @brief   Get the user data by the given port( role key ).[template]
         * @param   role: The data role key
         * @return  The user data
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
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
         * @brief   Check if the given actor is belonged to this graph object.[virtual]
         * @param   actor: The actor( 2D or 3D )
         * @return  Is belonged to
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual bool contains(vtkProp* actor);

        /**
         * @brief   Test function.[virtual]
         * @param   details: The input information
         * @return  Is success
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual bool test(QVariant& details);

    signals:
        /**
         * @brief   Signal - Need to be deleted while the data object has been deleted.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        void sig_needToBeDeleted();

    protected:
        /**
         * @brief   Constructor.( For copying data )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        FITKFluidVTKGraphObject3D();

        /**
         * @brief   Set the layer need to be render.[virtual]
         * @param   layer: The layer index
         * @param   layerHighlight: The layer index for highlighing[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void setRenderLayer(int layer, int layerHighlight = -1);

        /**
         * @brief   Set the layer for highlighting need to be render.[virtual]
         * @param   layer: The layer index
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void setRenderLayerHighlight(int layer);

        /**
         * @brief   Initialize the actor's visualization properties.[virtual]
         * @param   actor: The actor
         * @param   details: The detail information for setting properties[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void initActorProperties(vtkProp* actor, QVariant details = QVariant());

        /**
         * @brief   Delete all cache pointer which will not affect the visualization.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual void clearCache();

        /**
         * @brief   Modify all actors.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        void updateActors();

        /**
         * @brief   Get the visibility from the data object.[virtual]
         * @return  The visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        virtual bool getDataVisibility();

        /**
         * @brief   Get the parent model data visibility.
         * @param   id: The parent data object id
         * @return  Visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        bool getParentDataVisibility(int id);

        /**
         * @brief   Set the light propertied for rendering line actor.
         * @param   fActor: The actor
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        void setLightPropertiesForLine(FITKGraphActor* fActor);

        /**
         * @brief   Set the light propertied for rendering face actor.
         * @param   fActor: The actor
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        void setLightPropertiesForSurface(FITKGraphActor* fActor);

        /**
         * @brief   Delete the VTK object.[template]
         * @param   obj: The vtkObject.[quote]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        template <class T>
        void deleteVtkObj(T* & obj)
        {
            if (obj)
            {
                obj->Delete();
            }

            obj = nullptr;
        }

        /**
         * @brief   Delete the VTK none-smartpointer object list.[template]
         * @param   objs: The vtkObject list.[quote]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        template <class T>
        void deleteVtkObjs(QList<T*> & objs)
        {
            for (T* obj : objs)
            {
                if (obj)
                {
                    obj->Delete();
                }
            }

            objs.clear();
        }

        /**
         * @brief   Delete the c++ object.[template]
         * @param   obj: The object.[quote]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
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
         * @brief   Delete the c++ object list.[template]
         * @param   objs: The object list.[quote]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
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

        /**
         * @brief   Reset the vtk data object.[template]
         * @param   obj: The vtk data object.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-24
         */
        template <class T>
        void resetVtkObj(T* obj)
        {
            if (obj)
            {
                obj->Reset();
            }
        }

        ///**
        // * @brief   Convert the QString to the char*.
        // * @param   str: The QString
        // * @return  The char* from the string
        // * @author  ChengHaotian (yeguangbaozi@foxmail.com)
        // * @date    2024-06-05
        // */
        //char* QStringToCharA(QString str);

    protected:
        /**
         * @brief   The default input data role for setting the variant input.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        int m_defaultInputRole = 1000;

        /**
         * @brief   The default output data role for setting the variant input.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        int m_defaultOutputRole = 2000;

        /**
         * @brief   The user's information dict.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        QHash<int, QVariant> m_userInfo;

        /**
         * @brief   The view mode hash for visibility.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-14
         */
        QHash<FITKFluidVTKCommons::ShapeMeshViewMode, bool> m_viewModes;

        /**
         * @brief   The shape graph object's model information.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        FITKFluidVTKCommons::ShapeInfo m_shapeInfo;

        /**
         * @brief   The pick mode of this shape graph.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        FITKFluidVTKCommons::ShapePickMode m_pickMode = FITKFluidVTKCommons::PickNone;

        /**
         * @brief   If the graph object need to be simplified while the camera is changing.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
#ifdef QT_DEBUG
        bool m_graphSimplified = false;
#else
        bool m_graphSimplified = false;
#endif

        /**
         * @brief   If the graph object simplified to the feature edges.( For models only for now )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
#ifdef QT_DEBUG
        bool m_simplifiedWithFeature = false;
#else
        bool m_simplifiedWithFeature = true;
#endif

        /**
         * @brief   The graph widget who owns this graph object first time.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        Core::FITKAbstractGraphWidget* m_constGraphWidget{ nullptr };

        /**
         * @brief   The layer need to be renderer to, default is 0.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        int m_layer = 0;

        /**
         * @brief   The layer for highlighting need to be renderer to, default is 0.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        int m_layerHighlight = 0;

        /**
         * @brief   If the model is highlighting.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        bool m_isHighlighting = false;

        /**
         * @brief   If the model is pre-highlighting.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        bool m_isPreHighlighting = false;

        /**
         * @brief   If the model is advanced-highlighting.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-23
         */
        bool m_isAdvHighlighting = false;

        /**
         * @brief   The additional graph object list.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-05
         */
        QList<FITKFluidVTKGraphObject3D*> m_addinGraphObjList;

        // Mesh params.
        //@{
        /**
         * @brief   The factor for meshing.
         *          P.S.
         *          ( The smaller the value is, the greater the number of cells and points in VTK. )
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-06
         */
        double m_lineDef = 0.001;
        //@}

        /**
         * @brief   The flag for testing.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-26
         */
        bool m_testFlag = false;

    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKGRAPHOBJECT3D_H__
