/* This file was generated by scm2c from source file "segs.scm" */
/*  "segs.scm" WB-tree File Based Associative String Data Base System. */
/*  Copyright (C) 1991, 1992, 1993, 2000, 2007, 2010 Free Software Foundation, Inc. */
/*  */
/*  This program is free software: you can redistribute it and/or modify */
/*  it under the terms of the GNU Lesser General Public License as */
/*  published by the Free Software Foundation, either version 3 of the */
/*  License, or (at your option) any later version. */
/*  */
/*  This program is distributed in the hope that it will be useful, but */
/*  WITHOUT ANY WARRANTY; without even the implied warranty of */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/*  Lesser General Public License for more details. */
/*  */
/*  You should have received a copy of the GNU Lesser General Public */
/*  License along with this program.  If not, see */
/*  <http://www.gnu.org/licenses/>. */

/*  routines in this file return success codes */


#include "wbsys.h"


int init_wb(int max_num_ents_cnt,int max_num_buks,int max_blk_size)
{
  if (!(0xc<=(max_num_ents_cnt)))
    {
      dprintf((diagout,
	 ">>>>ERROR<<<< %s: %s (%d) must be %d or greater.\n",
	 "init_wb",
	 "max_num_ents_cnt",
	 max_num_ents_cnt,
	 0xc));
      return argerr;
    }
  else if (!(0xc<=(max_num_buks)))
    {
      dprintf((diagout,
	 ">>>>ERROR<<<< %s: %s (%d) must be %d or greater.\n",
	 "init_wb",
	 "max_num_buks",
	 max_num_buks,
	 0xc));
      return argerr;
    }
  else if (!(0x80<=(max_blk_size)))
    {
      dprintf((diagout,
	 ">>>>ERROR<<<< %s: %s (%d) must be %d or greater.\n",
	 "init_wb",
	 "max_blk_size",
	 max_blk_size,
	 0x80));
      return argerr;
    }
  else if ((lck(seg_chain_lck),try_lck_P(seg_chain_lck)))
    {
      dprintf((diagout,
	 ">>>>ERROR<<<< %s: lck (mutex) is not self-blocking!\n",
	 "init_wb"));
      return strangerr;
    }
  else if (!(lck_tab))
    {
      blkio_init();
      clear_stats();
      seg_cntr = 4;
      num_buks = max_num_buks;
      blk_size = max_blk_size;
      free_buk_cntr = 0;
      flush_ent_cntr = 0;
      buk_tab = calloc(num_buks, (sizeof (void *)));
      lck_tab = calloc(num_buks, (sizeof (void *)));
      ent_tab = calloc(ent_tab_inc, (sizeof (void *)));
      {
	int i = num_buks;
	while (i) {
	  lck_tab[(i)-1] = make_lck((i)-1);
	  {
	    i = (i)-1;
	  }
	}
      }
      lck(free_ent_lck);
      {
	int i = max_num_ents_cnt;
	ENTRY *bent = 0;
	while (i) {
	  {
	    ENTRY *newent = ent_make_ent(num_ents_ct);
	    if (newent)
	      {
		ent_set_next(newent, bent);
		bent = newent;
		ent_tab[num_ents_ct] = newent;
		ent_set_tag(newent, num_ents_ct);
		num_ents_ct = 0x1L+(num_ents_ct);
		if (!((num_ents_ct)%(ent_tab_inc)))
		  {
		    ENTRY **tmp_ent_tab = realloc(ent_tab, ((ent_tab_inc)+(num_ents_ct)) * (sizeof (void *)));
		    if (tmp_ent_tab)
		      {
			ent_tab = tmp_ent_tab;
		      }
		    else i = 1;
		  }
	      }
	    else i = 1;
	  }
	  {
	    i = (i)-1;
	  }
	}
	free_ents = bent;
      }
      unlck(free_ent_lck);
      unlck(seg_chain_lck);
      return num_ents_ct;
    }
  else if ((max_blk_size)>(blk_size))
    {
      dprintf((diagout,
	 "\
>>>>ERROR<<<< %s: already initialized with smaller blk-size: %d>%d\n",
	 "init_wb",
	 max_blk_size,
	 blk_size));
      unlck(seg_chain_lck);
      return argerr;
    }
  else {
    dprintf((diagout, "%s: already initialized\n", "init_wb"));
    unlck(seg_chain_lck);
    return notpres;
  }
}


