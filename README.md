# Paint

- A simple bitmap painting program built with Qt. 
- Can be compiled to an executable on platform of choice (tested on Windows 7/8 and  Ubuntu 18.04.1 LTS)

## Features: 

- Save and load .bmp files. 
- Stack-based undo-redo which can store up to 100 actions.
- Change background and foreground colors
- Fill image with a background color
- Resize image
- Pen tool with 3 different caps
- Line tool with 5 styles, 3 caps, and a poly mode (when active, drag to continuously draw connected lines)
- Rectangle tool with 5 styles, 3 shapes, and ability to fill or draw border only
- Eraser tool
- Can adjust thickness for all tools

![alt-text](https://i.imgur.com/IzC44vr.png "Paint")

# Build Instructions:

- Open the bitmap.pro file using Qt Creator and build using the default settings. Qt will take care of the rest!

    Version used: 4.2.1
