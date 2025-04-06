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

<pre> Least Squares Linear Regression: 
We want to find the weight vector \\( \theta \\) that minimizes the squared error between predicted and actual outputs. 
Loss Function: - \\( X \\in \mathbb{R}^{m \times n} \\): design matrix (with bias term added) - \\( y \\in \mathbb{R}^{m \times 1} \\): target output - \\( \theta \in \mathbb{R}^{n \times 1} \\): parameter vector We define the **least squares loss** as: $$ J(\theta) = \| X\theta - y \|^2 = (X\theta - y)^T (X\theta - y) $$ ### Taking the Gradient To minimize the loss, we take the gradient with respect to \\( \theta \\): $$ \nabla_\theta J(\theta) = 2 X^T (X\theta - y) $$ ### Solving for \\( \theta \\) Set the gradient to zero: $$ X^T (X\theta - y) = 0 $$ Distribute: $$ X^T X \theta = X^T y $$ Finally, solve for \\( \theta \\): $$ \theta = (X^T X)^{-1} X^T y $$ This is the **closed-form solution** to linear regression using the **normal equation**. </pre>

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