int final_wb(void)
{
  if (lck_tab)
    {
      {
	SEGD *seg = seg_chain;
	while (seg) {
	  lck(seg_chain_lck);
	  close_seg(seg, !0);
	  unlck(seg_chain_lck);
	  {
	    seg = seg_chain;
	  }
	}
      }
      {
	int i = num_ents_ct;
	while (i) {
	  free(ent_tab[ -1+(i)]);
	  ent_tab[ -1+(i)] = 0;
	  num_ents_ct =  -1+(num_ents_ct);
	  {
	    i = (i)-1;
	  }
	}
      }
      {
	int i = num_buks;
	while (i) {
	  free(lck_tab[(i)-1]);
	  lck_tab[(i)-1] = 0;
	  {
	    i = (i)-1;
	  }
	}
      }
      free(ent_tab);
      ent_tab = 0;
      free(lck_tab);
      lck_tab = 0;
      free(buk_tab);
      buk_tab = 0;
      blkio_final();
      return success;
    }
  else {
    dprintf((diagout, "%s: already finaled\n", "final_wb"));
    blkio_final();
    return success;
  }
}


int seg_cntr = 4;

SEGD *seg_chain = 0;

LCK seg_chain_lock = {0, -3, PTHREAD_MUTEX_INITIALIZER};
LCK *seg_chain_lck = &seg_chain_lock;
/*  Create segment-descriptor and splice into seg-chain. */

SEGD *new_seg(unsigned char *filename,char *caller)
{
  if (lck_tab)
    {
      lck(seg_chain_lck);
      {
	SEGD *sseg = seg_chain;
L_loop:
	{
	  if (!(sseg))
	    goto L_make_it;
	  else if ((seg_str(sseg))
	      && (!strcmp((filename),(seg_str(sseg)))))
	    {
	      dprintf((diagout,
	 ">>>>ERROR<<<< %s: segment already open to \"%s\"\n",
	 caller,
	 filename));
	      unlck(seg_chain_lck);
	      return 0;
	    }
	  else if ((seg_chain)==(seg_nxt(sseg)))
	    goto L_make_it;
	  else {
	    sseg = seg_nxt(sseg);
	    goto L_loop;
	  }
L_make_it:
	  {
	    SEGD *seg = new_segd(seg_cntr);
	    seg_set_str(seg, strdup(filename));
	    if (seg_chain)
	      {
		SEGD *prev_seg = seg_prv(seg_chain);
		seg_set_nxt(prev_seg, seg);
		seg_set_prv(seg, prev_seg);
		seg_set_nxt(seg, seg_chain);
		seg_set_prv(seg_chain, seg);
	      }
	    else {
	      seg_set_nxt(seg, seg);
	      seg_set_prv(seg, seg);
	    }
	    seg_set_flc(seg, calloc(flc_len, (sizeof (void *))));
	    seg_set_flc_len(seg, 0);
	    seg_cntr = 1+(seg_cntr);
	    seg_chain = seg;
	    unlck(seg_chain_lck);
	    return seg;
	  }
	}
      }
    }
  else {
    dprintf((diagout,
	 ">>>>ERROR<<<< %s: WB not initialized (need to call %s)\n",
	 caller,
	 "init_wb"));
    return 0;
  }
}

/*  lock must already be held on seg-chain-lck */

void free_seg(SEGD *seg)
{
  seg_set_port(seg, 0);
  free(seg_str(seg));
  seg_set_str(seg, 0);
  free(seg_flc(seg));
  seg_set_flc(seg, 0);
  seg_set_flc_len(seg, 0);
  seg_set_used(seg, 0);
  if (!(seg_chain))
    {
      dprintf((diagout, ">>>>ERROR<<<< %s: %s already empty!\n", "free_seg", "seg_chain"));
      return;
    }
  else if ((seg_prv(seg))==(seg))
    {
      if ((seg)!=(seg_chain))
	dprintf((diagout, ">>>>ERROR<<<< %s: corrupted %s.\n", "free_seg", "seg_chain"));
      seg_chain = 0;
      return;
    }
  else {
    SEGD *next_seg = seg_nxt(seg);
    SEGD *prev_seg = seg_prv(seg);
    seg_set_nxt(prev_seg, next_seg);
    seg_set_prv(next_seg, prev_seg);
    if ((seg)==(seg_chain))
      seg_chain = 
      ((seg_chain)==(next_seg)
	?0
	:next_seg);
      return;
  }
}


