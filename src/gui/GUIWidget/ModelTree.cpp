#include "ModelTree.h"

#include "model/ModelData/ApplicationRuntime.h"
#include "model/ModelData/GeometryManager.h"
#include "model/ModelData/MeshManager.h"

#include <QItemSelection>
#include <QSet>

#include <algorithm>
#include <utility>

namespace AppMesh::Gui
{
namespace
{
Common::Diagnostic treeDiagnostic(const QString& code,
                                  const QString& message,
                                  const QString& detail,
                                  Model::ObjectId id = Model::InvalidObjectId,
                                  const QString& path = QString())
{
    Common::Diagnostic diagnostic;
    diagnostic.category = QStringLiteral("validation");
    diagnostic.code = code;
    diagnostic.message = message;
    diagnostic.detail = detail;
    diagnostic.path = path;
    diagnostic.stage = QStringLiteral("gui.model-tree");
    if (id != Model::InvalidObjectId)
    {
        diagnostic.objectId = QString::number(id);
    }
    return diagnostic;
}
}

struct ModelTreeModel::Node
{
    Model::DataObjectSnapshot value;
    Node* parent = nullptr;
    int row = 0;
    std::vector<std::unique_ptr<Node>> children;
};

ModelTreeModel::ModelTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
}

ModelTreeModel::~ModelTreeModel() = default;

const ModelTreeModel::Node* ModelTreeModel::nodeForIndex(const QModelIndex& index) const
{
    return index.isValid() ? static_cast<const Node*>(index.internalPointer()) : nullptr;
}

ModelTreeModel::Node* ModelTreeModel::nodeForIndex(const QModelIndex& index)
{
    return index.isValid() ? static_cast<Node*>(index.internalPointer()) : nullptr;
}

QModelIndex ModelTreeModel::index(int row, int column, const QModelIndex& parentIndex) const
{
    if (row < 0 || column < 0 || column >= columnCount(parentIndex))
    {
        return {};
    }

    const auto* parentNode = nodeForIndex(parentIndex);
    const auto& children = parentNode ? parentNode->children : m_roots;
    if (row >= static_cast<int>(children.size()))
    {
        return {};
    }
    return createIndex(row, column, children[static_cast<std::size_t>(row)].get());
}

QModelIndex ModelTreeModel::parent(const QModelIndex& child) const
{
    const auto* childNode = nodeForIndex(child);
    if (!childNode || !childNode->parent)
    {
        return {};
    }
    return createIndex(childNode->parent->row, 0, childNode->parent);
}

int ModelTreeModel::rowCount(const QModelIndex& parentIndex) const
{
    if (parentIndex.column() > 0)
    {
        return 0;
    }
    const auto* parentNode = nodeForIndex(parentIndex);
    return static_cast<int>(parentNode ? parentNode->children.size() : m_roots.size());
}

int ModelTreeModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QString ModelTreeModel::typeLabel(Model::DataObjectType type)
{
    switch (type)
    {
    case Model::DataObjectType::Generic:
        return QStringLiteral("Group");
    case Model::DataObjectType::Geometry:
        return QStringLiteral("Geometry");
    case Model::DataObjectType::Mesh:
        return QStringLiteral("Mesh");
    case Model::DataObjectType::Invalid:
        break;
    }
    return QStringLiteral("Invalid");
}

QVariant ModelTreeModel::data(const QModelIndex& index, int role) const
{
    const auto* node = nodeForIndex(index);
    if (!node)
    {
        return {};
    }
    const auto& value = node->value;
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return index.column() == 0 ? QVariant(value.name) : QVariant(typeLabel(value.type));
    case Qt::CheckStateRole:
        return index.column() == 0
            ? QVariant(value.visible ? Qt::Checked : Qt::Unchecked)
            : QVariant();
    case ObjectIdRole:
        return QVariant::fromValue<qulonglong>(value.id);
    case ObjectNameRole:
        return value.name;
    case ObjectTypeRole:
        return static_cast<int>(value.type);
    case VisibleRole:
        return value.visible;
    case SelectedRole:
        return value.selected;
    case ParentObjectIdRole:
        return QVariant::fromValue<qulonglong>(value.parentId);
    default:
        return {};
    }
}

