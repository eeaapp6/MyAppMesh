/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKTableView.h
 * @brief  自定义表格视图类
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-05-20
 */
#ifndef __FITKTABLEVIEW_H__
#define __FITKTABLEVIEW_H__

#include "FITKWidgetAPI.h"
#include <QTableView>

namespace Comp {
/**
 * @brief  自定义表格视图类
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-05-20
 */
class FITKWidgetAPI FITKTableView : public QTableView {
public:
  /**
   * @brief  构造函数
   * @param  parent 父对象
   * @author YanZhiHui (chanyuantiandao@126.com)
   * @data   2024-05-20
   */
  FITKTableView(QWidget *parent = nullptr);
  /**
   * @brief  析构函数
   * @author YanZhiHui (chanyuantiandao@126.com)
   * @data   2024-05-20
   */
  ~FITKTableView();

protected:
  /**
   * @brief  鼠标按下事件
   * @param  event 事件
   * @author YanZhiHui (chanyuantiandao@126.com)
   * @data   2024-05-20
   */
  virtual void mousePressEvent(QMouseEvent *event) override;
  /**
   * @brief  显示事件
   * @param  event 事件
   * @author YanZhiHui (chanyuantiandao@126.com)
   * @data   2024-05-20
   */
  virtual void showEvent(QShowEvent *event) override;
};
} // namespace Comp

#endif // !__FITKTABLEVIEW_H__