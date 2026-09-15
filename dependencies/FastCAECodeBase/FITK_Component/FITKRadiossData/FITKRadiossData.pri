HEADERS += \ 
    $$PWD/FITKRadiossDataAPI.h \
    $$PWD/FITKAbstractRadiossData.h \
    $$PWD/FITKRadiossCase.h \
    $$PWD/FITKRadiossCaseManager.h \
    $$PWD/FITKRadiossSolution.h \
    $$PWD/FITKRadiossMeshModel.h \
    $$PWD/FITKRadiossGeomPart.h \
    $$PWD/FITKRadiossNodesParts.h \
    $$PWD/FITKRadiossNodeGroup.h \
    $$PWD/FITKNonLinerCurve.h \
    $$PWD/FITKAbstractRadMaterial.h \
    $$PWD/FITKMaterialElastic.h \
    $$PWD/FITKMaterialPlasJohns.h \
    $$PWD/FITKMaterialPlasTab.h \
    $$PWD/FITKPropAbstract.hpp \ 
    $$PWD/FITKPropShell.h \
    $$PWD/FITKPropSolid.h \
    $$PWD/FITKPropBeamSPR.h \
    $$PWD/FITKAbstractBCS.h \
    $$PWD/FITKRadiossBCS.h \
    $$PWD/FITKAbstractInitialField.h \
    $$PWD/FITKInitialField.h \
    $$PWD/FITKAbstractRadiossSurface.h \
    $$PWD/FITKAbstractRWall.h \
    $$PWD/FITKAbstractBEM.h \
    $$PWD/FITKBEMDoubleAsymptoticApproximation.h \
    $$PWD/FITKAbstractInteraction.h \
    $$PWD/FITKRWall.h \
    $$PWD/FITKGravity.h \
    $$PWD/FITKInteractionTied.h \
    $$PWD/FITKInteractionMultiUsageImpact.h \
    $$PWD/FITKRadiossSurfacePart.h \
    $$PWD/FITKRadiossSurfaceSeg.h \
    $$PWD/FITKRadiossElementGroup.h \
    $$PWD/FITKRadiossSolverSettings.h \
    $$PWD/FITKRadiossPostData.h \
    $$PWD/FITKRadiossResultRequest.h \
    $$PWD/FITKAbstractProbe.h \
    $$PWD/FITKProbeAcceleration.h \
    $$PWD/FITKAbstractTimeHistory.h \
    $$PWD/FITKTimeHistoryACCEL.h \
    $$PWD/FITKTimeHistoryINTER.h \
    $$PWD/FITKTimeHistoryRBODY.h \
    $$PWD/FITKTimeHistoryRWALL.h \
    $$PWD/FITKTimeHistorySECTIO.h \
    $$PWD/FITKAbstractConnection.h \
    $$PWD/FITKConnectionRBODY.h \
    $$PWD/FITKPropBeam.h \
    $$PWD/FITKMaterialHydro.h \
    $$PWD/FITKMaterialPlasBrit.h \
    $$PWD/FITKPropSandwichShell.h \
    $$PWD/FITKPropSPH.h \
    $$PWD/FITKAbstractEquationOfState.h \
    $$PWD/FITKEquationOfStatePolynomial.h \
    $$PWD/FITKProbeSection.h \
    $$PWD/FITKTimeHistorySPHCEL.h \
    $$PWD/FITKAbstractFailureModel.h \
    $$PWD/FITKFailureModelBIQUAD.h \
    $$PWD/FITKFailureModelJOHNSON.h \

SOURCES += \ 
    $$PWD/FITKAbstractRadiossData.cpp \
    $$PWD/FITKRadiossCase.cpp \
    $$PWD/FITKRadiossCaseManager.cpp \
    $$PWD/FITKRadiossSolution.cpp \
    $$PWD/FITKRadiossMeshModel.cpp \
    $$PWD/FITKRadiossGeomPart.cpp \
    $$PWD/FITKRadiossNodesParts.cpp \
    $$PWD/FITKRadiossNodeGroup.cpp \
    $$PWD/FITKNonLinerCurve.cpp \
    $$PWD/FITKAbstractRadMaterial.cpp \
    $$PWD/FITKMaterialElastic.cpp \
    $$PWD/FITKMaterialPlasJohns.cpp \
    $$PWD/FITKMaterialPlasTab.cpp \
    $$PWD/FITKPropShell.cpp \
    $$PWD/FITKPropSolid.cpp \
    $$PWD/FITKPropBeamSPR.cpp \
    $$PWD/FITKAbstractBCS.cpp \
    $$PWD/FITKRadiossBCS.cpp \
    $$PWD/FITKAbstractInitialField.cpp \
    $$PWD/FITKInitialField.cpp \
    $$PWD/FITKAbstractRadiossSurface.cpp \
    $$PWD/FITKAbstractRWall.cpp \
    $$PWD/FITKAbstractBEM.cpp \
    $$PWD/FITKBEMDoubleAsymptoticApproximation.cpp \
    $$PWD/FITKAbstractInteraction.cpp \
    $$PWD/FITKRWall.cpp \
    $$PWD/FITKGravity.cpp \
    $$PWD/FITKInteractionTied.cpp \
    $$PWD/FITKInteractionMultiUsageImpact.cpp \
    $$PWD/FITKRadiossSurfacePart.cpp \
    $$PWD/FITKRadiossSurfaceSeg.cpp \
    $$PWD/FITKRadiossElementGroup.cpp \
    $$PWD/FITKRadiossSolverSettings.cpp \
    $$PWD/FITKRadiossPostData.cpp \
    $$PWD/FITKRadiossResultRequest.cpp \
    $$PWD/FITKAbstractProbe.cpp \
    $$PWD/FITKProbeAcceleration.cpp \
    $$PWD/FITKAbstractTimeHistory.cpp \
    $$PWD/FITKTimeHistoryACCEL.cpp \
    $$PWD/FITKTimeHistoryINTER.cpp \
    $$PWD/FITKTimeHistoryRBODY.cpp \
    $$PWD/FITKTimeHistoryRWALL.cpp \
    $$PWD/FITKTimeHistorySECTIO.cpp \
    $$PWD/FITKAbstractConnection.cpp \
    $$PWD/FITKConnectionRBODY.cpp \
    $$PWD/FITKPropBeam.cpp \
    $$PWD/FITKMaterialHydro.cpp \
    $$PWD/FITKMaterialPlasBrit.cpp \
    $$PWD/FITKPropSandwichShell.cpp \
    $$PWD/FITKPropSPH.cpp \
    $$PWD/FITKAbstractEquationOfState.cpp \
    $$PWD/FITKEquationOfStatePolynomial.cpp \
    $$PWD/FITKProbeSection.cpp \
    $$PWD/FITKTimeHistorySPHCEL.cpp \
    $$PWD/FITKAbstractFailureModel.cpp \
    $$PWD/FITKFailureModelBIQUAD.cpp \
    $$PWD/FITKFailureModelJOHNSON.cpp \

FORMS += \
#    $$PWD/PluginManageDialog.ui \
 
 
#RESOURCES += ../qrc/qianfan.qrc

#RC_FILE += ../qrc/qianfan.rc
 