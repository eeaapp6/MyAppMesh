/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostGlyph.h
 * @brief  后处理图像数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-06
 *********************************************************************/
#ifndef _FITKCFDPostGlyph_H
#define _FITKCFDPostGlyph_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKAbstractCFDPostData.h"

namespace Interface
{
    class FITKAlgGlyph;
    /**
     * @brief  后处理图像数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-06
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostGlyph :public FITKAbstractCFDPostData
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        explicit FITKCFDPostGlyph(int parentID);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual ~FITKCFDPostGlyph();
        /**
         * @brief    获取后处理数据类型
         * @return   FITKPostDataType        后处理数据类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-14
         */
        virtual FITKPostDataType getPostDataType();
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
         * @brief    更新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-06
         */
        virtual void update() override;
        /**
         * @brief    获取矢量名称
         * @return   QString       矢量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        QString getVectorName();
        /**
         * @brief    设置矢量名称
         * @param[i] vectorName    矢量名称
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setVectorName(QString vectorName);
        /**
         * @brief    获取箭头维度
         * @return   int            维度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getTipResolution();
        /**
         * @brief    设置箭头维度
         * @param[i] tipResolution  维度 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setTipResolution(int tipResolution);
        /**
         * @brief    获取箭头半径
         * @return   double         半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getTipRadius();
        /**
         * @brief    设置箭头半径
         * @param[i] tipRadius      半径
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setTipRadius(double tipRadius);
        /**
         * @brief    获取箭头长度
         * @return   double         长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getTipLength();
        /**
         * @brief    设置箭头长度
         * @param[i] tipLength      长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setTipLength(double tipLength);
        /**
         * @brief    获取箭柄维度
         * @return   int           维度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getShaftResolution();
        /**
         * @brief    设置箭柄维度
         * @param[i] shaftResolution 
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setShaftResolution(int shaftResolution);
        /**
         * @brief    获取箭柄半径
         * @return   double         长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getShaftRadius();
        /**
         * @brief    设置箭柄半径
         * @param[i] tipLength      长度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setShaftRadius(double shaftRadius);
        /**
         * @brief    获取箭头缩放比例
         * @return   double         比例
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        double getScaleFactor();
        /**
         * @brief    设置箭头缩放比例
         * @param[i] tipLength      比例
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setScaleFactor(double scaleFactor);
        /**
         * @brief    获取是否使用点数据
         * @return   bool      是否使用点数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        bool getPointDataUsed();
        /**
         * @brief    获取是否使用点数据
         * @param[i] pointDataUsed  是否使用点数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setPointDataUsed(bool pointDataUsed);
        /**
         * @brief    获取点的数量
         * @return   int           数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getPointNum();
        /**
         * @brief    设置点的数量
         * @param[i] pointNum      数量
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setPointNum(int pointNum);
        /**
         * @brief    获取最大采样点
         * @return   int           最大采样点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getMaxNumber();
        /**
         * @brief    设置最大采样点
         * @param[i] pointNum      最大采样点
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setMaxNumber(int maxNumber);
        /**
         * @brief    获取箭头比例模式
         * @return   int           箭头比例模式:0-scalar,1-vector,2-vectorcomponent,3-off
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        int getScaleMode();
        /**
         * @brief    设置箭头比例模式
         * @param[i] pointNum      箭头比例模式:0-scalar,1-vector,2-vectorcomponent,3-off
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-30
         */
        void setScaleMode(int scaleMode);
    private:
        /**
         * @brief  图像算法数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-24
         */
        FITKAlgGlyph* _glyphAlg = nullptr;
        /**
         * @brief  矢量名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        QString _vectorName = "";
        /**
         * @brief  箭头维度
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _tipResolution = 6;
        /**
         * @brief  箭头半径
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _tipRadius = 0.2;
        /**
         * @brief  箭头长度
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _tipLength = 0.1;
        /**
         * @brief  箭柄维度
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _shaftResolution = 6;
        /**
         * @brief  箭柄半径
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _shaftRadius = 0.1;
        /**
         * @brief  箭头缩放比例
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        double _scaleFactor = 0.01;
        /**
         * @brief  使用点数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        bool _pointDataUsed = true;
        /**
         * @brief  点数量
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _pointNum = 0;
        /**
         * @brief  最大采样点
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _maxNumber = 100000;
        /**
         * @brief  箭头比例模式:0-scalar,1-vector,2-vectorcomponent,3-off
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-30
         */
        int _scaleMode = 1;
    };
}

#endif
