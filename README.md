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


NOTES: 

* **Singularity**: This was a big concern going into the project. In the dataset, I dropped columns that had empty values, and ensured that there
were no 0 values. One challenge was one-hot encoding (for the Cali housing dataset, turning all the ocean proximity options into binary columns [0,1]). 
This implicitly made the matrix $$X^{T}X$$ singular since these columns are **linearly independent** by nature. To combat this, the first one-hot encoded column (<1HR from the ocean) was dropped. This stops the columns from being linearly independent and ensures the matrix is not singular. 

* The one-hot encoding code can be found in src/python/one-hot-encoding.py

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


# DESIGN DECISIONS #

* dataset class: src/cpp/dataset.cpp: This class is responsible for reading data from the csv, separating the labels from the data, and making a train/test split. The train/test split ratio is passed into the dataset constructor. It also takes an option to scale the data using min/max scaling 
    - The option for min/max scaling was inlcuded so that large features do not dominate the regression problem; this code is within matrix.cpp

Visualization: After the lr is done within the C++ code, I will use Python to visualize the output 

This was done for ease, as visualizing in C++ can be tricky and the Python libraries for this are better

# TESTING & DATASETS #

* California Housing Dataset: 
 - https://www.kaggle.com/datasets/camnugent/california-housing-prices
 - ~ 20,000 rows
 - one-hot encoded (only for proximity to ocean)



## TESTING ## 
To test the matrix class that I wrote, I wrote an additional file (src/cpp/matrixtest.cc) that tests the matrix operations
Given that these are correct, we know that the linear regression implementation will work, since it just relies on the matrix operations. 
However I have included a toy example for linear regression as well, to verify correctness

# DEBUGGING & CHALLENGES # 
I had to write extensive test cases to get everythign working properly. Working with such a large dataset with such a wide range of values and using one-hot encoding resulted in numerical instability. The main way that this was fixed was via **ridge regression**. 

I ended up using $$\theta = (X^{T}X+\lambda I)^{-1}X^{T}y$$, with very small $$\lambda \approx 1e-3)$$. This solved the issue for me for smaller test cases, but it was still a struggle on larger ones. Perhaps LU factorization or a better inversion algorithm would have helped. 

## DATASETS ## 
California Housing Prices:  
    - Source: https://www.kaggle.com/datasets/camnugent/california-housing-prices
    - data/input/cali-housing.csv
    
From dataset page:  
1. longitude: A measure of how far west a house is; a higher value is farther west  
2. latitude: A measure of how far north a house is; a higher value is farther north  
3. housingMedianAge: Median age of a house within a block; a lower number is a newer building  
4. totalRooms: Total number of rooms within a block  
5. totalBedrooms: Total number of bedrooms within a block  
6. population: Total number of people residing within a block  
7. households: Total number of households, a group of people residing within a home unit, for a block  
8. medianIncome: Median income for households within a block of houses (measured in tens of thousands of US Dollars)  
9. medianHouseValue: Median house value for households within a block (measured in US Dollars)  
10. oceanProximity: Location of the house w.r.t ocean/sea  


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

./bin/main - Run the main code, which does linear regression on three different datatsets, the output of which can be seen in the terminal
./bin/test - Run the matrix test cases, along with the simple linear regression example





