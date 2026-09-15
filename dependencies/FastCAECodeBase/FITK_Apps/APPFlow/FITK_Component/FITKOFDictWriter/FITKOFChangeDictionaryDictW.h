/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFChangeDictionaryDictW.h
 * @brief  边界数据写出接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-28
 *********************************************************************/
#ifndef __FITKFLOWOFCHANGEDICTIONARYDICTWRITER_H__
#define __FITKFLOWOFCHANGEDICTIONARYDICTWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface 
{
    class FITKAbstractParameter;
    class FITKOFBoundary;
    class FITKOFAbsBoundaryType;
    class FITKOFInitialConditions;
    class FITKOFBoundaryManager;
    class FITKAbstractOFSolver;
}

namespace DICT
{
    class FITKDictVector;
    class FITKDictArray;
    class FITKDictGroup;
}

namespace IO
{
    /**
     * @brief ControlMeshDict 文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFChangeDictionaryDictW : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFChangeDictionaryDictW();
        virtual ~FITKOFChangeDictionaryDictW();

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

        bool paraBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* boundaryG, QString nameV, int regionMeshID = -1, int isBasic = true);

    private:
        /**
         * @brief 设置字典里的FoamFile
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        void dictObjectFoamFile();
        /**
         * @brief    写出字典文件
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-28
         */
        bool changeDictBoundaryFieldW();

        bool boundaryFieldW(QString boundMeshName, QString nameV, Interface::FITKOFAbsBoundaryType* boundPara);

        bool paraZeroGradientBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraFixedFluxPressureBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraFixedValueBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraStandardWallFunctionBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);
        bool paraVLowReWallFunctionBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);
        bool paraNoSlipBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraTurbulentIntensityInletBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);
        bool paraTurbulentMixingLengthInletBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);
        bool paraTotalPressureBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraPressureIOVelocityBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);
        bool paraSurNorFixedValueBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraSymmetryBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraOutletPhaseMeanVelocityBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);
        bool paraInletOutletBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);
        bool paraVariableHeightBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraTotalTemperatureBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName);
        bool paraCoupledTemperatureAndRadiationBTyW(Interface::FITKOFAbsBoundaryType* boundPara, DICT::FITKDictGroup* group, QString vName, int regionMeshID, int isBasic);

        QString getInitialBasicParaValue(QString vName);

        QString getInitialRegionsParaValue(QString vName, int regionMeshID);

    private:
        /**
         * @brief 文件名
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-07-19
         */
        QString m_fileName{};
        /**
         * @brief  init边界数据对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-03
         */
        Interface::FITKOFInitialConditions* _initialCond{};
        /**
         * @brief  边界管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-03
         */
        Interface::FITKOFBoundaryManager* _boundaryMgr{};
        /**
         * @brief  求解器数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-05
         */
        Interface::FITKAbstractOFSolver* _solver{};
    };
}



#endif
