/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKTableView.h"
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QMouseEvent>

namespace Comp {
// 构造函数
FITKTableView::FITKTableView(QWidget *parent) : QTableView(parent) {}
// 析构函数
FITKTableView::~FITKTableView() {}
// 鼠标按下事件
void FITKTableView::mousePressEvent(QMouseEvent *event) {
  QTableView::mousePressEvent(event);
  // 只判断左键
  if (event->button() == Qt::LeftButton) {
    int xPos = event->x();
    int yPos = event->y();
    // 判断鼠标按下位置的index是否合法
    auto index = indexAt(QPoint(xPos, yPos));
    if (!index.isValid()) {
      // 触发信号（原信号只在index合法的情况下触发）
      emit clicked(index);
    }
  }
}
void FITKTableView::showEvent(QShowEvent *event) {
  QTableView::showEvent(event);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  horizontalHeader()->resizeSections(QHeaderView::Stretch);
  horizontalHeader()->setStretchLastSection(true);
}
} // namespace Comp
