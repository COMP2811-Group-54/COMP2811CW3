#pragma once

#include <string>
#include <unordered_map>

using namespace std;

// ComplianceChecker objects are able to check the compliance
// of all chemicals as they store all the thresholds
class ComplianceChecker
{
public:
    ComplianceChecker();
    bool complianceCheck(string name, double value) const;
    double getComplianceThreshold(string name) const { return complianceThresholds.at(name); }

private:
    unordered_map<string, double> complianceThresholds;
};
