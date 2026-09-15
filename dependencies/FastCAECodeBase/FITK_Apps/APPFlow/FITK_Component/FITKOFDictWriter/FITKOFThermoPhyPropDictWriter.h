/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFThermoPhyPropDictWriter.h
 * @brief  热物理属性字典文件写出
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-26
 *********************************************************************/
#ifndef __FITKOFTHERMOPHYPROPDICTWRITER_H__
#define __FITKOFTHERMOPHYPROPDICTWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKOFAbsThermoPhysicalThermodynamics;
    class FITKOFAbsThermoPhysicalTransport;
    class FITKOFAbsThermoPhysicalEquationOfState;
    class FITKOFThermoPhysicalProp;
}

namespace Core
{
    class FITKParameter;
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
     * @brief  热物理属性字典文件写出器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-06-26
     */
    class FITKOFThermoPhyPropDictWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFThermoPhyPropDictWriter();
        virtual ~FITKOFThermoPhyPropDictWriter();

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
         * @brief    流体热物理属性写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-30
         */
        bool thermoFluidPhyPropDictW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp);
        /**
         * @brief    固体热物理属性写出
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-26
         */
        bool thermoSolidPhyPropDictW(Interface::FITKOFThermoPhysicalProp* thermoPhyProp);
        /**
         * @brief    specie属性
         * @param[i] group
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool speciePropW(Core::FITKParameter* specie, DICT::FITKDictGroup* group);
        /**
         * @brief    thermodynamics属性
         * @param[i] thermodynamics
         * @param[i] group
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool thermodynamicsPropW(Interface::FITKOFAbsThermoPhysicalThermodynamics* thermodynamics, DICT::FITKDictGroup* group);
        /**
         * @brief    transport属性
         * @param[i] group
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool transportPropW(Interface::FITKOFAbsThermoPhysicalTransport* transport, DICT::FITKDictGroup* group);
        /**
         * @brief    equationOfState属性
         * @param[i] group
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool equationOfStatePropW(Interface::FITKOFAbsThermoPhysicalEquationOfState* equation, DICT::FITKDictGroup* group);

        bool parametersW(Core::FITKParameter* para, DICT::FITKDictGroup* group);
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
