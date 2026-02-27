# 🏭 Pokemon Factory Management System

## 🌟 Introduction
The **Pokemon Factory** is a robust industrial management simulation built in C. It provides a real-time dashboard to manage the entire lifecycle of Pokemon figurine production, from client order reception to automated machine manufacturing and financial tracking.

## 🏗️ Technical Architecture

### Core Data Structures
The project relies on a deeply linked architecture for real-time consistency:
- **`Pokemon`**: Defines the product (Name, Type, Unit Cost).
- **`Client`**: Tracks loyalty and purchase history.
- **`Machine`**: The heart of the factory (Production Rate, Wear/Maintenance levels, Queue Management).
- **`Commande` (Order)**: Links Client and Product with real-time status tracking (`EN ATTENTE`, `PLANIFIEE`, `EN COURS`, `REALISEE`).

### Key Technologies
- **Language**: C11
- **Graphics**: Raylib + Raygui (Custom geometric dark-theme design)
- **Data Persistence**: Flat-file system (`.txt`) with custom parsers for state restoration.

## 🚀 Main Features

### 1. Smart Industrial Queue
- **Predictive Scheduling**: The system automatically queues orders if machines are busy.
- **Dynamic Estimates**: Real-time "Time to completion" countdowns that account for both production time and upcoming maintenance cycles.

### 2. Automated Maintenance System
- **Wear Tracking**: Machines track wear per figurine produced.
- **Auto-Maintenance**: Machines automatically enter a maintenance state when wear thresholds are met.
- **Priority Logic**: Maintenance cycles are integrated into the time estimations for queued orders.

### 3. Business Analytics Dashboard
- **Financial KPIs**: Real-time tracking of Factory Funds.
- **Performance Analysis**: Automatic identification of the "Top Pokemon", "Premium Client", and "Elite Machine" based on sales and productivity data.

## 📁 File Structure
- `main.c`: Entry point and main simulation triggers.
- `gui_main.c`: Raylib-based UI implementation and interaction logic.
- `machine.c/h`: Production logic, simulation, and maintenance.
- `commande.c/h`: Order lifecycle and status management.
- `fichiers.c/h`: Data persistence (Save/Load system).
- `stats.c/h`: Data aggregation and analytics.
