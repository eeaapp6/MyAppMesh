/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @brief  流体后处理结果分析抽象类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 */
#ifndef __FITKINTERFACESTRUPOSTVTK_H_ABS_
#define __FITKINTERFACESTRUPOSTVTK_H_ABS_

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKCFDPostEnum.h"

#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include "FITK_Kernel/FITKCore/FITKCoreMacros.h"

class vtkMapper;
class vtkDataSet;
class vtkTextProperty;
class vtkScalarBarWidget;
class vtkLookupTable;
class vtkAssembly;
class vtkActor;
class vtkRenderWindowInteractor;
class vtkAlgorithmOutput;
class vtkFeatureEdges;
class vtkDataSetMapper;

namespace Interface
{
    class FITKCFDPostColorLibaryData;
    /**
     * @brief  流体后处理结果分析抽象类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKAbstractCFDPostData : public Core::FITKAbstractNDataObject, public Core::FITKVarientParams
    {
        Q_OBJECT;
        FITKCLASS(Interface, FITKAbstractCFDPostData);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKAbstractCFDPostData();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKAbstractCFDPostData();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType() = 0;
        /**
         * @brief    更新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void update();
        /**
         * @brief    获取所有的mapper
         * @return   QList<vtkDataSetMapper*>   所有的mapper
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        QList<vtkMapper*> getMappers();
        /**
         * @brief    获取数据
         * @return   vtkDataSet*    数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual vtkDataSet* getOutput();
        /**
         * @brief    获取数据
         * @return   vtkAlgorithmOutput*   算法
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual vtkAlgorithmOutput* getOutputPort();
        /**
         * @brief    获取父对象id
         * @return   int    父对象id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-07
         */
        virtual int getCFDPostParentID();
        /**
         * @brief    设置操作器
         * @param[i] interactor        操作器 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        virtual void setInteractor(vtkRenderWindowInteractor* interactor);
        /**
         * @brief    设置标量界面交互器
         * @param[i] interactor     交互器
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void setScalarBarWidgetInteractor(vtkRenderWindowInteractor* interactor);
        /**
         * @brief 获取点数据名称
         * @return QStringList        点数据名称
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-11
         */
        QStringList getPointDataArray();
        /**
         * @brief 获取单元数据名称
         * @return QStringList        单元数据名称
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-11
         */
        QStringList getCellDataArray();
        /**
         * @brief    获取变量类型
         * @param[i] fieldType          点/单元
         * @param[i] fieldName          变量名称
         * @return   VariabelType       变量类型（标量、矢量、张量）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        FITKPostVariableType getVariableType(FITKPostFieldType fieldType, QString fieldName);
        /**
         * @brief    获取变量数量
         * @param[i] fieldType          点/单元
         * @param[i] fieldName          变量名称
         * @return   int                变量数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        int getVariableComponentCount(FITKPostFieldType fieldType, QString fieldName);
        /**
         * @brief    获取标量值界面
         * @return   vtkScalarBarWidget*     标量值界面
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        vtkScalarBarWidget* getScalarBarWidget();
        /**
         * @brief    获取色带数量
         * @return   int        色带数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        int getScalaraBarColorNum();
        /**
         * @brief    设置色带层数
         * @param[i] count      色带层数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        void setScalarBarColorNum(int count);
        /**
         * @brief    设置色带范围
         * @param[i] minRange          最小值
         * @param[i] maxRange          最大值
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        void setScalarColorRange(double minRange, double maxRange);
        /**
         * @brief    获取色带范围
         * @param[i] minRange          最小值
         * @param[i] maxRange          最大值
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        void getScalarColorRange(double& minRange, double& maxRange);
        /**
         * @brief    获取某点或单元场量数值
         * @param[i] id               点或单元id
         * @param[i] type             点或单元
         * @param[i] fieldName        场名称
         * @return   double           场量数值
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-24
         */
        double getFieldValue(int id, FITKPostFieldType type, QString fieldName);
        /**
         * @brief    获取场量范围
         * @param[o] range             范围 
         * @param[i] type              类型（点、单元）
         * @param[i] fieldName         名称（场量名称）
         * @param[i] componentIndex    索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        void getRange(double* range, FITKPostFieldType type, QString fieldName, int componentIndex = -1);
        /**
         * @brief    获取极值
         * @param[0] range            数据[2](Min、Max)
         * @param[0] point            数据[6](MinX、MaxX、MinY、MaxY、MinZ、MaxZ)
         * @param[i] type             场量类型
         * @param[i] fieldName        场量名称
         * @param[i] componentIndex   索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-12
         */
        void getExtremum(double* range, double* point, FITKPostFieldType type, QString fieldName, int componentIndex = -1);
        /**
         * @brief    设置颜色范围
         * @param[i] maxColor      最大颜色值（红色0-1）      
         * @param[i] buleColro     最小颜色值（蓝色0-1）
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        void setHueRange(double maxColor, double minColor);
        /**
         * @brief    获取颜色范围
         * @return   double*   颜色范围
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        void getHueRange(double* range);
        /**
         * @brief    获取点数量
         * @return   int       点数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        int getPointNumber();
        /**
         * @brief    获取单元数量
         * @return   int       单元数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        int getCellNumber();
        /**
         * @brief    获取网格类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        FITKPostGridType getGridType();
        /**
         * @brief    获取范围
         * @param[o] bounds      范围[6]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        void getBounds(double* bounds);
        /**
         * @brief    获取中心点
         * @param[o] origin      中心[3] 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-08
         */
        void getOrigin(double* origin);
        /**
         * @brief    获取网格单元类型
         * @return   QHash<int, int>    网格单元类型<单元类型，数量>
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-23
         */
        QHash<int, int> getGridCellType();
        //更新颜色
        void updateColor();
        /**
         * @brief    设置颜色数据
         * @param[i] colorData     颜色数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        void setColorData(Interface::FITKCFDPostColorLibaryData* colorData);
        /**
         * @brief    获取当前数据颜色
         * @return   Interface::FITKCFDPostColorLibaryData*   数据颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        Interface::FITKCFDPostColorLibaryData* getColorData();
        /**
         * @brief    获取特征Mapper
         * @return   vtkDataSetMapper*          特征Mapper
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-12-29
         */
        vtkDataSetMapper* getFeatureEdgeMapper();
    signals:
        ;
        /**
         * @brief    后处理更新信号
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-28
         */
        void sigCFDPostDataUpdate();
    protected:
        /**
         * @brief    初始化特征边
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-12-29
         */
        void initFeatureEdge();
    protected:
        /**
         * @brief vtkMapper
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-11
         */
        QList<vtkMapper*> _mappers = {};
        /**
         * @brief 文本属性
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-11
         */
        vtkTextProperty* _textProperty = nullptr;
        /**
         * @brief 标量界面
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-11
         */
        vtkScalarBarWidget* _scalarBarWidget = nullptr;
        /**
         * @brief  映射器对象用来映射标量的对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-08
         */
        vtkLookupTable* _lookUpTable = nullptr;
        /**
         * @brief  默认ScalarBarWidget色带数量
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-12
         */
        const int _defaultScalarBarNum = 5;
        /**
         * @brief  默认颜色范围
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-12
         */
        const QPair<double, double> _defaultHue = { 0.67,0.0 };
        /**
         * @brief  父对象id
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-12
         */
        int _CFDPostParentID = -1;
        /**
         * @brief  该数据对应的色库
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-29
         */
        FITKCFDPostColorLibaryData* _colorLibData = nullptr;
        /**
         * @brief  特征边
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-12-29
         */
        vtkFeatureEdges* _featureEdge = nullptr;
        /**
         * @brief  特征边Mapper
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-12-29
         */
        vtkDataSetMapper* _featureEdgeMapper = nullptr;
    };
}
#endif