int check_file_P(unsigned char *filename)
{
  int file = blkio_open_read_only_file(filename, 0x80);
  unsigned char tblk[0x80];
  if (file)
    {
      int allok_P = (blkio_read(file, tblk, 0x80, 0))
      && (!(str2long(tblk, 0)))
      && (!(str2long(tblk, 4)))
      && (!(str2long(tblk, 8)))
      && (0x77==((((unsigned char*)(tblk))[0x17])))
      && (0x62==((((unsigned char*)(tblk))[0x18])));
      blkio_file_close(file, 0x80, !(!0));
      return allok_P;
    }
  else return 0;
}


SEGD *open_seg(unsigned char *filename,int mutable_P)
{
  return open_segd(filename, mutable_P, !(!0));
}


SEGD *open_segd(unsigned char *filename,int mutable_P,int even_if_dirty_P)
{
  int bsiz = 3*0x80;
  if ((bsiz)>(blk_size))
    {
      dprintf((diagout, ">>>>ERROR<<<< unsupported %s %d > %d\n", "bsiz", bsiz, blk_size));
      return 0;
    }
  else if (!(check_file_P(filename)))
    return 0;
  else {
    SEGD *seg = new_seg(filename, "open_seg");
    char *reason;
    if (!(seg))
      return seg;
    else {
      int file = mutable_P
	?blkio_open_modify_file(filename, bsiz)
	:blkio_open_read_only_file(filename, bsiz);
L_loop:
      if (blkio_port_open_P(file, mutable_P))
	{
	  seg_set_port(seg, file);
	  seg_set_bsiz(seg, bsiz);
	  seg_set_used(seg, 2);
	  seg_set_flc_len(seg, 
	    (mutable_P
	      ? -1
	      : -2));
	  {
	    HAND *han = seg_rt_han(seg);
	    unsigned char tmp_str[0x100];
	    if (err_P(bt_open(seg, 0, han, (wcb_sap)+(wcb_sar))))
	      {
		reason = "bt-open 0";
		goto L_errout;
	      }
	    else if (2!=(bt_get(han, bsiz_byts, 4, tmp_str)))
	      {
		reason = "BSIZ";
		goto L_errout;
	      }
	    else if ((bsiz)!=(str2short(tmp_str, 0)))
	      {
		blkio_file_close(file, bsiz, mutable_P);
		bsiz = str2short(tmp_str, 0);
		if ((bsiz)>(blk_size))
		  {
		    reason = "BSIZ too big.";
		    goto L_errout;
		  }
		else {
		  file = mutable_P
		    ?blkio_open_modify_file(filename, bsiz)
		    :blkio_open_read_only_file(filename, bsiz);
		  goto L_loop;
		}
	      }
	    else {
	      int dty_P = (mutable_P)
	      && (err_P(bt_rem(han, cln_byts, 3, tmp_str)));
	      if ((dty_P)
		  && (!(even_if_dirty_P)))
		{
		  reason = "not cleanly saved; use wbcheck to repair.";
		  goto L_errout;
		}
	      else {
		if (dty_P)
		  dprintf((diagout, "WARNING: File \"%s\" not cleanly saved.\n", filename));
		if (4!=(bt_get(han, used_byts, 4, tmp_str)))
		  {
		    reason = "USED";
		    goto L_errout;
		  }
		else {
		  seg_set_used(seg, str2long(tmp_str, 0));
		  if (5!=(bt_get(han, fld_byts, 3, tmp_str)))
		    {
		      reason = "FLD";
		      goto L_errout;
		    }
		  else if (err_P(bt_open(seg, str2long(tmp_str, 1), seg_fl_han(seg), wcb_sar)))
		    {
		      reason = "FLC";
		      goto L_errout;
		    }
		  else {
		    if ((han_typ(seg_fl_han(seg)))!=(frl_typ))
		      dprintf((diagout, "Older type freelist - still supported.\n"));
		    han_set_wcb(seg_fl_han(seg), wcb_sar);
		    return seg;
		  }
		}
	      }
	    }
	  }
	}
      else {
	if (blkio_port_open_P(file, mutable_P))
	  blkio_file_close(file, bsiz, mutable_P);
	dprintf((diagout, ">>>>ERROR<<<< could not open file %s\n", filename));
	free_seg(seg);
	return 0;
      }
    }
L_errout:
    dprintf((diagout, ">>>>ERROR<<<< File \"%s\" %s\n", filename, reason));
    if (seg)
      {
	blkio_file_close(seg_port(seg), bsiz, mutable_P);
	free_seg(seg);
      }
    return 0;
  }
}


