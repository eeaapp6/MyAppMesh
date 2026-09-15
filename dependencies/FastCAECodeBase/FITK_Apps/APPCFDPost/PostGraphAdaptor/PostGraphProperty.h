/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphProperty.h
 * @brief  后处理属性类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-09
 *********************************************************************/
#ifndef _PostGraphProperty_H
#define _PostGraphProperty_H

#include <QObject>
#include "PostGraphAdaptorAPI.h"

#include "FITK_Interface/FITKInterfaceCFDPost/FITKAbstractCFDPostData.h"

#define SolidColor "Solid Color"

namespace Interface
{
    class FITKAbstractCFDPostData;
    class PostGraphObjectBase;

    enum class FITKPostDisplayType {
        Post_Point,             //点
        Post_Edge,              //线
        Post_Face,              //面
        Post_FaceWithEdge,      //带线的面
    };
    /**
     * @brief  后处理属性类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-09
     */
    class PostGraphAdaptorAPI PostGraphProperty : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] postData    后处理数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        PostGraphProperty(PostGraphObjectBase* postGraphObj, FITKAbstractCFDPostData* postData);
        /**
         * @brief    析构函数
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        ~PostGraphProperty();
        /**
         * @brief    设置当前场变量
         * @param[i] type              类型（1:点  2:单元）
         * @param[i] vaName            场变量名称
         * @param[i] componentIndex 
         * @return   void
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-09
         */
        void setCurrentField(FITKPostFieldType type, QString fieldName, int componentIndex = -1);
        /**
         * @brief    获取当前场信息
         * @param[o] type             类型（1:点  2:单元）
         * @param[o] filedName        场名称
         * @param[o] componentIndex   索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        void getCurrentField(FITKPostFieldType& type, QString& filedName, int& componentIndex);
        /**
         * @brief    设置显示类型
         * @param[i] type      显示类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        void setCurrentDisplayType(FITKPostDisplayType type);
        /**
         * @brief    获取当前显示类型
         * @return   FITKPostDisplayType    显示类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        FITKPostDisplayType getCurrentDisplayType();
        /**
         * @brief    显示/隐藏标量界面
         * 与setScalarBarWidgetIsShow接口不同的是，调用该接口不会更改属性类中ScalarBarWidget是否显示的标签
         * 只会显示/隐藏ScalarBarWidget
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void hideScalarBarWidget();
        void showScalarBarWidget();
        /**
         * @brief    是否显示标量界面（会记录标签）
         * @param[i] isShow         是否显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        void setScalarBarWidgetIsShow(bool isShow);
        /**
         * @brief    获取标量界面是否显示（返回的是实际的ScalarBarWidget显示状态）
         * @return   bool           是否显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        bool getScalarBarWidgetOnOrOff();
        /**
         * @brief    获取标量界面是否显示（返回的是标签，而不是实际的ScalarBarWidget显示状态）
         * @return   bool           是否显示
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-11
         */
        bool getScalarBarWidgetIsShow();
        /**
         * @brief    设置不透明度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        void setOpacity(double opacity);
        /**
         * @brief    获取不透明度
         * @return   double      不透明度
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-13
         */
        double getOpacity();
    private:
        /**
         * @brief  后处理渲染数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-12
         */
        PostGraphObjectBase* _postGraphObj = nullptr;
        /**
         * @brief  后处理数据
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-09
         */
        FITKAbstractCFDPostData* _postData = nullptr;

        FITKPostDisplayType _displayType = FITKPostDisplayType::Post_Face;

        FITKPostFieldType _fieldType = FITKPostFieldType::Post_None;
        /**
         * @brief  当前场名称
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-10
         */
        QString _fieldName = SolidColor;
        int _componentIndex = -1;
        /**
         * @brief  是否显示标量值界面
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-11
         */
        bool _isShowScalarBar = true;
    };
}

#endif
