/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCoordinateAlgorithmEX.h"

#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkIdList.h"
#include "vtkIdTypeArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"
#include "vtkTrivialProducer.h"
#include "vtkUniformGrid.h"
#include "vtkUnsignedCharArray.h"
#include "vtkStringArray.h"
#include "vtkArrowSource.h"
#include "vtkGlyph3D.h"
#include "vtkDoubleArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPolyData.h"
#include "vtkConeSource.h"
#include "vtkLineSource.h"
#include "vtkMathUtilities.h"

vtkStandardNewMacro(FITKCoordinateAlgorithmEX);

//----------------------------------------------------------------------------
// Construct object with scaling on, scaling mode is by scalar value,
// scale factor = 1.0.
FITKCoordinateAlgorithmEX::FITKCoordinateAlgorithmEX()
{
    this->SetNumberOfInputPorts(2);
    this->SetNumberOfOutputPorts(2);
    this->OutputPointsPrecision = vtkAlgorithm::DEFAULT_PRECISION;
    // this->SetLabelArrayName("Labels");

    // by default process active point scalars
    this->SetInputArrayToProcess(
        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, vtkDataSetAttributes::SCALARS);

    // Create the default axes source.
    this->ArrowSource = vtkArrowSource::New();
    this->ConeSource = vtkConeSource::New();
    this->LineSource = vtkLineSource::New();

    // Line shaft.
    this->ArrowSource->SetShaftRadius(0.);
    this->ArrowSource->SetTipLength(0.25);

    this->ConeSource->SetRadius(0.2);
    this->ConeSource->SetHeight(0.5);

    this->LineSource->SetPoint1(0., 0., 0.);
    this->LineSource->SetPoint2(1., 0., 0.);

    this->AxesShape = AxesShapeType::ArrowShape;
}

FITKCoordinateAlgorithmEX::~FITKCoordinateAlgorithmEX()
{
    // Delete the arrow source.
    if (this->ArrowSource)
    {
        this->ArrowSource->Delete();
        this->ArrowSource = nullptr;
    }

    // Delete the cone source.
    if (this->ConeSource)
    {
        this->ConeSource->Delete();
        this->ConeSource = nullptr;
    }

    // Delete the line source.
    if (this->LineSource)
    {
        this->LineSource->Delete();
        this->LineSource = nullptr;
    }
}

void FITKCoordinateAlgorithmEX::SetUseArrow()
{
    this->AxesShape = AxesShapeType::ArrowShape;
}

void FITKCoordinateAlgorithmEX::SetUseCone()
{
    this->AxesShape = AxesShapeType::ConeShape;
}

void FITKCoordinateAlgorithmEX::SetUseLine()
{
    this->AxesShape = AxesShapeType::LineShape;
}

//----------------------------------------------------------------------------
int FITKCoordinateAlgorithmEX::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector** inputVector,
    vtkInformationVector* outputVector)
{
    // get the info objects
    vtkDataSet* input = vtkDataSet::GetData(inputVector[0], 0);
    vtkUnstructuredGrid* output = vtkUnstructuredGrid::GetData(outputVector, 0);

    return this->Execute(input, inputVector[1], output) ? 1 : 0;
}

//----------------------------------------------------------------------------
bool FITKCoordinateAlgorithmEX::Execute(vtkDataSet* input, vtkInformationVector* sourceVector, vtkUnstructuredGrid* output)
{
    vtkDataArray* inSScalars = this->GetInputArrayToProcess(0, input);
    // vtkDataArray* inVectors = this->GetInputArrayToProcess(1, input);
    return this->Execute(input, sourceVector, output, inSScalars, nullptr);
}

