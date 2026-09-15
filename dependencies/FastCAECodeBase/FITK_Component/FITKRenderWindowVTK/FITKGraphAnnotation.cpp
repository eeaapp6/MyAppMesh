/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGraphAnnotation.h"

// Qt
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

// VTK
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>

namespace Comp
{
    FITKGraphAnnotation::FITKGraphAnnotation(FITKGraph3DWindowVTK* window) : m_graphWindow(window)
    {
        // Create the default properties.
        m_arrowProperty = vtkProperty2D::New();
        m_arrowProperty->SetColor(1., 1., 1.);
        m_arrowProperty->SetLineWidth(1.);

        m_textProperty = vtkTextProperty::New();
        m_textProperty->SetColor(1., 1., 1.);
        m_textProperty->SetFontSize(20);
        m_textProperty->SetBold(false);
        m_textProperty->SetItalic(false);
        m_textProperty->SetBackgroundColor(0., 0., 0.);
        m_textProperty->SetBackgroundOpacity(0.);
        m_textProperty->SetFrameColor(0.8, 0.8, 0.8);
        m_textProperty->SetFrameWidth(2);
        m_textProperty->SetJustificationToLeft();
    }

    FITKGraphAnnotation::~FITKGraphAnnotation()
    {
        // Delete the properties.
        if (m_arrowProperty)
        {
            m_arrowProperty->Delete();
            m_arrowProperty = nullptr;
        }

        if (m_textProperty)
        {
            m_textProperty->Delete();
            m_textProperty = nullptr;
        }
    }

    void FITKGraphAnnotation::copy(FITKGraphAnnotation* from)
    {
        if (!from)
        {
            return;
        }

        setStartAnchor(from->getStartAnchor());
        setEndAnchor(from->getEndAnchor());
        setWithArrow(from->getWithArrow());
        setArrowColor(from->getArrowColor());
        setArrowThickness(from->getArrowThickness());
        setWithText(from->getWithText());
        setFontColor(from->getFontColor());
        setFontSize(from->getFontSize());
        setFontBold(from->getFontBold());
        setFontFamily(from->getFontFamily());
        setUseTextBound(from->getUseTextBound());
        setUseTextBackground(from->getUseTextBackground());
        setTextBackgroundColor(from->getTextBackgroundColor());
        setTextAlign(from->getTextAlign());
        setText(from->getText());
    }

    void FITKGraphAnnotation::reset()
    {
        // setWithArrow(false);
        // setWithText(false);
        setArrowColor(Qt::white);
        setArrowThickness(1.);
        // setText("Text");
        setFontColor(Qt::white);
        setFontSize(25);
        setFontBold(false);
        setUseTextBound(false);
        setFontFamily(TextFont::TF_Simhei);
        setUseTextBackground(false);
        setTextBackgroundColor(Qt::black);
        setTextAlign(TextAlign::TA_Left);
        endModifyingAnchor();
    }

    FITKGraph3DWindowVTK* FITKGraphAnnotation::getGraph3DWindow() const
    {
        return m_graphWindow;
    }

    void FITKGraphAnnotation::setWithArrow(bool withArrow)
    {
        m_withArrow = withArrow;
    }

    bool FITKGraphAnnotation::getWithArrow() const
    {
        return m_withArrow;
    }

    void FITKGraphAnnotation::setWithText(bool withText)
    {
        m_withText = withText;
    }

    bool FITKGraphAnnotation::getWithText() const
    {
        return m_withText;
    }

    void FITKGraphAnnotation::setText(const QString& text)
    {
        m_text = text;
    }

    const QString& FITKGraphAnnotation::getText() const
    {
        return m_text;
    }

    void FITKGraphAnnotation::setStartAnchor(const AnnoAnchor& anchor)
    {
        m_startAnchor = anchor;
    }

    const AnnoAnchor& FITKGraphAnnotation::getStartAnchor() const
    {
        return m_startAnchor;
    }

    void FITKGraphAnnotation::setEndAnchor(const AnnoAnchor& anchor)
    {
        m_endAnchor = anchor;
    }

    const AnnoAnchor& FITKGraphAnnotation::getEndAnchor() const
    {
        return m_endAnchor;
    }

    void FITKGraphAnnotation::setArrowColor(QColor color)
    {
        // Set the color to the arrow property.
        if (m_arrowProperty)
        {
            m_arrowProperty->SetColor(color.redF(), color.greenF(), color.blueF());
        }
    }

