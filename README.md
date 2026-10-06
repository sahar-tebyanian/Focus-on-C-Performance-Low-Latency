# Focus-on-C-Performance-Low-Latency
High-performance, low-latency market data and ML feature engineering pipeline built with C++23 and Python (Polars/PyTorch). Designed for sub-microsecond feature calculation and real-time interface
# Hybrid C++23 / Python Low-Latency ML Pipeline

A high-performance market data analytics and feature extraction pipeline designed for sub-microsecond latency and real-time machine learning inference. Built using **C++23** for performance-critical execution and **Python (Polars / PyTorch)** for data engineering and research workflows.

![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![Python](https://img.shields.io/badge/Python-3.11+-green.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

---

## Key Features

* **Zero-Allocation Hot Path:** C++23 order book engine using `std::span` and contiguous memory buffers to guarantee sub-microsecond tick processing with zero heap allocations.
* **Lock-Free Thread Safety:** Atomic memory order operations (`std::memory_order_relaxed`) for high-throughput reads/writes across strategy execution threads.
* **High-Throughput Data Pipeline:** Python data engine leveraging **Polars** for vectorized feature generation and strategy backtesting.
* **Modern Tooling & Practices:** Clean Linux/CMake setup, GoogleTest unit testing, Docker containerization, and GitHub Actions CI/CD workflows.

---

## Architecture Overview
