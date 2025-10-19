#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "Export.hpp"

namespace QtNodes {

/**
 * `id` represents an internal unique data type for the given port.
 * `name` is a normal text description.
 */
struct NODE_EDITOR_PUBLIC DataType
{
    virtual auto type() const -> QString { return {}; }
    virtual auto name() const -> QString { return {}; }
    virtual auto same(std::shared_ptr<DataType const> const& _) const -> bool
    {
        assert(_); return type() == _->type();
    }
public:
    virtual ~DataType() = default;
};

struct NODE_EDITOR_PUBLIC StaticDataType : public DataType 
{
    QString _type;
    QString _name;
public:
    virtual auto type() const -> QString override { return _type; }
    virtual auto name() const -> QString override { return _name; }
public:
    StaticDataType(QString const& type_, QString const& name_) : _type(type_), _name(name_) {}
}; 

using NodeDataType = std::shared_ptr<DataType>;

/**
 * Class represents data transferred between nodes.
 * @param type is used for comparing the types
 * The actual data is stored in subtypes
 */
class NODE_EDITOR_PUBLIC NodeData
{
public:
    virtual ~NodeData() = default;

    // virtual bool sameType(NodeData const &nodeData) const
    // {
    //     return (this->type().id == nodeData.type().id);
    // }

    /// Type for inner use
    virtual NodeDataType type() const = 0;
};

} // namespace QtNodes
Q_DECLARE_METATYPE(QtNodes::NodeDataType)
Q_DECLARE_METATYPE(std::shared_ptr<QtNodes::NodeData>)
