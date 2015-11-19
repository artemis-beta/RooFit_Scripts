# RooFit Basic Demo

A collection of programs which are designed to show the simple use of the ROOT RooFit package in the generation and fitting of data sets. Such data sets are modelled using Probability Density Functions (PDFs) which are passed variables.

## First RooFit Toy

The first demonstration features how to create a basic decay model using the sum of a Gaussian PDF and Exponential PDF. We firstly define two such PDFs to form a model for the generation of a set data which will then form the basis for the "Toy". All functions within RooFit, unlike standard functions within C++, require a special type of variable declared using `RooRealVar`, which takes either a single numeric argument if constant, or a range if required to be dynamic (e.g. in the case of fitting). A typical RooRealVar can be declared in the following way:

`RooRealVar variable("root_variable_name", "Description of Variable", start_val, min_val, max_val);`

Note in the case of the generating PDFs used within this example we do not need to specify a range as we want the value to be constant. 

The PDFs themselves are all constructed in a similar way, the number of arguments depending on the number of parameters required to generate the function. For example, for a `RooGaussian` variables are required for the mean and standard deviation.
