#pragma once

enum class Side: std::uint8_t { UNKOWN, BUY, SELL };

enum class OrderParams : std::uint64_t {
    STOP = 1,        // stop limit order
    AON = 1U << 1U,  // all-or-nothing
    IOC = 1U << 2U,  // immediate-or-cancel
    FOK = AON | IOC, // fill-or-kill
    GTC = 1U << 3U,  // good-till-cancelled
    GFD = 1U << 4U,  // good-for-day
    GTD = 1U << 5U,  // good-till-date
    STOP_GTC = STOP | GTC,
    STOP_GFD = STOP | GFD,
    STOP_GTD = STOP | GTD,
};

inline OrderParams operator&(OrderParams lhs, OrderParams rhs) {
    return static_cast<OrderParams>(static_cast<std::uint64_t>(lhs) & static_cast<std::uint64_t>(rhs));
}

enum class OrderType: std::uint8_t  { MARKET, LIMIT };

enum class OrderStatus : std::uint8_t { ACCEPTED, MATCHED, NOT_MATCHED, REJECTED, CANCELLED, MODIFIED, NOT_FOUND};
