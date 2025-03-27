# Grapher
## Features
- Drawing and editing any simple graph/FSM
- Saving and Loading to files
- Export to image and vector graphic

## Keyboard Shortcuts
Editing
- `CTRL-LMB` Create new vertex with edges from selected vertices
- `CTRL-X`   Destroy selected vertices
- `CTRL-I`   Insert text in selected vertex
- `H`   Hide selected vertices
- `J`   Join 2 selected vertices
- `X`   Disconnect 2 selected vertices

File Manipulation
- `CTRL-S` open 'Save as' dialog
- `CTRL-E` open 'Export as' dialog

## How to build
All source files are in `src` directory, including a PCH. Use C++17. Only external dependency is SFML 2.6. `build.json` for mscmp is configured
to work with SFML 2.6 for msvc (static variant, see EmVance1/mscmp).

