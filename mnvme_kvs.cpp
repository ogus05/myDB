#include <errno.h>
#include <iostream>
#include "mnvme_kvs.h"
#include <cmath>
#include <string.h>


static bool setValue(__u32& value_len, __le64& prp1, __le64& prp2, const std::string* value){
	if(value->length() != VALUE_SIZE_IN_BYTES){
		return false;
	}
	prp1 = ((uint32_t)(value) & 0xffffffff);
	if(VALUE_SIZE_IN_BYTES > 31){
		prp2 = ((uint32_t)((uint64_t)value >> 31));
	}
	value_len = (value->length() / BYTES_PER_WORD) - 1;
}

static bool setKey(__u8& key_len, char* keyToSend, const std::string& key){
	if(key.length() != KEY_SIZE_IN_BYTES){
		return false;
	}
	key_len = (sizeof(key) / BYTES_PER_WORD) - 1;
	strcpy(keyToSend, key.c_str());
}

MNVME::MNVME(const std::string& ctrl)
{
	nvme_subsystem_t s;
	nvme_ctrl_t c;
	nvme_path_t p;

	this->root = nvme_create_root(NULL, DEFAULT_LOGLEVEL);
	if (!this->root){
		std::cerr << "Error in create root" << std::endl;
		exit(1);
	}
	else{
		std::cout << "Root has completly been created" << std::endl;
	}

	c = nvme_scan_ctrl(this->root, ctrl.c_str());

	if (!c) {
		std::cerr << "Error in scan controller" << std::endl;
		exit(1);
	} else{
		std::cout << "Scanning ctlr is completly conducted" << std::endl;
	}

	this->ns = nvme_ctrl_first_ns(c);
}

MNVME::~MNVME()
{
	nvme_free_ctrl(nvme_ns_get_ctrl(ns));
	nvme_free_ns(ns);
	nvme_free_tree(root);
}

int MNVME::mnvme_kvs_retreive(const std::string &key, const std::string *value)
{
	uint32_t* result;

	struct nvme_kv_command cmd;

	cmd.kv_retrieve = {
		.opcode = MNVME_SPEC_KV_OPC_RETRIEVE,
		.nsid = (unsigned int)nvme_ns_get_nsid(ns),
	};
	if(setValue(cmd.kv_retrieve.value_len, cmd.kv_retrieve.dptr.prp1, cmd.kv_retrieve.dptr.prp2, value)){
		std::cout << "Error in retreive : setValue - " << *value << std::endl;
	};
	if(setKey(cmd.kv_retrieve.key_len, cmd.kv_retrieve.key, key)){
		std::cout << "Error in retreive : setKey - " << key << std::endl;
	}

	std::cout << "retreive value...." << std::endl;
	int err = nvme_submit_io_passthru(nvme_ns_get_fd(ns), (nvme_passthru_cmd*)&cmd, result);
	printf("retreive result : %d\n", *result);

	return err;
}

int MNVME::mnvme_kvs_store(const std::string& key, const std::string* value)
{
	uint32_t* result;

	struct nvme_kv_command cmd;

	cmd.kv_store = {
		.opcode = MNVME_SPEC_KV_OPC_STORE,
		.nsid = (unsigned int)nvme_ns_get_nsid(ns),
	};
	if(setValue(cmd.kv_store.value_len, cmd.kv_store.dptr.prp1, cmd.kv_store.dptr.prp2, value)){
		std::cout << "Error in store : setValue - " << *value << std::endl;
	};
	if(setKey(cmd.kv_store.key_len, cmd.kv_store.key, key)){
		std::cout << "Error in store : setKey - " << key << std::endl;
	}

	std::cout << "store value...." << std::endl;
	int err = nvme_submit_io_passthru(nvme_ns_get_fd(ns), (nvme_passthru_cmd*)&cmd, result);
	printf("store result : %d\n", *result);

	return err;
}

int MNVME::mnvme_kvs_exists(const std::string &key)
{
    return 0;
}
