#include <iostream>

#include "RooAddPdf.h"
#include "RooArgusBG.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "RooRealVar.h"

#include "TCanvas.h"

using namespace RooFit;

int main(void){
	
	RooRealVar Inv_Mass("Inv_Mass", "Invariant Mass", 1E-2, 30.);

	RooRealVar gaus_mean("gaus_mean", "Gaussian Mean", 10.7, 8.5, 11.8);
	RooRealVar gaus_width("gaus_width", "Gaussian Width", 0.2, 0., 0.5);
	RooRealVar arg_param("arg_param", "Argus PDF Parameter", -29., -56., -10.);
	RooRealVar bkg_yield("bkg_yield", "Background Events", 1E6, 1E5, 5E6);
	RooRealVar sig_yield("sig_yield", "Signal Events", 1E5, 1E4, 5E5);

	RooGaussian gaus("gaus", "Gaussian", Inv_Mass, gaus_mean, gaus_width);
	RooArgusBG argbkg("argbkg", "Argus Background", Inv_Mass, RooConst(8.456), arg_param);

	RooAddPdf add_pdf("add_pdf", "Addition of PDFs", RooArgList(gaus, argbkg), RooArgList(sig_yield, bkg_yield));

	RooDataSet *data = add_pdf.generate("data", Inv_Mass);

	add_pdf.fitTo(*data, RooFit::Extended());


	RooPlot * Inv_Massframe = Inv_Mass.frame();

	TCanvas * canvas = new TCanvas("canvas", 600, 800);

	data->plotOn(Inv_Massframe);
	add_pdf.plotOn(Inv_Massframe);
	add_pdf.plotOn(Inv_Massframe, RooFit::Components(argbkg), RooFit::LineStyle(kDashed));

	canvas.SaveAs("Extended_PDF_Demo.pdf");
	canvas->Delete();
	data->Delete();
	Inv_Massframe->Delete();	
}
