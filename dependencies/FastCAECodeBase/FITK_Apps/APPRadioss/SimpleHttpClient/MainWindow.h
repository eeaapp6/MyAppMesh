/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef SIMPLE_HTTP_CLIENT_MAINWINDOW_H
#define SIMPLE_HTTP_CLIENT_MAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class QByteArray;
class QLabel;
class QLineEdit;
class QPushButton;
class QPlainTextEdit;
class QString;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onSendClicked();

private:
    void onRequestFinished(int taskState,
                           int taskError,
                           int statusCode,
                           const QString &reason,
                           const QByteArray &responseBody);
    void setUiEnabled(bool enabled);

    QLineEdit *m_urlEdit { nullptr };
    QLineEdit *m_uriEdit { nullptr };
    QComboBox *m_methodCombo { nullptr };
    QPlainTextEdit *m_requestHeadersEdit { nullptr };
    QPlainTextEdit *m_requestBodyEdit { nullptr };
    QPlainTextEdit *m_responseEdit { nullptr };
    QLabel *m_statusLabel { nullptr };
    QPushButton *m_sendButton { nullptr };
};

#endif
