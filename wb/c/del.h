/* This file was generated by scm2c from source file "del.scm" */

extern int del_defer_block_deletes_P;

SCM_EXPORT int del_delete_bck P((ENTRY *ent));

SCM_EXPORT int parent_delete_update_P P((SEGD *seg,long top_id,int level,long old_id,unsigned char *key_str,int k_len));

SCM_EXPORT int del_chain_rem P((ENTRY *ent,unsigned char *key_str,int k_len,unsigned char *ans_str,int *pkt,int wcb));
