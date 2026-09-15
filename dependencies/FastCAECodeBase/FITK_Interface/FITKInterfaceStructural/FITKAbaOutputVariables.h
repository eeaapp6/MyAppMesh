/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaOutputVariables.h
 * @brief  输出变量
 * @author LiJin (1003680421@qq.com)
 * @date   2024-04-11
 * 
 */
#ifndef FITKABAOUTPUTVARIABLES_H
#define FITKABAOUTPUTVARIABLES_H
#include "FITKInterfaceStructuralAPI.h"
#include <QObject>
#include "FITKAbaAbstractStep.h"


namespace Interface {
    
    /**
     * @brief  输出变量枚举
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-12
     */
    class FITKInterfaceStructuralAPI FITKAbaOutputVariable: public QObject
    {
        Q_OBJECT
    public:
        /**
     * @brief  输出作用区域枚举
     * @author LiJin (1003680421@qq.com)
     * @date   2024-04-12
     */
        enum OutputDomain
        {
            WholeModel,
            Set,
            BoltLoad,
            CompositeLayup,
            Crack,
            Fastener,
            AssembledFastennerSet,
            GeneralContactSurface,
            Substructure,
            Interaction,
            Skin,
            SpringDashpot,
            Stringer,
            IntegratedOutputSection
        };
        Q_ENUM(OutputDomain);
        /**
         * @brief  输出频率
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-12
         */
        enum OutputFrequency
        {
            LastIncrement,
            EvertyNIncreaments,
            EvenlySpacedTimeIntervals,
            EvertyXUnitsOfTime,
            FromTimePoints,
            AllModes,
            SpecifyModes,
            EveryTimeIncrement
        };
        Q_ENUM(OutputFrequency);
        /**
         * @brief  输出定时枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        enum OutputTiming
        {
            OutputAtApproximateTimes,
            OutputAtExactTimes
        };
        Q_ENUM(OutputTiming);
        /**
         * @brief  输出变量的选择模式
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-12
         */
        enum OutputVariablesSelectMode
        {
            SelectFromListBelow,
            PreselectedDefaults,
            All,
            EditVariables
        };
        Q_ENUM(OutputVariablesSelectMode);
        /**
         * @brief  壳、梁及复合层截面点输出枚举类
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        enum OutputAtSectionPoints
        {
            UseDefaults,
            Specify
        };
        Q_ENUM(OutputAtSectionPoints);

    public:
        /**
         * @brief  输出变量枚举0~999为节点变量，1000~1999为单元变量，2000~2999为接触变量，
         * 3000~3999为能量变量
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-15
         */
        enum OutputVariable
        {
            //节点变量 Nodal Output
            A = 0,
            AR,
            AT,
            CF,
            COORD,
            PABS,
            POR,
            RF,
            RM,
            RT,
            TIEADJUST,
            TIEDSTATUS,
            U,
            UR,
            UT,
            V,
            VR,
            VT,
            A1,
            A2,
            A3,
            AR1,
            AR2,
            AR3,
            CF1,
            CF2,
            CF3,
            CM1,
            CM2,
            CM3,
            RF1,
            RF2,
            RF3,
            RM1,
            RM2,
            RM3,
            RWM,
            U1,
            U2,
            U3,
            UR1,
            UR2,
            UR3,
            V1,
            V2,
            V3,
            VR1,
            VR2,
            VR3,
            //单元变量 Element Output
            BF = 1000,
            BURNF,
            CDERF,
            CDERU,
            CDMG,
            CEF,
            CTF,
            CTF1,
            CTF2,
            CTF3,
            CTM1,
            CTM2,
            CTM3,
            CU,
            CU1,
            CU2,
            CU3,
            CUR1,
            CUR2,
            CUR3,
            CUE,
            CUF,
            CUP,
            CUPEQ,
            CVF,
            CFAILURE,
            CTSHR,
            DAMAGEC,
            DAMAGEFC,
            DAMAGEFT,
            DAMAGEMC,
            DAMAGEMT,
            DAMAGESHR,
            DAMAGET,
            DBURNF,
            DENSITY,
            DENSITYVAVG,
            DMICRT,
            DMICRTMAX,
            E,
            EACTIVE,
            EDCDEN,
            EDMICRTMAX,
            EDT,
            EEQUT,
            EFABRIC,
            ELEDEN,
            ELEN,
            EMSF,
            ENER,
            ER,
            ERPRATIO,
            ERV,
            ESDV,
            ESOL,
            EVF,
            EVOL,
            FV,
            GRAV,
            HP,
            HSNFCCRT,
            HSNFTCRT,
            HSNMCCRT,
            HSNMTCRT,
            IVOL,
            IWCONWEP,
            LE,
            MFL,
            MFLT,
            MFR,
            MISES,
            MISESMAX,
            MISESONLY,
            MVF,
            NE,
            NEEQ,
            NEEQR,
            NFORC,
            NFORCSO,
            P,
            PE,
            PEEQ,
            PEEQMAX,
            PEEQT,
            PEEQVAVG,
            PEMAG,
            PEQC,
            PEVAVG,
            PRESSONLY,
            PS,
            RD,
            RHOE,
            RHOP,
            S,
            SBF,
            SDEG,
            SDV,
            SE,
            SEQUT,
            SF,
            SFABRIC,
            SHRRATIO,
            SSAVG,
            STAGP,
            STATUS,
            STH,
            SVAVG,
            SVOL,
            TE,
            TEEQ,
            TEMP,
            TEMPMAVG,
            TEVOL,
            TRIAX,
            TRNOR,
            TRSHR,
            TSHR,
            UVARM,
            VE,
            VEEQ,
            VP,
            VS,
            VVF,
            VVFG,
            VVFN,
            YIELDPOT,
            CTSHR13,
            CTSHR23,
            INV3,
            PRESS,
            PS11,
            PS12,
            PS13,
            PS22,
            PS23,
            PS33,
            S11,
            S12,
            S13,
            S22,
            S23,
            S33,
            SFABRIC11,
            SFABRIC12,
            SFABRIC13,
            SFABRIC22,
            SFABRIC23,
            SFABRIC33,
            SP,
            SSAVG1,
            SSAVG2,
            SSAVG3,
            SSAVG4,
            SSAVG5,
            SSAVG6,
            TRESC,
            TSHR13,
            TSHR23,
            VS11,
            VS12,
            VS13,
            VS22,
            VS23,
            VS33,
            BICURV,
            E11,
            E12,
            E13,
            E22,
            E23,
            E33,
            EFABRIC11,
            EFABRIC12,
            EFABRIC13,
            EFABRIC22,
            EFABRIC23,
            EFABRIC33,
            ER11,
            ER12,
            ER13,
            ER22,
            ER23,
            ER33,
            LE11,
            LE12,
            LE13,
            LE22,
            LE23,
            LE33,
            LEP,
            NE11,
            NE12,
            NE13,
            NE22,
            NE23,
            NE33,
            NEP,
            PE11,
            PE12,
            PE13,
            PE22,
            PE23,
            PE33,
            PEP,
            PEQC1,
            PEQC2,
            PEQC3,
            PEQC4,
            SE1,
            SE2,
            SE3,
            SF1,
            SF2,
            SF3,
            SK1,
            SK2,
            SK3,
            SM1,
            SM2,
            SM3,
            TE11,
            TE12,
            TE13,
            TE22,
            TE23,
            TE33,
            VE11,
            VE12,
            VE13,
            VE22,
            VE23,
            VE33,
            IRA1,
            IRA2,
            IRA3,
            IRAR1,
            IRAR2,
            IRAR3,
            IRF1,
            IRF2,
            IRF3,
            IRM1,
            IRM2,
            IRM3,
            IRMASS,
            IRRI11,
            IRRI12,
            IRRI13,
            IRRI22,
            IRRI23,
            IRRI33,
            IRX1,
            IRX2,
            IRX3,
            MASS,
            // Contact Output
            CDISP = 2000,
            CFORCE,
            CSDMG,
            CSLIPR,
            CSMAXSCRT,
            CTANDIR,
            CTHICK,
            FSLIP,
            FSLIPR,
            PPRESS,
            CAREA,
            CFN1,
            CFN2,
            CFN3,
            CFNM,
            CFS1,
            CFS2,
            CFS3,
            CFSM,
            CFT1,
            CFT2,
            CFT3,
            CFTM,
            CICPS,
            CMN1,
            CMN2,
            CMN3,
            CMNM,
            CMS1,
            CMS2,
            CMS3,
            CMSM,
            CMT1,
            CMT2,
            CMT3,
            CMTM,
            XN1,
            XN2,
            XN3,
            XS1,
            XS2,
            XS3,
            XT1,
            XT2,
            XT3,
            BDSTAT,
            CDSTRESS,
            CRSTS11,
            CRSTS12,
            CRSTS13,
            CSMAXUCRT,
            CSQUADSCRT,
            CSQUADUCRT,
            CSTRESS,
            CTRQ,
            DBS11,
            DBS12,
            DBSF,
            DBT,
            // Energy Output
            ALLAE = 3000,
            ALLCD,
            ALLCW,
            ALLDC,
            ALLDMD,
            ALLFD,
            ALLIE,
            ALLKE,
            ALLMW,
            ALLPD,
            ALLPW,
            ALLSE,
            ALLVD,
            ALLWK,
            ALLEE,
            ETOTAL,
            // Incrementation Output
            DT = 4000,
            DMASS,
            // Integrated Output
            ACOM1 = 5000,
            ACOM2,
            ACOM3,
            UCOM1,
            UCOM2,
            UCOM3,
            VCOM1,
            VCOM2,
            VCOM3,
        };
        Q_ENUM(OutputVariable)

        /**
         * @brief  获取变量的父节点列表，用于生成变量分类树结构。
         * @param[in]  
         * @return 父节点名称字符串列表。多级结构时，第一个为父级名称，第二个为父级的父级名称
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-22
         */
        static QStringList getVariableParents(const QString& var);
        /**
         * @brief  获取变量的描述
         * @param[in]  
         * @return 
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-23
         */
        static QString getVariableDescription(const QString& var);
    private:
        /**
        * @brief  变量的父节点名称
        * @author LiJin (1003680421@qq.com)
        * @date   2024-04-22
        */
        static const QMap<OutputVariable, QString> VaraiableParent;
        /**
        * @brief  中间节点的父节点名称
        * @author LiJin (1003680421@qq.com)
        * @date   2024-04-22
        */
        static const QMap<QString, QString> MiddleLabels;
        /**
         * @brief  变量的解释性描述
         * @author LiJin (1003680421@qq.com)
         * @date   2024-04-28
         */
        static const QMap<OutputVariable, QString> VariableDescription;
    };
}


#endif // FITKABAOUTPUTVARIABLES_H
