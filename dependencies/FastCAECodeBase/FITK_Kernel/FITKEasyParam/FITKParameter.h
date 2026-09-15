/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKParameter.h
 * @brief  数据参数接口声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-08-13
 *********************************************************************/
#ifndef FITKParameter_H_
#define FITKParameter_H_

#include "FITKEasyParamAPI.h"
#include "FITKAbstractEasyParam.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Core
{
    class FITKParamBool;
    class FITKParamInt;
    class FITKParamDouble;
    class FITKParamCombox;
    class FITKParamComboxVector;
    class FITKParamString;
    class FITKParamRadioGroup;
    class FITKParamDoubleList;
    class FITKParamBoolGroup;
    /**
     * @brief  数据参数接口类
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-08-13
     */
    class FITKEasyParamAPI FITKParameter : public FITKAbstractDataManager<Core::FITKAbstractEasyParam>
    {
    public:
        explicit FITKParameter() = default;
        virtual ~FITKParameter();

        /**
         * @brief    参数数据拷贝
         * @param[i] data
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-13
         */
        void copyPara(FITKParameter* data);
        /**
         * @brief    追加参数数据拷贝
         * @param[i] data
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-13
         */
        void appendPara(FITKParameter* data);
        /**
         * @brief    获取所用数据
         * @return   QList<FITKAbstractEasyParam*>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        QList<FITKAbstractEasyParam*> getParameter();
        /**
         * @brief    创建Int参数并追加到m_propertyList
         * @param[i] name 名称
         * @param[i] value 初始值
         * @param[i] upperLimit 阈值上限
         * @param[i] lowerLimit 阈值下限
         * @return   FITKParamInt *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKParamInt* createIntParameter(QString name, int value, int upperLimit = 99999999, int lowerLimit = -99999999);
        /**
         * @brief    创建Double参数并追加到m_propertyList
         * @param[i] name 名称
         * @param[i] value 初始值
         * @param[i] upperLimit 阈值上限
         * @param[i] lowerLimit 阈值下限
         * @return   FITKParamDouble *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKParamDouble* createDoubleParameter(QString name, double value, double upperLimit = 99999999, double lowerLimit = -99999999);
        /**
         * @brief    创建DoubleList参数并追加到m_propertyList
         * @param[i] name 名称
         * @param[i] value 初始值
         * @param[i] upperLimit 阈值上限
         * @param[i] lowerLimit 阈值下限
         * @return   FITKParamDoubleList *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        FITKParamDoubleList* createDoubleListParameter(QString name, QList<double> value, double upperLimit = 99999999, double lowerLimit = -99999999);
        /**
         * @brief    创建ComBoxVector参数
         * @param[i] objName                   数据对象名称
         * @param[i] values                    数据
         * @return   FITKParamComboxVector*    数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-25
         */
        FITKParamComboxVector* createComBoxVectorParameter(QString objName, QList<QStringList> values);
        /**
         * @brief    创建String参数并追加到m_propertyList
         * @param[i] name 名称
         * @param[i] value 初始值
         * @return   FITKParamString *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKParamString* createStringParameter(QString name, QString value);
        /**
         * @brief    创建Bool参数并追加到m_propertyList
         * @param[i] name 名称
         * @param[i] value 初始值
         * @return   FITKParamBool *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKParamBool* createBoolParameter(QString name, bool value);
        /**
         * @brief    创建BoolGroup参数并追加到m_propertyList
         * @param[i] name 名称
         * @param[i] value 初始值
         * @return   FITKParamBoolGroup *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-22
         */
        FITKParamBoolGroup* createBoolGroupParameter(QString name, bool value);
        /**
         * @brief    创建Combox参数并追加到m_propertyList
         * @param[i] name 名称
         * @param[i] value 参数
         * @param[i] index 初始值
         * @return   FITKParamCombox *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKParamCombox* createComboxParameter(QString name, QStringList value, int index);
        /**
         * @brief    创建RadioGroup参数并追加到m_propertyList
         * @param[i] name 名称
         * @return   FITKParamRadioGroup *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-21
         */
        FITKParamRadioGroup* createRadioGroupParameter(QString name);
        /**
         * @brief    仅保留名称一致的数据
         * @param[i] data 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-08-23
         */
        void onlyKeepPara(FITKParameter* data);
        /**
         * @brief    拷贝并添加数据
         * @param[i] oldData 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-09-04
         */
        void copyAppendPara(FITKAbstractEasyParam* oldData);

        double getDoubleParaByName(QString name, bool& ok);
        int getIntParaByName(QString name, bool& ok);
        bool getBoolParaByName(QString name, bool& ok);
        QString getStringParaByName(QString name, bool& ok);
        QString getComboxCurrentParaByName(QString name, bool& ok);
    };
}

#endif // !FITKParameter_H_
