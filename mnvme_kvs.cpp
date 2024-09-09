#include <errno.h>
#include <iostream>
#include "mnvme_kvs.h"
#include <string.h>
#include <sys/mman.h>
#include <linux/module.h>
#include <linux/kernel.h>


static void* setValue(__u32& value_len, __le64& prp1, __le64& prp2, std::string& value){
	void* buf = malloc(value_len);
	if(value == SPECIAL_VALUE){
		value_len = PAGE_SIZE_IN_WORD;
		memset(buf, 0, value_len);
	} else{
		value_len = (value.length() / BYTES_PER_WORD) + 1;
		memcpy(buf, value.c_str(), value.length());
	}
	prp1 = (__le64)(uintptr_t)buf;
	return buf;
}

static bool setKey(__u8& key_len, char* keyToSend, const std::string& key){
	key_len = (key.length());
	strcpy(keyToSend, key.c_str());

	return true;
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
}

std::string MNVME::mnvme_kvs_retreive(std::string& key)
{
	struct nvme_kv_command* cmd = (struct nvme_kv_command*)malloc(sizeof(struct nvme_kv_command));
	cmd->kv_retrieve = {
		.opcode = MNVME_SPEC_KV_OPC_RETRIEVE,
		.nsid = (unsigned int)nvme_ns_get_nsid(ns),
	};

	std::string specialValue = std::string(SPECIAL_VALUE);

	void* rbuf;
	if(!(rbuf = setValue(cmd->kv_store.value_len, cmd->kv_store.prp1, (__le64&)cmd->kv_store.prp2, specialValue))){
		std::cout << "Error in store : setValue - " << rbuf << std::endl;
	}
	if(!setKey(cmd->kv_retrieve.key_len, cmd->kv_retrieve.key, key)){
		std::cout << "Error in retreive : setKey - " << key << std::endl;
	}

	cmd->kv_retrieve.data_len = cmd->kv_retrieve.value_len * BYTES_PER_WORD;

	int err = ioctl(nvme_ns_get_fd(ns), NVME_IOCTL_IO_CMD, cmd);
	if(err != 0){
		std::cout << "Error in retreive : ioctl - " << errno << std::endl;
	}

	std::string retValue((const char*)rbuf);
	free(rbuf);
	free(cmd);
	return retValue;
}

int MNVME::mnvme_kvs_store(std::string& key, std::string& value)
{
	struct nvme_kv_command* cmd = (struct nvme_kv_command*)malloc(sizeof(struct nvme_kv_command));
	cmd->kv_store = {
		.opcode = MNVME_SPEC_KV_OPC_STORE,
		.nsid = (unsigned int)nvme_ns_get_nsid(ns),
	};

	void* dbuf;
	if(!(dbuf = setValue(cmd->kv_store.value_len, cmd->kv_store.prp1, (__le64&)cmd->kv_store.prp2, value))){
		std::cout << "Error in store : setValue - " << value << std::endl;
	}
	if(!setKey(cmd->kv_store.key_len, cmd->kv_store.key, key)){
		std::cout << "Error in store : setKey - " << key << std::endl;
	}

	cmd->kv_store.data_len = cmd->kv_store.value_len * BYTES_PER_WORD;

	int err = ioctl(nvme_ns_get_fd(ns), NVME_IOCTL_IO_CMD, cmd);
	if(err != 0){
		std::cout << "Error in store : ioctl - " << errno << std::endl;
	}
	
	free(dbuf);
	free(cmd);
	return err;
}

int MNVME::mnvme_kvs_exists(const std::string &key)
{
    return 0;
}
