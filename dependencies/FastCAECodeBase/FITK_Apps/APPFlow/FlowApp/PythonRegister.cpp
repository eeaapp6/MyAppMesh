/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PythonRegister.h"
#include "FITK_Kernel/FITKPython/FITKPythonInterface.h"

#include "PythonInterface/GlobalFiles.h"
#include "PythonInterface/PhysicsFactoryWapper.h"

PyRegister::PyRegister()
{
    _pyInterface = Python::FITKPythonInterface::getInstance();
}

void PyRegister::registWapper()
{
    _pyInterface->addDecorator(new FilesWapper());
    _pyInterface->registerCPPClass("Files", "", "Global");

    _pyInterface->addDecorator(new PhysicsFactoryWapper());
    _pyInterface->registerCPPClass("PhysicsFactory", "", "Physics");

    _pyInterface->importPython();
}
