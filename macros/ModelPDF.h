
///// DATA PDFs //////

//pdf for Data Iso Mu

//Generation

RooRealVar Bkg_IsoMu_coef1( "Bkg_IsoMu_coef1", "Bkg_IsoMu_coef1"  , -0.0104791,  "GeV^-1" );
RooRealVar Bkg_IsoMu_coef2( "Bkg_IsoMu_coef2", "Bkg_IsoMu_coef2"  , 2.92647e-05, "GeV^-2" );

RooPolynomial Bkg_IsoMu_polyn ( "Bkg_IsoMu_polyn", "Bkg_IsoMu_polyn", Reco_mass, RooArgList( Bkg_IsoMu_coef1, Bkg_IsoMu_coef2 ) );

//toy

RooRealVar Toy_Bkg_IsoMu_coef1( "Toy_Bkg_IsoMu_coef1", "Toy_Bkg_IsoMu_coef1"  , -0.0104791, -0.1, 0.1, "GeV^-1" );
RooRealVar Toy_Bkg_IsoMu_coef2( "Toy_Bkg_IsoMu_coef2", "Toy_Bkg_IsoMu_coef2"  , 2.92647e-05,  -0.001,  0.01, "GeV^-2" );

RooPolynomial Toy_Bkg_IsoMu_polyn ( "Toy_Bkg_IsoMu_polyn", "Toy_Bkg_IsoMu_polyn",
                 Reco_mass, RooArgList( Toy_Bkg_IsoMu_coef1, Toy_Bkg_IsoMu_coef2 ) );


//pdf for Data Iso Trk

//generation
RooRealVar     Bkg_IsoTrk_tau ( "Bkg_IsoTrk_tau", "Bkg_IsoTrk_tau" , -0.0302445,  "GeV");
RooExponential Bkg_IsoTrk_exp ( "Bkg_IsoTrk_exp", "Bkg_IsoTrk_exp" ,   Reco_mass, Bkg_IsoTrk_tau);//, ResoGauss, RooDecay::SingleSided );

//toy

RooRealVar     Toy_Bkg_IsoTrk_tau ( "Toy_Bkg_IsoTrk_tau", "Toy_Bkg_IsoTrk_tau" , -0.0302445, -1, 0., "GeV");
RooExponential Toy_Bkg_IsoTrk_exp ( "Toy_Bkg_IsoTrk_exp", "Toy_Bkg_IsoTrk_exp" ,   Reco_mass, Toy_Bkg_IsoTrk_tau);//, ResoGauss, RooDecay::SingleSided );


/////////// SIgnal PDFs

//iso mu

//Generation

RooRealVar  Sig_IsoMu_mean   ( "Sig_IsoMu_mean", "Sig_IsoMu_mean",   124.793,   "GeV" );
RooRealVar  Sig_IsoMu_sig1   ( "Sig_IsoMu_sig1", "Sig_IsoMu_sig1",  0.733419,   "GeV" );
RooRealVar  Sig_IsoMu_sig2   ( "Sig_IsoMu_sig2", "Sig_IsoMu_sig2",   2.48086,   "GeV" );
RooRealVar  Sig_IsoMu_sig3   ( "Sig_IsoMu_sig3", "Sig_IsoMu_sig3",   11.2167,   "GeV" );

RooRealVar  Sig_IsoMu_gfrac1 ( "Sig_IsoMu_gfrac1", "Sig_IsoMu_gfrac1", 0.263134);
RooRealVar  Sig_IsoMu_gfrac2 ( "Sig_IsoMu_gfrac2", "Sig_IsoMu_gfrac2", 0.698942);

RooGaussian Sig_IsoMu_gauss1 ( "Sig_IsoMu_gauss1", "Sig_IsoMu_gauss1", Reco_mass, Sig_IsoMu_mean, Sig_IsoMu_sig1 );
RooGaussian Sig_IsoMu_gauss2 ( "Sig_IsoMu_gauss2", "Sig_IsoMu_gauss2", Reco_mass, Sig_IsoMu_mean, Sig_IsoMu_sig2 );
RooGaussian Sig_IsoMu_gauss3 ( "Sig_IsoMu_gauss3", "Sig_IsoMu_gauss3", Reco_mass, Sig_IsoMu_mean, Sig_IsoMu_sig3 );

RooAddPdf Sig_IsoMu_GaussSum ( "Sig_IsoMu_GaussSum", "Sig_IsoMu_GaussSum", RooArgList( Sig_IsoMu_gauss1, Sig_IsoMu_gauss2, Sig_IsoMu_gauss3), RooArgList( Sig_IsoMu_gfrac1, Sig_IsoMu_gfrac2 ));

