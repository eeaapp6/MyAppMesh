/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelImportFlattenAssembly.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <TopLoc_Location.hxx>
#include <TDF_Label.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_AttributeIterator.hxx>
#include <Standard_Type.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFDoc_Location.hxx>
#include <TNaming_NamedShape.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFApp_Application.hxx>
#include <TDocStd_Document.hxx>
#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>
#include <BinXCAFDrivers_DocumentRetrievalDriver.hxx>
#include <BinXCAFDrivers_DocumentStorageDriver.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <StlAPI.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <IGESCAFControl_Reader.hxx>
#include <Quantity_Color.hxx>
#include <Interface_Static.hxx>

// Qt
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QDateTime>
#include <QDebug>

#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITKOCCModelImportedPart.h"

namespace OCC
{
    FITKOCCModelImportFlattenAssembly::FITKOCCModelImportFlattenAssembly() : OCCShapeAgent(this)
    {

    }

    bool FITKOCCModelImportFlattenAssembly::update()
    {
        // 此功能不支持部件。
        FITKOCCModelPart* part = _occShapeAgent->getPart();
        if (part)
        {
            return false;
        }

        //文件路径判断
        if (_importFileName.isEmpty() || !QFile::exists(_importFileName))
        {
            return false;
        }

        // 清除历史模型。
        this->clear();

        //AppFrame::FITKMessageNormal(QString("Import file from %1 ...").arg(_importFileName));

        QString suffix = _importFileName.split(".").last().toLower();
        QFileInfo fInfo(_importFileName);

        try
        {
            if (suffix == "brep")
            {
                TopoDS_Shape shape;
                BRep_Builder builder;
                bool flag = BRepTools::Read(shape, _importFileName.toStdString().c_str(), builder);

                if (flag)
                {
                    QString name = fInfo.baseName();
                    if (name.isEmpty())
                    {
                        name = "Imported Geometry";
                    }

                    // Create the part.
                    FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
                    importedPart->setDataObjectName(name);
                    importedPart->setShape(shape);
                    this->appendDataObj(importedPart);
                }
            }
            else if (suffix == "stl")
            {
                TopoDS_Shape shape;
                bool flag = StlAPI::Read(shape, _importFileName.toStdString().c_str());

                if (flag)
                {
                    QString name = fInfo.baseName();
                    if (name.isEmpty())
                    {
                        name = "Imported Geometry";
                    }

                    // Create the part.
                    FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
                    importedPart->setDataObjectName(name);
                    importedPart->setShape(shape);
                    this->appendDataObj(importedPart);
                }
            }
            else if (suffix == "stp" || suffix == "step")
            {
                STEPCAFControl_Reader reader;

                //Interface_Static::SetIVal("read.step.shape.relationship", 1);
                //Interface_Static::SetIVal("read.step.assembly.level", 1);
                //Interface_Static::SetIVal("read.step.constructivegeom.relationship", 1);

                reader.SetColorMode(true);
                reader.SetNameMode(true);
                reader.SetLayerMode(true);

                Standard_Integer status = reader.ReadFile(_importFileName.toStdString().c_str());
                if (status != IFSelect_RetDone)
                {
                    return false;
                }

                Handle(TDocStd_Document) doc;
                XCAFApp_Application::GetApplication()->NewDocument("MDTV-XCAF", doc);
                bool flag = reader.Transfer(doc);
                if (!flag)
                {
                    return false;
                }

                TDF_Label root = doc->Main();

                // Get shape label
                TDF_Label shapeLabel;
                flag = getShapeLabel(root, shapeLabel);
                if (!flag || shapeLabel.IsNull())
                {
                    return false;
                }

                Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(root);
                Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(root);

                // Split shapes
                TDF_LabelSequence shapeLabels;

                // GetFreeShapes
                //@{
                shapeTool->GetFreeShapes(shapeLabels);
                for (Standard_Integer i = 1; i <= shapeLabels.Length(); i++)
                {
                    // Get the shapes and attributes
                    const TDF_Label & label = shapeLabels.Value(i);

                    flag = splitShapeLabel(colorTool.get(), shapeTool.get(), TDF_Label(), label, TopLoc_Location(), false);
                    if (!flag)
                    {
                        return false;
                    }
                }
            }
            else if (suffix == "igs" || suffix == "iges")
            {
                IGESCAFControl_Reader reader;

                reader.SetColorMode(true);
                reader.SetNameMode(true);
                reader.SetLayerMode(true);

                Standard_Integer status = reader.ReadFile(_importFileName.toStdString().c_str());
                if (status != IFSelect_RetDone)
                {
                    return false;
                }

                Handle(TDocStd_Document) doc;
                XCAFApp_Application::GetApplication()->NewDocument("MDTV-XCAF", doc);
                bool flag = reader.Transfer(doc);
                if (!flag)
                {
                    return false;
                }

                TDF_Label root = doc->Main();

                // Get shape label
                TDF_Label shapeLabel;
                flag = getShapeLabel(root, shapeLabel);
                if (!flag || shapeLabel.IsNull())
                {
                    return false;
                }

                Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(root);
                Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(root);

                // Split shapes
                TDF_LabelSequence shapeLabels;

                // GetShapes
                //@{
                shapeTool->GetFreeShapes(shapeLabels);
                for (Standard_Integer i = 1; i <= shapeLabels.Length(); i++)
                {
                    // Get the shapes and attributes
                    const TDF_Label & label = shapeLabels.Value(i);

                    flag = splitShapeLabel(colorTool.get(), shapeTool.get(), TDF_Label(), label, TopLoc_Location(), false);
                    if (!flag)
                    {
                        return false;
                    }
                }
            }
            else
            {
                //AppFrame::FITKMessageError(QString("Unknown file format %1").arg(_importFileName));
                return false;
            }
        }
        catch (Standard_Failure const & e)
        {
            Q_UNUSED(e);
            //AppFrame::FITKMessageError(QString(e.GetMessageString()));
            return false;
        }
        catch (...)
        {
            //AppFrame::FITKMessageError(QString("Cannot read the geometry file!"));
            return false;
        }

        //AppFrame::FITKMessageNormal(QString("Success import file from %1").arg(_importFileName));

        return true;
    }

