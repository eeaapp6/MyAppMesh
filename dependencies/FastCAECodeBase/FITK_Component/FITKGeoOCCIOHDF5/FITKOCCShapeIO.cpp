/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCShapeIO.h"

// Geometry
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Component/FITKGeoCompOCC/FITKAbstractOCCModel.h"
#include "FITK_Component/FITKGeoCompOCC/FITKOCCVirtualTopoCreator.h"

// OCC
#include <TopoDS_Shape.hxx>
#include <XCAFApp_Application.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_Label.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDataStd_Name.hxx>
#include <BinXCAFDrivers_DocumentRetrievalDriver.hxx>
#include <BinXCAFDrivers_DocumentStorageDriver.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS_Iterator.hxx>

// Qt
#include <QFile>
#include <QDir>
#include <QApplication>

namespace IO
{
    // Base.
    //@{
    FITKOCCShapeIO::FITKOCCShapeIO()
    {
        // Nothing to do here.
    }

    FITKOCCShapeIO::~FITKOCCShapeIO()
    {
        // Clear data.
        m_topoShape = nullptr;
        m_fileName.clear();
        m_errorText.clear();
    }

    void FITKOCCShapeIO::setFileName(QString fileName)
    {
        // Save the file name.
        m_fileName = fileName;
    }

    QString FITKOCCShapeIO::getFileName()
    {
        return m_fileName;
    }

    void FITKOCCShapeIO::setShape(TopoDS_Shape* shape)
    {
        // Save the shape data pointer.
        m_topoShape = shape;
    }

    TopoDS_Shape* FITKOCCShapeIO::getShape()
    {
        return m_topoShape;
    }

    QString FITKOCCShapeIO::errorInfo()
    {
        // Get the error text.
        return m_errorText;
    }

    bool FITKOCCShapeIO::checkInputData()
    {
        if (m_fileName.isEmpty())
        {
            m_errorText = tr("The file name cannot be empty !");
            return false;
        }

        if (!m_topoShape)
        {
            m_errorText = tr("The shape data cannot be empty !");
            return false;
        }

        return true;
    }

    bool FITKOCCShapeIO::getShapeLabel(TDF_Label& root, TDF_Label &label)
    {
        // Get shape root label
        TDF_ChildIterator iter(root);

        // Get all sub labels.
        for (; iter.More(); iter.Next())
        {
            const TDF_Label childLabel = iter.Value();

            // Get all attributes.
            TDF_AttributeIterator iterAttri(childLabel);
            for (; iterAttri.More(); iterAttri.Next())
            {
                Handle(TDF_Attribute) childAttri = iterAttri.Value();
                if (childAttri->DynamicType() == STANDARD_TYPE(TDataStd_Name))
                {
                    Handle(TDataStd_Name) val = Handle(TDataStd_Name)::DownCast(childAttri);
                    QString aName = QString::fromUtf16(val->Get().ToExtString());

                    if (aName == "Shapes")
                    {
                        label = childLabel;
                        return true;
                    }
                }
            }
        }

        return false;
    }
    //@}

