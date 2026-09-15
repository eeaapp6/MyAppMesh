/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
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
        //文件路径判断
        if (_importFileName.isEmpty() || !QFile::exists(_importFileName))
        {
            return false;
        }

        AppFrame::FITKMessageNormal(QString("Import file from %1 ...").arg(_importFileName));

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
                    // Create the part.
                    FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
                    importedPart->setDataObjectName(fInfo.baseName());
                    importedPart->setShape(shape);
                    m_newCmdIds.push_back(importedPart->getDataObjectID());
                }
            }
            else if (suffix == "stl")
            {
                TopoDS_Shape shape;
                bool flag = StlAPI::Read(shape, _importFileName.toStdString().c_str());

                if (flag)
                {
                    // Create the part.
                    FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
                    importedPart->setDataObjectName(fInfo.baseName());
                    importedPart->setShape(shape);
                    m_newCmdIds.push_back(importedPart->getDataObjectID());
                }
            }
            else if (suffix == "stp" || suffix == "step")
            {
                STEPCAFControl_Reader reader;
                Standard_Integer status = reader.ReadFile(_importFileName.toStdString().c_str());
                if (status != IFSelect_RetDone)
                {
                    return false;
                }

                reader.SetColorMode(true);
                reader.SetNameMode(true);
                reader.SetLayerMode(true);

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

                // Split shapes
                TDF_LabelSequence shapeLabels;

                // GetFreeShapes
                //@{
                shapeTool->GetFreeShapes(shapeLabels);
                for (Standard_Integer i = 1; i <= shapeLabels.Length(); i++)
                {
                    // Get the shapes and attributes
                    const TDF_Label & label = shapeLabels.Value(i);

                    flag = splitShapeLabel(shapeTool.get(), label, TopLoc_Location());
                    if (!flag)
                    {
                        return false;
                    }
                }
            }
            else if (suffix == "igs" || suffix == "iges")
            {
                IGESCAFControl_Reader reader;

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

                // Split shapes
                TDF_LabelSequence shapeLabels;

                // GetShapes
                //@{
                shapeTool->GetFreeShapes(shapeLabels);
                for (Standard_Integer i = 1; i <= shapeLabels.Length(); i++)
                {
                    // Get the shapes and attributes
                    const TDF_Label & label = shapeLabels.Value(i);

                    flag = splitShapeLabel(shapeTool.get(), label, TopLoc_Location());
                    if (!flag)
                    {
                        return false;
                    }
                }
            }
            else
            {
                AppFrame::FITKMessageError(QString("Unknown file format %1").arg(_importFileName));
                return false;
            }
        }
        catch (Standard_Failure const & e)
        {
            AppFrame::FITKMessageError(QString(e.GetMessageString()));
            return false;
        }
        catch (...)
        {
            AppFrame::FITKMessageError(QString("Cannot read the geometry file!"));
            return false;
        }

        AppFrame::FITKMessageNormal(QString("Success import file from %1").arg(_importFileName));

        return true;
    }

    QList<int> FITKOCCModelImportFlattenAssembly::getCreatedCommandIds()
    {
        return m_newCmdIds;
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

    bool FITKOCCModelImportFlattenAssembly::splitShapeLabel(XCAFDoc_ShapeTool* shapeTool, const TDF_Label & parent, TopLoc_Location loc)
    {
        // Get part location
        Handle(XCAFDoc_Location) xLoc;
        bool flag = parent.FindAttribute(XCAFDoc_Location::GetID(), xLoc);
        if (flag)
        {
            // Self * Parent's location
            loc = loc * xLoc->Get();
        }

        // Get the part name.
        //@{
        Handle(TDataStd_Name) name;
        QString partName;
        parent.FindAttribute(TDataStd_Name::GetID(), name);
        if (name)
        {
            partName = QString::fromUtf16(name->Get().ToExtString());
        }
        //@}

        // Check the part name.
        bool isNum = false;
        int partNameTest = partName.toInt(&isNum);
        if (isNum)
        {
            partName = "Geometry_" + partName;
        }

        // If current label is refered by another label,
        // the lable MUST split children labels with the
        // label who refer to it and use the location get
        // by itself.
        TDF_Label ref;
        bool isRef = (shapeTool->IsReference(parent) && shapeTool->GetReferredShape(parent, ref));
        if (isRef)
        {
            splitShapeLabel(shapeTool, ref, loc);
            return true;
        }

        // Split sub-labels
        bool hasChild = parent.HasChild();
        if (hasChild && partName != "COMPOUND")
        {
            TDF_ChildIterator itL(parent);
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

                    // Create the part.
                    FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
                    importedPart->setDataObjectName(partName);
                    importedPart->setShape(shape);
                    m_newCmdIds.push_back(importedPart->getDataObjectID());
                }
                else
                {
                    bool flag = splitShapeLabel(shapeTool, childL, loc);
                    Q_UNUSED(flag);
                }
            }
        }
        else 
        {
            // Get main shape and set the location
            TopoDS_Shape shape;
            XCAFDoc_ShapeTool::GetShape(parent, shape);
            shape.Location(loc);

            // Create the part.
            FITKOCCModelImportedPart* importedPart = new FITKOCCModelImportedPart;
            importedPart->setDataObjectName(partName);
            importedPart->setShape(shape);
            m_newCmdIds.push_back(importedPart->getDataObjectID());
        }

        return true;
    }
}
