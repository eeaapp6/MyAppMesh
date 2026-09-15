/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PythonRegister.h"
#include "FITK_Kernel/FITKPython/FITKPythonInterface.h"

#include "PythonInterface/BoxWapper.h"
#include "PythonInterface/SphereWapper.h"
#include "PythonInterface/CylinderWapper.h"
#include "PythonInterface/GeometrysBoolOper.h"
#include "PythonInterface/GlobalFiles.h"
#include "PythonInterface/AI_tool.h"
#include <QDebug>
PyRegister::PyRegister()
{
    _pyInterface = Python::FITKPythonInterface::getInstance();
}

void PyRegister::registWapper()
{
    _pyInterface->addDecorator(new BoxWapper());
    _pyInterface->registerCPPClass("Box", "", "Geometry");

    _pyInterface->addDecorator(new SphereWapper());
    _pyInterface->registerCPPClass("Sphere", "", "Geometry");

    _pyInterface->addDecorator(new CylinderWapper());
    _pyInterface->registerCPPClass("Cylinder", "", "Geometry");

    _pyInterface->addDecorator(new BoolOperWapper());
    _pyInterface->registerCPPClass("BoolOper", "", "Geometry");

    _pyInterface->addDecorator(new FilesWapper());
    _pyInterface->registerCPPClass("Files", "", "Global");

    _pyInterface->addDecorator(new AI_toolWrapper());
    _pyInterface->registerCPPClass("AI_tool", "", "AI");

    _pyInterface->importPython();

}

