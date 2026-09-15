/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIWriteCFDCompressibleSIMPLE.h"
#include "FITKPHengLEIParaWriterGrid.h"
#include "FITKPHengLEIData.h"
#include "FITKPHengLEIInFlow.h"
#include "FITKPHengLEIDiscretisation.h"
#include "FITKPHengLEIMeshProcess.h"
#include "FITKPHengLEIGeneralControl.h"
#include "FITKPHengLEIAbsParaWriter.h"
#include "FITKPHengLEIAbsSolver.h"
#include "FITKPHengLEIAbstractInFlowType.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITKPHengLEIOutputSetting.h"
#include "FITKPHengLEIInitialCondition.h"
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEICalculation.h"
#include "FITKPHengLEIViscousType.h"
#include "FITKPHengLEIMeshPara.h"
#include "FITKPHengLEIAbsSpatialDiscretisation.h"
#include "FITKPHengLEIModels.h"
#include "FITKPHengLEIEnergyModel.h"
#include "FITKPHengLEISpeciesModel.h"

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"

#include "FITK_Kernel/FITKEasyParam/FITKWidgetComLine.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDouble.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamDoubleList.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamString.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include "FITK_Kernel/FITKEasyParam/FITKAbstractEasyParam.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Component/FITKWidget/FITKTabWidget.h"

#include <QFileInfo>
#include <QList>
#include <QVector>
#include <QFile>
#include <QToolBox>
#include <QTextStream>

