/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*=========================================================================

  Program:   ParaView
  Module:    FITKXMLCompositeDataWriter.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "FITKXMLCompositeDataWriter.h"

#include "vtkCallbackCommand.h"
#include "vtkCompositeDataPipeline.h"
#include "vtkCompositeDataSet.h"
#include "vtkDataObjectTreeIterator.h"
#include "vtkDoubleArray.h"
#include "vtkErrorCode.h"
#include "vtkExecutive.h"
#include "vtkFieldData.h"
#include "vtkGarbageCollector.h"
#include "vtkHierarchicalBoxDataSet.h"
#include "vtkHyperTreeGrid.h"
#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkRectilinearGrid.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkStructuredGrid.h"
#include "vtkTable.h"
#include "vtkUnstructuredGrid.h"
#include "vtkXMLDataElement.h"
#include "vtkXMLDataObjectWriter.h"

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <vtksys/SystemTools.hxx>

//------------------------------------------------------------------------------

class FITKXMLCompositeDataWriterInternals
{
    // These are used to by GetDefaultFileExtension(). This helps us avoid
    // creating new instances repeatedly for the same dataset type.
    std::map<int, vtkSmartPointer<vtkXMLWriter>> TmpWriters;

public:
    std::vector<vtkSmartPointer<vtkXMLWriter>> Writers;
    std::string FilePath;
    std::string FilePrefix;
    vtkSmartPointer<vtkXMLDataElement> Root;
    std::vector<int> DataTypes;

    // Get the default extension for the dataset_type. Will return nullptr if an
    // extension cannot be determined.
    const char* GetDefaultFileExtensionForDataSet(int dataset_type)
    {
        std::map<int, vtkSmartPointer<vtkXMLWriter>>::iterator iter =
            this->TmpWriters.find(dataset_type);
        if (iter == this->TmpWriters.end())
        {
            vtkSmartPointer<vtkXMLWriter> writer;
            writer.TakeReference(vtkXMLDataObjectWriter::NewWriter(dataset_type));
            if (writer)
            {
                std::pair<int, vtkSmartPointer<vtkXMLWriter>> pair(dataset_type, writer);
                iter = this->TmpWriters.insert(pair).first;
            }
        }
        if (iter != this->TmpWriters.end())
        {
            return iter->second->GetDefaultFileExtension();
        }
        return nullptr;
    }
};

//------------------------------------------------------------------------------
FITKXMLCompositeDataWriter::FITKXMLCompositeDataWriter()
{
    this->Internal = new FITKXMLCompositeDataWriterInternals;
    this->GhostLevel = 0;
    this->WriteMetaFile = 1;

    // Setup a callback for the internal writers to report progress.
    this->InternalProgressObserver = vtkCallbackCommand::New();
    this->InternalProgressObserver->SetCallback(&FITKXMLCompositeDataWriter::ProgressCallbackFunction);
    this->InternalProgressObserver->SetClientData(this);

    this->InputInformation = nullptr;
}

//------------------------------------------------------------------------------
FITKXMLCompositeDataWriter::~FITKXMLCompositeDataWriter()
{
    this->InternalProgressObserver->Delete();
    delete this->Internal;
}

//------------------------------------------------------------------------------
const char* FITKXMLCompositeDataWriter::GetDefaultFileExtensionForDataSet(int dataset_type)
{
    return this->Internal->GetDefaultFileExtensionForDataSet(dataset_type);
}

//------------------------------------------------------------------------------
unsigned int FITKXMLCompositeDataWriter::GetNumberOfDataTypes()
{
    return static_cast<unsigned int>(this->Internal->DataTypes.size());
}

//------------------------------------------------------------------------------
int* FITKXMLCompositeDataWriter::GetDataTypesPointer()
{
    return &this->Internal->DataTypes[0];
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);
    os << indent << "GhostLevel: " << this->GhostLevel << endl;
    os << indent << "WriteMetaFile: " << this->WriteMetaFile << endl;
}

