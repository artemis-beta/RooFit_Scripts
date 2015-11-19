#include <iostream>
#include <RooRealVar.h>
#include <RooGenericPdf.h>
#include <RooDataSet.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooProdPdf.h>
#include <RooBreitWigner.h>

using namespace RooFit;

int main(){
    
    
    //All variables within RooFit are defined using the RooRealVar Class,
    //the format is RooRealVar c++_variable_name("RootName", "Description", optional Value or Start Value, optional range...)
    
    RooRealVar x_var("x_var", "x", -10, 10);                                                                                //"x" variable
    RooRealVar y_var("y_var", "y", 0, 5);                                                                                   //"y" another variable
    RooRealVar z_var("z_var", "z", 0, 30);                                                                                   //"z" another variable
    RooRealVar a_var("a_var", "a", 3.0);                                                                                    //"a" a constant term
    RooRealVar lambda("lambda", "lambda", -2.0);                                                                            //"lambda" a constant term
    RooRealVar mean("mean", "mean", -5.0, -10, 10);                                                                         //mean of gaussian
    RooRealVar mean_weigner("mean_weigner", "mean_weigner", -1.0, -5, 0.1);                                                 //mean of Breit-Wigner
    RooRealVar sigma("sigma", "sigma", 2.0, 0.1, 5);                                                                        //gaussian width
    RooRealVar width("width", "width", 2.0, 0.1, 5);                                                                        //Breit-Wigner width
  
    
    
    //For addition of PDFs (these are the functions) it is required that the user define what fraction of each should be used,
    //i.e. TotalFunction = PDF1*frac1+PDF2*(1-frac1)+... etc
    
    RooRealVar fraction("fraction", "fraction", 0.1, 0.5);
    RooRealVar fraction2("fraction2", "fraction2", 0.1, 0.5);
   
    //RooGenericPdf allows you to define the function within a string, it is important that you use the Root variable name here
    //RooGaussian and RooBreitWigner are PDF models (see documentation for the arguments required for a PDF)
    //RooArgSets or RooArgLists are used for arguments which require a collection of objects
    
    RooGenericPdf poly("poly", "poly", "exp(x_var*y_var+a_var)-lambda*x_var", RooArgSet(x_var, y_var, a_var,lambda));
    RooGaussian gauss("gauss", "gauss", x_var, mean, sigma);
    RooGaussian gauss_z("gauss_z", "gauss_z", z_var, mean, sigma);
    RooBreitWigner RBW("RBW", "RBW", x_var, mean_weigner, width);
    
    //RooAddPdf adds PDFs, note the order of the PDFs and fractions coincides, e.g. in this case poly has is associated with fraction 1 etc
    //Addition of Pdfs means components are added (consider x+a)
   
    RooAddPdf total_pdf("total_pdf", "Total PDF", RooArgSet(poly, gauss, RBW), RooArgSet(fraction, fraction2));
    
    //RooProdPdf multiplies PDFs consider this to be adding an extra dimension to the PDF (P1(x)*P2(y)), components must not share dependents
    
    RooProdPdf prod_pdf("prod_pdf", "prod_pdf", RooArgList(total_pdf, gauss_z));
    
    
    //To Generate events from a collection of PDFs we do the following (here we create 1000 data)
    
    RooDataSet* data = prod_pdf.generate(RooArgList(x_var, y_var, z_var), 1000);
    
    
    //Fit the Product PDF to the data set
    
    prod_pdf.fitTo(*data);
    
    //Create a ROOT Canvas so we can save output
    
    TCanvas canvas("canvas", "canvas", 800, 600);
    
    RooPlot* xframe = x_var.frame();                                                                //Create the plotting frame
    xframe->SetTitle("Demo of RooAddPdf Using Gaussian, Breit-Wigner and Polynomial");              //Set Plot Title
    data->plotOn(xframe);                                                                           //Plot "x" data
    
    
    //We Can plot individual components with different formatting
    
    prod_pdf.plotOn(xframe, Components(RooArgSet(RBW)), LineColor(kRed));
    prod_pdf.plotOn(xframe, Components(RooArgSet(gauss)), LineColor(kGreen));
    prod_pdf.plotOn(xframe, Components(RooArgSet(poly)), LineColor(kBlue));
    prod_pdf.plotOn(xframe, LineColor(kOrange), LineStyle(kDashed));
    xframe->Draw();
    
    canvas.SaveAs("RooDemo1.pdf"); //Save the Canvas
    
    RooPlot* zframe = z_var.frame();
    data->plotOn(zframe);
    prod_pdf.plotOn(zframe);
    zframe->Draw();
    
    canvas.SaveAs("RooDemo1_2.pdf");

    
    return 0;
}
