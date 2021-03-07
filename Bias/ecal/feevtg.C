class StFcsDbMaker;
StFcsDbMaker* mFcsDbMaker=0;

int GAIN=0;           //This remain 0 for now
const int SLP=3366;   //3366*4 for Hcal?

const int NDEP=24;    //9 for Hcal
const int NBRANCH=2;  //always 2
const int NI2C=11;    //13 for Hcal
const int NCOL=22;    //13 for Hcal
const int NROW=34;    //20 for Hcal

const char* cflag[15]={"allon","col_even","row_even",
		       "cr_even","cr20_even","cr21_even",
		       "cr30_even","cr31_even","cr32_even",
		       "FCS","TARGET","POOH",
		       "XTALK","att6","att12"};

int getFee(int dep, int br, int i2c){
  return dep*NBRANCH*NI2C + br*NI2C + i2c;
}

char *pat1[NROW] = {"......................",
		    ".........111111111111.",
		    ".1111111111111........",
		    ".11...................",
		    ".11...................",
		    "..11.....1111111111...",
		    "..11111111111.........",
		    "...11.................",
		    "....11................",
		    "....111...............",
		    "....111...............",
		    "....111...............",
		    "...111................",
		    ".111.....1111111......",
		    "........11.......111..",
		    "......11.........111..",
		    "......11..............",
		    "......11..............",
		    "......11..............",
		    "......11..............",
		    ".......11.........11..",
		    ".........111.....111..",
		    "...........1111111....",
		    "......................",
		    "......111111111.......",
		    ".....11......11.......",
		    "...11..........11.....",
		    "....1111..............",
		    "......111111..........",
		    "..........1111........",
		    ".111..........111.....",
		    "...111..........11....",
		    ".....1111...1111......",
		    ".......11111111......."};

char *pat2[NROW] = {"......................",
                    "......................",
                    "...(.I,_I.)...........",
                    "....)....II...........",
                    ".../....II.|..........",
                    "...|.......I,.........",
                    "....I,_..I-/..........",
                    "....,&&&&&V...........",
                    "...,&&&&&&&&:.........",
                    "..,&&&&&&&&&&&;.......",
                    "..|...|&&&&&&&;I......",
                    "..|...|.......:_)._...",
                    "..|...|.......;--I.|..",
                    "..I---I...-I-----..|..",
                    ".....I_....|..|—I.....",
                    ".......I-I_I__/.......",
                    "......................",
                    "......................",
                    "...(.I,_I.)...........",
                    "....)....II...........",
                    ".../....II.|..........",
                    "...|.......I,.........",
                    "....I,_..I-/..........",
                    "....,&&&&&V...........",
                    "...,&&&&&&&&:.........",
                    "..,&&&&&&&&&&&;.......",
                    "..|...|&&&&&&&;I......",
                    "..|...|.......:_)._...",
                    "..|...|.......;--I.|..",
                    "..I---I...-I-----..|..",
                    ".....I_....|..|—I.....",
                    ".......I-I_I__/.......",
                    "......................",
                    "......................"};


char tgt[NROW][NCOL];

void feevtg(int flag=0){
  gSystem->Load("libPhysics");
  gSystem->Load("St_base");
  gSystem->Load("StChain");
  gSystem->Load("StUtilities");
  gSystem->Load("StEvent");
  gSystem->Load("StFcsDbMaker");
  mFcsDbMaker = new StFcsDbMaker();
  mFcsDbMaker->setDebug();
  mFcsDbMaker->Init();

  memset(tgt,0,sizeof(tgt));
    
  char* filename="ecal_tab.tsv";
  FILE *F=fopen(filename,"r");
  if(F==NULL){
    cout << "Cannot open " << filename << endl;
    return 0;
  }
  cout << "Reading " << filename << endl;

  char outname[100];
  sprintf(outname,"fcs_fee_ecal_physics_%s.csv",cflag[flag]);
  FILE *O=fopen(outname,"w");
  if(O==NULL){
    cout << "Cannot open " << outname << endl;
    return 0;
  }
  cout << "Writing " << outname << endl;

  int n=0;
  int d,i,r,c,s,ret;
  float v;
  int dac[2][NDEP*NBRANCH*NI2C][4];
  int fee[2][NDEP*NBRANCH*NI2C];
  memset(dac,0,sizeof(dac));
  memset(fee,0,sizeof(fee));
  while( (ret = fscanf(F,"%d %d %d %d %d %f",&d,&i,&r,&c,&s,&v)) > 0){
    //printf("%3d %3d %3d %3d %3d %5d %12.8f\n",n,d,i,r,c,s,v);
    int ehp,ns,dep,br,i2c,sipm,pp,j;
    mFcsDbMaker->getSCmap(d,i,ehp,ns,dep,br,i2c,sipm,pp,j);
    int f=getFee(dep,br,i2c);
    fee[ns][f]=1;
    int d = int((v-1.19+1.0)*16384/70.6869);
    switch(flag){
    case  0: break;
    case  1: if(c%2==1) d=0; break;
    case  2: if(r%2==1) d=0; break;
    case  3: if((c+r)%2==1) d=0; break;
    case  4: if( ( (c+0)/2 + (r+0)/2 )%2==1) d=0; break;
    case  5: if( ( (c+1)/2 + (r+1)/2 )%2==1) d=0; break;
    case  6: if( ( (c+0)/3 + (r+0)/3 )%2==1) d=0; break;
    case  7: if( ( (c+1)/3 + (r+1)/3 )%2==1) d=0; break;
    case  8: if( ( (c+2)/3 + (r+2)/3 )%2==1) d=0; break;
    case  9: if(pat1[r-1][c-1]=='.') d=0; break;
    case 10:
      float rr = sqrt((c-0.5)*(c-0.5) + (r-0.5-NROW/2)*(r-0.5-NROW/2));
      int ir = int(rr/2.0);
      if(ir%2 == 1) {d=0;}
      else          {tgt[r-1][c-1]=1;}
      break;
    case 11: if(pat2[r-1][c-1]=='.') d=0; break;
    case 12: 
      if( (c==6 && r==9) || (c==14 && r==21) ){
	//
      }else{
	d=0;		
      }	    
      break;
    case 13: GAIN=6; break;
    case 14: GAIN=12; break;
    }
    dac[ns][f][sipm]=d;
    printf("%1d %1d %2d %1d %2d  %2d %2d %1d %6d\n",
	   ehp,ns,dep,br,i2c,sipm,r,c,dac[ns][f][sipm]);		
    n++;
  }
  if(flag==10){
    for(int r=0; r<NROW; r++){
      for(int c=0; c<NCOL; c++){
	if(tgt[r][c]==1) {printf("@");}
	else             {printf(".");}
      }
      printf("\n");
    }
  }
  cout << "Found n=" << n << endl;
  int ehp=0;
  for(ns=0; ns<2; ns++){
    for(dep=0; dep<NDEP; dep++){
      for(br=0; br<NBRANCH; br++){
	for(i2c=0; i2c<NI2C; i2c++){
	  int f=getFee(dep,br,i2c);
	  if(fee[ns][f]>0){		    
	    fprintf(O, "%1d %1d %2d %1d %2d %3d %5d %5d %5d %5d %5d\n",
		    ehp,ns,dep,br,i2c,GAIN,SLP,
		    dac[ns][f][0],dac[ns][f][1],dac[ns][f][2],dac[ns][f][3]);  
	  }
	}
      }
    }
  }    
  fclose(F);
  fclose(O);
  delete mFcsDbMaker;
  printf("Wrote %s\n",outname);
}
