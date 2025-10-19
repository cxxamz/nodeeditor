#include <QApplication>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeDelegateModel>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/NodeDelegateModelRegistry>

struct IntData : public QtNodes::NodeData 
{
    int _value = {};
    std::shared_ptr<QtNodes::StaticDataType> _datatype;
public:
    virtual auto type() const -> QtNodes::NodeDataType override { return _datatype; }
public:
    IntData() : _datatype(std::make_shared<QtNodes::StaticDataType>("int", "int")) {}
};

struct IntNode : public QtNodes::NodeDelegateModel
{
    Q_OBJECT
public:
    std::shared_ptr<IntData> _int_data;
public:
    virtual auto name() const -> QString override { return "int"; }
    virtual auto caption() const -> QString override { return "int"; }
    virtual auto captionVisible() const -> bool override { return true; }
    virtual auto resizable() const -> bool override { return false; }
public:
    virtual auto nPorts(QtNodes::PortType t_) const -> unsigned int override
    {
        return 1;
    }
    virtual auto dataType(QtNodes::PortType t_, QtNodes::PortIndex i_) const -> QtNodes::NodeDataType override
    {
        return _int_data->type();
    }
    virtual auto outData(QtNodes::PortIndex i_) -> std::shared_ptr<QtNodes::NodeData> override
    {
        return _int_data;
    }
    virtual auto setInData(std::shared_ptr<QtNodes::NodeData> d_, QtNodes::PortIndex i_) -> void override
    {
        if (auto d = std::dynamic_pointer_cast<IntData>(d_))
        {
            _int_data->_value = d->_value;
        }
    }
public:
    virtual auto embeddedWidget() -> QWidget* override { return {}; }
public:
    IntNode() : _int_data(std::make_shared<IntData>()) {}
};

struct StrData : public QtNodes::NodeData 
{
    std::string _string = {};
    std::shared_ptr<QtNodes::StaticDataType> _datatype;
public:
    virtual auto type() const -> QtNodes::NodeDataType override { return _datatype; }
public:
    StrData() : _datatype(std::make_shared<QtNodes::StaticDataType>("str", "str")) {}
};

struct StrNode : public QtNodes::NodeDelegateModel
{
    Q_OBJECT
public:
    std::shared_ptr<StrData> _str_data;
public:
    virtual auto name() const -> QString override { return "str"; }
    virtual auto caption() const -> QString override { return "str"; }
    virtual auto captionVisible() const -> bool override { return true; }
    virtual auto resizable() const -> bool override { return false; }
public:
    virtual auto nPorts(QtNodes::PortType t_) const -> unsigned int override
    {
        return 1;
    }
    virtual auto dataType(QtNodes::PortType t_, QtNodes::PortIndex i_) const -> QtNodes::NodeDataType override
    {
        return _str_data->type();
    }
    virtual auto outData(QtNodes::PortIndex i_) -> std::shared_ptr<QtNodes::NodeData> override
    {
        return _str_data;
    }
    virtual auto setInData(std::shared_ptr<QtNodes::NodeData> d_, QtNodes::PortIndex i_) -> void override
    {
        if (auto d = std::dynamic_pointer_cast<StrData>(d_))
        {
            _str_data->_string = d->_string;
        }
    }
public:
    virtual auto embeddedWidget() -> QWidget* override { return {}; }
public:
    StrNode() : _str_data(std::make_shared<StrData>()) {}
};

struct AnyDataType : public QtNodes::DynamicDataType 
{
    std::shared_ptr<QtNodes::DataType> _type;
public:
    virtual auto type() const -> QString override { return _type ? _type->type() : "any"; }
    virtual auto name() const -> QString override { return _type ? _type->name() : "any"; }
    virtual auto null() const -> bool override { return !bool(_type); }
};

struct AnyData : public QtNodes::NodeData 
{
    std::shared_ptr<AnyDataType> _type;
    std::shared_ptr<QtNodes::NodeData> _data = {};
public:
    virtual auto type() const -> QtNodes::NodeDataType override 
    { 
        return _data ? _data->type() : _type; 
    }
public:
    AnyData() : _type(std::make_shared<AnyDataType>()) {}
};

