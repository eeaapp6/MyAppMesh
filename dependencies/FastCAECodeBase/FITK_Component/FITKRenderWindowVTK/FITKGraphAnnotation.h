/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKGraphAnnotation.h
 * @brief       The annotation data of the graph window.
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-05-09
 *********************************************************************/

#ifndef __FITKGRAPHANNOTATION_H__
#define __FITKGRAPHANNOTATION_H__

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

#include "FITKRenderWindowVTKAPI.h"

#include <QColor>

 // Forward declaration.
class vtkProperty2D;
class vtkTextProperty;

namespace Comp
{
    class FITKGraph3DWindowVTK;
}

/**
 * @brief       The type of the anchor.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-05-09
 */
enum AnnoAnchorType
{
    ViewportRatioXY = 0,
    WorldXYZ = 1,
};
Q_DECLARE_METATYPE(AnnoAnchorType)

/**
 * @brief       The anchor of the annotation.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-05-09
 */
struct AnnoAnchor
{
    // The anchor type.
    AnnoAnchorType Type = AnnoAnchorType::WorldXYZ;

    // The anchor position. 
    // If the type is ViewportRatioXY, the x and y components are in the range [0, 100].
    // 
    // y
    // ↑   100,100
    // 丨
    // 丨
    // 0,0 ——→ x
    // 
    // If the type is WorldXYZ, the components are in world coordinates.
    double Pos[3] = { 0. ,0. ,0. };

};
Q_DECLARE_METATYPE(AnnoAnchor)

/**
 * @brief       The text align method.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-05-14
 */
enum TextAlign
{
    TA_Left = 0,
    TA_Center,
    TA_Right
};
Q_DECLARE_METATYPE(TextAlign)

/**
 * @brief       The supportted font for the annotation text.
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2026-05-20
 */
enum TextFont
{
    TF_SystemDEfault = -1,  // Arial
    TF_Simhei = 0,          // 黑体
    TF_Simsun,              // 宋体
    TF_WQY_Micro            // 文泉驿微米黑
};
Q_DECLARE_METATYPE(TextFont)

namespace Comp
{
    /**
     * @brief       The annotation data of the graph window.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-05-09
     */
    class FITKRenderWindowVTKAPI FITKGraphAnnotation : public Core::FITKAbstractNDataObject
    {
        friend class FITKGraph3DWindowVTK;

        Q_OBJECT

    public:
        /**
         * @brief       Destructor.[virtual]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        virtual ~FITKGraphAnnotation();

        /**
         * @brief       Copy data.
         * @param[in]   The data copied from
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void copy(FITKGraphAnnotation* from);

        /**
         * @brief       Reset to default properties.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-21
         */
        void reset();

        /**
         * @brief       Get the graph 3D window that the annotation belongs to.
         * @return      The graph 3D window
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        FITKGraph3DWindowVTK* getGraph3DWindow() const;

        /**
         * @brief       Set whether to show the arrow.
         * @param[in]   withArrow: Whether to show the arrow
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setWithArrow(bool withArrow);

        /**
         * @brief       Get whether to show the arrow.
         * @return      Whether to show the arrow
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        bool getWithArrow() const;

        /**
         * @brief       Set whether to show the text.
         * @param[in]   withText: Whether to show the text
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setWithText(bool withText);

        /**
         * @brief       Get whether to show the text.
         * @return      Whether to show the text
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        bool getWithText() const;

        /**
         * @brief       Set the text content of the annotation.
         * @param[in]   text: The text content of the annotation[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setText(const QString& text);

        /**
         * @brief       Get the text content of the annotation.
         * @return      The text content of the annotation[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        const QString& getText() const;

        /**
         * @brief       Set the start anchor of the annotation.
         * @param[in]   anchor: The start anchor of the annotation[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setStartAnchor(const AnnoAnchor& anchor);

        /**
         * @brief       Get the start anchor of the annotation.
         * @return      The start anchor of the annotation[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        const AnnoAnchor& getStartAnchor() const;

        /**
         * @brief       Set the end anchor of the annotation.
         * @param[in]   anchor: The end anchor of the annotation[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setEndAnchor(const AnnoAnchor& anchor);

        /**
         * @brief       Get the end anchor of the annotation.
         * @return      The end anchor of the annotation[quote]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        const AnnoAnchor& getEndAnchor() const;

        /**
         * @brief       Set the arrow color of the annotation.
         * @param[in]   color: The arrow color of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setArrowColor(QColor color);

        /**
         * @brief       Get the arrow color of the annotation.
         * @return      The arrow color of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        QColor getArrowColor() const;

        /**
         * @brief       Set the arrow thickness of the annotation.
         * @param[in]   thickness: The arrow thickness of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setArrowThickness(double thickness);

        /**
         * @brief       Get the arrow thickness of the annotation.
         * @return      The arrow thickness of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        double getArrowThickness() const;

        /**
         * @brief       Set the font family.
         * @param[in]   ff: The font family
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-20
         */
        void setFontFamily(TextFont ff);

        /**
         * @brief       Get the font family.
         * @return      The font family
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-20
         */
        TextFont getFontFamily();

