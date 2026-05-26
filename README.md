# Sorting Algorithm Benchmark

A C++ implementation and empirical runtime comparison of six classic sorting algorithms, with a Python/Jupyter notebook for visualization.

## Implemented Algorithms

| Algorithm     | Complexity (avg) | Notes                                      |
| ------------- | ---------------- | ------------------------------------------ |
| InsertionSort | O(n²)            | Reference / baseline                       |
| QuickSort     | O(n log n)       | Lomuto partition, last-element pivot       |
| MergeSort     | O(n log n)       | Top-down with auxiliary buffer             |
| HeapSort      | O(n log n)       | In-place, max-heap                         |
| ShellSort     | ~O(n^1.5)        | Hibbard gap sequence (2^k − 1)             |
| CountingSort  | O(n + k)         | Range-aware, stable                        |
| RadixSort     | O(d · n)         | LSD, base-10, uses counting sort per digit |

All algorithms (except insertion sort) are exercised by the benchmark driver. Correctness is verified with Catch unit tests.

## Project Structure

```
.
├── main.cpp              # Entry point: runs unit tests, then the benchmark
├── sorting.cpp / .h      # Algorithm implementations
├── unit_tests.cpp        # Catch test cases
├── catch.h               # Catch v1 single-header test framework
├── benchmark_plot.ipynb  # Reads ./data/*.txt and plots the results
├── data/                 # Benchmark output
│   ├── quicksort.txt
│   ├── mergesort.txt
│   ├── heapsort.txt
│   ├── shellsort_2n.txt
│   ├── counting.txt
│   └── radix.txt
└── laufzeitvergleich.png # Generated runtime comparison plot
```

Each `*.txt` file has two whitespace-separated columns: input size `n` and average runtime in nanoseconds (averaged over 10 runs per `n`).

## Build

Use an optimizing build — debug builds produce misleading benchmark numbers:

```bash
clang++ -O2 -std=c++17 main.cpp sorting.cpp -o main
```

(GNU `g++` works identically. On macOS, `gcc` is an alias for Apple Clang.)

## Run

```bash
mkdir -p data
mv *.txt /data
caffeinate -i ./main
```

The program first runs the Catch unit tests, then waits for an Enter keypress before starting the benchmark. The benchmark sweeps `n` from 1,000 to ~1,000,000 in steps of 10,000 and writes results to `data/*.txt`.

`caffeinate -i` (macOS) prevents idle sleep during the run. A full benchmark takes roughly 5–10 minutes on Apple Silicon at `-O2`.

## Visualize

The Jupyter notebook reads the `data/` directory and produces `laufzeitvergleich.png`:

```bash
source .venv/bin/activate         # if using a virtualenv
jupyter notebook benchmark_plot.ipynb
```

Required Python packages: `pandas`, `matplotlib`, `numpy`.

## Notes on Methodology

- **Random data range:** `randomizeVector` populates arrays with values in `[0, 1000000)`. A small range (e.g. 0–9) produces many duplicates, which degrades Lomuto quicksort to O(n²) — avoid it.
- **Sleep / thermal throttling:** results are sensitive to background CPU activity. Use `caffeinate -i` and avoid running other heavy processes during measurement.
- **Compiler flags:** `-O2` is recommended for fair comparison. `-O3` may auto-vectorize some loops asymmetrically, which can bias the comparison.
- **Timing:** `std::chrono::steady_clock` with nanosecond resolution; each data point is the mean of 10 fresh random inputs of the same size.
