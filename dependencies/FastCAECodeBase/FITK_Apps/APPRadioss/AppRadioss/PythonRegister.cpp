/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "PythonRegister.h"
#include "FITK_Kernel/FITKPython/FITKPythonInterface.h"

#include "PythonInterface/PythonInterfaceGlobal/GlobalFiles.h"
#include "PythonInterface/PythonInterfaceGlobal/GlobalGeometrys.h"
#include "PythonInterface/PythonInterfaceGlobal/GlobalMeshs.h"
#include "PythonInterface/PythonInterfaceGlobal/MeshGenerateWapper.h"
#include "PythonInterface/PythonInterfaceGlobal/ModelSetWapper.h"
#include "PythonInterface/PythonInterfaceGlobal/ModelSurfaceWapper.h"
#include "PythonInterface/PythonInterfaceGlobal/SolverWapper.h"

#include "PythonInterface/PythonInterfaceProperties/AI_Properties.h"
#include "PythonInterface/PythonInterfaceProperties/CurveWapper.h"
#include "PythonInterface/PythonInterfaceProperties/EOSPolynomialWapper.h"
#include "PythonInterface/PythonInterfaceProperties/FailureModelBIQUADWapper.h"
#include "PythonInterface/PythonInterfaceProperties/FailureModelJOHNSONWapper.h"
#include "PythonInterface/PythonInterfaceProperties/MaterialElasticWapper.h"
#include "PythonInterface/PythonInterfaceProperties/MaterialHydroWapper.h"
#include "PythonInterface/PythonInterfaceProperties/MaterialPlasJohnsWapper.h"
#include "PythonInterface/PythonInterfaceProperties/MaterialPlasBritWapper.h"
#include "PythonInterface/PythonInterfaceProperties/MaterialPlasTabWapper.h"
#include "PythonInterface/PythonInterfaceProperties/PropSolidWapper.h"
#include "PythonInterface/PythonInterfaceProperties/PropShellWapper.h"
#include "PythonInterface/PythonInterfaceProperties/PropSandwichShellWapper.h"
#include "PythonInterface/PythonInterfaceProperties/PropBeamWapper.h"
#include "PythonInterface/PythonInterfaceProperties/PropSPHWapper.h"
#include "PythonInterface/PythonInterfaceProperties/PropBeamSPRWapper.h"
#include "PythonInterface/PythonInterfaceSolution/GravityWapper.h"
#include "PythonInterface/PythonInterfaceSolution/AI_Solution.h"
#include "PythonInterface/PythonInterfaceSolution/BCSWapper.h"
#include "PythonInterface/PythonInterfaceSolution/InterMultiUsageImpactWapper.h"
#include "PythonInterface/PythonInterfaceSolution/InterMultiUsageImpactDefaultWapper.h"
#include "PythonInterface/PythonInterfaceSolution/InterTiedWapper.h"
#include "PythonInterface/PythonInterfaceSolution/InterTiedDefaultWapper.h"
#include "PythonInterface/PythonInterfaceSolution/InitialValueAxisWapper.h"
#include "PythonInterface/PythonInterfaceSolution/InitialValueTraWapper.h"
#include "PythonInterface/PythonInterfaceSolution/ProbeAccelerometerWapper.h"
#include "PythonInterface/PythonInterfaceSolution/ProbeSectionWapper.h"
#include "PythonInterface/PythonInterfaceSolution/RigidBodyWapper.h"
#include "PythonInterface/PythonInterfaceSolution/RWallCylinderWapper.h"
#include "PythonInterface/PythonInterfaceSolution/RWallPlaneWapper.h"
#include "PythonInterface/PythonInterfaceSolution/SolutionWapper.h"
#include "PythonInterface/PythonInterfaceSolution/ResultRequestWapper.h"
#include "PythonInterface/PythonInterfaceSolution/SolverSettingWapper.h"

PyRegister::PyRegister()
{
    _pyInterface = Python::FITKPythonInterface::getInstance();
}

