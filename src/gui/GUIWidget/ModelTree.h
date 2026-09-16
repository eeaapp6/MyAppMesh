#pragma once

#include "common/AppDiagnostic.h"
#include "model/ModelData/DataObject.h"

#include <QAbstractItemModel>
#include <QTreeView>

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <vector>

class QItemSelection;

namespace AppMesh::Model
{
class ApplicationRuntime;
class GeometryManager;
class MeshManager;
}

namespace AppMesh::Gui
{
class ModelTreeModel final : public QAbstractItemModel
{
public:
    enum DataRole
    {
        ObjectIdRole = Qt::UserRole + 1,
        ObjectNameRole,
        ObjectTypeRole,
        VisibleRole,
        SelectedRole,
        ParentObjectIdRole
    };

    enum class Mutation
    {
        Rename,
        SetVisible,
        SetSelected
    };

    using MutationHandler =
        std::function<std::optional<Model::DataObjectSnapshot>(
            Model::ObjectId,
            Mutation,
            const QVariant&)>;

    explicit ModelTreeModel(QObject* parent = nullptr);
    ~ModelTreeModel() override;

    QModelIndex index(int row,
                      int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

    Common::OperationResult replaceSnapshots(
        const QVector<Model::DataObjectSnapshot>& snapshots);
    void clear();
    QModelIndex indexForObject(Model::ObjectId id, int column = 0) const;
    std::optional<Model::DataObjectSnapshot> snapshotForObject(Model::ObjectId id) const;
    void setMutationHandler(MutationHandler handler);

private:
    struct Node;

    const Node* nodeForIndex(const QModelIndex& index) const;
    Node* nodeForIndex(const QModelIndex& index);
    QModelIndex indexForNode(const Node* node, int column) const;
    static QString typeLabel(Model::DataObjectType type);

    std::vector<std::unique_ptr<Node>> m_roots;
    std::map<Model::ObjectId, Node*> m_nodesById;
    MutationHandler m_mutationHandler;
};

// The bound services are non-owning. Their lifetime must exceed ModelTree's,
// and unbind() must be called before those services are destroyed.
class ModelTree final : public QTreeView
{
public:
    using DiagnosticHandler = std::function<void(const Common::Diagnostic&)>;

    explicit ModelTree(QWidget* parent = nullptr);
    ~ModelTree() override;

    Common::OperationResult bind(Model::ApplicationRuntime* runtime,
                                 Model::GeometryManager* geometryManager,
                                 Model::MeshManager* meshManager);
    void unbind();
    bool isBound() const noexcept;

    Common::OperationResult refreshFromRuntime();
    Common::OperationResult requestRename(Model::ObjectId id, const QString& name);
    Common::OperationResult requestVisible(Model::ObjectId id, bool visible);
    Common::OperationResult requestSelected(Model::ObjectId id, bool selected);
    Common::OperationResult requestParent(Model::ObjectId id, Model::ObjectId parentId);

    ModelTreeModel* objectModel() noexcept;
    const ModelTreeModel* objectModel() const noexcept;
    void setDiagnosticHandler(DiagnosticHandler handler);

protected:
    void selectionChanged(const QItemSelection& selected,
                          const QItemSelection& deselected) override;

private:
    enum class Request
    {
        Rename,
        SetVisible,
        SetSelected,
        SetParent
    };

    Common::OperationResult routeRequest(Model::ObjectId id,
                                         Request request,
                                         const QVariant& value);
    std::optional<Model::DataObjectSnapshot> routeModelMutation(
        Model::ObjectId id,
        ModelTreeModel::Mutation mutation,
        const QVariant& value);
    Common::OperationResult bindingError(Model::ObjectId id,
                                         const QString& code,
                                         const QString& message,
                                         const QString& detail) const;
    void report(const Common::OperationResult& result) const;

    ModelTreeModel* m_model = nullptr;
    Model::ApplicationRuntime* m_runtime = nullptr;
    Model::GeometryManager* m_geometryManager = nullptr;
    Model::MeshManager* m_meshManager = nullptr;
    DiagnosticHandler m_diagnosticHandler;
    bool m_refreshing = false;
};
}
