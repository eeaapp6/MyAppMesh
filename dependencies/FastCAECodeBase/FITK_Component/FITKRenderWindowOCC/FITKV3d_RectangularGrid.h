/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file    FITKV3d_RectangularGrid.h
 * @brief   Subclass of Aspect_RectangularGrid and QObject.( For changing
 *          grid properties. )
 *
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-13
 *********************************************************************/

#ifndef __FITKV3D_RECTANGULARGRID_H__
#define __FITKV3D_RECTANGULARGRID_H__

#include <QObject>
#include <Aspect_RectangularGrid.hxx>

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <gp_Ax3.hxx>
#include <V3d_ViewerPointer.hxx>
#include <Standard_Boolean.hxx>
#include <Aspect_GridDrawMode.hxx>
#include <Standard_Real.hxx>
#include <Graphic3d_Structure.hxx>

// class Aspect_RectangularGrid;
DEFINE_STANDARD_HANDLE(FITKV3d_RectangularGrid, Aspect_RectangularGrid);

/**
 * @brief   The sub-class of V3d_Viewer.
 * @author  ChengHaotian (yeguangbaozi@foxmail.com)
 * @date    2024-09-13
 */
class FITKV3d_RectangularGrid : public QObject, public Aspect_RectangularGrid
{
    Q_OBJECT
public:
    /**
     * @brief   Constructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    FITKV3d_RectangularGrid(const V3d_ViewerPointer& aViewer, const Quantity_Color& aColor, const Quantity_Color& aTenthColor);

    /**
     * @brief   Destructor.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-09-13
     */
    ~FITKV3d_RectangularGrid();

    /**
     * @brief   Set the color for the grid lines.[virtual][override]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    virtual void SetColors(const Quantity_Color& aColor, const Quantity_Color& aTenthColor) Standard_OVERRIDE;

    /**
     * @brief   Show the grid.[virtual][override]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    virtual void Display() Standard_OVERRIDE;

    /**
     * @brief   Hide the grid.[virtual][const][override]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    virtual void Erase() const Standard_OVERRIDE;

    /**
     * @brief   Is the grid shown.[virtual][const][override]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    virtual Standard_Boolean IsDisplayed() const Standard_OVERRIDE;

    /**
     * @brief   Get the grid parameters.[const]
     * @param   XSize: The x size[quote]
     * @param   YSize: The y size[quote]
     * @param   OffSet: The offset[quote]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    void GraphicValues(Standard_Real& XSize, Standard_Real& YSize, Standard_Real& OffSet) const;

    /**
     * @brief   Set the grid parameters.
     * @param   XSize: The x size
     * @param   YSize: The y size
     * @param   OffSet: The offset
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    void SetGraphicValues(const Standard_Real XSize, const Standard_Real YSize, const Standard_Real OffSet);

protected:
    /**
     * @brief   Update the grid.[virtual][override]
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    Standard_EXPORT virtual void UpdateDisplay() Standard_OVERRIDE;

private:
    /**
     * @brief   Draw lines.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    Standard_EXPORT void DefineLines();

    /**
     * @brief   Draw points.
     * @author  ChengHaotian (yeguangbaozi@foxmail.com)
     * @date    2024-10-10
     */
    Standard_EXPORT void DefinePoints();

private:

    //! Custom Graphic3d_Structure implementation.
    class FITKRectangularGridStructure;

private:
    // From V3d_RectangularGrid.
    //@{
    Handle(Graphic3d_Structure) myStructure;
    Handle(Graphic3d_Group) myGroup;
    gp_Ax3 myCurViewPlane;
    V3d_ViewerPointer myViewer;
    Standard_Boolean myCurAreDefined;
    Standard_Boolean myToComputePrs;
    Aspect_GridDrawMode myCurDrawMode;
    Standard_Real myCurXo;
    Standard_Real myCurYo;
    Standard_Real myCurAngle;
    Standard_Real myCurXStep;
    Standard_Real myCurYStep;
    Standard_Real myXSize;
    Standard_Real myYSize;
    Standard_Real myOffSet;
    //@}

};

#endif // __FITKV3D_RECTANGULARGRID_H__