void PyRegister::registWapper()
{
    _pyInterface->addDecorator(new FilesWapper());
    _pyInterface->registerCPPClass("Files", "", "Global");

    _pyInterface->addDecorator(new GeometrysWapper());
    _pyInterface->registerCPPClass("Geometrys", "", "Global");

    _pyInterface->addDecorator(new MeshsWapper());
    _pyInterface->registerCPPClass("Meshs", "", "Global");

    _pyInterface->addDecorator(new MeshGenerateWapper());
    _pyInterface->registerCPPClass("MeshGenerate", "", "Global");

    _pyInterface->addDecorator(new SolverWapper());
    _pyInterface->registerCPPClass("Solver", "", "Global");

    _pyInterface->addDecorator(new ModelSetWapper());
    _pyInterface->registerCPPClass("ModelSet", "", "Model");

    _pyInterface->addDecorator(new ModelSurfaceWapper());
    _pyInterface->registerCPPClass("ModelSurface", "", "Model");

    _pyInterface->addDecorator(new AI_PropertiesWapper());
    _pyInterface->registerCPPClass("AI_Properties", "", "AI");

    _pyInterface->addDecorator(new CurveWapper());
    _pyInterface->registerCPPClass("Curve", "", "Curve");

    _pyInterface->addDecorator(new BCSWapper());
    _pyInterface->registerCPPClass("BCS", "", "BCS");

    _pyInterface->addDecorator(new EOSPolynomialWapper());
    _pyInterface->registerCPPClass("EOSPolynomial", "", "EOS");

    _pyInterface->addDecorator(new FailureModelBIQUADWapper());
    _pyInterface->registerCPPClass("BIQUAD", "", "Failure");

    _pyInterface->addDecorator(new FailureModelJOHNSONWapper());
    _pyInterface->registerCPPClass("JOHNSON", "", "Failure");

    _pyInterface->addDecorator(new MaterialElasticWapper());
    _pyInterface->registerCPPClass("Elastic", "", "Material");

    _pyInterface->addDecorator(new MaterialHydroWapper());
    _pyInterface->registerCPPClass("Hydro", "", "Material");

    _pyInterface->addDecorator(new MaterialPlasJohnsWapper());
    _pyInterface->registerCPPClass("PlasJohns", "", "Material");

    _pyInterface->addDecorator(new MaterialPlasTabWapper());
    _pyInterface->registerCPPClass("PlasTab", "", "Material");

    _pyInterface->addDecorator(new MaterialPlasBritWapper());
    _pyInterface->registerCPPClass("PlasBrit", "", "Material");

    _pyInterface->addDecorator(new PropSolidWapper());
    _pyInterface->registerCPPClass("Solid", "", "Property");

    _pyInterface->addDecorator(new PropShellWapper());
    _pyInterface->registerCPPClass("Shell", "", "Property");

    _pyInterface->addDecorator(new PropSandwichShellWapper());
    _pyInterface->registerCPPClass("SandwichShell", "", "Property");

    _pyInterface->addDecorator(new PropBeamWapper());
    _pyInterface->registerCPPClass("Beam", "", "Property");

    _pyInterface->addDecorator(new PropSPHWapper());
    _pyInterface->registerCPPClass("SPH", "", "Property");

    _pyInterface->addDecorator(new PropBeamSPRWapper());
    _pyInterface->registerCPPClass("BeamSPR", "", "Property");

    _pyInterface->addDecorator(new SolutionWapper());
    _pyInterface->registerCPPClass("Solution", "", "Solution");

    _pyInterface->addDecorator(new GravityWapper());
    _pyInterface->registerCPPClass("Gravity", "", "Gravity");

    _pyInterface->addDecorator(new InterTiedWapper());
    _pyInterface->registerCPPClass("InterTied", "", "Interaction");

    _pyInterface->addDecorator(new InterMultiUsageImpactWapper());
    _pyInterface->registerCPPClass("InterMultiUsageImpact", "", "Interaction");

    _pyInterface->addDecorator(new InterTiedDefaultWapper());
    _pyInterface->registerCPPClass("InterTiedDefault", "", "Interaction");

    _pyInterface->addDecorator(new InterMultiUsageImpactDefaultWapper());
    _pyInterface->registerCPPClass("InterMultiUsageImpactDefault", "", "Interaction");

    _pyInterface->addDecorator(new InitialValueTraWapper());
    _pyInterface->registerCPPClass("InitialValueTra", "", "InitialValue");

    _pyInterface->addDecorator(new InitialValueAxisWapper());
    _pyInterface->registerCPPClass("InitialValueAxis", "", "InitialValue");

    _pyInterface->addDecorator(new ProbeAccelerometerWapper());
    _pyInterface->registerCPPClass("ProbeAccelerometer", "", "Probe");

    _pyInterface->addDecorator(new ProbeSectionWapper());
    _pyInterface->registerCPPClass("ProbeSection", "", "Probe");

    _pyInterface->addDecorator(new RigidBodyWapper());
    _pyInterface->registerCPPClass("RigidBody", "", "Connection");

    _pyInterface->addDecorator(new RWallCylinderWapper());
    _pyInterface->registerCPPClass("RWallCylinder", "", "RWall");

    _pyInterface->addDecorator(new RWallPlaneWapper());
    _pyInterface->registerCPPClass("RWallPlane", "", "RWall");

    _pyInterface->addDecorator(new AI_SolutionWapper());
    _pyInterface->registerCPPClass("AI_Solution", "", "AI");

    _pyInterface->addDecorator(new SolverSettingWapper());
    _pyInterface->registerCPPClass("SolverSetting", "", "Solve");

    _pyInterface->addDecorator(new ResultRequestWapper());
    _pyInterface->registerCPPClass("ResultRequest", "", "Solve");

    _pyInterface->importPython();
}
