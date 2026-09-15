/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKXMLMultiBlockDataWriter.h"

#include <vtkObjectFactory.h>
#include <vtkXMLDataElement.h>
#include <vtkCompositeDataSet.h>
#include <vtkDataObjectTreeIterator.h>
#include <vtkDataObjectTree.h>
#include <vtkInformation.h>

vtkStandardNewMacro(FITKXMLMultiBlockDataWriter);

void FITKXMLMultiBlockDataWriter::setComNameByIndex(int comIndex, char * comName)
{
    // 先释放原有指针，防止内存泄漏
    auto it = _comNameMap.find(comIndex);
    if (it != _comNameMap.end()) {
        delete[] it->second;
    }
    // 深拷贝字符串，避免悬挂指针
    size_t len = strlen(comName) + 1;
    char* nameCopy = new char[len];
    memcpy(nameCopy, comName, len);
    _comNameMap[comIndex] = nameCopy;
}

FITKXMLMultiBlockDataWriter::FITKXMLMultiBlockDataWriter():
    FITKXMLCompositeDataWriter()
{

}

FITKXMLMultiBlockDataWriter::~FITKXMLMultiBlockDataWriter()
{

}

int FITKXMLMultiBlockDataWriter::WriteComposite(vtkCompositeDataSet * compositeData, vtkXMLDataElement * parent, int & writerIdx)
{
    if (!(compositeData->IsA("vtkMultiBlockDataSet") || compositeData->IsA("vtkMultiPieceDataSet")))
    {
        vtkErrorMacro("Unsupported composite dataset type: " << compositeData->GetClassName() << ".");
        return 0;
    }

    // Write each input.
    vtkSmartPointer<vtkDataObjectTreeIterator> iter;
    iter.TakeReference(vtkDataObjectTree::SafeDownCast(compositeData)->NewTreeIterator());
    iter->VisitOnlyLeavesOff();
    iter->TraverseSubTreeOff();
    iter->SkipEmptyNodesOff();
    int toBeWritten = 0;
    for (iter->InitTraversal(); !iter->IsDoneWithTraversal(); iter->GoToNextItem())
    {
        toBeWritten++;
    }

    float progressRange[2] = { 0.f, 0.f };
    this->GetProgressRange(progressRange);

    int index = 0;
    int RetVal = 0;
    for (iter->InitTraversal(); !iter->IsDoneWithTraversal(); iter->GoToNextItem(), index++)
    {
        vtkDataObject* curDO = iter->GetCurrentDataObject();
        const char* name = nullptr;
        if (iter->HasCurrentMetaData())
        {
            name = iter->GetCurrentMetaData()->Get(vtkCompositeDataSet::NAME());
        }

        if (curDO && curDO->IsA("vtkCompositeDataSet"))
            // if node is a supported composite dataset
            // note in structure file and recurse.
        {
            vtkXMLDataElement* tag = vtkXMLDataElement::New();
            if (name)
            {
                tag->SetAttribute("name", name);
            }

            if (curDO->IsA("vtkMultiPieceDataSet"))
            {
                tag->SetName("Piece");
                tag->SetIntAttribute("index", index);
            }
            else if (curDO->IsA("vtkMultiBlockDataSet"))
            {
                tag->SetName("Block");
                tag->SetIntAttribute("index", index);
            }
            vtkCompositeDataSet* curCD = vtkCompositeDataSet::SafeDownCast(curDO);
            if (!this->WriteComposite(curCD, tag, writerIdx))
            {
                tag->Delete();
                return 0;
            }
            RetVal = 1;
            parent->AddNestedElement(tag);
            tag->Delete();
        }
        else
            // this node is not a composite data set.
        {
            vtkXMLDataElement* datasetXML = vtkXMLDataElement::New();
            datasetXML->SetName("DataSet");
            datasetXML->SetIntAttribute("index", index);
            if (name)
            {
                datasetXML->SetAttribute("name", name);
            }
            vtkStdString fileName = this->CreatePieceFileNameFITK(writerIdx);

            this->SetProgressRange(progressRange, writerIdx, toBeWritten);
            if (this->WriteNonCompositeData(curDO, datasetXML, writerIdx, fileName.c_str()))
            {
                parent->AddNestedElement(datasetXML);
                RetVal = 1;
            }
            datasetXML->Delete();
        }
    }
    return RetVal;
}

vtkStdString FITKXMLMultiBlockDataWriter::CreatePieceFileNameFITK(int piece)
{
    int fileTpe = GetFileTypeByPiece(piece);
    if (fileTpe < 0) {
        return "";
    }

    char* name = _comNameMap.at(piece);
    const char* ext = this->GetDefaultFileExtensionForDataSet(fileTpe);

    std::ostringstream stream;
    stream << GetFilePrefix() << "/" << name << "." << ext;
    return stream.str();
}