    QColor FITKGraphAnnotation::getArrowColor() const
    {
        if (!m_arrowProperty)
        {
            return QColor();
        }

        // Get the color from the arrow property.
        double* c3 = m_arrowProperty->GetColor();
        QColor color = QColor::fromRgbF(c3[0], c3[1], c3[2]);
        return color;
    }

    void FITKGraphAnnotation::setArrowThickness(double thickness)
    {
        // Set the line width to the arrow property.
        if (m_arrowProperty)
        {
            m_arrowProperty->SetLineWidth(thickness);
        }
    }

    double FITKGraphAnnotation::getArrowThickness() const
    {
        // Get the line width from the arrow property.
        if (!m_arrowProperty)
        {
            return 0.;
        }

        return m_arrowProperty->GetLineWidth();
    }

    void FITKGraphAnnotation::setFontFamily(TextFont ff)
    {
        m_font = ff;
    }

    TextFont FITKGraphAnnotation::getFontFamily()
    {
        return m_font;
    }

    void FITKGraphAnnotation::setFontColor(QColor color)
    {
        // Set the color to the text property.
        if (m_textProperty)
        {
            m_textProperty->SetColor(color.redF(), color.greenF(), color.blueF());
        }
    }

    QColor FITKGraphAnnotation::getFontColor() const
    {
        if (!m_textProperty)
        {
            return QColor();
        }

        // Get the color from the text property.
        double* c3 = m_textProperty->GetColor();
        QColor color = QColor::fromRgbF(c3[0], c3[1], c3[2]);
        return color;
    }

    void FITKGraphAnnotation::setFontSize(int fontSize)
    {
        // Set the font size to the text property.
        if (m_textProperty)
        {
            m_textProperty->SetFontSize(fontSize);
        }
    }

    int FITKGraphAnnotation::getFontSize() const
    {
        // Get the font size from the text property.
        if (!m_textProperty)
        {
            return 0;
        }

        return m_textProperty->GetFontSize();
    }

    void FITKGraphAnnotation::setFontBold(bool bold)
    {
        // Set the bold flag to the text property.
        if (m_textProperty)
        {
            m_textProperty->SetBold(bold);
        }
    }

    bool FITKGraphAnnotation::getFontBold() const
    {
        // Get the bold flag from the text property.
        if (!m_textProperty)
        {
            return false;
        }

        return m_textProperty->GetBold();
    }

    void FITKGraphAnnotation::setUseTextBound(bool flag)
    {
        // Set the bold flag to the text property.
        if (m_textProperty)
        {
            m_textProperty->SetFrame(flag);
        }
    }

    bool FITKGraphAnnotation::getUseTextBound()
    {
        // Get the frame flag from the text property.
        if (!m_textProperty)
        {
            return false;
        }

        return m_textProperty->GetFrame();
    }

    void FITKGraphAnnotation::setUseTextBackground(bool flag)
    {
        // Set the background opacity to the text property.
        if (m_textProperty)
        {
            m_textProperty->SetBackgroundOpacity(flag ? 1. : 0.);
        }
    }

    bool FITKGraphAnnotation::getUseTextBackground()
    {
        // Get the background opacity from the text property.
        if (!m_textProperty)
        {
            return false;
        }

        double opa = m_textProperty->GetBackgroundOpacity();
        return !qFuzzyIsNull(opa);
    }

    void FITKGraphAnnotation::setTextBackgroundColor(QColor color)
    {
        // Set the background color to the text property.
        if (m_textProperty)
        {
            m_textProperty->SetBackgroundColor(color.redF(), color.greenF(), color.blueF());
        }
    }

    QColor FITKGraphAnnotation::getTextBackgroundColor()
    {
        // Get the background color from the text property.
        if (!m_textProperty)
        {
            return QColor();
        }

        double rgba[4]{ 0., 0., 0., 0. };
        m_textProperty->GetBackgroundRGBA(rgba);
        QColor color = QColor::fromRgbF(rgba[0], rgba[1], rgba[2]);
        return color;
    }

    void FITKGraphAnnotation::setTextAlign(TextAlign type)
    {
        // Set the text align method to the text property.
        if (m_textProperty)
        {
            switch (type)
            {
            case TA_Left:
            {
                m_textProperty->SetJustificationToLeft();
                break;
            }
            case TA_Center:
            {
                m_textProperty->SetJustificationToCentered();
                break;
            }
            case TA_Right:
            {
                m_textProperty->SetJustificationToRight();
                break;
            }
            default:
                return;
            }
        }
    }

