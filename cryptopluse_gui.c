/*
 * CryptoPluse - GUI Edition
 * Windows desktop GUI in C
 *
 * Educational data-structure project:
 * Array, Linked List, Stack, Queue, Bubble Sort, Linear Search,
 * Tree, Graph/BFS and Portfolio Linked List.
 *
 * Build:
 *   gcc cryptopluse_gui.c -o CryptoPluseGUI.exe -mwindows
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COINS 5
#define QUEUE_SIZE 10
#define MAX_ALERTS 20
#define MAX_NAME 20
#define MAX_SYMBOL 8

typedef struct {
    char name[MAX_NAME];
    char symbol[MAX_SYMBOL];
    double price;
    double change;
    int searchCount;
    int watchCount;
} Coin;

typedef struct WatchNode {
    char coinName[MAX_NAME];
    struct WatchNode *next;
} WatchNode;

typedef struct StackNode {
    char coinName[MAX_NAME];
    struct StackNode *next;
} StackNode;

typedef struct {
    char time[16];
    char symbol[MAX_SYMBOL];
    double price;
} HistoryEntry;

typedef struct PortNode {
    char coinName[MAX_NAME];
    double buyPrice;
    double quantity;
    struct PortNode *next;
} PortNode;

typedef struct {
    char symbol[MAX_SYMBOL];
    double target;
} Alert;

static Coin market[MAX_COINS] = {
    {"Bitcoin",  "BTC",  63000.00,  2.50, 0, 0},
    {"Ethereum", "ETH",   3200.00,  1.80, 0, 0},
    {"Solana",   "SOL",    145.00,  4.30, 0, 0},
    {"Dogecoin", "DOGE",     0.15, -1.20, 0, 0},
    {"XRP",      "XRP",      0.58,  0.90, 0, 0}
};

static WatchNode *watchHead = NULL;
static StackNode *stackTop = NULL;
static HistoryEntry historyQueue[QUEUE_SIZE];
static int queueFront = 0, queueRear = 0, queueCount = 0;
static PortNode *portHead = NULL;

static Alert alerts[MAX_ALERTS];
static int alertCount = 0;

static int graph[MAX_COINS][MAX_COINS] = {
    {0,1,0,0,0},
    {0,0,1,0,0},
    {0,0,0,1,0},
    {0,0,0,0,1},
    {1,0,0,0,0}
};

/* ---------- GUI state ---------- */

static HWND gMain, gContent, gSearchEdit;
static HFONT fNormal, fSmall, fTitle, fBig, fButton;
static HBRUSH bWindow, bPanel, bSidebar, bEdit;
static int currentPage = 0;

static const COLORREF CLR_BG      = RGB(15, 18, 24);
static const COLORREF CLR_SIDEBAR = RGB(20, 24, 32);
static const COLORREF CLR_PANEL   = RGB(27, 33, 43);
static const COLORREF CLR_INPUT   = RGB(37, 44, 56);
static const COLORREF CLR_TEXT    = RGB(235, 240, 247);
static const COLORREF CLR_MUTED   = RGB(155, 166, 183);
static const COLORREF CLR_ACCENT  = RGB(55, 190, 155);
static const COLORREF CLR_GREEN   = RGB(70, 210, 135);
static const COLORREF CLR_RED     = RGB(240, 95, 105);
static const COLORREF CLR_BLUE    = RGB(85, 150, 235);
static const COLORREF CLR_WHITE   = RGB(255, 255, 255);

/* ---------- Data structures / logic ---------- */

static int findCoin(const char *q) {
    int i;
    for (i = 0; i < MAX_COINS; ++i) {
        if (_stricmp(market[i].name, q) == 0 ||
            _stricmp(market[i].symbol, q) == 0) return i;
    }
    return -1;
}

static void seedHistory(void) {
    const char *times[] = {"09:00","10:00","11:00","12:00","13:00"};
    int idx[] = {0,0,1,0,2};
    int i;
    for (i = 0; i < 5; ++i) {
        strcpy(historyQueue[queueRear].time, times[i]);
        strcpy(historyQueue[queueRear].symbol, market[idx[i]].symbol);
        historyQueue[queueRear].price =
            (i == 0 ? 62000.0 : i == 1 ? 62500.0 : i == 2 ? 3150.0 :
             i == 3 ? 63000.0 : 144.5);
        queueRear = (queueRear + 1) % QUEUE_SIZE;
        queueCount++;
    }
}

