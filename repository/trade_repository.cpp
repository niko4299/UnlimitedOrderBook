#include "trade_repository.hpp"

TradeRepository::TradeRepository(std::string& db_path, uint32_t batch_size){
            auto file_system_db_path = std::filesystem::path(db_path);
            if(!std::filesystem::is_directory(file_system_db_path)){
                auto ok = std::filesystem::create_directory(file_system_db_path);
                if(!ok){
                    throw std::runtime_error("Couldn't create directory with path: " + db_path);
                }
            }

            rocksdb::Options options{};
            options.IncreaseParallelism();
            options.OptimizeLevelStyleCompaction();
            options.create_if_missing = true;

            auto status = rocksdb::DB::Open(options, db_path, &_db);
            if(!status.ok()){
                throw std::runtime_error("Couldn't open connection with order repository database");
            }

            _batch_size = batch_size;
}

TradeRepository::~TradeRepository(){
    delete _db;
}

bool TradeRepository::save(std::vector<Trade> trades){
    rocksdb::WriteBatch batch;
    rocksdb::WriteOptions writeOptions;

    for(auto& trade: trades){
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << trade;

        time_t now = time(0); // get the current time
        char timestamp_string[80];
        std::strftime(timestamp_string, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        batch.Put(timestamp_string, oss.str());
    }

    auto status = _db->Write(writeOptions, &batch);

    return status.ok();
}

bool TradeRepository::save(Trade trade){
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << trade;

    auto status = _db->Put(rocksdb::WriteOptions(), trade.buyer_order_id+trade.seller_order_id,oss.str());

    return status.ok();
}

std::vector<Trade> get_all_trades_between(std::time_t starting_timestamp, std::time_t ending_time_stamp){
    char upper_timestamp[80];
    std::strftime(upper_timestamp, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&ending_time_stamp));

    char lower_timestamp[80];
    std::strftime(lower_timestamp, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&starting_timestamp));

    // rocksdb::ReadOptions readOptions;
    // readOptions.iterate_upper_bound = start_slice(upper_timestamp.data(), key_string.size());
    // std::unique_ptr<rocksdb::Iterator> it(_db->NewIterator(readOptions));

    // // Iterate over the range of keys
    // for (it->Seek(lower_key); it->Valid(); it->Next()) {
    //     Slice keySlice = it->key();
    //     Slice valueSlice = it->value();
    //     std::string key(keySlice.data(), keySlice.size());
    //     std::string value(valueSlice.data(), valueSlice.size());
    //     std::cout << "key: " << key << ", value: " << value << std::endl;
    // }

    return std::vector<Trade>{};
}