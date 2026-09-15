/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>

#include <iostream>

int main0()
{
    // 1) 构造点
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0.0, 0.0, 0.0); // 0
    points->InsertNextPoint(1.0, 0.0, 0.0); // 1
    points->InsertNextPoint(1.0, 1.0, 0.0); // 2
    points->InsertNextPoint(0.0, 1.0, 0.0); // 3
    points->InsertNextPoint(0.0, 0.0, 1.0); // 4
    points->InsertNextPoint(1.0, 0.0, 1.0); // 5
    points->InsertNextPoint(1.0, 1.0, 1.0); // 6
    points->InsertNextPoint(0.0, 1.0, 1.0); // 7

    // 2) 构造面（6个四边形）
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();

    vtkIdType face0[4] = {0, 1, 2, 3};
    vtkIdType face1[4] = {4, 5, 6, 7};
    vtkIdType face2[4] = {0, 1, 5, 4};
    vtkIdType face3[4] = {1, 2, 6, 5};
    vtkIdType face4[4] = {2, 3, 7, 6};
    vtkIdType face5[4] = {3, 0, 4, 7};

    polys->InsertNextCell(4, face0);
    polys->InsertNextCell(4, face1);
    polys->InsertNextCell(4, face2);
    polys->InsertNextCell(4, face3);
    polys->InsertNextCell(4, face4);
    polys->InsertNextCell(4, face5);

    // 3) 构造 PolyData
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetPolys(polys);

    // 4) 添加三个节点变量（POINT_DATA）
    vtkSmartPointer<vtkFloatArray> var1 = vtkSmartPointer<vtkFloatArray>::New();
    var1->SetName("temperature");
    var1->SetNumberOfComponents(1);

    vtkSmartPointer<vtkFloatArray> var2 = vtkSmartPointer<vtkFloatArray>::New();
    var2->SetName("pressure");
    var2->SetNumberOfComponents(1);

    vtkSmartPointer<vtkFloatArray> var3 = vtkSmartPointer<vtkFloatArray>::New();
    var3->SetName("displacement");
    var3->SetNumberOfComponents(1);

    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); ++i)
    {
        var1->InsertNextValue(100.0f + static_cast<float>(i) * 10.0f);
        var2->InsertNextValue(1.0f + static_cast<float>(i) * 0.2f);
        var3->InsertNextValue(static_cast<float>(i) * 0.05f);
    }

    polyData->GetPointData()->AddArray(var1);
    polyData->GetPointData()->AddArray(var2);
    polyData->GetPointData()->AddArray(var3);
    // 指定默认激活标量（可选）
    polyData->GetPointData()->SetActiveScalars("temperature");

    // 5) 写出 legacy VTK 文件（.vtk）
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName("test_polydata_from_vtk_api.vtk");
    writer->SetInputData(polyData);
    writer->SetFileTypeToASCII();

    if (writer->Write() == 0)
    {
        std::cerr << "Failed to write VTK file." << std::endl;
        return 1;
    }

    std::cout << "VTK file written: test_polydata_from_vtk_api.vtk" << std::endl;
    return 0;
}