RooRealVar Sig_IsoMu_CBSig   ("Sig_IsoMu_CBSig"  , "Sig_IsoMu_CBSig"  , 1.41684, "Gev");
RooRealVar Sig_IsoMu_CBn     ("Sig_IsoMu_CBn"    , "Sig_IsoMu_CBn"    , 1.79598, "Gev");
RooRealVar Sig_IsoMu_CBalpha ("Sig_IsoMu_CBalpha", "Sig_IsoMu_CBalpha", 1.66116, "Gev");
RooCBShape Sig_IsoMu_CB ("Sig_IsoMu_CB", "Sig_IsoMu_CB", Reco_mass ,
                         Sig_IsoMu_mean, Sig_IsoMu_CBSig  ,
                         Sig_IsoMu_CBn , Sig_IsoMu_CBalpha);

RooRealVar Sig_IsoMu_Frac( "Sig_IsoMu_Frac", "Sig_IsoMu_Frac" , 0.31048 "." );

RooAddPdf  Sig_IsoMu_Model  ( "Sig_IsoMu_Model", "Sig_IsoMu_Model ",
                            RooArgList(Sig_IsoMu_GaussSum, Sig_IsoMu_CB), Sig_IsoMu_Frac );

//toy

RooRealVar  Toy_Sig_IsoMu_mean   ( "Toy_Sig_IsoMu_mean", "Toy_Sig_IsoMu_mean",   124.793,   120,  130, "GeV" );
RooRealVar  Toy_Sig_IsoMu_sig1   ( "Toy_Sig_IsoMu_sig1", "Toy_Sig_IsoMu_sig1",  0.733419,   0.01,  10, "GeV" );
RooRealVar  Toy_Sig_IsoMu_sig2   ( "Toy_Sig_IsoMu_sig2", "Toy_Sig_IsoMu_sig2",   2.48086,   0.01,  10, "GeV" );
RooRealVar  Toy_Sig_IsoMu_sig3   ( "Toy_Sig_IsoMu_sig3", "Toy_Sig_IsoMu_sig3",   11.2167,      1, 100, "GeV" );

RooRealVar  Toy_Sig_IsoMu_gfrac1 ( "Toy_Sig_IsoMu_gfrac1", "Toy_Sig_IsoMu_gfrac1", 0.263134,  0.0, 1.0 );
RooRealVar  Toy_Sig_IsoMu_gfrac2 ( "Toy_Sig_IsoMu_gfrac2", "Toy_Sig_IsoMu_gfrac2", 0.698942,  0.0, 1.0 );

RooGaussian Toy_Sig_IsoMu_gauss1 ( "Toy_Sig_IsoMu_gauss1", "Toy_Sig_IsoMu_gauss1", Reco_mass, Toy_Sig_IsoMu_mean, Toy_Sig_IsoMu_sig1 );
RooGaussian Toy_Sig_IsoMu_gauss2 ( "Toy_Sig_IsoMu_gauss2", "Toy_Sig_IsoMu_gauss2", Reco_mass, Toy_Sig_IsoMu_mean, Toy_Sig_IsoMu_sig2 );
RooGaussian Toy_Sig_IsoMu_gauss3 ( "Toy_Sig_IsoMu_gauss3", "Toy_Sig_IsoMu_gauss3", Reco_mass, Toy_Sig_IsoMu_mean, Toy_Sig_IsoMu_sig3 );

RooAddPdf Toy_Sig_IsoMu_GaussSum ( "Toy_Sig_IsoMu_GaussSum", "Toy_Sig_IsoMu_GaussSum"  ,
                                RooArgList( Toy_Sig_IsoMu_gauss1, Toy_Sig_IsoMu_gauss2, Toy_Sig_IsoMu_gauss3),
                                RooArgList( Toy_Sig_IsoMu_gfrac1, Toy_Sig_IsoMu_gfrac2 ));

