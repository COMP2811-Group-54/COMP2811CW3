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

    static std::vector<std::string> getLocations() {
        return {

        };
    }

    int complianceCheck(const string &name, double value) const;

    double getComplianceThreshold(const string &name) const { return complianceThresholds.at(name); }
    unordered_map<string, double> complianceThresholds;

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