    // Reader.
    //@{
    bool FITKOCCShapeReader::read()
    {
        m_errorText.clear();

        // Check input.       
        bool flag = checkInputData();
        if (!flag)
        {
            return false;
        }

        try
        {
            // Create OCC XCAF bin reader.
            BinXCAFDrivers_DocumentRetrievalDriver binReader;

            // Create XCAD document.
            Handle(TDocStd_Document) doc;
            XCAFApp_Application::GetApplication()->NewDocument("MDTV-XCAF", doc);

            // Read bin file.
            binReader.Read(m_fileName.toUtf8().data(), doc, XCAFApp_Application::GetApplication());

            // Get the document shape tool.
            Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());

            // Get shape from document.
            TDF_Label rootLabel = doc->Main();

            // Get the shape label.
            TDF_Label shapeLabel;
            flag = getShapeLabel(rootLabel, shapeLabel);
            if (!flag)
            {
                m_errorText = tr("Invalid document data, no shape lable in this document !");
                return false;
            }

            // Check the shape labels count.
            int nChild = shapeLabel.NbChildren();
            if (nChild != 1)
            {
                m_errorText = tr("Invalid document data, wrong shape labels number !");
                return false;
            }

            // Get first child label's shape.
            TDF_ChildIterator iter(shapeLabel);
            for (; iter.More(); iter.Next())
            {
                const TDF_Label childLabel = iter.Value();

                TopoDS_Shape childShape = shapeTool->GetShape(childLabel);
                if (childShape.IsNull())
                {
                    m_errorText = tr("Invalid shape data in bin file !");
                    return false;
                }

                // Save the shape data.
                *m_topoShape = childShape;

                break;
            }
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCShapeReader::update()
    {
        return read();
    }
    //@}

    // Writer.
    //@{
    bool FITKOCCShapeWriter::write()
    {
        m_errorText.clear();

        // Check input.       
        bool flag = checkInputData();
        if (!flag)
        {
            return false;
        }

        // Check the shape.
        if (m_topoShape->IsNull())
        {
            m_errorText = tr("The shape cannot be null !");
            return false;
        }

        try
        {
            // Create OCC XCAF bin writer.
            BinXCAFDrivers_DocumentStorageDriver binWriter;

            // Create XCAD document.
            Handle(TDocStd_Document) doc;
            XCAFApp_Application::GetApplication()->NewDocument("MDTV-XCAF", doc);

            // Get the document shape tool.
            TDF_Label rootLabel = doc->Main();
            Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(rootLabel);

            // Get the shape label.
            TDF_Label shapeLabel;
            flag = getShapeLabel(rootLabel, shapeLabel);
            if (!flag)
            {
                m_errorText = tr("Invalid document data, no shape lable in this document !");
                return false;
            }

            // Create a shape label.
            TDF_Label newShapeLabel = shapeLabel.NewChild();

            // Add shape to document.
            shapeTool->SetShape(newShapeLabel, *m_topoShape);

            // Write bin file.
            binWriter.Write(doc, m_fileName.toUtf8().data());
        }
        catch (...)
        {
            return false;
        }

        return true;
    }

    bool FITKOCCShapeWriter::update()
    {
        return write();
    }
    //@}

    // Tools.
    //@{
    bool FITKOCCIOTool::ShapeToData(TopoDS_Shape* shape, uint & len, char* & data, QString fileName)
    {
        // Check the input.
        if (!shape)
        {
            return false;
        }

        // Prepare the temp file folder.
        if (fileName.isEmpty())
        {
            QString folderPath = qApp->applicationDirPath() + "/temp";
            QDir dir(folderPath);
            if (!dir.exists())
            {
                dir.mkpath(folderPath);
            }

            fileName = qApp->applicationDirPath() + "/temp/bin.temp";
        }

        // Use OCC shape bin writer.
        FITKOCCShapeWriter writer;
        writer.setFileName(fileName);
        writer.setShape(shape);
        writer.update();

        // Read the raw data file.
        //@{
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }
      
        // Read binary data.
        len = file.size();
        data = new char[len];
        file.read(data, len);
        file.close();
        //@}

        // Remove temp file.
        QFile::remove(fileName);

        return true;
    }

    bool FITKOCCIOTool::DataToShape(TopoDS_Shape* shape, uint len, char* data, QString fileName)
    {
        // Check the input.
        if (!shape || !data || len == 0)
        {
            return false;
        }

        // Prepare the temp file folder.
        if (fileName.isEmpty())
        {
            QString folderPath = qApp->applicationDirPath() + "/temp";
            QDir dir(folderPath);
            if (!dir.exists())
            {
                dir.mkpath(folderPath);
            }

            fileName = qApp->applicationDirPath() + "/temp/bin.temp";
        }

        // Write the raw data to file.
        //@{
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            return false;
        }

        // Write binary data.
        file.write(data, len);
        file.close();
        //@}

        // Use OCC shape bin reader.
        FITKOCCShapeReader reader;
        reader.setFileName(fileName);
        reader.setShape(shape);
        bool flag = reader.update();

        // Remove temp file.
        QFile::remove(fileName);

