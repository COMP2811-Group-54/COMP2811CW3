#include "compliance.hpp"

// Constructor populates map with pollutant names and thresholds
ComplianceChecker::ComplianceChecker() 
{
    complianceThresholds = {
        // PFAs
        {"Perfluorooctanesulfonamide", -1},
        {"Perfluorooctanoic", -1},
        {"Perfluorooctanoic Acid", -1},
        {"Perfluorohaxanesulfonamide", -1},
        {"Perfluorodecanoic Acid", -1},
        {"Perfluorohexanoic", -1},
        {"Perfluoropentanoic", -1},

        // POPs
        {"PCB", -1},
        {"DDT", -1},
        {"Hexachlorobenzene", -1},
        {"Cis-chlordane", -1},
        {"Aldrin", -1},
        {"Dieldrin", -1},
        {"Endrin", -1},
        {"Heptachlor", -1},
        {"Carbofuran", -1},

        // Heavy Metals
        {"Arsenic", 50},
        {"Chromium(III)", 4.7},
        {"Chromium(IV)", 3.4},
        {"Copper", 1},
        {"Iron", 1},
        {"Maganese", 123},
        {"Zinc", 10.9},

        // Organic Chemicals
        {"Carbenazim", 0.15},
        {"Chlorothalonil", 0.035},
        {"Cypermethrin", 0.1},
        {"Diazinon", 0.01},
        {"Dimethoate", 0.48},
        {"Glyphosate", 196},
        {"Linuron", 0.5},
        {"Mecoprop", 18},
        {"Pendimethalin", 0.3},
        {"Permethrin", 0.001},
        {"Methiocarb", 0.01},
        {"Benzyl butyl phthalate", 7.5},
        {"Tetrachloroethene", 140},
        {"Toluene", 74},
        {"2,4-dichlorophenol", 4.2},
        {"Phenol", 7.7},
        {"Triclosan", 0.1},
        {"Chlorine", 2},
        {"Cyanide", 1},
        {"Fluoride", -1},
        {"Silica", -1},
        {"Bicorbonate", -1},

        // Nutrients
        {"Nitrate", -1},
        {"Ammonia", -1},
        {"Phosphate", -1},
        {"Phosphorus", -1},

        // Volatile Organic Compounds
        {"Benzene", -1},
        {"Ethylbenzene", -1},
        {"Xylenes", -1},
        {"Chloroform", -1},
        {"Trichloroethylene", -1},

        {"Tetrachloroethylene", -1},
        {"Perchloroethylene", -1},

        {"Vinyl Chloride", -1},
        {"Carbon Tetrachloride", -1},

        {"Dichloromethane", -1},
        {"Methylene Chloride", -1}
    };
}

// Checks if a measurement value is compliant
bool ComplianceChecker::complianceCheck(string name, double value) const
{
    double threshold = complianceThresholds.at(name);

    return threshold > value;
}