int close_seg(SEGD *seg,int hammer_P)
{
  if ((!(seg))
      || (!(seg_port(seg)))
      || (!(seg_str(seg)))
      || (!(seg_used(seg))))
    return notpres;
  else {
    flush_flc(seg, 5);
    /*leave only enough blocks to fit in flc in superblock.*/
    if ((seg_flc_len(seg))>=0)
      {
	unsigned char tmp_str[0x14];
	{
	  int i =  -1+(seg_flc_len(seg));
	  while (!(0 > (i))) {
	    long2str(tmp_str, 4*(i), seg_flc(seg)[i]);
	    {
	      i =  -1+(i);
	    }
	  }
	}
	bt_put(seg_rt_han(seg), flc_byts, 3, tmp_str, 4*(seg_flc_len(seg)));
	seg_set_flc_len(seg,  -1);
      }
    {
      int mutable_P = seg_mutable_P(seg);
      int ans = do_seg_buffers(seg, flush_buffer);
      if ((success_P(ans))
	  || (hammer_P))
	{
	  if (!(success_P(ans)))
	    ans = notpres;
	  do_seg_buffers(seg, purge_buffer);
	  if (mutable_P)
	    {
	      blkio_flush_to_file(seg_port(seg), !(!0));
	      if (err_P(bt_write(seg_rt_han(seg), cln_byts, 3, no_byts, 0)))
		dprintf((diagout,
	 "WARNING: mutable file \"%s\" already clean?\n",
	 seg_str(seg)));
	    }
	  bt_close(seg_rt_han(seg));
	  bt_close(seg_fl_han(seg));
	  blkio_file_close(seg_port(seg), seg_bsiz(seg), mutable_P);
	  free_seg(seg);
	}
      return ans;
    }
  }
}


unsigned char db_version_str[] = "wb-2b3";

unsigned char db_authors_str[] = "A. Jaffer, J. Finger, R. Zito-Wolf";


SEGD *make_seg(unsigned char *filename,int bsiz)
{
  SEGD *seg = new_seg(filename, "make_seg");
  if (!(seg))
    return seg;
  else if ((bsiz)>(blk_size))
    {
      dprintf((diagout, ">>>>ERROR<<<< unsupported %s %d > %d\n", "bsiz", bsiz, blk_size));
      unlck(seg_lck(seg));
      return 0;
    }
  else {
    int file = blkio_create_file(filename, bsiz);
    if (blkio_port_open_P(file, !0))
      {
	seg_set_port(seg, file);
	seg_set_bsiz(seg, bsiz);
	seg_set_used(seg, 3);
	seg_set_str(seg, strdup(filename));
	seg_set_flc_len(seg,  -1);
	{
	  HAND *rt_han = seg_rt_han(seg);
	  HAND *han = seg_fl_han(seg);
	  unsigned char tmp_str[5];
	  if ((bt_open_new(seg, 0, rt_han, (wcb_sap)+(wcb_sar), dir_typ))
	      && (bt_open_new(seg, 1, han, wcb_sap, dir_typ))
	      && (bt_open_new(seg, 2, han, wcb_sar, frl_typ)))
	    {
	      bt_put(rt_han, no_byts, 0, db_version_str, strlen(db_version_str));
	      long2str(tmp_str, 0, seg_used(seg));
	      bt_put(rt_han, used_byts, 4, tmp_str, 4);
	      short2str(tmp_str, 0, seg_bsiz(seg));
	      bt_put(rt_han, bsiz_byts, 4, tmp_str, 2);
	      tmp_str[0] = 4;
	      long2str(tmp_str, 1, 1);
	      bt_put(rt_han, root_byts, 4, tmp_str, 5);
	      long2str(tmp_str, 1, 2);
	      bt_put(rt_han, fld_byts, 3, tmp_str, 5);
	      bt_put(rt_han, flc_byts, 3, no_byts, 0);
	      if ((bsiz)>0x80)
		bt_put(rt_han, authors_byts, 7, db_authors_str, strlen(db_authors_str));
	      return seg;
	    }
	  else {
	    dprintf((diagout,
	 ">>>>ERROR<<<< couldn't allocate ents for file %s\n",
	 filename));
	    unlck(seg_lck(seg));
	    close_seg(seg, !0);
	    return 0;
	  }
	}
      }
    else {
      dprintf((diagout,
	 ">>>>ERROR<<<< couldn't create new file %s\n",
	 filename));
      unlck(seg_lck(seg));
      return 0;
    }
  }
}


