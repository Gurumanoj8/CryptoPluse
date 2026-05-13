/*
 * ============================================================
 *  CryptoPulse: A Data Structure Based Cryptocurrency
 *               Price Tracking and Analysis System
 * ============================================================
 *  College Mini Project - Data Structures (C Language)
 *  
 *  Data Structures Used:
 *    1. Array          - Coin market data
 *    2. Linked List    - Watchlist, Portfolio
 *    3. Stack          - Undo watchlist remove
 *    4. Queue          - Price history
 *    5. Bubble Sort    - Top gainers / losers
 *    6. Linear Search  - Search by name / symbol
 *    7. Tree           - Crypto category display
 *    8. Graph (BFS)    - Market relationships
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ============================================================
 *  ANSI COLOR CODES
 * ============================================================ */
#define RESET       "\x1b[0m"
#define BOLD        "\x1b[1m"
#define DIM         "\x1b[2m"
#define CYAN        "\x1b[36m"
#define GREEN       "\x1b[32m"
#define RED         "\x1b[31m"
#define YELLOW      "\x1b[33m"
#define MAGENTA     "\x1b[35m"
#define WHITE       "\x1b[37m"
#define BG_CYAN     "\x1b[46m"
#define BG_GREEN    "\x1b[42m"
#define BG_RED      "\x1b[41m"

/* ============================================================
 *  CONSTANTS
 * ============================================================ */
#define MAX_COINS        5
#define QUEUE_SIZE      10
#define MAX_GRAPH_NODES  5
#define MAX_NAME        20
#define MAX_SYMBOL       8
#define MAX_TIME        10

/* ============================================================
 *  STRUCTURE: Coin (used in Array)
 * ============================================================ */
typedef struct {
    char   name[MAX_NAME];
    char   symbol[MAX_SYMBOL];
    double price;
    double change;       /* daily % change */
    int    searchCount;  /* for trending */
    int    watchCount;   /* for trending */
} Coin;

/* ============================================================
 *  STRUCTURE: Watchlist Node (Linked List)
 * ============================================================ */
typedef struct WatchNode {
    char           coinName[MAX_NAME];
    struct WatchNode *next;
} WatchNode;

/* ============================================================
 *  STRUCTURE: Stack Node (for undo remove)
 * ============================================================ */
typedef struct StackNode {
    char           coinName[MAX_NAME];
    struct StackNode *next;
} StackNode;

/* ============================================================
 *  STRUCTURE: Price History Entry (Queue)
 * ============================================================ */
typedef struct {
    char   time[MAX_TIME];
    char   symbol[MAX_SYMBOL];
    double price;
} HistoryEntry;

/* ============================================================
 *  STRUCTURE: Portfolio Node (Linked List)
 * ============================================================ */
typedef struct PortNode {
    char   coinName[MAX_NAME];
    double buyPrice;
    double quantity;
    struct PortNode *next;
} PortNode;

/* ============================================================
 *  GLOBAL DATA
 * ============================================================ */

/* --- Market array (initial coin data) --- */
Coin market[MAX_COINS] = {
    {"Bitcoin",  "BTC", 63000.00, +2.50, 0, 0},
    {"Ethereum", "ETH",  3200.00, +1.80, 0, 0},
    {"Solana",   "SOL",   145.00, +4.30, 0, 0},
    {"Dogecoin", "DOGE",    0.15, -1.20, 0, 0},
    {"XRP",      "XRP",     0.58, +0.90, 0, 0}
};

/* --- Watchlist head pointer (Linked List) --- */
WatchNode *watchHead = NULL;

/* --- Stack top pointer (Undo remove) --- */
StackNode *stackTop = NULL;

/* --- Price History Queue --- */
HistoryEntry historyQueue[QUEUE_SIZE];
int queueFront = 0, queueRear = 0, queueCount = 0;

/* --- Portfolio head pointer (Linked List) --- */
PortNode *portHead = NULL;

/* --- Graph adjacency matrix (MAX_COINS x MAX_COINS) --- */
int graph[MAX_GRAPH_NODES][MAX_GRAPH_NODES] = {
    /* BTC ETH SOL DOGE XRP */
    {0,  1,  0,  0,  0},  /* BTC -> ETH */
    {0,  0,  1,  0,  0},  /* ETH -> SOL */
    {0,  0,  0,  1,  0},  /* SOL -> DOGE */
    {0,  0,  0,  0,  1},  /* DOGE -> XRP */
    {1,  0,  0,  0,  0}   /* XRP -> BTC */
};