        /**
         * @brief       Set the font color of the annotation.
         * @param[in]   color: The font color of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setFontColor(QColor color);

        /**
         * @brief       Get the font color of the annotation.
         * @return      The font color of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        QColor getFontColor() const;

        /**
         * @brief       Set the font size of the annotation.
         * @param[in]   fontSize: The font size of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setFontSize(int fontSize);

        /**
         * @brief       Get the font size of the annotation.
         * @return      The font size of the annotation
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        int getFontSize() const;

        /**
         * @brief       Set whether the font is bold.
         * @param[in]   bold: Whether the font is bold
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void setFontBold(bool bold);

        /**
         * @brief       Get whether the font is bold.
         * @return      Whether the font is bold
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        bool getFontBold() const;

        /**
         * @brief       Set wether the text has a bound box.
         * @param[in]   flag: Wether the text has a bound box
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        void setUseTextBound(bool flag);

        /**
         * @brief       Get wether the text has a bound box.
         * @return      Wether the text has a bound box
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        bool getUseTextBound();

        /**
         * @brief       Set wether the text has background.
         * @param[in]   flag: Wether the text has background
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        void setUseTextBackground(bool flag);

        /**
         * @brief       Get wether the text has background.
         * @return      Wether the text has background
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        bool getUseTextBackground();

        /**
         * @brief       Set the text background color.
         * @param[in]   color: The text background color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        void setTextBackgroundColor(QColor color);

        /**
         * @brief       Get the text background color.
         * @return      The text background color
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        QColor getTextBackgroundColor();

        /**
         * @brief       Set the text align method.
         * @param[in]   type: The text align method.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        void setTextAlign(TextAlign type);

        /**
         * @brief       Get the text align method.
         * @return      The text align method.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-14
         */
        TextAlign getTextAlign();

        /**
         * @brief       Update the given property to arrow property of the annotation.
         * @param[in]   arrowProperty: The property to be updated
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void updateProperty(vtkProperty2D* arrowProperty);

        /**
         * @brief       Update the given property to text property of the annotation.
         * @param[in]   textProperty: The property to be updated
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        void updateProperty(vtkTextProperty* textProperty);

        /**
         * @brief       Update the data and graph object.
         * @param[in]   forceUpdate: Force to update the data[default]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        void update(bool forceUpdate = false);

        /**
         * @brief       Start modifying the start anchor by mouse move.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void modifyStartAnchor();

        /**
         * @brief       Start modifying the end anchor by mouse move.
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
         * @brief       Save the temp mouse event position window ratio[0, 100].
         * @param[in]   pos2: The mouse event position
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void setEventPostionRatio(double posRatio2[2]);

        /**
         * @brief       Get the temp mouse event position window ratio[0, 100].
         * @param[in]   pos2: The mouse event position
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void getEventPostionRatio(double posRatio2[2]);

        /**
         * @brief       Save the flag for editing.
         * @param[in]   flag: The editing flag
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void setIsEditing(bool flag);

        /**
         * @brief       Save the flag for editing.
         * @return      The editing flag
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        bool getIsEditing();

    signals:
        /**
         * @brief       Signal - Update the data and its graph object.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        void sig_dataUpdated(bool forceUpdate = false);

        /**
         * @brief       Signal - Start modifying the start anchor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void sig_modifyStart();

        /**
         * @brief       Signal - Start modifying the end anchor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void sig_modifyEnd();

        /**
         * @brief       Signal - Stop modifying the anchor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        void sig_stopModifying();

    private:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        FITKGraphAnnotation(FITKGraph3DWindowVTK* window);

    private:
        /**
         * @brief       The graph window to which the annotation belongs.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-11
         */
        FITKGraph3DWindowVTK* m_graphWindow{ nullptr };

        /**
         * @brief       Wether to show the arrow.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        bool m_withArrow = false;

        /**
         * @brief       Wether to show the text.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        bool m_withText = false;

        /**
         * @brief       The text content of the annotation.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        QString m_text;

        /**
         * @brief       The start anchor of the annotation.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        AnnoAnchor m_startAnchor;

        /**
         * @brief       The end anchor of the annotation.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        AnnoAnchor m_endAnchor;

        /**
         * @brief       The font of the text label.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-20
         */
        TextFont m_font = TextFont::TF_Simhei;

        /**
         * @brief       The arrow property of the annotation.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        vtkProperty2D* m_arrowProperty{ nullptr };

        /**
         * @brief       The text property of the annotation.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        vtkTextProperty* m_textProperty{ nullptr };

        /**
         * @brief       The saved mosue event position window ratio[0, 100].
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        double m_eventPosRatio2[2]{ 0, 0 };

        /**
         * @brief       The editing flag.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-12
         */
        bool m_isEditing = false;

    };

    /**
     * @brief       The manager of the annotation data of the graph window.
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2026-05-09
     */
    class FITKRenderWindowVTKAPI FITKGraphAnnotationManager
        : public Core::FITKAbstractDataManager<FITKGraphAnnotation>
    {
    public:
        /**
         * @brief       Constructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        explicit FITKGraphAnnotationManager() = default;

        /**
         * @brief       Destructor.
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2026-05-09
         */
        virtual ~FITKGraphAnnotationManager() = default;

    };
}   // namespace Exchange

#endif // __FITKGRAPHANNOTATION_H__
