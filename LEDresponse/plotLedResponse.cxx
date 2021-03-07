#include <iostream>
#include <fstream>

void plotLedResponse()
{
  TFile *file = TFile::Open("totalResults.root");
  if(!file) {cout << "Wrong file!" << endl; return;}

  std::ofstream flaggedFile("flaggedTowers.txt");
  std::ofstream flatteningHNfile("flatteningHNtowers.txt");
  std::ofstream flatteningHSfile("flatteningHStowers.txt");

  
  TCanvas *canvas = new TCanvas("canvas", "Canvas", 600, 600);
  canvas->cd();
  canvas->SetGrid();
  canvas->SetTicks();
  canvas->SetLeftMargin(0.15);

  gStyle->SetOptStat(0);
  
  TMultiGraph *mgEN = new TMultiGraph();
  mgEN->SetTitle("All ECal North;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgES = new TMultiGraph();
  mgES->SetTitle("All ECal South;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgEN1to30 = new TMultiGraph();
  mgEN1to30->SetTitle("ECal North Rows 1-30;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgES1to30 = new TMultiGraph();
  mgES1to30->SetTitle("ECal South Rows 1-30;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgEN31to34 = new TMultiGraph();
  mgEN31to34->SetTitle("ECal North Rows 31-34;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgES31to34 = new TMultiGraph();
  mgES31to34->SetTitle("ECal South Rows 31-34;V - V_{nominal} (V);LED/LED_{nominal}");




  /*  
  TMultiGraph *mgHN = new TMultiGraph();
  mgHN->SetTitle("All HCal North;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgHS = new TMultiGraph();
  mgHS->SetTitle("All HCal South;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgHN_flat = new TMultiGraph();
  mgHN_flat->SetTitle("HCal North flattening;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgHS_flat = new TMultiGraph();
  mgHS_flat->SetTitle("HCal South flattening;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgHN_notFlat = new TMultiGraph();
  mgHN_notFlat->SetTitle("HCal North no flattening;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgHS_notFlat = new TMultiGraph();
  mgHS_notFlat->SetTitle("HCal South no flattening;V - V_{nominal} (V);LED/LED_{nominal}");
  */


  //std::vector<TString> flagged;
  //std::vector<TString> flatteningHN;
  //std::vector<TString> flatteningHS;

  TGraph *graph;
  TString name;
  TString number_str;
  Int_t number;
  //Double_t *xValues;
  //Double_t *yValues;

  TIter next(file->GetListOfKeys());
  TKey *key;

  while ((key = (TKey*)next()))
    {
      graph = (TGraph*)key->ReadObj();
      graph->SetTitle(";;");
      name = graph->GetName();
      number_str = (name[2] == '0') ? name(3,2) : name(2,3);
      number = number_str.Atoi();
      //xValues = graph->GetX();
      //yValues = graph->GetY();

      if (name.Contains("flagsFound"))
	{
	  //std::cout << "Flags found for " << name(0,5) << std::endl;
	  //flagged.push_back(name);
	  flaggedFile << name(0,5) << std::endl;
	  continue;
	}
      else if (name.Contains("temps"))
	continue;
      
      if (name.Contains("EN"))
	{
	  mgEN->Add(graph, "CP");

	  if (number < 660)
	    mgEN1to30->Add(graph, "CP");
	  else
	    mgEN31to34->Add(graph, "CP");
	}
      else if (name.Contains("ES"))
	{
	  mgES->Add(graph, "CP");

	  if (number < 660)
	    mgES1to30->Add(graph, "CP");
	  else
	    mgES31to34->Add(graph, "CP");
	}
      /*
      else if (name.Contains("HN"))
	{
	  mgHN->Add(graph, "CP");

	  if (yValues[3] - yValues[2] < 0.3)
	    {
	      mgHN_flat->Add(graph, "CP");
	      //flatteningHN.push_back(name);
	      flatteningHNfile << name << std::endl;
	    }
	  else
	    mgHN_notFlat->Add(graph, "CP");
	}
      else if (name.Contains("HS"))
	{
	  mgHS->Add(graph, "CP");

	  if (yValues[3] - yValues[2] < 0.3)
	    {
	      mgHS_flat->Add(graph, "CP");
	      //flatteningHS.push_back(name);
	      flatteningHSfile << name << std::endl;
	    }
	  else
	    mgHS_notFlat->Add(graph, "CP");
	}
      */
    }// End while loop

  
  Double_t lowX  = -1.1;
  Double_t highX = 1.1;
  Double_t lowY  = 0.0;
  Double_t highY = 3.0;
  
  mgEN->Draw("A");
  gPad->Modified();
  mgEN->GetXaxis()->SetLimits(lowX, highX);
  mgEN->SetMinimum(lowY);
  mgEN->SetMaximum(highY);

  canvas->SaveAs("combinedEN.png");
  canvas->Clear();

  mgES->Draw("A");
  gPad->Modified();
  mgES->GetXaxis()->SetLimits(lowX, highX);
  mgES->SetMinimum(lowY);
  mgES->SetMaximum(highY);

  canvas->SaveAs("combinedES.png");
  canvas->Clear();

  mgEN1to30->Draw("A");
  gPad->Modified();
  mgEN1to30->GetXaxis()->SetLimits(lowX, highX);
  mgEN1to30->SetMinimum(lowY);
  mgEN1to30->SetMaximum(highY);

  canvas->SaveAs("EN_1to30.png");
  canvas->Clear();

  mgES1to30->Draw("A");
  gPad->Modified();
  mgES1to30->GetXaxis()->SetLimits(lowX, highX);
  mgES1to30->SetMinimum(lowY);
  mgES1to30->SetMaximum(highY);

  canvas->SaveAs("ES_1to30.png");
  canvas->Clear();

  mgEN31to34->Draw("A");
  gPad->Modified();
  mgEN31to34->GetXaxis()->SetLimits(lowX, highX);
  mgEN31to34->SetMinimum(lowY);
  mgEN31to34->SetMaximum(highY);

  canvas->SaveAs("EN_31to34.png");
  canvas->Clear();

  mgES31to34->Draw("A");
  gPad->Modified();
  mgES31to34->GetXaxis()->SetLimits(lowX, highX);
  mgES31to34->SetMinimum(lowY);
  mgES31to34->SetMaximum(highY);

  canvas->SaveAs("ES_31to34.png");
  canvas->Clear();


  /*
  mgHN->Draw("A");
  gPad->Modified();
  mgHN->GetXaxis()->SetLimits(lowX, highX);
  mgHN->SetMinimum(lowY);
  mgHN->SetMaximum(highY);

  canvas->SaveAs("combinedHN.png");
  canvas->Clear();

  mgHS->Draw("A");
  gPad->Modified();
  mgHS->GetXaxis()->SetLimits(lowX, highX);
  mgHS->SetMinimum(lowY);
  mgHS->SetMaximum(highY);

  canvas->SaveAs("combinedHS.png");
  canvas->Clear();

  mgHN_flat->Draw("A");
  gPad->Modified();
  mgHN_flat->GetXaxis()->SetLimits(lowX, highX);
  mgHN_flat->SetMinimum(lowY);
  mgHN_flat->SetMaximum(highY);

  canvas->SaveAs("HN_flat.png");
  canvas->Clear();

  mgHS_flat->Draw("A");
  gPad->Modified();
  mgHS_flat->GetXaxis()->SetLimits(lowX, highX);
  mgHS_flat->SetMinimum(lowY);
  mgHS_flat->SetMaximum(highY);

  canvas->SaveAs("HS_flat.png");
  canvas->Clear();

  mgHN_notFlat->Draw("A");
  gPad->Modified();
  mgHN_notFlat->GetXaxis()->SetLimits(lowX, highX);
  mgHN_notFlat->SetMinimum(lowY);
  mgHN_notFlat->SetMaximum(highY);

  canvas->SaveAs("HN_notFlat.png");
  canvas->Clear();

  mgHS_notFlat->Draw("A");
  gPad->Modified();
  mgHS_notFlat->GetXaxis()->SetLimits(lowX, highX);
  mgHS_notFlat->SetMinimum(lowY);
  mgHS_notFlat->SetMaximum(highY);

  canvas->SaveAs("HS_notFlat.png");
  canvas->Clear();
  */
  file->Close();
  flaggedFile.close();
  flatteningHNfile.close();
  flatteningHSfile.close();







  // TEMPERATURE STUFF //

  TFile *tempFile = TFile::Open("tempStability.root");
  if(!file) {cout << "Wrong file!" << endl; return;}

  TMultiGraph *mgEN1to30_temp = new TMultiGraph();
  mgEN1to30_temp->SetTitle("ECal North Rows 1-30;V - V_{nominal} (V);Temperature (#circC)");

  TMultiGraph *mgES1to30_temp = new TMultiGraph();
  mgES1to30_temp->SetTitle("ECal South Rows 1-30;V - V_{nominal} (V);Temperature (#circC)");

  TMultiGraph *mgEN31to34_temp = new TMultiGraph();
  mgEN31to34_temp->SetTitle("ECal North Rows 31-34;V - V_{nominal} (V);Temperature (#circC)");

  TMultiGraph *mgES31to34_temp = new TMultiGraph();
  mgES31to34_temp->SetTitle("ECal South Rows 31-34;V - V_{nominal} (V);Temperature (#circC)");


  TIter tempNext(tempFile->GetListOfKeys());

  while ((key = (TKey*)tempNext()))
    {
      graph = (TGraph*)key->ReadObj();
      graph->SetTitle(";;");
      name = graph->GetName();
      number_str = (name[2] == '0') ? name(3,2) : name(2,3);
      number = number_str.Atoi();
  
      if (name.Contains("flagsFound"))continue;
      if (name.Contains("temps")) continue;
      
      if (name.Contains("EN"))
	{
	  if (number < 660)
	    mgEN1to30_temp->Add(graph, "LP");
	  else
	    mgEN31to34_temp->Add(graph, "LP");
	}
      else if (name.Contains("ES"))
	{
	  if (number < 660)
	    mgES1to30_temp->Add(graph, "LP");
	  else
	    mgES31to34_temp->Add(graph, "LP");
	}

    }// End while loop


  lowX  = -1.1;
  highX = 1.1;
  lowY  = 22.0;
  highY = 28.0;


  mgEN1to30_temp->Draw("A");
  gPad->Modified();
  mgEN1to30_temp->GetXaxis()->SetLimits(lowX, highX);
  mgEN1to30_temp->SetMinimum(lowY);
  mgEN1to30_temp->SetMaximum(highY);

  canvas->SaveAs("EN_1to30_temp.png");
  canvas->Clear();

  mgES1to30_temp->Draw("A");
  gPad->Modified();
  mgES1to30_temp->GetXaxis()->SetLimits(lowX, highX);
  mgES1to30_temp->SetMinimum(lowY);
  mgES1to30_temp->SetMaximum(highY);

  canvas->SaveAs("ES_1to30_temp.png");
  canvas->Clear();

  mgEN31to34_temp->Draw("A");
  gPad->Modified();
  mgEN31to34_temp->GetXaxis()->SetLimits(lowX, highX);
  mgEN31to34_temp->SetMinimum(lowY);
  mgEN31to34_temp->SetMaximum(highY);

  canvas->SaveAs("EN_31to34_temp.png");
  canvas->Clear();

  mgES31to34_temp->Draw("A");
  gPad->Modified();
  mgES31to34_temp->GetXaxis()->SetLimits(lowX, highX);
  mgES31to34_temp->SetMinimum(lowY);
  mgES31to34_temp->SetMaximum(highY);

  canvas->SaveAs("ES_31to34_temp.png");
  canvas->Clear();

  tempFile->Close();






  // PLOT FOR DIFFERENT TEMPERATURE BINS
  TFile *temp22to23File = TFile::Open("temp22to23.root");
  if(!temp22to23File) {cout << "temp22to23.root not found!" << endl; return;}

  TMultiGraph *mgE_r1to30_temp22to23 = new TMultiGraph();
  mgE_r1to30_temp22to23->SetTitle("ECal Rows 1-30 with 22 #leq T_{avg} < 23;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgE_r31to34_temp22to23 = new TMultiGraph();
  mgE_r31to34_temp22to23->SetTitle("ECal Rows 31-34 with 22 #leq T_{avg} < 23;V - V_{nominal} (V);LED/LED_{nominal}");

  if (temp22to23File->GetListOfKeys()->GetEntries() != 0)
    {
      TIter next22to23(temp22to23File->GetListOfKeys());

      while ((key = (TKey*)next22to23()))
	{
	  graph = (TGraph*)key->ReadObj();
	  graph->SetTitle(";;");
	  name = graph->GetName();
	  number_str = (name[2] == '0') ? name(3,2) : name(2,3);
	  number = number_str.Atoi();
  
	  if (name.Contains("flagsFound"))continue;
	  if (name.Contains("temps")) continue;
	  if (name.Contains("H")) continue;
	  else
	    {
	      if (number < 660)
		mgE_r1to30_temp22to23->Add(graph, "LP");
	      else
		mgE_r31to34_temp22to23->Add(graph, "LP");
	    }
	}// End while loop
  

      lowX  = -1.1;
      highX = 1.1;
      lowY  = 0.0;
      highY = 3.0;
  
      mgE_r1to30_temp22to23->Draw("A");
      gPad->Modified();
      mgE_r1to30_temp22to23->GetXaxis()->SetLimits(lowX, highX);
      mgE_r1to30_temp22to23->SetMinimum(lowY);
      mgE_r1to30_temp22to23->SetMaximum(highY);

      canvas->SaveAs("temp22to23_r1to30.png");
      canvas->Clear();

      mgE_r31to34_temp22to23->Draw("A");
      gPad->Modified();
      mgE_r31to34_temp22to23->GetXaxis()->SetLimits(lowX, highX);
      mgE_r31to34_temp22to23->SetMinimum(lowY);
      mgE_r31to34_temp22to23->SetMaximum(highY);

      canvas->SaveAs("temp22to23_r31to34.png");
      canvas->Clear();
    }// End if temp22to23 is not empty
  temp22to23File->Close();






  TFile *temp23to24File = TFile::Open("temp23to24.root");
  if(!temp23to24File) {cout << "temp23to24.root not found!" << endl; return;}

  TMultiGraph *mgE_r1to30_temp23to24 = new TMultiGraph();
  mgE_r1to30_temp23to24->SetTitle("ECal Rows 1-30 with 23 #leq T_{avg} < 24;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgE_r31to34_temp23to24 = new TMultiGraph();
  mgE_r31to34_temp23to24->SetTitle("ECal Rows 31-34 with 23 #leq T_{avg} < 24;V - V_{nominal} (V);LED/LED_{nominal}");

  if (temp23to24File->GetListOfKeys()->GetEntries() != 0)
    {
      TIter next23to24(temp23to24File->GetListOfKeys());

      while ((key = (TKey*)next23to24()))
	{
	  graph = (TGraph*)key->ReadObj();
	  graph->SetTitle(";;");
	  name = graph->GetName();
	  number_str = (name[2] == '0') ? name(3,2) : name(2,3);
	  number = number_str.Atoi();
  
	  if (name.Contains("flagsFound"))continue;
	  if (name.Contains("temps")) continue;
	  if (name.Contains("H")) continue;
	  else
	    {
	      if (number < 660)
		mgE_r1to30_temp23to24->Add(graph, "LP");
	      else
		mgE_r31to34_temp23to24->Add(graph, "LP");
	    }
	}// End while loop
  

      lowX  = -1.1;
      highX = 1.1;
      lowY  = 0.0;
      highY = 3.0;
  
      mgE_r1to30_temp23to24->Draw("A");
      gPad->Modified();
      mgE_r1to30_temp23to24->GetXaxis()->SetLimits(lowX, highX);
      mgE_r1to30_temp23to24->SetMinimum(lowY);
      mgE_r1to30_temp23to24->SetMaximum(highY);

      canvas->SaveAs("temp23to24_r1to30.png");
      canvas->Clear();

      mgE_r31to34_temp23to24->Draw("A");
      gPad->Modified();
      mgE_r31to34_temp23to24->GetXaxis()->SetLimits(lowX, highX);
      mgE_r31to34_temp23to24->SetMinimum(lowY);
      mgE_r31to34_temp23to24->SetMaximum(highY);

      canvas->SaveAs("temp23to24_r31to34.png");
      canvas->Clear();
    }
  temp23to24File->Close();







  TFile *temp24to25File = TFile::Open("temp24to25.root");
  if(!temp24to25File) {cout << "temp24to25.root not found!" << endl; return;}

  TMultiGraph *mgE_r1to30_temp24to25 = new TMultiGraph();
  mgE_r1to30_temp24to25->SetTitle("ECal Rows 1-30 with 24 #leq T_{avg} < 25;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgE_r31to34_temp24to25 = new TMultiGraph();
  mgE_r31to34_temp24to25->SetTitle("ECal Rows 31-34 with 24 #leq T_{avg} < 25;V - V_{nominal} (V);LED/LED_{nominal}");

  if (temp24to25File->GetListOfKeys()->GetEntries() != 0)
    {
      TIter next24to25(temp24to25File->GetListOfKeys());

      while ((key = (TKey*)next24to25()))
	{
	  graph = (TGraph*)key->ReadObj();
	  graph->SetTitle(";;");
	  name = graph->GetName();
	  number_str = (name[2] == '0') ? name(3,2) : name(2,3);
	  number = number_str.Atoi();
  
	  if (name.Contains("flagsFound"))continue;
	  if (name.Contains("temps")) continue;
	  if (name.Contains("H")) continue;
	  else
	    {
	      if (number < 660)
		mgE_r1to30_temp24to25->Add(graph, "LP");
	      else
		mgE_r31to34_temp24to25->Add(graph, "LP");
	    }
	}// End while loop
  

      lowX  = -1.1;
      highX = 1.1;
      lowY  = 0.0;
      highY = 3.0;
  
      mgE_r1to30_temp24to25->Draw("A");
      gPad->Modified();
      mgE_r1to30_temp24to25->GetXaxis()->SetLimits(lowX, highX);
      mgE_r1to30_temp24to25->SetMinimum(lowY);
      mgE_r1to30_temp24to25->SetMaximum(highY);

      canvas->SaveAs("temp24to25_r1to30.png");
      canvas->Clear();

      mgE_r31to34_temp24to25->Draw("A");
      gPad->Modified();
      mgE_r31to34_temp24to25->GetXaxis()->SetLimits(lowX, highX);
      mgE_r31to34_temp24to25->SetMinimum(lowY);
      mgE_r31to34_temp24to25->SetMaximum(highY);

      canvas->SaveAs("temp24to25_r31to34.png");
      canvas->Clear();
    }
  temp24to25File->Close();








  
  TFile *temp25to26File = TFile::Open("temp25to26.root");
  if(!temp25to26File) {cout << "temp25to26.root not found!" << endl; return;}

  TMultiGraph *mgE_r1to30_temp25to26 = new TMultiGraph();
  mgE_r1to30_temp25to26->SetTitle("ECal Rows 1-30 with 25 #leq T_{avg} < 26;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgE_r31to34_temp25to26 = new TMultiGraph();
  mgE_r31to34_temp25to26->SetTitle("ECal Rows 31-34 with 25 #leq T_{avg} < 26;V - V_{nominal} (V);LED/LED_{nominal}");

  if (temp25to26File->GetListOfKeys()->GetEntries() != 0)
    {  
      TIter next25to26(temp25to26File->GetListOfKeys());

      while ((key = (TKey*)next25to26()))
	{
	  graph = (TGraph*)key->ReadObj();
	  graph->SetTitle(";;");
	  name = graph->GetName();
	  number_str = (name[2] == '0') ? name(3,2) : name(2,3);
	  number = number_str.Atoi();
  
	  if (name.Contains("flagsFound"))continue;
	  if (name.Contains("temps")) continue;
	  if (name.Contains("H")) continue;
	  else
	    {
	      if (number < 660)
		mgE_r1to30_temp25to26->Add(graph, "LP");
	      else
		mgE_r31to34_temp25to26->Add(graph, "LP");
	    }
	}// End while loop
  

      lowX  = -1.1;
      highX = 1.1;
      lowY  = 0.0;
      highY = 3.0;
  
      mgE_r1to30_temp25to26->Draw("A");
      gPad->Modified();
      mgE_r1to30_temp25to26->GetXaxis()->SetLimits(lowX, highX);
      mgE_r1to30_temp25to26->SetMinimum(lowY);
      mgE_r1to30_temp25to26->SetMaximum(highY);

      canvas->SaveAs("temp25to26_r1to30.png");
      canvas->Clear();

      mgE_r31to34_temp25to26->Draw("A");
      gPad->Modified();
      mgE_r31to34_temp25to26->GetXaxis()->SetLimits(lowX, highX);
      mgE_r31to34_temp25to26->SetMinimum(lowY);
      mgE_r31to34_temp25to26->SetMaximum(highY);

      canvas->SaveAs("temp25to26_r31to34.png");
      canvas->Clear();
    }
  temp25to26File->Close();





  TFile *temp26to27File = TFile::Open("temp26to27.root");
  if(!temp26to27File) {cout << "temp26to27.root not found!" << endl; return;}

  TMultiGraph *mgE_r1to30_temp26to27 = new TMultiGraph();
  mgE_r1to30_temp26to27->SetTitle("ECal Rows 1-30 with 26 #leq T_{avg} < 27;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgE_r31to34_temp26to27 = new TMultiGraph();
  mgE_r31to34_temp26to27->SetTitle("ECal Rows 31-34 with 26 #leq T_{avg} < 27;V - V_{nominal} (V);LED/LED_{nominal}");

  if (temp26to27File->GetListOfKeys()->GetEntries() != 0)
    {
      TIter next26to27(temp26to27File->GetListOfKeys());

      while ((key = (TKey*)next26to27()))
	{
	  graph = (TGraph*)key->ReadObj();
	  graph->SetTitle(";;");
	  name = graph->GetName();
	  number_str = (name[2] == '0') ? name(3,2) : name(2,3);
	  number = number_str.Atoi();
  
	  if (name.Contains("flagsFound"))continue;
	  if (name.Contains("temps")) continue;
	  if (name.Contains("H")) continue;
	  else
	    {
	      if (number < 660)
		mgE_r1to30_temp26to27->Add(graph, "LP");
	      else
		mgE_r31to34_temp26to27->Add(graph, "LP");
	    }
	}// End while loop
  

      lowX  = -1.1;
      highX = 1.1;
      lowY  = 0.0;
      highY = 3.0;
  
      mgE_r1to30_temp26to27->Draw("A");
      gPad->Modified();
      mgE_r1to30_temp26to27->GetXaxis()->SetLimits(lowX, highX);
      mgE_r1to30_temp26to27->SetMinimum(lowY);
      mgE_r1to30_temp26to27->SetMaximum(highY);

      canvas->SaveAs("temp26to27_r1to30.png");
      canvas->Clear();

      mgE_r31to34_temp26to27->Draw("A");
      gPad->Modified();
      mgE_r31to34_temp26to27->GetXaxis()->SetLimits(lowX, highX);
      mgE_r31to34_temp26to27->SetMinimum(lowY);
      mgE_r31to34_temp26to27->SetMaximum(highY);

      canvas->SaveAs("temp26to27_r31to34.png");
      canvas->Clear();
    }
  temp26to27File->Close();






  TFile *temp27to28File = TFile::Open("temp27to28.root");
  if(!temp27to28File) {cout << "temp27to28.root not found!" << endl; return;}

  TMultiGraph *mgE_r1to30_temp27to28 = new TMultiGraph();
  mgE_r1to30_temp27to28->SetTitle("ECal Rows 1-30 with 27 #leq T_{avg} < 28;V - V_{nominal} (V);LED/LED_{nominal}");

  TMultiGraph *mgE_r31to34_temp27to28 = new TMultiGraph();
  mgE_r31to34_temp27to28->SetTitle("ECal Rows 31-34 with 27 #leq T_{avg} < 28;V - V_{nominal} (V);LED/LED_{nominal}");

  if (temp27to28File->GetListOfKeys()->GetEntries() != 0)
    {
      TIter next27to28(temp27to28File->GetListOfKeys());

      while ((key = (TKey*)next27to28()))
	{
	  graph = (TGraph*)key->ReadObj();
	  graph->SetTitle(";;");
	  name = graph->GetName();
	  number_str = (name[2] == '0') ? name(3,2) : name(2,3);
	  number = number_str.Atoi();
  
	  if (name.Contains("flagsFound"))continue;
	  if (name.Contains("temps")) continue;
	  if (name.Contains("H")) continue;
	  else
	    {
	      if (number < 660)
		mgE_r1to30_temp27to28->Add(graph, "LP");
	      else
		mgE_r31to34_temp27to28->Add(graph, "LP");
	    }
	}// End while loop
  

      lowX  = -1.1;
      highX = 1.1;
      lowY  = 0.0;
      highY = 3.0;
  
      mgE_r1to30_temp27to28->Draw("A");
      gPad->Modified();
      mgE_r1to30_temp27to28->GetXaxis()->SetLimits(lowX, highX);
      mgE_r1to30_temp27to28->SetMinimum(lowY);
      mgE_r1to30_temp27to28->SetMaximum(highY);

      canvas->SaveAs("temp27to28_r1to30.png");
      canvas->Clear();

      mgE_r31to34_temp27to28->Draw("A");
      gPad->Modified();
      mgE_r31to34_temp27to28->GetXaxis()->SetLimits(lowX, highX);
      mgE_r31to34_temp27to28->SetMinimum(lowY);
      mgE_r31to34_temp27to28->SetMaximum(highY);

      canvas->SaveAs("temp27to28_r31to34.png");
      canvas->Clear();
    }
  temp27to28File->Close();


}
