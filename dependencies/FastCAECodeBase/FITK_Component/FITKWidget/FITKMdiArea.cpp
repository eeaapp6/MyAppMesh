/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKMdiArea.h"
#include "FITKMdiSubWindow.h"

#include <QResizeEvent>
#include <QMutexLocker>
#include <QTabBar>
#include <cmath>

namespace Comp
{
    //互斥锁初始化
    QMutex FITKMdiArea::_mutex;

    FITKMdiArea::FITKMdiArea(QWidget * parent) :
        QMdiArea(parent)
    {
        //设置在tab模式下bar中包含关闭按钮
        this->setTabsClosable(true);
        //设置tab页bar为矩形
        this->setTabShape(QTabWidget::TabShape::Rounded);
    }

    FITKMdiArea::~FITKMdiArea()
    {
        _widgetHash.clear();
    }

    int FITKMdiArea::addSubWidget(QWidget* widgwt, QString title)
    {
        //互斥锁添加
        QMutexLocker locker(&_mutex);

        FITKMdiSubWindow* subWin = new FITKMdiSubWindow;
        subWin->setWidget(widgwt);
        subWin->setParent(this);

        this->addSubWindow(subWin);
        _objectID++;
        _widgetHash.insert(_objectID, subWin);
        
        //Tab模式下，激活窗口设置必须在设置布局方式之前
        if (_layoutType == FITKVportsLayoutType::Tab) {
            this->setActiveSubWindow(subWin);
        }
        //初始化界面的布局方式
        setLayoutType(_layoutType);

        //QMdiArea中添加widget后需要调用一下show，不然可能无法显示
        subWin->setWindowTitle(title);
        subWin->show();
        //窗口手动最大化事件处理
        connect(subWin, &FITKMdiSubWindow::sigWinStateMax, this, &FITKMdiArea::slotCurWinStateMax);
        //返回ID
        return _objectID;
    }

    void FITKMdiArea::setLayoutType(FITKVportsLayoutType type)
    {
        _layoutType = type;
        switch (_layoutType){
            //当前视口最大化
        case Comp::FITKVportsLayoutType::CurrentMax:this->setViewMode(QMdiArea::ViewMode::SubWindowView); break;
            //层叠
        case Comp::FITKVportsLayoutType::Cascade:this->setViewMode(QMdiArea::ViewMode::SubWindowView); break;
            //水平平铺
        case Comp::FITKVportsLayoutType::TileHorizontally:this->setViewMode(QMdiArea::ViewMode::SubWindowView); break;
            //垂直平铺
        case Comp::FITKVportsLayoutType::TileVertically:this->setViewMode(QMdiArea::ViewMode::SubWindowView); break;
            //Tab页
        case Comp::FITKVportsLayoutType::Tab: this->setViewMode(QMdiArea::ViewMode::TabbedView); break;
        }

        //更新布局
        updateLayout();
    }

    void FITKMdiArea::setTabPos(FITKVportsTabPos pos)
    {
        //设置tab下toolbar的位置
        if (_layoutType != Comp::FITKVportsLayoutType::Tab)return;
        _tabPos = pos;
        switch (_tabPos) {
            //顶部
        case Comp::FITKVportsTabPos::Top:this->setTabPosition(QTabWidget::TabPosition::North); break;
            //底部
        case Comp::FITKVportsTabPos::Bottom:this->setTabPosition(QTabWidget::TabPosition::South); break;
            //左侧
        case Comp::FITKVportsTabPos::Left:this->setTabPosition(QTabWidget::TabPosition::West); break;
            //右侧
        case Comp::FITKVportsTabPos::Right:this->setTabPosition(QTabWidget::TabPosition::East); break;
        }
    }

    void FITKMdiArea::showTabBar(bool show)
    {
        //不是tab布局不处理
        if (_layoutType != FITKVportsLayoutType::Tab) return;
        //查找全部tab bar，只能查到一个
        QList<QTabBar*> tabBars = this->findChildren<QTabBar*>();
        for (QTabBar* bar : tabBars)
        {
            if (bar) bar->setVisible(show);
        }

    }

    QMdiSubWindow * FITKMdiArea::getSubWidget(int objectID)
    {
        return _widgetHash.value(objectID);
    }

