# 💹 CryptoPulse
### A Data Structure Based Cryptocurrency Price Tracking and Analysis System

> A terminal-based mini project written in **C**, built as part of a **Data Structures** college course.  
> Every core feature maps directly to a data structure — making it perfect for viva explanations.

---

## 📋 Table of Contents

- [About the Project](#about-the-project)
- [Data Structures Used](#data-structures-used)
- [Features](#features)
- [Getting Started](#getting-started)
- [How to Run](#how-to-run)
- [Menu Overview](#menu-overview)
- [Sample Output](#sample-output)
- [Project Structure](#project-structure)
- [Viva Q&A](#viva-qa)

---

## About the Project

CryptoPulse is a **single-file, console-based** cryptocurrency tracker that demonstrates practical usage of 8 fundamental data structures. The project simulates a real-world crypto dashboard — with a market view, watchlist, price alerts, portfolio management, and more — all inside a clean terminal UI.

**Tech Stack:** Pure C (C99) · No external libraries · Single `.c` file

---

## Data Structures Used

| # | Data Structure | Where It's Used |
|---|---|---|
| 1 | **Array** | Stores the crypto market (coin name, symbol, price, change%) |
| 2 | **Linked List** | Watchlist management & Portfolio holdings |
| 3 | **Stack** | Undo the last watchlist remove operation |
| 4 | **Queue** (Circular) | Price history — stores recent price updates in FIFO order |
| 5 | **Bubble Sort** | Ranks coins by daily % change for Top Gainers / Losers |
| 6 | **Linear Search** | Search a coin by name or symbol |
| 7 | **Tree** (Static) | Displays crypto categories in a visual tree structure |
| 8 | **Graph + BFS** | Market relationships between coins; BFS traversal |

---

## Features

### 1. 📊 View Crypto Market
Displays a formatted table of all coins with live-style data.
```
| Coin         | Symbol | Price($)   | Change%    |
| Bitcoin      | BTC    | 63000.00   | +2.50      |
| Ethereum     | ETH    | 3200.00    | +1.80      |
```

### 2. 🔍 Search Coin
Search by **coin name** or **symbol** using Linear Search. Also increments the coin's search counter for trending.

### 3. ⭐ Watchlist Management
- Add / Remove coins using a **Linked List**
- Undo the last removal with a **Stack** (push on remove, pop on undo)

### 4. 📈 Top Gainers & Losers
Sorts coins by daily % change using **Bubble Sort** and displays the ranked list.

### 5. 🕐 Price History
A **circular queue** stores the 10 most recent price updates (time, symbol, price). Oldest entry is overwritten when full.

### 6. 🔔 Price Alerts
Set a target price for any coin. The system immediately checks if the current price has already met or crossed the target.

### 7. 🔥 Trending Coins
Counter-based tracking shows the **most searched** and **most watched** coins using `searchCount` and `watchCount` fields in the coin array.

### 8. 🌳 Crypto Category Tree
ASCII tree showing the category hierarchy of cryptocurrencies:
```
Cryptocurrency
├── Layer 1 Coins  →  BTC, ETH, SOL
├── Meme Coins     →  DOGE
├── Payment Coins  →  XRP
├── Stablecoins    →  USDT, USDC
└── AI Coins       →  FET, RNDR
```

### 9. 🕸️ Market Graph (BFS)
Coins are nodes in a directed graph. An adjacency matrix stores edges, and **Breadth-First Search** traverses the graph starting from Bitcoin.
```
BTC --> ETH --> SOL --> DOGE --> XRP --> BTC
```

### 10. 💼 Portfolio Management
- **Buy** a coin (stores entry as a Linked List node)
- **Sell** a coin (removes node, calculates profit/loss vs current price)
- **View Portfolio** with per-holding and total P&L

---

## Getting Started

### Prerequisites

You need a C compiler. Any of these work:

- `gcc` (Linux / macOS / WSL)
- `MinGW` or `TDM-GCC` (Windows)
- Any online C compiler (e.g., [onlinegdb.com](https://www.onlinegdb.com/online_c_compiler))

### Clone the Repository

```bash
git clone https://github.com/your-username/cryptopulse.git
cd cryptopulse
```

---

## How to Run

**Linux / macOS / WSL:**
```bash
gcc cryptopulse.c -o cryptopulse
./cryptopulse
```

**Windows (MinGW):**
```bash
gcc cryptopulse.c -o cryptopulse.exe
cryptopulse.exe
```

**With warnings enabled (recommended for learning):**
```bash
gcc -Wall -Wextra cryptopulse.c -o cryptopulse
./cryptopulse
```

> No external libraries needed. Compiles with zero warnings.

---

## Menu Overview

```
============================================
         CRYPTOPULSE DASHBOARD
============================================
   1.  View Market
   2.  Search Coin
   3.  Watchlist Management
   4.  Top Gainers & Losers
   5.  Price History
   6.  Price Alerts
   7.  Trending Coins
   8.  Crypto Categories (Tree)
   9.  Market Graph (BFS)
   10. Portfolio Management
   11. Exit
============================================
```

---

## Sample Output

**Market View:**
```
==========================================================
                    CRYPTO MARKET
==========================================================
| Coin         | Symbol | Price($)   | Change%    |
----------------------------------------------------------
| Bitcoin      | BTC    | 63000.00   | +2.50      |
| Ethereum     | ETH    | 3200.00    | +1.80      |
| Solana       | SOL    | 145.00     | +4.30      |
| Dogecoin     | DOGE   | 0.15       | -1.20      |
| XRP          | XRP    | 0.58       | +0.90      |
==========================================================
```

**BFS Graph Traversal:**
```
BTC --> ETH
ETH --> SOL
SOL --> DOGE
DOGE --> XRP
XRP --> BTC

BFS starting from Bitcoin:
Bitcoin  Ethereum  Solana  Dogecoin  XRP
```

**Portfolio:**
```
==========================================================
                   YOUR PORTFOLIO
==========================================================
| Coin         | Qty      | Buy($)   | P&L($)     |
----------------------------------------------------------
| Bitcoin      | 0.0500   | 60000.00 | +150.00    |
| Solana       | 2.0000   | 130.00   | +30.00     |
----------------------------------------------------------
  Total P&L: +$180.00
```

---

## Project Structure

```
cryptopulse/
│
└── cryptopulse.c        ← entire project in one file
```

The single file is organized into clear sections:

```
cryptopulse.c
│
├── Structures          (Coin, WatchNode, StackNode, HistoryEntry, PortNode)
├── Global Data         (market array, list/stack heads, queue, graph matrix)
├── Feature 1           viewMarket()         — Array
├── Feature 2           searchCoin()         — Linear Search
├── Feature 3           watchlistMenu()      — Linked List + Stack
├── Feature 4           topGainersLosers()   — Bubble Sort
├── Feature 5           priceHistoryMenu()   — Queue
├── Feature 6           priceAlerts()        — Array lookup
├── Feature 7           trendingCoins()      — Counters
├── Feature 8           cryptoTree()         — Tree (static)
├── Feature 9           marketGraph()        — Graph + BFS
├── Feature 10          portfolioMenu()      — Linked List
└── main()
```

---

## Viva Q&A

**Q: Why did you use a Linked List for the watchlist instead of an array?**  
A: A Linked List allows dynamic insertion and deletion without shifting elements, which is efficient when we don't know how many coins a user will add.

**Q: How does the Stack help with the undo feature?**  
A: When a coin is removed from the watchlist, its name is pushed onto the stack. When the user chooses "Undo", we pop from the stack and re-insert it into the linked list — following the classic LIFO principle.

**Q: Why is the Queue circular?**  
A: A circular queue reuses the same fixed memory. When the queue is full and a new entry arrives, it overwrites the oldest entry by advancing the `front` pointer — avoiding wasted space.

**Q: What is the time complexity of Bubble Sort?**  
A: O(n²) in the average and worst case. For our use case with only 5 coins, this is perfectly fine and easy to understand.

**Q: How does BFS work in your graph?**  
A: BFS uses a queue. We start from a source node (Bitcoin), mark it visited, and enqueue it. We then dequeue a node, visit all its unvisited neighbors, and enqueue them — repeating until the queue is empty.

**Q: Why is the graph stored as an adjacency matrix?**  
A: An adjacency matrix is simple to implement for a small, fixed number of nodes. With only 5 coins, the 5×5 matrix uses minimal memory and allows O(1) edge lookup.

---

## Concepts Demonstrated

- Dynamic memory allocation (`malloc`, `free`)
- Pointer-based data structures
- Modular programming with functions
- Formatted console output (`printf` with field widths)
- Circular indexing for queue management
- Graph traversal (BFS)

---

## Author

**Your Name**  
B.Tech / BCA — [Your College Name]  
Data Structures Mini Project — [Semester & Year]

---

## License

This project is open source and available under the [MIT License](LICENSE).

> *Built for learning. Designed for clarity.*