QVariant ModelTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    {
        return {};
    }
    return section == 0 ? QVariant(QStringLiteral("Object"))
                        : QVariant(QStringLiteral("Type"));
}

Qt::ItemFlags ModelTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    auto result = QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (index.column() == 0)
    {
        result |= Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
    }
    return result;
}

bool ModelTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    auto* node = nodeForIndex(index);
    if (!node || !m_mutationHandler || index.column() != 0)
    {
        return false;
    }

    Mutation mutation;
    QVariant requestValue;
    if (role == Qt::EditRole)
    {
        mutation = Mutation::Rename;
        requestValue = value.toString();
    }
    else if (role == Qt::CheckStateRole)
    {
        mutation = Mutation::SetVisible;
        requestValue = value.toInt() == Qt::Checked;
    }
    else if (role == SelectedRole)
    {
        mutation = Mutation::SetSelected;
        requestValue = value.toBool();
    }
    else
    {
        return false;
    }

    const auto authoritative = m_mutationHandler(node->value.id, mutation, requestValue);
    if (!authoritative.has_value())
    {
        return false;
    }
    node->value = *authoritative;
    const auto first = createIndex(node->row, 0, node);
    const auto last = createIndex(node->row, columnCount() - 1, node);
    emit dataChanged(first,
                     last,
                     {Qt::DisplayRole,
                      Qt::EditRole,
                      Qt::CheckStateRole,
                      ObjectNameRole,
                      VisibleRole,
                      SelectedRole,
                      ParentObjectIdRole});
    return true;
}

QHash<int, QByteArray> ModelTreeModel::roleNames() const
{
    auto roles = QAbstractItemModel::roleNames();
    roles.insert(ObjectIdRole, "objectId");
    roles.insert(ObjectNameRole, "objectName");
    roles.insert(ObjectTypeRole, "objectType");
    roles.insert(VisibleRole, "visible");
    roles.insert(SelectedRole, "selected");
    roles.insert(ParentObjectIdRole, "parentObjectId");
    return roles;
}

Common::OperationResult ModelTreeModel::replaceSnapshots(
    const QVector<Model::DataObjectSnapshot>& snapshots)
{
    Common::OperationResult result;
    std::map<Model::ObjectId, Model::DataObjectSnapshot> values;
    std::map<Model::ObjectId, std::vector<Model::ObjectId>> children;
    for (const auto& snapshot : snapshots)
    {
        if (snapshot.id == Model::InvalidObjectId ||
            snapshot.type == Model::DataObjectType::Invalid)
        {
            result.add(treeDiagnostic(QStringLiteral("GUI-MODEL-TREE-SNAPSHOT-INVALID"),
                                      QStringLiteral("The object snapshot is invalid."),
                                      QStringLiteral("Every tree node requires a non-zero ObjectId and a valid type."),
                                      snapshot.id));
            return result;
        }
        if (!values.emplace(snapshot.id, snapshot).second)
        {
            result.add(treeDiagnostic(QStringLiteral("GUI-MODEL-TREE-ID-DUPLICATE"),
                                      QStringLiteral("The object snapshot contains a duplicate ObjectId."),
                                      QStringLiteral("Runtime snapshots must identify each object exactly once."),
                                      snapshot.id));
            return result;
        }
    }

    for (const auto& item : values)
    {
        const auto& value = item.second;
        if (value.parentId != Model::InvalidObjectId && values.find(value.parentId) == values.end())
        {
            result.add(treeDiagnostic(QStringLiteral("GUI-MODEL-TREE-PARENT-MISSING"),
                                      QStringLiteral("An object snapshot references a missing parent."),
                                      QStringLiteral("Refresh requires one self-consistent Runtime snapshot."),
                                      value.id,
                                      QStringLiteral("parentId")));
            return result;
        }
        children[value.parentId].push_back(value.id);
    }

    std::map<Model::ObjectId, int> visitState;
    std::function<bool(Model::ObjectId)> visit = [&](Model::ObjectId id) {
        int& state = visitState[id];
        if (state == 1)
        {
            return false;
        }
        if (state == 2)
        {
            return true;
        }
        state = 1;
        const auto found = children.find(id);
        if (found != children.end())
        {
            for (const auto childId : found->second)
            {
                if (!visit(childId))
                {
                    return false;
                }
            }
        }
        state = 2;
        return true;
    };
    for (const auto rootId : children[Model::InvalidObjectId])
    {
        if (!visit(rootId))
        {
            break;
        }
    }
    if (visitState.size() != values.size())
    {
        result.add(treeDiagnostic(QStringLiteral("GUI-MODEL-TREE-PARENT-CYCLE"),
                                  QStringLiteral("The object snapshot contains a parent cycle."),
                                  QStringLiteral("The last valid model tree was preserved.")));
        return result;
    }

    std::vector<std::unique_ptr<Node>> roots;
    std::map<Model::ObjectId, Node*> nodesById;
    std::function<void(Model::ObjectId, Node*, std::vector<std::unique_ptr<Node>>&)> append;
    append = [&](Model::ObjectId id, Node* parent, std::vector<std::unique_ptr<Node>>& target) {
        auto node = std::make_unique<Node>();
        node->value = values.at(id);
        node->parent = parent;
        node->row = static_cast<int>(target.size());
        auto* nodePointer = node.get();
        nodesById.emplace(id, nodePointer);
        target.push_back(std::move(node));
        const auto found = children.find(id);
        if (found != children.end())
        {
            for (const auto childId : found->second)
            {
                append(childId, nodePointer, nodePointer->children);
            }
        }
    };
    for (const auto rootId : children[Model::InvalidObjectId])
    {
        append(rootId, nullptr, roots);
    }

    beginResetModel();
    m_roots = std::move(roots);
    m_nodesById = std::move(nodesById);
    endResetModel();
    return result;
}

