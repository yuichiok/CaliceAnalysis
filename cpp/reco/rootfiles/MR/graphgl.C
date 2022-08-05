#include <iostream>

#include <TString.h>
#include <TStyle.h>
#include <TMath.h>
#include <TFile.h>
#include <TList.h>
#include <TH3.h>
#include <TF1.h>

namespace
{

//_______________________________________________________________________
TH3F * clone_sizes(const TH3 * src)
{
    TString name(src->GetName() + TString("_clone"));
    //
    const double xMin = src->GetXaxis()->GetXmin();
    const double xMax = src->GetXaxis()->GetXmax();
    const int xBins = src->GetNbinsX();
    //
    const double yMin = src->GetYaxis()->GetXmin();
    const double yMax = src->GetYaxis()->GetXmax();
    const int yBins = src->GetNbinsY();
    //
    const double zMin = src->GetZaxis()->GetXmin();
    const double zMax = src->GetZaxis()->GetXmax();
    const int zBins = src->GetNbinsZ();

    return new TH3F(name.Data(), name.Data(), xBins, xMin, xMax, yBins, yMin, yMax, zBins, zMin, zMax);
}

//_______________________________________________________________________
TH3F * transform_hist(const TH3 * src, double (*map)(double))
{
    TH3F * dst = clone_sizes(src);

    for(int i = 1; i <= src->GetNbinsX(); ++i)
    {
        for(int j = 1; j <= src->GetNbinsY(); ++j)
        {
            for(int k = 1; k <= src->GetNbinsZ(); ++k)
            {
                const double val = src->GetBinContent(i, j, k);
                if(map(val)<0.1){
                    continue;
                }
                // cout << map(val) << endl;
                dst->SetBinContent(i, j, k, map(val));
            }
        }
    }

    return dst;
}

//_______________________________________________________________________
double log_scale(double val)
{
    if(val <= 0.)
        // val = 10e-50;
        return 0;
    return TMath::Log(val);
}

//_______________________________________________________________________
double transfer_function(const double * px, const Double_t *)
{
    const double x = *px;
    float setval = 1.0;

    // cout << x << endl;

    if(x < setval)
        return 0.025;
    if(x > setval && x < setval+1)
        return 0.08;
    if(x > setval+1 && x < setval+2)
        return 0.09;
    if(x > setval+2 && x < setval+3)
        return 0.1;
    if(x > setval+3 && x < setval+4)
        return 0.2;

    return 0.8;
}

}

void graphgl(){

    TFile * file = TFile::Open("run_90320.e.10GeV.MR.root");
 
    TH3F *hist = (TH3F*) file->Get("h_3d_charge_map_dist");        

    // hist->Draw("BOX2Z");

    gStyle->SetCanvasPreferGL(1);
    TH3F * energyHistModified = transform_hist(hist, log_scale);

    //Transfer function.
    TList * lf = energyHistModified->GetListOfFunctions();
    if(lf)
    {
        TF1 * tf = new TF1("TransferFunction", transfer_function);
        lf->Add(tf);
    }
    lf->Print();


    // gStyle->SetPalette(1);
    energyHistModified->SetDirectory(0);
    energyHistModified->Draw("glcolz");

/*

    hist->Draw("glcolz");

    hist->GetXaxis()->SetTitle("Length (mm)");
    hist->GetXaxis()->SetTitleOffset(1.5);
    hist->GetYaxis()->SetTitle("Width (mm)");
    hist->GetYaxis()->SetTitleOffset(1.5);
    hist->GetZaxis()->SetTitle("Height (mm)");
    hist->GetZaxis()->SetTitleOffset(1.5);
    hist->GetZaxis()->SetRangeUser(-10,0);

    hist->SetTitle("OT");
    gStyle->SetCanvasPreferGL(1);
     gStyle->SetPalette(kRainBow);
    hist->SetFillColor(9);
    hist->Draw("glcol0");

    TPaveLabel *title = new TPaveLabel(-1., 0.86, 1., 0.98, "Sample 5480 (360 nm) - Optical Transmittance Deviance");
*/

/*
    TLatex *tex = new TLatex(0.75,0.05,"Optical Transmittance Deviance (%)");
    tex->SetNDC();
    tex->SetTextSize(.015);
    tex->SetLineWidth(2);
    tex->Draw();
    title->SetFillColor(32);
    title->Draw();



    TLatex *avg3 = new TLatex(0.81, 0.01, TString::Format("Average: %s", average));
    avg3->SetNDC();
    avg3->SetTextSize(.015);
    avg3->SetLineWidth(2);
    avg3->Draw();
    title->SetFillColor(32);
    title->Draw();
*/
    // //Now, specify the transfer function.
    // TList * lf = hist->GetListOfFunctions();
    // if (lf) {
    //     TF1 * tf = new TF1("TransferFunction", my_transfer_function);
    //     lf->Add(tf);
    // }
}
