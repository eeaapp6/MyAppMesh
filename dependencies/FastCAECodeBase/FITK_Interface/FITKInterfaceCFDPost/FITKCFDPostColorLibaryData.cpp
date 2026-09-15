/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKCFDPostColorLibaryData.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QPropertyAnimation>
#include <QtGlobal>

#include <iostream>
#include <vector>
#include <QTextStream>

namespace Interface
{
    FITKCFDPostColorLibaryData::FITKCFDPostColorLibaryData()
    {

    }

    FITKCFDPostColorLibaryData::~FITKCFDPostColorLibaryData()
    {

    }

    void FITKCFDPostColorLibaryData::clear()
    {
        _colorPoints.clear();
    }

    QMap<double, FITKPostColorStr> FITKCFDPostColorLibaryData::getColorPoints()
    {
        return _colorPoints;
    }

    void FITKCFDPostColorLibaryData::setColorPoints(QMap<double, FITKPostColorStr> colorPoints)
    {
        _colorPoints.clear();
        _colorPoints = colorPoints;
    }

    void FITKCFDPostColorLibaryData::insertColorPoints(double pos, FITKPostColorStr color)
    {
        _colorPoints.insert(pos, color);
    }

    QColor FITKCFDPostColorLibaryData::getColor(double pos)
    {
        if (_colorPoints.keys().contains(pos))
        {
            QColor color = QColor::fromRgbF(_colorPoints.value(pos)._R, _colorPoints.value(pos)._G, _colorPoints.value(pos)._B);
            return color;
        }
        // else, emulate a linear gradient
        QPropertyAnimation interpolator;
        const qreal granularite = 255;
        interpolator.setEasingCurve(QEasingCurve::Linear);
        interpolator.setDuration(granularite);
        for(double key : _colorPoints.keys())
        {
            QColor color = QColor::fromRgbF(_colorPoints.value(key)._R, _colorPoints.value(key)._G, _colorPoints.value(key)._B);
            interpolator.setKeyValueAt(key, color);
        }
        interpolator.setCurrentTime(pos*granularite);
        return interpolator.currentValue().value<QColor>();
    }

    bool FITKCFDPostColorLibaryData::readColorJson(QString filePath)
    {
        _colorPoints.clear();
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
        QByteArray jsonData = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

        // 检查 JSON 文档是否为数组
        if (!jsonDoc.isArray()) return false;

        // 获取 JSON 数组
        QJsonArray jsonArray = jsonDoc.array();

        QString name = "";
        QList<double> RGBPoints = {};

        // 遍历数组中的每个对象
        for (const QJsonValue& value : jsonArray) {
            if (value.isObject()) {
                QJsonObject jsonObject = value.toObject();
                name = jsonObject.value("Name").toString();
                
                QJsonArray rgbPointsArray = jsonObject.value("RGBPoints").toArray();
                for (const QJsonValue& rgbValue : rgbPointsArray) {
                    RGBPoints.append(rgbValue.toDouble());
                }
            }
        }

        this->setDataObjectName(name);

        int row = RGBPoints.size() / 4;
        for (int i = 0; i < row; i++) {
            double pointPos = RGBPoints[i * 4];
            FITKPostColorStr color;
            color._R = RGBPoints[i * 4 + 1];
            color._G = RGBPoints[i * 4 + 2];
            color._B = RGBPoints[i * 4 + 3];
            _colorPoints.insert(pointPos, color);
        }

        return true;
    }

    bool FITKCFDPostColorLibaryData::writeColorJson(QString filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            return false;
        }
        QJsonArray jsonArray;
        QJsonObject jsonObj;
        jsonObj.insert("Name", this->getDataObjectName());

        QJsonArray pointArray;
        for (double pos : _colorPoints.keys()) {
            pointArray.append(pos);
            pointArray.append(_colorPoints.value(pos)._R);
            pointArray.append(_colorPoints.value(pos)._G);
            pointArray.append(_colorPoints.value(pos)._B);
        }
        jsonObj.insert("RGBPoints", pointArray);
        jsonArray.append(jsonObj);
        QJsonDocument doc(jsonArray);
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        // 写入文件
        stream << doc.toJson();
        file.close();

        return true;
    }

    bool FITKCFDPostColorLibaryData::copy(FITKCFDPostColorLibaryData * colorLibData)
    {
        if (colorLibData == nullptr)return false;
        QMap<double, FITKPostColorStr> colorPoints = colorLibData->getColorPoints();
        _colorPoints = colorPoints;
        return true;
    }
}