static int watchContains(const char *name) {
    WatchNode *p = watchHead;
    while (p) {
        if (_stricmp(p->coinName, name) == 0) return 1;
        p = p->next;
    }
    return 0;
}

static void addWatch(const char *query) {
    int idx = findCoin(query);
    if (idx < 0 || watchContains(market[idx].name)) return;

    WatchNode *n = (WatchNode *)malloc(sizeof(WatchNode));
    if (!n) return;
    strcpy(n->coinName, market[idx].name);
    n->next = watchHead;
    watchHead = n;
    market[idx].watchCount++;
}

static void removeWatch(const char *name) {
    WatchNode *p = watchHead, *prev = NULL;
    while (p) {
        if (_stricmp(p->coinName, name) == 0) {
            if (prev) prev->next = p->next;
            else watchHead = p->next;

            StackNode *s = (StackNode *)malloc(sizeof(StackNode));
            if (s) {
                strcpy(s->coinName, p->coinName);
                s->next = stackTop;
                stackTop = s;
            }
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
}

static void undoRemove(void) {
    if (!stackTop) return;
    StackNode *s = stackTop;
    stackTop = s->next;
    addWatch(s->coinName);
    free(s);
}

static void addHistory(int idx) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(historyQueue[queueRear].time, sizeof(historyQueue[queueRear].time),
             "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    strcpy(historyQueue[queueRear].symbol, market[idx].symbol);
    historyQueue[queueRear].price = market[idx].price;
    queueRear = (queueRear + 1) % QUEUE_SIZE;
    if (queueCount < QUEUE_SIZE) queueCount++;
    else queueFront = (queueFront + 1) % QUEUE_SIZE;
}

static void buyCoin(const char *query, double qty, double price) {
    int idx = findCoin(query);
    if (idx < 0 || qty <= 0 || price < 0) return;

    PortNode *p = portHead;
    while (p) {
        if (_stricmp(p->coinName, market[idx].name) == 0) {
            double total = p->quantity + qty;
            p->buyPrice = (p->buyPrice * p->quantity + price * qty) / total;
            p->quantity = total;
            return;
        }
        p = p->next;
    }

    PortNode *n = (PortNode *)malloc(sizeof(PortNode));
    if (!n) return;
    strcpy(n->coinName, market[idx].name);
    n->buyPrice = price;
    n->quantity = qty;
    n->next = portHead;
    portHead = n;
}

static void sellCoin(const char *query, double qty) {
    int idx = findCoin(query);
    if (idx < 0 || qty <= 0) return;

    PortNode *p = portHead, *prev = NULL;
    while (p) {
        if (_stricmp(p->coinName, market[idx].name) == 0) {
            if (qty >= p->quantity) {
                if (prev) prev->next = p->next;
                else portHead = p->next;
                free(p);
            } else {
                p->quantity -= qty;
            }
            return;
        }
        prev = p;
        p = p->next;
    }
}

/* ---------- GUI helpers ---------- */

static void setFont(HWND h, HFONT f) {
    SendMessage(h, WM_SETFONT, (WPARAM)f, TRUE);
}

static HWND addLabel(HWND parent, const char *text, int x, int y, int w, int h,
                     HFONT font) {
    HWND hWnd = CreateWindowA("STATIC", text,
        WS_CHILD | WS_VISIBLE, x, y, w, h, parent, NULL,
        GetModuleHandle(NULL), NULL);
    setFont(hWnd, font);
    return hWnd;
}

static HWND addButton(HWND parent, const char *text, int x, int y, int w, int h, int id) {
    HWND hWnd = CreateWindowA("BUTTON", text,
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, w, h, parent, (HMENU)(INT_PTR)id,
        GetModuleHandle(NULL), NULL);
    setFont(hWnd, fButton);
    return hWnd;
}

static HWND addEdit(HWND parent, const char *text, int x, int y, int w, int h, int id) {
    HWND hWnd = CreateWindowA("EDIT", text,
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        x, y, w, h, parent, (HMENU)(INT_PTR)id,
        GetModuleHandle(NULL), NULL);
    setFont(hWnd, fNormal);
    return hWnd;
}

static void clearContent(void) {
    HWND c = GetWindow(gContent, GW_CHILD);
    while (c) {
        HWND next = GetWindow(c, GW_HWNDNEXT);
        DestroyWindow(c);
        c = next;
    }
    gSearchEdit = NULL;
    InvalidateRect(gContent, NULL, TRUE);
    UpdateWindow(gContent);
}

static void heading(const char *title, const char *subtitle) {
    addLabel(gContent, title, 28, 22, 600, 38, fTitle);
    addLabel(gContent, subtitle, 30, 61, 600, 25, fSmall);
}

static void card(const char *title, const char *value, const char *caption,
                 int x, int y, int w) {
    addLabel(gContent, title, x, y, w, 22, fSmall);
    addLabel(gContent, value, x, y + 23, w, 34, fBig);
    addLabel(gContent, caption, x, y + 59, w, 22, fSmall);
}

static void infoBox(const char *title, const char *text, int x, int y, int w, int h) {
    addLabel(gContent, title, x, y, w, 24, fNormal);
    addLabel(gContent, text, x, y + 27, w, h - 27, fSmall);
}

static void message(const char *title, const char *text) {
    MessageBoxA(gMain, text, title, MB_OK | MB_ICONINFORMATION);
}

/* ---------- Pages ---------- */

static void showDashboard(void) {
    clearContent();
    heading("Market overview", "CryptoPluse | Data Structures Dashboard");

    char watch[32];
    snprintf(watch, sizeof(watch), "%d coins", 0);
    {
        int count = 0;
        WatchNode *p = watchHead;
        while (p) { count++; p = p->next; }
        snprintf(watch, sizeof(watch), "%d", count);
    }

    char hist[32];
    snprintf(hist, sizeof(hist), "%d", queueCount);

    card("MARKET COINS", "5", "Array", 28, 105, 180);
    card("WATCHLIST", watch, "Linked List", 228, 105, 180);
    card("HISTORY", hist, "Circular Queue", 428, 105, 180);

    addLabel(gContent, "Top movers", 28, 205, 300, 30, fTitle);

    int order[MAX_COINS], i, j, t;
    for (i = 0; i < MAX_COINS; ++i) order[i] = i;
    for (i = 0; i < MAX_COINS - 1; ++i)
        for (j = 0; j < MAX_COINS - i - 1; ++j)
            if (market[order[j]].change < market[order[j+1]].change) {
                t = order[j]; order[j] = order[j+1]; order[j+1] = t;
            }

    for (i = 0; i < MAX_COINS; ++i) {
        char row[160];
        snprintf(row, sizeof(row), "%-6s    $%10.2f      %+.2f%%",
                 market[order[i]].symbol, market[order[i]].price,
                 market[order[i]].change);
        addLabel(gContent, row, 38, 248 + i * 34, 420, 27, fNormal);
    }

    infoBox("Data structures",
            "Array | Linked List | Stack | Queue | Bubble Sort\n"
            "Linear Search | Tree | Graph + BFS | Portfolio List",
            28, 440, 590, 95);
}

static void showMarket(void) {
    clearContent();
    heading("Crypto Market", "Market data stored in the Coin array");

    addLabel(gContent, "COIN", 30, 110, 180, 24, fSmall);
    addLabel(gContent, "SYMBOL", 215, 110, 90, 24, fSmall);
    addLabel(gContent, "PRICE", 325, 110, 150, 24, fSmall);
    addLabel(gContent, "24H CHANGE", 490, 110, 130, 24, fSmall);

    int i;
    for (i = 0; i < MAX_COINS; ++i) {
        char p[64], c[64];
        snprintf(p, sizeof(p), "$ %.2f", market[i].price);
        snprintf(c, sizeof(c), "%+.2f%%", market[i].change);
        addLabel(gContent, market[i].name, 30, 145 + i*43, 180, 30, fNormal);
        addLabel(gContent, market[i].symbol, 215, 145 + i*43, 90, 30, fNormal);
        addLabel(gContent, p, 325, 145 + i*43, 150, 30, fNormal);
        HWND hc = addLabel(gContent, c, 490, 145 + i*43, 130, 30, fNormal);
        (void)hc;
    }

    addButton(gContent, "Add all prices to history", 30, 380, 210, 36, 3001);
}

static void showSearch(void) {
    clearContent();
    heading("Search Coin", "Linear Search by name or symbol");

    addLabel(gContent, "Coin name or symbol", 30, 112, 180, 25, fNormal);
    gSearchEdit = addEdit(gContent, "", 30, 142, 290, 36, 3002);
    addButton(gContent, "Search", 335, 142, 110, 36, 3003);

    addLabel(gContent, "Examples: BTC, Bitcoin, ETH, Solana", 30, 190, 400, 25, fSmall);
}

static void showWatchlist(void) {
    clearContent();
    heading("Watchlist", "Linked List + Stack for Undo Remove");

    addLabel(gContent, "Coin name / symbol", 30, 108, 170, 25, fNormal);
    gSearchEdit = addEdit(gContent, "", 30, 138, 250, 36, 3010);
    addButton(gContent, "Add", 295, 138, 90, 36, 3011);
    addButton(gContent, "Undo Remove", 395, 138, 125, 36, 3012);

    addLabel(gContent, "Current watchlist", 30, 205, 250, 28, fTitle);

    int y = 245, n = 1;
    WatchNode *p = watchHead;
    if (!p) {
        addLabel(gContent, "Your watchlist is empty.", 40, y, 400, 28, fNormal);
    }
    while (p) {
        char row[100];
        snprintf(row, sizeof(row), "%d.  %s", n++, p->coinName);
        addLabel(gContent, row, 45, y, 300, 28, fNormal);
        y += 32;
        p = p->next;
    }

    addLabel(gContent, "To remove a coin, type its name/symbol and use Remove below.",
             30, 425, 560, 25, fSmall);
    addButton(gContent, "Remove typed coin", 30, 458, 160, 36, 3013);
}

static void showMovers(void) {
    clearContent();
    heading("Top Gainers & Losers", "Bubble Sort by daily percentage change");

    int order[MAX_COINS], i, j, t;
    for (i = 0; i < MAX_COINS; ++i) order[i] = i;
    for (i = 0; i < MAX_COINS - 1; ++i)
        for (j = 0; j < MAX_COINS - i - 1; ++j)
            if (market[order[j]].change < market[order[j+1]].change) {
                t = order[j]; order[j] = order[j+1]; order[j+1] = t;
            }

    for (i = 0; i < MAX_COINS; ++i) {
        char row[150];
        snprintf(row, sizeof(row), "%d.  %-8s    $%10.2f    %+.2f%%",
                 i + 1, market[order[i]].symbol, market[order[i]].price,
                 market[order[i]].change);
        addLabel(gContent, row, 40, 120 + i*45, 520, 30, fNormal);
    }
}

static void showHistory(void) {
    clearContent();
    heading("Price History", "Circular Queue - last 10 updates");

    if (queueCount == 0) {
        addLabel(gContent, "No price history yet.", 35, 120, 400, 30, fNormal);
    } else {
        int i;
        for (i = 0; i < queueCount; ++i) {
            int idx = (queueFront + i) % QUEUE_SIZE;
            char row[140];
            snprintf(row, sizeof(row), "%-10s   %-6s   $ %.2f",
                     historyQueue[idx].time,
                     historyQueue[idx].symbol,
                     historyQueue[idx].price);
            addLabel(gContent, row, 40, 115 + i*32, 480, 27, fNormal);
        }
    }

    addButton(gContent, "Refresh / Add current prices", 30, 470, 220, 36, 3020);
}

static void showAlerts(void) {
    clearContent();
    heading("Price Alerts", "Set a target price for a market coin");

    addLabel(gContent, "Symbol", 30, 112, 70, 25, fNormal);
    gSearchEdit = addEdit(gContent, "BTC", 100, 108, 110, 36, 3021);

    addLabel(gContent, "Target price", 230, 112, 100, 25, fNormal);
    addEdit(gContent, "63000", 330, 108, 130, 36, 3022);
    addButton(gContent, "Set Alert", 475, 108, 110, 36, 3023);

    addLabel(gContent, "Saved alerts", 30, 185, 250, 28, fTitle);
    int y = 225, i;
    for (i = 0; i < alertCount; ++i) {
        char row[100];
        snprintf(row, sizeof(row), "%s  >=  $%.2f",
                 alerts[i].symbol, alerts[i].target);
        addLabel(gContent, row, 45, y, 300, 28, fNormal);
        y += 32;
    }
    if (alertCount == 0)
        addLabel(gContent, "No alerts set yet.", 45, y, 300, 28, fSmall);
}

static void showTrending(void) {
    clearContent();
    heading("Trending Coins", "Based on search and watch counters");

    int i;
    for (i = 0; i < MAX_COINS; ++i) {
        char row[150];
        snprintf(row, sizeof(row), "%-6s    Searches: %-3d    Watches: %-3d",
                 market[i].symbol, market[i].searchCount, market[i].watchCount);
        addLabel(gContent, row, 40, 120 + i*42, 500, 30, fNormal);
    }
}

static void showCategories(void) {
    clearContent();
    heading("Crypto Categories", "Static Tree representation");

    addLabel(gContent, "Cryptocurrency", 35, 120, 300, 30, fBig);
    addLabel(gContent, "|-- Layer 1 Coins", 55, 165, 300, 27, fNormal);
    addLabel(gContent, "|   |-- Bitcoin (BTC)", 75, 195, 300, 27, fNormal);
    addLabel(gContent, "|   |-- Ethereum (ETH)", 75, 225, 300, 27, fNormal);
    addLabel(gContent, "|   `-- Solana (SOL)", 75, 255, 300, 27, fNormal);
    addLabel(gContent, "|-- Meme Coins", 55, 295, 300, 27, fNormal);
    addLabel(gContent, "|   `-- Dogecoin (DOGE)", 75, 325, 300, 27, fNormal);
    addLabel(gContent, "|-- Payment Coins", 55, 365, 300, 27, fNormal);
    addLabel(gContent, "|   `-- XRP", 75, 395, 300, 27, fNormal);
    addLabel(gContent, "`-- AI Coins", 55, 435, 300, 27, fNormal);
    addLabel(gContent, "    |-- Fetch.ai (FET)", 75, 465, 300, 27, fNormal);
    addLabel(gContent, "    `-- Render (RNDR)", 75, 495, 300, 27, fNormal);
}

static void showGraph(void) {
    clearContent();
    heading("Market Graph + BFS", "Directed graph using an adjacency matrix");

    addLabel(gContent, "BTC  ->  ETH  ->  SOL  ->  DOGE  ->  XRP  ->  BTC",
             35, 120, 600, 30, fBig);
    addLabel(gContent, "BFS traversal from Bitcoin:", 35, 185, 350, 28, fNormal);
    addLabel(gContent, "Bitcoin -> Ethereum -> Solana -> Dogecoin -> XRP",
             55, 220, 560, 28, fNormal);

    addLabel(gContent, "Adjacency Matrix", 35, 285, 250, 28, fTitle);
    int i, j, y = 325;
    for (i = 0; i < MAX_COINS; ++i) {
        char row[100];
        snprintf(row, sizeof(row), "%s :  %d  %d  %d  %d  %d",
                 market[i].symbol, graph[i][0], graph[i][1],
                 graph[i][2], graph[i][3], graph[i][4]);
        addLabel(gContent, row, 50, y, 420, 26, fNormal);
        y += 28;
    }
}

static void showPortfolio(void) {
    clearContent();
    heading("Portfolio", "Linked List holdings and profit / loss");

    addButton(gContent, "Buy BTC", 30, 108, 100, 36, 3040);
    addButton(gContent, "Buy ETH", 140, 108, 100, 36, 3041);
    addButton(gContent, "Sell BTC", 250, 108, 100, 36, 3042);

    addLabel(gContent, "Holdings", 30, 175, 250, 28, fTitle);

    int y = 215;
    double total = 0.0;
    PortNode *p = portHead;

    if (!p)
        addLabel(gContent, "Portfolio is empty.", 40, y, 400, 28, fNormal);

    while (p) {
        int idx = findCoin(p->coinName);
        double pl = 0.0;
        if (idx >= 0) pl = (market[idx].price - p->buyPrice) * p->quantity;
        total += pl;

        char row[180];
        snprintf(row, sizeof(row), "%-10s  Qty %.4f   Buy $%.2f   P&L $%+.2f",
                 p->coinName, p->quantity, p->buyPrice, pl);
        addLabel(gContent, row, 40, y, 580, 28, fNormal);
        y += 34;
        p = p->next;
    }

    char totalText[80];
    snprintf(totalText, sizeof(totalText), "Total P&L: $%+.2f", total);
    addLabel(gContent, totalText, 40, y + 20, 350, 32, fBig);
}

static void refreshPage(void) {
    switch (currentPage) {
        case 0: showDashboard(); break;
        case 1: showMarket(); break;
        case 2: showSearch(); break;
        case 3: showWatchlist(); break;
        case 4: showMovers(); break;
        case 5: showHistory(); break;
        case 6: showAlerts(); break;
        case 7: showTrending(); break;
        case 8: showCategories(); break;
        case 9: showGraph(); break;
        case 10: showPortfolio(); break;
    }
}


/* The content area is a child window. Its buttons/edit boxes send
   WM_COMMAND to their immediate parent, so forward those messages
   to the main window where the page actions are handled. */
static LRESULT CALLBACK ContentProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_COMMAND:
            if (gMain) SendMessageA(gMain, WM_COMMAND, wp, lp);
            return 0;
        case WM_CTLCOLORSTATIC: {
            HDC dc = (HDC)wp;
            SetTextColor(dc, CLR_TEXT);
            SetBkMode(dc, TRANSPARENT);
            SetBkColor(dc, CLR_PANEL);
            return (LRESULT)bPanel;
        }
        case WM_CTLCOLOREDIT: {
            HDC dc = (HDC)wp;
            SetTextColor(dc, CLR_TEXT);
            SetBkColor(dc, CLR_INPUT);
            return (LRESULT)bEdit;
        }
        case WM_ERASEBKGND: {
            RECT r;
            GetClientRect(hwnd, &r);
            FillRect((HDC)wp, &r, bPanel);
            return 1;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hwnd, &ps);
            RECT r;
            GetClientRect(hwnd, &r);
            FillRect(dc, &r, bPanel);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProcA(hwnd, msg, wp, lp);
}

/* ---------- Sidebar ---------- */

static void sidebarButton(const char *text, int y, int id) {
    addButton(gMain, text, 18, y, 178, 35, id);
}

static void createUI(HWND hwnd) {
    gMain = hwnd;

    addLabel(hwnd, "CRYPTOPLUSE", 22, 20, 185, 32, fTitle);
    addLabel(hwnd, "CRYPTO ANALYTICS", 22, 52, 185, 20, fSmall);

    const char *items[] = {
        "Dashboard", "Market", "Search", "Watchlist", "Gainers / Losers",
        "Price History", "Price Alerts", "Trending", "Categories",
        "Graph + BFS", "Portfolio"
    };
    int i;
    for (i = 0; i < 11; ++i)
        sidebarButton(items[i], 92 + i * 41, 2000 + i);

    addButton(hwnd, "Refresh Data", 18, 565, 178, 35, 2015);

    gContent = CreateWindowA("CryptoContent", "",
        WS_CHILD | WS_VISIBLE, 215, 15, 665, 600,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    setFont(gContent, fNormal);

    seedHistory();
    refreshPage();
}

/* ---------- Windows message handling ---------- */

static void getText(HWND h, char *out, int size) {
    if (!h) { out[0] = '\0'; return; }
    GetWindowTextA(h, out, size);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            createUI(hwnd);
            return 0;

        case WM_COMMAND: {
            int id = LOWORD(wp);

            if (id >= 2000 && id <= 2010) {
                currentPage = id - 2000;
                refreshPage();
                return 0;
            }

            if (id == 2015) {
                int i;
                for (i = 0; i < MAX_COINS; ++i) addHistory(i);
                refreshPage();
                return 0;
            }

            if (id == 3001) {
                int i;
                for (i = 0; i < MAX_COINS; ++i) addHistory(i);
                refreshPage();
                return 0;
            }

            if (id == 3003) {
                char q[64], text[220];
                getText(gSearchEdit, q, sizeof(q));
                int idx = findCoin(q);
                if (idx < 0) {
                    message("Search", "Coin not found. Try BTC, Bitcoin, ETH, Solana, etc.");
                } else {
                    market[idx].searchCount++;
                    snprintf(text, sizeof(text),
                        "%s (%s)\n\nPrice: $%.2f\n24H Change: %+.2f%%\n\nLinear Search: FOUND",
                        market[idx].name, market[idx].symbol,
                        market[idx].price, market[idx].change);
                    message("Coin Found", text);
                    refreshPage();
                }
                return 0;
            }

            if (id == 3011) {
                char q[64];
                getText(gSearchEdit, q, sizeof(q));
                if (findCoin(q) < 0) message("Watchlist", "Coin not found.");
                else { addWatch(q); refreshPage(); }
                return 0;
            }

            if (id == 3012) {
                undoRemove();
                refreshPage();
                return 0;
            }

            if (id == 3013) {
                char q[64];
                getText(gSearchEdit, q, sizeof(q));
                if (findCoin(q) < 0) message("Watchlist", "Coin not found.");
                else { removeWatch(q); refreshPage(); }
                return 0;
            }

            if (id == 3020) {
                int i;
                for (i = 0; i < MAX_COINS; ++i) addHistory(i);
                refreshPage();
                return 0;
            }

            if (id == 3023) {
                char sym[32], targetText[64], msgText[180];
                HWND targetEdit = GetDlgItem(gContent, 3022);
                getText(gSearchEdit, sym, sizeof(sym));
                getText(targetEdit, targetText, sizeof(targetText));
                int idx = findCoin(sym);

                if (idx < 0) {
                    message("Price Alert", "Coin not found.");
                } else if (alertCount >= MAX_ALERTS) {
                    message("Price Alert", "Alert limit reached.");
                } else {
                    double target = atof(targetText);
                    strcpy(alerts[alertCount].symbol, market[idx].symbol);
                    alerts[alertCount].target = target;
                    alertCount++;

                    if (market[idx].price >= target)
                        snprintf(msgText, sizeof(msgText),
                            "ALERT TRIGGERED!\n%s is already at $%.2f.",
                            market[idx].symbol, market[idx].price);
                    else
                        snprintf(msgText, sizeof(msgText),
                            "Alert saved.\nNotify when %s reaches $%.2f.",
                            market[idx].symbol, target);

                    message("Price Alert", msgText);
                    refreshPage();
                }
                return 0;
            }

            if (id == 3040) {
                buyCoin("BTC", 0.05, market[0].price);
                refreshPage();
                return 0;
            }

            if (id == 3041) {
                buyCoin("ETH", 1.0, market[1].price);
                refreshPage();
                return 0;
            }

            if (id == 3042) {
                sellCoin("BTC", 0.02);
                refreshPage();
                return 0;
            }
        } break;

        case WM_CTLCOLORSTATIC: {
            HDC dc = (HDC)wp;
            SetTextColor(dc, CLR_TEXT);
            SetBkMode(dc, TRANSPARENT);
            SetBkColor(dc, CLR_PANEL);
            return (LRESULT)bPanel;
        }

        case WM_CTLCOLOREDIT: {
            HDC dc = (HDC)wp;
            SetTextColor(dc, CLR_TEXT);
            SetBkColor(dc, CLR_INPUT);
            return (LRESULT)bEdit;
        }

        case WM_CTLCOLORBTN: {
            HDC dc = (HDC)wp;
            SetTextColor(dc, CLR_WHITE);
            SetBkColor(dc, CLR_SIDEBAR);
            return (LRESULT)bSidebar;
        }

        case WM_ERASEBKGND: {
            RECT r;
            GetClientRect(hwnd, &r);
            FillRect((HDC)wp, &r, bWindow);
            return 1;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcA(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmd, int show) {
    (void)hPrev;
    (void)cmd;

    fNormal = CreateFontA(16,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
                          OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
                          DEFAULT_PITCH|FF_DONTCARE,"Segoe UI");
    fSmall = CreateFontA(13,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
                         OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
                         DEFAULT_PITCH|FF_DONTCARE,"Segoe UI");
    fTitle = CreateFontA(23,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
                         OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
                         DEFAULT_PITCH|FF_DONTCARE,"Segoe UI");
    fBig = CreateFontA(21,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
                       OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
                       DEFAULT_PITCH|FF_DONTCARE,"Segoe UI");
    fButton = CreateFontA(15,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
                          OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
                          DEFAULT_PITCH|FF_DONTCARE,"Segoe UI");

    bWindow = CreateSolidBrush(CLR_BG);
    bPanel = CreateSolidBrush(CLR_PANEL);
    bSidebar = CreateSolidBrush(CLR_SIDEBAR);
    bEdit = CreateSolidBrush(CLR_INPUT);

    WNDCLASSA wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "CryptoPluseGUI";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = bWindow;

    RegisterClassA(&wc);

    WNDCLASSA cc;
    ZeroMemory(&cc, sizeof(cc));
    cc.lpfnWndProc = ContentProc;
    cc.hInstance = hInst;
    cc.lpszClassName = "CryptoContent";
    cc.hCursor = LoadCursor(NULL, IDC_ARROW);
    cc.hbrBackground = bPanel;
    RegisterClassA(&cc);

    gMain = CreateWindowA(
        "CryptoPluseGUI",
        "CryptoPluse - Crypto Dashboard",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 920, 660,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(gMain, show);
    UpdateWindow(gMain);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
