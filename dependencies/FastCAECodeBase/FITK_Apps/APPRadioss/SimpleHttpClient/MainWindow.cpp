/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MainWindow.h"

#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMetaObject>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QStringList>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

#include <string>

#include "workflow/WFTaskFactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Simple FastCAE HTTP Client"));

    QWidget *central = new QWidget(this);
    QVBoxLayout *rootLayout = new QVBoxLayout(central);

    QGroupBox *requestBox = new QGroupBox(QStringLiteral("Request"), central);
    QVBoxLayout *requestLayout = new QVBoxLayout(requestBox);

    QFormLayout *formLayout = new QFormLayout();
    m_urlEdit = new QLineEdit(QStringLiteral("http://127.0.0.1:2222/"), requestBox);
    m_uriEdit = new QLineEdit(QStringLiteral("/demo"), requestBox);
    m_uriEdit->setPlaceholderText(QStringLiteral("Request URI, e.g. /api/v1/ping"));
    m_methodCombo = new QComboBox(requestBox);
    m_methodCombo->addItem(QStringLiteral("GET"));
    m_methodCombo->addItem(QStringLiteral("POST"));
    formLayout->addRow(QStringLiteral("URL:"), m_urlEdit);
    formLayout->addRow(QStringLiteral("URI:"), m_uriEdit);
    formLayout->addRow(QStringLiteral("Method:"), m_methodCombo);

    m_requestBodyEdit = new QPlainTextEdit(requestBox);
    m_requestBodyEdit->setPlaceholderText(QStringLiteral("POST body (plain text or JSON)"));

    m_requestHeadersEdit = new QPlainTextEdit(requestBox);
    m_requestHeadersEdit->setPlaceholderText(
        QStringLiteral("Custom headers, one per line:\nHeader-Name: value"));
    m_requestHeadersEdit->setPlainText(
        QStringLiteral("# Header examples:\n"
                       "username: user\n"
                       "password: 1213\n"
                       "Accept: application/json"));

    QHBoxLayout *actionLayout = new QHBoxLayout();
    m_sendButton = new QPushButton(QStringLiteral("Send"), requestBox);
    m_statusLabel = new QLabel(QStringLiteral("Ready"), requestBox);
    actionLayout->addWidget(m_sendButton);
    actionLayout->addWidget(m_statusLabel, 1);

    requestLayout->addLayout(formLayout);
    requestLayout->addWidget(m_requestHeadersEdit);
    requestLayout->addWidget(m_requestBodyEdit);
    requestLayout->addLayout(actionLayout);

    QGroupBox *responseBox = new QGroupBox(QStringLiteral("Response"), central);
    QVBoxLayout *responseLayout = new QVBoxLayout(responseBox);
    m_responseEdit = new QPlainTextEdit(responseBox);
    m_responseEdit->setReadOnly(true);
    responseLayout->addWidget(m_responseEdit);

    rootLayout->addWidget(requestBox);
    rootLayout->addWidget(responseBox, 1);

    setCentralWidget(central);

    connect(m_sendButton, &QPushButton::clicked, this, &MainWindow::onSendClicked);
}