/* ============================================================
 *  HELPER: Print decorative borders
 * ============================================================ */
void printLine(int width) {
    int i;
    printf("%s", CYAN);
    printf("═");
    for (i = 1; i < width - 1; i++) printf("═");
    printf("═%s\n", RESET);
}

void printDash(int width) {
    int i;
    printf("%s", CYAN);
    printf("─");
    for (i = 1; i < width - 1; i++) printf("─");
    printf("─%s\n", RESET);
}

/* ============================================================
 *  HELPER: Welcome / Exit Screen
 * ============================================================ */
void showWelcome() {
    printf("\n");
    printLine(58);
    printf("%s%s          ╔═══════════════════════╗%s\n", BOLD, YELLOW, RESET);
    printf("%s%s          ║  ★ CRYPTOPULSE ★      ║%s\n", BOLD, YELLOW, RESET);
    printf("%s%s          ║  🔐 Crypto Tracker 🔐 ║%s\n", BOLD, YELLOW, RESET);
    printf("%s%s          ╚═══════════════════════╝%s\n", BOLD, YELLOW, RESET);
    printLine(58);
    printf("%s   Data Structures: [Array] [List] [Stack] [Queue] [Tree] [Graph]%s\n", CYAN, RESET);
    printLine(58);
    printf("\n%s Press ENTER to begin your crypto journey...%s\n\n", MAGENTA, RESET);
    getchar();
}

void showExit() {
    printf("\n");
    printLine(58);
    printf("%s%s          Thank you for using CryptoPulse!%s\n", BOLD, GREEN, RESET);
    printf("%s%s       Happy Trading! Goodbye! 🚀📈%s\n", BOLD, GREEN, RESET);
    printLine(58);
    printf("\n");
}

/* ============================================================
 *  FEATURE 1: VIEW CRYPTO MARKET  (uses Array)
 * ============================================================ */
void viewMarket() {
    int i;
    printf("\n");
    printLine(72);
    printf("%s%s               📊 CRYPTO MARKET 📊%s\n", BOLD, CYAN, RESET);
    printLine(72);
    printf("%s| %-15s | %-8s | %-12s | %-12s |%s\n", CYAN, 
           "Coin", "Symbol", "Price($)", "Change(%)", RESET);
    printDash(72);
    for (i = 0; i < MAX_COINS; i++) {
        printf("| %-15s | %-8s | %12.2f | ", 
               market[i].name,
               market[i].symbol,
               market[i].price);
        
        if (market[i].change >= 0) {
            printf("%s%s+%-11.2f%s |%s\n", GREEN, BOLD, market[i].change, RESET, RESET);
        } else {
            printf("%s%s%-12.2f%s |%s\n", RED, BOLD, market[i].change, RESET, RESET);
        }
    }
    printLine(72);
}

/* ============================================================
 *  FEATURE 2: SEARCH COIN  (uses Linear Search)
 * ============================================================ */
int linearSearch(char *query) {
    int i;
    /* Compare query with both coin name and symbol (case check basic) */
    for (i = 0; i < MAX_COINS; i++) {
        if (strcmp(market[i].name, query) == 0 ||
            strcmp(market[i].symbol, query) == 0) {
            return i;  /* Return index if found */
        }
    }
    return -1;  /* Not found */
}

void searchCoin() {
    char query[MAX_NAME];
    int  idx;

    printf("\n");
    printLine(50);
    printf("%s%s           🔍 SEARCH COIN 🔍%s\n", BOLD, CYAN, RESET);
    printLine(50);
    printf("  Enter Coin Name or Symbol: ");
    scanf("%s", query);

    idx = linearSearch(query);

    if (idx == -1) {
        printf("\n  %s✗ Coin '%s' not found.%s\n", RED, query, RESET);
    } else {
        /* Increment search counter for trending */
        market[idx].searchCount++;

        printf("\n");
        printDash(50);
        printf("  %sName   %s: %s%s%s\n", CYAN, RESET, BOLD, market[idx].name, RESET);
        printf("  %sSymbol %s: %s%s%s\n", CYAN, RESET, BOLD, market[idx].symbol, RESET);
        printf("  %sPrice  %s: $%s%.2f%s\n", CYAN, RESET, YELLOW, market[idx].price, RESET);
        printf("  %sChange %s: ", CYAN, RESET);
        if (market[idx].change >= 0) {
            printf("%s+%.2f%%%s\n", GREEN, market[idx].change, RESET);
        } else {
            printf("%s%.2f%%%s\n", RED, market[idx].change, RESET);
        }
        printDash(50);
    }
}