int bt_open(SEGD *seg,long blk_num,HAND *han,int wcb)
{
  if ((seg)
      && (seg_str(seg)))
    {
      ENTRY *ent = get_ent(seg, blk_num, accread);
      if (!(ent))
	return argerr;
      else if (!(root_P(ent_blk(ent))))
	{
	  release_ent(ent, accread);
	  dprintf((diagout, ">>>>ERROR<<<< %s: not a root %ld:%ld\n", "bt_open", seg_id(seg), blk_num));
	  return argerr;
	}
      else {
	bt_open_init_han(han, ent, wcb);
	release_ent(ent, accread);
	return han_typ(han);
      }
    }
  else return argerr;
}


void bt_open_init_han(HAND *han,ENTRY *ent,int wcb)
{
  int typ = blk_typ(ent_blk(ent));
  SEGD *seg = ent_seg(ent);
  long blk_num = ent_id(ent);
  han_set_seg(han, seg);
  han_set_num(han, blk_num);
  han_set_last(han, blk_num);
  han_set_typ(han, typ);
  if ((typ)==(dir_typ))
    wcb = (wcb)|((wcb_sap)+(wcb_sar));
  han_set_wcb(han, wcb);
  return;
}

/*  Does calling GET-BUK-WAIT admit the possiblity of deadly embrace? */

int bt_open_new(SEGD *seg,long blk_num,HAND *han,int wcb,int typ)
{
  if (0 > (blk_num))
    {
      dprintf((diagout, ">>>>ERROR<<<< negative block number %ld\n", blk_num));
      return 0;
    }
  else {
    ENTRY *ent = try_get_free_ent(0,  -1);
    while (!(ent)) {
      {
	ent = try_get_free_ent(0,  -1);
      }
    }
    init_leaf_blk(ent_blk(ent), blk_num, typ);
    blk_set_time(ent_blk(ent), time(0L));
    ent_set_next(ent, get_buk_wait(seg_id(seg), blk_num));
    set_buk(seg_id(seg), blk_num, ent);
    ent_set_acc(ent, accwrite);
    ent_set_seg(ent, seg);
    ent_set_id(ent, blk_num);
    ent_set_age(ent,  -127);
    ent_set_dty(ent, !0);
    ent_set_pus(ent, 0);
    ent_set_ref(ent, 1);
    rel_buk(seg_id(seg), blk_num);
    bt_open_init_han(han, ent, wcb);
    ents_ent_write(ent);
    release_ent(ent, accwrite);
    return !0;
  }
}


int bt_create(SEGD *seg,int typ,HAND *han,int wcb)
{
  if ((seg)
      && (han))
    {
      ENTRY *ent = create_new_blk_ent(seg);
      if (ent)
	{
	  long blk_num = ent_id(ent);
	  init_leaf_blk(ent_blk(ent), blk_num, typ);
	  ent_set_pus(ent, 0);
	  bt_open_init_han(han, ent, wcb);
	  ents_ent_write(ent);
	  release_ent(ent, accwrite);
	  return success;
	}
      else return noroom;
    }
  else return argerr;
}


int bt_close(HAND *han)
{
  if (han)
    {
      han_set_seg(han, 0);
      han_set_num(han, 0);
      han_set_typ(han, 0);
      han_set_last(han, 0);
      return success;
    }
  else return argerr;
}