    TextAlign FITKGraphAnnotation::getTextAlign()
    {
        // Get the text align method to the text property.
        if (!m_textProperty)
        {
            return TextAlign::TA_Left;
        }

        switch (m_textProperty->GetJustification())
        {
        case VTK_TEXT_LEFT:
        {
            return TextAlign::TA_Left;
        }
        case VTK_TEXT_CENTERED:
        {
            return TextAlign::TA_Center;
        }
        case VTK_TEXT_RIGHT:
        {
            return TextAlign::TA_Right;
        }
        }

        return TextAlign::TA_Left;
    }

    void FITKGraphAnnotation::updateProperty(vtkProperty2D* arrowProperty)
    {
        if (!arrowProperty || !m_arrowProperty)
        {
            return;
        }

        // Copy the properties to the arrow property.
        arrowProperty->SetColor(m_arrowProperty->GetColor());
        arrowProperty->SetLineWidth(m_arrowProperty->GetLineWidth());
    }

    void FITKGraphAnnotation::updateProperty(vtkTextProperty* textProperty)
    {
        if (!textProperty || !m_textProperty)
        {
            return;
        }

        // Change the font.
        QString tempFolder = qApp->applicationDirPath() + "/vz_Temp/";
        QString fontFileName;

        bool useCustomFont = false;
        switch (m_font)
        {
        case TF_Simhei:
        {
            fontFileName = "simhei.ttf";
            break;
        }
        case TF_Simsun:
        {
            fontFileName = "simsun.ttc";
            break;
        }
        case TF_WQY_Micro:
        {
            fontFileName = "Wenquanyi Micron Black.ttc";
            break;
        }
        default:
        {
            break;
        }
        }

        QString fontFilePath = tempFolder + fontFileName;
        if (!QFile::exists(fontFilePath))
        {
            QString fontQrc = ":/fonts/" + fontFileName;
            if (QFile::exists(fontQrc))
            {
                QDir dir(tempFolder);
                if (!dir.exists())
                {
                    dir.mkpath(tempFolder);
                }

                QFile::copy(fontQrc, fontFilePath);
            }
        }

        if (QFile::exists(fontFilePath))
        {
            useCustomFont = true;
        }

        if (useCustomFont)
        {
            textProperty->SetFontFamily(VTK_FONT_FILE);
            textProperty->SetFontFile(fontFilePath.toUtf8());
        }
        else
        {
            textProperty->SetFontFamilyToArial();
        }

        // Copy the properties to the text property.
        textProperty->SetColor(m_textProperty->GetColor());
        textProperty->SetFontSize(m_textProperty->GetFontSize());
        textProperty->SetBold(m_textProperty->GetBold());
        textProperty->SetBackgroundOpacity(m_textProperty->GetBackgroundOpacity());
        textProperty->SetBackgroundColor(m_textProperty->GetBackgroundColor());
        textProperty->SetJustification(m_textProperty->GetJustification());
        textProperty->SetFrameColor(m_textProperty->GetFrameColor());
        textProperty->SetFrameWidth(m_textProperty->GetFrameWidth());
        textProperty->SetFrame(m_textProperty->GetFrame());
    }

    void FITKGraphAnnotation::update(bool forceUpdate)
    {
        emit sig_dataUpdated(forceUpdate);
    }

    void FITKGraphAnnotation::modifyStartAnchor()
    {
        emit sig_modifyStart();
    }

    void FITKGraphAnnotation::modifyEndAnchor()
    {
        emit sig_modifyEnd();
    }

    void FITKGraphAnnotation::endModifyingAnchor()
    {
        emit sig_stopModifying();
    }

    void FITKGraphAnnotation::setEventPostionRatio(double posRatio2[2])
    {
        m_eventPosRatio2[0] = posRatio2[0];
        m_eventPosRatio2[1] = posRatio2[1];
    }

    void FITKGraphAnnotation::getEventPostionRatio(double posRatio2[2])
    {
        posRatio2[0] = m_eventPosRatio2[0];
        posRatio2[1] = m_eventPosRatio2[1];
    }

    void FITKGraphAnnotation::setIsEditing(bool flag)
    {
        m_isEditing = flag;
    }

    bool FITKGraphAnnotation::getIsEditing()
    {
        return m_isEditing;
    }
}   // namespace Comp
