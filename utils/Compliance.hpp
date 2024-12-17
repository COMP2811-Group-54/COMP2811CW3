#pragma once

#include <string>
#include <unordered_map>
#include <QStyledItemDelegate>
#include <QPainter>

using namespace std;

// ComplianceChecker objects are able to check the compliance
// of all chemicals as they store all the thresholds
class ComplianceChecker {
public:
    ComplianceChecker();

    static std::vector<std::string> getPFAs() {
        return {
            "FOSA",
            "Perfluorooctanoic",
            "PFoctncAcid",
            "PFHxSA",
            "PFdecncAcid",
            "PFhexncAcid",
            "PFpentncAcid"
        };
    }

    static std::vector<std::string> getPOPs() {
        return {
            "PCB Con 052",
            "DDT (PP)",
            "Hexachlorbnz",
            "Chlrdn-cs/Z/",
            "Aldrin",
            "Dieldrin",
            "Endrin",
            "Heptachlor",
            "Carbofuran"
        };
    }

    static std::vector<std::string> getMetals() {
        return {
            "As-Filtered",
            "Chromium -Cr",
            "Copper - Cu",
            "Iron - as Fe",
            "Manganse-Mn",
            "Zinc - as Zn"
        };
    };

    static std::vector<std::string> getVOCs() {
        return {
            "Ethylbenzene",
            "o-Xylene",
            "Chloroform",
            "Trichloroeth",
            "TetClEthene",
            "Vinyl Cl",
            "Carbon Tet",
            "DiClMe"
        };
    };

    static std::vector<std::string> getNutrients() {
        return {
            "Nitrate-N",
            "Ammonia(N)",
            "Phosphorus-P"
        };
    }

    static std::vector<std::string> getOrganicChemicals() {
        return {
            "Carbendazim",
            "Chlorothalnl",
            "Cypermethrin",
            "Diazinon",
            "Dimethoate",
            "Glyphosate",
            "Linuron",
            "Mecoprop",
            "Pendimethaln",
            "Permethrn-cs",
            "Methiocarb",
            "BenzButPhtha",
            "1122TetClEth",
            "Toluene",
            "24Dichloropl",
            "Phenol",
            "Triclosan"
        };
    }

    static std::vector<std::string> getInOrganicChemicals() {
        return {
            "Chlorine",
            "Cyanide - CN",
            "Fluoride - F",
            "SiO2 Rv",
            "Bicarb HCO3"
        };
    }

    int complianceCheck(const string &name, double value) const;

    double getComplianceThreshold(const string &name) const {
        return complianceThresholds.at(name);
    }

    unordered_map<string, double> complianceThresholds;

    // Green < 0.8
    // Orange: 0.8-1
    // Red > 1
private:
    int overThreshold = 3;
    int atThreshold = 2;
    int underThreshold = 1;
    double thresholdTolerance = 1.2;
};

// ComplianceDelegate class for custom rendering based on compliance
class ComplianceDelegate : public QStyledItemDelegate {
public:
    explicit ComplianceDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private:
    ComplianceChecker complianceChecker; // Use ComplianceChecker for compliance logic
};