//----------------------------------------------------------------------------
bool FITKCoordinateAlgorithmEX::Execute(vtkDataSet* input, vtkInformationVector* vtkNotUsed(sourceVector), vtkUnstructuredGrid* output,
    vtkDataArray* inSScalars, vtkDataArray* vtkNotUsed(inVectors))
{
    // Get the output of axes line.
    vtkUnstructuredGrid* outDataText = vtkUnstructuredGrid::SafeDownCast(this->GetExecutive()->GetOutputData(1));

    assert(input && output);
    if (input == nullptr || output == nullptr || outDataText == nullptr || inSScalars == nullptr)
    {
        // nothing to do.
        return true;
    }

    vtkPointData* pd = input->GetPointData();

    // Check the array name and component indice of x, y, z.
    vtkDataArray* direcArray = pd->GetArray(this->DirectionArrayName);
    if (direcArray == nullptr)
    {
        // nothing to do.
        return true;
    }

    // Check component indice.
    if (direcArray->GetNumberOfComponents() < 3)
    {
        // nothing to do.
        return true;
    }
    
    vtkIdType numPts, inPtId;
    double x[3], s = 1.0;

    vtkDebugMacro(<< "Generating glyphs");

    numPts = input->GetNumberOfPoints();
    if (numPts < 1)
    {
        vtkDebugMacro(<< "No points to glyph!");
        return 1;
    }

    // Allocate storage for output PolyData
    //
    //outputPD->CopyVectorsOff();
    //outputPD->CopyNormalsOff();
    //outputPD->CopyTCoordsOff();

    //outputPDText->CopyVectorsOff();
    //outputPDText->CopyNormalsOff();
    //outputPDText->CopyTCoordsOff();

    // Create the input data.
    //@{
    // X.
    vtkSmartPointer<vtkPolyData> inputForGlyph = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> inputPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkDoubleArray> inputNormals = vtkSmartPointer<vtkDoubleArray>::New();
    vtkSmartPointer<vtkDoubleArray> inputScalars = vtkSmartPointer<vtkDoubleArray>::New();
    inputScalars->SetName("DistanceToCamera");
    inputNormals->SetNumberOfComponents(3);
    inputForGlyph->SetPoints(inputPoints);
    inputForGlyph->GetPointData()->SetNormals(inputNormals);
    inputForGlyph->GetPointData()->AddArray(inputScalars);

    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
    glyph->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "DistanceToCamera");

    switch (this->AxesShape)
    {
    case AxesShapeType::ArrowShape:
    {
        glyph->SetSourceConnection(this->ArrowSource->GetOutputPort());
        break;
    }
    case AxesShapeType::ConeShape:
    {
        glyph->SetSourceConnection(this->ConeSource->GetOutputPort());
        break;
    }
    case AxesShapeType::LineShape:
    {
        glyph->SetSourceConnection(this->LineSource->GetOutputPort());
        break;
    }
    default:
        break;
    }

    glyph->SetScaling(true);
    glyph->SetVectorModeToUseNormal();
    glyph->SetColorModeToColorByScalar();
    glyph->SetInputData(inputForGlyph);

    for (int i = 0; i < numPts; i++)
    {
        double* dire = direcArray->GetTuple3(i);
        double m = sqrt(pow(dire[0], 2) + pow(dire[1], 2) + pow(dire[2], 2));
        if (vtkMathUtilities::FuzzyCompare(m, 0.))
        {
            continue;
        }

        double* pt = input->GetPoint(i);
        inputPoints->InsertNextPoint(pt);
        inputNormals->InsertNextTuple3(dire[0], dire[1], dire[2]);

        // Get the scalar and vector data
        if (inSScalars)
        {
            s = inSScalars->GetComponent(i, 0);
        }

        inputScalars->InsertNextValue(s);
    }

    glyph->Update();

    vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    grid->DeepCopy(glyph->GetOutput());
    //@}

    // Initialize the output array.
    vtkStringArray* labelArray = vtkStringArray::New();
    labelArray->SetName(this->LabelArrayName);

    vtkSmartPointer<vtkUnstructuredGrid> textGridData = vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkPoints> textPoints = vtkSmartPointer<vtkPoints>::New();

    for (inPtId = 0; inPtId < numPts; inPtId++)
    {
        if (!(inPtId % 10000))
        {
            this->UpdateProgress(static_cast<double>(inPtId) / numPts);
            if (this->GetAbortExecute())
            {
                break;
            }
        }

        double* dire = direcArray->GetTuple3(inPtId);
        double m = sqrt(pow(dire[0], 2) + pow(dire[1], 2) + pow(dire[2], 2));
        if (vtkMathUtilities::FuzzyCompare(m, 0.))
        {
            continue;
        }

        double mx = dire[0] / m;
        double my = dire[1] / m;
        double mz = dire[2] / m;

        // Get the scalar and vector data
        s = inSScalars->GetComponent(inPtId, 0);

        input->GetPoint(inPtId, x);
    
        double newPt[3]{ x[0] + mx * s,  x[1] + my * s,  x[2] + mz * s };

        textPoints->InsertNextPoint(newPt);

        // Add axes label.
        //@{
        // Set text.
        labelArray->InsertNextValue(this->Label);
    }

    textGridData->SetPoints(textPoints);

    // Update ourselves and release memory
    //
    output->CopyStructure(grid);
    outDataText->CopyStructure(textGridData);

    outDataText->GetPointData()->AddArray(labelArray);
    labelArray->Delete();

    output->Squeeze();
    outDataText->Squeeze();

    return true;
}

int FITKCoordinateAlgorithmEX::ProcessRequest(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
    if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
    {
        return this->RequestInformation(request, inputVector, outputVector);
    }
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

//----------------------------------------------------------------------------
int FITKCoordinateAlgorithmEX::FillInputPortInformation(int port, vtkInformation* info)
{
    if (port == 0)
    {
        info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
        return 1;
    }
    else if (port == 1)
    {
        info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
        info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
        info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------
//int FITKCoordinateAlgorithmEX::FillOutputPortInformation(int port, vtkInformation* info)
//{
//    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
//}

//----------------------------------------------------------------------------
void FITKCoordinateAlgorithmEX::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Output Points Precision: " << this->OutputPointsPrecision << "\n";

    os << endl;
}