//------------------------------------------------------------------------------
vtkTypeBool FITKXMLCompositeDataWriter::ProcessRequest(
    vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
    {
        return this->RequestUpdateExtent(request, inputVector, outputVector);
    }
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
    {
        return this->RequestData(request, inputVector, outputVector);
    }

    return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::SetWriteMetaFile(int flag)
{
    if (this->WriteMetaFile != flag)
    {
        this->WriteMetaFile = flag;
        this->Modified();
    }
}

//------------------------------------------------------------------------------
int FITKXMLCompositeDataWriter::RequestUpdateExtent(vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector, vtkInformationVector* vtkNotUsed(outputVector))
{
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(), this->GhostLevel);
    return 1;
}

//------------------------------------------------------------------------------
int FITKXMLCompositeDataWriter::RequestData(
    vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector*)
{
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);

    this->InputInformation = inInfo;

    vtkCompositeDataSet* compositeData =
        vtkCompositeDataSet::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!compositeData)
    {
        vtkErrorMacro("No hierarchical input has been provided. Cannot write");
        this->InputInformation = nullptr;
        return 0;
    }

    // Create writers for each input.
    this->CreateWriters(compositeData);

    this->SetErrorCode(vtkErrorCode::NoError);

    // Make sure we have a file to write.
    if (!this->Stream && !this->FileName)
    {
        vtkErrorMacro("Writer called with no FileName set.");
        this->SetErrorCode(vtkErrorCode::NoFileNameError);
        this->InputInformation = nullptr;
        return 0;
    }

    // We are just starting to write.  Do not call
    // UpdateProgressDiscrete because we want a 0 progress callback the
    // first time.
    this->UpdateProgress(0);

    // Initialize progress range to entire 0..1 range.
    float wholeProgressRange[2] = { 0.f, 1.f };
    this->SetProgressRange(wholeProgressRange, 0, 1);

    // Prepare file prefix for creation of internal file names.
    this->SplitFileName();

    float progressRange[2] = { 0.f, 0.f };
    this->GetProgressRange(progressRange);

    // Create the subdirectory for the internal files.
    std::string subdir = this->Internal->FilePath;
    subdir += this->Internal->FilePrefix;
    this->MakeDirectory(subdir.c_str());

    this->Internal->Root = vtkSmartPointer<vtkXMLDataElement>::New();
    this->Internal->Root->SetName(compositeData->GetClassName());

    int writerIdx = 0;
    if (!this->WriteComposite(compositeData, this->Internal->Root, writerIdx))
    {
        this->RemoveWrittenFiles(subdir.c_str());
        return 0;
    }

    if (this->WriteMetaFile)
    {
        this->SetProgressRange(progressRange, this->GetNumberOfInputConnections(0),
            this->GetNumberOfInputConnections(0) + this->WriteMetaFile);
        int retVal = this->WriteMetaFileIfRequested();
        this->InputInformation = nullptr;
        return retVal;
    }

    // We have finished writing.
    this->UpdateProgressDiscrete(1);

    this->InputInformation = nullptr;
    return 1;
}