void ModelTreeModel::clear()
{
    beginResetModel();
    m_roots.clear();
    m_nodesById.clear();
    endResetModel();
}

QModelIndex ModelTreeModel::indexForNode(const Node* node, int column) const
{
    return node && column >= 0 && column < columnCount()
        ? createIndex(node->row, column, const_cast<Node*>(node))
        : QModelIndex();
}

QModelIndex ModelTreeModel::indexForObject(Model::ObjectId id, int column) const
{
    const auto found = m_nodesById.find(id);
    return found == m_nodesById.end() ? QModelIndex() : indexForNode(found->second, column);
}

std::optional<Model::DataObjectSnapshot> ModelTreeModel::snapshotForObject(Model::ObjectId id) const
{
    const auto found = m_nodesById.find(id);
    return found == m_nodesById.end()
        ? std::optional<Model::DataObjectSnapshot>{}
        : std::optional<Model::DataObjectSnapshot>{found->second->value};
}

void ModelTreeModel::setMutationHandler(MutationHandler handler)
{
    m_mutationHandler = std::move(handler);
}

ModelTree::ModelTree(QWidget* parent)
    : QTreeView(parent), m_model(new ModelTreeModel(this))
{
    setModel(m_model);
    setObjectName(QStringLiteral("appmeshModelTree"));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
    setUniformRowHeights(true);
    m_model->setMutationHandler(
        [this](Model::ObjectId id, ModelTreeModel::Mutation mutation, const QVariant& value) {
            return routeModelMutation(id, mutation, value);
        });
}

ModelTree::~ModelTree()
{
    unbind();
}

Common::OperationResult ModelTree::bind(Model::ApplicationRuntime* runtime,
                                        Model::GeometryManager* geometryManager,
                                        Model::MeshManager* meshManager)
{
    m_runtime = runtime;
    m_geometryManager = geometryManager;
    m_meshManager = meshManager;
    return refreshFromRuntime();
}

void ModelTree::unbind()
{
    m_runtime = nullptr;
    m_geometryManager = nullptr;
    m_meshManager = nullptr;
    m_refreshing = true;
    m_model->clear();
    m_refreshing = false;
}

bool ModelTree::isBound() const noexcept
{
    return m_runtime != nullptr;
}

