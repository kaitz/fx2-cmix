#include "context-manager.h"
extern unsigned long long wrtcxt;

extern const unsigned char wrt_2b[256];
extern const unsigned char wrt_3b[256];
const unsigned char wrt_4b[256]={
 6, 0,12,15,12,15,14,14, 5, 3,14, 0,15,13, 8,13,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
13, 5,15,11,10,12, 6,12, 0,11,14, 1, 1,10, 9, 8,
 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 9,11, 6, 1, 0, 4,
 9,10,10, 4, 5, 1, 4, 2,11, 8, 4, 1, 0,10,10, 5,
 4, 7,15, 4, 5,13, 0, 1, 4,12, 0, 1, 3, 3, 3,11,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 8, 0,11, 7,

 
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
 };
 const unsigned char wrt_5b[256]={
 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 3, 2, 2, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  };
#define COLON         'J' // :
#define SEMICOLON     'K' // ;
#define LESSTHAN      'L' // <
#define EQUALS        'M' // =
#define GREATERTHAN   'N' // >
#define QUESTION      'O' // ?
#define FIRSTUPPER     64 // @ - wrt first char in word is in upper case
#define SQUAREOPEN     91 // [
#define BACKSLASH      92 // '\'
#define SQUARECLOSE    93 // ]
#define CURLYOPENING  'P' // {
#define VERTICALBAR   'Q' // |
#define CURLYCLOSE    'R' // }

ContextManager::ContextManager() : history_(60000000, 0),
    shared_map_(256*400000, 0), words_(8, 0), recent_bytes_(8, 0) {
    hashes_ind1.resize(0x1000000, 0);
    hashes_ind2.resize(0x1000000, 0);
    hashes_ind3.resize(0x2000000, 0);
    hashes_ind4.resize(0x100, 0);
    hashes_ind5.resize(0x100, 0);
}

void ContextManager::UpdateHistory() {
  history_[history_pos_] = bit_context_;
  ++history_pos_;
  if (history_pos_ == history_.size()) history_pos_ = 0;
}

void ContextManager::UpdateWords() {
  unsigned char c = bit_context_;
  if (c==CURLYCLOSE || c==CURLYOPENING ||c==SQUARECLOSE)    b3stream= (b3stream&0xfffffff8)+3;
  else if (c==EQUALS)  b3stream=(b3stream&0xfffffff8)+4;
  n2bState=wrt_2b[c];
  b2stream=b2stream*4+n2bState;
  n3bState=wrt_3b[c];
  b3stream=b3stream*8+n3bState;
  if (o3bState!=n3bState){
      stream3bR=(stream3bR<<3)+n3bState;
      o3bState=n3bState;
  }
  if (c==10 || c==')') b3stream=b3stream<<6;
  if (c==VERTICALBAR)  b3stream=b3stream*8+wrt_3b[c];
  b2streamcxt=b2stream&0x3ff;// 2^10 bits
  b3streamcxt=b3stream&0x1ff;// 2^9 bits
  
  if (o2bState!=n2bState){
      stream2bR=(stream2bR<<2)+n2bState;
      o2bState=n2bState;
  }
  b4stream=b4stream*16+wrt_4b[c];
  mx18cxt=mx18cxt*16+wrt_5b[c];
  mx18=mx18cxt&0xff;
        
  words=words*2;

  if ((c >= 'a' && c <= 'z') || c >= 0x80) {
    words_[7] = words_[7] * 997*16 + c;
    if (recent_bytes_[0]!=12) words=words+1;
   
  } else {
    words_[7] = 0;
  }
  if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == 8 || c == 6 ||
      c >= 0x80) {
    words_[0] = words_[0] * 997*16 + c;
    words_[0] &= 0xfffffff;
    words_[1] = words_[1] * 263*32 + c;
  } else {
    for (int i = 6; i >= 2; --i) {
      words_[i] = words_[i-1];
    }
    words_[1] = 0;
  }
   if (c==10  )     words=0xfffc;
   else if (c=='.')                 words= words|0xffc;
   else if (c==',')                 words=words|0xffc;
   mx5=b2stream&0xffff;//2^16 bits 
   mx7=    b4stream&0xff;
   mx8=(mx8*(1 << 2)+(b3stream&0x3f))& 0x3FFF;// o1, mask 2^(7*2) , n3 should be 8 bits, use 7 for now
   mx9cxt=(mx9cxt * (1 << 4) + c) &0xff;

   mx10cxt=c;
   mx11cxt=c;
   mx12cxt=0;
   mx13cxt=0;
  
   mx14 = c*256+recent_bytes_[0];
   mx15 = recent_bytes_[0]*256+recent_bytes_[1];
   
   hashes_ind1[context1_ind] = (ind1 * (1 << 8) + c) & (0x100-1);
   context1_ind = (context1_ind * (1 << 8) + c) & (0x1000000-1);
   ind1 = hashes_ind1[context1_ind];
  
    hashes_ind2[context1_ind2] = (ind2 * (1 << 8) + c) & (0x100000000-1);
   context1_ind2 = (context1_ind2 * (1 << 6) + c) & (0x1000000-1);
   ind2 = hashes_ind2[context1_ind2];
 
  hashes_ind3[context1_ind3] = (ind3 * (1 << 5) + c) & (0x2000000-1);
  context1_ind3 = (context1_ind3 * (1 << 5) + c) &     (0x2000000-1);
  ind3 = hashes_ind3[context1_ind3];
  
  hashes_ind5[context1_ind5] = (ind5 * (1 << 6) + c) & (0x40000000-1);
  context1_ind5 = c;
  ind5 = hashes_ind5[context1_ind5];
}