//------------------------------------------------------------------------------
int FITKXMLCompositeDataWriter::WriteNonCompositeData(
    vtkDataObject* dObj, vtkXMLDataElement* datasetXML, int& writerIdx, const char* fileName)
{
    // Write a leaf dataset.
    int myWriterIndex = writerIdx;
    writerIdx++;

    // Locate the actual data writer for this dataset.
    vtkXMLWriter* writer = this->GetWriter(myWriterIndex);
    if (!writer)
    {
        return 0;
    }

    vtkDataSet* curDS = vtkDataSet::SafeDownCast(dObj);
    vtkTable* curTable = vtkTable::SafeDownCast(dObj);
    vtkHyperTreeGrid* curHTG = vtkHyperTreeGrid::SafeDownCast(dObj);
    if (!curDS && !curTable && !curHTG)
    {
        if (dObj)
        {
            vtkWarningMacro("This writer cannot handle sub-datasets of type: "
                << dObj->GetClassName() << " Dataset will be skipped.");
        }
        return 0;
    }

    if (datasetXML)
    {
        // Create the entry for the collection file.
        datasetXML->SetAttribute("file", fileName);
    }

    // FIXME
    // Ken's note, I do not think you can fix this, the
    // setprogress range has to be done in the loop that calls
    // this function.
    // this->SetProgressRange(progressRange, myWriterIndex,
    //                       GetNumberOfInputConnections(0)+writeCollection);

    std::string full = this->Internal->FilePath;
    full += fileName;

    writer->SetFileName(full.c_str());

    // Write the data.
    writer->AddObserver(vtkCommand::ProgressEvent, this->InternalProgressObserver);
    writer->Write();
    writer->RemoveObserver(this->InternalProgressObserver);

    if (writer->GetErrorCode() == vtkErrorCode::OutOfDiskSpaceError)
    {
        this->SetErrorCode(vtkErrorCode::OutOfDiskSpaceError);
        vtkErrorMacro("Ran out of disk space; deleting file: " << this->FileName);
        return 0;
    }
    return 1;
}

//------------------------------------------------------------------------------
int FITKXMLCompositeDataWriter::WriteData()
{
    // Write the collection file.
    this->StartFile();
    vtkIndent indent = vtkIndent().GetNextIndent();

    // Open the primary element.
    ostream& os = *(this->Stream);

    if (this->Internal->Root)
    {
        this->Internal->Root->PrintXML(os, indent);
    }

    // We want to avoid using appended data mode as it
    // is not supported in meta formats.
    int dataMode = this->DataMode;
    if (dataMode == vtkXMLWriter::Appended)
    {
        this->DataMode = vtkXMLWriter::Binary;
    }

    vtkDataObject* input = this->GetInput();
    vtkFieldData* fieldData = input->GetFieldData();

    vtkInformation* meta = input->GetInformation();
    bool hasTime = meta->Has(vtkDataObject::DATA_TIME_STEP()) ? true : false;
    if ((fieldData && fieldData->GetNumberOfArrays()) || hasTime)
    {
        vtkSmartPointer<vtkFieldData> fieldDataCopy = vtkSmartPointer<vtkFieldData>::New();
        fieldDataCopy->ShallowCopy(fieldData);
        if (hasTime)
        {
            vtkSmartPointer<vtkDoubleArray> time = vtkSmartPointer<vtkDoubleArray>::New();
            time->SetNumberOfTuples(1);
            time->SetTypedComponent(0, 0, meta->Get(vtkDataObject::DATA_TIME_STEP()));
            time->SetName("TimeValue");
            fieldDataCopy->AddArray(time);
        }
        this->WriteFieldDataInline(fieldDataCopy, indent);
    }
    this->DataMode = dataMode;

    return this->EndFile();
}

