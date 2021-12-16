#include "json.hpp"
#include "fileIO.hpp"
class Bootloader{
    public:
    static nlohmann::json boot(const char* fileProcess){
        return FileIO().readJson(fileProcess);
    }
};