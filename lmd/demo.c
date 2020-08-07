#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flag.h"
#include "constant.h"
#include "lmd2.h"
#include "lmd4.h"
#include "lmd5.h"
#include "lmd6.h"
#include "lmd7.h"
#include "lmd8.h"
#include "lmd2.c"
#include "lmd4.c"
#include "lmd5.c"
#include "lmd6_32.c"
#include "lmd6_64.c"
#include "lmd7.c"
#include "lmd8.c"

void
print_status(u8 lmd_number,u8 status){
  if(status==0){
    printf("LMD%d is working.\n",lmd_number);
  }else{
    printf("ERROR: LMD%d is not working. Compiler bug? Try compiling in GCC with -O0.\n",lmd_number);
  }
  return;
}

void
print_dashes(void){
  printf("\n------------------------------------------\n");
  return;
}

void
print_note(int lmd_number){
  printf("LMD%d for comparison to Mathematica output:",lmd_number);
  print_dashes();
  return;
}

int
main(void){
  u8 bits;
  u32 block_base[LMD6_BLOCK_U32_COUNT];
  u64 block_base_64[LMD6_BLOCK_U64_COUNT];
  u32 i;
  u32 j;
  u64 lmd;
  u32 lmd_base[LMD6_U32_COUNT];
  u64 lmd_base_64[LMD6_U64_COUNT];
  u32 p_base[LMD6_U32_COUNT<<1];
  u64 p_base_64[LMD6_U64_COUNT<<1];
  u8 status;

  printf("LMD Demo, build %d\n",BUILD_NUMBER);
  printf("This is a test for correct implemention of the Leidich Message Digest,\nspecifically, LMD2 through LMD6. Special thanks to Wolfram Mathematica for\nenabling independent verification of these complex test cases.\n\n");
  printf("http://leidich-message-digest.blogspot.com\n\n");
  for(i=0;i<LMD6_BLOCK_U32_COUNT;i++){
    block_base[i]=i;
  }
  lmd=LMD_STRING_LMD2_GET((u8 *)(block_base),0,LMD6_BLOCK_SIZE);
  if(lmd==0xBFC3037579683DF3ULL){
    status=0;
  }else{
    status=1;
  }
  print_status(2,status);
  lmd=LMD_STRING_LMD3_GET((u8 *)(block_base),0,LMD6_BLOCK_SIZE);
  if(lmd==0x5A9752132554F563ULL){
    status=0;
  }else{
    status=1;
  }
  print_status(3,status);
  for(i=0;i<=((LMD4_U32_COUNT<<1)-1);i++){
    p_base[i]=0;
  }
  p_base[0]=0;
  p_base[LMD4_U32_COUNT>>1]=1;
  p_base[LMD4_U32_COUNT]=2;
  p_base[LMD4_U32_COUNT+(LMD4_U32_COUNT>>1)]=3;
  for(i=0;i<=(LMD4_BLOCK_U32_COUNT-1);i++){
    block_base[i]=0;
  }
  j=1;
  for(i=0;i<=(LMD4_BLOCK_U32_COUNT-1);i+=(LMD4_U32_COUNT>>1)){
    block_base[i]=j;
    j++;
  }
  lmd_block_lmd4_get(block_base,0,p_base,lmd_base);
  print_note(4);
  for(i=0;i<=(LMD4_U32_COUNT-1);i++){
    printf("%08x",lmd_base[LMD4_U32_COUNT-1-i]);
  }
  print_dashes();
  status=1;
  if(lmd_base[0]==0xEEA525B3){
    if(lmd_base[1]==0x040A71EE){
      if(lmd_base[2]==0xBF42E053){
        if(lmd_base[3]==0x0D7D58C6){
          if(lmd_base[4]==0xED691A7D){
            if(lmd_base[5]==0x703CEB0A){
              if(lmd_base[6]==0x0CAE3656){
                if(lmd_base[7]==0x6F2FEFEC){
                  status=0;
                }
              }
            }
          }
        }
      }
    }
  }
  print_status(4,status);
  for(i=0;i<=((LMD5_U32_COUNT<<1)-1);i++){
    p_base[i]=0;
  }
  p_base[0]=0;
  p_base[LMD5_U32_COUNT>>1]=1;
  p_base[LMD5_U32_COUNT]=2;
  p_base[LMD5_U32_COUNT+(LMD5_U32_COUNT>>1)]=3;
  for(i=0;i<=(LMD5_BLOCK_U32_COUNT-1);i++){
    block_base[i]=0;
  }
  j=1;
  for(i=0;i<=(LMD5_BLOCK_U32_COUNT-1);i+=(LMD5_U32_COUNT>>1)){
    block_base[i]=j;
    j++;
  }
  lmd_block_lmd5_get(block_base,0,p_base,lmd_base);
  print_note(5);
  for(i=0;i<=(LMD5_U32_COUNT-1);i++){
    printf("%08x",lmd_base[LMD5_U32_COUNT-1-i]);
  }
  print_dashes();
  status=1;
  if(lmd_base[0]==0x42D59230){
    if(lmd_base[1]==0xE41EA736){
      if(lmd_base[2]==0x7F9FA819){
        if(lmd_base[3]==0x2F96A786){
          if(lmd_base[4]==0x3213C04C){
            if(lmd_base[5]==0x2194810C){
              if(lmd_base[6]==0x358CD9F9){
                if(lmd_base[7]==0x52995621){
                  if(lmd_base[8]==0xD811300B){
                    if(lmd_base[9]==0x57167BA3){
                      if(lmd_base[10]==0x8146E018){
                        if(lmd_base[11]==0xD12F1D99){
                          if(lmd_base[12]==0x14704503){
                            if(lmd_base[13]==0xA20F890F){
                              if(lmd_base[14]==0xC4330243){
                                if(lmd_base[15]==0x25CACE35){
                                  status=0;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  print_status(5,status);
  for(bits=32;bits<=64;bits=(u8)(bits+32)){
    printf("Testing %d-bit version of LMD6...\n",bits);
    for(i=0;i<=((LMD6_U32_COUNT<<1)-1);i++){
      p_base[i]=0;
    }
    p_base[0]=0;
    p_base[LMD6_U32_COUNT>>1]=1;
    p_base[LMD6_U32_COUNT]=2;
    p_base[LMD6_U32_COUNT+(LMD6_U32_COUNT>>1)]=3;
    for(i=0;i<=(LMD6_BLOCK_U32_COUNT-1);i++){
      block_base[i]=0;
    }
    j=1;
    for(i=0;i<=(LMD6_BLOCK_U32_COUNT-1);i+=(LMD6_U32_COUNT>>1)){
      block_base[i]=j;
      j++;
    }
    if(bits==32){
      lmd_block_lmd6_get32(block_base,0,p_base,lmd_base);
    }else{
      memcpy((void *)(block_base_64),(const void *)(block_base),(size_t)(LMD6_BLOCK_SIZE));
      memcpy((void *)(p_base_64),(const void *)(p_base),(size_t)(LMD6_SIZE<<1));
      lmd_block_lmd6_get64((u64 *)(block_base_64),0,p_base_64,lmd_base_64);
      memcpy((void *)(lmd_base),(const void *)(lmd_base_64),(size_t)(LMD6_SIZE));
    }
    print_note(6);
    for(i=0;i<=(LMD6_U32_COUNT-1);i++){
      printf("%08x",lmd_base[LMD6_U32_COUNT-1-i]);
    }
    print_dashes();
    status=1;
    if(lmd_base[0]==0xE470C565){
      if(lmd_base[1]==0xE5C6D636){
        if(lmd_base[2]==0x566415CC){
          if(lmd_base[3]==0xCEDE240C){
            if(lmd_base[4]==0xDF75C186){
              if(lmd_base[5]==0x8BE8366E){
                if(lmd_base[6]==0x2E66F790){
                  if(lmd_base[7]==0xC080A4F3){
                    if(lmd_base[8]==0x71234550){
                      if(lmd_base[9]==0xBFF40FE5){
                        if(lmd_base[10]==0x737C3C70){
                          if(lmd_base[11]==0x95A9BD8B){
                            if(lmd_base[12]==0xF05CCBF5){
                              if(lmd_base[13]==0xB113FF81){
                                if(lmd_base[14]==0xD0005B8A){
                                  if(lmd_base[15]==0x83E22E63){
                                    if(lmd_base[16]==0xF7C9BDC5){
                                      if(lmd_base[17]==0xD228D1D1){
                                        if(lmd_base[18]==0x463C3418){
                                          if(lmd_base[19]==0x0495AEC0){
                                            if(lmd_base[20]==0x7F45D990){
                                              if(lmd_base[21]==0xE85D2986){
                                                if(lmd_base[22]==0x77C68748){
                                                  if(lmd_base[23]==0xC9F770CA){
                                                    if(lmd_base[24]==0x98EE0CD0){
                                                      if(lmd_base[25]==0xECC2A6ED){
                                                        if(lmd_base[26]==0x699452EE){
                                                          if(lmd_base[27]==0x0925B996){
                                                            if(lmd_base[28]==0x3CC31952){
                                                              if(lmd_base[29]==0x9DA4A097){
                                                                if(lmd_base[30]==0xE7B79195){
                                                                  if(lmd_base[31]==0xA5F593B3){
                                                                    status=0;
                                                                  }
                                                                }
                                                              }
                                                            }
                                                          }
                                                        }
                                                      }
                                                    }
                                                  }
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    print_status(6,status);
  }
  for(i=0;i<=((LMD7_U32_COUNT<<1)-1);i++){
    p_base[i]=0;
  }
  p_base[0]=1;
  p_base[LMD7_U32_COUNT>>1]=2;
  p_base[LMD7_U32_COUNT]=3;
  p_base[LMD7_U32_COUNT+(LMD7_U32_COUNT>>1)]=4;
  for(i=0;i<=(LMD7_BLOCK_U32_COUNT-1);i++){
    block_base[i]=0;
  }
  j=1;
  for(i=0;i<=(LMD7_BLOCK_U32_COUNT-1);i+=(LMD7_U32_COUNT>>1)){
    block_base[i]=j;
    j++;
  }
  lmd_block_lmd7_get(block_base,0,p_base,lmd_base);
  print_note(7);
  for(i=0;i<=(LMD7_U32_COUNT-1);i++){
    printf("%08x",lmd_base[LMD7_U32_COUNT-1-i]);
  }
  print_dashes();
  status=1;
  if(lmd_base[0]==0xCE63A70E){
    if(lmd_base[1]==0x3D9F6422){
      if(lmd_base[2]==0x65322561){
        if(lmd_base[3]==0x79FB724A){
          if(lmd_base[4]==0xAD49CF32){
            if(lmd_base[5]==0x03C6BEBA){
              if(lmd_base[6]==0x8DB03A6B){
                if(lmd_base[7]==0x8CAEA627){
                  if(lmd_base[8]==0xD8DEA9B2){
                    if(lmd_base[9]==0x1CAA4B86){
                      if(lmd_base[10]==0xC2E4C5EB){
                        if(lmd_base[11]==0x9878115E){
                          if(lmd_base[12]==0xA871B570){
                            if(lmd_base[13]==0xF6ADFF9F){
                              if(lmd_base[14]==0x034626D7){
                                if(lmd_base[15]==0x19B4AA63){
                                  if(lmd_base[16]==0x0DD01C5A){
                                    if(lmd_base[17]==0x9FA2D5C4){
                                      if(lmd_base[18]==0x7F1053BF){
                                        if(lmd_base[19]==0x6B7DA187){
                                          if(lmd_base[20]==0x4EA33E77){
                                            if(lmd_base[21]==0xA2C158CB){
                                              if(lmd_base[22]==0x41A40605){
                                                if(lmd_base[23]==0x8128AF2D){
                                                  if(lmd_base[24]==0x71BDB4DF){
                                                    if(lmd_base[25]==0xAFCF7C4D){
                                                      if(lmd_base[26]==0x6DFFE8C4){
                                                        if(lmd_base[27]==0x9BF25A74){
                                                          if(lmd_base[28]==0x6D00B7E1){
                                                            if(lmd_base[29]==0x672706D4){
                                                              if(lmd_base[30]==0x5B0D92DB){
                                                                if(lmd_base[31]==0x23F02B7E){
                                                                  status=0;
                                                                }
                                                              }
                                                            }
                                                          }
                                                        }
                                                      }
                                                    }
                                                  }
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  print_status(7,status);
  for(i=0;i<=((LMD8_U32_COUNT<<1)-1);i++){
    p_base[i]=0;
  }
  p_base[0]=1;
  p_base[LMD8_U32_COUNT>>1]=2;
  p_base[LMD8_U32_COUNT]=3;
  p_base[LMD8_U32_COUNT+(LMD8_U32_COUNT>>1)]=4;
  for(i=0;i<=(LMD8_BLOCK_U32_COUNT-1);i++){
    block_base[i]=0;
  }
  j=1;
  for(i=0;i<=(LMD8_BLOCK_U32_COUNT-1);i+=(LMD8_U32_COUNT>>1)){
    block_base[i]=j;
    j++;
  }
  lmd_block_lmd8_get(block_base,0,p_base,lmd_base);
  print_note(8);
  for(i=0;i<=(LMD8_U32_COUNT-1);i++){
    printf("%08x",lmd_base[LMD8_U32_COUNT-1-i]);
  }
  print_dashes();
  status=1;
  if(lmd_base[0]==0xF37A3EF6){
    if(lmd_base[1]==0x67EC5483){
      if(lmd_base[2]==0x23632045){
        if(lmd_base[3]==0x06EB86D9){
          if(lmd_base[4]==0xDB872570){
            if(lmd_base[5]==0xFCAFD945){
              if(lmd_base[6]==0xA7B8F647){
                if(lmd_base[7]==0x3D324C97){
                  if(lmd_base[8]==0xF3470C8E){
                    if(lmd_base[9]==0xC46D25C5){
                      if(lmd_base[10]==0x38BB8DDA){
                        if(lmd_base[11]==0x7A79DD88){
                          if(lmd_base[12]==0xE785DB95){
                            if(lmd_base[13]==0x9F47B9AE){
                              if(lmd_base[14]==0x19A49F85){
                                if(lmd_base[15]==0x7909B4A0){
                                  status=0;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  print_status(8,status);
  return 0;
}
