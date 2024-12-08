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

    // List of locations with more than 3 PFAs
    static std::vector<std::string> getLocations() {
        return {
            "ACRON FARM, DRIFFIELD",
            "BRIMHAM HALL FARM HARROGATE 3094",
            "BURTIS BECK CARR HOUSE",
            "CLIVE SOANES BROILERS",
            "COMMON FARM 2/26/31/009",
            "CORPSLANDING FARM 2/26/31/010",
            "COVER AT COVERHAM",
            "CRESKELD HALL FARM ARTHINGTON 0313",
            "CRIMPLE BECK AT ALMSFORD BRIDGE",
            "CRIMPLE BECK AT BLACKSTONES",
            "CRODA CHEMICALS 2/27/09/120",
            "DARRINGTON GOLF CLUB",
            "DERWENT AT LOFTSOME BRIDGE",
            "DON AT HALFPENNY BRIDGE",
            "ELMSWELL BECK AT SPRINGWELL FARM",
            "ELSLACK SPRING 2/27/15/209",
            "ESK AT RUSWARP - BANKSIDE",
            "GAYLE BECK AT GAYLE",
            "GRAINHOLME FARM, GUNNERSIDE, RICHMOND",
            "GUNDRIFS BECK AT RIBSTON PARK",
            "GYPSEY RACE AT RUDSTON",
            "HAYWOLD FARM",
            "HIGH EWECOTE, CHOPGATE",
            "HIGH JERVAULX FARM MASHAM 5236",
            "HOLMFIRTH DYERS LTD, DUNFORD RD",
            "HOME FARM WATH RIPON 6009",
            "JAGGER WELL",
            "KEX BECK D/S KIRKBY MALZEARD",
            "KILPIN PIKE 2/27/24/111",
            "KIPLINGCOTES FARM 2/26/32/159",
            "KYLE AT RASKELF BRIDGE",
            "LITTLE LANGTON GRANGE NORTHALLERTON 2446",
            "LOW FARM 2/26/32/181",
            "LOWER HEIGHTS FARM THRESHFIELD 5577",
            "LUND WOLD HOUSE - LUND",
            "LYLANDS FARM - LITTLE OUSEBURN - 1016(1)",
            "MALHAM TARN FSC",
            "MC CAINS RAW (CONSTANT PUMPING)",
            "MOUNT PLEASANT",
            "NEW HALL FARM BOLTON ABBEY 6051",
            "NEWBY FOODS SE363883",
            "NIDD AT CATTAL",
            "OUSE AT LINTON LOCK",
            "OUSE AT NABURN LOCK",
            "OUSE AT NETHER POPPLETON (SKELTON BANK)",
            "OUSE AT SKELDERGATE BRIDGE",
            "PARK BECK AT SHAW BRIDGE - SPOFFORTH",
            "POPLAR NURSERIES 2/26/32/343",
            "RIVELIN LODGE SPRING 2/27/05/120",
            "ROTHER AT CANKLOW",
            "SARIA LTD",
            "SEAL HOUSES, ARKENGARTHDALE",
            "SOUR BECK - BEECHWOOD GROVE",
            "SWINITHWAITE HALL 4461   LEYBURN",
            "TANGLEWOOD SPRING BUCKDEN",
            "THE HAGG, FREMINGTON, RICHMOND",
            "THIRSK RACECOURSE",
            "TOWER BREWERY TADCASTER",
            "VIEWLY HILL FARM, HIGH WORSALL, YARM",
            "WANSFORD FISH FARM",
            "WAYWORTH FARM, GUISBOROUGH",
            "WEST SCRAFTON WEST SCRAFTON LEYBURN 0630",
            "WESTWOOD FARM WEST TANFIELD RIPON 094"
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
