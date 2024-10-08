#include <libnvme.h>
#include <cstring>
#include <string>
#include "mnvme_spec.h"

class MNVME{
private:
    nvme_root_t root;
    nvme_ns_t ns;
    

public:
	MNVME(const std::string& ctrl);
	~MNVME();
    std::string mnvme_kvs_retreive(std::string& key);
    int mnvme_kvs_store(std::string& key, std::string& value);
    int mnvme_kvs_delete(const std::string &key);
    int mnvme_kvs_exists(const std::string &key);
    
    //TODO.
    int mnvme_kvs_list();
};