/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIerDriver.h"

#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponents.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKAppFramework/FITKProgramTaskManager.h"
#include "FITKPHengLEIExecProgramDriver.h"
#include "FITKPHengLEIInputInfo.h"
#include <QApplication>

#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKEasyParam/FITKAbstractEasyParam.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamBool.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIData.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterKey.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterGrid.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIParaWriterPartion.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIMeshProcess.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIAbsSolver.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDHypersonsic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDSupersonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDSubsonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDTransonic.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDCompressibleSIMPLE.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDSIMPLE.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteCFDPISO.h"
#include "FITK_Interface/FITKInterfaceFlowPHengLEI/FITKPHengLEIWriteBoundary.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredFluidMeshVTK.h"
#include "FITK_Interface/FITKInterfaceMesh/FITKUnstructuredMeshVTK.h"
#include "FITKPHengLEICalculation.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamInt.h"
#include "FITKPHengLEILinuxDriver.h"
#include "FITKPHengLEILinuxShellDriver.h"


namespace Interface
{
    FITKPHengLEIerDriver::~FITKPHengLEIerDriver()
    {
		if (_execDriver)
		{
			delete _execDriver;
			_execDriver = nullptr;
		}
    }


    void FITKPHengLEIerDriver::setDataObject(Core::FITKAbstractDataObject* data)
    {
        //临时数据
        _data = data;
    }

    void FITKPHengLEIerDriver::setArgs(const QStringList& args)
    {
        //启动参数
        _args = args;
    }

    void FITKPHengLEIerDriver::startDriver(QStringList info /*= QStringList()*/)
    {
		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIMeshProcess* meshProcess = physicData->getMeshProcess();

		Core::FITKParameter* partionPara = meshProcess->getAdditionalMeshPartion();
		Core::FITKAbstractEasyParam* partionEnableData = partionPara->getDataByName("Enable");
		Core::FITKParamBool* boolPartionEnableData = dynamic_cast<Core::FITKParamBool*>(partionEnableData);

		// 判断是否是否有网格模型
		Interface::FITKUnstructuredFluidMeshVTK* mesh = FITKAPP->getGlobalData()->getMeshData<Interface::FITKUnstructuredFluidMeshVTK>();
		QString fileName = mesh->getFieldMesh()->getUserData(Core::FITKUserData::FITKUserRole + 10).toString();
		Interface::FITKBoundaryMeshVTKManager* boundMgr = mesh->getBoundaryMeshManager();
		if (!boundMgr || boundMgr->getDataCount() <= 0)
		{
			AppFrame::FITKMessageError(tr("Err: not Mesh"));
			return;
		}

		startMeshTransfer();
    }

    void FITKPHengLEIerDriver::stopDriver(QStringList info /*= QStringList()*/)
    {
		if (_execDriver == nullptr)return;
		_execDriver->stop();
		// 显示求解器程序已经结束的提示信息
		AppFrame::FITKMessageNormal(tr("Solver program has finished execution."));
		_execDriver = nullptr;

		//auto app = dynamic_cast<AppFrame::FITKApplication*>(qApp);
		//auto proGramManager = app->getProgramTaskManager();
		//if (proGramManager)proGramManager->removeDataByID(_execDriver->getDataObjectID());
    }

	void FITKPHengLEIerDriver::setTaskType(FITKPHengLEIerDriver::taskType type)
	{
		_taskType = type;
	}

	bool FITKPHengLEIerDriver::isProcessRunning() const
	{
		if (_execDriver == nullptr) return false;
		return _execDriver->isProcessRunning();
	}

