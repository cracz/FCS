#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"

struct Data
{
  std::string id;
  int idNumber;
  std::string flags;
  float nominalLed;
  std::vector<float> temperatures;
  std::vector<float> ledValues;
  std::vector<float> deltaV;

  void reset()
  {
    id = "none";
    flags = "none";

    idNumber = 99;
    nominalLed = 99;

    std::vector<float>().swap(temperatures);
    std::vector<float>().swap(ledValues);
    std::vector<float>().swap(deltaV);
  }
};



//void analyzeLED()
int main()
{
  //std::string fileName_minus1   = "compiledInfo_1107038.tsv";  // Marked as useless
  std::string fileName_minus0p5 = "compiledInfo_22098021.tsv";
  std::string fileName_plus0p5  = "compiledInfo_22098022.tsv";
  std::string fileName_plus1    = "compiledInfo_22098023.tsv";
  std::string fileName_nominal  = "compiledInfo_22098020.tsv";

  std::string fileNameList[4] = { fileName_minus0p5, fileName_nominal, fileName_plus0p5, fileName_plus1 };

  TFile *totalResultsFile = new TFile("totalResults.root", "RECREATE");
  TFile *tempStabilityFile = new TFile("tempStability.root", "RECREATE");
  TFile *temp22to23File = new TFile("temp22to23.root", "RECREATE");
  TFile *temp23to24File = new TFile("temp23to24.root", "RECREATE");
  TFile *temp24to25File = new TFile("temp24to25.root", "RECREATE");
  TFile *temp25to26File = new TFile("temp25to26.root", "RECREATE");
  TFile *temp26to27File = new TFile("temp26to27.root", "RECREATE");
  TFile *temp27to28File = new TFile("temp27to28.root", "RECREATE");

  // This is a mapping from a SiPM ID (i.e. EN057) to the struct that holds that SiPMs data
  std::map<std::string, Data> idToDataMap;

  // Loop over the input files
  for (int i = 0; i < 4; i++)
    {
      std::ifstream inputStream(fileNameList[i].c_str());
      if (!inputStream.is_open()) { std::cout << "File " << i << " not found!" << std::endl; }
      
      std::string line;
      std::getline(inputStream, line);   // Get the first line

      // Loop over lines of input in current file
      while (inputStream.good())
	{
	  // Skip the text lines
	  if (line[0] == '#') { std::getline(inputStream, line); continue; }


	  Data sipmData;
	  sipmData.reset();


	  std::istringstream iss(line);
	  int iterator = 1;
	  std::string element, currentID, currentFlags;
	  float currentLed = 99;
	  float currentTemp = 0;
	  int currentVsldac, currentAdc = 0;
	  int currentDet = -1;
	  int currentIdNumber = -1;

	  // Loop over elements of the current line and get the data for that SiPM
	  while(iss >> element)
	    {
	      switch (iterator)
		{
		case 1: currentID = element.substr(0, 5); // just first 5 chars
		  break;
		case 2: currentLed = atoi(element.c_str());
		  break;
		case 3: currentDet = atoi(element.c_str());
		  break;
		case 8: currentVsldac = atoi(element.c_str());
		  break;
		case 9: currentAdc = atoi(element.c_str());
		  break;
		case 10: currentFlags = element;
		  break;
		}
	      iterator++;
	    }

	  currentIdNumber = atoi(currentID.substr(2,3).c_str());
	  
	  currentTemp = (4.363391*currentAdc/65536) / (0.07667*currentVsldac/(currentDet==0 ? 4096:16384)) + 1.01;

	  
	  if (i == 0) //First file, create the mapping
	    {
	      sipmData.id = currentID;
	      sipmData.idNumber = currentIdNumber;
	      sipmData.ledValues.push_back(currentLed);
	      sipmData.temperatures.push_back(currentTemp);

	      if (currentFlags.compare("0_Good") == 0 || currentFlags.compare("0_Good_TooManyLedRatioBad") == 0) {} // Good
	      else
		sipmData.flags = "flagsFound";
	      
	      if (fileNameList[i].find("22098020") != std::string::npos)
		{
		  sipmData.nominalLed = currentLed;
		  sipmData.deltaV.push_back(0.0);
		}
	      else if (fileNameList[i].find("22098021") != std::string::npos) { sipmData.deltaV.push_back(-0.5); }
	      else if (fileNameList[i].find("22098022") != std::string::npos) { sipmData.deltaV.push_back(0.5); }
	      else if (fileNameList[i].find("22098023") != std::string::npos) { sipmData.deltaV.push_back(1.0); }

	      idToDataMap[currentID] = sipmData;
	    }
	  else   //Not the first file, mapping is created and must only be edited
	    {
	      idToDataMap[currentID].ledValues.push_back(currentLed);
	      idToDataMap[currentID].temperatures.push_back(currentTemp);

	      if (currentFlags.compare("0_Good") == 0 || currentFlags.compare("0_Good_TooManyLedRatioBad") == 0) {} // Good
	      else 
		idToDataMap[currentID].flags = "flagsFound";
	      
	      if (fileNameList[i].find("22098020") != std::string::npos)
		{
		  idToDataMap[currentID].nominalLed = currentLed;
		  idToDataMap[currentID].deltaV.push_back(0.0);
		}
	      else if (fileNameList[i].find("22098021") != std::string::npos) { idToDataMap[currentID].deltaV.push_back(-0.5); }
	      else if (fileNameList[i].find("22098022") != std::string::npos) { idToDataMap[currentID].deltaV.push_back(0.5); }
	      else if (fileNameList[i].find("22098023") != std::string::npos) { idToDataMap[currentID].deltaV.push_back(1.0); }
	    }


	  // Done, get the next line
	  std::getline(inputStream, line);
	}// End while over input lines

      inputStream.close();

      std::cout << "File " << i+1 << " read in..." << std::endl;
    }// End for loop over file names


  TGraph *graphLed;
  TGraph *graphTemp;

  TH1F *E_normLED_R1to30_Vm0p5 = new TH1F("E_normLED_R1to30_Vm0p5", "ECal Rows 1-30 Normalized LEDs at -0.5 V;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  TH1F *E_normLED_R31to34_Vm0p5 = new TH1F("E_normLED_R31to34_Vm0p5", "ECal Rows 31-34 Normalized LEDs at -0.5 V;LED/LED_{nominal};Towers", 100, 0.5, 0.7);

  TH1F *E_normLED_R1to30_Vm0p5_temp22to23 = new TH1F("E_normLED_R1to30_Vm0p5_temp22to23", "ECal Rows 1-30 at -0.5 V and 22 #leq T_{avg} < 23;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  TH1F *E_normLED_R31to34_Vm0p5_temp22to23 = new TH1F("E_normLED_R31to34_Vm0p5_temp22to23", "ECal Rows 31-34 at -0.5 V and 22 #leq T_{avg} < 23;LED/LED_{nominal};Towers", 100, 0.5, 0.7);

  TH1F *E_normLED_R1to30_Vm0p5_temp23to24 = new TH1F("E_normLED_R1to30_Vm0p5_temp23to24", "ECal Rows 1-30 at -0.5 V and 23 #leq T_{avg} < 24;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  TH1F *E_normLED_R31to34_Vm0p5_temp23to24 = new TH1F("E_normLED_R31to34_Vm0p5_temp23to24", "ECal Rows 31-34 at -0.5 V and 23 #leq T_{avg} < 24;LED/LED_{nominal};Towers", 100, 0.5, 0.7);

  TH1F *E_normLED_R1to30_Vm0p5_temp24to25 = new TH1F("E_normLED_R1to30_Vm0p5_temp24to25", "ECal Rows 1-30 at -0.5 V and 24 #leq T_{avg} < 25;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  TH1F *E_normLED_R31to34_Vm0p5_temp24to25 = new TH1F("E_normLED_R31to34_Vm0p5_temp24to25", "ECal Rows 31-34 at -0.5 V and 24 #leq T_{avg} < 25;LED/LED_{nominal};Towers", 100, 0.5, 0.7);

  TH1F *E_normLED_R1to30_Vm0p5_temp25to26 = new TH1F("E_normLED_R1to30_Vm0p5_temp25to26", "ECal Rows 1-30 at -0.5 V and 25 #leq T_{avg} < 26;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  //TH1F *E_normLED_R31to34_Vm0p5_temp25to26 = new TH1F("E_normLED_R31to34_Vm0p5_temp25to26", "ECal Rows 31-34 at -0.5 V and 25 #leq T_{avg} < 26;LED/LED_{nominal};Towers", 100, 0.5, 0.7);

  TH1F *E_normLED_R1to30_Vm0p5_temp26to27 = new TH1F("E_normLED_R1to30_Vm0p5_temp26to27", "ECal Rows 1-30 at -0.5 V and 26 #leq T_{avg} < 27;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  //TH1F *E_normLED_R31to34_Vm0p5_temp26to27 = new TH1F("E_normLED_R31to34_Vm0p5_temp26to27", "ECal Rows 31-34 at -0.5 V and 26 #leq T_{avg} < 27;LED/LED_{nominal};Towers", 100, 0.5, 0.7);

  /*
  TH1F *E_normLED_R1to30_Vm0p5_temp27to28 = new TH1F("E_normLED_R1to30_Vm0p5_temp27to28", "ECal Rows 1-30 at -0.5 V and 27 #leq T_{avg} < 28;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  TH1F *E_normLED_R31to34_Vm0p5_temp27to28 = new TH1F("E_normLED_R31to34_Vm0p5_temp27to28", "ECal Rows 31-34 at -0.5 V and 27 #leq T_{avg} < 28;LED/LED_{nominal};Towers", 100, 0.5, 0.7);
  */
  TH1F *E_rawLED_R1to30_Vm0p5 = new TH1F("E_rawLED_R1to30_Vm0p5", "ECal Rows 1-30 Raw LEDs at -0.5 V;LED;Towers", 200, 200, 2600);
  TH1F *E_rawLED_R31to34_Vm0p5 = new TH1F("E_rawLED_R31to34_Vm0p5", "ECal Rows 31-34 Raw LEDs at -0.5 V;LED;Towers", 200, 500, 3250);

  TH1F *EN_tempsAtVm0p5 = new TH1F("EN_tempsAtVm0p5", "EN Temps at V_{nom}- 0.5;Temperature (#circC);Towers", 200, 20, 30);
  TH1F *ES_tempsAtVm0p5 = new TH1F("ES_tempsAtVm0p5", "ES Temps at V_{nom}- 0.5;Temperature (#circC);Towers", 200, 20, 30);

  TH1F *EN_tempsAtVnom = new TH1F("EN_tempsAtVnom", "EN Temps at V_{nom};Temperature (#circC);Towers", 200, 20, 30);
  TH1F *ES_tempsAtVnom = new TH1F("ES_tempsAtVnom", "ES Temps at V_{nom};Temperature (#circC);Towers", 200, 20, 30);

  TH1F *EN_tempsAtVp0p5 = new TH1F("EN_tempsAtVp0p5", "EN Temps at V_{nom}+ 0.5;Temperature (#circC);Towers", 200, 20, 30);
  TH1F *ES_tempsAtVp0p5 = new TH1F("ES_tempsAtVp0p5", "ES Temps at V_{nom}+ 0.5;Temperature (#circC);Towers", 200, 20, 30);

  TH1F *EN_tempsAtVp1p0 = new TH1F("EN_tempsAtVp1p0", "EN Temps at V_{nom}+ 1.0;Temperature (#circC);Towers", 200, 20, 30);
  TH1F *ES_tempsAtVp1p0 = new TH1F("ES_tempsAtVp1p0", "ES Temps at V_{nom}+ 1.0;Temperature (#circC);Towers", 200, 20, 30);

  std::map<std::string, Data>::iterator it;
  for (it = idToDataMap.begin(); it != idToDataMap.end(); it++)
    {
      //it->first is the key (the sipm ID)
      //it->second is the value (the 'Data' type with all the info)


      // Get the average temperature of the SiPM
      float avgTemp = 0;
      int measurements = (int)it->second.temperatures.size();
      for (unsigned int i = 0; i < it->second.temperatures.size(); i++)
	{ avgTemp += it->second.temperatures.at(i); }

      avgTemp /= (float)measurements;


      // Graphs      
      graphLed = new TGraph();
      graphTemp = new TGraph();
      std::string graphName;
      
      if ((it->second.flags).compare("none") != 0)
	graphName = it->first + "_" + it->second.flags;
      else
	graphName = it->first;
      
      graphLed->SetName(graphName.c_str());
      graphTemp->SetName(graphName.c_str());

      graphLed->SetTitle((it->first + ";#DeltaV;LED/LED_{nominal}").c_str());
      graphTemp->SetTitle((it->first + ";#DeltaV;Temperature (#circC)").c_str());

      graphLed->SetMarkerStyle(20);
      graphTemp->SetMarkerStyle(20);

      //Fill the graphs with the data from the current SiPM
      for (unsigned int i = 0; i < it->second.ledValues.size(); i++)
	{ graphLed->SetPoint(graphLed->GetN(), it->second.deltaV.at(i), it->second.ledValues.at(i) / it->second.nominalLed); }

      for (unsigned int i = 0; i < it->second.temperatures.size(); i++)
	{ graphTemp->SetPoint(graphTemp->GetN(), it->second.deltaV.at(i), it->second.temperatures.at(i)); }

      graphTemp->SetMinimum(22);
      graphTemp->SetMaximum(28);

      tempStabilityFile->cd();
      graphTemp->Write();
      delete graphTemp;

      totalResultsFile->cd();
      graphLed->Write();

      
      // Separating graphs into temperature bins
      if (avgTemp >= 22.0 && avgTemp < 23.0)
	{
	  temp22to23File->cd();
	  graphLed->Write();
	  delete graphLed;
	}
      else if (avgTemp >= 23.0 && avgTemp < 24.0)
	{
	  temp23to24File->cd();
	  graphLed->Write();
	  delete graphLed;
	}
      else if (avgTemp >= 24.0 && avgTemp < 25.0)
	{
	  temp24to25File->cd();
	  graphLed->Write();
	  delete graphLed;
	}
      else if (avgTemp >= 25.0 && avgTemp < 26.0)
	{
	  temp25to26File->cd();
	  graphLed->Write();
	  delete graphLed;
	}
      else if (avgTemp >= 26.0 && avgTemp < 27.0)
	{
	  temp26to27File->cd();
	  graphLed->Write();
	  delete graphLed;
	}
      else if (avgTemp >= 27.0 && avgTemp < 28.0)
	{
	  temp27to28File->cd();
	  graphLed->Write();
	  delete graphLed;
	}


      // Temperature distributions of individual bias settings. 
      // Since the input files can have any order, find the indeces for the different bias settings first.

      int indexM0p5, indexNom, indexP0p5, indexP1p0;
      for (int i = 0; i < it->second.deltaV.size(); i++)
	{
	  if (it->second.deltaV.at(i) == -0.5) indexM0p5 = i;
	  else if (it->second.deltaV.at(i) == 0.0) indexNom = i;
	  else if (it->second.deltaV.at(i) == 0.5) indexP0p5 = i;
	  else if (it->second.deltaV.at(i) == 1.0) indexP1p0 = i;
	}

      if ((it->second.flags).compare("none") == 0)
	{
	  if ((it->first.substr(0,1)).compare("E") == 0 && it->second.idNumber < 660)
	    {
	      E_normLED_R1to30_Vm0p5->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed);
	      E_rawLED_R1to30_Vm0p5->Fill(it->second.ledValues.at(indexM0p5));

	      if (avgTemp >= 22.0 && avgTemp < 23.0)
		{ E_normLED_R1to30_Vm0p5_temp22to23->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 23.0 && avgTemp < 24.0)
		{ E_normLED_R1to30_Vm0p5_temp23to24->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 24.0 && avgTemp < 25.0)
		{ E_normLED_R1to30_Vm0p5_temp24to25->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 25.0 && avgTemp < 26.0)
		{ E_normLED_R1to30_Vm0p5_temp25to26->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 26.0 && avgTemp < 27.0)
		{ E_normLED_R1to30_Vm0p5_temp26to27->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      /*
	      else if (avgTemp >= 27.0 && avgTemp < 28.0)
		{ E_normLED_R1to30_Vm0p5_temp27to28->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      */
	    }
	  else if ((it->first.substr(0,1)).compare("E") == 0 && it->second.idNumber >= 660)
	    {
	      E_normLED_R31to34_Vm0p5->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed);
	      E_rawLED_R31to34_Vm0p5->Fill(it->second.ledValues.at(indexM0p5));

	      if (avgTemp >= 22.0 && avgTemp < 23.0)
		{ E_normLED_R31to34_Vm0p5_temp22to23->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 23.0 && avgTemp < 24.0)
		{ E_normLED_R31to34_Vm0p5_temp23to24->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 24.0 && avgTemp < 25.0)
		{ E_normLED_R31to34_Vm0p5_temp24to25->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      /*
	      else if (avgTemp >= 25.0 && avgTemp < 26.0)
		{ E_normLED_R31to34_Vm0p5_temp25to26->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 26.0 && avgTemp < 27.0)
		{ E_normLED_R31to34_Vm0p5_temp26to27->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      else if (avgTemp >= 27.0 && avgTemp < 28.0)
		{ E_normLED_R31to34_Vm0p5_temp27to28->Fill(it->second.ledValues.at(indexM0p5) / it->second.nominalLed); }
	      */
	    }
	  
	  if ((it->first.substr(0,2)).compare("EN") == 0)
	    {
	      EN_tempsAtVm0p5->Fill(it->second.temperatures.at(indexM0p5));
	      EN_tempsAtVnom->Fill(it->second.temperatures.at(indexNom));
	      EN_tempsAtVp0p5->Fill(it->second.temperatures.at(indexP0p5));
	      EN_tempsAtVp1p0->Fill(it->second.temperatures.at(indexP1p0));
	    }
	  else if ((it->first.substr(0,2)).compare("ES") == 0)
	    {
	      ES_tempsAtVm0p5->Fill(it->second.temperatures.at(indexM0p5));
	      ES_tempsAtVnom->Fill(it->second.temperatures.at(indexNom));
	      ES_tempsAtVp0p5->Fill(it->second.temperatures.at(indexP0p5));
	      ES_tempsAtVp1p0->Fill(it->second.temperatures.at(indexP1p0));
	    }
	}
    }// End loop through idToDataMap

  totalResultsFile->cd();

  E_normLED_R1to30_Vm0p5->Write();
  E_normLED_R31to34_Vm0p5->Write();

  E_normLED_R1to30_Vm0p5_temp22to23->Write();
  E_normLED_R31to34_Vm0p5_temp22to23->Write();

  E_normLED_R1to30_Vm0p5_temp23to24->Write();
  E_normLED_R31to34_Vm0p5_temp23to24->Write();

  E_normLED_R1to30_Vm0p5_temp24to25->Write();
  E_normLED_R31to34_Vm0p5_temp24to25->Write();

  E_normLED_R1to30_Vm0p5_temp25to26->Write();
  //E_normLED_R31to34_Vm0p5_temp25to26->Write();

  E_normLED_R1to30_Vm0p5_temp26to27->Write();
  //E_normLED_R31to34_Vm0p5_temp26to27->Write();

  //E_normLED_R1to30_Vm0p5_temp27to28->Write();
  //E_normLED_R31to34_Vm0p5_temp27to28->Write();

  E_rawLED_R1to30_Vm0p5->Write();
  E_rawLED_R31to34_Vm0p5->Write();

  EN_tempsAtVm0p5->Write();
  ES_tempsAtVm0p5->Write();

  EN_tempsAtVnom->Write();
  ES_tempsAtVnom->Write();

  EN_tempsAtVp0p5->Write();
  ES_tempsAtVp0p5->Write();

  EN_tempsAtVp1p0->Write();
  ES_tempsAtVp1p0->Write();

  totalResultsFile->Close();
  tempStabilityFile->Close();

  delete totalResultsFile;
  delete tempStabilityFile;

  std::cout << "Regular graphs saved in totalResults.root." << std::endl;
  std::cout << "Temperature graphs saved in tempStability.root." << std::endl;
}
