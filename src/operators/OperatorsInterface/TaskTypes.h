#pragma once

#include "model/ModelData/DataObject.h"

#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <QVector>

#include <optional>

namespace AppMesh::Operators
{
using TaskId = quint64;
constexpr TaskId InvalidTaskId = 0;

enum class TaskStatus : quint8
{
    Created,
    Executing,
    Succeeded,
    Failed
};

struct OperatorInput
{
    QString operationKey;
    QVariantMap parameters;
    QVector<Model::ObjectId> sourceObjectIds;
    QString description;
};

struct ResultReference
{
    Model::ObjectId objectId = Model::InvalidObjectId;
    QString path;
    QVariant value;
};
}