	void FITKPHengLEIerDriver::startMeshTransfer()
	{
		QString PHengLEIDir = this->getValue("PHengLEI").toString();
		QString PHengLEIBinDir = this->getValue("PHengLEIBin").toString();
		QString PHengLEIResultDir = this->getValue("PHengLEIResults").toString();

		// 写key.hypara
		Interface::FITKPHengLEIParaWriterKey *keywriter = new Interface::FITKPHengLEIParaWriterKey();
		FITKPHengLEIAbsParaWriter::WriteCFDParam(PHengLEIBinDir + "/cfd_para.hypara");
		FITKPHengLEIAbsParaWriter::CopyPHengLEIprogram();
		keywriter->setTask(Interface::PLTGrid);
		keywriter->setFileName(PHengLEIBinDir + "/key.hypara");
		keywriter->writeParamFile();
		keywriter->closeFileStream();

		// 写grid_para.hypara
		Interface::FITKPHengLEIParmWriterGrid* gridWriter = new Interface::FITKPHengLEIParmWriterGrid();
		gridWriter->setFileName(PHengLEIBinDir + "/grid_para.hypara");
		gridWriter->writeParamFile();
		gridWriter->closeFileStream();

        Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		
#ifdef Q_OS_WIN
		auto proGramManager = FITKAPP->getProgramTaskManager();
		if (proGramManager == nullptr) return;

		Interface::FITKPHengLEICalculation* calculation = physicData->getCalculation();
		Core::FITKParameter* meshConversionParams = calculation->getAdditionalParallelData();
		Core::FITKAbstractEasyParam* ProcessesPara = meshConversionParams->getDataByName("Processes");
		Core::FITKParamInt* ProcessesData = dynamic_cast<Core::FITKParamInt*>(ProcessesPara);
		int Processes = ProcessesData->getValue();

		//设置程序参数
		QStringList PHengLEIMeshProcess = { "-n",QString::number(Processes),getValue("PHengLEI").toString() + "/PHengLEIv3d0.exe" };
		AppFrame::FITKProgramInputInfo* inputInfo = new PHengLEIDriver::FITKPHengLEIInputInfo();
		//inputInfo->setArgs(PHengLEIMeshProcess);

		auto driver = proGramManager->createProgram(1, "FITKPHengLEIDriver", inputInfo);
		_execDriver = dynamic_cast<FITKPHengLEIExecProgramDriver *>(driver);
		if (nullptr != _execDriver)
		{
			_execDriver->setWorkingDirectory(getValue("PHengLEI").toString());
			_execDriver->setExecProgram(getValue("PHengLEI").toString() + "/PHengLEIv3d0.exe");
			_execDriver->enableSendMessage(true);
			connect(_execDriver, SIGNAL(sig_Finish()), this, SLOT(onExecDriverFinished()));
			_execDriver->start();
		}
#elif defined(Q_OS_LINUX)
		auto proGramManager = FITKAPP->getProgramTaskManager();
		if (proGramManager == nullptr) return;

        AppFrame::FITKProgramInputInfo* inputInfo = new PHengLEIDriver::FITKPHengLEIInputInfo();
		auto driver = proGramManager->createProgram(2, "FITKPHengLEILinuxDriver", inputInfo);
         _linuxshellDriver = dynamic_cast<Interface::FITKPHengLEILinuxShellDriver*>(driver);

        if (nullptr != _linuxshellDriver)
		{
			QString fullCommand = QString("cd %1 && ./PHengLEIv3d0").arg(getValue("PHengLEI").toString());
            _linuxshellDriver->setShellCommand(fullCommand);

            //connect(_linuxshellDriver, SIGNAL(sig_Finish()), this, SLOT(onExecDriverFinished()));
            _linuxshellDriver->start();
		}
#endif
		Interface::FITKPHengLEIMeshProcess* meshProcess = physicData->getMeshProcess();
		Core::FITKParameter* partionPara = meshProcess->getAdditionalMeshPartion();
		Core::FITKAbstractEasyParam* partionEnableData = partionPara->getDataByName("Enable");
		Core::FITKParamBool* boolPartionEnableData = dynamic_cast<Core::FITKParamBool*>(partionEnableData);

		if (boolPartionEnableData->getValue())
		{
			QObject::connect(driver, SIGNAL(sig_Finish()), this, SLOT(startMeshPartion()));
		}
		else
		{
			QObject::connect(driver, SIGNAL(sig_Finish()), this, SLOT(startCFDCalculate()));
		}
	}