RooRealVar Toy_Sig_IsoMu_CBSig   ("Toy_Sig_IsoMu_CBSig"  , "Toy_Sig_IsoMu_CBSig"  , 1.41684, 0.1, 10, "Gev");
RooRealVar Toy_Sig_IsoMu_CBn     ("Toy_Sig_IsoMu_CBn"    , "Toy_Sig_IsoMu_CBn"    , 1.79598, 0, 10, "Gev");
RooRealVar Toy_Sig_IsoMu_CBalpha ("Toy_Sig_IsoMu_CBalpha", "Toy_Sig_IsoMu_CBalpha", 1.66116, 0, 3, "Gev");
RooCBShape Toy_Sig_IsoMu_CB ("Toy_Sig_IsoMu_CB", "Toy_Sig_IsoMu_CB", Reco_mass ,
                         Toy_Sig_IsoMu_mean, Toy_Sig_IsoMu_CBSig  ,
                         Toy_Sig_IsoMu_CBn , Toy_Sig_IsoMu_CBalpha); 

RooRealVar Toy_Sig_IsoMu_Frac( "Toy_Sig_IsoMu_Frac", "Toy_Sig_IsoMu_Frac" , 0.31048, 0.0, 1.0, "." );

RooAddPdf  Toy_Sig_IsoMu_Model  ( "Toy_Sig_IsoMu_Model", "Toy_Sig_IsoMu_Model ",
                            RooArgList(Toy_Sig_IsoMu_GaussSum, Toy_Sig_IsoMu_CB), Toy_Sig_IsoMu_Frac );

//Iso Trk
//Generation

RooRealVar  Sig_IsoTrk_mean   ( "Sig_IsoTrk_mean", "Sig_IsoTrk_mean",   124.795,   "GeV" );
RooRealVar  Sig_IsoTrk_sig1   ( "Sig_IsoTrk_sig1", "Sig_IsoTrk_sig1",  0.776242,   "GeV" );
RooRealVar  Sig_IsoTrk_sig2   ( "Sig_IsoTrk_sig2", "Sig_IsoTrk_sig2",   2.45992,   "GeV" );
RooRealVar  Sig_IsoTrk_sig3   ( "Sig_IsoTrk_sig3", "Sig_IsoTrk_sig3",   12.1068,   "GeV" );

RooRealVar  Sig_IsoTrk_gfrac1 ( "Sig_IsoTrk_gfrac1", "Sig_IsoTrk_gfrac1", 0.336607);
RooRealVar  Sig_IsoTrk_gfrac2 ( "Sig_IsoTrk_gfrac2", "Sig_IsoTrk_gfrac2", 0.632098);

RooGaussian Sig_IsoTrk_gauss1 ( "Sig_IsoTrk_gauss1", "Sig_IsoTrk_gauss1", Reco_mass, Sig_IsoTrk_mean, Sig_IsoTrk_sig1 );
RooGaussian Sig_IsoTrk_gauss2 ( "Sig_IsoTrk_gauss2", "Sig_IsoTrk_gauss2", Reco_mass, Sig_IsoTrk_mean, Sig_IsoTrk_sig2 );
RooGaussian Sig_IsoTrk_gauss3 ( "Sig_IsoTrk_gauss3", "Sig_IsoTrk_gauss3", Reco_mass, Sig_IsoTrk_mean, Sig_IsoTrk_sig3 );

RooAddPdf Sig_IsoTrk_GaussSum ( "Sig_IsoTrk_GaussSum", "Sig_IsoTrk_GaussSum"  ,
                                RooArgList( Sig_IsoTrk_gauss1, Sig_IsoTrk_gauss2, Sig_IsoTrk_gauss3),
                                RooArgList( Sig_IsoTrk_gfrac1, Sig_IsoTrk_gfrac2 ));

RooRealVar Sig_IsoTrk_CBSig   ("Sig_IsoTrk_CBSig"  , "Sig_IsoTrk_CBSig"  , 1.39755, "Gev");
RooRealVar Sig_IsoTrk_CBn     ("Sig_IsoTrk_CBn"    , "Sig_IsoTrk_CBn"    , 1.81178, "Gev");
RooRealVar Sig_IsoTrk_CBalpha ("Sig_IsoTrk_CBalpha", "Sig_IsoTrk_CBalpha", 1.59337, "Gev");
RooCBShape Sig_IsoTrk_CB ("Sig_IsoTrk_CB", "Sig_IsoTrk_CB", Reco_mass ,
                         Sig_IsoTrk_mean, Sig_IsoTrk_CBSig  ,
                         Sig_IsoTrk_CBn , Sig_IsoTrk_CBalpha);

RooRealVar Sig_IsoTrk_Frac( "Sig_IsoTrk_Frac", "Sig_IsoTrk_Frac" , 0.325142 "." );

RooAddPdf  Sig_IsoTrk_Model  ( "Sig_IsoTrk_Model", "Sig_IsoTrk_Model ",
                            RooArgList(Sig_IsoTrk_GaussSum, Sig_IsoTrk_CB), Sig_IsoTrk_Frac );

