#include <iostream>
#include <span >
#include <numeric>
#include <array>
#include <atomic>
#include <chrono>

struct Order {
    double price;
    double volume;
};

class OrderBookTracker {
public:
    static constexpr size_t MAX_DEPTH = 10;

    // Hot path tick processing - zero heap allocation, contiguous memory iteration
    void on_tick(std::span<const Order> bids, std::span<const Order> asks) noexcept {
        // Enforce depth limits without dynamic memory
        const auto effective_bids = bids.subspan(0, std::min(bids.size(), MAX_DEPTH));
        const auto effective_asks = asks.subspan(0, std::min(asks.size(), MAX_DEPTH));

        // High-performance cache-friendly summation via standard algorithms
        const double total_bid_vol = std::accumulate(
            effective_bids.begin(), effective_bids.end(), 0.0,
            [](double acc, const Order& order) noexcept { return acc + order.volume; }
        );

        const double total_ask_vol = std::accumulate(
            effective_asks.begin(), effective_asks.end(), 0.0,
            [](double acc, const Order& order) noexcept { return acc + order.volume; }
        );

        const double denominator = total_bid_vol + total_ask_vol;
        const double imbalance = (denominator > 0.0) 
            ? (total_bid_vol - total_ask_vol) / denominator 
            : 0.0;

        // Lock-free atomic update for multi-threaded strategy access
        current_imbalance_.store(imbalance, std::memory_order_relaxed);
    }

    [[nodiscard]] double get_imbalance() const noexcept {
        return current_imbalance_.load(std::memory_order_relaxed);
    }

private:
    std::atomic<double> current_imbalance_{0.0};
};

int main() {
    OrderBookTracker tracker;

    constexpr std::array<Order, 3> bids = {{{100.5, 10.0}, {100.4, 15.0}, {100.3, 20.0}}}; // Total: 45.0
    constexpr std::array<Order, 3> asks = {{{100.6, 5.0},  {100.7, 10.0}, {100.8, 15.0}}}; // Total: 30.0

    // Measure tick-to-metric latency over 1,000,000 iterations
    constexpr size_t iterations = 1'000'000;
    
    const auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
        tracker.on_tick(bids, asks);
    }
    const auto end = std::chrono::high_resolution_clock::now();

    const auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    const double avg_latency_ns = static_cast<double>(total_ns) / iterations;

    std::cout << "Calculated Imbalance: " << tracker.get_imbalance() << " (Expected: 0.2)\n";
    std::cout << "Average Hot Path Latency: " << avg_latency_ns << " nanoseconds per tick\n";

    return 0;
}