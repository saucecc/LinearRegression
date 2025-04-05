## INTRODUCTION ##

The goal of this project is to implement Linear Regression from scratch in C++.

The motivation was to gain a more comprehensive understanding of the linear algebra behind regression, and how it is implemented.

With this goal in mind, all matrix operations were implemented from scratch (see src/cpp/matrix.cc); these include: 
    - **transposing a matrix**
    - **inverting a matrix (implemented via gaussian eimination w/ partial pivoting)**
    - 

I went with the traditional approach of solving LR (least squares) rather than gradient descent

## DESIGN DECISIONS ##

Visualization: After the lr is done within the C++ code, I will use Python to visualize the output 

This was done for ease, as visualizing in C++ can be tricky and the Python libraries for this are better


## REQUIREMENTS ##

Requirements: Python 3 installed, g++ to compile the c++ code

If you do not have these installed, do the following: 
    WINDOWS: 
        - python3: Install from https://www.python.org/downloads/
        - g++ and make: 
            - the (easier) option is to just use WSL. But if you do not have this, we can do: 
            - Install https://www.msys2.org/
                - download the .exe and install 
                - to install g++ and make, in the MSYS terminal run: 
                    pacman -Syu 
                    pacman -S make gcc 
                - Now you can use this terminal to run make and compile the C++ code

    MAC: 
        - python3: brew install python
        - g++: xcode-select --install (installs g++ and make)

pip should be installed with python 3. run: 
pip install -r python/requirements.txt 
    - This is just to install the python libraries for visualiziation

Now we should be all good to run the project! 

## BUILD DIRECTIONS ##




