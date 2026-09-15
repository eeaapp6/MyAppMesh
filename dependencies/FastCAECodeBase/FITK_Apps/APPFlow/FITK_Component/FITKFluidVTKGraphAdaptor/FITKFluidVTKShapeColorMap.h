/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKFluidVTKShapeColorMap.h
 * @brief   The color array manager for the model's vertex, edge or face.
 *          For FITKGraphObjectShapeVTK only.
 *  
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-06-11
 *********************************************************************/

#ifndef __FITKFLUIDVTKSHAPECOLORMAP_H__
#define __FITKFLUIDVTKSHAPECOLORMAP_H__

#include <QObject>

#include <QHash>
#include <QList>
#include <QColor>
#include "FITKFluidVTKGraphAdaptorAPI.h"


// Forward declaration
class vtkDataSet;
class vtkDataArray;
class vtkUnsignedCharArray;

namespace Exchange
{
    /**
     * @brief   The color array manager for the model's vertex, edge or face.
     *          For FITKGraphObjectShapeVTK only.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-06-11
     */
    class FITKFLUIDGRAPHADAPTORAPI  FITKFluidVTKShapeColorMap : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief   Constructure.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        FITKFluidVTKShapeColorMap() = default;

        /**
         * @brief   Desturcture.[virtual]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        virtual ~FITKFluidVTKShapeColorMap();

        /**
         * @brief   Get the array by the given array name in array map.
         * @param   name: The array name
         * @return  The array
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        vtkDataArray* getMappedArray(QString name);

        /**
         * @brief   Set the input model's data set.
         * @param   dataSet：The data set for model's vertex
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        void setInputData(vtkDataSet* dataSet);

        /**
         * @brief   Set a series cell color to the given array for the data set.
         * @param   name: The color array name
         * @param   indice: The cell indice
         * @param   color: The color of cells
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        void setCellsColor(QString name, const QList<int>& indice, QColor color);

        /**
         * @brief   Set all cells to the given color with the given array.
         * @param   name: The color array name
         * @param   color: The color of cells
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        void setColor(QString name, QColor color);

        /**
         * @brief   Update the color array and modified dataset.
         * @param   forceUpdate: Force update the array's data set[default]
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        void update(bool forceUpdate = false);

        /**
         * @brief   Get the update state.
         * @return  If need to be updated
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        bool needUpdate();

        /**
         * @brief   Clear the color arrays.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-07-25
         */
        void clear();

    private:
        /**
         * @brief   The input data set.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        vtkDataSet* m_dataSet{ nullptr };

        /**
         * @brief   The map of color arrays.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        QHash<QString, vtkUnsignedCharArray*> m_colorArrayHash;

        /**
         * @brief   If the data array need to be updated.
         * @author  ChengHaotian (yeguangbaozi@foxmail.com)
         * @date    2024-06-11
         */
        bool m_needUpdate = false;
    };
}   // namespace Exchange

#endif // __FITKFLUIDVTKSHAPECOLORMAP_H__
