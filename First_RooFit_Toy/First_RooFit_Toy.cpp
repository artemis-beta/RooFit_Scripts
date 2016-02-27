//******************************************************************************************
//
//			RooFit Easy Intro to Generation & Fitting
//				Kristian Zarebski, 2015
//
//	Aims to introduce an example for the generation and fitting of data sets using
//	the Probability Distribution Functions (PDFs) provided within CERN's ROOT Analysis
//	framework. A simple model featuring Gaussian and Exponential PDFs combined.
//
//******************************************************************************************

#include <iostream>

// All of the functions included in RooFit are contained in their own header file

#include <RooAddPdf.h>		// Allows addition of PDFs
#include <RooGaussian.h>	// Gaussian type PDF
#include "RooExponential.h"	// Exponential type PDF
#include "RooDataSet.h"		// Header for RooFit data type
#include "RooPlot.h"		// Construction of plots from RooFit results
#include <RooRealVar.h>		// Header for creation of RooFit fitting variables

#include "TCanvas.h"		// Allows the creation of a canvas from which graphs
				// can be exported

using namespace RooFit;

int main(void){

	// Need to first define a variable for the data itself, in this case Invariant Mass
	// in this declaration we only need to specify a range to be fitted, if the data was
 	// imported data would hence be cut, only values between these limits being recorded

	RooRealVar inv_mass("inv_mass", "Invariant Mass", 400, 600);


	// For fitting additional variables need to be created, in the case of a Gaussian
	// Gaussian distribution we require a value for the mean and sigma and for the
	// the Exponential PDF we require an exponent constant.

	// RooRealVar variable("root_variable_name", "Description  of Variable", start value, min_val, max_val);

	// PDFs for data generation can have constant values (no range being specified)

	RooRealVar gen_gaus_mean("gen_gaus_mean", "Generator Gaussian Mean", 493.6);
	RooRealVar gen_gaus_sigma("gen_gaus_sigma", "Generator Gaussian Sigma", 10.1);
	
	RooRealVar gen_exp_const("gen_exp_const", "Generator Exponential Constant", 0.05);

	RooGaussian gen_gaus("gen_gaus", "Generator Gaussian", inv_mass, gen_gaus_mean, gen_gaus_sigma);
	RooExponential gen_exp("gen_exp", "Generator Exponential", inv_mass, gen_exp_const);
	
	// We now need to specify the fraction of each PDF to be used during the data set generation
	// this follows the form of frac1*PDF_1+frac2*PDF_2+...+fracN*(1-frac1-frac2-...-frac[N-1])
	// hence we require N-1 RooRealVars

	RooRealVar gen_pdf_frac("gen_pdf_frac", "Generator PDF Fraction", 0.3);

	// We now combine the two PDFs into a single function using a RooAddPdf type, the arguments for this:
	//	
	// RooAddPdf add_pdf("root_var_name", "Description of Add PDF", PDF_1, PDF_2, fraction);
	//
	// Note only a maximum of two PDFs may be declared in a RooAddPdf in the above manner, we will in another
	// example later on how to combine above two PDFs including the relevant fractions 

	RooAddPdf gen_pdf("gen_pdf", "Generator PDF", gen_gaus, gen_exp, gen_pdf_frac);

	// A data set can now be created from the final PDF using the RooDataSet type and the "generate" function
	// the arguments here being the variable to generate and the number of data to be generated

	RooDataSet* data_set = gen_pdf.generate(inv_mass, 10000);


	// When it comes to fitting data we repeat the above, initialising variables relevant to the PDFs being
	// used, however now we specify a range. RooFit will aim to fit the final PDF keeping to these ranges,
	// if the range is insufficient the user is presented with a warning informing them that one of the
	// the variables has reached a limit (e.g. in the example below fit_gaus_mean may reach the limit
	// "fit_gaus_mean = 4.7000E+02" meaning the true fit value lies above the upper bound (hence the range
	// needs extending before re-running the fit)

	RooRealVar fit_gaus_mean("fit_gaus_mean", "Fitter Gaussian Mean", 470, 450, 550);
	RooRealVar fit_gaus_sigma("fit_gaus_sigma", "Fitter Gaussian Sigma", 12., 7., 15.);
	
	RooRealVar fit_exp_const("fit_exp_const", "Fitter Exponential Constant", 0.05, 0.001, 0.09);
	
	RooGaussian fit_gaus("fit_gaus", "Fitter Gaussian", inv_mass, fit_gaus_mean, fit_gaus_sigma);
	RooExponential fit_exp("fit_exp", "Fitter Exponential", inv_mass, fit_exp_const);
	
	RooRealVar fit_pdf_frac("fit_pdf_frac", "Fitter PDF Fraction", 0.3, 0.1, 0.5);

	RooAddPdf fit_pdf("fit_pdf", "Fitter PDF", fit_gaus, fit_exp, fit_pdf_frac);

	// We now fit the generated data set with the fitter PDF

	fit_pdf.fitTo(*data_set);

	// Create a canvas for producing the plots of interest

	TCanvas canvas("canvas", "canvas", 800,600);

	// Create a RooPlot object to allow us to plot the data and PDFs, we tell the object which variable frame
	// we are interested in	

	RooPlot* inv_massframe = inv_mass.frame();	

	data_set->plotOn(inv_massframe);			// Plot the data onto the RooPlot
	fit_pdf.plotOn(inv_massframe, LineColor(kBlue));	// Plot the fit function onto the RooPlot
	inv_massframe->Draw();					// Draw the RooPlot object onto the canvas
	canvas.SaveAs("Demo_1.pdf"); 				// Save the canvas
	dataset->Delete();
	inv_massframe->Delete();
}
	
