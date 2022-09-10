void ex1()
{
    TRandom2 *rand = new TRandom2();

    //We generate the 10 input files
    for (Int_t i = 0; i < 10; i++)
    {
        TString filename = Form("input0%d.txt", i);

        fstream outfile;
        outfile.open(filename, ios::out);

        for (Int_t k = 0; k < 6000; k++)
        {
            for (Int_t j = 0; j < 6; j++)
            {
                Double_t val = rand->Gaus(0, 0.5*j);
                outfile << val << " ";
            }

            outfile << endl;
        }

        outfile.close();
    }
    /////////////////////////////////////////////

    TH1F *hist[6];

    THStack *hstack = new THStack("stack", "Distribution;X;Entries");

    TLegend *leg = new TLegend(0.7, 0.6, 0.9, 0.9);

    //Customize each hist
    for (Int_t i = 0; i < 6; i++)
    {
        TString name = Form("hist%d", i);
        hist[i] = new TH1F(name, "Histogram", 50, -5, 5);

        hist[i]->SetMarkerStyle(i+20);
        hist[i]->SetMarkerColor(i+0);

        hist[i]->GetYaxis()->SetRangeUser(0, 10000);

        hstack->Add(hist[i]);
        leg->AddEntry(hist[i], name, "p");
    }

    //Read the input files
    for (Int_t i = 0; i < 10; i++)
    {
        TString filename = Form("input0%d.txt", i);

        ifstream infile;
        infile.open(filename);

        while(1)
        {
            for (Int_t j = 0; j < 6; j++)
            {
                Double_t val;

                infile >> val;

                hist[j]->Fill(val);
            }

            if(infile.eof()) break;
        }

        infile.close();
    }

    TCanvas *c1 = new TCanvas();
    c1->SetGrid();
    c1->SetTickx();
    c1->SetTicky();
    hstack->Draw("Pnostack");
    leg->Draw();
}
