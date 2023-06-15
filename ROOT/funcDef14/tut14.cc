void tut14()
{
    TCanvas *c1 = new TCanvas();

    TF1 *func = new TF1("func", "[0] + [1]*x + [2]*x*x", 0 , 10);

    func->SetTitle("");

    func->SetParameter(0, 1);
    func->SetParameter(1, -1);
    func->SetParameter(2, 0.5);

    func->Draw();
}