//toy

RooRealVar  Toy_Sig_IsoTrk_mean   ( "Toy_Sig_IsoTrk_mean", "Toy_Sig_IsoTrk_mean",   124.795,   120,  130, "GeV" );
RooRealVar  Toy_Sig_IsoTrk_sig1   ( "Toy_Sig_IsoTrk_sig1", "Toy_Sig_IsoTrk_sig1",  0.776242,   0.01,  10, "GeV" );
RooRealVar  Toy_Sig_IsoTrk_sig2   ( "Toy_Sig_IsoTrk_sig2", "Toy_Sig_IsoTrk_sig2",   2.45992,   0.01,  10, "GeV" );
RooRealVar  Toy_Sig_IsoTrk_sig3   ( "Toy_Sig_IsoTrk_sig3", "Toy_Sig_IsoTrk_sig3",   12.1068,      1, 100, "GeV" );

RooRealVar  Toy_Sig_IsoTrk_gfrac1 ( "Toy_Sig_IsoTrk_gfrac1", "Toy_Sig_IsoTrk_gfrac1", 0.336607,  0.0, 1.0 );
RooRealVar  Toy_Sig_IsoTrk_gfrac2 ( "Toy_Sig_IsoTrk_gfrac2", "Toy_Sig_IsoTrk_gfrac2", 0.632098,  0.0, 1.0 );

RooGaussian Toy_Sig_IsoTrk_gauss1 ( "Toy_Sig_IsoTrk_gauss1", "Toy_Sig_IsoTrk_gauss1", Reco_mass, Toy_Sig_IsoTrk_mean, Toy_Sig_IsoTrk_sig1 );
RooGaussian Toy_Sig_IsoTrk_gauss2 ( "Toy_Sig_IsoTrk_gauss2", "Toy_Sig_IsoTrk_gauss2", Reco_mass, Toy_Sig_IsoTrk_mean, Toy_Sig_IsoTrk_sig2 );
RooGaussian Toy_Sig_IsoTrk_gauss3 ( "Toy_Sig_IsoTrk_gauss3", "Toy_Sig_IsoTrk_gauss3", Reco_mass, Toy_Sig_IsoTrk_mean, Toy_Sig_IsoTrk_sig3 );

RooAddPdf Toy_Sig_IsoTrk_GaussSum ( "Toy_Sig_IsoTrk_GaussSum", "Toy_Sig_IsoTrk_GaussSum"  ,
                                RooArgList( Toy_Sig_IsoTrk_gauss1, Toy_Sig_IsoTrk_gauss2, Toy_Sig_IsoTrk_gauss3),
                                RooArgList( Toy_Sig_IsoTrk_gfrac1, Toy_Sig_IsoTrk_gfrac2 ));

RooRealVar Toy_Sig_IsoTrk_CBSig   ("Toy_Sig_IsoTrk_CBSig"  , "Toy_Sig_IsoTrk_CBSig"  , 1.39755, 0.1, 10, "Gev");
RooRealVar Toy_Sig_IsoTrk_CBn     ("Toy_Sig_IsoTrk_CBn"    , "Toy_Sig_IsoTrk_CBn"    , 1.81178, 0, 10, "Gev");
RooRealVar Toy_Sig_IsoTrk_CBalpha ("Toy_Sig_IsoTrk_CBalpha", "Toy_Sig_IsoTrk_CBalpha", 1.59337, 0, 3, "Gev");
RooCBShape Toy_Sig_IsoTrk_CB ("Toy_Sig_IsoTrk_CB", "Toy_Sig_IsoTrk_CB", Reco_mass ,
                         Toy_Sig_IsoTrk_mean, Toy_Sig_IsoTrk_CBSig  ,
                         Toy_Sig_IsoTrk_CBn , Toy_Sig_IsoTrk_CBalpha);

RooRealVar Toy_Sig_IsoTrk_Frac( "Toy_Sig_IsoTrk_Frac", "Toy_Sig_IsoTrk_Frac" , 0.325142, 0.0, 1.0, "." );

RooAddPdf  Toy_Sig_IsoTrk_Model  ( "Toy_Sig_IsoTrk_Model", "Toy_Sig_IsoTrk_Model ",
                            RooArgList(Toy_Sig_IsoTrk_GaussSum, Toy_Sig_IsoTrk_CB), Toy_Sig_IsoTrk_Frac );
