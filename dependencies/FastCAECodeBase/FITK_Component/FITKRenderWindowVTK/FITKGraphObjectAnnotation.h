/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGraphObjectAnnotation.h
 * @brief       Graph object for annotation.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-05-09
 *********************************************************************/

#ifndef __FITKGRAPHOBJECTANNOTATION_H__
#define __FITKGRAPHOBJECTANNOTATION_H__

#include "FITKGraphObjectVTK.h"

#include <vtkCommand.h>

#include "FITKRenderWindowVTKAPI.h"

 // Forward declaration
class vtkPoints;
class vtkCellArray;
class vtkPolyData;

class vtkActor2D;
class vtkTextActor;

namespace Comp
{
    class FITKGraphObjectAnnotation;
    class FITKGraphAnnotation;
}

namespace Comp
{
    /**
     * @brief       Graph object for annotation.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-05-09
     */
    class FITKRenderWindowVTKAPI FITKGraphObjectAnnotation : public FITKGraphObjectVTK
    {
        friend class FITKGraph3DWindowVTK;

        // Register.
        FITKGraphObjectRegist(FITKGraphObjectAnnotation, FITKGraphObjectVTK);
        FITKCLASS(Comp, FITKGraphObjectAnnotation);

    public:
        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        ~FITKGraphObjectAnnotation();

        /**
         * @brief       Initialize graph object.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void initializeObject();

        /**
         * @brief       Show or hide graph object.
         * @param[in]   visibility: The visibility of graph object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setVisible(bool visibility);

        /**
         * @brief       Get the visibility of two actors.
         * @return      The visibility
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-13
         */
        bool getVisibility();

        /**
         * @brief       Update the properties of the actors by the annotation data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        void updateProperties();

        /**
         * @brief       Update the positions of the actors by the annotation data.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        void updatePositions();

        /**
         * @brief       Update the actors by the annotation data.[override]
         * @param[in]   forceUpdate: Force update[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void update(bool forceUpdate = false) override;

        /**
         * @brief       Update the visibility of graph object.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void updateVisibility();

        /**
         * @brief       Start modifying the start anchor by callback.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void modifyStartAnchor();

        /**
         * @brief       Start modifying the end anchor by callback.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void modifyEndAnchor();

        /**
         * @brief       Disable modifying the anchor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void endModifyingAnchor();

        /**
         * @brief       Wether the graph object is being modified.
         * @return      The modify flag
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-13
         */
        bool isModifying();

    private:
        /**
         * @brief       Create new instance.[static]
         * @param[in]   annotation: The annotation data object
         * @return      The instance
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        static FITKGraphObjectAnnotation* New(FITKGraphAnnotation* annotation);

        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        FITKGraphObjectAnnotation(FITKGraphAnnotation* annotation);

        /**
         * @brief       Initialize.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void init();

        /**
         * @brief       Generate the actor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void generateGraph();

    private:
        /**
         * @brief       The flag for initialization.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        bool m_initialized = false;

        /**
         * @brief       The points for arrow.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        vtkPoints* m_pointsArrow{ nullptr };

        /**
         * @brief       The cells for arrow.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        vtkCellArray* m_cellsArrow{ nullptr };

        /**
         * @brief       The poly data for arrow.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        vtkPolyData* m_polyDataArrow{ nullptr };

        /**
         * @brief       The arrow actor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        vtkActor2D* m_fActorArrow{ nullptr };

        /**
         * @brief       The text actor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        vtkTextActor* m_fActorText{ nullptr };

        /**
         * @brief       Wether the start anchor is modifing.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        bool m_movingStart = false;

        /**
         * @brief       Wether the end anchor is modifing.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        bool m_movingEnd = false;

    };

    /**
     * @brief       The manager of the annotation graph object of the graph window.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-05-11
     */
    class FITKRenderWindowVTKAPI FITKGraphObjectAnnotationManager : public QObject
    {
        friend class FITKGraph3DWindowVTK;

        Q_OBJECT
    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        explicit FITKGraphObjectAnnotationManager() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        virtual ~FITKGraphObjectAnnotationManager();

        /**
         * @brief       Append a new annotation graph object.
         * @param[in]   obj: The graph object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        void appendObject(FITKGraphObjectAnnotation* obj);

        /**
         * @brief       Get the number of graph objects.
         * @return      The number of graph objects
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        int getNumberOfObjects();

        /**
         * @brief       Get the graph object by the given index.
         * @param[in]   index: The given index
         * @return      The graph object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        FITKGraphObjectAnnotation* getObjectAt(int index);

        /**
         * @brief       Get the graph object by the given annotation data ID.
         * @param[in]   id: The annotation data id
         * @return      The graph object
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        FITKGraphObjectAnnotation* getObjectByAnnoID(int id);

    protected:
        /**
         * @brief       Remove the graph object.
         * @param[in]   obj: The graph object
         * @param[in]   delData: Wether to delete the object[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        void removeObject(FITKGraphObjectAnnotation* obj, bool delData = false);

    protected slots:
        /**
         * @brief       Slot - The data object has been deleted.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        void slot_dataDeleted(Core::FITKAbstractGraphObject* gobj);

    protected:
        /**
         * @brief       The internal annotation graph object list.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        QList<FITKGraphObjectAnnotation*> m_annoList;

    };
}   // namespace Comp

#endif // __FITKGRAPHOBJECTANNOTATION_H__
