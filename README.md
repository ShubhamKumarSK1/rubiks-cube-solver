# High-Performance Bitboard Rubik's Cube Solver & Visualizer

An ultra-optimized Rubik's Cube analytical engine combining a low-latency **C++17 simulation backend** with an asynchronous **Python graphical orchestration pipeline**. 

The architecture models the combinatorial permutation space of a 3x3x3 Rubik's Cube using bitwise parallel registers (**Bitboards**) and computes mathematically optimal (God's Number compliant) paths via **Parallel Iterative Deepening A\* (IDA\*) Search** guided by custom-generated heuristics.

---

## 🏛️ Architecture & System Design

The system decouples intense combinatorial search tasks from the rendering thread via an asynchronous execution bridge:
[ Python Tkinter GUI ] --(Spawns Asynchronous Subprocess)--> [ Compiled C++ Core Engine ]
▲                                                               │
│                                                     (Bitboard Rotations)
(Animates 2D Matrix)                                                   │
│                                                               ▼
└-------(Parses Clean Stdout Output Sequence)------- [ Korf IDA* Search Loop ]
▲
│
(Consults Lookups)
│
[ Pattern Databases ]

### Key Technical Implementations:
* **Bitboard State Representation:** Instead of using standard arrays or structures to represent individual cubies, the cube's configuration is packed into native CPU registers. Face rotations are mathematically evaluated using high-speed bitwise masking and shifting operations.
* **Korf's IDA\* Search Optimization:** Implements Iterative Deepening A\* to provide absolute shortest-path solutions while strictly constraining memory allocations to $O(d)$ spatial complexity, where $d$ is the current search depth threshold.
* **Pattern Database Heuristics:** Generates and memory-maps localized indexing tables for isolated subset configurations (e.g., corner and edge groupings). These databases provide admissible, monotonic distance bounds that structurally prune impossible branches from the search tree.
* **Inter-Process Communication Bridge:** The Python visualization framework communicates with the native binary via standard I/O pipes, securely deserializing optimal moveset tokens into sequential spatial matrix transitions.

---

## 🛠️ Tech Stack & Constraints

* **Core Engine:** C++17 (Optimized via `-O3` pipeline vectorization flags)
* **Visualization Matrix:** Python 3.x, Native Tkinter Framework (Zero external engine dependencies)
* **Algorithmic Focus:** Admissible heuristics, tree pruning, graph traversal optimization.

---