void MainWindow::onSendClicked()
{
    const QUrl url = QUrl::fromUserInput(m_urlEdit->text().trimmed());
    if (!url.isValid() || url.scheme().isEmpty() || url.host().isEmpty())
    {
        m_statusLabel->setText(QStringLiteral("Invalid URL"));
        return;
    }

    m_responseEdit->clear();
    setUiEnabled(false);
    m_statusLabel->setText(QStringLiteral("Sending..."));

    QString method = m_methodCombo->currentText();
    const QByteArray body = m_requestBodyEdit->toPlainText().toUtf8();

    // If user filled body but forgot to switch method, send it as POST.
    if (method == QStringLiteral("GET") && !body.isEmpty())
    {
        method = QStringLiteral("POST");
    }

    QPointer<MainWindow> self(this);
    WFHttpTask *task = WFTaskFactory::create_http_task(
        url.toString().toStdString(),
        3,
        1,
        [self](WFHttpTask *task) {
            int statusCode = 0;
            QString reason;
            QByteArray responseBody;

            if (task->get_state() == WFT_STATE_SUCCESS)
            {
                protocol::HttpResponse *resp = task->get_resp();
                std::string code;
                std::string phrase;
                resp->get_status_code(code);
                resp->get_reason_phrase(phrase);

                if (!code.empty())
                {
                    statusCode = QString::fromStdString(code).toInt();
                }

                reason = QString::fromStdString(phrase);

                const void *bodyData = nullptr;
                size_t bodySize = 0;
                if (resp->get_parsed_body(&bodyData, &bodySize) && bodyData && bodySize > 0)
                {
                    responseBody = QByteArray(static_cast<const char *>(bodyData), static_cast<int>(bodySize));
                }
            }

            if (!self)
            {
                return;
            }

            QMetaObject::invokeMethod(
                self,
                [self, taskState = task->get_state(), taskError = task->get_error(), statusCode, reason, responseBody]() {
                    if (!self)
                    {
                        return;
                    }

                    self->onRequestFinished(taskState, taskError, statusCode, reason, responseBody);
                },
                Qt::QueuedConnection);
        });

    if (!task)
    {
        m_statusLabel->setText(QStringLiteral("Failed to create request task"));
        setUiEnabled(true);
        return;
    }

    protocol::HttpRequest *req = task->get_req();
    req->set_method(method == QStringLiteral("POST") ? "POST" : "GET");

    QString requestUri = m_uriEdit->text().trimmed();
    if (requestUri.isEmpty())
    {
        requestUri = url.path().isEmpty() ? QStringLiteral("/") : url.path();
        if (!url.query().isEmpty())
        {
            requestUri += QStringLiteral("?") + url.query();
        }
    }

    if (!requestUri.startsWith('/'))
    {
        requestUri.prepend('/');
    }

    req->set_request_uri(requestUri.toStdString());

    const QStringList headerLines = m_requestHeadersEdit->toPlainText().split('\n', Qt::SkipEmptyParts);
    for (const QString& rawLine : headerLines)
    {
        const QString line = rawLine.trimmed();
        if (line.isEmpty())
        {
            continue;
        }

        if (line.startsWith('#'))
        {
            continue;
        }

        const int colonIndex = line.indexOf(':');
        if (colonIndex <= 0)
        {
            continue;
        }

        const QString headerName = line.left(colonIndex).trimmed();
        const QString headerValue = line.mid(colonIndex + 1).trimmed();
        if (headerName.isEmpty())
        {
            continue;
        }

        req->set_header_pair(headerName.toStdString(), headerValue.toStdString());
    }

    if (method == QStringLiteral("POST"))
    {
        req->set_header_pair("Content-Type", "application/json; charset=utf-8");
        req->append_output_body(body.constData(), static_cast<size_t>(body.size()));
    }

    task->start();
}

void MainWindow::onRequestFinished(int taskState,
                                   int taskError,
                                   int statusCode,
                                   const QString &reason,
                                   const QByteArray &responseBody)
{
    QString output;
    output += QStringLiteral("Status: ") + (statusCode > 0 ? QString::number(statusCode) : QStringLiteral("N/A"));
    if (!reason.isEmpty())
    {
        output += QStringLiteral(" ") + reason;
    }
    output += QStringLiteral("\n\n");

    if (taskState != WFT_STATE_SUCCESS)
    {
        output += QStringLiteral("Network Error: state=") + QString::number(taskState)
            + QStringLiteral(", error=") + QString::number(taskError) + QStringLiteral("\n\n");
    }

    output += QString::fromUtf8(responseBody);
    m_responseEdit->setPlainText(output);

    m_statusLabel->setText(taskState == WFT_STATE_SUCCESS
        ? QStringLiteral("Done")
        : QStringLiteral("Failed"));

    setUiEnabled(true);
}

void MainWindow::setUiEnabled(bool enabled)
{
    m_urlEdit->setEnabled(enabled);
    m_uriEdit->setEnabled(enabled);
    m_methodCombo->setEnabled(enabled);
    m_requestHeadersEdit->setEnabled(enabled);
    m_requestBodyEdit->setEnabled(enabled);
    m_sendButton->setEnabled(enabled);
}
