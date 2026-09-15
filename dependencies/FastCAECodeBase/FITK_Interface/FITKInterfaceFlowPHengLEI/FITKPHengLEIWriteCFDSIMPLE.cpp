/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIWriteCFDSIMPLE.h"
#include "FITKPHengLEIParaWriterGrid.h"
#include "FITKPHengLEIData.h"
#include "FITKPHengLEIInFlow.h"
#include "FITKPHengLEIDiscretisation.h"
#include "FITKPHengLEIMeshProcess.h"
#include "FITKPHengLEIGeneralControl.h"
#include "FITKPHengLEIAbsParaWriter.h"
#include "FITKPHengLEIAbsSolver.h"
#include "FITKPHengLEIWriteCFDCompressibleSIMPLE.h"
#include "FITKPHengLEIAbstractInFlowType.h"
#include "FITKPHengLEIViscousModel.h"
#include "FITKPHengLEIOutputSetting.h"
#include "FITKPHengLEIInitialCondition.h"
#include "FITKPHengLEISolution.h"
#include "FITKPHengLEICalculation.h"
#include "FITKPHengLEIViscousType.h"
#include "FITKPHengLEIMeshPara.h"
#include "FITKPHengLEIAbsSpatialDiscretisation.h"

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
	bool FITKPHengLEIWriteCFDSIMPLE::writeParamFile()
	{
 		//创建文件
 		FITKPHengLEIAbsParaWriter::writeParamFile();

		Interface::FITKPHengLEIData* data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		if (!data_) return false;

		//Physical models
		// 获取"DES Type"
		FITKPHengLEIViscousModel* viscousData = data_->getViscousModel();
		Core::FITKParameter* viscousPara = viscousData->getAdditionalVicousModelData();
		Core::FITKAbstractEasyParam* dataDESType = viscousPara->getDataByName("DES Type");
		Core::FITKParamCombox* dataStrDESType = dynamic_cast<Core::FITKParamCombox*>(dataDESType);
		QString valueDataStrDESType = dataStrDESType->getCurrentValue();

		//Inflow Parameter
		// 获取"RefMachNumber","Attacked","Angle Slide","Wall Temperature"
		Interface::FITKPHengLEIData* inflowData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		if (!inflowData) return false;
		auto _getInFlow = inflowData->getInFlow();
		Core::FITKParameter* inflowData_ = _getInFlow->getInFlowAdditionalData();

		Core::FITKAbstractEasyParam* dataFlowRefMachNumber = inflowData_->getDataByName("RefMachNumber");
		Core::FITKParamDouble* dataDouRefMachNumber = dynamic_cast<Core::FITKParamDouble*>(dataFlowRefMachNumber);
		double valueDataDouRefMachNumber = dataDouRefMachNumber->getValue();

		Core::FITKAbstractEasyParam* dataFlowAttacked = inflowData_->getDataByName("Attacked");
		Core::FITKParamDouble* dataDouAttacked = dynamic_cast<Core::FITKParamDouble*>(dataFlowAttacked);
		double valueDataDouAttacked = dataDouAttacked->getValue();

		Core::FITKAbstractEasyParam* dataFlowAngleSlide = inflowData_->getDataByName("Angle Slide");
		Core::FITKParamDouble* dataDouAngleSlide = dynamic_cast<Core::FITKParamDouble*>(dataFlowAngleSlide);
		double valueDataDoudataFlowAngleSlide = dataDouAngleSlide->getValue();

		Core::FITKAbstractEasyParam* dataFlowWallTemperature = inflowData_->getDataByName("Wall Temperature");
		Core::FITKParamDouble* dataDouFlowWallTemperature = dynamic_cast<Core::FITKParamDouble*>(dataFlowWallTemperature);
		double valueDataDouFlowWallTemperature = dataDouFlowWallTemperature->getValue();

		// Mesh Parameter
		FITKPHengLEIMeshPara* meshPara = data_->getMeshPara();
		Core::FITKParameter* meshParaData = meshPara->getAdditionalMeshData();
		//Core::FITKAbstractEasyParam* dataKnudsenLength = meshParaData->getDataByName("Knudsen Length");
		//Core::FITKParamDouble* dataDouKnudsenLength = dynamic_cast<Core::FITKParamDouble*>(dataKnudsenLength);
		//double valueDataDouKnudsenLength = dataDouKnudsenLength->getValue();

		Core::FITKAbstractEasyParam* dataGridScaleFactor = meshParaData->getDataByName("Grid Scale Factor");
		Core::FITKParamDouble* dataDouGridScaleFactor = dynamic_cast<Core::FITKParamDouble*>(dataGridScaleFactor);
		double valueDataDouGridScaleFactor = dataDouGridScaleFactor->getValue();

		FITKPHengLEIMeshPara* forceReference = data_->getMeshPara();
		Core::FITKParameter* forceReferenceData = forceReference->getAdditionalForceReference();

		Core::FITKAbstractEasyParam* dataForceReferenceLength = forceReferenceData->getDataByName("Length");
		Core::FITKParamDouble* dataDouForceReferenceLength = dynamic_cast<Core::FITKParamDouble*>(dataForceReferenceLength);
		double valueDataDouForceReferenceLength = dataDouForceReferenceLength->getValue();

		Core::FITKAbstractEasyParam* dataForceReferenceWise = forceReferenceData->getDataByName("Span Wise");
		Core::FITKParamDouble* dataDouForceReferenceWise = dynamic_cast<Core::FITKParamDouble*>(dataForceReferenceWise);
		double valueDataDouForceReferenceWise = dataDouForceReferenceWise->getValue();

		Core::FITKAbstractEasyParam* dataForceReferenceArea = forceReferenceData->getDataByName("Area");
		Core::FITKParamDouble* dataDouForceReferenceArea = dynamic_cast<Core::FITKParamDouble*>(dataForceReferenceArea);
		double valueDataDouForceReferenceArea = dataDouForceReferenceArea->getValue();

		FITKPHengLEIMeshPara* referencePoint = data_->getMeshPara();
		Core::FITKParameter* referencePointData = referencePoint->getAdditionalReferencePoint();

		Core::FITKAbstractEasyParam* dataReferenceTorqueRefx = referencePointData->getDataByName("TorqueRefx");
		Core::FITKParamDouble* dataDouReferenceTorqueRefx = dynamic_cast<Core::FITKParamDouble*>(dataReferenceTorqueRefx);
		double valueDataDouReferenceTorqueRefx = dataDouReferenceTorqueRefx->getValue();

		Core::FITKAbstractEasyParam* dataReferenceTorqueRefy = referencePointData->getDataByName("TorqueRefy");
		Core::FITKParamDouble* dataDouReferenceTorqueRefy = dynamic_cast<Core::FITKParamDouble*>(dataReferenceTorqueRefy);
		double valueDataDouReferenceTorqueRefy = dataDouReferenceTorqueRefy->getValue();

		Core::FITKAbstractEasyParam* dataReferenceTorqueRefz = referencePointData->getDataByName("TorqueRefz");
		Core::FITKParamDouble* dataDouReferenceTorqueRefz = dynamic_cast<Core::FITKParamDouble*>(dataReferenceTorqueRefz);
		double valueDataDouReferenceTorqueRefz = dataDouReferenceTorqueRefz->getValue();

		//获取spatial
		if (!data_) return false;
		auto discretisationData = data_->getDiscretisation();
		Interface::FITKPHengLEIAbsSpatialDiscretisation* spatialData_ = discretisationData->getSpatialDiscretisation();
		//获取空间离散类型
		QString spatialType = spatialData_->getDataObjectName();
		//获取空间离散类型对应的参数
		Core::FITKParameter* spatialPara = spatialData_->getSchemeTypeParams();

		//获取时间离散参数
		auto getTempralData = data_->getDiscretisation();
		Core::FITKParameter* tempralData_ = getTempralData->getTemporalAdditionalData();

		Core::FITKAbstractEasyParam* dataIunsteady = tempralData_->getDataByName("Flow State");
		Core::FITKParamCombox* dataIntIunsteady = dynamic_cast<Core::FITKParamCombox*>(dataIunsteady);
		QString valueDataIntIunsteady = dataIntIunsteady->getCurrentValue();

		Core::FITKAbstractEasyParam* dataCFLMethod = tempralData_->getDataByName("CFL Method");
		Core::FITKParamCombox* dataIntCFLMethod = dynamic_cast<Core::FITKParamCombox*>(dataCFLMethod);
		QString valueDataIntCFLMethod = dataIntCFLMethod->getCurrentValue();

		Core::FITKAbstractEasyParam* dataCFLStart = tempralData_->getDataByName("CFL Start");
		Core::FITKParamDouble* dataDouCFLStart = dynamic_cast<Core::FITKParamDouble*>(dataCFLStart);
		double valueDataDouCFLStart = dataDouCFLStart->getValue();

		Core::FITKAbstractEasyParam* dataCFLEnd = tempralData_->getDataByName("CFL End");
		Core::FITKParamDouble* dataDouCFLEnd = dynamic_cast<Core::FITKParamDouble*>(dataCFLEnd);
		double valueDataDouCFLEnd = dataDouCFLEnd->getValue();

		Core::FITKAbstractEasyParam* dataCFLVaryStep = tempralData_->getDataByName("CFL Vary Step");
		Core::FITKParamInt* dataIntCFLVaryStep = dynamic_cast<Core::FITKParamInt*>(dataCFLVaryStep);
		int valueDataIntCFLVaryStep = dataIntCFLVaryStep->getValue();

		Core::FITKAbstractEasyParam* dataktmax = tempralData_->getDataByName("ktmax");
		Core::FITKParamDouble* dataDouktmax = dynamic_cast<Core::FITKParamDouble*>(dataktmax);
		double valueDataDouktmax = dataDouktmax->getValue();

		Core::FITKAbstractEasyParam* dataNLUSGSSweeps = tempralData_->getDataByName("LU-SGS Sub-Iterations");
		Core::FITKParamInt* dataIntNLUSGSSweeps = dynamic_cast<Core::FITKParamInt*>(dataNLUSGSSweeps);
		int valueDataIntNLUSGSSweeps = dataIntNLUSGSSweeps->getValue();

		// Advanced Parameters
		QStringList visualVariables = {};
		int variableCount = 0;
		Interface::FITKPHengLEIOutputSetting* outputSetting = data_->getOutputSetting();
		Core::FITKParameter* basicFlowParams = outputSetting->getBasicFlowData();
		QList<Core::FITKAbstractEasyParam*> basicFLowList = basicFlowParams->getParameter();
		for (Core::FITKAbstractEasyParam* basicFlow : basicFLowList)
		{
			Core::FITKParamBool* basicFLowBool = dynamic_cast<Core::FITKParamBool*>(basicFlow);
			if (basicFLowBool->getValue())
			{
				visualVariables << QString::number(basicFlow->getData().toInt());
				++variableCount;
			}
		}

		Core::FITKParameter* derivedFlowParams = outputSetting->getDerivedFlowData();
		QList<Core::FITKAbstractEasyParam*> deriveFLowList = derivedFlowParams->getParameter();
		for (Core::FITKAbstractEasyParam* deriveFlow : deriveFLowList)
		{
			Core::FITKParamBool* deriveFLowBool = dynamic_cast<Core::FITKParamBool*>(deriveFlow);
			if (deriveFLowBool->getValue())
			{
				visualVariables << QString::number(deriveFlow->getData().toInt());
				++variableCount;
			}
		}

		Core::FITKParameter* dimensionalParams = outputSetting->getDimensionalData();
		QList<Core::FITKAbstractEasyParam*> dimensionalList = dimensionalParams->getParameter();
		for (Core::FITKAbstractEasyParam* dimensional : dimensionalList)
		{
			Core::FITKParamBool* dimensionalBool = dynamic_cast<Core::FITKParamBool*>(dimensional);
			if (dimensionalBool->getValue())
			{
				visualVariables << QString::number(dimensional->getData().toInt());
				++variableCount;
			}
		}

		Core::FITKParameter* dimensionlessParams = outputSetting->getDimensionlessData();
		QList<Core::FITKAbstractEasyParam*> dimensionlessList = dimensionlessParams->getParameter();
		for (Core::FITKAbstractEasyParam* dimensionless : dimensionlessList)
		{
			Core::FITKParamBool* dimensionlessBool = dynamic_cast<Core::FITKParamBool*>(dimensionless);
			if (dimensionlessBool->getValue())
			{
				visualVariables << QString::number(dimensionless->getData().toInt());
				++variableCount;
			}
		}

		Core::FITKParameter* nonEquilibriumParams = outputSetting->getNonEquilibriumData();
		QList<Core::FITKAbstractEasyParam*> nonEquilibriumList = nonEquilibriumParams->getParameter();
		for (Core::FITKAbstractEasyParam* nonEquilibrium : nonEquilibriumList)
		{
			Core::FITKParamBool* nonEquilibriumBool = dynamic_cast<Core::FITKParamBool*>(nonEquilibrium);
			if (nonEquilibriumBool->getValue())
			{
				visualVariables << QString::number(nonEquilibrium->getData().toInt());
				++variableCount;
			}
		}

		Core::FITKParameter* turbulenceModelParams = outputSetting->getTurbulenceModelData();
		QList<Core::FITKAbstractEasyParam*> turbulenceModelList = turbulenceModelParams->getParameter();
		for (Core::FITKAbstractEasyParam* turbulenceModel : turbulenceModelList)
		{
			Core::FITKParamBool* turbulenceModelBool = dynamic_cast<Core::FITKParamBool*>(turbulenceModel);
			if (turbulenceModelBool->getValue())
			{
				visualVariables << QString::number(turbulenceModel->getData().toInt());
				++variableCount;
			}
		}

		Core::FITKParameter* viscosityParams = outputSetting->getViscosityData();
		QList<Core::FITKAbstractEasyParam*> viscosityList = viscosityParams->getParameter();
		for (Core::FITKAbstractEasyParam* viscosity : viscosityList)
		{
			Core::FITKParamBool* viscosityBool = dynamic_cast<Core::FITKParamBool*>(viscosity);
			if (viscosityBool->getValue())
			{
				visualVariables << QString::number(viscosity->getData().toInt());
				++variableCount;
			}
		}

		Core::FITKParameter* vorticityParams = outputSetting->getVorticityData();
		QList<Core::FITKAbstractEasyParam*> vorticityList = vorticityParams->getParameter();
		for (Core::FITKAbstractEasyParam* vorticity : vorticityList)
		{
			Core::FITKParamBool* vorticityBool = dynamic_cast<Core::FITKParamBool*>(vorticity);
			if (vorticityBool->getValue())
			{
				visualVariables << QString::number(vorticity->getData().toInt());
				++variableCount;
			}
		}
		// 转换字符串为整数列表
		QList<int> intList;
		for (const QString &str : visualVariables) {
			intList.append(str.toInt());
		}
		// 排序整数列表
		std::sort(intList.begin(), intList.end());
		// 转换整数回字符串
		visualVariables.clear();
		for (int num : intList) {
			visualVariables.append(QString::number(num));
		}

		// General Control Parameter
		// 获取"Max Intervals Step","Flow Step","Plot Step","Force Step","Res Step"参数值
		FITKPHengLEICalculation* calculation = data_->getCalculation();
		Core::FITKParameter* convergencePara = calculation->getAdditionalConvergenceData();
		Core::FITKAbstractEasyParam* dataMaxSimuStep = convergencePara->getDataByName("Max Intervals Step");
		Core::FITKParamInt* dataDouMaxSimuStep = dynamic_cast<Core::FITKParamInt*>(dataMaxSimuStep);
		int valueDataDouMaxSimuStep = dataDouMaxSimuStep->getValue();

		Core::FITKParameter* savingIntervalsPara = calculation->getAdditionalSavingIntervalsData();
		Core::FITKAbstractEasyParam* dataIntervalStepFlow = savingIntervalsPara->getDataByName("Flow Step");
		Core::FITKParamInt* dataDouIntervalStepFlow = dynamic_cast<Core::FITKParamInt*>(dataIntervalStepFlow);
		int valueDataDouIntervalStepFlow = dataDouIntervalStepFlow->getValue();

		Core::FITKAbstractEasyParam* dataIntervalStepPlot = savingIntervalsPara->getDataByName("Plot Step");
		Core::FITKParamInt* dataDouIntervalStepPlot = dynamic_cast<Core::FITKParamInt*>(dataIntervalStepPlot);
		int valueDataDouIntervalStepPlot = dataDouIntervalStepPlot->getValue();

		Core::FITKAbstractEasyParam* dataIntervalStepForce = savingIntervalsPara->getDataByName("Force Step");
		Core::FITKParamInt* dataDouIntervalStepForce = dynamic_cast<Core::FITKParamInt*>(dataIntervalStepForce);
		int valueDataDouIntervalStepForce = dataDouIntervalStepForce->getValue();

		Core::FITKAbstractEasyParam* dataIntervalStepRes = savingIntervalsPara->getDataByName("Res Step");
		Core::FITKParamInt* dataDouIntervalStepRes = dynamic_cast<Core::FITKParamInt*>(dataIntervalStepRes);
		int valueDataDouIntervalStepRes = dataDouIntervalStepRes->getValue();

		//获取RoeEntropyData
		//Core::FITKParameter* VRoeEntropyData_ = viscousData->getAdditionalRoeEntropyData();
		//Core::FITKAbstractEasyParam* dataFixMethod = VRoeEntropyData_->getDataByName("FixMethod");
		//Core::FITKParamInt* dataIntFixMethod = dynamic_cast<Core::FITKParamInt*>(dataFixMethod);
		//int valueDataFixMethod = dataIntFixMethod->getValue();
		//Core::FITKAbstractEasyParam* dataScale = VRoeEntropyData_->getDataByName("Scale");
		//Core::FITKParamDouble* dataDoubleScale = dynamic_cast<Core::FITKParamDouble*>(dataScale);
		//double valueDataScale = dataDoubleScale->getValue();


		if (!this->openFileStream()) return false;
		QTextStream* stream = this->getStream();
		if (stream == nullptr) return false;

		*stream << "#########################################################################" << endl;
		*stream << "##                     General Control Parameter                         #" << endl;
		*stream << "#########################################################################" << endl;

		*stream << "int maxSimuStep       = " << valueDataDouMaxSimuStep << ";" << endl;
		*stream << "int intervalStepFlow  = " << valueDataDouIntervalStepFlow << ";" << endl;
		*stream << "int intervalStepPlot  = " << valueDataDouIntervalStepPlot << ";" << endl;
		*stream << "int intervalStepForce = " << valueDataDouIntervalStepForce << ";" << endl;
		*stream << "int intervalStepRes   = " << valueDataDouIntervalStepRes << ";" << endl;
		*stream << endl;
		
		*stream << "#########################################################################" << endl;
		*stream << "#                       Inflow Parameter                                #" << endl;
		*stream << "#########################################################################" << endl;
		*stream << "double refMachNumber   = " << valueDataDouRefMachNumber << ";" << endl;
		*stream << "double attackd       = " << valueDataDouAttacked << ";" << endl;
		*stream << "double angleSlide    = " << valueDataDoudataFlowAngleSlide << ";" << endl;
		*stream << endl;
		*stream << "double wallTemperature    = " << valueDataDouFlowWallTemperature << ";" << endl;

		//获取InflowParaType
		FITKPHengLEIAbstractInFlowType* _inFlow = _getInFlow->getInFlowType();
		QString inFlowTaype = _inFlow->getDataObjectName();
		Core::FITKParameter* inFlowTypeData = _inFlow->getInFlowTypeParams();
		if (inFlowTaype == "Nondimensional")//type 0
		{
			Core::FITKAbstractEasyParam* dataTemperature = inFlowTypeData->getDataByName("Dimensional Temperature");
			Core::FITKParamDouble* dataDouTemperature = dynamic_cast<Core::FITKParamDouble*>(dataTemperature);
			double valueDataDoudataTemperature = dataDouTemperature->getValue();

			Core::FITKAbstractEasyParam* dataRefReNumber = inFlowTypeData->getDataByName("RefReNumber");
			Core::FITKParamDouble* dataDouRefReNumber = dynamic_cast<Core::FITKParamDouble*>(dataRefReNumber);
			double valueDataDoudataRefReNumber = dataDouRefReNumber->getValue();

			Core::FITKAbstractEasyParam* dataVibrationTemperature = inFlowTypeData->getDataByName("Vibration Temperature");
			Core::FITKParamDouble* dataDouVibrationTemperature = dynamic_cast<Core::FITKParamDouble*>(dataVibrationTemperature);
			double valueDataDouVibrationTemperature = dataDouVibrationTemperature->getValue();

			*stream << endl;
			*stream << "int inflowParaType   = 0 "  << ";"<< endl;
			*stream << "double refReNumber   = " << valueDataDoudataRefReNumber << ";" << endl;
			*stream << "double refDimensionalTemperature   = " << valueDataDoudataTemperature << ";" << endl;
			*stream << "double freestream_vibration_temperature   = " << valueDataDouVibrationTemperature << ";" << endl;
		}

		if (inFlowTaype == "Flight")//type 1
		{
			Core::FITKAbstractEasyParam* dataHeight = inFlowTypeData->getDataByName("height");
			Core::FITKParamDouble* dataDouHeight = dynamic_cast<Core::FITKParamDouble*>(dataHeight);
			double valueDataDouHeight = dataDouHeight->getValue();

			*stream << "int inflowParaType = 1 " << endl;
			*stream << "double height = " << valueDataDouHeight << endl;
		}

		if (inFlowTaype == "Pressure")//type 5
		{
			Core::FITKAbstractEasyParam* dataVelocity = inFlowTypeData->getDataByName("Velocity");
			Core::FITKParamDouble* dataDouVelocity = dynamic_cast<Core::FITKParamDouble*>(dataVelocity);
			double valueDataDouVelocity = dataDouVelocity->getValue();

			Core::FITKAbstractEasyParam* dataPressure = inFlowTypeData->getDataByName("Pressure");
			Core::FITKParamDouble* dataDouPressure = dynamic_cast<Core::FITKParamDouble*>(dataPressure);
			double valueDataDouPressure = dataDouPressure->getValue();

			*stream << "int inflowParaType = 5 " << ";" << endl;
			*stream << "double refDimensionalVelocity = " << valueDataDouVelocity << ";" << endl;
			*stream << "double refDimensionalPressure = " << valueDataDouPressure << ";" << endl;
		}

		if (inFlowTaype == "Density")//type 4
		{
			Core::FITKAbstractEasyParam* dataVelocity = inFlowTypeData->getDataByName("Velocity");
			Core::FITKParamDouble* dataDouVelocity = dynamic_cast<Core::FITKParamDouble*>(dataVelocity);
			double valueDataDouVelocity = dataDouVelocity->getValue();

			Core::FITKAbstractEasyParam* dataDensity = inFlowTypeData->getDataByName("Density");
			Core::FITKParamDouble* dataDouDensity = dynamic_cast<Core::FITKParamDouble*>(dataDensity);
			double valueDataDouDensity = dataDouDensity->getValue();

			*stream << "int inflowParaType = 4 " << ";" << endl;
			*stream << "double refDimensionalVelocity = " << valueDataDouVelocity << ";" << endl;
			*stream << "double refDimensionalDensity = " << valueDataDouDensity << ";" << endl;
		}

		*stream << "#########################################################################" << endl;
		*stream << "#                         Mesh Parameter                                #" << endl;
		*stream << "#########################################################################" << endl;

		*stream << "double gridScaleFactor = " << valueDataDouGridScaleFactor << ";" << endl;
		*stream << "double forceReferenceLengthSpanWise = " << valueDataDouForceReferenceWise << ";" << endl;
		*stream << "double forceReferenceLength = " << valueDataDouForceReferenceLength << ";" << endl;
		*stream << "double forceReferenceArea = " << valueDataDouForceReferenceArea << ";" << endl;
		*stream << "double TorqueRefX = " << valueDataDouReferenceTorqueRefx << ";" << endl;
		*stream << "double TorqueRefY = " << valueDataDouReferenceTorqueRefy << ";" << endl;
		*stream << "double TorqueRefZ = " << valueDataDouReferenceTorqueRefz << ";" << endl;
		//*stream << "double knudsenLength = " << valueDataDouKnudsenLength << ";" << endl;
		*stream << endl;

		*stream << "#########################################################################" << endl;
		*stream << "#                       Physical models                                 #" << endl;
		*stream << "#########################################################################" << endl;
		FITKPHengLEIViscousType* viscousType = viscousData->getViscousType();
		if (viscousType->getDataObjectName() == "Euler")
		{
			*stream << "int    viscousType = 0" << ";" << endl;
			*stream << "string viscousName = \"Euler\"" << ";" << endl;
		}
		else if (viscousType->getDataObjectName() == "Laminar")
		{
			*stream << "int    viscousType = 1" << ";" << endl;
			*stream << "string viscousName = \"laminar\"" << ";" << endl;
		}
		else if (viscousType->getDataObjectName() == "Turbulent1eq")
		{
			*stream << "int    viscousType = 3" << ";" << endl;
			*stream << "string viscousName = \"1eq-sa\"" << ";" << endl;
		}
		else if (viscousType->getDataObjectName() == "Turbulent2eq")
		{
			*stream << "int    viscousType = 4" << ";" << endl;

			Core::FITKParameter* viscousPara = viscousType->getViscousTypeParams();
			Core::FITKAbstractEasyParam* turbulent2eqModelPara = viscousPara->getDataByName("2eq turbulent model");
			Core::FITKParamCombox* turbulent2eqModelCom = dynamic_cast<Core::FITKParamCombox*>(turbulent2eqModelPara);
			*stream << "string viscousName = \"" << turbulent2eqModelCom->getCurrentValue() << "\";" << endl;
		}

		//写入valueDataDouDensity
		if (valueDataStrDESType == "RANS")
		{
			*stream << "int DESType = 0" << ";" << endl;
		}
		if (valueDataStrDESType == "DES")
		{
			*stream << "int DESType = 1" << ";" << endl;
		}
		if (valueDataStrDESType == "DDES")
		{
			*stream << "int DESType = 2" << ";" << endl;
		}
		if (valueDataStrDESType == "IDDES")
		{
			*stream << "int DESType = 3" << ";" << endl;
		}

		//*stream << "int roeEntropyFixMethod = " << valueDataFixMethod << ";" << endl;
		//*stream << "double roeEntropyScale  = " << valueDataScale << ";" << endl;
		*stream << endl;

		*stream << "#########################################################################" << endl;
		*stream << "#                       Spatial Discretisation                          #" << endl;
		*stream << "#########################################################################" << endl;

		if (spatialType == "Struct") {
			Core::FITKAbstractEasyParam* dataInviscidScheme = spatialPara->getDataByName("inviscid Scheme");
			Core::FITKParamCombox* dataStrInviscidScheme = dynamic_cast<Core::FITKParamCombox*>(dataInviscidScheme);
			QString InviscidScheme_ = dataStrInviscidScheme->getCurrentValue();

			Core::FITKAbstractEasyParam* dataLimiter = spatialPara->getDataByName("limiter");
			Core::FITKParamCombox* dataStrLimiter = dynamic_cast<Core::FITKParamCombox*>(dataLimiter);
			QString Limiter_ = dataStrLimiter->getCurrentValue();

			Core::FITKAbstractEasyParam* dataRoeEntropyFixmethod = spatialPara->getDataByName("Roe Entropy FixMethod");
			Core::FITKParamCombox* dataStrRoeEntropyFixmethod = dynamic_cast<Core::FITKParamCombox*>(dataRoeEntropyFixmethod);
			QString roeEntropyFixmethod = dataStrRoeEntropyFixmethod->getCurrentValue();

			Core::FITKAbstractEasyParam* dataRoeEntropyScale = spatialPara->getDataByName("Roe Entropy Scale");
			Core::FITKParamDouble* dataStrRoeEntropyScale = dynamic_cast<Core::FITKParamDouble*>(dataRoeEntropyScale);
			double roeEntropyScale = dataStrRoeEntropyScale->getValue();

			*stream << "#*******************************************************************" << endl;
			*stream << "#                       Struct Solver                              *" << endl;
			*stream << "#*******************************************************************" << endl;
			*stream << "string inviscidSchemeName = \"" << InviscidScheme_ << "\";" << endl;
			*stream << "string str_limiter_name   = \"" << Limiter_ << "\";" << endl;
			if(roeEntropyFixmethod == "direct fix")
			{
				*stream << "int roeEntropyFixMethod = 1;" << endl;
			}
			else if (roeEntropyFixmethod == "multi-dimensional fix")
			{
				*stream << "int roeEntropyFixMethod = 2;" << endl;
			}
			else if (roeEntropyFixmethod == "Harten type")
			{
				*stream << "int roeEntropyFixMethod = 3;" << endl;
			}
			*stream << "double roeEntropyScale = " << roeEntropyScale <<";" << endl;
		}

		if (spatialType == "UnStruct") {
			Core::FITKAbstractEasyParam* dataInviscidScheme = spatialPara->getDataByName("inviscid Scheme");
			Core::FITKParamCombox* dataStrInviscidScheme = dynamic_cast<Core::FITKParamCombox*>(dataInviscidScheme);
			QString InviscidScheme_ = dataStrInviscidScheme->getCurrentValue();

			Core::FITKAbstractEasyParam* dataLimiter = spatialPara->getDataByName("limiter");
			Core::FITKParamCombox* dataStrLimiter = dynamic_cast<Core::FITKParamCombox*>(dataLimiter);
			QString Limiter_ = dataStrLimiter->getCurrentValue();

			Core::FITKAbstractEasyParam* datavenkatCoeff = spatialPara->getDataByName("venkatCoeff");
			Core::FITKParamDouble* dataDoubleVenkatCoeff = dynamic_cast<Core::FITKParamDouble*>(datavenkatCoeff);
			double valueDoubleVenkatCoeff = dataDoubleVenkatCoeff->getValue();

			Core::FITKAbstractEasyParam* dataRoeEntropyFixmethod = spatialPara->getDataByName("Roe Entropy FixMethod");
			Core::FITKParamCombox* dataStrRoeEntropyFixmethod = dynamic_cast<Core::FITKParamCombox*>(dataRoeEntropyFixmethod);
			QString roeEntropyFixmethod = dataStrRoeEntropyFixmethod->getCurrentValue();

			Core::FITKAbstractEasyParam* dataRoeEntropyScale = spatialPara->getDataByName("Roe Entropy Scale");
			Core::FITKParamDouble* dataStrRoeEntropyScale = dynamic_cast<Core::FITKParamDouble*>(dataRoeEntropyScale);
			double roeEntropyScale = dataStrRoeEntropyScale->getValue();

			*stream << "#**************************************" << endl;
			*stream << "#            UnStruct Solver          #" << endl;
			*stream << "#**************************************" << endl;
			*stream << "string uns_scheme_name  = \"" << InviscidScheme_ << "\";" << endl;
			*stream << "string uns_limiter_name = \"" << Limiter_ << "\";" << endl;
			if (Limiter_ == "vencat")
			{
				*stream << "double venkatCoeff     = " << valueDoubleVenkatCoeff << ";" << endl;
			}

			if (roeEntropyFixmethod == "direct fix")
			{
				*stream << "int roeEntropyFixMethod = 1;" << endl;
			}
			else if (roeEntropyFixmethod == "Harten type")
			{
				*stream << "int roeEntropyFixMethod = 3;" << endl;
			}
			*stream << "double roeEntropyScale = " << roeEntropyScale << ";" << endl;
		}

		*stream << "#########################################################################" << endl;
		*stream << "#                       Temporal Discretisation                         #" << endl;
		*stream << "#########################################################################" << endl;
		if (valueDataIntIunsteady == "Steady State")
		{
			*stream << "int iunsteady = " << 0 << ";" << endl;
		}
		else if (valueDataIntIunsteady == "Unsteady State")
		{
			*stream << "int iunsteady = " << 1 << ";" << endl;
		}
		if (valueDataIntCFLMethod == "Linear change")
		{
			*stream << "int    CFLMethod   = 0;" << endl;
		}
		else
		{
			*stream << "int    CFLMethod   = 1;" << endl;
		}
		*stream << "double CFLStart = " << valueDataDouCFLStart << ";" << endl;
		*stream << "double CFLEnd = " << valueDataDouCFLEnd << ";" << endl;
		*stream << "int    CFLVaryStep = " << valueDataIntCFLVaryStep << ";" << endl;
		*stream << "double ktmax = " << valueDataDouktmax << ";" << endl;
		*stream << "int nLUSGSSweeps = " << valueDataIntNLUSGSSweeps << ";" << endl;
			
		*stream << "#########################################################################" << endl;
		*stream << "#                       File In or Out                                  #" << endl;
		*stream << "#########################################################################" << endl;
		
		Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
		QString fileName = mesh->getFieldMesh()->getUserData(Core::FITKUserData::FITKUserRole + 10).toString();

		//获取网格参数
		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIMeshProcess* meshProcess = physicData->getMeshProcess();
		Core::FITKParameter* meshData = meshProcess->getAdditionalMeshData();

		Core::FITKAbstractEasyParam* meshDataGridType = meshData->getDataByName("Grid Type");
		Core::FITKParamCombox* comboxMeshDataGridType = dynamic_cast<Core::FITKParamCombox*>(meshDataGridType);
		QString valueDataGridType = comboxMeshDataGridType->getCurrentValue();
		//获取网格分区参数
		Core::FITKParameter* meshPartionParams = meshProcess->getAdditionalMeshPartion();
		Core::FITKAbstractEasyParam* maxProcessors = meshPartionParams->getDataByName("Max Processors");
		Core::FITKParamInt* doublemaxProcessors = dynamic_cast<Core::FITKParamInt*>(maxProcessors);
		int valuemaxProcessors = doublemaxProcessors->getValue();

		Core::FITKParameter* partionPara = meshProcess->getAdditionalMeshPartion();
		Core::FITKAbstractEasyParam* partionEnableData = partionPara->getDataByName("Enable");
		Core::FITKParamBool* boolPartionEnableData = dynamic_cast<Core::FITKParamBool*>(partionEnableData);
		if (boolPartionEnableData->getValue())
		{
			*stream << "string gridfile = \"./grid/" << QFileInfo(fileName).baseName() << "__" << valuemaxProcessors << ".fts" << "\";" << endl;
		}
		else
		{
			*stream << "string gridfile = \"./grid/" << QFileInfo(fileName).baseName() << ".fts" << "\";" << endl;
		}
		
		*stream << "int plotFieldType = 0" << ";" << endl;

		*stream << "// ----------------- Advanced Parameters, DO NOT care it ----------------" << endl;
		*stream << "int nVisualVariables = " << variableCount << ";" << endl;
		*stream << "int visualVariables[] = [" << visualVariables.join(", ") << "];" << endl;
		
	
		this->closeFileStream();
		return true;

	}

	void FITKPHengLEIWriteCFDSIMPLE::consoleMessage(int level, const QString& str)
	{
		return;
	}

	void FITKPHengLEIWriteCFDSIMPLE::run()
	{
		writeParamFile();
		closeFileStream();
		return;
	}
}