namespace Interface
{
	bool FITKPHengLEIWriteCFDCompressibleSIMPLE::writeParamFile()
	{
		//创建文件
		FITKPHengLEIAbsParaWriter::writeParamFile();

		Interface::FITKPHengLEIData* data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		if (!data_) return false;



		if (!this->openFileStream()) return false;
		QTextStream* stream = this->getStream();
		if (stream == nullptr) return false;

		// 输出 General Control Parameter
		bool GeneralControlState = { true };
		Interface::FITKPHengLEICalculation* calculation = data_->getCalculation();
		if (calculation)
		{
			Core::FITKParameter* convergencePara = calculation->getAdditionalConvergenceData();
			int maxSimuStep = convergencePara->getIntParaByName("Max Intervals Step", GeneralControlState);

			Core::FITKParameter* SavingIntervalsPara = calculation->getAdditionalSavingIntervalsData();
			int intervalStepFlow = SavingIntervalsPara->getIntParaByName("Flow Step", GeneralControlState);
			int intervalStepPlot = SavingIntervalsPara->getIntParaByName("Plot Step", GeneralControlState);
			int intervalStepForce = SavingIntervalsPara->getIntParaByName("Force Step", GeneralControlState);
			int intervalStepRes = SavingIntervalsPara->getIntParaByName("Res Step", GeneralControlState);

			// 写入 General Control Parameter
			*stream << "#########################################################################" << endl;
			*stream << "##                     General Control Parameter                         #" << endl;
			*stream << "#########################################################################" << endl;
			*stream << "// maxSimuStep: The max simulation step, don't care simulation is restart or not." << endl;
			*stream << "// intervalStepFlow: The step intervals for flow variables file 'flow.dat' saved." << endl;
			*stream << "// intervalStepPlot: The step intervals for tecplot visual file 'tecflow.dat' saved." << endl;
			*stream << "// intervalStepForce: The step intervals for aerodynamics coefficients file 'aircoef.dat' saved." << endl;
			*stream << "// intervalStepRes: The step intervals for residual 'res.dat' saved." << endl;
			*stream << "int maxSimuStep       = " << maxSimuStep << ";" << endl;
			*stream << "int intervalStepFlow  = " << intervalStepFlow << ";" << endl;
			*stream << "int intervalStepPlot  = " << intervalStepPlot << ";" << endl;
			*stream << "int intervalStepForce = " << intervalStepForce << ";" << endl;
			*stream << "int intervalStepRes   = " << intervalStepRes << ";" << endl;
			*stream << "// gridfile: The partitioned Grid file path, using relative path," << endl;
			*stream << "which is relative to the working directory." << endl;
		}

		Interface::FITKPHengLEIMeshProcess* meshProcess = data_->getMeshProcess();
		Core::FITKParameter* meshData = meshProcess->getAdditionalMeshData();

		Core::FITKParameter* partionPara = meshProcess->getAdditionalMeshPartion();
		Core::FITKAbstractEasyParam* partionEnableData = partionPara->getDataByName("Enable");
		Core::FITKParamBool* boolPartionEnableData = dynamic_cast<Core::FITKParamBool*>(partionEnableData);

		Core::FITKParameter* meshPartionParams = meshProcess->getAdditionalMeshPartion();
		Core::FITKAbstractEasyParam* maxProcessors = meshPartionParams->getDataByName("Max Processors");
		Core::FITKParamInt* doublemaxProcessors = dynamic_cast<Core::FITKParamInt*>(maxProcessors);
		int valuemaxProcessors = doublemaxProcessors->getValue();

		Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
		QString fileName = mesh->getFieldMesh()->getUserData(Core::FITKUserData::FITKUserRole + 10).toString();

		*stream << "// gridfile: The partitioned Grid file path, using relative path," << endl;
		*stream << "//           which is relative to the working directory." << endl;
		if (boolPartionEnableData->getValue())
		{
			*stream << "string gridfile = \"./grid/" << QFileInfo(fileName).baseName() << "__" << valuemaxProcessors << ".fts" << "\";" << endl;
		}
		else
		{
			*stream << "string gridfile = \"./grid/" << QFileInfo(fileName).baseName() << ".fts" << "\";" << endl;
		}

		// 输出Flow Parameter
		QString solverName = data_->getSolver()->getDataObjectName();
		bool FlowState = {true};
		Interface::FITKPHengLEIInitialCondition* initialCondition = data_->getInitialCondition();
		if (initialCondition)
		{
			Core::FITKParameter* flowDataPara = initialCondition->getInitialConditionAdditionalData();
			double iniRho = flowDataPara->getDoubleParaByName("Density", FlowState);
			Core::FITKAbstractEasyParam* velocityList = flowDataPara->getDataByName("Velocity");
			Core::FITKParamDoubleList* dataDouVelocity = dynamic_cast<Core::FITKParamDoubleList*>(velocityList);
			double initU = dataDouVelocity->getValue(0);
			double initV = dataDouVelocity->getValue(1);
			double initW = dataDouVelocity->getValue(2);
			double initP = flowDataPara->getDoubleParaByName("Pressure", FlowState);
			double initMu = flowDataPara->getDoubleParaByName("Dynamic Viscosity", FlowState);
			double initRg = flowDataPara->getDoubleParaByName("Gas Constant", FlowState);
			double refP = flowDataPara->getDoubleParaByName("Reference Pressure", FlowState);
			double refPLocate = flowDataPara->getDoubleParaByName("Reference Pressure Location", FlowState);
			// 写入 Flow Parameter

			*stream << "#########################################################################" << endl;
			*stream << "#                               Flow Parameter                          #" << endl;
			*stream << "#########################################################################" << endl;
			*stream << "//flowSolverName : SIMPLE/CompressibleSIMPLE/PISO" << endl;
			*stream << "string flowSolverName = \"" << solverName << "\";" << endl;
			*stream << "// Initial value of flow variables " << endl;
			*stream << "double initRho = " << iniRho << ";" << endl;
			*stream << "double initU = " << initU << ";" << endl;
			*stream << "double initV   = " << initV << ";" << endl;
			*stream << "double initW   = = " << initW << ";" << endl;
			*stream << "double initP   = " << initP << ";" << endl;
			*stream << "double initMu =" << initMu << ";" << endl;
			*stream << "double initRg  = " << initRg << ";" << endl;
			*stream << "double refP    = " << refP << ";" << endl;
			*stream << "int refPLocate = " << refPLocate << ";" << endl;
		}


		// 输出Parameters For Solving the Momentum Equation
		Interface::FITKPHengLEISolution* solution = data_->getSolution();
		if (solution)
		{
			Core::FITKParameter* momentumDataPara = solution->getAdditionalMomentumEquationData();

			// 获取动量方程求解参数
			bool MomEqState = { true };
			Core::FITKAbstractEasyParam* MomEqSolMethod = momentumDataPara->getDataByName("Solver Method");
			Core::FITKParamCombox* dataMomEqSolMethod = dynamic_cast<Core::FITKParamCombox*>(MomEqSolMethod);
			int valueDataMomEqSolMethod = dataMomEqSolMethod->getIndex();
			Core::FITKAbstractEasyParam* MomEqPreconditioner = momentumDataPara->getDataByName("Preconditioner");
			Core::FITKParamCombox* dataMomEqPreconditioner = dynamic_cast<Core::FITKParamCombox*>(MomEqPreconditioner);
			int valueDataMomEqPreconditioner = dataMomEqPreconditioner->getIndex();
			int momEqMaxSweep = momentumDataPara->getIntParaByName("Max Sweep", MomEqState);
			double momEqIterSolTolerence = momentumDataPara->getDoubleParaByName("Iter Sol Tolerence", MomEqState);
			double momEqRelaxCoeff = momentumDataPara->getDoubleParaByName("Relax Coeff", MomEqState);

			// 写入Parameters For Solving the Momentum Equation
			*stream << "#########################################################################" << endl;
			*stream << "##                 Parameters For Solving the Equations                 #" << endl;
			*stream << "#########################################################################" << endl;
			*stream << "// Parameters For Solving the Momentum Equation" << endl;
			*stream << "MomEqSolMethod : GaussSeidel(0) / GMRES(1) / Bicgstab(2) / AMG(3) / PCG(4) / FlexGMRES(5) / ILU(10) / AMGDD(11) / Hybrid(12)" << endl;
			*stream << "int MomEqSolMethod = " << valueDataMomEqSolMethod << endl;
			*stream << "// MomEqPrecondMethod : None(0)/ILU(1)/AMG(2)/Euclid(3)/LduDiag(8) /LduDIC(9) /LduDILU(10)" << endl;
			*stream << "int MomEqPrecondMethod = " << valueDataMomEqPreconditioner << ";" << endl;
			*stream << "int MomEqMaxSweep = " << momEqMaxSweep << ";" << endl;
			*stream << "double MomEqIterSolvTol = " << momEqIterSolTolerence << ";" << endl;
			*stream << "double MomEqRelaxCoeff = " << momEqRelaxCoeff << ";" << endl;
			*stream << endl;

			// 获取压力泊松方程求解参数
			Core::FITKParameter* pressureDataPara = solution->getAdditionalPressurePossionEquation();
			Core::FITKAbstractEasyParam* PresEqSolMethod = pressureDataPara->getDataByName("Solver Method");
			Core::FITKParamCombox* dataPresEqSolMethod = dynamic_cast<Core::FITKParamCombox*>(PresEqSolMethod);
			int valueDataPresEqSolMethod = dataPresEqSolMethod->getIndex();
			Core::FITKAbstractEasyParam* PresEqPreconditioner = pressureDataPara->getDataByName("Preconditioner");
			Core::FITKParamCombox* dataPresEqPreconditioner = dynamic_cast<Core::FITKParamCombox*>(PresEqPreconditioner);
			int valueDataPresEqPreconditioner = dataPresEqPreconditioner->getIndex();
			int presEqMaxSweep = pressureDataPara->getIntParaByName("Max Sweep", MomEqState);
			double presEqIterSolTolerence = pressureDataPara->getDoubleParaByName("Iter Sol Tolerence", MomEqState);
			double presEqRelaxCoeff = pressureDataPara->getDoubleParaByName("Relax Coeff", MomEqState);

			*stream << "// Parameters For Solving the Pressure Possion Equation" << endl;
			*stream << "int PPEqSolMethod = " << valueDataPresEqSolMethod << ";" << endl;
			*stream << "int PPEqPrecondMethod = " << valueDataPresEqPreconditioner << ";" << endl;
			*stream << "int PPEqMaxSweep = " << presEqMaxSweep << ";" << endl;
			*stream << "double PPEqIterSolvTol = " << presEqIterSolTolerence << ";" << endl;
			*stream << "double PPEqRelaxCoeff = " << presEqRelaxCoeff << ";" << endl;
			*stream << endl;


			// 获取能量方程求解参数
			Interface::FITKPHengLEIModels* models = data_->getModels();
			Interface::FITKPHengLEIEnergyModel* energyModel = models->getEnergyModel();
			bool energyState = { true };
			Core::FITKParameter* energyPara = energyModel->getAdditionalData();
			bool enableEnergy = energyPara->getBoolParaByName("Enable Energy Equations", energyState);
			if (enableEnergy)
			{
				Core::FITKParameter* energyDataPara = solution->getAdditionalEnergyEquationData();
				Core::FITKAbstractEasyParam* EnergyEqSolMethod = energyDataPara->getDataByName("Solver Method");
				Core::FITKParamCombox* dataEnergyEqSolMethod = dynamic_cast<Core::FITKParamCombox*>(EnergyEqSolMethod);
				int valueDataEnergyEqSolMethod = dataEnergyEqSolMethod->getIndex();
				Core::FITKAbstractEasyParam* EnergyEqPreconditioner = energyDataPara->getDataByName("Preconditioner");
				Core::FITKParamCombox* dataEnergyEqPreconditioner = dynamic_cast<Core::FITKParamCombox*>(EnergyEqPreconditioner);
				int valueDataEnergyEqPreconditioner = dataEnergyEqPreconditioner->getIndex();
				int energyEqMaxSweep = energyDataPara->getIntParaByName("Max Sweep", MomEqState);
				double energyEqIterSolTolerence = energyDataPara->getDoubleParaByName("Iter Sol Tolerence", MomEqState);
				double energyEqRelaxCoeff = energyDataPara->getDoubleParaByName("Relax Coeff", MomEqState);

				*stream << "// Energy Equation Solver Parameters" << endl;
				*stream << "int EnergyEqSolMethod = \"" << valueDataEnergyEqSolMethod << "\";" << endl;
				*stream << "int EnergyEqPrecondMethod = \"" << valueDataEnergyEqPreconditioner << "\";" << endl;
				*stream << "int EnergyEqMaxSweep = " << energyEqMaxSweep << ";" << endl;
				*stream << "double EnergyEqIterSolvTol = " << energyEqIterSolTolerence << ";" << endl;
				*stream << "double EnergyEqRelaxCoeff = " << energyEqRelaxCoeff << ";" << endl;
				*stream << endl;
			}


			// 获取湍流方程求解参数
			Interface::FITKPHengLEIViscousModel* viscousModel = models->getViscousModel();
			if (viscousModel->getViscousEnumType() == Interface::FITKPHengLEIAbsSolver::RANS)
			{
				Core::FITKParameter* turbulenceDataPara = solution->getAdditionalTurbulenceEquationData();
				Core::FITKAbstractEasyParam* TurbEqSolMethod = turbulenceDataPara->getDataByName("Solver Method");
				Core::FITKParamCombox* dataTurbEqSolMethod = dynamic_cast<Core::FITKParamCombox*>(TurbEqSolMethod);
				int valueDataTurbEqSolMethod = dataTurbEqSolMethod->getIndex();
				Core::FITKAbstractEasyParam* TurbEqPreconditioner = turbulenceDataPara->getDataByName("Preconditioner");
				Core::FITKParamCombox* dataTurbEqPreconditioner = dynamic_cast<Core::FITKParamCombox*>(TurbEqPreconditioner);
				int valueDataTurbEqPreconditioner = dataTurbEqPreconditioner->getIndex();
				int turbEqMaxSweep = turbulenceDataPara->getIntParaByName("Max Sweep", MomEqState);
				double turbEqIterSolTolerence = turbulenceDataPara->getDoubleParaByName("Iter Sol Tolerence", MomEqState);
				double turbEqRelaxCoeff = turbulenceDataPara->getDoubleParaByName("Relax Coeff", MomEqState);

				*stream << "// Turbulence Equation Solver Parameters" << endl;
				*stream << "int TurbEqSolMethod = \"" << valueDataTurbEqSolMethod << "\";" << endl;
				*stream << "int TurbEqPrecondMethod = \"" << valueDataTurbEqPreconditioner << "\";" << endl;
				*stream << "int TurbEqMaxSweep = " << turbEqMaxSweep << ";" << endl;
				*stream << "double TurbEqIterSolvTol = " << turbEqIterSolTolerence << ";" << endl;
				*stream << "double TurbEqRelaxCoeff = " << turbEqRelaxCoeff << ";" << endl;
				*stream << endl;
			}

			// 获取物种方程求解参数
			Interface::FITKPHengLEISpeciesModel* speciesModel = models->getSpeciesModel();
			bool speciesState = { true };
			Core::FITKParameter* speciesPara = speciesModel->getAdditionalData();
			bool enableSpecies = speciesPara->getBoolParaByName("Enable Solve Species", speciesState);

			if(enableSpecies)
			{
				Core::FITKParameter* speciesDataPara = solution->getAdditionalSpeciesEquationData();
				Core::FITKAbstractEasyParam* SpeciesEqSolMethod = speciesDataPara->getDataByName("Solver Method");
				Core::FITKParamCombox* dataSpeciesEqSolMethod = dynamic_cast<Core::FITKParamCombox*>(SpeciesEqSolMethod);
				int valueDataSpeciesEqSolMethod = dataSpeciesEqSolMethod->getIndex();
				Core::FITKAbstractEasyParam* SpeciesEqPreconditioner = speciesDataPara->getDataByName("Preconditioner");
				Core::FITKParamCombox* dataSpeciesEqPreconditioner = dynamic_cast<Core::FITKParamCombox*>(SpeciesEqPreconditioner);
				int valueDataSpeciesEqPreconditioner = dataSpeciesEqPreconditioner->getIndex();
				int speciesEqMaxSweep = speciesDataPara->getIntParaByName("Max Sweep", MomEqState);
				double speciesEqIterSolTolerence = speciesDataPara->getDoubleParaByName("Iter Sol Tolerence", MomEqState);
				double speciesEqRelaxCoeff = speciesDataPara->getDoubleParaByName("Relax Coeff", MomEqState);

				*stream << "// Species Equation Solver Parameters" << endl;
				*stream << "int SpeciesEqSolMethod = \"" << valueDataSpeciesEqSolMethod << "\";" << endl;
				*stream << "int SpeciesEqPrecondMethod = \"" << valueDataSpeciesEqPreconditioner << "\";" << endl;
				*stream << "int SpeciesEqMaxSweep = " << speciesEqMaxSweep << ";" << endl;
				*stream << "double SpeciesEqIterSolvTol = " << speciesEqIterSolTolerence << ";" << endl;
				*stream << "double SpeciesEqRelaxCoeff = " << speciesEqRelaxCoeff << ";" << endl;
				*stream << endl;
			}




			// 获取松弛因子参数
			Core::FITKParameter* relaxationDataPara = solution->getAdditionalRelaxationData();
			double faceFluxRelaxation = relaxationDataPara->getDoubleParaByName("Face Flux Relaxation", MomEqState);
			bool gravityViscousSource = relaxationDataPara->getBoolParaByName("Gravity Viscous Source", MomEqState);
			bool bodyForce = relaxationDataPara->getBoolParaByName("Body Force", MomEqState);
			bool boussinesqModel = relaxationDataPara->getBoolParaByName("Boussinesq Model", MomEqState);
			double gravityX = relaxationDataPara->getDoubleParaByName("Gravity X", MomEqState);
			double gravityY = relaxationDataPara->getDoubleParaByName("Gravity Y", MomEqState);
			double gravityZ = relaxationDataPara->getDoubleParaByName("Gravity Z", MomEqState);
			double thermalExpansion = relaxationDataPara->getDoubleParaByName("Thermal Expansion", MomEqState);

			*stream << "// Relaxation factor of surface flux" << endl;
			*stream << "double FaceFluxRelaxCoeff = " << faceFluxRelaxation << ";" << endl;
			*stream << "int isCalGravityVisSource = " << static_cast<int>(gravityViscousSource) << ";" << endl;
			*stream << " int bodyForceFlag = " << static_cast<int>(bodyForce) << ";" << endl;
			*stream << "int isBoussinesqApproximation = " << static_cast<int>(boussinesqModel) << ";" << endl;
			*stream << "double gravityX = " << gravityX << ";" << endl;
			*stream << "double gravityY = " << gravityY << ";" << endl;
			*stream << "double gravityZ = " << gravityZ << ";" << endl;
			*stream << "double thermalExpansionCoeff = " << thermalExpansion << ";" << endl;
			*stream << endl;
		}


		// 输出Parameters For Solving the Turbulence Equation Parameter
		Interface::FITKPHengLEIModels* models =  data_->getModels();
		if (models)
		{
			Interface::FITKPHengLEIViscousModel* viscousModel = models->getViscousModel();
			bool viscousTypeState = { false };
			if (viscousModel)
			{
				if (viscousModel->getViscousEnumType() == FITKPHengLEIAbsSolver::Lamilar)
				{
					*stream << "#########################################################################" << endl;
					*stream << "#                  Turbulence Equation Parameter                        #" << endl;
					*stream << "#########################################################################" << endl;
					*stream << "// isSolveTurb : is(1)/not(0);" << endl;
					*stream << "int isSolveTurb = 0" << endl;
				}
				else if (viscousModel->getViscousEnumType() == FITKPHengLEIAbsSolver::RANS)
				{
				    Interface::FITKPHengLEIViscousType* viscousType = viscousModel->getViscousType();
					QString viscousName = viscousType->getDataObjectName();
					Core::FITKParameter* dataPara = viscousType->getViscousTypeParams();
					double initalKinetic = dataPara->getDoubleParaByName("Inital Kinetic", viscousTypeState);
					double initalEpsilon = dataPara->getDoubleParaByName("Inital Epsilon", viscousTypeState);

					*stream << "#########################################################################" << endl;
					*stream << "#                  Turbulence Equation Parameter                        #" << endl;
					*stream << "#########################################################################" << endl;
					*stream << "// isSolveTurb : is(1)/not(0);" << endl;
					*stream << "int isSolveTurb = 0" << endl;
					*stream << "// TurbEqSolverName : k-epsilon(KE)/Spalart-Allmaras(SA)" << endl;
					*stream << "// Initial value of Turbulence variables " << endl;
					*stream << "double initKinetic = " << initalKinetic << ";" << endl;
					*stream << "double initEpsilon = " << initalEpsilon << ";" << endl;
				}
			}
		}

			
		// 输出Parameters For Solving the Energy Equation Parameter
		bool EnergyModelState = { true };
		if (models)
		{
			Interface::FITKPHengLEIEnergyModel* energyModel = models->getEnergyModel();
			if (energyModel)
			{

				Core::FITKParameter* dataPara = energyModel->getAdditionalData();
				bool isSolveEnergy = dataPara->getBoolParaByName("Enable Energy Equations", EnergyModelState);
				double refT = dataPara->getDoubleParaByName("refT", EnergyModelState);
				*stream << "#########################################################################" << endl;
				*stream << "#                  Energy Equation Parameter                            #" << endl;
				*stream << "#########################################################################" << endl;
				*stream << "// isSolveEnergy : is(1)/not(0);" << endl;
				*stream << "int isSolveEnergy = " << static_cast<int>(isSolveEnergy) << ";" << endl;
				*stream << "double refT = " << refT << ";" << endl;
				if (isSolveEnergy)
				{
					double initT = dataPara->getDoubleParaByName("T", EnergyModelState);
					double initCPg = dataPara->getDoubleParaByName("CPg", EnergyModelState);
					double initK = dataPara->getDoubleParaByName("K", EnergyModelState);
					double initkappa = dataPara->getDoubleParaByName("kappa", EnergyModelState);
					double refT = dataPara->getDoubleParaByName("refT", EnergyModelState);

					*stream << "// Initial value of Energy variables " << endl;
					*stream << "double initT   = " << initT << ";" << endl;
					*stream << "double initCPg = " << initCPg << ";" << endl;
					*stream << "double initK   = " << initK << ";" << endl;
					*stream << "double initkappa = " << initkappa << ";" << endl;
					*stream << "double refT = " << refT << ";" << endl;
				}
			}
		}

		// 输出Parameters For Solving the Species Equation Parameter
		bool SpeciesModelState = { true };
		if (models)
		{
			Interface::FITKPHengLEISpeciesModel* speciesModel = models->getSpeciesModel();
			if (speciesModel)
			{

				Core::FITKParameter* dataPara = speciesModel->getAdditionalData();
				bool isSolveSpecies = dataPara->getBoolParaByName("Solve Species Number", SpeciesModelState);

				*stream << "#########################################################################" << endl;
				*stream << "#                  Species Equation Parameter                           #" << endl;
				*stream << "#########################################################################" << endl;
				*stream << "// isSolveSpecies : is(1)/not(0);" << endl;
				*stream << "int isSolveSpecies = " << static_cast<int>(isSolveSpecies) << ";" << endl;
				*stream << "//Parameters For Solving the Species Equation" << endl;
				*stream << " int numberOfSpeciesIncom = 1;" << endl;
				*stream << "string speciesNameIncom[] = [AIR];" << endl;
				*stream << "double initMassFractionIncom[] = [1.0];" << endl;
			}
		}



		// 输出 Unsteady Parameter
		if (calculation)
		{
			Core::FITKParameter* unsteadyDataPara = calculation->getAdditionalSolverControlData();
			// 根据求解器标签获取启动稳态/非稳态参数

			bool UnsteadyState = { true };
			double startTime = unsteadyDataPara->getDoubleParaByName("Start Time", UnsteadyState);
			double endTime = unsteadyDataPara->getDoubleParaByName("End Time", UnsteadyState);
			double dt = unsteadyDataPara->getDoubleParaByName("Dt", UnsteadyState);
			int min_sub_iter = unsteadyDataPara->getIntParaByName("Min Sub Iter", UnsteadyState);
			int max_sub_iter = unsteadyDataPara->getIntParaByName("Max Sub Iter", UnsteadyState);
			double tol_sub_iter = unsteadyDataPara->getDoubleParaByName("Tol Sub Iter", UnsteadyState);
			bool isStableUnsteadyMethod = unsteadyDataPara->getBoolParaByName("Enable Stable Unsteady Method", UnsteadyState);

			// 写入 Unsteady Parameter
			*stream << "#########################################################################" << endl;
			*stream << "#                  Unsteady Parameter                                   #" << endl;
			*stream << "#########################################################################" << endl;
			*stream << "// iunsteady : is(1)/not(0);" << endl;
			*stream << "int iunsteady = 1" << ";" << endl;
			*stream << "double startTime = " << startTime << ";" << endl;
			*stream << "double endTime = " << endTime << ";" << endl;
			*stream << "double dt = " << dt << ";" << endl;
			*stream << "int min_sub_iter = " << min_sub_iter << ";" << endl;
			*stream << "int max_sub_iter = " << max_sub_iter << ";" << endl;
			*stream << "double tol_sub_iter = " << tol_sub_iter << ";" << endl;
			*stream << "// isStableUnsteadyMethod : is(1)/not(0);" << endl;
			*stream << "int isStableUnsteadyMethod = " << static_cast<int>(isStableUnsteadyMethod) << ";" << endl;
		}


		// 输出 Solution Method
		Interface::FITKPHengLEIDiscretisation* discretisation = data_->getDiscretisation();
		if (discretisation)
		{
			Core::FITKParameter* flowDataPara = discretisation->getFlowDiscretisation();
			bool flowDataState = { true };
			if (flowDataPara)
			{
				Core::FITKAbstractEasyParam* ConvectionMethodList = flowDataPara->getDataByName("Convection Method");
				Core::FITKParamCombox* dataConvectionMethod = dynamic_cast<Core::FITKParamCombox*>(ConvectionMethodList);
				QString valueDataMomEqSolMethod = dataConvectionMethod->getCurrentValue();
				
				Core::FITKAbstractEasyParam* DiffusionMethodList = flowDataPara->getDataByName("Diffusion Method");
				Core::FITKParamCombox* dataDiffusionMethod = dynamic_cast<Core::FITKParamCombox*>(DiffusionMethodList);
				QString valueDataDiffusionMethod = dataDiffusionMethod->getCurrentValue();

				Core::FITKAbstractEasyParam* TransportMethodList = flowDataPara->getDataByName("Time Integration Method");
				Core::FITKParamCombox* dataTransportMethod = dynamic_cast<Core::FITKParamCombox*>(TransportMethodList);
				QString valueDataTransportMethod = dataTransportMethod->getCurrentValue();

				Core::FITKAbstractEasyParam* GradientMethodList = flowDataPara->getDataByName("Gradient Method");
				Core::FITKParamCombox* dataGradientMethod = dynamic_cast<Core::FITKParamCombox*>(GradientMethodList);
				QString valueDataGradientMethod = dataGradientMethod->getCurrentValue();

				Core::FITKAbstractEasyParam* mathLibTypeList = flowDataPara->getDataByName("Mathematical Library Type");
				Core::FITKParamCombox* dataMathLibType = dynamic_cast<Core::FITKParamCombox*>(mathLibTypeList);
				int valueDataMathLibType = dataMathLibType->getIndex();

				Core::FITKAbstractEasyParam* PressureVelocityCouplingList = flowDataPara->getDataByName("Pressure-Velocity Coupling");
				Core::FITKParamCombox* dataPressureVelocityCoupling = dynamic_cast<Core::FITKParamCombox*>(PressureVelocityCouplingList);
				int valueDataPressureVelocityCoupling = dataPressureVelocityCoupling->getIndex();

				bool gradidentCorrection = flowDataPara->getBoolParaByName("Gradient Correction",flowDataState);

				Core::FITKAbstractEasyParam* DensityModelList = flowDataPara->getDataByName("Density Model");
				Core::FITKParamCombox* dataDensityModelList = dynamic_cast<Core::FITKParamCombox*>(DensityModelList);
				int valueDatadataDensityModel = dataDensityModelList->getIndex();

				Core::FITKAbstractEasyParam* ViscosityModelList = flowDataPara->getDataByName("Viscosity Model");
				Core::FITKParamCombox* dataViscosityModelList = dynamic_cast<Core::FITKParamCombox*>(ViscosityModelList);
				int valueDataViscosityModel = dataViscosityModelList->getIndex();

				Core::FITKAbstractEasyParam* ThermalConductivityModelList = flowDataPara->getDataByName("Thermal Conductivity");
				Core::FITKParamCombox* dataThermalConductivityModelList = dynamic_cast<Core::FITKParamCombox*>(ThermalConductivityModelList);
				int valueDataThermalConductivityModel = dataThermalConductivityModelList->getIndex();

				Core::FITKAbstractEasyParam* SpecificHeatModelList = flowDataPara->getDataByName("Specific Heat");
				Core::FITKParamCombox* dataSpecificHeatModelList = dynamic_cast<Core::FITKParamCombox*>(SpecificHeatModelList);
				int valueDataSpecificHeatModel = dataSpecificHeatModelList->getIndex();

				Core::FITKAbstractEasyParam* MassDiffusionModelList = flowDataPara->getDataByName("Mass Diffusion Model");
				Core::FITKParamCombox* dataMassDiffusionModelList = dynamic_cast<Core::FITKParamCombox*>(MassDiffusionModelList);
				int valueDataMassDiffusionModel = dataMassDiffusionModelList->getIndex();



				// 写出 Solution Method
				*stream << "#########################################################################" << endl;
				*stream << "#                               Solution Method                         #" << endl;
				*stream << "#########################################################################" << endl;
				*stream << "// ConvCalcMethod : UPWIND/CDS/QUICK/SUDS" << endl;
				*stream << "string ConvCalcMethod = \"" << valueDataMomEqSolMethod << "\";" << endl;
				*stream << "// DiffCalcMethod : NON_ORTHOGONAL(default)" << endl;
				*stream << "string DiffCalcMethod = \"" << valueDataDiffusionMethod << "\";" << endl;
				*stream << "// TranCalcMethod : IMPLICIT_EULER(1st)/IMPLICIT_2ND_ORDER(2st)" << endl;
				*stream << "string TranCalcMethod = \"" << valueDataTransportMethod << "\";" << endl;
				*stream << "// GradCalcMethod : GAUSS/LSQ" << endl;
				*stream << "string GradCalcMethod = \"" << valueDataGradientMethod << "\";" << endl;
				*stream << "// mathLibType : hypre(0)/unap(1)/yhamg(2)" << endl;
				*stream << "int mathLibType = " << valueDataMathLibType << ";" << endl;
				*stream << "// compressible : is(0, default)/not(1)" << endl;
				*stream << "int compressible = " << 0 << ";" << endl;
				*stream << "// SIMPLEC : SIMPLE(0)/SIMPLEC(1)" << endl;
				*stream << "int SIMPLEC = " << valueDataPressureVelocityCoupling << ";" << endl;
				*stream << "// isSkewness : stable gradient method: is(1)/not(0)" << endl;
				*stream << "int isSkewness = " << gradidentCorrection << ";" << endl;
				*stream << "// rhoType : Constant(0)/IncompressibleIdealGas(1)/IdealGas(2)" << endl;
				*stream << "int rhoType = " << valueDatadataDensityModel << ";" << endl;
				*stream << "// muType:  Constant(0)/SutherLand(1)/IdealGasMixingLaw(2)," << endl;
				*stream << "int muType = " << valueDataViscosityModel << ";" << endl;
				*stream << "// kType : Constant(0)/MassWeightedMixingLaw(1)/IdealGasMixingLaw(2)" << endl;
				*stream << "int kType = " << valueDataThermalConductivityModel << ";" << endl;
				*stream << "// cpType : Constant(0)/Mixing(1)" << endl;
				*stream << "int cpType = " << valueDataSpecificHeatModel << ";" << endl;
				*stream << "// massdiffType : ConstantDiluteApprox(1)/DiluteApprox(2)" << endl;
				*stream << "int massdiffType = " << valueDataMassDiffusionModel << ";" << endl;
				
			}
		}


		// 输出 Post Processing Parameter
		Interface::FITKPHengLEIOutputSetting* outsetting = data_->getOutputSetting();
		QList<int> visualVariables = {};
		int variableCount = 0;
		if (outsetting)
		{
			Core::FITKParameter* dataPara = outsetting->getBasicFlowData();

			QList<Core::FITKAbstractEasyParam*> basicFlowList = dataPara->getParameter();
			for (Core::FITKAbstractEasyParam* basicFLow : basicFlowList)
			{
				Core::FITKParamBool* vorticityBool = dynamic_cast<Core::FITKParamBool*>(basicFLow);
				if (vorticityBool->getValue())
				{
					visualVariables << basicFLow->getData().toInt();
					++variableCount;
				}
			}

			// 排序整数列表
			std::sort(visualVariables.begin(), visualVariables.end());

			QStringList visualVariablesStrList;
			for (int var : visualVariables) {
				visualVariablesStrList << QString::number(var);
			}
			QString visualVariablesStr = visualVariablesStrList.join(", ");

			*stream << "#########################################################################" << endl;
			*stream << "#                              Post - Processing                        #" << endl;
			*stream << "#########################################################################" << endl;
			*stream << "// nVisualVariables: Number of variables want to be dumped for tecplot visualization." << endl;
			*stream << "// visualVariables : Variable types dumped, listed as following:" << endl;
			*stream << "// -- U(22), V(23), W(24), P(25), CP(26), T(27), DEN(28), VIS(29), TE(31), ED(32), enthalpy(70), species(from 71 ~ 70 + numberOfSpecies)" << endl;
			*stream << "// Important Warning: Array size of visualVariables MUST be equal to nVisualVariables!!!" << endl;
			*stream << "// Variables order must from small to big." << endl;
			*stream << "int nVisualVariables = " << variableCount << ";" << endl;
			*stream << "int visualVariables[] = [" << visualVariablesStr << "];" << endl;
			*stream << "int plotFieldType = 4" << ";" << endl;
			*stream << "string resSaveFile = \"results/res.dat\";" << endl;
			*stream << "string aircoeffile = \"results/aircoef.dat\";" << endl;
		}






		this->closeFileStream();
		return true;

	}

	void FITKPHengLEIWriteCFDCompressibleSIMPLE::consoleMessage(int level, const QString& str)
	{
		return;
	}

	void FITKPHengLEIWriteCFDCompressibleSIMPLE::run()
	{
		writeParamFile();
		closeFileStream();
		return;
	}
}
