/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKPHengLEIParaWriterKey.h"
#include "FITKPHengLEIData.h"
#include "FITKPHengLEIAbsSolver.h"
#include "FITKPHengLEIMeshProcess.h"
#include "FITK_Kernel/FITKEasyParam/FITKParamCombox.h"
#include <QTextStream>

namespace Interface
{
    void FITKPHengLEIParaWriterKey::setTask(PHengLEITask task)
    {
        _task = task;
    }

    bool FITKPHengLEIParaWriterKey::writeParamFile()
    {
        //任务阶段不明确，不执行写出
        if (_task == PLTNone) return false;
        FITKPHengLEIAbsParaWriter::writeParamFile();
        //获取文件流
		if (!this->openFileStream()) return false;

		//获取网格转换参数
		Interface::FITKPHengLEIData* physicData = Interface::FITKPHengLEIData::GetPHengLEIDataFromFrame();
		Interface::FITKPHengLEIMeshProcess* meshProcess = physicData->getMeshProcess();
		Core::FITKParameter* meshData = meshProcess->getAdditionalMeshData();
		Core::FITKAbstractEasyParam* meshDataGridDimension = meshData->getDataByName("Grid Dimension");
		Core::FITKParamCombox* comboxMeshDataGridDimension = dynamic_cast<Core::FITKParamCombox*>(meshDataGridDimension);
		QString valueDataGridDimension = comboxMeshDataGridDimension->getCurrentValue();

        QTextStream* stream = this->getStream();
        if (stream == nullptr) return false;
        *stream << "string title = \"PHengLEI Main Parameter Control File\";" << endl;
		*stream << endl;
        *stream << "// IMPORTANT NOTICE: DON NOT MODIFY THE FOWLLOWING LINE." << endl;
		*stream << "string defaultParaFile = \"./bin/cfd_para.hypara\";" << endl;
		*stream << endl;
        *stream << "// ndim: Dimensional of the grid, 2 or 3." <<endl;
		*stream << "// nparafile: the number of parameter files." << endl;
		if (valueDataGridDimension == "2D")
		{
			*stream << "int ndim = " << 2 << ";" << endl;
		}
		else
		{
			*stream << "int ndim = " << 3 << ";" << endl;
		}
        *stream << "int nparafile = 1;" << endl;

        switch (_task)
        {
            //网格转化
        case PLTGrid:
			*stream << endl;
            *stream << "int    nsimutask       = 1;" <<endl;
            *stream << "string parafilename = \"./bin/grid_para.hypara\";" << endl;
			*stream << endl;
            break;
            //网格分区
        case PLTPartion:
			*stream << endl;
            *stream << "int    nsimutask    = 3;" << endl;
            *stream << "string parafilename = \"./bin/partition.hypara\";" << endl;
            *stream << endl;
            break;
            //求解
        case PLTCFDSolve:
			*stream << endl;
            *stream << "int    nsimutask    = 0;" << endl;
            *stream << "string parafilename = \"./bin/" << this->getCFDSolverParaFileName() << "\";"<<endl;
            *stream << endl;
            break;
        }
        //高级参数
		*stream << "// ---------------- Advanced Parameters, DO NOT care it ----------------" << endl;
        *stream << "int numberOfGridProcessor   = 0;" << endl;
		*stream << "// ATP read" << endl;
		*stream << "//@string parafilename1 = \"\";" << endl;
		*stream << "//@string parafilename2 = \"\";" << endl;

        this->closeFileStream();
        return true;
    }

    QString FITKPHengLEIParaWriterKey::getCFDSolverParaFileName()
    {
        //获取当前求解器类型
        FITKPHengLEIData* phe = FITKPHengLEIData::GetPHengLEIDataFromFrame();
        if (phe == nullptr) return QString();
        FITKPHengLEIAbsSolver* solver = phe->getSolver();
        if (solver == nullptr) return QString();

        //根据类型，确定参数文件
        FITKPHengLEIAbsSolver::PHengLEISolverType t = solver->getSolverType();
        switch (t)
        {
        case FITKPHengLEIAbsSolver::PLSubsonic:
            return QString("cfd_para_subsonic.hypara");
        case FITKPHengLEIAbsSolver::PLTransonic:
            return QString("cfd_para_transonic.hypara");
        case FITKPHengLEIAbsSolver::PLSupersonic:
            return QString("cfd_para_supersonic.hypara");
        case FITKPHengLEIAbsSolver::PLHypersonic:
            return QString("cfd_para_hypersonic.hypara");
		case FITKPHengLEIAbsSolver::PLIncompressible:
			return QString("cfd_para_incompressible.hypara");
        case FITKPHengLEIAbsSolver::PLCompressibleSIMPLE:
            return QString("cfd_para_incompressible.hypara");
        case FITKPHengLEIAbsSolver::PLSIMPLE:
            return QString("cfd_para_incompressible.hypara");
        case FITKPHengLEIAbsSolver::PLPISO:
            return QString("cfd_para_incompressible.hypara");
        default: 
            return QString();
        }
        //预防错误
        return QString();

    }
 

}


