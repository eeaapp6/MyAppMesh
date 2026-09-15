HEADERS += \ 
    $$PWD/FITKInterfaceFlowOFAPI.h \
    $$PWD/FITKOFGeometryData.h \
    $$PWD/FITKOFEnum.hpp \
    $$PWD/FITKOFPhysicsData.h \
    $$PWD/FITKOFSolverManager.h \
    $$PWD/FITKOFBoundary.h \
    $$PWD/FITKOFTurbulenceManager.h \
    $$PWD/FITKAbstractOFSolver.h \
    $$PWD/FITKOFSolverSIMPLE.h \
    $$PWD/FITKAbstractOFTurbulence.h \
    $$PWD/FITKOFDiscretization.h \
    $$PWD/FITKOFTransportProp.h \
    $$PWD/FITKOFTurbKEpsilon.h \
    $$PWD/FITKOFPhysicsManager.h \
    $$PWD/FITKOFTurbKKLOmega.h \
    $$PWD/FITKOFTurbKOmega.h \
    $$PWD/FITKOFTurbKOmegaSST.h \
    $$PWD/FITKOFTurbKOmegaSSTSAS.h \
    $$PWD/FITKOFTurbRealizableKEpsilon.h \
    $$PWD/FITKOFTurbRNGKEpsilon.h \
    $$PWD/FITKOFTurbSpalartAllmaras.h \
    $$PWD/FITKOFTurbKOmegaSSTDDES.h \
    $$PWD/FITKOFTurbKOmegaSSTIDDES.h \
    $$PWD/FITKOFTurbKOmegaSSTDES.h \
    $$PWD/FITKOFTurbSpalartAllmarasDES.h \
    $$PWD/FITKOFTurbSpalartAllmarasDDES.h \
    $$PWD/FITKOFTurbSpalartAllmarasIDDES.h \
    $$PWD/FITKOFTurbSmagorinsky.h \
    $$PWD/FITKOFTurbWALE.h \
    $$PWD/FITKFlowPhysicsHandlerFactory.h \
    $$PWD/FITKOFSolutionSolver.h \
    $$PWD/FITKOFSolution.h \
    $$PWD/FITKOFSolutionSolverManager.h \
    $$PWD/FITKOFInitialConditions.h \
    $$PWD/FITKOFTransportModel.h \
    $$PWD/FITKOFTransportModelManager.h \
    $$PWD/FITKOFBoundaryType.h \
    $$PWD/FITKOFBoundaryTypeManager.h \
    $$PWD/FITKOFSolverInter.h \
    $$PWD/FITKOFRunControl.h \
    $$PWD/FITKOFDynamicMesh.h \
    $$PWD/FITKOFOperatingConditions.h \
    $$PWD/FITKOFSolverCHTMultiRegion.h \
    $$PWD/FITKOFRadiation.h \
    $$PWD/FITKOFThermo.h \
    $$PWD/FITKOFRadiationModel.h \
    $$PWD/FITKOFRadiationParaManager.h \
    $$PWD/FITKOFRadiationAbsorptionModel.h \
    $$PWD/FITKOFRadiationScatterModel.h \
    $$PWD/FITKOFRadiationSunDirection.h \
    $$PWD/FITKOFRadiationSunLoad.h \
    $$PWD/FITKOFThermoPhysicalEquationOfState.h \
    $$PWD/FITKOFThermoPhysicalThermodynamics.h \
    $$PWD/FITKOFThermoPhysicalTransport.h \
    $$PWD/FITKOFThermoPropManager.h \

SOURCES += \
    $$PWD/FITKOFGeometryData.cpp \
    $$PWD/FITKOFPhysicsData.cpp \
    $$PWD/FITKOFSolverManager.cpp \
    $$PWD/FITKOFBoundary.cpp \
    $$PWD/FITKOFTurbulenceManager.cpp \
    $$PWD/FITKAbstractOFSolver.cpp \
    $$PWD/FITKOFSolverSIMPLE.cpp \
    $$PWD/FITKAbstractOFTurbulence.cpp \
    $$PWD/FITKOFDiscretization.cpp \
    $$PWD/FITKOFTransportProp.cpp \
    $$PWD/FITKOFTurbKEpsilon.cpp \
    $$PWD/FITKOFPhysicsManager.cpp \
    $$PWD/FITKOFTurbKKLOmega.cpp \
    $$PWD/FITKOFTurbKOmega.cpp \
    $$PWD/FITKOFTurbKOmegaSST.cpp \
    $$PWD/FITKOFTurbKOmegaSSTSAS.cpp \
    $$PWD/FITKOFTurbRealizableKEpsilon.cpp \
    $$PWD/FITKOFTurbRNGKEpsilon.cpp \
    $$PWD/FITKOFTurbSpalartAllmaras.cpp \
    $$PWD/FITKOFTurbKOmegaSSTDDES.cpp \
    $$PWD/FITKOFTurbKOmegaSSTIDDES.cpp \
    $$PWD/FITKOFTurbKOmegaSSTDES.cpp \
    $$PWD/FITKOFTurbSpalartAllmarasDES.cpp \
    $$PWD/FITKOFTurbSpalartAllmarasDDES.cpp \
    $$PWD/FITKOFTurbSpalartAllmarasIDDES.cpp \
    $$PWD/FITKOFTurbSmagorinsky.cpp \
    $$PWD/FITKOFTurbWALE.cpp \
    $$PWD/FITKFlowPhysicsHandlerFactory.cpp \
    $$PWD/FITKOFSolutionSolver.cpp \
    $$PWD/FITKOFSolution.cpp \
    $$PWD/FITKOFSolutionSolverManager.cpp \
    $$PWD/FITKOFInitialConditions.cpp \
    $$PWD/FITKOFTransportModel.cpp \
    $$PWD/FITKOFTransportModelManager.cpp \
    $$PWD/FITKOFBoundaryType.cpp \
    $$PWD/FITKOFBoundaryTypeManager.cpp \
    $$PWD/FITKOFSolverInter.cpp \
    $$PWD/FITKOFRunControl.cpp \
    $$PWD/FITKOFDynamicMesh.cpp \
    $$PWD/FITKOFOperatingConditions.cpp \
    $$PWD/FITKOFSolverCHTMultiRegion.cpp \
    $$PWD/FITKOFRadiation.cpp \
    $$PWD/FITKOFThermo.cpp \
    $$PWD/FITKOFRadiationModel.cpp \
    $$PWD/FITKOFRadiationParaManager.cpp \
    $$PWD/FITKOFRadiationAbsorptionModel.cpp \
    $$PWD/FITKOFRadiationScatterModel.cpp \
    $$PWD/FITKOFRadiationSunDirection.cpp \
    $$PWD/FITKOFRadiationSunLoad.cpp \
    $$PWD/FITKOFThermoPhysicalEquationOfState.cpp \
    $$PWD/FITKOFThermoPhysicalThermodynamics.cpp \
    $$PWD/FITKOFThermoPhysicalTransport.cpp \
    $$PWD/FITKOFThermoPropManager.cpp \

FORMS += \
 
 
#RESOURCES += ../qrc/qianfan.qrc

#RC_FILE += ../qrc/qianfan.rc

DISTFILES += \
    $$PWD/License.txt \
    


 