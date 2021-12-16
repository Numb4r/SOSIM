#include "../modules/json/json.hpp"
#include "./FileIO/fileIO.hpp"
class Bootloader{
    static nlohmann::json boot(const char* fileProcess){
        return FileIO().readJson(fileProcess);
    }
};