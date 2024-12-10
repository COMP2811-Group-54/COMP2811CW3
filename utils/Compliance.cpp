#include "Compliance.hpp"

// Constructor populates map with pollutant names and thresholds
ComplianceChecker::ComplianceChecker() {
    // All compliance values are in micrograms per litre
    complianceThresholds = {
        // PFAs (based off the RSoC drinking water regulations)
        {"FOSA", 0.01},
        {"Perfluorooctanoic", 0.01},
        {"PFoctncAcid", 0.01},
        {"PFHxSA", 0.01},
        {"PFdecncAcid", 0.01},
        {"PFhexncAcid", 0.01},
        {"PFpentncAcid", 0.01},
        // PFAs (based off the RSoC drinking water regulations)
        {"FOSA", 0.01},
        {"Perfluorooctanoic", 0.01},
        {"PFoctncAcid", 0.01},
        {"PFHxSA", 0.01},
        {"PFdecncAcid", 0.01},
        {"PFhexncAcid", 0.01},
        {"PFpentncAcid", 0.01},

        // POPs (effectively banned, but trace amounts are acceptable)
        {"PCB Con 052", 0.1},
        {"DDT (PP)", 0.1},
        {"Hexachlorbnz", 0.1},
        {"Chlrdn-cs/Z/", 0.1},
        {"Aldrin", 0.1},
        {"Dieldrin", 0.1},
        {"Endrin", 0.1},
        {"Heptachlor", 0.1},
        {"Carbofuran", 0.1},
        // POPs (effectively banned, but trace amounts are acceptable)
        {"PCB Con 052", 0.1},
        {"DDT (PP)", 0.1},
        {"Hexachlorbnz", 0.1},
        {"Chlrdn-cs/Z/", 0.1},
        {"Aldrin", 0.1},
        {"Dieldrin", 0.1},
        {"Endrin", 0.1},
        {"Heptachlor", 0.1},
        {"Carbofuran", 0.1},

        // // Heavy Metals
        {"As-Filtered", 50},
        {"Chromium -Cr", 4.7},
        {"Copper - Cu", 1},
        {"Iron - as Fe", 1},
        {"Manganse-Mn", 123},
        {"Zinc - as Zn", 10.9},

        // // Organic Chemicals
        // {"Carbendazim", 0.15},
        // {"Chlorothalnl", 0.035},
        // {"Cypermethrin", 0.1},
        // {"Diazinon", 0.01},
        // {"Dimethoate", 0.48},
        // {"Glyphosate", 196},
        // {"Linuron", 0.5},
        // {"Mecoprop", 18},
        // {"Pendimethaln", 0.3},
        // {"Permethrn-cs", 0.001},
        // {"Methiocarb", 0.01},
        // {"BenzButPhtha", 7.5},
        // {"1122TetClEth", 140},
        // {"Toluene", 74},
        // {"24Dichloropl", 4.2},
        // {"Phenol", 7.7},
        // {"Triclosan", 0.1},
        // {"Chlorine", 2},
        // {"Cyanide - CN", 1},
        // {"Fluoride - F", 1.5},
        // {"SiO2 Rv", 300},
        // {"Bicarb HCO3", 4000},
        // {"Fluoride - F", 1.5},
        // {"SiO2 Rv", 300},
        // {"Bicarb HCO3", 4000},
        //
        // // Nutrients
        // {"Nitrate-N", 25},
        // {"Ammonia(N)", 25},
        // {"Phosphorus-P", 75},
        // {"Nitrate-N", 25},
        // {"Ammonia(N)", 25},
        // {"Phosphorus-P", 75},
        //
        // // Volatile Organic Compounds
        {"Ethylbenzene", 300},
        {"o-Xylene", 0.1},
        {"Chloroform", 100},
        {"Trichloroeth", 40},
        {"TetClEthene", 10},
        {"Vinyl Cl", 0.5},
        {"Carbon Tet", 3},
        {"DiClMe", 0.1}
    };
}

// Checks if a measurement value is compliant
int ComplianceChecker::complianceCheck(const string &name, double value) const {
    const double threshold = complianceThresholds.at(name);

    if (value >= threshold * thresholdTolerance) {
        return overThreshold;
    }
    if (value < threshold) {
        return underThreshold;
    }
    return atThreshold;
}

// Constructor for ComplianceDelegate
ComplianceDelegate::ComplianceDelegate(QObject *parent)
    : QStyledItemDelegate(parent), complianceChecker() {
}

void ComplianceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const {
    QVariant value = index.data(Qt::DisplayRole);

    if (index.column() == 3) {
        // Assuming column 3 is for values
        QString name = index.sibling(index.row(), 0).data().toString(); // Column 0 has names
        double value = index.data().toDouble();

        int complianceStatus = complianceChecker.complianceCheck(name.toStdString(), value);
        QColor backgroundColor;

        switch (complianceStatus) {
            case 1: backgroundColor = Qt::green;
                break;
            case 2: backgroundColor = Qt::yellow;
                break;
            case 3: backgroundColor = Qt::red;
                break;
            default: backgroundColor = Qt::white;
                break;
        }

        painter->fillRect(option.rect, backgroundColor);
    }

    QStyledItemDelegate::paint(painter, option, index);
}
