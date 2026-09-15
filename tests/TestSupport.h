#pragma once

#include <QDebug>
#include <QString>

class TestSuite
{
public:
    void expect(bool condition, const QString& description)
    {
        if (condition)
        {
            qInfo().noquote() << "PASS:" << description;
            return;
        }

        qCritical().noquote() << "FAIL:" << description;
        ++m_failures;
    }

    int result() const
    {
        return m_failures == 0 ? 0 : 1;
    }

private:
    int m_failures = 0;
};
