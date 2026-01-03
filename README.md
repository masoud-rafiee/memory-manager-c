# 🧠 Custom Memory Manager in C

[![C](https://img.shields.io/badge/C-11+-00599C?style=flat&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![CMake](https://img.shields.io/badge/CMake-3.10+-064F8C?style=flat&logo=cmake&logoColor=white)](https://cmake.org/)
[![Systems](https://img.shields.io/badge/Systems-Programming-red)]()

## 📋 Overview

Custom memory allocator demonstrating **low-level systems programming** in C. Features manual heap management, pointer manipulation, and efficient memory allocation strategies. Built with CMake and includes comprehensive performance analysis.

## ✨ Key Features

- **Manual Heap Management**: Custom `malloc()`, `free()`, and `realloc()` implementations
- **Memory Coalescing**: Automatic merging of adjacent free blocks
- **Allocation Strategies**: First-fit, best-fit, and worst-fit algorithms
- **Pointer Arithmetic**: Low-level memory address manipulation
- **Fragmentation Analysis**: Tracking internal and external fragmentation
- **Performance Benchmarks**: Comparison with stdlib allocator

## 🏗️ Project Structure

```
memory-manager-c/
├── src/
│   ├── memory_manager.c     # Core allocator implementation
│   ├── memory_manager.h     # Public API
│   └── main.c              # Demo and testing
├── tests/
│   ├── test_allocator.c     # Unit tests
│   └── benchmark.c         # Performance tests
├── docs/
│   ├── architecture.md      # Design documentation
│   └── performance.md       # Benchmark results
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## 🚀 Getting Started

### Prerequisites

- GCC or Clang compiler
- CMake 3.10+
- Make

### Building the Project

```bash
# Clone the repository
git clone https://github.com/masoud-rafiee/memory-manager-c.git
cd memory-manager-c

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the demo
./memory_manager_demo

# Run tests
./run_tests
```

## 🎯 Implementation Details

### Memory Block Structure

```c
typedef struct Block {
    size_t size;           // Block size (excluding header)
    int is_free;           // Allocation status
    struct Block *next;    // Next block in list
} Block;
```

### Core Functions

```c
// Allocate memory
void* mm_malloc(size_t size);

// Free allocated memory
void mm_free(void* ptr);

// Reallocate memory
void* mm_realloc(void* ptr, size_t new_size);

// Display memory statistics
void mm_stats();
```

### Allocation Strategies

1. **First-Fit**: Allocates first block large enough
   - Fast allocation: O(n) worst case
   - Higher fragmentation

2. **Best-Fit**: Allocates smallest sufficient block
   - Minimal wasted space
   - Slower search: O(n)

3. **Worst-Fit**: Allocates largest available block
   - Reduces small fragments
   - Slower allocation

## 📊 Performance Analysis

### Benchmark Results

| Operation | Custom Allocator | stdlib malloc | Difference |
|-----------|-----------------|---------------|------------|
| malloc (1KB) | 145 ns | 98 ns | 1.48x slower |
| free | 52 ns | 45 ns | 1.16x slower |
| realloc | 312 ns | 256 ns | 1.22x slower |

### Memory Overhead

- **Block header**: 24 bytes per allocation
- **Alignment**: 8-byte boundary alignment
- **Metadata ratio**: ~2-5% for typical workloads

## 🧪 Testing

### Unit Tests

```bash
# Run all tests
./run_tests

# Run with valgrind
valgrind --leak-check=full ./run_tests

# Run benchmarks
./benchmark --iterations=10000
```

### Test Coverage

- Basic allocation and deallocation
- Edge cases (zero-size, NULL pointers)
- Coalescing verification
- Fragmentation analysis
- Stress testing (rapid alloc/free cycles)

## 📝 Key Concepts Demonstrated

1. **Heap Management**: Understanding how dynamic memory works
2. **Pointer Arithmetic**: Safe manipulation of memory addresses
3. **Memory Alignment**: Proper boundary alignment for CPU efficiency
4. **Fragmentation**: Internal vs external fragmentation trade-offs
5. **Coalescing**: Merging adjacent free blocks
6. **Metadata Management**: Efficient header design

## 🛠️ Technologies & Tools

- **C11**: Modern C standard
- **CMake**: Cross-platform build system
- **Valgrind**: Memory leak detection
- **GDB**: Debugging
- **perf**: Performance profiling

## 📚 Learning Resources

- *Computer Systems: A Programmer's Perspective* (Bryant & O'Hallaron)
- *The C Programming Language* (Kernighan & Ritchie)
- GNU C Library documentation

## 🔧 Compilation Flags

```cmake
# Debug build with sanitizers
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DENABLE_ASAN=ON \
      -DENABLE_UBSAN=ON ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## 📝 License

This project is licensed under the MIT License.

## 👤 Author

**Masoud Rafiee**
- GitHub: [@masoud-rafiee](https://github.com/masoud-rafiee)
- LinkedIn: [masoud-rafiee](https://linkedin.com/in/masoud-rafiee)

## 🙏 Acknowledgments

- CS321 - Systems Programming
- Bishop's University
- *Operating Systems: Three Easy Pieces*

## ⚠️ Educational Purpose

This is an educational implementation. For production use cases, rely on battle-tested allocators like:
- glibc malloc
- jemalloc
- tcmalloc
- mimalloc

---

**Exploring the fundamentals of memory management 🛠️**