        return flag;
    }

    bool FITKOCCIOTool::SetDataToCommand(Interface::FITKAbsGeoCommand* cmd, uint len, char* data, QString fileName)
    {
        // Check the input data first.
        if (!cmd)
        {
            return false;
        }

        // Get the agent's shape data.
        //@{
        OCC::FITKAbstractOCCModel* occModel = dynamic_cast<OCC::FITKAbstractOCCModel*>(cmd->getShapeAgent());
        if (!occModel)
        {
            return false;
        }

        TopoDS_Shape* shape = occModel->getShape();
        //@}

        // Get virtual topo managers.
        Interface::FITKVirtualTopoManager* vMgrs = occModel->getVirtualTopoManager();

        // Get all types of virtual topo.( except assembly )
        QList<Interface::FITKGeoEnum::VTopoShapeType> types = vMgrs->getTopoTypes();
        types.removeOne(Interface::FITKGeoEnum::VTopoShapeType::VSAssembly);

        if (!shape || !vMgrs || types.isEmpty())
        {
            return false;
        }

        // Convert the data to the TopoDS_Shape.
        bool flag = FITKOCCIOTool::DataToShape(shape, len, data, fileName);
        if (!flag)
        {
            return false;
        }

        // Initialize index maps.
        //@{
        TopTools_IndexedMapOfShape iVert, iEdge, iWire, iFace, iShell, iSolid;

        TopExp::MapShapes(*shape, TopAbs_VERTEX, iVert);
        TopExp::MapShapes(*shape, TopAbs_EDGE, iEdge);
        TopExp::MapShapes(*shape, TopAbs_WIRE, iWire);
        TopExp::MapShapes(*shape, TopAbs_FACE, iFace);
        TopExp::MapShapes(*shape, TopAbs_SHELL, iShell);
        TopExp::MapShapes(*shape, TopAbs_SOLID, iSolid);

        // The enum map for virtual topo shape type and OCC shape index map.
        QHash<Interface::FITKGeoEnum::VTopoShapeType, TopTools_IndexedMapOfShape> enumMap =
        {
            {Interface::FITKGeoEnum::VSPoint, iVert},
            {Interface::FITKGeoEnum::VSEdge, iEdge},
            {Interface::FITKGeoEnum::VSWire, iWire},
            {Interface::FITKGeoEnum::VSFace, iFace},
            {Interface::FITKGeoEnum::VSShell, iShell},
            {Interface::FITKGeoEnum::VSSolid, iSolid},
        };
        //@}

        // Set the base shape to virtual topos.
        for (const Interface::FITKGeoEnum::VTopoShapeType & type : types)
        {
            Interface::FITKShapeVirtualTopoManager* shapeTopoMgr =  vMgrs->getShapeVirtualTopoManager(type);
            if (!shapeTopoMgr)
            {
                return false;
            }

            // Get the index map.
            TopTools_IndexedMapOfShape & map = enumMap[type];

            int nTopo = shapeTopoMgr->getDataCount();
            for (int i = 0; i < nTopo; i++)
            {
                Interface::FITKAbsVirtualTopo* vTopo = shapeTopoMgr->getDataByIndex(i);
                if (!vTopo)
                {
                    return false;
                }

                // Get the shape by index.
                const TopoDS_Shape & subShape = map.FindKey(vTopo->getIndexLabel() + 1);
                vTopo->setShape(new OCC::FITKOCCTopoShape(subShape));

                // Update the shape hash code.
                const int sHash = subShape.HashCode(std::numeric_limits<int>::max());
                vTopo->setTag(sHash);
            }
        }

        // Set the composite shape to virtual topos.
        //@{
        Interface::FITKShapeVirtualTopoManager* assemblyTopoMgr = vMgrs->
            getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSAssembly);

        TopAbs_ShapeEnum shapeType = shape->ShapeType();
        if (shapeType == TopAbs_ShapeEnum::TopAbs_COMPOUND || shapeType == TopAbs_ShapeEnum::TopAbs_COMPSOLID)
        {
            // Initialize the root topo.
            // P.S. 
            // If the shape is an compound or compsolid, the first virtual topo
            // will be the root virtual topo which contains the whole shape data.
            Interface::FITKAbsVirtualTopo* rootTopo = assemblyTopoMgr->getDataByIndex(0);
            if (rootTopo)
            {
                // Create the tools for iterator.
                FITKOCCIOTool* tool = new FITKOCCIOTool;

                rootTopo->setShape(new OCC::FITKOCCTopoShape(*shape));
                tool->m_indexOfAss++;

                // Update the shape hash code.
                const int sHash = shape->HashCode(std::numeric_limits<int>::max());
                rootTopo->setTag(sHash);

                recurSplitAssembly(*shape, assemblyTopoMgr, tool);
                delete tool;
            }
        }
        //@}

        return true;
    }

    void FITKOCCIOTool::recurSplitAssembly(const TopoDS_Shape & shape, Interface::FITKShapeVirtualTopoManager* assemblyTopoMgr, FITKOCCIOTool* tool)
    {
        TopoDS_Iterator iter(shape);
        for (; iter.More(); iter.Next())
        {
            // Get the composite shape.
            const TopoDS_Shape & subShape = iter.Value();

            // Skip the base shapes.
            TopAbs_ShapeEnum subShapeType = subShape.ShapeType();
            if (subShapeType != TopAbs_ShapeEnum::TopAbs_COMPOUND &&
                subShapeType != TopAbs_ShapeEnum::TopAbs_COMPSOLID)
            {
                continue;
            }

            int currIndex = tool->m_indexOfAss;
            tool->m_indexOfAss++;
            Interface::FITKAbsVirtualTopo* vTopo = assemblyTopoMgr->getDataByIndex(currIndex);
            if (!vTopo)
            {
                continue;
            }

            vTopo->setShape(new OCC::FITKOCCTopoShape(subShape));

            // Update the shape hash code.
            const int sHash = subShape.HashCode(std::numeric_limits<int>::max());
            vTopo->setTag(sHash);

            if (subShape != shape)
            {
                recurSplitAssembly(subShape, assemblyTopoMgr, tool);
            }
            else
            {
                recurSplitAssembly(subShape, assemblyTopoMgr, tool);
            }
        }
    }

    FITKOCCIOTool::FITKOCCIOTool()
    {

    }

    FITKOCCIOTool::~FITKOCCIOTool()
    {

    }
}   // namespace IO
