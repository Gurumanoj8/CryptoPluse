# 💹 CryptoPluse

### A Data Structure Based Cryptocurrency Price Tracking and Analysis System

CryptoPluse is a cryptocurrency tracking and analysis application developed in **C** with a **Windows Desktop GUI**.

The project demonstrates how fundamental data structures and algorithms can be used to build a practical cryptocurrency application with features such as market tracking, coin search, watchlist management, price history, alerts, sorting, graph traversal, and portfolio management.

This version converts the original terminal-based interface into a user-friendly graphical interface while maintaining the core data-structure concepts.

---

## 📋 Table of Contents

- [About the Project](#about-the-project)
- [Objectives](#objectives)
- [Features](#features)
- [Data Structures Used](#data-structures-used)
- [GUI Overview](#gui-overview)
- [Technologies Used](#technologies-used)
- [Getting Started](#getting-started)
- [Installation](#installation)
- [How to Run](#how-to-run)
- [Project Structure](#project-structure)
- [How the Data Structures Work](#how-the-data-structures-work)
- [Sample Market Data](#sample-market-data)
- [Future Improvements](#future-improvements)
- [Learning Outcomes](#learning-outcomes)
- [Author](#author)
- [License](#license)

---

## 📌 About the Project

CryptoPluse is a **Data Structures and Algorithms based cryptocurrency tracking system** developed in the C programming language.

The application provides a graphical interface for interacting with cryptocurrency market data and demonstrates the practical implementation of different data structures.

The project includes:

- Cryptocurrency market display
- Cryptocurrency search
- Watchlist management
- Top gainers and losers
- Price history
- Price alerts
- Trending cryptocurrency tracking
- Cryptocurrency categories
- Graph and BFS traversal
- Portfolio management

The main purpose of this project is to demonstrate how theoretical data-structure concepts can be applied to a real-world application.

---

## 🎯 Objectives

The main objectives of CryptoPluse are:

- To implement fundamental data structures in C.
- To understand practical applications of data structures.
- To develop a user-friendly desktop GUI.
- To implement searching and sorting algorithms.
- To demonstrate graph traversal using BFS.
- To implement dynamic memory allocation.
- To build a practical cryptocurrency analysis system.
- To provide an easy-to-understand project for Data Structures learning and viva presentation.

---

# 🚀 Features

## 1. 📊 Dashboard

The Dashboard provides an overview of the cryptocurrency market.

It displays:

- Number of cryptocurrencies
- Number of watchlist coins
- Price history information
- Top market movers
- Data structures used in the project

---

## 2. 💰 Crypto Market

The Market section displays cryptocurrency information in a structured format.

Each cryptocurrency contains:

- Coin Name
- Symbol
- Current Price
- 24-hour Percentage Change

Example:

```text
Bitcoin      BTC       $63000.00       +2.50%
Ethereum     ETH        $3200.00       +1.80%
Solana       SOL         $145.00       +4.30%
Dogecoin     DOGE          $0.15       -1.20%
XRP          XRP           $0.58       +0.90%
The cryptocurrency market data is stored using an Array.

3. 🔍 Search Cryptocurrency

The Search feature allows users to search for a cryptocurrency using:

Cryptocurrency name
Cryptocurrency symbol

For example:

BTC

or:

Bitcoin

The application uses Linear Search to find the requested cryptocurrency.

The search counter is also updated when a cryptocurrency is searched.

Example
Input:
BTC

Output:
Bitcoin (BTC)

Price: $63000.00
24H Change: +2.50%

Linear Search: FOUND
4. ⭐ Watchlist Management

The Watchlist allows users to keep track of selected cryptocurrencies.

Users can:

Add a cryptocurrency
Remove a cryptocurrency
Undo the last removal

The Watchlist is implemented using a Linked List.

The Undo functionality uses a Stack.

Working
Watchlist Linked List
        ↓
Coin Removed
        ↓
Coin pushed into Stack
        ↓
User selects Undo
        ↓
Coin popped from Stack
        ↓
Coin added back to Linked List
5. 📈 Top Gainers and Losers

The Gainers/Losers section ranks cryptocurrencies according to their daily percentage change.

The project uses Bubble Sort for sorting the market data.

Example:

1. SOL       +4.30%
2. BTC       +2.50%
3. ETH       +1.80%
4. XRP       +0.90%
5. DOGE      -1.20%
Algorithm

Bubble Sort repeatedly compares adjacent elements and swaps them when they are in the wrong order.

Time Complexity
Best Case:    O(n)
Average Case: O(n²)
Worst Case:   O(n²)

For a small cryptocurrency dataset, Bubble Sort is simple and suitable for demonstrating the algorithm.

6. 🕐 Price History

The Price History feature stores recent cryptocurrency price updates.

A Circular Queue is used for storing the history.

Each entry contains:

Time
Cryptocurrency symbol
Price

The queue stores a maximum of 10 recent entries.

When the queue becomes full, the oldest entry is replaced by the newest entry.

Why Circular Queue?

A circular queue efficiently reuses the available memory instead of continuously allocating new memory.

7. 🔔 Price Alerts

The Price Alerts feature allows users to set a target price for a cryptocurrency.

For example:

Coin: BTC
Target Price: $65000

The application checks the current price against the target price.

If the current price has reached or crossed the target, an alert message is displayed.

8. 🔥 Trending Coins

The Trending section keeps track of cryptocurrency activity.

Two counters are maintained:

searchCount
watchCount

These counters represent:

Number of times a coin was searched
Number of times a coin was added to the watchlist

This helps demonstrate how additional information can be maintained alongside array-based market records.

9. 🌳 Cryptocurrency Categories

The Categories section demonstrates a Tree data structure.

Cryptocurrencies are organized into categories such as:

Cryptocurrency
|
|-- Layer 1 Coins
|   |-- Bitcoin
|   |-- Ethereum
|   `-- Solana
|
|-- Meme Coins
|   `-- Dogecoin
|
|-- Payment Coins
|   `-- XRP
|
`-- AI Coins
    |-- Fetch.ai
    `-- Render

This demonstrates hierarchical organization using tree concepts.

10. 🕸️ Market Graph and BFS

The Graph section demonstrates a Graph data structure.

Cryptocurrencies are represented as nodes and relationships between them are represented as edges.

An Adjacency Matrix is used to represent the graph.

Example:

BTC → ETH → SOL → DOGE → XRP → BTC

The project uses Breadth-First Search (BFS) to traverse the graph.

BFS Traversal

Starting from Bitcoin:

Bitcoin
   ↓
Ethereum
   ↓
Solana
   ↓
Dogecoin
   ↓
XRP
Why BFS?

BFS explores nodes level by level and uses a queue internally.

11. 💼 Portfolio Management

The Portfolio section allows users to manage cryptocurrency holdings.

Users can:

Buy cryptocurrency
Sell cryptocurrency
View holdings
Calculate profit/loss

The portfolio is implemented using a Linked List.

Each portfolio entry stores:

Cryptocurrency name
Quantity
Buy price

The application calculates profit/loss using the current market price.

Profit/Loss Formula
P&L = (Current Price - Buy Price) × Quantity
🧠 Data Structures Used
#	Data Structure	Application
1	Array	Stores cryptocurrency market data
2	Linked List	Watchlist and portfolio
3	Stack	Undo watchlist removal
4	Circular Queue	Recent price history
5	Bubble Sort	Gainers and losers ranking
6	Linear Search	Cryptocurrency search
7	Tree	Cryptocurrency categories
8	Graph + BFS	Market relationships and traversal
🖥️ GUI Overview

The application provides a Windows desktop graphical interface.

Main Navigation
CryptoPluse
│
├── Dashboard
├── Market
├── Search
├── Watchlist
├── Gainers / Losers
├── Price History
├── Price Alerts
├── Trending
├── Categories
├── Graph + BFS
└── Portfolio

The GUI replaces the terminal-based menu system with buttons and dedicated application screens.

🛠️ Technologies Used
Programming Language
C
GUI Framework
Windows API (Win32)
Compiler
GCC / MinGW
Development Environment
Visual Studio Code
Concepts
Data Structures
Algorithms
Pointers
Dynamic Memory Allocation
Graph Traversal
Searching
Sorting
GUI Programming
💻 Getting Started
Prerequisites

Before running the project, make sure you have:

Windows operating system
Visual Studio Code
GCC / MinGW C compiler
Git

Check whether GCC is installed:

gcc --version

Check Git:

git --version
📥 Installation

Clone the repository:

git clone https://github.com/Gurumanoj8/CryptoPluse.git

Move into the project directory:

cd CryptoPluse
▶️ How to Run

Compile the GUI application using GCC:

gcc cryptopluse_gui.c -o CryptoPluseGUI.exe -mwindows

Run the application:

.\CryptoPluseGUI.exe

The CryptoPluse desktop GUI will open.

📁 Project Structure
CryptoPluse/
│
├── cryptopluse_gui.c
├── README.md
├── LICENSE
└── output/
Main Source File
cryptopluse_gui.c

This file contains:

GUI implementation
Cryptocurrency market data
Array operations
Linked List operations
Stack operations
Queue operations
Searching
Sorting
Tree representation
Graph and BFS
Portfolio management
🔄 How the Data Structures Work
Array

The market array stores cryptocurrency information.

Coin
├── Name
├── Symbol
├── Price
├── Change
├── Search Count
└── Watch Count
Linked List

The Watchlist is maintained using nodes.

HEAD
 ↓
BTC → ETH → SOL → NULL

Linked Lists allow dynamic insertion and deletion.

Stack

The Stack is used for the Undo operation.

TOP
 ↓
DOGE
 ↓
ETH
 ↓
BTC

The last removed coin is restored first because Stack follows:

LIFO
Last In First Out
Circular Queue

Price history uses a circular queue.

Front → [Entry] [Entry] [Entry] [Entry] ← Rear

When the queue reaches its maximum size, the oldest entry is replaced.

Bubble Sort

Market coins are sorted according to their percentage change.

Compare
   ↓
Swap if required
   ↓
Repeat
Linear Search

The Search feature checks each coin sequentially until the requested coin is found.

BTC
 ↓
Check Bitcoin
 ↓
Match
 ↓
Return result
Tree

The Category section demonstrates hierarchical relationships.

Cryptocurrency
       |
   Categories
       |
     Coins
Graph

Cryptocurrencies are represented as nodes.

BTC → ETH → SOL → DOGE → XRP

The relationships are stored using an adjacency matrix.

BFS

Breadth-First Search starts from a selected cryptocurrency and visits connected nodes level by level.

BFS uses a queue internally.

📊 Sample Market Data

The current sample market contains:

Cryptocurrency	Symbol	Price	Change
Bitcoin	BTC	$63,000.00	+2.50%
Ethereum	ETH	$3,200.00	+1.80%
Solana	SOL	$145.00	+4.30%
Dogecoin	DOGE	$0.15	-1.20%
XRP	XRP	$0.58	+0.90%

Note: The project currently uses sample market data for demonstration and educational purposes. It is not intended to represent real-time cryptocurrency prices.

🎓 Learning Outcomes

This project helps demonstrate practical understanding of:

Arrays
Structures
Pointers
Linked Lists
Stacks
Queues
Circular Queues
Searching Algorithms
Sorting Algorithms
Trees
Graphs
BFS
Dynamic Memory Allocation
GUI Programming
Git and GitHub
🔮 Future Improvements

Possible future improvements include:

Real-time cryptocurrency API integration
Live cryptocurrency prices
Interactive price charts
More cryptocurrencies
User authentication
Persistent watchlists
Persistent portfolios
Database integration
Advanced technical indicators
Dark/light theme switching
Improved graphical charts
Mobile/web version
🎯 Project Purpose

CryptoPluse was developed as an educational Data Structures and Algorithms project.

The goal is to demonstrate how different data structures and algorithms can be combined to create a practical application instead of implementing each concept independently.

👨‍💻 Author
JINKA GURUMANOJ
Data Structures & Algorithms Project

📄 License

This project is distributed under the MIT License.

See the LICENSE file for more information.

⭐ Acknowledgement

The GUI version is a modified version of the original CryptoPluse project.

The modification focuses on converting the terminal-based interface into a Windows desktop GUI while retaining the educational data-structure concepts.

⭐ Built with C | Data Structures | Algorithms | Win32 GUI
