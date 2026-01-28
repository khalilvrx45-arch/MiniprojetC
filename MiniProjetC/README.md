# Pokemon Factory Management System

## Description
This is a C-based management system for a Pokemon figurine factory. It manages:
- Pokemon types and their properties
- Production machines
- Clients
- Orders (Commandes)

## Files Included
- **main.c** - Main program with menu system
- **structures.h/c** - Data structures and type definitions
- **pokemon.h/c** - Pokemon management functions
- **machine.h/c** - Machine management functions
- **client.h/c** - Client management functions
- **commande.h/c** - Order management functions
- **MiniProjetC.cbp** - CodeBlocks project file

## How to Open in Code::Blocks

### Method 1: Double-click
1. Extract all files to a folder
2. Double-click on **MiniProjetC.cbp**
3. Code::Blocks will open with all files loaded

### Method 2: From Code::Blocks
1. Open Code::Blocks
2. File → Open → Select **MiniProjetC.cbp**
3. All source files will be loaded automatically

## How to Compile and Run

1. **Build the project**: Press **Ctrl + F9** or click Build → Build
2. **Run the program**: Press **Ctrl + F10** or click Build → Run
3. **Build & Run together**: Press **F9**

## System Requirements
- Code::Blocks IDE (with GCC compiler)
- Windows, Linux, or macOS

## Usage
The program provides a menu-driven interface to:
1. Manage Pokemon (Add/View/Modify/Delete)
2. Manage Machines (Add/View/Modify/Delete)
3. Manage Clients (Add/View/Modify/Delete)
4. Manage Orders (Add/View/Modify/Delete)
5. Display factory status
6. Exit

## Notes
- All data is stored in memory (not persistent)
- Auto-incrementing IDs for Pokemon, Machines, and Orders
- French language interface
- Some French characters (é, è, ç) may display incorrectly depending on your system locale

## Author
Generated for academic/learning purposes
