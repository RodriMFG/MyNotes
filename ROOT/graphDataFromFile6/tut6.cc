void tut6()
{
    TGraph *gr = new TGraph();

    gr->SetMarkerStyle(kFullCircle);
    gr->SetMarkerSize(1);
    gr->SetMarkerColor(kBlue);
    gr->SetLineWidth(1);
    gr->SetLineColor(kRed);

    fstream file;
    file.open("data.txt", ios::in);

    while(1)
    {
        double x, y;
        file >> x >> y;
        //SetPoint takes number of datapoint, x, y
        gr->SetPoint(gr->GetN(), x, y);
        if(file.eof()) break;
    }

    file.close();

    TCanvas *c1 = new TCanvas();
    gr->Draw("AP");
}
