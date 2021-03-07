class StFcsDbMaker;
StFcsDbMaker* mFcsDbMaker=0;

int GAIN=0;     //This remain 0 for now
const int SLP=3366*4;   //3366*4 for Hcal? //3366 for Ecal

const int NDEP=9;     //9 for Hcal //24 for Ecal
const int NBRANCH=2;  //always 2
const int NI2C=13;    //13 for Hcal //11 for Ecal
const int NCOL=13;    //13 for Hcal //22 for Ecal
const int NROW=20;    //20 for Hcal //34 for Ecal

const char* cflag[14]={"allon","col_even","row_even",
		       "cr_even","cr20_even","cr21_even",
		       "cr30_even","cr31_even","cr32_even",
		       "FCS","TARGET","XTALK",
                       "att6","att12"};

int getFee(int dep, int br, int i2c){
    return dep*NBRANCH*NI2C + br*NI2C + i2c;
}
/*
char *pat1[NROW] = {".............",
		    "....11111....",
		    "...11...11...",
		    "...11...11...",
		    "...11...11...",
		    "...11...11...",
		    "...11...11...",
		    "...11...11...",
		    "...11...11...",
		    "....11111....",
		    ".............",
		    "..11....11...",
		    "..11...11....",
		    "..11..11.....",
		    "..11.11......",
		    "..111........",
		    "..11.11......",
		    "..11..11.....",
		    "..11...11....",
		    "..11....11..."
		    };
*/

char *pat1[NROW]=      {"111..111.1111",
		    	"1...1....1...",
			"111.1....1111",
			"1...1.......1",
			"1....111.1111",
			"......1......",
			".....1.1.....",
			"....1...1....",
			"1111.....1111",
			".1.........1.",
			"..1.......1..",
			"...1.....1...",
			"....1...1....",
			"...1..1..1...",
			"..1..1.1..1..",
			".1..1...1..1.",
			"1..1.....1..1",
			"1.1.......1.1",
			".1.........1.",
			"1111.....1111",};

char tgt[NROW][NCOL];

void feevtg_hcal(int flag=0){
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
    
    char* filename="hcal_tab.tsv";
    FILE *F=fopen(filename,"r");
    if(F==NULL){
        cout << "Cannot open " << filename << endl;
        return 0;
    }
    cout << "Reading " << filename << endl;

    char outname[100];
    sprintf(outname,"fcs_fee_hcal_physics_%s.csv",cflag[flag]);
    FILE *O=fopen(outname,"w");
    if(O==NULL){
        cout << "Cannot open " << outname << endl;
        return 0;
    }
    cout << "Writing " << outname << endl;

    int n=0;
    int d,i,r,c,s,ret,curr;
    float v0,v;
    char* wlsp[100];
    int dac[2][NDEP*NBRANCH*NI2C][4];
    int fee[2][NDEP*NBRANCH*NI2C];
    memset(dac,0,sizeof(dac));
    memset(fee,0,sizeof(fee));
    while( (ret = fscanf(F,"%d %d %d %d %d %f %s %d %f",&d,&i,&r,&c,&s,&v0,wlsp,&curr,&v)) > 0){
	//printf("%3d %3d %3d %3d %3d %5d %12.8f\n",n,d,i,r,c,s,v);
	int ehp,ns,dep,br,i2c,sipm,pp,j;
	mFcsDbMaker->getSCmap(d,i,ehp,ns,dep,br,i2c,sipm,pp,j);
	int f=getFee(dep,br,i2c);
	fee[ns][f]=1;
	int d = int((v-1.19)*16384/70.6869);
	switch(flag){
	case 0: break;
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
        case 11:
            if(c==6 && r==9){
		//
            }else{
                d=0;
            }
            break;
	case 12: GAIN=6; break;
	case 13: GAIN=12; break;
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
    int ehp=1;
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

