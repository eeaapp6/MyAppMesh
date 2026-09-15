/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFluidVTKShapeColorMap.h"

// VTK
#include <vtkDataSet.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>

namespace Exchange
{
    FITKFluidVTKShapeColorMap::~FITKFluidVTKShapeColorMap()
    {
        // Delete all color arrays.
        clear();

        m_dataSet = nullptr;
    }

    void FITKFluidVTKShapeColorMap::clear()
    {
        // Delete all color arrays.
        for (vtkUnsignedCharArray* colorArray : m_colorArrayHash.values())
        {
            if (colorArray)
            {
                colorArray->Delete();
            }
        }

        m_colorArrayHash.clear();
    }

    vtkDataArray* FITKFluidVTKShapeColorMap::getMappedArray(QString name)
    {
        // Return nullptr if the name does npt has a mapped array.
        if (!m_colorArrayHash.contains(name))
        {
            return nullptr;
        }

        return m_colorArrayHash[name];
    }

    void FITKFluidVTKShapeColorMap::setInputData(vtkDataSet* dataSet)
    {
        m_dataSet = dataSet;
    }

    void FITKFluidVTKShapeColorMap::setCellsColor(QString name, const QList<int>& indice, QColor color)
    {
        if (name.isEmpty() || !m_dataSet)
        {
            return;
        }

        // Get the color array by the given name.
        vtkUnsignedCharArray* colorArray{ nullptr };
        if (m_colorArrayHash.contains(name))
        {
            colorArray = m_colorArrayHash[name];
        }
        else
        {
            // Intialize the color array if the array does'n exist.
            //@{
            colorArray = vtkUnsignedCharArray::New();
            colorArray->SetName(name.toUtf8());
            colorArray->SetNumberOfComponents(3);
            m_colorArrayHash[name] = colorArray;
            //@}

            m_dataSet->GetCellData()->AddArray(colorArray);
            m_needUpdate = true;
        }

        // Check if the color array has been initialized.
        //@{
        int nCell = m_dataSet->GetNumberOfCells();
        int nTup = colorArray->GetNumberOfTuples();

        if (nCell != nTup)
        {
            colorArray->SetNumberOfTuples(nCell);
            m_needUpdate = true;
        }
        //@}

        // Get the integer value of the QColor.
        int r = color.red();
        int g = color.green();
        int b = color.blue();

        for (const int & id : indice)
        {
            if (!m_needUpdate)
            {
                int ro = colorArray->GetComponent(id, 0);
                int go = colorArray->GetComponent(id, 1);
                int bo = colorArray->GetComponent(id, 2);
                
                if (ro != r || go != g || bo != b)
                {
                    m_needUpdate = true;
                }
            }

            colorArray->SetComponent(id, 0, r);
            colorArray->SetComponent(id, 1, g);
            colorArray->SetComponent(id, 2, b);
        }
    }

    void FITKFluidVTKShapeColorMap::setColor(QString name, QColor color)
    {
        if (name.isEmpty() || !m_dataSet)
        {
            return;
        }

        // Get the color array by the given name.
        vtkUnsignedCharArray* colorArray{ nullptr };
        if (m_colorArrayHash.contains(name))
        {
            colorArray = m_colorArrayHash[name];
        }
        else
        {
            // Intialize the color array if the array does'n exist.
            //@{
            colorArray = vtkUnsignedCharArray::New();
            colorArray->SetName(name.toUtf8());
            colorArray->SetNumberOfComponents(3);
            m_colorArrayHash[name] = colorArray;
            //@}

            m_dataSet->GetCellData()->AddArray(colorArray);
        }

        // Check if the color array has been initialized.
        //@{
        int nCell = m_dataSet->GetNumberOfCells();
        int nTup = colorArray->GetNumberOfTuples();

        if (nCell != nTup)
        {
            colorArray->SetNumberOfTuples(nCell);
        }
        //@}

        colorArray->FillComponent(0, color.red());
        colorArray->FillComponent(1, color.green());
        colorArray->FillComponent(2, color.blue());

        m_needUpdate = true;
    }
  
    void FITKFluidVTKShapeColorMap::update(bool forceUpdate)
    {
        // Update the dataset.
        if (!m_dataSet)
        {
            return;
        }

        if (forceUpdate || m_needUpdate)
        {
            m_dataSet->Modified();
        }

        // Reset the flag.
        m_needUpdate = false;
    }

    bool FITKFluidVTKShapeColorMap::needUpdate()
    {
        return m_needUpdate;
    }
}