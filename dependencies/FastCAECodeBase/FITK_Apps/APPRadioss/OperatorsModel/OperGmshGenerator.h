/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   OperGmshGenerator.h
 * @brief  Gmsh网格生成操作器
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-05
 *********************************************************************/
#ifndef __OPERGMSHGENERATOR_H___
#define __OPERGMSHGENERATOR_H___

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace ModelOper
{
    /**
     * @brief  Gmsh网格生成操作器
     * @author guqingtao (15598887859@163.com)
     * @date   2025-06-06
     */
    class OperGmshGenerator :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        /**
         * @brief    Gmsh网格生成操作器
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-06
         */
        explicit OperGmshGenerator() = default;
        virtual ~OperGmshGenerator() = default;
        /**
         * @brief    执行界面操作
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-06
         */
        virtual bool execGUI() override;
        /**
         * @brief    执行读入操作
         * @return   bool
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-06
         */
        virtual bool execProfession() override;

    private:
        /**
         * @brief    网格生成操作
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-20
         */
        void meshGenOper();
        /**
         * @brief    将网格生成驱动操作写入Python脚本
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-16
         */
        void writePythonScript();

    private slots:
        /**
         * @brief  网格生成完成
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-06
         */
        void meshGenFinished();

    private:
        
    };
    Register2FITKOperatorRepo(actionGmshSettings, OperGmshGenerator);

}
#endif