    QWidget* FITKMdiArea::getWidget(int id)
    {
        QMdiSubWindow* subWidget = _widgetHash.value(id);
        if (subWidget == nullptr)return nullptr;
        return subWidget->widget();
    }

    void FITKMdiArea::removeWidget(int id)
    {
        //互斥锁
        QMutexLocker locker(&_mutex);

        QMdiSubWindow* widget = _widgetHash.value(id);
        if (widget == nullptr)return;
        this->removeSubWindow(widget);
        _widgetHash.remove(id);
        
        //更新布局
        updateLayout();
    }

    QWidget* FITKMdiArea::getWidget(QMdiSubWindow* subWidget)
    {
        if (subWidget == nullptr)return nullptr;
        QList<QMdiSubWindow*> subWidgets = _widgetHash.values();
        if(!subWidgets.contains(subWidget))return nullptr;
        return subWidget->widget();
    }

    QMdiSubWindow * FITKMdiArea::getSubWidget(QWidget* widget)
    {
        if (widget == nullptr)return nullptr;
        QList<QMdiSubWindow*> subWidgets = _widgetHash.values();
        for (QMdiSubWindow* subWidget : subWidgets) {
            if (subWidget == nullptr)continue;
            if (subWidget->widget() == widget)return subWidget;
        }
        return nullptr;
    }

    QWidget* FITKMdiArea::getCurrentWidget()
    {
        FITKMdiSubWindow* subMainwin = dynamic_cast<FITKMdiSubWindow*>(this->currentSubWindow());
        if (subMainwin == nullptr)return nullptr;
        return subMainwin->widget();
    }

    QMdiSubWindow * FITKMdiArea::getCurrentSubWidget()
    {
        return this->currentSubWindow();
    }

    void FITKMdiArea::removeCurrentWidget()
    {
        QMutexLocker locker(&_mutex);

        QMdiSubWindow* subMainwin = this->currentSubWindow();
        if (subMainwin == nullptr)return;
        int id = _widgetHash.key(subMainwin);
        removeWidget(id);
    }

    void FITKMdiArea::updateLayout()
    {
        switch (_layoutType){
            //当前视图最大化
        case Comp::FITKVportsLayoutType::CurrentMax:currentMaxViewports(); break;
            //叠层布局
        case Comp::FITKVportsLayoutType::Cascade:cascadeViewports(); break;
            //水平平铺
        case Comp::FITKVportsLayoutType::TileHorizontally:tileHorizontallyViewports(); break;
            //垂直平铺
        case Comp::FITKVportsLayoutType::TileVertically:tileVerticallyViewports(); break;
        }
    }

    void FITKMdiArea::setTabBarExpanding(bool ex /*= true */)
    {
        if (_layoutType != FITKVportsLayoutType::Tab) return;
        QTabBar* bar = this->findChild<QTabBar*>();
        if (bar) bar->setExpanding(ex);
    }

    void FITKMdiArea::resizeEvent(QResizeEvent * event)
    {
        QMdiArea::resizeEvent(event);

        //大小发生变化时重新计算窗口位置
        updateLayout();
    }

    bool FITKMdiArea::eventFilter(QObject * object, QEvent * event)
    {
        QMdiArea::eventFilter(object, event);
        QList<QMdiSubWindow*> subWidgets = _widgetHash.values();
        for (QMdiSubWindow* subWidget : subWidgets) {
            if (object == subWidget) {
                if (event->type() == QEvent::Close) {
                    slotWidgetClose(subWidget);
                }
            }
        }
        return false;
    }

    void FITKMdiArea::slotWidgetClose(QMdiSubWindow* subWidget)
    {
        if (subWidget == nullptr)return;
        QList<QMdiSubWindow*> subWidgets = _widgetHash.values();
        if (!subWidgets.contains(subWidget))return;
        _widgetHash.remove(_widgetHash.key(subWidget));
        this->removeSubWindow(subWidget);
    }

    void FITKMdiArea::slotCurWinStateMax()
    {
        _layoutType = FITKVportsLayoutType::CurrentMax;
    }

