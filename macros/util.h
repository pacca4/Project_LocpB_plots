class Consts
{
    public: //dichiaro pubbliche le costanti così posso accedere direttamente
        static const double lightVelocity; // cm/ps
        static const double massJPsi;
};

const double Consts::lightVelocity = 0.029979246; // cm/ps
const double Consts::massJPsi      = 3.096900; //Gev

void cart_from_spher(float  pt, float phi, float eta,
                     float& px, float& py, float& pz  ){
    px=pt* cos(phi);    py=pt* sin(phi);    pz=pt*sinh(eta);   
}


void cart_to_spher  ( float px, float py, float pz, 
                      float&  pt, float& phi, float& eta ){
    pt=sqrt(px*px+py*py); phi=acos(px/pt); eta=asinh(pz/pt);
}

float sc_prod (float x1, float y1, float z1, 
              float x2, float y2, float z2 ){
    return (x1*x2+y1*y2+z1*z2);
}

float norm (float x, float y, float z){
    return sqrt(x*x+y*y+z*z);
}

float JpsiGamma_angle(float ptmu1, float phimu1, float etamu1,
                     float ptmu2, float phimu2, float etamu2,
                     float ptG, float phiG, float etaG ){

    float pxmu1, pymu1, pzmu1,pxmu2, pymu2, pzmu2,pxG, pyG, pzG;
    cart_from_spher(ptmu1, phimu1, etamu1, pxmu1, pymu1, pzmu1);
    cart_from_spher(ptmu2, phimu2, etamu2, pxmu2, pymu2, pzmu2);
    cart_from_spher(  ptG,   phiG,   etaG,   pxG,   pyG,   pzG);

    float pxjpsi=pxmu1+pxmu2; float pyjpsi=pymu1+pymu2; float pzjpsi=pzmu1+pzmu2;
    return sc_prod(pxjpsi, pyjpsi, pzjpsi, pxG, pyG, pzG)/
             (norm(pxjpsi, pyjpsi, pzjpsi)*norm(pxG, pyG, pzG));
}


float MuMu_angle(float ptmu1, float phimu1, float etamu1,
                     float ptmu2, float phimu2, float etamu2){

    float pxmu1, pymu1, pzmu1,pxmu2, pymu2, pzmu2;
    cart_from_spher(ptmu1, phimu1, etamu1, pxmu1, pymu1, pzmu1);
    cart_from_spher(ptmu2, phimu2, etamu2, pxmu2, pymu2, pzmu2);

    return sc_prod(pxmu1, pymu1, pzmu1, pxmu2, pymu2, pzmu2)/
             (norm(pxmu1, pymu1, pzmu1)*norm(pxmu2, pymu2, pzmu2));
}

void DrawAngleJPsiG(TTree* tree){
    TTreeReader myread(tree);
	TTreeReaderArray<Float_t> muon_pt     = {myread, "Muon_pt"       };
	TTreeReaderArray<Float_t> muon_eta    = {myread, "Muon_eta"      };
	TTreeReaderArray<Float_t> muon_phi    = {myread, "Muon_phi"      };
	TTreeReaderArray<Float_t> photon_pt   = {myread, "Photon_pt"     };
	TTreeReaderArray<Float_t> photon_eta  = {myread, "Photon_eta"    };
    TTreeReaderArray<Float_t> photon_phi  = {myread, "Photon_phi"    };
    TH1F *angle= new TH1F ("Angles", "Angles", 200, -1, 1);
    while(myread.Next()){
        angle->Fill(JpsiGamma_angle(muon_pt[0], muon_phi[0], muon_eta[0],
                                    muon_pt[1], muon_phi[1], muon_eta[1],
                                    photon_pt[0], photon_phi[0], photon_eta[0]) );
    }
    angle->Draw();
}


void DrawAngleMuMu(TTree* tree){
    TTreeReader myread(tree);
	TTreeReaderArray<Float_t> muon_pt     = {myread, "Muon_pt"       };
	TTreeReaderArray<Float_t> muon_eta    = {myread, "Muon_eta"      };
	TTreeReaderArray<Float_t> muon_phi    = {myread, "Muon_phi"      };
	
    TH1F *angle= new TH1F ("Angles", "Angles", 200, -1, 1);
    while(myread.Next()){
        angle->Fill(MuMu_angle(muon_pt[0], muon_phi[0], muon_eta[0],
                               muon_pt[1], muon_phi[1], muon_eta[1] ));
    }
    angle->Draw();
}

TH1F* DrawAngleJPsiG(TTree* tree, Color_t color, int lwd, string name){
    TTreeReader myread(tree);
	TTreeReaderArray<Float_t> muon_pt     = {myread, "Muon_pt"       };
	TTreeReaderArray<Float_t> muon_eta    = {myread, "Muon_eta"      };
	TTreeReaderArray<Float_t> muon_phi    = {myread, "Muon_phi"      };
	TTreeReaderArray<Float_t> photon_pt   = {myread, "Photon_pt"     };
	TTreeReaderArray<Float_t> photon_eta  = {myread, "Photon_eta"    };
    TTreeReaderArray<Float_t> photon_phi  = {myread, "Photon_phi"    };
    TH1F *angle= new TH1F (name.c_str(), name.c_str(), 200, -1, 1);
    while(myread.Next()){
        angle->Fill(JpsiGamma_angle(muon_pt[0]  , muon_phi[0]  , muon_eta[0],
                                    muon_pt[1]  , muon_phi[1]  , muon_eta[1],
                                    photon_pt[0], photon_phi[0], photon_eta[0]) );
    }
    angle->SetLineWidth(lwd); angle->SetLineColor(color);
    return angle;
}

TH1F* DrawAngleMuMu(TTree* tree, Color_t color, int lwd, string name){
    TTreeReader myread(tree);
	TTreeReaderArray<Float_t> muon_pt     = {myread, "Muon_pt"       };
	TTreeReaderArray<Float_t> muon_eta    = {myread, "Muon_eta"      };
	TTreeReaderArray<Float_t> muon_phi    = {myread, "Muon_phi"      };
	
    TH1F *angle= new TH1F (name.c_str(), name.c_str(), 200, -1, 1);
    while(myread.Next()){
        angle->Fill(MuMu_angle(muon_pt[0], muon_phi[0], muon_eta[0],
                               muon_pt[1], muon_phi[1], muon_eta[1] ));
    }
    angle->SetLineWidth(lwd); angle->SetLineColor(color);
    return angle;
}























