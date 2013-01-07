#include "sha_func.h"

void init(SHAContext * context){
    context->Message_Digest[0]=0x67452301;
    context->Message_Digest[1]=0xEFCDAB89;
    context->Message_Digest[2]=0x98BADCFE;
    context->Message_Digest[3]=0x10325476;
    context->Message_Digest[4]=0xC3D2E1F0;
}

void init_words(unsigned int *words){
    int i;
    for( i = 0; i < 80; i++)
        words[i] = (unsigned int)0;
}
unsigned int get_word(unsigned int word, unsigned char msgpart, int link){
    word = (unsigned int) (word | (unsigned int) (msgpart << ((3-link)*8)));
    return (unsigned int)word;
}
void init_words_msg(unsigned int *words, unsigned char *msg){
    int i;
    for( i = 0; i < 64; i++){
        words[i/4]=(unsigned int)get_word(words[i/4],msg[i],i%4);
    }
}

unsigned int circular_left_shift(unsigned int val, int shifts){
    int i;
    unsigned int firstbit,temp;
    for(i = 0; i < shifts; i++){
        firstbit = (unsigned int)((0x00000001 << 31) && val) >> 31;
        temp = (unsigned int) val << 1;
        //printf("Temp: %d",temp);
        //val = (unsigned int) temp;
        if(firstbit == 1){
            temp = (unsigned int) (temp || 0x00000001);
        }
    }
    val = (unsigned int) temp;
    return (unsigned int)val;
}

void init_words_remain(unsigned int *w){
    int t;
    unsigned int temp;
    for( t = 16; t < 80; t++){
        w[t] = (unsigned int)(w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]);
        if(t ==16)
            printf("spl: %d",w[t]);
        temp = (unsigned int)circular_left_shift(w[t],1);
        w[t] = (unsigned int) temp;
        if(t == 16)
            printf("spl1: %d",w[t]);
    }
}

void init_intermediate_hash(unsigned *abcdef, SHAContext *con){
    int i;
    for( i = 0; i < 5; i++)
        abcdef[i] = con->Message_Digest[i];
}

char setbit(char val,int index){
    unsigned char temp=1;
    val=-128;
    val = val | (temp << index);
    return val;
}

void calc_SHA(int *abcdef, int *words, SHAContext *con){
    int i,inhash[5];
    unsigned f = 0x00000000;
    unsigned k = 0x00000000;
    unsigned temp = 0x00000000;
    for(i = 0; i < 5; i++)
        inhash[i] = abcdef[i];
    for( i = 0; i < 80; i++){
        if(i < 20){
            f = (inhash[3] ^ (inhash[1] & (inhash[2] ^ inhash[3])));
            k = 0x5A827999;
        }
        else{
            if(i >= 20 && i < 40){
                f = inhash[1] ^ inhash[2] ^ inhash[3];
                k = 0x6ED9EBA1;
            }
            else{
                if(i >= 40 && i < 60){
                    f = (inhash[1] & inhash[2]) | (inhash[1] & inhash[3]) | (inhash[2] & inhash[3]);
                    k = 0x8F1BBCDC;
                }
                else{
                    if(i >= 60 && i<80){
                        f = inhash[1] ^ inhash[2] ^ inhash[3];
                        k = 0xCA62C1D6;
                    }
                }
            }
        }
        temp = circular_left_shift(inhash[0],5) + f + inhash[4] + k + words[i];
        inhash[4] = inhash[3];
        inhash[3] = inhash[2];
        inhash[2] = circular_left_shift(inhash[1],30);
        inhash[1] = inhash[0];
        inhash[0] = temp;
    }

    for( i = 0; i < 5; i++)
        con->Message_Digest[i] += inhash[i];
}
void convert_int_char(int len, unsigned char *msglen){
    //int i;
    msglen[0] = (unsigned char) (len & 0x000000FF);
    msglen[1] = (unsigned char) (len & 0x0000FF00) >> 8;
    msglen[2] = (unsigned char) (len & 0x00FF0000) >> 16;
    msglen[3] = (unsigned char) (len & 0xFF000000) >> 24;
}
void display_hash(SHAContext *con){
    int i;
    printf("\n\nSHA Hash:\t");
    for( i = 0; i < 5; i++)
        printf("%X",(int)con->Message_Digest[i]);
    printf("\n");
}
