/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKOFRadiationPropDictWriter.h
 * @brief  辐射属性字典文件写出
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-06-26
 *********************************************************************/
#ifndef __FITKOFRADIATIONPROPDICTWRITER_H__
#define __FITKOFRADIATIONPROPDICTWRITER_H__

#include "FITKOFDictWriterAPI.h"
#include "FITKOFAbstractDictWriter.h"

namespace Interface
{
    class FITKAbstractRegionMeshSize;
    class FITKAbstractOFSolver;
    class FITKOFAbsRadiationSunDirection;
    class FITKOFAbsRadiationSunLoad;
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
     * @brief  辐射属性字典文件写出
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date 2024-07-15
     */
    class FITKOFRadiationPropDictWriter : public FITKOFAbstractDictWriter
    {
    public:
        explicit FITKOFRadiationPropDictWriter();
        virtual ~FITKOFRadiationPropDictWriter();

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
         * @brief    辐射流体属性写出接口
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-06-26
         */
        bool radiationFluidPropDictW();
        /**
         * @brief    辐射固体属性写出接口
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool radiationSolidPropDictW();
        /**
         * @brief    写出辐射模型的参数数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool radiationModelParaW();
        /**
         * @brief    太阳源参数写出
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool solarLoadPropW();
        /**
         * @brief    写出太阳源方向参数
         * @param[i] direSun 
         * @param[i] group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool sunDirectionParaW(Interface::FITKOFAbsRadiationSunDirection * direSun, DICT::FITKDictGroup* group);
        /**
         * @brief    写出太阳源载荷参数
         * @param[i] loadSun 
         * @param[i] group 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-07-01
         */
        bool sunLoadParaW(Interface::FITKOFAbsRadiationSunLoad * loadSun, DICT::FITKDictGroup* group);

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
