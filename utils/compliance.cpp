#include "compliance.hpp"

// Constructor populates map with pollutant names and thresholds
ComplianceChecker::ComplianceChecker() 
{
    complianceThresholds = {
        // PFAs
        {"FOSA", -1},
        {"Perfluorooctanoic", -1},
        {"PFoctncAcid", -1},
        {"PFHxSA", -1},
        {"PFdecncAcid", -1},
        {"PFhexncAcid", -1},
        {"PFpentncAcid", -1},

        // POPs
        {"PCB Con 052", -1},
        {"DDT (PP)", -1},
        {"Hexachlorbnz", -1},
        {"Chlrdn-cs/Z/", -1},
        {"Aldrin", -1},
        {"Dieldrin", -1},
        {"Endrin", -1},
        {"Heptachlor", -1},
        {"Carbofuran", -1},

        // Heavy Metals
        {"As-Filtered", 50},
        {"Chromium -Cr", 4.7},
        {"Copper - Cu", 1},
        {"Iron - as Fe", 1},
        {"Manganse-Mn", 123},
        {"Zinc - as Zn", 10.9},

        // Organic Chemicals
        {"Carbendazim", 0.15},
        {"Chlorothalnl", 0.035},
        {"Cypermethrin", 0.1},
        {"Diazinon", 0.01},
        {"Dimethoate", 0.48},
        {"Glyphosate", 196},
        {"Linuron", 0.5},
        {"Mecoprop", 18},
        {"Pendimethaln", 0.3},
        {"Permethrn-cs", 0.001},
        {"Methiocarb", 0.01},
        {"BenzButPhtha", 7.5},
        {"1122TetClEth", 140},
        {"Toluene", 74},
        {"24Dichloropl", 4.2},
        {"Phenol", 7.7},
        {"Triclosan", 0.1},
        {"Chlorine", 2},
        {"Cyanide - CN", 1},
        {"Fluoride - F", -1},
        {"SiO2 Rv", -1},
        {"Bicarb HCO3", -1},

        // Nutrients
        {"Nitrate-N", -1},
        {"Ammonia(N)", -1},
        {"Phosphorus-P", -1},

        // Volatile Organic Compounds
        {"Ethylbenzene", -1},
        {"o-Xylene", -1},
        {"Chloroform", -1},
        {"Trichloroeth", -1},
        {"TetClEthene", -1},
        {"Vinyl Cl", -1},
        {"Carbon Tet", -1},
        {"DiClMe", -1}
    };
}

// Checks if a measurement value is compliant
bool ComplianceChecker::complianceCheck(string name, double value) const
{
    double threshold = complianceThresholds.at(name);

    return threshold >= value;
}


