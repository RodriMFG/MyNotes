void tut12()
{
    TGraphErrors *gr = new TGraphErrors();

    //we generate the values into a file
    TRandom2 *rand = new TRandom2(3);

    fstream file;

    file.open("data.txt", ios::out);

    for (int i = 1; i < 10; i++)
    {
        double r = rand->Rndm()*0.4*i - 0.2*i;
        file << i << " " << i + r << " " << 0 << " " << 0.2*i << endl;
    }

    file.close();

    ///

    file.open("data.txt", ios::in);

    double x , y, ex, ey;
    int n = 0;

    while(1)
    {
        file >> x >> y >> ex >> ey;

        n = gr->GetN();

        //We set the points and errors
        gr->SetPoint(n, x, y);
        gr->SetPointError(n, ex, ey);

        if(file.eof()) break;
    }

    TCanvas *c1 = new TCanvas();
    gr->Draw("A*");

    //We name the fit and select the function pol1 (pol of power 1)
    TF1 *fit = new TF1("fit", "pol1", 0, 10);
    gr->Fit("fit");
}