Common::OperationResult ModelTree::refreshFromRuntime()
{
    if (!m_runtime)
    {
        auto result = bindingError(Model::InvalidObjectId,
                                   QStringLiteral("GUI-MODEL-TREE-RUNTIME-NOT-BOUND"),
                                   QStringLiteral("The model tree is not bound to ApplicationRuntime."),
                                   QStringLiteral("Bind a Runtime whose lifetime exceeds the widget lifetime."));
        report(result);
        return result;
    }

    m_refreshing = true;
    auto result = m_model->replaceSnapshots(m_runtime->snapshots());
    m_refreshing = false;
    if (result.succeeded())
    {
        expandAll();
        resizeColumnToContents(0);
    }
    else
    {
        report(result);
    }
    return result;
}

Common::OperationResult ModelTree::requestRename(Model::ObjectId id, const QString& name)
{
    auto result = routeRequest(id, Request::Rename, name);
    if (result.succeeded())
    {
        const auto refreshResult = refreshFromRuntime();
        if (!refreshResult.succeeded())
        {
            return refreshResult;
        }
    }
    else
    {
        report(result);
    }
    return result;
}

Common::OperationResult ModelTree::requestVisible(Model::ObjectId id, bool visible)
{
    auto result = routeRequest(id, Request::SetVisible, visible);
    if (result.succeeded())
    {
        const auto refreshResult = refreshFromRuntime();
        if (!refreshResult.succeeded())
        {
            return refreshResult;
        }
    }
    else
    {
        report(result);
    }
    return result;
}

Common::OperationResult ModelTree::requestSelected(Model::ObjectId id, bool selected)
{
    auto result = routeRequest(id, Request::SetSelected, selected);
    if (result.succeeded())
    {
        const auto refreshResult = refreshFromRuntime();
        if (!refreshResult.succeeded())
        {
            return refreshResult;
        }
    }
    else
    {
        report(result);
    }
    return result;
}

Common::OperationResult ModelTree::requestParent(Model::ObjectId id, Model::ObjectId parentId)
{
    auto result = routeRequest(id,
                               Request::SetParent,
                               QVariant::fromValue<qulonglong>(parentId));
    if (result.succeeded())
    {
        const auto refreshResult = refreshFromRuntime();
        if (!refreshResult.succeeded())
        {
            return refreshResult;
        }
    }
    else
    {
        report(result);
    }
    return result;
}

ModelTreeModel* ModelTree::objectModel() noexcept
{
    return m_model;
}

const ModelTreeModel* ModelTree::objectModel() const noexcept
{
    return m_model;
}

void ModelTree::setDiagnosticHandler(DiagnosticHandler handler)
{
    m_diagnosticHandler = std::move(handler);
}

void ModelTree::selectionChanged(const QItemSelection& selected,
                                 const QItemSelection& deselected)
{
    QTreeView::selectionChanged(selected, deselected);
    if (m_refreshing || !m_runtime)
    {
        return;
    }

    QSet<Model::ObjectId> selectedIds;
    for (const auto& index : selected.indexes())
    {
        if (index.column() == 0)
        {
            selectedIds.insert(index.data(ModelTreeModel::ObjectIdRole).toULongLong());
        }
    }
    QSet<Model::ObjectId> deselectedIds;
    for (const auto& index : deselected.indexes())
    {
        if (index.column() == 0)
        {
            deselectedIds.insert(index.data(ModelTreeModel::ObjectIdRole).toULongLong());
        }
    }
    for (const auto id : deselectedIds)
    {
        if (!selectedIds.contains(id))
        {
            const auto index = m_model->indexForObject(id);
            m_model->setData(index, false, ModelTreeModel::SelectedRole);
        }
    }
    for (const auto id : selectedIds)
    {
        const auto index = m_model->indexForObject(id);
        m_model->setData(index, true, ModelTreeModel::SelectedRole);
    }
}

