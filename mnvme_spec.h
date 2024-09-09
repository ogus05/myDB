#pragma once

#define BYTES_PER_WORD 4
#define PAGE_SIZE_IN_WORD 1024
#define SPECIAL_VALUE "ohs"

enum MVNME_KVS_OPS {
	MNVME_SPEC_KV_OPC_STORE    = 0x81,
	MNVME_SPEC_KV_OPC_RETRIEVE = 0x90,
	MNVME_SPEC_KV_OPC_DELETE   = 0xA1,
	MNVME_SPEC_KV_OPC_EXIST    = 0xB3,
	MNVME_SPEC_KV_OPC_LIST     = 0x85,
};

struct nvme_common_command {
	__u8 opcode;
	__u8 flags;	//fused op : 2, reserved : 4, psdt : 2
	__u16 command_id;

	__le32 nsid;

	__le32 cdw2[2];

	__le64 metadata;

	__le64 prp1;
	
	union {
		struct{
			__u32 metadata_len;
			__u32 data_len;
		};
		struct{
			__le64 prp2;
		};
	};
	
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
	__le64 prp1;
	union {
		struct{
			__u32 metadata_len;
			__u32 data_len;
		};
		struct{
			__le64 prp2;
		};
	};

	/* cdw 10 */
	__le32 value_len; /* size in word */

	/*cdw 11 */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u8 invalid_byte : 2;
	__u8 rsvd3 : 6;
	__u8 rsvd4;

	char key[16];
};

struct nvme_kv_append_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd;
	__le32 offset;
	__u32 rsvd2;

	/* cdw 6-9*/
	__le64 prp1;
	union {
		struct{
			__u32 metadata_len;
			__u32 data_len;
		};
		struct{
			__le64 prp2;
		};
	};

	__le32 value_len; /* size in word */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u8 invalid_byte : 2;
	__u8 rsvd3 : 6;
	__u8 rsvd4;
	char key[16];
};

struct nvme_kv_retrieve_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd;
	__le32 offset;
	__u32 rsvd2;
	
	/* cdw 6-9*/
	__le64 prp1;
	union {
		struct{
			__u32 metadata_len;
			__u32 data_len;
		};
		struct{
			__le64 prp2;
		};
	};

	__le32 value_len; /* size in word */
	__u8 key_len; /* 0 ~ 255 (keylen - 1) */
	__u8 option;
	__u16 rsvd3;
	char key[16];
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
	char key[16];
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
	char key[16];
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