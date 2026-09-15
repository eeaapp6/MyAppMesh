/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

// /**
//  * 
//  * @file FITKPHengLEIParaWriterKey.h
//  * @brief Key 控制文件写出 
//  * @author LiBaojun (libaojunqd@foxmail.com)
//  * @date 2024-09-12
//  * 
//  */
// #ifndef _FITKPARAMWRITER_PHENGLEI_KEY_H___
// #define _FITKPARAMWRITER_PHENGLEI_KEY_H___
//  
// #include "FITKPHengLEIAbsParaWriter.h"
// #include <QString>
// 
// namespace Interface
// {
//     /**
//      * @brief 任务执行阶段
//      * @author LiBaojun (libaojunqd@foxmail.com)
//      * @date 2024-09-12
//      */
//     enum PHengLEITask
//     {
//         PLTNone,       ///< 无任务
//         PLTGrid,       ///< 网格转化
//         PLTPartion,    ///< 网格分区
//         PLTCFDSolve,   ///< 求解
//     };
//     /**
//      * @brief 写出key控制文件
//      * @author LiBaojun (libaojunqd@foxmail.com)
//      * @date 2024-09-12
//      */
//     class FITKFlowPHengLEIAPI FITKPHengLEIParaWriterKey : public FITKPHengLEIAbsParaWriter
//     {
//     public:
//         /**
//          * @brief Construct a new FITKPHengLEIParaWriterKey object
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         explicit  FITKPHengLEIParaWriterKey() = default;
//         /**
//          * @brief Destroy the FITKPHengLEIParaWriterKey object
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         virtual ~FITKPHengLEIParaWriterKey() = default;
//         /**
//          * @brief 设置当前任务阶段
//          * @param[i]  task         当前任务
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         void setTask(PHengLEITask task);
//         /**
//          * @brief 设置网格维度
//          * @param[i]  dim            网格维度 2 或者3
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         void setMeshDim(int dim);
//         /**
//          * @brief 执行写出操作
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         bool writeParamFile() override;
// 
//     private:
//         /**
//          * @brief 根据不同速域获取文件名称
//          * @return QString 
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         QString getCFDSolverParaFileName();
// 
//     private:
//         /**
//          * @brief 任务阶段
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         PHengLEITask _task{ PLTNone };
//         /**
//          * @brief 网格维度
//          * @author LiBaojun (libaojunqd@foxmail.com)
//          * @date 2024-09-12
//          */
//         int _meshDim{ 2 };
//     };
// }
// 
// 
// #endif
