class cutParams
{
    public: //dichiaro pubbliche le costanti così posso accedere direttamente
        static const double massMuMuG_min; // cm/ps
        static const double massMuMuG_max;
        static const double massJPsi_diff;
        static const double muonPt_min;
        static const double photonPt_min;
        static const double JPsiGAngle_min;
        static const double JPsiGAngle_max;
        static const double MuMuAngle_min;
        static const double MuMuAngle_max;
};

const double cutParams::massMuMuG_min  = 20;;
const double cutParams::massMuMuG_max  = 150;
const double cutParams::massJPsi_diff  = 0.09;
const double cutParams::muonPt_min     = 20;
const double cutParams::photonPt_min   = 30;
const double cutParams::JPsiGAngle_min = -1;
const double cutParams::JPsiGAngle_max = 0.95;
const double cutParams::MuMuAngle_min  = 0.95;
const double cutParams::MuMuAngle_max  = 1;