Common::OperationResult ModelTree::routeRequest(Model::ObjectId id,
                                                Request request,
                                                const QVariant& value)
{
    if (!m_runtime)
    {
        return bindingError(id,
                            QStringLiteral("GUI-MODEL-TREE-RUNTIME-NOT-BOUND"),
                            QStringLiteral("The model tree is not bound to ApplicationRuntime."),
                            QStringLiteral("The request was not applied."));
    }
    const auto snapshot = m_runtime->findById(id);
    if (!snapshot.has_value())
    {
        return bindingError(id,
                            QStringLiteral("GUI-MODEL-TREE-OBJECT-NOT-FOUND"),
                            QStringLiteral("The requested object no longer exists."),
                            QStringLiteral("Refresh the model tree before retrying."));
    }

    const auto genericRequest = [&]() {
        switch (request)
        {
        case Request::Rename:
            return m_runtime->renameObject(id, value.toString());
        case Request::SetVisible:
            return m_runtime->setVisible(id, value.toBool());
        case Request::SetSelected:
            return m_runtime->setSelected(id, value.toBool());
        case Request::SetParent:
            return m_runtime->setParent(id, value.toULongLong());
        }
        return Common::OperationResult{};
    };

    if (snapshot->type == Model::DataObjectType::Generic)
    {
        return genericRequest();
    }
    if (snapshot->type == Model::DataObjectType::Geometry)
    {
        if (!m_geometryManager)
        {
            return bindingError(id,
                                QStringLiteral("GUI-MODEL-TREE-GEOMETRY-MANAGER-NOT-BOUND"),
                                QStringLiteral("GeometryManager is not bound."),
                                QStringLiteral("Geometry requests cannot bypass their lifecycle manager."));
        }
        switch (request)
        {
        case Request::Rename:
            return m_geometryManager->renameGeometry(id, value.toString());
        case Request::SetVisible:
            return m_geometryManager->setVisible(id, value.toBool());
        case Request::SetSelected:
            return m_geometryManager->setSelected(id, value.toBool());
        case Request::SetParent:
            return m_geometryManager->setParent(id, value.toULongLong());
        }
    }
    if (snapshot->type == Model::DataObjectType::Mesh)
    {
        if (!m_meshManager)
        {
            return bindingError(id,
                                QStringLiteral("GUI-MODEL-TREE-MESH-MANAGER-NOT-BOUND"),
                                QStringLiteral("MeshManager is not bound."),
                                QStringLiteral("Mesh requests cannot bypass their lifecycle manager."));
        }
        switch (request)
        {
        case Request::Rename:
            return m_meshManager->renameMesh(id, value.toString());
        case Request::SetVisible:
            return m_meshManager->setVisible(id, value.toBool());
        case Request::SetSelected:
            return m_meshManager->setSelected(id, value.toBool());
        case Request::SetParent:
            return m_meshManager->setParent(id, value.toULongLong());
        }
    }
    return bindingError(id,
                        QStringLiteral("GUI-MODEL-TREE-OBJECT-TYPE-INVALID"),
                        QStringLiteral("The object type cannot be edited."),
                        QStringLiteral("Only Generic, Geometry and Mesh objects are supported."));
}

std::optional<Model::DataObjectSnapshot> ModelTree::routeModelMutation(
    Model::ObjectId id,
    ModelTreeModel::Mutation mutation,
    const QVariant& value)
{
    Request request = Request::Rename;
    switch (mutation)
    {
    case ModelTreeModel::Mutation::Rename:
        request = Request::Rename;
        break;
    case ModelTreeModel::Mutation::SetVisible:
        request = Request::SetVisible;
        break;
    case ModelTreeModel::Mutation::SetSelected:
        request = Request::SetSelected;
        break;
    }
    const auto result = routeRequest(id, request, value);
    if (!result.succeeded())
    {
        report(result);
        return std::nullopt;
    }
    return m_runtime ? m_runtime->findById(id) : std::optional<Model::DataObjectSnapshot>{};
}

Common::OperationResult ModelTree::bindingError(Model::ObjectId id,
                                                const QString& code,
                                                const QString& message,
                                                const QString& detail) const
{
    Common::OperationResult result;
    result.add(treeDiagnostic(code, message, detail, id));
    return result;
}

void ModelTree::report(const Common::OperationResult& result) const
{
    if (!m_diagnosticHandler)
    {
        return;
    }
    for (const auto& diagnostic : result.diagnostics)
    {
        m_diagnosticHandler(diagnostic);
    }
}
}
