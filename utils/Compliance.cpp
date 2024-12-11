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

        //
        // // Nutrients
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
    // Compliance levels
    if (value < 0.8) {
        return underThreshold; // Green
    }
    if (value >= 0.8 && value <= 1.0) {
        return atThreshold; // Orange
    }
    if (value > 1.0) {
        return overThreshold; // Red
    }
    return underThreshold; // Default to Green if no specific case matches
}

// Constructor for ComplianceDelegate
ComplianceDelegate::ComplianceDelegate(QObject *parent)
    : QStyledItemDelegate(parent), complianceChecker() {
}

void ComplianceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const {
    QVariant value = index.data(Qt::DisplayRole);

    if (index.column() == 3) { // Assuming column 3 is for measured values
        QString name = index.sibling(index.row(), 0).data().toString(); // Column 0 has parameter names
        double value = index.data().toDouble();

        // Get the compliance level
        int complianceStatus = complianceChecker.complianceCheck(name.toStdString(), value);

        // Determine background color based on compliance level
        QColor backgroundColor;
        switch (complianceStatus) {
            case 1: // Green
                backgroundColor = Qt::green;
            break;
            case 2: // Orange/Yellow
                backgroundColor = QColor(255, 165, 0); // RGB value for Orange
            break;
            case 3: // Red
                backgroundColor = Qt::red;
            break;
            default: // Default background color
                backgroundColor = Qt::white;
            break;
        }

        // Paint the background
        painter->fillRect(option.rect, backgroundColor);
    }

    // Draw the text as usual
    QStyledItemDelegate::paint(painter, option, index);
}