#pragma once

#include <string>
#include <unordered_map>

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

    int complianceCheck(const string &name, double value) const;

    double getComplianceThreshold(const string &name) const { return complianceThresholds.at(name); }
    unordered_map<string, double> complianceThresholds;

private:
    int overThreshold = 3;
    int atThreshold = 2;
    int underThreshold = 1;
    double thresholdTolerance = 1.2;
};