//------------------------------------------------------------------------------
int FITKXMLCompositeDataWriter::WriteMetaFileIfRequested()
{
    if (this->WriteMetaFile)
    {
        if (!this->Superclass::WriteInternal())
        {
            return 0;
        }
    }
    return 1;
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::MakeDirectory(const char* name)
{
    if (!vtksys::SystemTools::MakeDirectory(name))
    {
        vtkErrorMacro(<< "Sorry unable to create directory: " << name << endl
            << "Last system error was: "
            << vtksys::SystemTools::GetLastSystemError().c_str());
    }
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::RemoveADirectory(const char* name)
{
    if (!vtksys::SystemTools::RemoveADirectory(name))
    {
        vtkErrorMacro(<< "Sorry unable to remove a directory: " << name << endl
            << "Last system error was: "
            << vtksys::SystemTools::GetLastSystemError().c_str());
    }
}

//------------------------------------------------------------------------------
const char* FITKXMLCompositeDataWriter::GetDefaultFileExtension()
{
    return "vtm";
}

//------------------------------------------------------------------------------
const char* FITKXMLCompositeDataWriter::GetDataSetName()
{
    if (!this->InputInformation)
    {
        return "CompositeDataSet";
    }
    vtkDataObject* hdInput =
        vtkDataObject::SafeDownCast(this->InputInformation->Get(vtkDataObject::DATA_OBJECT()));
    if (!hdInput)
    {
        return nullptr;
    }
    return hdInput->GetClassName();
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::FillDataTypes(vtkCompositeDataSet* hdInput)
{
    vtkSmartPointer<vtkCompositeDataIterator> iter;
    iter.TakeReference(hdInput->NewIterator());
    vtkDataObjectTreeIterator* treeIter = vtkDataObjectTreeIterator::SafeDownCast(iter);
    if (treeIter)
    {
        treeIter->VisitOnlyLeavesOn();
        treeIter->TraverseSubTreeOn();
    }
    iter->SkipEmptyNodesOff();

    this->Internal->DataTypes.clear();
    for (iter->InitTraversal(); !iter->IsDoneWithTraversal(); iter->GoToNextItem())
    {
        vtkDataObject* dataObject = iter->GetCurrentDataObject();
        vtkDataSet* ds = vtkDataSet::SafeDownCast(dataObject);
        // BUG #0015942: Datasets with no cells or points are considered empty and
        // we'll skip then in our serialization code.
        if (ds && (ds->GetNumberOfPoints() > 0 || ds->GetNumberOfCells() > 0))
        {
            this->Internal->DataTypes.push_back(ds->GetDataObjectType());
        }
        else if (!ds && dataObject)
        {
            this->Internal->DataTypes.push_back(dataObject->GetDataObjectType());
        }
        else
        {
            this->Internal->DataTypes.push_back(-1);
        }
    }
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::CreateWriters(vtkCompositeDataSet* hdInput)
{
    this->Internal->Writers.clear();
    this->FillDataTypes(hdInput);

    vtkSmartPointer<vtkCompositeDataIterator> iter;
    iter.TakeReference(hdInput->NewIterator());
    vtkDataObjectTreeIterator* treeIter = vtkDataObjectTreeIterator::SafeDownCast(iter);
    if (treeIter)
    {
        treeIter->VisitOnlyLeavesOn();
        treeIter->TraverseSubTreeOn();
    }
    iter->SkipEmptyNodesOff();

    size_t numDatasets = this->Internal->DataTypes.size();
    this->Internal->Writers.resize(numDatasets);

    int i = 0;
    for (iter->InitTraversal(); !iter->IsDoneWithTraversal(); iter->GoToNextItem(), ++i)
    {
        vtkSmartPointer<vtkXMLWriter>& writer = this->Internal->Writers[i];
        vtkDataSet* ds = vtkDataSet::SafeDownCast(iter->GetCurrentDataObject());
        vtkTable* table = vtkTable::SafeDownCast(iter->GetCurrentDataObject());
        vtkHyperTreeGrid* htg = vtkHyperTreeGrid::SafeDownCast(iter->GetCurrentDataObject());
        if (ds == nullptr && table == nullptr && htg == nullptr)
        {
            writer = nullptr;
            continue;
        }

        // Create a writer based on the type of this input. We just instantiate
        // vtkXMLDataObjectWriter. That internally creates the write type of writer
        // based on the data type.
        writer.TakeReference(vtkXMLDataObjectWriter::NewWriter(this->Internal->DataTypes[i]));
        if (writer)
        {
            // Copy settings to the writer.
            writer->SetDebug(this->GetDebug());
            writer->SetByteOrder(this->GetByteOrder());
            writer->SetCompressor(this->GetCompressor());
            writer->SetBlockSize(this->GetBlockSize());
            writer->SetDataMode(this->GetDataMode());
            writer->SetEncodeAppendedData(this->GetEncodeAppendedData());
            writer->SetHeaderType(this->GetHeaderType());
            writer->SetIdType(this->GetIdType());

            // Pass input.
            writer->SetInputDataObject(iter->GetCurrentDataObject());
        }
    }
}

//------------------------------------------------------------------------------
vtkXMLWriter* FITKXMLCompositeDataWriter::GetWriter(int index)
{
    int size = static_cast<int>(this->Internal->Writers.size());
    if (index >= 0 && index < size)
    {
        return this->Internal->Writers[index];
    }
    return nullptr;
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::SplitFileName()
{
    std::string fileName = this->FileName;
    std::string name;

    // Split the file name and extension from the path.
    std::string::size_type pos = fileName.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        // Keep the slash in the file path.
        this->Internal->FilePath = fileName.substr(0, pos + 1);
        name = fileName.substr(pos + 1);
    }
    else
    {
        this->Internal->FilePath = "./";
        name = fileName;
    }

    // Split the extension from the file name.
    pos = name.find_last_of('.');
    if (pos != std::string::npos)
    {
        this->Internal->FilePrefix = name.substr(0, pos);
    }
    else
    {
        this->Internal->FilePrefix = name;

        // Since a subdirectory is used to store the files, we need to
        // change its name if there is no file extension.
        this->Internal->FilePrefix += "_data";
    }
}

//------------------------------------------------------------------------------
const char* FITKXMLCompositeDataWriter::GetFilePrefix()
{
    return this->Internal->FilePrefix.c_str();
}

//------------------------------------------------------------------------------
const char* FITKXMLCompositeDataWriter::GetFilePath()
{
    return this->Internal->FilePath.c_str();
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::ProgressCallbackFunction(
    vtkObject* caller, unsigned long, void* clientdata, void*)
{
    vtkAlgorithm* w = vtkAlgorithm::SafeDownCast(caller);
    if (w)
    {
        reinterpret_cast<FITKXMLCompositeDataWriter*>(clientdata)->ProgressCallback(w);
    }
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::ProgressCallback(vtkAlgorithm* w)
{
    float width = this->ProgressRange[1] - this->ProgressRange[0];
    float internalProgress = w->GetProgress();
    float progress = this->ProgressRange[0] + internalProgress * width;
    this->UpdateProgressDiscrete(progress);
    if (this->AbortExecute)
    {
        w->SetAbortExecute(1);
    }
}

//------------------------------------------------------------------------------
vtkStdString FITKXMLCompositeDataWriter::CreatePieceFileName(int piece)
{
    if (this->Internal->DataTypes[piece] < 0)
    {
        return "";
    }

    std::ostringstream stream;
    stream << this->Internal->FilePrefix.c_str() << "/" << this->Internal->FilePrefix.c_str() << "_"
        << piece << ".";
    const char* ext = this->GetDefaultFileExtensionForDataSet(this->Internal->DataTypes[piece]);
    stream << (ext ? ext : "");
    return stream.str();
}

int FITKXMLCompositeDataWriter::GetFileTypeByPiece(int piece)
{
    return this->Internal->DataTypes[piece];
}

//------------------------------------------------------------------------------
vtkExecutive* FITKXMLCompositeDataWriter::CreateDefaultExecutive()
{
    return vtkCompositeDataPipeline::New();
}

//------------------------------------------------------------------------------
int FITKXMLCompositeDataWriter::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkCompositeDataSet");
    return 1;
}

//------------------------------------------------------------------------------
void FITKXMLCompositeDataWriter::RemoveWrittenFiles(const char* SubDirectory)
{
    this->RemoveADirectory(SubDirectory);
    this->DeleteAFile();
    this->InputInformation = nullptr;
}
