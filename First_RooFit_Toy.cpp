#include <iostream>

#include <RooAddPdf.h>
#include <RooGaussian.h>
#include "RooExponential.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include <RooRealVar.h>

#include "TCanvas.h"

using namespace RooFit;

int main(void){

	RooRealVar inv_mass("inv_mass", "Invariant Mass", 400, 600);

	RooRealVar gen_gaus_mean("gen_gaus_mean", "Generator Gaussian Mean", 493.6);
	RooRealVar gen_gaus_sigma("gen_gaus_sigma", "Generator Gaussian Sigma", 10.1);
	
	RooRealVar gen_exp_const("gen_exp_const", "Generator Exponential Constant", 0.05);

	RooGaussian gen_gaus("gen_gaus", "Generator Gaussian", inv_mass, gen_gaus_mean, gen_gaus_sigma);
	RooExponential gen_exp("gen_exp", "Generator Exponential", inv_mass, gen_exp_const);
	
	RooRealVar gen_pdf_frac("gen_pdf_frac", "Generator PDF Fraction", 0.3);

	RooAddPdf gen_pdf("gen_pdf", "Generator PDF", gen_gaus, gen_exp, gen_pdf_frac);

	RooDataSet* data_set = gen_pdf.generate(inv_mass, 10000);

	RooRealVar fit_gaus_mean("fit_gaus_mean", "Fitter Gaussian Mean", 470, 450, 550);
	RooRealVar fit_gaus_sigma("fit_gaus_sigma", "Fitter Gaussian Sigma", 12., 7., 15.);
	
	RooRealVar fit_exp_const("fit_exp_const", "Fitter Exponential Constant", 0.05, 0.001, 0.09);
	
	RooGaussian fit_gaus("fit_gaus", "Fitter Gaussian", inv_mass, fit_gaus_mean, fit_gaus_sigma);
	RooExponential fit_exp("fit_exp", "Fitter Exponential", inv_mass, fit_exp_const);
	
	RooRealVar fit_pdf_frac("fit_pdf_frac", "Fitter PDF Fraction", 0.3, 0.1, 0.5);

	RooAddPdf fit_pdf("fit_pdf", "Fitter PDF", fit_gaus, fit_exp, fit_pdf_frac);

	fit_pdf.fitTo(*data_set);

	TCanvas canvas("canvas", "canvas", 800,600);
	
	RooPlot* inv_massframe = inv_mass.frame();

	
	data_set->plotOn(inv_massframe);	
	fit_pdf.plotOn(inv_massframe, LineColor(kBlue));
	inv_massframe->Draw();
	canvas.SaveAs("Demo_1.pdf");
}
	
