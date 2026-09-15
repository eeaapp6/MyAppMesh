/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   PostGraphObjectBase.h
 * @brief  后处理渲染对象基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-10
 *********************************************************************/
#ifndef _PostGraphObjectBase_H
#define _PostGraphObjectBase_H

#include "PostGraphAdaptorAPI.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

namespace Interface {
    class FITKAbstractCFDPostData;
}

namespace Comp {
    class FITKGraph3DWindowVTK;
}

class vtkProp;
class vtkActor;

namespace Interface 
{
    class PostGraphProperty;
    /**
     * @brief  后处理渲染对象基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-09-10
     */
    class PostGraphAdaptorAPI PostGraphObjectBase : public Comp::FITKGraphObjectVTK
    {
        Q_OBJECT;
    public:
        /**
         * @brief    构造函数
         * @param[i] postData       后处理数据对象 
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        explicit PostGraphObjectBase(FITKAbstractCFDPostData* postData, Comp::FITKGraph3DWindowVTK* graph3DWidget);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        virtual ~PostGraphObjectBase();
        /**
         * @brief    创建
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-18
         */
        virtual void create();
        /**
         * @brief    获取后处理数据ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-10
         */
        int getPostID();
        /**
         * @brief    获取属性
         * @return   PostGraphProperty*   属性
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        PostGraphProperty* getProperty();
        /**
         * @brief    获取全部的渲染对象
         * @return   QList<vtkActor*>     渲染对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-09-12
         */
        QList<vtkProp*> getActors();
        /**
         * @brief 设置当前数据是否显示
         * @param[i]  isShow         是否显示
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-09-12
         */
        void setIsShow(bool isShow);
        /**
         * @brief    获取所在的渲染层数
         * @return   int       渲染层级
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-22
         */
        virtual int getRenderLayer();
        /**
         * @brief    更新旋转可见性
         * @param[i] isVis                      是否可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-19
         */
        virtual void updateRotateVis(bool isVis);
    protected:
        /**
         * @brief  后处理数据对象
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-09-10
         */
        FITKAbstractCFDPostData* _postData = nullptr;
        /**
         * @brief  后处理渲染属性
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-12-29
         */
        PostGraphProperty* _property = nullptr;
        /**
         * @brief  当前渲染窗口
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-12-29
         */
        Comp::FITKGraph3DWindowVTK* _currentWidget = nullptr;
        /**
         * @brief  特征演员
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-12-29
         */
        vtkActor* _featureActor = nullptr;
    };
}

#endif