	void FITKPHengLEIerDriver::startMeshPartion()
	{
		QString PHengLEIDir = this->getValue("PHengLEI").toString();
		QString PHengLEIBinDir = this->getValue("PHengLEIBin").toString();

		// 写key.hypara
		Interface::FITKPHengLEIParaWriterKey *keywriter = new Interface::FITKPHengLEIParaWriterKey();
		keywriter->setTask(Interface::PLTPartion);
		keywriter->setFileName(PHengLEIBinDir + "/key.hypara");
		keywriter->writeParamFile();
		keywriter->closeFileStream();

		// 写partition.hypara
		Interface::FITKPHengLEIParmWriterPartion* partionWriter = new Interface::FITKPHengLEIParmWriterPartion();
		partionWriter->setFileName(PHengLEIBinDir + "/partition.hypara");
		partionWriter->writeParamFile();
		partionWriter->closeFileStream();

		auto proGramManager = FITKAPP->getProgramTaskManager();
		if (proGramManager == nullptr) return;

		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEICalculation* calculation = physicData->getCalculation();
		Core::FITKParameter* meshConversionParams = calculation->getAdditionalParallelData();
		Core::FITKAbstractEasyParam* ProcessesPara = meshConversionParams->getDataByName("Processes");
		Core::FITKParamInt* ProcessesData = dynamic_cast<Core::FITKParamInt*>(ProcessesPara);
		int Processes = ProcessesData->getValue();
#ifdef Q_OS_WIN
		//设置程序参数
		QStringList PHengLEIMeshProcess = {"-n",QString::number(Processes),getValue("PHengLEI").toString() + "/PHengLEIv3d0.exe" };
		AppFrame::FITKProgramInputInfo* inputInfo = new PHengLEIDriver::FITKPHengLEIInputInfo();
		//inputInfo->setArgs(PHengLEIMeshProcess);

		auto driver = proGramManager->createProgram(1, "FITKPHengLEIDriver", inputInfo);
		_execDriver = dynamic_cast<FITKPHengLEIExecProgramDriver *>(driver);
		if (nullptr != _execDriver)
		{
			_execDriver->setWorkingDirectory(getValue("PHengLEI").toString());
			_execDriver->setExecProgram(getValue("PHengLEI").toString() + "/PHengLEIv3d0.exe");
			_execDriver->enableSendMessage(true);
			connect(_execDriver, SIGNAL(sig_Finish()), this, SLOT(onExecDriverFinished()));
			_execDriver->start();
		}
#elif defined(Q_OS_LINUX)
        AppFrame::FITKProgramInputInfo* inputInfo = new PHengLEIDriver::FITKPHengLEIInputInfo();
		auto driver = proGramManager->createProgram(2, "FITKPHengLEILinuxDriver", inputInfo);
        _linuxshellDriver = dynamic_cast<Interface::FITKPHengLEILinuxShellDriver*>(driver);

        if (nullptr != _linuxshellDriver)
		{
			// 考虑并行的Linux命令
			QString fullCommand;
			if (Processes > 1) {
				fullCommand = QString("cd %1 && mpirun -np %2 ./PHengLEIv3d0")
					.arg(getValue("PHengLEI").toString())
					.arg(Processes);
			}
			else {
				fullCommand = QString("cd %1 && ./PHengLEIv3d0")
					.arg(getValue("PHengLEI").toString());
			}

            _linuxshellDriver->setShellCommand(fullCommand);
           // connect(_linuxshellDriver, SIGNAL(sig_Finish()), this, SLOT(onExecDriverFinished()));
            _linuxshellDriver->start();
		}
#endif
		QObject::connect(driver, SIGNAL(sig_Finish()), this, SLOT(startCFDCalculate()));
	}

