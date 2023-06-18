#include "order.hpp"
#include "limit.hpp"

Order::Order(std::string id,std::string instrument, std::string user_id, float qty,float price, Side side, OrderParams params, OrderType type): _id{id},_instrument{instrument},_user_id{user_id},_timestamp{time(0)},_qty{qty},_current_qty{qty},_price{price}, _side{side}, _params{params}, _type{type} {}
            
std::string& Order::get_id(){
    return _id;
}

float Order::get_price(){
    return _price;
}

void Order::set_price(float price){
    _price = price;
}

bool Order::is_buy(){
    return _side == Side::BUY;
}

void Order::cancel(){
    _cancelled = true;
}

bool Order::is_cancelled(){
    return _cancelled;
}

float Order::get_qty(){
    return _current_qty;
}

bool Order::is_fullfilled(){
    return _current_qty == 0;
}

void Order::set_limit_parent(std::shared_ptr<Limit>& limit){
    _parent = limit;
}

std::shared_ptr<Limit>& Order::get_limit_parent(){
    return _parent;
}

void Order::increase_qty(float delta_qty){
    _current_qty += delta_qty;
}

void Order::decrease_qty(float delta_qty){
    if (delta_qty > _current_qty) {
        throw std::invalid_argument("Modifying quantity must not be bigger than current quantity");
    }

    _current_qty -= delta_qty;
}

void Order::fill(){
    _current_qty = 0;
}

OrderType Order::get_type(){
    return _type;
}

void Order::set_params(OrderParams params){
    _params = params;
}

std::string& Order::get_user_id(){
    return _user_id;
}

bool Order::has_param(OrderParams param) const {
    return static_cast<std::uint64_t>(_params) & static_cast<std::uint64_t>(param);
}


bool Order::operator==(const Order& other){
    return _id == other._id && _user_id == other._user_id && _cancelled == other._cancelled && _side == other._side && _price == other._price && _qty == other._qty && _current_qty == other._current_qty && _instrument == other._instrument && std::difftime(_timestamp ,other._timestamp) == 0.0;
}

bool Order::operator!=(const Order& other){
    return !(*this == other);
}
