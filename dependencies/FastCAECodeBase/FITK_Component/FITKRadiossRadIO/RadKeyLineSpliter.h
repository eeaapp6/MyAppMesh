/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   RadKeyLineSpliter.h
 * @brief  RAD关键字分割，处理所有支持的关键字
 * @author libaojun
 * @date   2025-07-02
 *********************************************************************/
#ifndef _RADKEYLINESPLITER_H_
#define _RADKEYLINESPLITER_H_

#include <QStringList>
#include <QHash>
#include <QString>

namespace Radioss
{
    class FITKRadiossRadReader;

    /**
     * @brief   RAD关键字分割
     * @author libaojun
     * @date   2025-07-02
     */
    class RadKeyLineSpliter
    {
    public:
        explicit  RadKeyLineSpliter() = default;
        virtual ~RadKeyLineSpliter() = default;
        /**
         * @brief    通过字符串重置
         * @param[i] aline 重置字符串
         * @return   void
         * @author   libaojun
         * @date     2025-07-03
         */
        void setText(const QString& aline);

        /**
         * @brief    获取当前行内容
         * @return   QString 当前行字符串
         * @author   libaojun
         * @date     2025-07-03
         */
        QString getCurrentLine() const;

        /**
         * @brief    获取关键字
         * @return   QString
         * @author   libaojun
         * @date     2025-07-02
         */
        QString getKeys();

        /**
         * @brief    获取关键字数量
         * @return   int 关键字数量
         * @author   libaojun
         * @date     2025-07-03
         */
        int getKeyWordCount() const;

        /**
         * @brief    获取指定索引的关键字
         * @param[i] index 索引值，默认为0
         * @return   const QString& 关键字引用
         * @author   libaojun
         * @date     2025-07-03
         */
        const QString& getKey(int index = 0) const;
        
        /**
         * @brief    获取ID数量
         * @return   int ID数量
         * @author   libaojun
         * @date     2025-07-03
         */
        int getIdCount() const;

        /**
         * @brief    获取指定索引的ID
         * @param[i] index 索引值，默认为0
         * @return   int ID值
         * @author   libaojun
         * @date     2025-07-03
         */
        int getId(int index = 0) const;

    private:
        /**
         * @brief    分割行数据
         * @param[i] line 待分割的行字符串
         * @return   void
         * @author   libaojun
         * @date     2025-07-03
         */
        void splitLine(QString line);

    private:
        /**
         * @brief  当前行数据
         * @author libaojun
         * @date   2025-07-02
         */
        QString _line{};
        /**
         * @brief  关键字 除了ID之外的所有关键字
         * @details 例如：/KEYWORD/KEYWORD2/KEYWORD3
         * @author libaojun
         * @date   2025-07-02
         */
        QString _keys{};
        /**
         * @brief  关键字列表
         * @details 例如：/KEYWORD /KEYWORD2 /KEYWORD3 /ID1 /ID2
         * @author libaojun
         * @date   2025-07-02
         */
        QStringList _keyList{};
        /**
         * @brief  ID列表
         * @details 存储行中所有的ID
         * @author libaojun
         * @date   2025-07-02
         */
        QList<int> _ids{};
    
    };
}
#endif // !_RADKEYLINESPLITER_H_