	void FITKPHengLEIerDriver::startCFDCalculate()
	{
		QString PHengLEIDir = this->getValue("PHengLEI").toString();
		QString PHengLEIBinDir = this->getValue("PHengLEIBin").toString();

		// 写key.hypara
		Interface::FITKPHengLEIParaWriterKey *keywriter = new Interface::FITKPHengLEIParaWriterKey();
		keywriter->setTask(Interface::PLTCFDSolve);
		keywriter->setFileName(PHengLEIBinDir + "/key.hypara");
		keywriter->writeParamFile();
		keywriter->closeFileStream();

		Interface::FITKPHengLEIData* data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		if (data_ != nullptr && data_->getBoundaryManager() != nullptr) {
			int boundCount = data_->getBoundaryManager()->getDataCount();
			if (boundCount > 0) {
				// 写boundary_condition.hypara
				Interface::FITKPHengLEIWriteBoundary *boundaryWriter = new Interface::FITKPHengLEIWriteBoundary();
				boundaryWriter->setFileName(PHengLEIBinDir + "/boundary_condition.hypara");
				boundaryWriter->writeParamFile();
				boundaryWriter->closeFileStream();
			}
		}

		//写cfd_para_求解器类型.hypara
		data_ = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame(); 
		Interface::FITKPHengLEIAbsSolver* solver = data_->getSolver();
		Interface::FITKPHengLEIAbsSolver::PHengLEISolverType solverType = solver->getSolverType();
		switch (solverType)
		{
		case Interface::FITKPHengLEIAbsSolver::PLSolverNone:
			break;
		case Interface::FITKPHengLEIAbsSolver::PLSubsonic:
		{
			Interface::FITKPHengLEIWriteCFDSubsonic* subsonicWriter = new Interface::FITKPHengLEIWriteCFDSubsonic();
			subsonicWriter->setFileName(PHengLEIBinDir + "/cfd_para_subsonic.hypara");
			subsonicWriter->writeParamFile();
			subsonicWriter->closeFileStream();
		}
			break;
		case Interface::FITKPHengLEIAbsSolver::PLTransonic:
		{
			Interface::FITKPHengLEIWriteCFDTransonic* transonicWriter = new Interface::FITKPHengLEIWriteCFDTransonic();
			transonicWriter->setFileName(PHengLEIBinDir + "/cfd_para_transonic.hypara");
			transonicWriter->writeParamFile();
			transonicWriter->closeFileStream();
		}
			break;
		case Interface::FITKPHengLEIAbsSolver::PLSupersonic:
		{
			Interface::FITKPHengLEIWriteCFDSupersonic* supersonicWriter = new Interface::FITKPHengLEIWriteCFDSupersonic();
			supersonicWriter->setFileName(PHengLEIBinDir + "/cfd_para_supersonic.hypara");
			supersonicWriter->writeParamFile();
			supersonicWriter->closeFileStream();
		}
			break;
		case Interface::FITKPHengLEIAbsSolver::PLHypersonic:
		{
			Interface::FITKPHengLEIWriteCFDHypersonsic* hypersonicWriter = new Interface::FITKPHengLEIWriteCFDHypersonsic();
			hypersonicWriter->setFileName(PHengLEIBinDir + "/cfd_para_hypersonic.hypara");
			hypersonicWriter->writeParamFile();
			hypersonicWriter->closeFileStream();
			break;
		}
		case Interface::FITKPHengLEIAbsSolver::PLCompressibleSIMPLE:
		{
			Interface::FITKPHengLEIWriteCFDCompressibleSIMPLE* compressibleSIMPLEWriter = new Interface::FITKPHengLEIWriteCFDCompressibleSIMPLE;
			compressibleSIMPLEWriter->setFileName(PHengLEIBinDir + "/cfd_para_incompressible.hypara");
			compressibleSIMPLEWriter->writeParamFile();
			compressibleSIMPLEWriter->closeFileStream();
			break;
		}
		case Interface::FITKPHengLEIAbsSolver::PLSIMPLE:
		{
			Interface::FITKPHengLEIWriteCFDSIMPLE* incompressibleSIMPLEWriter = new Interface::FITKPHengLEIWriteCFDSIMPLE;
			incompressibleSIMPLEWriter->setFileName(PHengLEIBinDir + "/cfd_para_incompressible.hypara");
			incompressibleSIMPLEWriter->writeParamFile();
			incompressibleSIMPLEWriter->closeFileStream();
			break;
		}
		case Interface::FITKPHengLEIAbsSolver::PLPISO:
		{
			Interface::FITKPHengLEIWriteCFDPISO* pisoWriter = new Interface::FITKPHengLEIWriteCFDPISO();
            pisoWriter->setFileName(PHengLEIBinDir + "/cfd_para_incompressible.hypara");
			pisoWriter->writeParamFile();
			pisoWriter->closeFileStream();
			break;
		}
		 default:
			break;
		}

		auto proGramManager = FITKAPP->getProgramTaskManager();  
		if (proGramManager == nullptr) return;

		Interface::FITKPHengLEICalculation* calculation = data_->getCalculation();
		Core::FITKParameter* meshConversionParams = calculation->getAdditionalParallelData();
		Core::FITKAbstractEasyParam* ProcessesPara = meshConversionParams->getDataByName("Processes");
		Core::FITKParamInt* ProcessesData = dynamic_cast<Core::FITKParamInt*>(ProcessesPara);
		int Processes = ProcessesData->getValue();

#ifdef Q_OS_WIN
		//设置程序参数
		QStringList PHengLEIMeshProcess = { "-n",QString::number(Processes),getValue("PHengLEI").toString() + "/PHengLEIv3d0.exe" };
		AppFrame::FITKProgramInputInfo* inputInfo = new PHengLEIDriver::FITKPHengLEIInputInfo();
		//inputInfo->setArgs(PHengLEIMeshProcess);

		auto driver = proGramManager->createProgram(1, "FITKPHengLEIDriver", inputInfo);
		_execDriver = dynamic_cast<FITKPHengLEIExecProgramDriver *>(driver);
		if (nullptr != _execDriver)
		{
			_execDriver->setWorkingDirectory(getValue("PHengLEI").toString());
			_execDriver->setExecProgram(getValue("PHengLEI").toString() + "/PHengLEIv3d0.exe");
			_execDriver->enableSendMessage(true);
			connect(_execDriver, SIGNAL(sig_Finish()), this, SLOT(onExecDriverFinished()));
			_execDriver->start();
		}

#elif defined(Q_OS_LINUX)

		// Linux 执行逻辑
        AppFrame::FITKProgramInputInfo* inputInfo = new PHengLEIDriver::FITKPHengLEIInputInfo();
		auto driver = proGramManager->createProgram(2, "FITKPHengLEILinuxDriver", inputInfo);
        _linuxshellDriver = dynamic_cast<Interface::FITKPHengLEILinuxShellDriver*>(driver);

        if (nullptr != _linuxshellDriver)
		{
			// 构建Linux命令，考虑并行处理
			QString fullCommand;
			if (Processes > 1) {
				// 多进程并行执行
				fullCommand = QString("cd %1 && mpirun -np %2 ./PHengLEIv3d0")
					.arg(getValue("PHengLEI").toString())
					.arg(Processes);
			}
			else {
				// 单进程执行
				fullCommand = QString("cd %1 && ./PHengLEIv3d0")
					.arg(getValue("PHengLEI").toString());
			}

            _linuxshellDriver->setShellCommand(fullCommand);
           // connect(_linuxshellDriver, SIGNAL(sig_Finish()), this, SLOT(onExecDriverFinished()));
            _linuxshellDriver->start();
		}
#endif
	}

	void FITKPHengLEIerDriver::onExecDriverFinished()
	{
		// 执行器完成时，将指针设置为空，避免野指针问题
		if (_execDriver != nullptr)
		{
			emit processFinished();
			_execDriver = nullptr;
		}
	}

}