void ContextManager::UpdateRecentBytes() {
  for (int i = 7; i >= 1; --i) {
    recent_bytes_[i] = recent_bytes_[i-1];
  }
  recent_bytes_[0] = bit_context_;
}

void ContextManager::UpdateWRTContext() {
  if (bit_context_ < 0x80) {
    wrt_state_ = 0;
  } else {
    if (wrt_state_ == 0) wrt_context_ = 0;
    wrt_state_ = 1;
    wrt_context_ <<= 8;
    wrt_context_ += bit_context_;
    if (wrt_context_ > 0xFFEFCF) wrt_context_ = 0;
  }
}

void ContextManager::UpdateContexts(int bit) {
  bit_context_ += bit_context_ + bit;
  long_bit_context_ = bit_context_;
  if (bit_context_ >= 256) {
    bit_context_ -= 256;
    long_bit_context_ = 1;
    longest_match_ = 0;

    if (bit_context_ == '\n') {
      line_break_ = 0;
    } else if (line_break_ < 99) {
      ++line_break_;
    }

    UpdateHistory();
    UpdateWords();
    UpdateRecentBytes();
    UpdateWRTContext();

    for (auto& context : context_hash_contexts_) {
      context.Update();
    }
    for (auto& context : sparse_contexts_) {
      context.Update();
    }
    for (auto& context : bracket_contexts_) {
      context.Update();
    }
  }
  wordscxt=(words&0x7F)*256+long_bit_context_;

  bpos=(bpos+1)&7;

  mx6=(stream2bR&0xff)*256+long_bit_context_;

  mx19cxt=    wrtcxt;
  mx9=(mx9cxt)*256+long_bit_context_;
  mx10=(mx10cxt)*256+long_bit_context_;
  mx11=(mx11cxt)*256+long_bit_context_;
  mx12=long_bit_context_;
  mx13=long_bit_context_;
  mx16=(recent_bytes_[1])*256+long_bit_context_;
  mx17=(b3stream&0x3f)*256+long_bit_context_;// 7f or 3f
  
      if (bpos==0)  mxx=(stream2bR&63)*8 + (b3stream&7);
    else if (bpos>3) {
        mxx=((b2stream<<2)&63)+wrt_2b[(long_bit_context_<<(8-bpos))&255]*8+(b3stream&7);
    } else    
        mxx=(stream2bR&63)*8 +(b3stream&7);
        
}

