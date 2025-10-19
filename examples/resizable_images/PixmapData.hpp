#pragma once

#include <QtGui/QPixmap>

#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class PixmapData : public NodeData
{
public:
    PixmapData() {}

    PixmapData(QPixmap const &pixmap)
        : _pixmap(pixmap)
        , _datatype(std::make_shared<QtNodes::StaticDataType>("pixmap", "P"))
    {}

    NodeDataType type() const override { return _datatype; }

    QPixmap pixmap() const { return _pixmap; }

private:
    QPixmap _pixmap;
    NodeDataType _datatype;
};
