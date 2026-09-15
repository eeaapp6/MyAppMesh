/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file GraphArea.h
 * @brief 绘图区域声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-10-15
 * 
 */
#ifndef __FITK_GRAPHH_AREA_H___
#define __FITK_GRAPHH_AREA_H___

#include "GUIWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

namespace Ui
{
    class GraphArea;
}
namespace Comp
{
    class FITKGraph3DWindowVTK;
    class FITKGraphObjectVTK;
}

namespace Core
{
    class FITKAbstractGraphObject;
}

namespace GUI
{
    /**
      * @brief 绘图窗口类型
      * @author LiBaojun (libaojunqd@foxmail.com)
      * @date 2024-12-24
      */
    enum  GraphAreaType
    {
        GATNone,    ///< 空类型
        GATPre,     ///< 前处理
        GATPost,    ///< 后处理
    };

     /**
      * @brief 绘图区域声明
      * @author LiBaojun (libaojunqd@foxmail.com)
      * @date 2024-10-15
      */
    class GUIWIDGETAPI GraphArea : public Core::FITKWidget
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new Graph Area object
         * @param[i]  parent         父对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-15
         */
        explicit GraphArea(QWidget* parent = nullptr);
        /**
         * @brief Destroy the Graph Area object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-15
         */
        virtual ~GraphArea() =0;
        /**
         * @brief 获取窗口类型
         * @return 绘图窗口类型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-24
         */
        virtual GraphAreaType getGraphAreaType() = 0;
        /**
         * @brief 获取窗口指针
         * @return Comp::FITKGraph3DWindowVTK* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-24
         */
        Comp::FITKGraph3DWindowVTK* getRenderWindow();
        /**
         * @brief 刷新渲染
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-12-24
         */
        void reRender();

       /**
       * @brief 添加渲染对象
       * @param[i]  index          渲染图层索引值
       * @param[i]  object         渲染对象
       * @param[i]  fitview        是否自适应(仅在窗口为空时生效)
       * @author BaGuijun (baguijun@163.com)
       * @date 2024-03-18
       */
        void addObject(int index, Comp::FITKGraphObjectVTK* object, bool fitview = true);
        /**
         * @brief 清空所有显示对象（不删除对象）
         * @author BaGuijun (baguijun@163.com)
         * @date 2024-03-28
         */
        void clear();
        /**
        * @brief 自适应对象大小
        * @author BaGuijun (baguijun@163.com)
        * @date 2024-03-14
        */
        void fitView();
        /**
         * @brief 强制刷新窗口
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-17
         */
        void flush();
        /**
         * @brief 从渲染窗口移除渲染对象
         * @param[i]  FITKAbstractGraphObject    移除的渲染对象
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        void removeGraphObj(Core::FITKAbstractGraphObject* gobj);

        /**
        * @brief 保存当前渲染窗口为图片
        * @param[i]  image          图片路径
        * @param[i]  w              图片宽度
        * @param[i]  h              图片高度
        * @author BaGuijun (baguijun@163.com)
        * @date 2024-05-28
        */
        void saveImage(const QString& image, int w = -1, int h = -1);

    protected:
        /**
        * @brief 渲染窗口
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-10-15
        */
        Comp::FITKGraph3DWindowVTK* _renderWindow{};
 
    private:
        /**
         * @brief 界面对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-10-15
         */
        Ui::GraphArea* _ui{};
       
    
    };
}


#endif
