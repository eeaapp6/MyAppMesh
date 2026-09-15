/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

// #include "FITKPHengLEIParaWriterKey.h"
// #include "FITKPHengLEIData.h"
// #include "FITKPHengLEIAbsSolver.h"
// #include <QTextStream>
// 
// namespace Interface
// {
//     void FITKPHengLEIParaWriterKey::setTask(PHengLEITask task)
//     {
//         _task = task;
//     }
// 
//     void FITKPHengLEIParaWriterKey::setMeshDim(int dim)
//     {
//         //只有2 3 是合法值
//         if(dim == 2 || dim == 3)
//             _meshDim = dim;
//     }
// 
//     bool FITKPHengLEIParaWriterKey::writeParamFile()
//     {
//         //任务阶段不明确，不执行写出
//         if (_task == PLTNone) return false;
//         FITKPHengLEIAbsParaWriter::writeParamFile();
//         //获取文件流
// 		if (!this->openFileStream()) return false;
// 
//         QTextStream* stream = this->getStream();
//         if (stream == nullptr) return false;
//         *stream << "string title   = \"PHengLEI Main Parameter Control File\";" << endl;
//         *stream << endl;
//         *stream << "//IMPORTANT NOTICE: DO NOT MODIFY THEFOLLOWING LINE" << endl;
//         *stream << "string defaultparaFile = \"./bin/cfd_para.hypara\";" << endl;
//         *stream << endl;
//         *stream << "// set mesh dim, 2 or 3" <<endl;
//         *stream << "int  ndim      = " << _meshDim << ";" <<endl;
//         *stream << "int  nparafile = 1;" << endl;
//         *stream << endl;
// 
//         switch (_task)
//         {
//             //网格转化
//         case PLTGrid:
//             *stream << "int    nsimutask       = 1;" <<endl;
//             *stream << "string parafilename = \"./bin/grid_para.hypara\";" << endl;
//             break;
//             //网格分区
//         case PLTPartion:
//             *stream << "int    nsimutask    = 3;";
//             *stream << "string parafilename = \"./bin/partion.hypara\";" << endl;
//             break;
//             //求解
//         case PLTCFDSolve:
//             *stream << "int    nsimutask    = 0;";
//             *stream << "string parafilename = \"" << this->getCFDSolverParaFileName() << "\";"<<endl;
//             break;
//         }
//         //高级参数
//         *stream << endl;
//         *stream << "int iorlap                  = 0;";
//         *stream << "int numberofGridProcessor   = 0;";
//         *stream << endl;
// 
//         this->closeFileStream();
//         return true;
//     }
// 
//     QString FITKPHengLEIParaWriterKey::getCFDSolverParaFileName()
//     {
//         //获取当前求解器类型
//         FITKPHengLEIData* phe = FITKPHengLEIData::GetPHengLEIDataFromFrame();
//         if (phe == nullptr) return QString();
//         FITKPHengLEIAbsSolver* solver = phe->getSolver();
//         if (solver == nullptr) return QString();
// 
//         //根据类型，确定参数文件
//         FITKPHengLEIAbsSolver::PHengLEISolverType t = solver->getSolverType();
//         switch (t)
//         {
//         case FITKPHengLEIAbsSolver::PLSubsonic:
//             return QString("cfd_para_subsonic.hypara");
//         case FITKPHengLEIAbsSolver::PLTransonic:
//             return QString("cfd_para_transonic.hypara");
//         case FITKPHengLEIAbsSolver::PLSupersonic:
//             return QString("cfd_para_supersonic.hypara");
//         case FITKPHengLEIAbsSolver::PLHypersonic:
//             return QString("cfd_para_hypersonic.hypara");
//         default: 
//             return QString();
//         }
//         //预防错误
//         return QString();
// 
//     }
//  
// 
// }
// 
// 
