
# Stock Market Client-Server Application

This project implements a networked client-server application where the client can query a server to fetch stock market information for two major stocks: **TESLA (TSLA)** and **Microsoft (MSFT)**. The server processes commands related to stock prices and provides responses to the client. The data is based on historical stock prices available in CSV files.

## Features

- **List**: Displays the available stock (TSLA and MSFT).
- **Prices**: Displays the stock prices for a given stock on a specific date.
- **MaxProfit**: Calculates the maximum possible profit from buying and selling stock within a specified date range.
- **Quit**: Terminates the client-server interaction.

## Components

The project consists of two components:
- **Client**: A program that sends requests to the server and displays the responses.
- **Server**: A program that listens for client requests, processes the commands, and returns the requested information.

## Setup

### 1. Server

To start the server, run the following command in your terminal:

```bash
./server MSFT.csv TSLA.csv 30000
```

- The first argument (`MSFT.csv`) is the stock data file for Microsoft.
- The second argument (`TSLA.csv`) is the stock data file for Tesla.
- The third argument (`30000`) is the port number where the server will listen for incoming requests.

Once the server is started, it will read the stock data from the CSV files and wait for client requests. The server will print "server started" and then listen for connections.

### 2. Client

To start the client, run the following command:

```bash
./client localhost 30000
```

- The first argument (`localhost`) is the domain name or IP address of the server.
- The second argument (`30000`) is the port number on which the server is listening.

Once the client is started, it will display a prompt (`> `) where the user can enter commands.

## Commands

The client supports the following commands:

### 1. **List**

Displays the available stocks:

```bash
> List
TSLA | MSFT
```

### 2. **Prices**

Displays the stock price for a given stock on a specific date:

```bash
> Prices MSFT 2021-11-08
269.36
> Prices TSLA 2021-11-09
187.66
```

### 3. **MaxProfit**

Calculates the maximum possible profit from buying and selling stock within a given date range:

```bash
> MaxProfit MSFT 2021-11-04 2021-11-17
29.92
> MaxProfit TSLA 2021-11-04 2021-11-17
6.34
```

### 4. **Quit**

Terminates the client and server interaction:

```bash
> quit
```

## Message Format

Each message sent between the client and the server contains:
- Byte 0: The length of the string (n)
- Bytes 1 – n: The characters of the string

Messages from the client contain a query command, and messages from the server contain the requested stock information or calculated profit.

## Handling Invalid Commands

If the user enters an invalid command or incorrect syntax, the program will respond with:

```bash
Invalid syntax
```

## File Formats

The server reads data from two CSV files: `MSFT.csv` for Microsoft and `TSLA.csv` for Tesla. Each file contains the following columns:
- **Date** (YYYY-MM-DD)
- **Closing Price** (represented as the fifth column in the CSV)

When querying for stock prices, the server returns the closing price for the specified date. If the date is not available, the server will respond with "Unknown".

## Notes

- The client and server can run on the same or different machines, provided they can communicate over the specified port.
- Ensure that the server has the required CSV files (`MSFT.csv` and `TSLA.csv`) before starting.
- The date format for commands should be `YYYY-MM-DD`.
