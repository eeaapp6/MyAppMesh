/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiationParaManager.h
 * @brief  Radiation 参数管理器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-18
 *********************************************************************/
#ifndef _FITK___OF__RADIATION__PARA_MANAGER__H___
#define _FITK___OF__RADIATION__PARA_MANAGER__H___

#include "FITKInterfaceFlowOFAPI.h"
#include "FITKOFEnum.hpp"
#include <functional>
#include <QHash>
#include <QStringList>

namespace Interface
{
    class FITKOFAbsRadiationModel;
    class FITKOFAbsRadiationAbsorptionModel;
    class FITKOFAbsRadiationScatterModel;
    class FITKOFAbsRadiationSunDirection;
    class FITKOFAbsRadiationSunLoad;
}

typedef std::function<Interface::FITKOFAbsRadiationModel*()>  CREATERADIATIONMODELFUN;
typedef std::function<Interface::FITKOFAbsRadiationAbsorptionModel*()>  CREATERADIATIONABSORPTIONMODELFUN;
typedef std::function<Interface::FITKOFAbsRadiationScatterModel*()>  CREATERADIATIONSCATTERMODELFUN;
typedef std::function<Interface::FITKOFAbsRadiationSunDirection*()>  CREATERADIATIONSUNDIRECTIONFUN;
typedef std::function<Interface::FITKOFAbsRadiationSunLoad*()>  CREATERADIATIONSUNLOADFUN;


namespace Interface
{
    /**
     * @brief  RadiationModel信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFRadiationModelInfo
    {
        QString _radiationModelName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableType;//SolverType
        CREATERADIATIONMODELFUN _createFuns{};
    };
    /**
     * @brief  RadiationAbsorptionModel信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFRadiationAbsorptionModelInfo
    {
        QString _radiationAbsorptionModelName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableType;//SolverType
        CREATERADIATIONABSORPTIONMODELFUN _createFuns{};
    };
    /**
     * @brief  RadiationScatterModel信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFRadiationScatterModelInfo
    {
        QString _radiationScatterModelName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableType;//SolverType
        CREATERADIATIONSCATTERMODELFUN _createFuns{};
    };
    /**
     * @brief  RadiationSunDirection信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFRadiationSunDirectionInfo
    {
        QString _radiationSunDirectionName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableType;//SolverType
        CREATERADIATIONSUNDIRECTIONFUN _createFuns{};
    };
    /**
     * @brief  RadiationSunLoad信息 结构体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    struct FITKOFRadiationSunLoadInfo
    {
        QString _radiationSunLoadName{};
        QList<FITKOFSolverTypeEnum::FITKOFSolverType> _enableType;//SolverType
        CREATERADIATIONSUNLOADFUN _createFuns{};
    };
    /**
     * @brief  Radiation Para管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-21
     */
    class FITKInterfaceFlowOFAPI FITKOFRadiationParaManager
    {
    public:
        explicit FITKOFRadiationParaManager();
        virtual ~FITKOFRadiationParaManager();

        /**
         * @brief    过滤Radiation Model类型
         * @param[i] solverType 
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QStringList filterRadiationModels(FITKOFSolverTypeEnum::FITKOFSolverType solverType);
        /**
         * @brief    获取Radiation Model信息
         * @param[i] radiationModelName Radiation Model名称
         * @return   FITKOFRadiationModelInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFRadiationModelInfo getRadiationModelInfo(QString radiationModelName);
        /**
         * @brief    过滤Radiation Absorption Model类型
         * @param[i] solverType
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QStringList filterRadiationAbsorptionModels(FITKOFSolverTypeEnum::FITKOFSolverType solverType);
        /**
         * @brief    获取Radiation Absorption Model信息
         * @param[i] radiationModelName Radiation Model名称
         * @return   FITKOFRadiationModelInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFRadiationAbsorptionModelInfo getRadiationAbsorptionModelInfo(QString radiationModelName);
        /**
         * @brief    过滤Radiation Scatter Model类型
         * @param[i] solverType
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QStringList filterRadiationScatterModels(FITKOFSolverTypeEnum::FITKOFSolverType solverType);
        /**
         * @brief    获取Radiation Scatter Model信息
         * @param[i] radiationModelName Radiation Model名称
         * @return   FITKOFRadiationModelInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFRadiationScatterModelInfo getRadiationScatterModelInfo(QString radiationModelName);
        /**
         * @brief    过滤Radiation SunDirection类型
         * @param[i] solverType
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QStringList filterRadiationSunDirections(FITKOFSolverTypeEnum::FITKOFSolverType solverType);
        /**
         * @brief    获取Radiation SunDirection信息
         * @param[i] radiationModelName Radiation Model名称
         * @return   FITKOFRadiationModelInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFRadiationSunDirectionInfo getRadiationSunDirectionInfo(QString radiationModelName);
        /**
         * @brief    过滤Radiation SunLoad类型
         * @param[i] solverType
         * @return   QStringList
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        QStringList filterRadiationSunLoads(FITKOFSolverTypeEnum::FITKOFSolverType solverType);
        /**
         * @brief    获取Radiation SunLoad信息
         * @param[i] radiationModelName Radiation Model名称
         * @return   FITKOFRadiationModelInfo
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKOFRadiationSunLoadInfo getRadiationSunLoadInfo(QString radiationModelName);

    private:
        /**
         * @brief    初始化Radiation Models数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initRadiationModels();
        /**
         * @brief    初始化Radiation Absorption Models数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initRadiationAbsorptionModels();
        /**
         * @brief    初始化Radiation Scatter Models数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        void initRadiationScatterModels();
        /**
         * @brief    初始化Radiation SunDirections数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        void initRadiationSunDirections();
        /**
         * @brief    初始化Radiation SunLoads数据信息
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-18
         */
        void initRadiationSunLoads();
    
    private:
        /**
         * @brief  Solution Solver数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-08-21
         */
        QList<FITKOFRadiationModelInfo> m_radiationModels{};
        /**
         * @brief  Solution Absorption Solver数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        QList<FITKOFRadiationAbsorptionModelInfo> m_radiationAbsorptionModels{};
        /**
         * @brief  Solution Scatter Solver数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        QList<FITKOFRadiationScatterModelInfo> m_radiationScatterModels{};
        /**
         * @brief  Solution SunDirection数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        QList<FITKOFRadiationSunDirectionInfo> m_radiationSunDirections{};
        /**
         * @brief  Solution SunLoad数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-06-18
         */
        QList<FITKOFRadiationSunLoadInfo> m_radiationSunLoads{};
    };
}


#endif
