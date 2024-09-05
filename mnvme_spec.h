#pragma once

#define BYTES_PER_WORD 4
#define KEY_SIZE_IN_BYTES 4
#define VALUE_SIZE_IN_BYTES 64

enum MVNME_KVS_OPS {
	MNVME_SPEC_KV_OPC_STORE    = 0x81,
	MNVME_SPEC_KV_OPC_RETRIEVE = 0x90,
	MNVME_SPEC_KV_OPC_DELETE   = 0xA1,
	MNVME_SPEC_KV_OPC_EXIST    = 0xB3,
	MNVME_SPEC_KV_OPC_LIST     = 0x85,
};


union nvme_data_ptr {
	struct {
		__le64 prp1;
		__le64 prp2;
	};
};

struct nvme_common_command {
	__u8 opcode;
	__u8 flags;	//fused op : 2, reserved : 4, psdt : 2
	__u16 command_id;

	__le32 nsid;

	__le32 cdw2[2];

	__le64 metadata;

	__le64 prp1;
	
	__le64 prp2;
	
	__le32 cdw10[6];
};

struct nvme_kv_store_command {
	/* cdw 0*/
	__u8 opcode;
	__u8 flags;
	__u16 command_id;

	/* cdw 1*/
	__le32 nsid;

	/* cdw 2-3*/
	__u64 rsvd;

	/* cdw 4*/
	__le32 offset;

	/* cdw 5*/
	__u32 rsvd2;

	/* cdw 6-9*/
	union nvme_data_ptr dptr; /* value dptr prp1,2 */

	/* cdw 10 */
	__le32 value_len; /* size in word */

	/*cdw 11 */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u8 invalid_byte : 2;
	__u8 rsvd3 : 6;
	__u8 rsvd4;

	union {
		struct {
			char key[16];
		};
		struct {
			__le64 key_prp;
			__le64 key_prp2;
		};
	};
};

struct nvme_kv_append_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd;
	__le32 offset;
	__u32 rsvd2;
	union nvme_data_ptr dptr; /* value dptr prp1,2 */
	__le32 value_len; /* size in word */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u8 invalid_byte : 2;
	__u8 rsvd3 : 6;
	__u8 rsvd4;
	union {
		struct {
			char key[16];
		};
		struct {
			__le64 key_prp;
			__le64 key_prp2;
		};
	};
};

struct nvme_kv_retrieve_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd;
	__le32 offset;
	__u32 rsvd2;
	union nvme_data_ptr dptr; /* value dptr prp1,2 */
	__le32 value_len; /* size in word */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u16 rsvd3;
	union {
		struct {
			char key[16];
		};
		struct {
			__le64 key_prp;
			__le64 key_prp2;
		};
	};
};

struct nvme_kv_delete_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd;
	__le32 offset;
	__u32 rsvd2;
	__u64 rsvd3[2];
	__le32 value_len; /* size in word */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u16 rsvd4;
	union {
		struct {
			char key[16];
		};
		struct {
			__le64 key_prp;
			__le64 key_prp2;
		};
	};
};

struct nvme_kv_exist_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd;
	__le32 offset;
	__u32 rsvd2;
	__u64 rsvd3[2];
	__le32 value_len; /* size in word */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u16 rsvd4;
	union {
		struct {
			char key[16];
		};
		struct {
			__le64 key_prp;
			__le64 key_prp2;
		};
	};
};

struct nvme_kv_command {
	union {
		struct nvme_common_command common;
		struct nvme_kv_store_command kv_store;
		struct nvme_kv_retrieve_command kv_retrieve;
		struct nvme_kv_delete_command kv_delete;
		struct nvme_kv_append_command kv_append;
		struct nvme_kv_exist_command kv_exist;
	};
};

struct mnvme_spec_kvs_cmd {
	struct {
		uint8_t opcode;
		uint8_t flags;
		uint16_t command_id;
	} cdw0;
	int nsid;
	/* cdw 02-03 */
	uint64_t key_lo;    ///< KV key bits 0:63
	uint64_t mptr;   ///< Reserverd for MPTR
	/* cdw 06-09: */ ///< DPTR -- data pointer
	uint64_t cdw06;
	uint64_t cdw08;
	uint32_t cdw10; ///< Host Buffer Size or Value Size

	/* cdw 11 */
	struct {
		uint8_t key_len;
		uint8_t so; ///< Store Option
		uint16_t rsvd;
	} cdw11;
	uint32_t cdw12;
	uint32_t cdw13;
	/* cdw 14-15 */
	uint64_t key_hi; ///< KV key bits 64:127
	uint32_t timeout_ms;
	uint32_t result;
};