/* ============================================================
 *  WATCHLIST HELPER FUNCTIONS (Linked List)
 * ============================================================ */

/* Check if coin already exists in watchlist */
int isInWatchlist(char *name) {
    WatchNode *temp = watchHead;
    while (temp != NULL) {
        if (strcmp(temp->coinName, name) == 0) return 1;
        temp = temp->next;
    }
    return 0;
}

/* Add coin to watchlist linked list */
void addToWatchlist(char *name) {
    int found = 0, i;

    /* Verify coin exists in market */
    for (i = 0; i < MAX_COINS; i++) {
        if (strcmp(market[i].name, name) == 0 ||
            strcmp(market[i].symbol, name) == 0) {
            found = 1;
            /* Copy full name in case symbol was entered */
            strcpy(name, market[i].name);
            market[i].watchCount++;
            break;
        }
    }

    if (!found) {
        printf("\n  %s✗ Coin not found in market.%s\n", RED, RESET);
        return;
    }

    if (isInWatchlist(name)) {
        printf("\n  %s⚠ '%s' is already in watchlist.%s\n", YELLOW, name, RESET);
        return;
    }

    /* Create new node and insert at beginning of list */
    WatchNode *newNode = (WatchNode *)malloc(sizeof(WatchNode));
    strcpy(newNode->coinName, name);
    newNode->next = watchHead;
    watchHead = newNode;

    printf("\n  %s✓ '%s' added to watchlist!%s\n", GREEN, name, RESET);
}

