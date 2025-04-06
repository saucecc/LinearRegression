# INTRODUCTION #

The goal of this project is to implement Linear Regression from scratch in C++.

The motivation was to gain a more comprehensive understanding of the linear algebra behind regression, and how these algorithms are implemented in practice.
I included concepts from each chapter of MATH-3130

## MATRIX OPERATIONS ##

With this goal in mind, all matrix operations were implemented from scratch (see src/cpp/matrix.cc); these include: 

    * **transposing a matrix**
    * **inverting a matrix (implemented via gaussian eimination w/ partial pivoting)**
    * **multiplying two matrices**
    * **matrix arithmetic**

I went with the traditional approach of solving LR (least squares) rather than gradient descent. 
I extensively commented my code in hopes of making it as readable as possible and ensuring the logic is clear. 
I also included the time complexity of my implementations for consideration

## LINEAR REGRESSION ## 
See src/cpp/linear_regression.cpp;

Least Squares Linear Regression: 
    In class, Least Squares was presented as the element-wise minimum; but in matrix form our goal is to minimize this loss:
        - our loss: $$L(D, \theta) = ||Y-X\theta||^{2} = (Y-X\theta)^{T}(Y-X\theta)$$
            = $$Y^{T}Y-Y^{T}X\theta - (theta)^{T}XY + (theta)^{T}X^{T}X\theta$$
            = $$Y^{T}Y-X^{T}Y\theta - X^{T}Y\theta+X^{T}X\theta^{2}$$
        Taking the gradient: 
            $$\frac{\partial L(D, \theta)}{\partial \theta} = \frac{\partial(Y^{T}Y-X^{T}Y\theta-X^{T}Y\theta + X^{T}X\theta^{2})}{\partial \theta}$$
            $$=2X^{T}Y+2X^{T}X\theta$$
            We know that to minmize, we set this to 0. So; 
                $$2X^{T}Y+2X^{T}X\theta=0\implies X^{T}Y=X^{T}X\theta$$
                $$\implies \hat{\theta} = (X^{T}X)^{-1}X^{T}Y$$

$$ X^{T}X $$ 

# DESIGN DECISIONS #

Visualization: After the lr is done within the C++ code, I will use Python to visualize the output 

This was done for ease, as visualizing in C++ can be tricky and the Python libraries for this are better

# TESTING & DATASETS #
To test the matrix class that I wrote, I wrote an additional file (src/cpp/matrixtest.cc) that 


# REQUIREMENTS #

Requirements: Python 3 installed, g++ to compile the c++ code

If you do not have these installed, do the following: 
    WINDOWS: 
        * python3: Install from https://www.python.org/downloads/
        * g++ and make: 
            * the (easier) option is to just use WSL. But if you do not have this, we can do: 
            * Install https://www.msys2.org/
                * download the .exe and install 
                * to install g++ and make, in the MSYS terminal run: 
                    pacman -Syu 
                    pacman -S make gcc 
                * Now you can use this terminal to run make and compile the C++ code

    MAC: 
        * python3: brew install python
        * g++: xcode-select --install (installs g++ and make)

pip should be installed with python 3. run: 
pip install -r python/requirements.txt 
    * This is just to install the python libraries for visualiziation

Now we should be all good to run the project! 

## BUILD DIRECTIONS ##




