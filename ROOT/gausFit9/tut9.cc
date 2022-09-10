void tut9()
{
    double value;

    //TH1F takes name, title, no. of pins, start, end
    TH1F *hist = new TH1F("hist", "Histogram", 100, 0, 10);

    //TRandom2 takes seed
    TRandom2 *rand = new TRandom2(3);

    //we generate the values into a file
    fstream file;
    file.open("data.txt", ios::out);

    for (int i = 0; i < 1000; i++)
    {
        //Gaus takes mean, sigma
        double r = rand->Gaus(5, 1);
        file << r << endl;
    }

    file.close();

    //Read the values from the filled file
    file.open("data.txt", ios::in);
    while(1)
    {
        file >> value;
        hist->Fill(value);
        if(file.eof()) break;
    }
    file.close();

    //TF1 takes name, function, interval(,)
    TF1 *fit = new TF1("fit", "gaus", 1, 9);

    //Parameters to help root with the fit
    //Set the initial paramter #, to value
    //Max
    fit->SetParameter(0, 40);
    //Mean
    fit->SetParameter(1, 25);
    //Std dev
    fit->SetParameter(2, 1);

    TCanvas *c1 = new TCanvas();
    hist->Draw();
    hist->Fit("fit", "R");

    //Get the parameter number #
    double mean = fit->GetParameter(1);
    double sigma = fit->GetParameter(2);
    cout << mean/sigma << endl;
}
