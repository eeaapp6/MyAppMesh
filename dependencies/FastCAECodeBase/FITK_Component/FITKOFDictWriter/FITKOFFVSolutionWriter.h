/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFFVSolutionWriter.h
 * @brief  fvSolution 写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-27
 *********************************************************************/
#ifndef __FITKFLOWOFFVSOLUTIONWRITER_H__
#define __FITKFLOWOFFVSOLUTIONWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface 
{
    class FITKAbstractParameter;
    class FITKOFAbsSolutionSolver;
    class FITKOFSolution;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictArray;
    class FITKDictGroup;
}

namespace Core
{
    class FITKParameter;
}

namespace IO
{
    /**
     * @brief ControlMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFFVSolutionWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFFVSolutionWriter();
        virtual ~FITKOFFVSolutionWriter();

        /**
         * @brief 设置文件路径
         * @param[i] filePath
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void setFilePath(const QString& filePath) override;
        /**
         * @brief 重写run函数
         * @return 状态 true:成功 false:失败
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-15
         */
        bool run() override;
        /**
         * @brief 获取文件
         * @return QString
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString getFileName() override;

    private:
        /**
         * @brief 设置字典里的FoamFile
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void dictObjectFoamFile(QString location);
        /**
         * @brief    写出区域空类型solution
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool writeNoneRegionSolution();
        /**
         * @brief    写出区域流体类型solution
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool writeFluidRegionSolution();
        /**
         * @brief    写出区域固体类型solution
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool writeSolidRegionSolution();

        /**
         * @brief    写出默认solution
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool writeDefaultSolution();
        /**
         * @brief    写出空solution
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-02
         */
        bool writeEmptySolution();

        bool fvSolutionWrite();

        bool writeAdditionalPara(Core::FITKParameter* additionalPara, DICT::FITKDictGroup* group);
        QString getAdditionalParaKey(QString name);
        bool writeGAMGSolverVariable(Interface::FITKOFAbsSolutionSolver* solverV, DICT::FITKDictGroup* group);
        bool writePCGSolverVariable(Interface::FITKOFAbsSolutionSolver* solverV, DICT::FITKDictGroup* group);
        bool writeSmoothSolverSolverVariable(Interface::FITKOFAbsSolutionSolver* solverV, DICT::FITKDictGroup* group);
        bool writePBiCGStabilizedSolverVariable(Interface::FITKOFAbsSolutionSolver* solverV, DICT::FITKDictGroup* group);
        bool writePBiCGSolverVariable(Interface::FITKOFAbsSolutionSolver* solverV, DICT::FITKDictGroup* group);

        bool arithmeticSIMPLEW(Interface::FITKOFSolution* solutionData);
        bool arithmeticPIMPLEW(Interface::FITKOFSolution* solutionData);
        QString getArithmeticParaKey(QString name);
    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
    };
}



#endif