/* Remove coin from watchlist and push to undo stack */
void removeFromWatchlist(char *name) {
    WatchNode *temp = watchHead, *prev = NULL;

    while (temp != NULL) {
        if (strcmp(temp->coinName, name) == 0) {
            /* Remove from linked list */
            if (prev == NULL)
                watchHead = temp->next;
            else
                prev->next = temp->next;

            /* Push removed coin onto stack (for undo) */
            StackNode *sNode = (StackNode *)malloc(sizeof(StackNode));
            strcpy(sNode->coinName, name);
            sNode->next = stackTop;
            stackTop = sNode;

            free(temp);
            printf("\n  %s✓ '%s' removed from watchlist.%s\n", GREEN, name, RESET);
            printf("  %sℹ Use 'Undo Remove' to restore it.%s\n", CYAN, RESET);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("\n  %s✗ '%s' not found in watchlist.%s\n", RED, name, RESET);
}

/* Undo last remove (pop from stack, add back to list) */
void undoRemove() {
    if (stackTop == NULL) {
        printf("\n  %s✗ Nothing to undo.%s\n", RED, RESET);
        return;
    }
    /* Pop from stack */
    StackNode *top = stackTop;
    stackTop = stackTop->next;

    /* Re-add to watchlist */
    addToWatchlist(top->coinName);
    printf("  %s⟲ Undo successful!%s\n", YELLOW, RESET);
    free(top);
}

/* Display all watchlist entries */
void displayWatchlist() {
    if (watchHead == NULL) {
        printf("\n  %sℹ Watchlist is empty.%s\n", CYAN, RESET);
        return;
    }
    printf("\n");
    printDash(40);
    printf("%s%s  ⭐ YOUR WATCHLIST ⭐%s\n", BOLD, YELLOW, RESET);
    printDash(40);
    WatchNode *temp = watchHead;
    int count = 1;
    while (temp != NULL) {
        printf("  %s%d.%s %s%s%s\n", CYAN, count++, RESET, BOLD, temp->coinName, RESET);
        temp = temp->next;
    }
    printDash(40);
}

/* ============================================================
 *  FEATURE 3: WATCHLIST MENU
 * ============================================================ */
void watchlistMenu() {
    int   choice;
    char  coinName[MAX_NAME];

    do {
        printf("\n");
        printLine(45);
        printf("%s%s      📋 WATCHLIST MANAGEMENT 📋%s\n", BOLD, CYAN, RESET);
        printLine(45);
        printf("  %s1.%s Add Coin\n", YELLOW, RESET);
        printf("  %s2.%s Remove Coin\n", YELLOW, RESET);
        printf("  %s3.%s Display Watchlist\n", YELLOW, RESET);
        printf("  %s4.%s Undo Last Remove\n", YELLOW, RESET);
        printf("  %s5.%s Back to Main Menu\n", YELLOW, RESET);
        printLine(45);
        printf("  Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("  Enter coin name or symbol: ");
                scanf("%s", coinName);
                addToWatchlist(coinName);
                break;
            case 2:
                displayWatchlist();
                printf("  Enter coin name to remove: ");
                scanf("%s", coinName);
                removeFromWatchlist(coinName);
                break;
            case 3:
                displayWatchlist();
                break;
            case 4:
                undoRemove();
                break;
            case 5:
                break;
            default:
                printf("  %s✗ Invalid choice.%s\n", RED, RESET);
        }
    } while (choice != 5);
}

/* ============================================================
 *  FEATURE 4: TOP GAINERS & LOSERS  (Bubble Sort)
 * ============================================================ */
void topGainersLosers() {
    /* Create a copy of market to sort (don't disturb original) */
    Coin sorted[MAX_COINS];
    int  i, j;
    Coin temp;

    for (i = 0; i < MAX_COINS; i++)
        sorted[i] = market[i];

    /* Bubble Sort: sort by change% in descending order */
    for (i = 0; i < MAX_COINS - 1; i++) {
        for (j = 0; j < MAX_COINS - i - 1; j++) {
            if (sorted[j].change < sorted[j + 1].change) {
                temp       = sorted[j];
                sorted[j]  = sorted[j + 1];
                sorted[j+1] = temp;
            }
        }
    }

    printf("\n");
    printLine(56);
    printf("%s%s   📈 TOP GAINERS (Sorted by Daily Change) 📈%s\n", BOLD, GREEN, RESET);
    printLine(56);
    printf("%s| %-15s | %-8s | %-12s |%s\n", CYAN, "Coin", "Symbol", "Change(%)", RESET);
    printDash(56);
    for (i = 0; i < MAX_COINS; i++) {
        printf("| %-15s | %-8s | ", sorted[i].name, sorted[i].symbol);
        if (sorted[i].change >= 0)
            printf("%s%s+%-11.2f%s |%s\n", GREEN, BOLD, sorted[i].change, RESET, RESET);
        else
            printf("%s%s%-12.2f%s |%s\n", RED, BOLD, sorted[i].change, RESET, RESET);
    }
    printLine(56);
}

/* ============================================================
 *  FEATURE 5: PRICE HISTORY  (Queue - Circular)
 * ============================================================ */

/* Enqueue a new price update */
void enqueueHistory(char *time, char *symbol, double price) {
    if (queueCount == QUEUE_SIZE) {
        /* Queue full: overwrite oldest entry */
        queueFront = (queueFront + 1) % QUEUE_SIZE;
        queueCount--;
    }
    strcpy(historyQueue[queueRear].time,   time);
    strcpy(historyQueue[queueRear].symbol, symbol);
    historyQueue[queueRear].price = price;
    queueRear  = (queueRear + 1) % QUEUE_SIZE;
    queueCount++;
}

/* Display price history from queue */
void displayHistory() {
    printf("\n");
    printLine(50);
    printf("%s%s         📊 PRICE HISTORY (Queue) 📊%s\n", BOLD, CYAN, RESET);
    printLine(50);

    if (queueCount == 0) {
        printf("  %sℹ No price history yet.%s\n", CYAN, RESET);
        printf("  %sℹ Add entries using 'Add Price Update'.%s\n", CYAN, RESET);
    } else {
        printf("  %s%-8s | %-6s | %s%s\n", CYAN, "Time", "Symbol", "Price($)", RESET);
        printDash(50);
        int i, idx;
        for (i = 0; i < queueCount; i++) {
            idx = (queueFront + i) % QUEUE_SIZE;
            printf("  %-8s | %s%-6s%s | %s%.2f%s\n",
                   historyQueue[idx].time,
                   YELLOW, historyQueue[idx].symbol, RESET,
                   MAGENTA, historyQueue[idx].price, RESET);
        }
    }
    printLine(50);
}

void addPriceHistory() {
    char   time[MAX_TIME], symbol[MAX_SYMBOL];
    double price;

    printf("\n  Enter Time (e.g. 10:00): ");
    scanf("%s", time);
    printf("  Enter Symbol (e.g. BTC): ");
    scanf("%s", symbol);
    printf("  Enter Price: ");
    scanf("%lf", &price);

    enqueueHistory(time, symbol, price);
    printf("\n  %s✓ Price update added to history!%s\n", GREEN, RESET);
}

void priceHistoryMenu() {
    int choice;
    do {
        printf("\n");
        printLine(45);
        printf("%s%s         📈 PRICE HISTORY 📈%s\n", BOLD, CYAN, RESET);
        printLine(45);
        printf("  %s1.%s View History\n", YELLOW, RESET);
        printf("  %s2.%s Add Price Update\n", YELLOW, RESET);
        printf("  %s3.%s Back\n", YELLOW, RESET);
        printLine(45);
        printf("  Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayHistory();  break;
            case 2: addPriceHistory(); break;
            case 3: break;
            default: printf("  %s✗ Invalid choice.%s\n", RED, RESET);
        }
    } while (choice != 3);
}

/* ============================================================
 *  FEATURE 6: PRICE ALERTS
 * ============================================================ */
void priceAlerts() {
    char   symbol[MAX_SYMBOL];
    double target;
    int    i, found = -1;

    printf("\n");
    printLine(50);
    printf("%s%s           🔔 PRICE ALERTS 🔔%s\n", BOLD, YELLOW, RESET);
    printLine(50);
    printf("  Enter Symbol (e.g. BTC): ");
    scanf("%s", symbol);
    printf("  Enter Target Price ($): ");
    scanf("%lf", &target);

    /* Find coin in market */
    for (i = 0; i < MAX_COINS; i++) {
        if (strcmp(market[i].symbol, symbol) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n  %s✗ Coin not found.%s\n", RED, RESET);
        return;
    }

    printf("\n  Alert set for %s at $%.2f\n",
           market[found].name, target);
    printf("  Current Price: %s$%.2f%s\n", MAGENTA, market[found].price, RESET);

    /* Check if alert condition is already met */
    if (market[found].price >= target) {
        printf("\n  %s%s*** ALERT TRIGGERED! ***%s\n", BOLD, GREEN, RESET);
        printf("  %s%s%s has REACHED your target of $%.2f!%s\n",
               BOLD, market[found].name, RESET, target, RESET);
    } else {
        printf("\n  %sℹ Alert saved. Notified when price reaches $%.2f%s\n", CYAN, target, RESET);
        printf("  %sℹ (In a real app, this runs in background)%s\n", CYAN, RESET);
    }
    printLine(50);
}

/* ============================================================
 *  FEATURE 7: TRENDING COINS  (Counter-based)
 * ============================================================ */
void trendingCoins() {
    int i;
    /* Find most searched and most watched */
    int maxSearch = 0, maxWatch = 0;
    int searchIdx = 0, watchIdx = 0;

    for (i = 0; i < MAX_COINS; i++) {
        if (market[i].searchCount > maxSearch) {
            maxSearch = market[i].searchCount;
            searchIdx = i;
        }
        if (market[i].watchCount > maxWatch) {
            maxWatch = market[i].watchCount;
            watchIdx = i;
        }
    }

    printf("\n");
    printLine(50);
    printf("%s%s           🔥 TRENDING COINS 🔥%s\n", BOLD, RED, RESET);
    printLine(50);

    printf("\n  %s[*] Most Searched:%s\n", CYAN, RESET);
    if (maxSearch == 0)
        printf("      %sNo searches yet. Try 'Search Coin' first.%s\n", YELLOW, RESET);
    else
        printf("      %s%s%s (%s) - %d search(es)%s\n",
               BOLD, market[searchIdx].name, RESET,
               market[searchIdx].symbol,
               maxSearch, GREEN);

    printf("\n  %s[*] Most Watched:%s\n", CYAN, RESET);
    if (maxWatch == 0)
        printf("      %sNo watches yet. Try 'Add to Watchlist' first.%s\n", YELLOW, RESET);
    else
        printf("      %s%s%s (%s) - added %d time(s)%s\n",
               BOLD, market[watchIdx].name, RESET,
               market[watchIdx].symbol,
               maxWatch, GREEN);

    printf("\n");
    printLine(50);
    printf("  %sAll Coins - Search & Watch Counts:%s\n", BOLD, RESET);
    printLine(50);
    printf("  %-12s | Searches | Watches\n", "Coin");
    printDash(50);
    for (i = 0; i < MAX_COINS; i++) {
        printf("  %s%-12s%s | %-8d | %d\n", YELLOW, market[i].name, RESET,
               market[i].searchCount,
               market[i].watchCount);
    }
    printLine(50);
}

/* ============================================================
 *  FEATURE 8: CRYPTO CATEGORY TREE  (Simple static tree)
 * ============================================================ */
void cryptoTree() {
    printf("\n");
    printLine(50);
    printf("%s%s       🌳 CRYPTO CATEGORY TREE 🌳%s\n", BOLD, GREEN, RESET);
    printLine(50);
    printf("\n");
    printf("  %sCryptocurrency%s\n", BOLD, RESET);
    printf("  %s│%s\n", CYAN, RESET);
    printf("  %s├── Layer 1 Coins%s\n", CYAN, RESET);
    printf("  %s│   ├── %sBitcoin   %s(BTC)%s\n", CYAN, YELLOW, RESET, RESET);
    printf("  %s│   ├── %sEthereum  %s(ETH)%s\n", CYAN, YELLOW, RESET, RESET);
    printf("  %s│   └── %sSolana    %s(SOL)%s\n", CYAN, YELLOW, RESET, RESET);
    printf("  %s│%s\n", CYAN, RESET);
    printf("  %s├── Meme Coins%s\n", CYAN, RESET);
    printf("  %s│   └── %sDogecoin  %s(DOGE)%s\n", CYAN, YELLOW, RESET, RESET);
    printf("  %s│%s\n", CYAN, RESET);
    printf("  %s├── Payment Coins%s\n", CYAN, RESET);
    printf("  %s│   └── %sXRP       %s(XRP)%s\n", CYAN, YELLOW, RESET, RESET);
    printf("  %s│%s\n", CYAN, RESET);
    printf("  %s├── Stablecoins%s\n", CYAN, RESET);
    printf("  %s│   ├── %sUSDT%s\n", CYAN, MAGENTA, RESET);
    printf("  %s│   └── %sUSDC%s\n", CYAN, MAGENTA, RESET);
    printf("  %s│%s\n", CYAN, RESET);
    printf("  %s└── AI Coins%s\n", CYAN, RESET);
    printf("      ├── %sFetch.ai  %s(FET)%s\n", GREEN, RESET, RESET);
    printf("      └── %sRender    %s(RNDR)%s\n", GREEN, RESET, RESET);
    printf("\n");
    printLine(50);
}

/* ============================================================
 *  FEATURE 9: MARKET GRAPH with BFS Traversal
 * ============================================================ */
void bfsTraversal(int start) {
    int visited[MAX_GRAPH_NODES] = {0};
    int bfsQueue[MAX_GRAPH_NODES];
    int front = 0, rear = 0;

    printf("  %s", YELLOW);

    /* Enqueue start node */
    bfsQueue[rear++] = start;
    visited[start]   = 1;

    while (front < rear) {
        int current = bfsQueue[front++];
        printf("%s  →  ", market[current].name);

        /* Visit all adjacent nodes */
        int j;
        for (j = 0; j < MAX_GRAPH_NODES; j++) {
            if (graph[current][j] == 1 && !visited[j]) {
                visited[j]       = 1;
                bfsQueue[rear++] = j;
            }
        }
    }
    printf("%s✓%s\n", GREEN, RESET);
}

void marketGraph() {
    int i, j;
    printf("\n");
    printLine(56);
    printf("%s%s         🔗 MARKET GRAPH 🔗%s\n", BOLD, MAGENTA, RESET);
    printLine(56);
    printf("  %sCoin Relationships (directed edges):%s\n", BOLD, RESET);
    printDash(56);

    /* Display adjacency list style */
    for (i = 0; i < MAX_GRAPH_NODES; i++) {
        printf("  %s%-8s%s -->", YELLOW, market[i].symbol, RESET);
        int hasEdge = 0;
        for (j = 0; j < MAX_GRAPH_NODES; j++) {
            if (graph[i][j] == 1) {
                printf(" %s%s%s", GREEN, market[j].symbol, RESET);
                hasEdge = 1;
            }
        }
        if (!hasEdge) printf(" %s(no outgoing)%s", CYAN, RESET);
        printf("\n");
    }

    printDash(56);
    printf("\n  %sBFS Traversal:%s\n", BOLD, RESET);
    bfsTraversal(0);  /* BFS from Bitcoin */
    printLine(56);
}

/* ============================================================
 *  PORTFOLIO HELPERS (Linked List)
 * ============================================================ */

/* Add or update holding in portfolio */
void buyCoin(char *name, double qty, double price) {
    int i, found = -1;
    /* Verify coin exists */
    for (i = 0; i < MAX_COINS; i++) {
        if (strcmp(market[i].name, name) == 0 ||
            strcmp(market[i].symbol, name) == 0) {
            found = i;
            strcpy(name, market[i].name);
            break;
        }
    }
    if (found == -1) {
        printf("\n  %s✗ Coin not found.%s\n", RED, RESET);
        return;
    }

    /* Create new portfolio node */
    PortNode *newNode = (PortNode *)malloc(sizeof(PortNode));
    strcpy(newNode->coinName, name);
    newNode->buyPrice = price;
    newNode->quantity = qty;
    newNode->next     = portHead;
    portHead          = newNode;

    printf("\n  %s✓ Bought %.4f %s at $%.2f%s\n", GREEN, qty, name, price, RESET);
}

/* Remove/sell a coin from portfolio */
void sellCoin(char *name) {
    PortNode *temp = portHead, *prev = NULL;
    int found = 0;

    while (temp != NULL) {
        if (strcmp(temp->coinName, name) == 0) {
            /* Find current market price */
            double currentPrice = 0;
            int i;
            for (i = 0; i < MAX_COINS; i++) {
                if (strcmp(market[i].name, name) == 0) {
                    currentPrice = market[i].price;
                    break;
                }
            }
            double profit = (currentPrice - temp->buyPrice) * temp->quantity;

            printf("\n  %sSelling %s...%s\n", YELLOW, name, RESET);
            printf("  Buy Price    : $%.2f\n", temp->buyPrice);
            printf("  Current Price: $%.2f\n", currentPrice);
            printf("  Quantity     : %.4f\n", temp->quantity);
            if (profit >= 0)
                printf("  %sProfit       : +$%.2f%s\n", GREEN, profit, RESET);
            else
                printf("  %sLoss         : -$%.2f%s\n", RED, -profit, RESET);

            /* Remove from list */
            if (prev == NULL) portHead = temp->next;
            else prev->next = temp->next;
            free(temp);
            found = 1;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (!found) printf("\n  %s✗ '%s' not in portfolio.%s\n", RED, name, RESET);
}

/* Display portfolio and P&L */
void viewPortfolio() {
    if (portHead == NULL) {
        printf("\n  %sℹ Portfolio is empty.%s\n", CYAN, RESET);
        return;
    }

    printf("\n");
    printLine(72);
    printf("%s%s               💼 YOUR PORTFOLIO 💼%s\n", BOLD, CYAN, RESET);
    printLine(72);
    printf("%s| %-15s | %-10s | %-10s | %-12s |%s\n", CYAN,
           "Coin", "Qty", "Buy($)", "P&L($)", RESET);
    printDash(72);

    PortNode *temp = portHead;
    double totalPL = 0;

    while (temp != NULL) {
        double currentPrice = 0;
        int i;
        for (i = 0; i < MAX_COINS; i++) {
            if (strcmp(market[i].name, temp->coinName) == 0) {
                currentPrice = market[i].price;
                break;
            }
        }
        double pl = (currentPrice - temp->buyPrice) * temp->quantity;
        totalPL += pl;

        printf("| %-15s | %-10.4f | %-10.2f | ", 
               temp->coinName, temp->quantity, temp->buyPrice);
        if (pl >= 0)
            printf("%s%s+%-11.2f%s |%s\n", GREEN, BOLD, pl, RESET, RESET);
        else
            printf("%s%s%-12.2f%s |%s\n", RED, BOLD, pl, RESET, RESET);

        temp = temp->next;
    }

    printDash(72);
    if (totalPL >= 0)
        printf("  %sTOTAL P&L: +$%.2f%s\n", GREEN, totalPL, RESET);
    else
        printf("  %sTOTAL P&L: -$%.2f%s\n", RED, -totalPL, RESET);
    printLine(72);
}

/* ============================================================
 *  FEATURE 10: PORTFOLIO MENU
 * ============================================================ */
void portfolioMenu() {
    int    choice;
    char   coinName[MAX_NAME];
    double qty, price;

    do {
        printf("\n");
        printLine(45);
        printf("%s%s       💰 PORTFOLIO MANAGEMENT 💰%s\n", BOLD, CYAN, RESET);
        printLine(45);
        printf("  %s1.%s Buy Coin\n", YELLOW, RESET);
        printf("  %s2.%s Sell Coin\n", YELLOW, RESET);
        printf("  %s3.%s View Portfolio\n", YELLOW, RESET);
        printf("  %s4.%s Back to Main Menu\n", YELLOW, RESET);
        printLine(45);
        printf("  Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("  Enter coin name/symbol: ");
                scanf("%s", coinName);
                printf("  Enter quantity: ");
                scanf("%lf", &qty);
                printf("  Enter buy price ($): ");
                scanf("%lf", &price);
                buyCoin(coinName, qty, price);
                break;
            case 2:
                viewPortfolio();
                printf("  Enter coin name to sell: ");
                scanf("%s", coinName);
                sellCoin(coinName);
                break;
            case 3:
                viewPortfolio();
                break;
            case 4:
                break;
            default:
                printf("  %s✗ Invalid choice.%s\n", RED, RESET);
        }
    } while (choice != 4);
}

/* ============================================================
 *  MAIN MENU
 * ============================================================ */
void mainMenu() {
    int choice;

    do {
        printf("\n");
        printLine(54);
        printf("%s%s         🎯 CRYPTOPULSE DASHBOARD 🎯%s\n", BOLD, MAGENTA, RESET);
        printLine(54);
        printf("   %s1.%s   View Market\n", YELLOW, RESET);
        printf("   %s2.%s   Search Coin\n", YELLOW, RESET);
        printf("   %s3.%s   Watchlist Management\n", YELLOW, RESET);
        printf("   %s4.%s   Top Gainers & Losers\n", YELLOW, RESET);
        printf("   %s5.%s   Price History\n", YELLOW, RESET);
        printf("   %s6.%s   Price Alerts\n", YELLOW, RESET);
        printf("   %s7.%s   Trending Coins\n", YELLOW, RESET);
        printf("   %s8.%s   Crypto Categories (Tree)\n", YELLOW, RESET);
        printf("   %s9.%s   Market Graph (BFS)\n", YELLOW, RESET);
        printf("   %s10.%s  Portfolio Management\n", YELLOW, RESET);
        printf("   %s11.%s  Exit\n", YELLOW, RESET);
        printLine(54);
        printf("   Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case  1: viewMarket();       break;
            case  2: searchCoin();       break;
            case  3: watchlistMenu();    break;
            case  4: topGainersLosers(); break;
            case  5: priceHistoryMenu(); break;
            case  6: priceAlerts();      break;
            case  7: trendingCoins();    break;
            case  8: cryptoTree();       break;
            case  9: marketGraph();      break;
            case 10: portfolioMenu();    break;
            case 11: break;
            default: printf("\n  %s✗ Invalid choice. Try again.%s\n", RED, RESET);
        }
    } while (choice != 11);
}

/* ============================================================
 *  MAIN FUNCTION
 * ============================================================ */
int main() {

    SetConsoleOutputCP(CP_UTF8);
    /* Seed some sample price history in the queue */
    enqueueHistory("09:00", "BTC", 62000.00);
    enqueueHistory("10:00", "BTC", 62500.00);
    enqueueHistory("11:00", "ETH",  3150.00);
    enqueueHistory("12:00", "BTC", 63000.00);
    enqueueHistory("13:00", "SOL",   144.50);

    showWelcome();
    mainMenu();
    showExit();

    return 0;
}