#pragma once

#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class DecimalData : public NodeData
{
public:
    DecimalData()
        : _number(0.0)
        , _datatype(std::make_shared<QtNodes::StaticDataType>("decimal", "Decimal"))
    {}

    DecimalData(double const number)
        : _number(number)
        , _datatype(std::make_shared<QtNodes::StaticDataType>("decimal", "Decimal"))
    {}

    NodeDataType type() const override { return _datatype; }

    double number() const { return _number; }

    QString numberAsText() const { return QString::number(_number, 'f'); }

private:
    double _number;
    NodeDataType _datatype;
};