    void FITKMdiArea::currentMaxViewports()
    {
        //最大化当前窗口
        QMdiSubWindow* currentSubWin = this->currentSubWindow();
        if (currentSubWin == nullptr)return;
        currentSubWin->showMaximized();
    }

    void FITKMdiArea::cascadeViewports()
    {
        QPoint position(0, 0);
        // 分别设置每个视口大小为总尺寸的75%
        auto viewportWidth = width() * 0.75;
        auto viewportHeight = height() * 0.75;

        QList<QMdiSubWindow*> widgetList = this->subWindowList(QMdiArea::CreationOrder);

        for (QMdiSubWindow* widget : widgetList)
        {
            // 宽度超出最大值，则从左上角从新开始排列
            if (position.x() + viewportWidth > width()) {
                position.setX(0);
                position.setY(0);
            }
            // 高度超出最大值，则从新开始一列
            if (position.y() + viewportHeight > height()) {
                position.setY(0);
            }
            // 分别设置每个视口的位置、大小
            widget->setWindowState(Qt::WindowState::WindowNoState);
            QRect rect(0, 0, viewportWidth, viewportHeight);
            widget->setGeometry(rect);
            widget->move(position);
            position.setX(position.x() + 30);
            position.setY(position.y() + 30);
        }
    }

    void FITKMdiArea::tileHorizontallyViewports()
    {
        QList<QMdiSubWindow*> widgetList = this->subWindowList(QMdiArea::CreationOrder);

        // 最大可用长度和宽度
        int availableWidth = width();
        int availableHeight = height();
        // 计算行数和列数
        auto count = widgetList.size();
        int row = floor(sqrt((double)count));
        if (row == 0) return;
        int column = ceil((double)count / (double)row);
        if (column == 0) return;
        // 计算每个视口的显示宽度和高度
        int viewportWidth = availableWidth / column;
        int viewportHeight = availableHeight / row;
        // 当前行索引与列索引
        int rowIndex = 0, columnIndex = 0;
        // 分别计算每个视口的长度，分别设置显示位置和大小
        QPoint position(0, 0);

        int i = 0;
        for (QMdiSubWindow* widget : widgetList)
        {
            if (widget == nullptr) continue;
            // 计算索引为i的视口的行索引和列索引
            rowIndex = floor(i / column);
            columnIndex = i % column;
            // 设置视口的显示状态
            widget->setWindowState(Qt::WindowState::WindowNoState);
            // 设置视口大小
            QRect rect(0, 0, viewportWidth, viewportHeight);
            widget->setGeometry(rect);
            // 计算视口的位置
            position.setX(columnIndex * viewportWidth);
            position.setY(rowIndex * viewportHeight);
            // 移动到指定位置
            widget->move(position);
            i++;
        }
    }

    void FITKMdiArea::tileVerticallyViewports()
    {
        //已创建的顺序获取列表
        QList<QMdiSubWindow*> widgetList = this->subWindowList(QMdiArea::CreationOrder);

        // 最大可用长度和宽度
        int availableWidth = width();
        int availableHeight = height();
        // 计算行数和列数
        int count = widgetList.size();
        int column = floor(sqrt((double)count));
        if (column == 0) return;
        int row = ceil((double)count / (double)column);
        if (row == 0) return;
        // 计算每个视口的显示宽度和高度
        int viewportWidth = availableWidth / column;
        int viewportHeight = availableHeight / row;
        // 当前行索引与列索引
        int rowIndex = 0, columnIndex = 0;
        // 分别计算每个视口的长度，分别设置显示位置和大小
        QPoint position(0, 0);

        int i = 0;
        for (auto widget : widgetList)
        {
            if (widget == nullptr) continue;
            // 计算索引为i的视口的行索引和列索引
            rowIndex = floor(i / column);
            columnIndex = i % column;
            // 设置视口的显示状态
            widget->setWindowState(Qt::WindowState::WindowNoState);
            // 设置视口大小
            QRect rect(0, 0, viewportWidth, viewportHeight);
            widget->setGeometry(rect);
            // 计算视口的位置
            position.setX(columnIndex * viewportWidth);
            position.setY(rowIndex * viewportHeight);
            // 移动到指定位置
            widget->move(position);
            i++;
        }
    }
}
