/* This file was generated by scm2java from source file "db.scm" */
/*  "db.scm" WB-tree File Based Associative String Data Base System. */
/*  Copyright (C) 1991, 1992, 1993, 2000, 2010 Free Software Foundation, Inc. */
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

/*  Advertised Java and C# API */



package wb;

import static wb.SchlepRT.*;
import static wb.Pkt.*;
import static wb.Wbsys.*;
import static wb.Blink.*;
import static wb.Segs.*;
import static wb.Handle.*;
import static wb.Ents.*;
import static wb.Wbdefs.*;
import static wb.Han.*;
import static wb.Scan.*;
public class Db {


public static Han createBt(Seg seg,int typ,int wcb)
{
  {
    Han aHan = makeHand();
    int ans = btCreate(seg, typ, aHan, wcb);
    if (err_P(ans))
      return null;
    else return aHan;
  }
}


public static Han openBt(Seg seg,int blknum,int wcb)
{
  {
    Han aHan = makeHand();
    int ans = btOpen(seg, blknum, aHan, wcb);
    if (err_P(ans))
      return null;
    else return aHan;
  }
}


public static void closeBt(Han han)
{
  btClose(han);
  
}


public static Han createDb(Seg seg,int typ,byte []nameStr)
{
  byte []tmpStr = new byte[256];
  Han aHan = createBt(seg, typ, 0);
  Han dHan = openBt(seg, 1, (wcbSap)+(wcbSar));
  if ((!(a2b(aHan))
      || !(a2b(dHan))))
    return null;
  else {
    long2str(tmpStr, 1, han_Id(aHan));
    tmpStr[0] = (byte) (255&4);
    btPut(dHan, nameStr, nameStr.length, tmpStr, 5);
    closeBt(dHan);
    return aHan;
  }
}


public static Han openDb(Seg seg,byte []nameStr)
{
  byte []tmpStr = new byte[256];
  Han dHan = openBt(seg, 1, (wcbSap)+(wcbSar));
  if (!(a2b(dHan)))
    return null;
  else {
    int tlen = btGet(dHan, nameStr, nameStr.length, tmpStr);
    closeBt(dHan);
    if (err_P(tlen))
      return null;
    else if ((tlen)==5)
      return openBt(seg, str2long(tmpStr, 1), 0);
    else return null;
  }
}


public static int flushEnts(int attempts,int k)
{
  return entsFlush(attempts, k);
}

/*  get returns a string of the value or #f */


public static byte []bt_Get(Han han,byte []key)
{
  {
    byte []tmpStr = new byte[256];
    int tlen = btGet(han, key, key.length, tmpStr);
    if (err_P(tlen))
      return null;
    else return subbytes(tmpStr, 0, tlen);
  }
}

/*  next returns a string of the next key or #f if at end. */
/*  (bt:next blk #f) returns the first key. */


public static byte []bt_Next(Han han,byte []key)
{
  byte []tmpStr = new byte[256];
  {
    int tlen = (a2b(key)
      ?btNext(han, key, key.length, tmpStr)
      :btNext(han, noByts, startOfChain, tmpStr));
    if (err_P(tlen))
      return null;
    else return subbytes(tmpStr, 0, tlen);
  }
}

/*  prev returns a string of the previous key or #f if at end. */
/*  (bt:prev blk #f) returns the last key. */


public static byte []bt_Prev(Han han,byte []key)
{
  byte []tmpStr = new byte[256];
  if (a2b(key)
      && 0==(key.length))
    System.err.print(">>>>ERROR<<<< "+("bt_Prev")+" of \"\".\n");
  {
    int tlen = (a2b(key)
      ?btPrev(han, key, key.length, tmpStr)
      :btPrev(han, noByts, endOfChain, tmpStr));
    if (err_P(tlen))
      return null;
    else return subbytes(tmpStr, 0, tlen);
  }
}

/*  put adds an key value pair to the database whose root is blk */


public static void bt_Put(Han han,byte []key,byte []valStr)
{
  btPut(han, key, key.length, valStr, valStr.length);
  return;
}

/*  rem! removes key and value.  returns #t if found, #f if not. */


public static boolean bt_Del(Han han,byte []key)
{
  return success_P(btRem(han, key, key.length, null));
}


public static boolean bt_Insert(Han han,byte []key,byte []valStr)
{
  return success_P(btWrite(han, key, key.length, valStr, valStr.length));
}


public static byte []bt_Rem(Han han,byte []key)
{
  {
    byte []tmpStr = new byte[256];
    int tlen = btRem(han, key, key.length, tmpStr);
    if (err_P(tlen))
      return null;
    else return subbytes(tmpStr, 0, tlen);
  }
}


public static int bt_Delete(Han han,byte []key,byte []key2)
{
  byte []tmpstr = new byte[256];
  int klen = (a2b(key)
    ?key.length
    :startOfChain);
  int klen2 = (a2b(key2)
    ?key2.length
    :endOfChain);
  if (0 < (klen))
    subbytesMove(key, 0, klen, tmpstr, 0);
  return btRemRange(han, tmpstr, klen, key2, klen2);
}

/*  TBD, create second version of bt:scan which returns count (no blklimit). */


public static byte []bt_Scan(Han bthan,int op,byte []kstr1,byte []kstr2,java.lang.reflect.Method func,int blklimit)
{
  int len1 = 0;
  int len2 = 0;
  if (a2b(kstr1))
    {
      len1 = kstr1.length;
    }
  else {
    len1 = startOfChain;
    kstr1 = noByts;
  }
  if (a2b(kstr2)
      && 0==(len2))
    System.err.print(">>>>ERROR<<<< "+("bt_Scan")+" end key is \"\".\n");
  if (a2b(kstr2))
    {
      len2 = kstr2.length;
    }
  else {
    len2 = endOfChain;
    kstr2 = noByts;
  }
  {
    byte []key1 = new byte[256];
    int []respkt = new int[pktSize];
    pkt_SetSkeyCount(respkt, 0);
    pkt_SetSkeyLen(respkt, len1);
    subbytesMove(kstr1, 0, len1, key1, 0);
    {
      int res = btScan(bthan, op, key1, len1, kstr2, len2, func, null, respkt, blklimit);
      if (err_P(res))
	return null;
      else return subbytes(key1, 0, pkt_SkeyLen(respkt));
    }
  }
}



/* Wrapper functions for db access that take strings instead
   of byte arrays */

public static Han createDb(Seg seg, int typ, String nameStr)
{
    return createDb(seg, typ, stringToBytes(nameStr));
}

public static Han createDb(Seg seg, char typ, String nameStr)
{
    return createDb(seg, (int)typ, stringToBytes(nameStr));
}

public static Han openDb(Seg seg, String nameStr)
{
    return openDb(seg, stringToBytes(nameStr));
}

public static void bt_Delete(Han han, String keyStr, String key2Str)
{
    bt_Delete(han, stringToBytes(keyStr), stringToBytes(key2Str));
}

public static String bt_Get(Han han, String keyStr)
{
    byte[] byts = bt_Get(han, stringToBytes(keyStr));
    return bytesToString(byts);
}

public static String bt_Next(Han han, String keyStr)
{
    byte[] byts = bt_Next(han, stringToBytes(keyStr));
    return bytesToString(byts);
}

public static String bt_Prev(Han han, String keyStr)
{
    byte[] byts =  bt_Prev(han, stringToBytes(keyStr));
    return bytesToString(byts);
}

public static void bt_Put(Han han, String keyStr, String key2Str)
{
    bt_Put(han, stringToBytes(keyStr), stringToBytes(key2Str));
}

public static String bt_Rem(Han han, String keyStr)
{
    byte[] byts =  bt_Rem(han, stringToBytes(keyStr));
    return bytesToString(byts);
}

}
