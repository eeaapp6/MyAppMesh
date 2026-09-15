/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKGraphObjectOCC.h
 * @brief   The graph object base for OCC graph widget.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-11
 *********************************************************************/

#ifndef __FITKGRAPHOBJECTOCC_H__
#define __FITKGRAPHOBJECTOCC_H__

#include "FITK_Kernel/FITKCore/FITKAbstractGraphObject.h"

#include "FITKRenderWindowOCCAPI.h"

#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

#include "TypeDefOCCWindow.h"

#include <QObject>

// Forward declaration.
class AIS_InteractiveObject;
class AIS_InteractiveContext;
class V3d_View;

namespace opencascade
{
    template <class T>
    class handle;
}

namespace Core 
{
    class FITKAbstractDataObject;
}

namespace Render
{
    /**
     * @brief   The graph object base for OCC graph widget.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-11
     */
    class FITKRENDERWINDOWOCCAPI FITKGraphObjectOCC : public Core::FITKAbstractGraphObject
    {
        Q_OBJECT

        // Regist.
        FITKCLASS(Render, FITKGraphObjectOCC);     
        FITKGraphObjectRegist(FITKGraphObjectOCC, Core::FITKAbstractGraphObject);

    public:
        /**
         * @brief   Constructor.
         * @param   dataObj: The data object
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        explicit FITKGraphObjectOCC(Core::FITKAbstractDataObject* dataObj);

        /**
         * @brief   Destructor.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        virtual ~FITKGraphObjectOCC();

        /**
         * @brief   Update the graph object.[virtual][override]
         * @param   forceUpdate: Force update[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        virtual void update(bool forceUpdate = false) override;

        /**
         * @brief   Set the interactive context.
         * @param   context: The interactive context
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-13
         */
        void setInteractiveContext(AIS_InteractiveContext* context);

        /**
         * @brief   Set the actived view.
         * @param   view: The interactive context's actived view
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        void setView(V3d_View* view);

        /**
         * @brief   Clear the interactive objects in this graph object.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-04
         */
        void clearInteractiveObjects();

        /**
         * @brief   Add a new interactive object.
         * @param   obj: The interactive object
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        void addInteractiveObj(opencascade::handle<AIS_InteractiveObject> obj);

        /**
         * @brief   Get the interactive object at the index if exist.[virtual]
         * @param   index: The index
         * @return  The interactive object
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-12
         */
        virtual opencascade::handle<AIS_InteractiveObject> getInteractiveObjAt(int index);
 
        /**
         * @brief   Get the number of interactive objects.[virtual]
         * @return  The count
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        virtual int getNumberOfInteractiveObjs();

        /**
         * @brief   Get the visibility from the data object.[virtual]
         * @return  The visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-20
         */
        virtual bool getDataVisibility();

        /**
         * @brief   Get the visibility of the graph object.[virtual]
         * @return  The visibility
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-20
         */
        virtual bool getVisibility();

        /**
         * @brief   Set the select mode for this graph object's interactive objects.[virtual]
         * @param   mode: The select mode
         * @param   force: Force add the select mode[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        virtual void addSelectMode(CommonOCC::SelectMode mode, bool force = false);

        /**
         * @brief   Clear all interactive objects select modes.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        virtual void clearSelectMode();

        /**
         * @brief   Update the graph objects by the saved select modes.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-10
         */
        virtual void updateSelectModes();

        /**
         * @brief   Update the graph objects by the mouse position.[virtual]
         * @param   mx: The mouse x
         * @param   my: The mouse y
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        virtual void updateByMouseMove(int mx, int my);

        /**
         * @brief   Update the graph objects by the mouse picked position.[virtual]
         * @param   mx: The mouse x
         * @param   my: The mouse y
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        virtual void updateByMousePick(int mx, int my);

        /**
         * @brief   Get if the object has the fixed size.
         * @return  Whether has fixed size
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-08
         */
        bool hasFixedSize();

    protected:
        /**
         * @brief   Constructor.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        explicit FITKGraphObjectOCC();
         
    protected:
        /**
         * @brief   The interactive object list.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        QList<opencascade::handle<AIS_InteractiveObject>> m_objs;

        /**
         * @brief   The context for this interactive object.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-13
         */
        AIS_InteractiveContext* m_context{ nullptr };

        /**
         * @brief   The view for this interactive object.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-09-12
         */
        V3d_View* m_view{ nullptr };

        /**
         * @brief   The select mode supportted by this graph object.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        QHash<CommonOCC::SelectMode, bool> m_supportedSelectModeHash;

        /**
         * @brief   The current select modes.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-08-31
         */
        QList<CommonOCC::SelectMode> m_currSelectModes;

        /**
         * @brief   Whether the object has fixed size.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-10-08
         */
        bool m_hasFixedSize = true;

    };
}

#endif // __FITKGRAPHOBJECTOCC_H__
