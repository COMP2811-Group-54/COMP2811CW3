#pragma once

#include <QObject>
#include "DataModel.hpp"
#include "Dataset.hpp"


// Instance of DataModel that is publicly accessible

class GlobalDataModel : public QObject {
    Q_OBJECT

public:
    static GlobalDataModel &instance();

    DataModel &getDataModel();

    Dataset &getDataset();

    void loadData(const std::string &filePath);

    void emitFetchesComplete();

signals:
    void dataReady();

    void allRequestsComplete();

private:
    GlobalDataModel();

    ~GlobalDataModel() = default;

    GlobalDataModel(const GlobalDataModel &) = delete;

    GlobalDataModel &operator=(const GlobalDataModel &) = delete;

    DataModel dataModel;
    Dataset dataset;
};
