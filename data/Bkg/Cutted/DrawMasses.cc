void DrawMasses(){
TCanvas *c=new TCanvas ("c","MuMuG_invMass",1800,1200);
c->Divide(3,2);
c->SetTitle("Masses_TriggerComparison");

c->cd(1);
TVirtualPad *c_1 = c->cd(1);
//c_1->SetLogy();
Events->Draw("MuMuG_invMass");
TH1F *hist = (TH1F*)gPad->GetPrimitive("htemp");
hist->SetTitle("No Trigger");

c->cd(2);
TVirtualPad *c_2 = c->cd(2);
//c_2->SetLogy();
Events_TriggerIsoMu27->Draw("MuMuG_invMass");
hist = (TH1F*)gPad->GetPrimitive("htemp");
hist->SetTitle("HLT_IsoMu27 Trigger");

c->cd(3);
TVirtualPad *c_3 = c->cd(3);
//c_3->SetLogy();
Events_TriggerMu17Mu8Iso->Draw("MuMuG_invMass");
hist = (TH1F*)gPad->GetPrimitive("htemp");
hist->SetTitle("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ Trigger");

c->cd(4);
TVirtualPad *c_4 = c->cd(4);
//c_4->SetLogy();
Events_TriggerOR->Draw("MuMuG_invMass");
hist = (TH1F*)gPad->GetPrimitive("htemp");
hist->SetTitle("OR Trigger");

c->cd(5);
TVirtualPad *c_5 = c->cd(5);
//c_5->SetLogy();
Events_TriggerAND->Draw("MuMuG_invMass");
hist = (TH1F*)gPad->GetPrimitive("htemp");
hist->SetTitle("AND Trigger");

c->Print("Masses_TriggerComparison.pdf");
}