struct AnyNode : public QtNodes::NodeDelegateModel
{
    Q_OBJECT
public:
    int _in = {};
    int _out = {};
    std::shared_ptr<AnyData> _any_data;
public:
    virtual auto name() const -> QString override { return "any"; }
    virtual auto caption() const -> QString override { return "any"; }
    virtual auto captionVisible() const -> bool override { return true; }
    virtual auto resizable() const -> bool override { return false; }
public:
    virtual auto nPorts(QtNodes::PortType t_) const -> unsigned int override
    {
        return 1;
    }
    virtual auto dataType(QtNodes::PortType t_, QtNodes::PortIndex i_) const -> QtNodes::NodeDataType override
    {
        return _any_data->type();
    }
    virtual auto outData(QtNodes::PortIndex i_) -> std::shared_ptr<QtNodes::NodeData> override
    {
        return _any_data->_data;// ? _any_data->_data : _any_data;
    }
    virtual auto setInData(std::shared_ptr<QtNodes::NodeData> d_, QtNodes::PortIndex i_) -> void override
    {
        if (auto d = std::dynamic_pointer_cast<AnyData>(d_)) 
        {
            _any_data->_data = d->_data;
        } 
        else 
        {
            if (d_) _any_data->_data = d_;
            else if (_in == 0 && _out == 0) _any_data->_data = {};
        }
    }
public:
    virtual auto embeddedWidget() -> QWidget* override { return {}; }
// public Q_SLOTS:
public:
    virtual auto inputConnectionCreated(QtNodes::ConnectionId const& conn_) -> void override 
    {
        auto model = reinterpret_cast<QtNodes::DataFlowGraphModel*>(property("model").toULongLong());
        assert(model->delegateModel<AnyNode>(conn_.inNodeId) == this);
        auto node = model->delegateModel<QtNodes::NodeDelegateModel>(conn_.outNodeId); assert(node);
        auto data = node->outData(conn_.outPortIndex); //assert(data);
        ++_in;
    }
    virtual auto inputConnectionDeleted(QtNodes::ConnectionId const& conn_) -> void override 
    {
        auto model = reinterpret_cast<QtNodes::DataFlowGraphModel*>(property("model").toULongLong());
        assert(model->delegateModel<AnyNode>(conn_.inNodeId) == this);
        auto node = model->delegateModel<QtNodes::NodeDelegateModel>(conn_.outNodeId); assert(node);
        auto data = node->outData(conn_.outPortIndex); assert(data);
        --_in;
    }
    virtual auto outputConnectionCreated(QtNodes::ConnectionId const& conn_) -> void override 
    {
        auto model = reinterpret_cast<QtNodes::DataFlowGraphModel*>(property("model").toULongLong());
        assert(model->delegateModel<AnyNode>(conn_.outNodeId) == this);
        auto node = model->delegateModel<QtNodes::NodeDelegateModel>(conn_.inNodeId); assert(node);
        auto dynn = dynamic_cast<AnyNode*>(node) != nullptr;
        auto data = node->outData(conn_.inPortIndex); //assert(data);
        if (!(_any_data->_data && dynn)) _any_data->_data = data;
        ++_out;
    }
    virtual auto outputConnectionDeleted(QtNodes::ConnectionId const& conn_) -> void override 
    {
        auto model = reinterpret_cast<QtNodes::DataFlowGraphModel*>(property("model").toULongLong());
        assert(model->delegateModel<AnyNode>(conn_.outNodeId) == this);
        auto node = model->delegateModel<QtNodes::NodeDelegateModel>(conn_.inNodeId); assert(node);
        auto data = node->outData(conn_.inPortIndex); assert(data);
        --_out;
        if (_out == 0 && _in == 0) _any_data->_data = {};
    }
public:
    AnyNode() : _any_data(std::make_shared<AnyData>()) {}
};

auto main(int argc, char** argv) -> int
{
    auto app = std::make_unique<QApplication>(argc, argv);

    auto registry = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    {
        registry->registerModel<IntNode>("test");
        registry->registerModel<StrNode>("test");
        registry->registerModel<AnyNode>("test");
    }
    auto dataFlowGraphModel = QtNodes::DataFlowGraphModel(registry);
    auto dataFlowGraphicsScene = QtNodes::DataFlowGraphicsScene(dataFlowGraphModel);

    dataFlowGraphModel.connect
    (
        &dataFlowGraphModel, 
        &QtNodes::DataFlowGraphModel::nodeCreated,
        [&](QtNodes::NodeId id) -> void
        {
            if (auto model = dataFlowGraphModel.delegateModel<QtNodes::NodeDelegateModel>(id))
            {
                auto address = reinterpret_cast<std::uint64_t>(&dataFlowGraphModel);
                model->setProperty("model", address);
            }
        }
    );

    auto graphicsView = QtNodes::GraphicsView();
    graphicsView.setScene(&dataFlowGraphicsScene);

    graphicsView.show();

    return app->exec();
}

#include "main.moc"