    bool FITKOCCModelImportFlattenAssembly::getShapeLabel(TDF_Label & root, TDF_Label & label)
    {
        // Get shape root label.
        TDF_ChildIterator itL(root);
        for (; itL.More(); itL.Next())
        {
            const TDF_Label childL = itL.Value();

            TDF_AttributeIterator itA(childL);
            for (; itA.More(); itA.Next())
            {
                Handle(TDF_Attribute) childA = itA.Value();
                if (childA->DynamicType() == STANDARD_TYPE(TDataStd_Name))
                {
                    Handle(TDataStd_Name) val = Handle(TDataStd_Name)::DownCast(childA);
                    QString aName = QString::fromUtf16(val->Get().ToExtString());

                    if (aName == "Shapes")
                    {
                        label = childL;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool FITKOCCModelImportFlattenAssembly::splitShapeLabel(XCAFDoc_ColorTool* colorTool, XCAFDoc_ShapeTool* shapeTool,
        const TDF_Label & parent, const TDF_Label & label, TopLoc_Location loc, bool isRefLabel)
    {
        // Get part location
        Handle(XCAFDoc_Location) xLoc;
        bool flag = label.FindAttribute(XCAFDoc_Location::GetID(), xLoc);
        if (flag)
        {
            // Self * Parent's location
            loc = loc * xLoc->Get();
        }

        // Get the part name.
        //@{
        Handle(TDataStd_Name) name;
        QString partName;
        label.FindAttribute(TDataStd_Name::GetID(), name);
        if (name)
        {
            partName = QString::fromUtf16(name->Get().ToExtString());
        }
        //@}

        // Check the part name.
        bool isNum = false;
        partName.toInt(&isNum);
        if (isNum)
        {
            partName = "Imported Geometry " + partName;
        }

        // If current label is refered by another label,
        // the label MUST split children labels with the
        // label who refer to it and use the location get
        // by itself.
        TDF_Label ref;
        bool isRef = (shapeTool->IsReference(label) && shapeTool->GetReferredShape(label, ref));
        if (isRef)
        {
            splitShapeLabel(colorTool, shapeTool, label, ref, loc, true);
            return true;
        }

        // Split sub-labels
        bool hasChild = label.HasChild();
        if (hasChild && partName != "COMPOUND")
        {
            TDF_ChildIterator itL(label);
            for (; itL.More(); itL.Next())
            {
                const TDF_Label & childL = itL.Value();

                // Get main shape.
                TopoDS_Shape shape;
                XCAFDoc_ShapeTool::GetShape(childL, shape);

                if (shapeTool->IsSimpleShape(childL) || shapeTool->IsCompound(childL) ||
                    shape.ShapeType() == TopAbs_ShapeEnum::TopAbs_SOLID ||
                    shape.ShapeType() == TopAbs_ShapeEnum::TopAbs_SHELL)
                {
                    TopLoc_Location locChild = loc;

                    bool flag = childL.FindAttribute(XCAFDoc_Location::GetID(), xLoc);
                    if (flag)
                    {
                        // Self * Parent's location
                        locChild = locChild * xLoc->Get();
                    }

                    // Set the location
                    shape.Location(locChild);

                    // Get color
                    Quantity_Color colorQuan;
                    flag = queryColorRecursively(colorTool, childL, XCAFDoc_ColorType::XCAFDoc_ColorSurf, colorQuan);
                    if (!flag && isRefLabel && !parent.IsNull())
                    {
                        flag = queryColorRecursively(colorTool, parent, XCAFDoc_ColorType::XCAFDoc_ColorSurf, colorQuan);
                    }

                    // Create the part.
                    FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
                    importedPart->setDataObjectName(partName);
                    importedPart->setShape(shape);

                    if (flag)
                    {
                        QColor qc(colorQuan.Red() * 255, colorQuan.Green() * 255, colorQuan.Blue() * 255);
                        importedPart->setColor(qc);
                    }

                    this->appendDataObj(importedPart);
                }
                else
                {
                    bool flag = splitShapeLabel(colorTool, shapeTool, label, childL, loc, false);
                    Q_UNUSED(flag);
                }
            }
        }
        else 
        {
            // Get main shape and set the location
            TopoDS_Shape shape;
            XCAFDoc_ShapeTool::GetShape(label, shape);
            shape.Location(loc);

            // Get color
            Quantity_Color colorQuan;
            flag = queryColorRecursively(colorTool, label, XCAFDoc_ColorType::XCAFDoc_ColorSurf, colorQuan);
            if (!flag && isRefLabel && !parent.IsNull())
            {
                flag = queryColorRecursively(colorTool, parent, XCAFDoc_ColorType::XCAFDoc_ColorSurf, colorQuan);
            }

            // Create the part.
            FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
            importedPart->setDataObjectName(partName);
            importedPart->setShape(shape);

            if (flag)
            {
                QColor qc(colorQuan.Red() * 255, colorQuan.Green() * 255, colorQuan.Blue() * 255);
                importedPart->setColor(qc);
            }
            
            this->appendDataObj(importedPart);
        }

        return true;
    }

    bool FITKOCCModelImportFlattenAssembly::queryColorRecursively(XCAFDoc_ColorTool* colorTool, const TDF_Label& label,
        XCAFDoc_ColorType colorType, Quantity_Color& color)
    {
        if (!colorTool || label.IsNull())
        {
            return false;
        }

        bool ok = colorTool->GetColor(label, colorType, color);
        if (!ok && colorType != XCAFDoc_ColorType::XCAFDoc_ColorGen)
        {
            ok = colorTool->GetColor(label, XCAFDoc_ColorType::XCAFDoc_ColorGen, color);
        }
        if (ok)
        {
            return true;
        }

        TopoDS_Shape shape;
        XCAFDoc_ShapeTool::GetShape(label, shape);
        if (!shape.IsNull())
        {
            ok = colorTool->GetInstanceColor(shape, colorType, color)
                || colorTool->GetColor(shape, colorType, color);
            if (!ok && colorType != XCAFDoc_ColorType::XCAFDoc_ColorGen)
            {
                ok = colorTool->GetInstanceColor(shape, XCAFDoc_ColorType::XCAFDoc_ColorGen, color)
                    || colorTool->GetColor(shape, XCAFDoc_ColorType::XCAFDoc_ColorGen, color);
            }
            if (ok)
            {
                return true;
            }
        }

        const TDF_Label parentLabel = label.Father();
        if (parentLabel.IsNull() || parentLabel == label)
        {
            return false;
        }

        return queryColorRecursively(colorTool, parentLabel, colorType, color);
    }
}
