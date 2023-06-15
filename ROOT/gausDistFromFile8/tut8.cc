void tut8()
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

    TCanvas *c1 = new TCanvas();
    hist->Draw();
}
